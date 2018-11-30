package com.example.albertreed.asuforiacppsupport;

import android.content.DialogInterface;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.media.Image;
import android.nfc.Tag;
import android.view.MenuItem;
import android.view.MotionEvent;
import android.view.Surface;
import android.view.SurfaceView;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.SurfaceView;
import android.view.View;
import android.view.WindowManager;
import android.support.v7.app.AppCompatActivity;
import android.widget.FrameLayout;
import android.widget.RelativeLayout;
import android.widget.Toast;

import org.opencv.android.BaseLoaderCallback;
import org.opencv.android.CameraBridgeViewBase.CvCameraViewFrame;
import org.opencv.android.LoaderCallbackInterface;
import org.opencv.android.OpenCVLoader;
import org.opencv.core.CvType;
import org.opencv.core.KeyPoint;
import org.opencv.core.Mat;
import org.opencv.android.CameraBridgeViewBase;
import org.opencv.android.CameraBridgeViewBase.CvCameraViewListener2;
import org.opencv.core.MatOfKeyPoint;
import org.opencv.core.Point;
import org.opencv.core.Point3;
import org.opencv.core.Scalar;
import org.opencv.features2d.Features2d;
import org.opencv.android.Utils;
import org.opencv.imgcodecs.*;
import org.opencv.videoio.*;
import android.view.View;


import java.io.IOException;
import java.util.Vector;

public class asuforia extends AppCompatActivity implements CvCameraViewListener2 {

    public static void p(String s) {
        Log.d("MYPRINT", s);

    }

    /*Used to hold reference keypoints*/
    KeyPoint[] keypoints;
    /*Used to hold reference descriptors*/
    Mat descriptors = new Mat();
    /*necessary conversion*/
    MatOfKeyPoint kpMat = new MatOfKeyPoint();
    /*3D model from user*/
    Point3[] points = new Point3[8];
    /*Estimated 2D points to be returned to user*/
    Point[] points2D;

    private CameraBridgeViewBase cv;
    PoseListener listener;

    /*estimating between current frames and reference iamge*/
    private boolean estimating = false;
    /*preview until user takes picture of reference image by tapping screen*/
    private boolean preview = false;
    /*User tapped screen, set from callback in mainActivity*/
    public static boolean getRefImage = false;
    private boolean startWhenReady = false;
    private boolean stopped = false;

    Mat frame;
    Mat refImage = null;
    Mat outFrame;


    /*constructor*/
    public asuforia(CameraBridgeViewBase cv, Point3[] p) {
        /*Used to capture and process frames so need to save*/
        this.cv = cv;
        this.cv.setCvCameraViewListener(this);//this class contains the callback
        /*tell listener where the onPose function is*/
        this.listener = new MainActivity();
        /*copy over 3D model to our points array*/
        for(int i = 0; i < p.length; i++) {
            this.points[i] = new Point3();
            this.points[i] = p[i];
        }
        /*Turns the camera on and triggers the onCameraStarted() callback*/
        cv.enableView();
        this.preview = true;

    }




    /*uses boolean value to turn onCameraFrame callback on*/
    public void startEstimation() {
        if(this.refImage != null) {
            /*Find keypoints from the reference image and save them*/
            this.keypoints = OpencvNativeClass.getReferencePoints(this.refImage.getNativeObjAddr(), descriptors.getNativeObjAddr());
            this.kpMat.fromArray(keypoints);
            this.estimating = true;
        } else if(this.stopped == true) {
            cv.enableView();
            this.preview = true;
            this.startWhenReady = true;
            this.stopped = false;
        } else {
            /*Can't start estimating until user takes reference image*/
            this.startWhenReady = true;
        }

    }
    /*Same as Functionality as onImageAvailable()*/
    /*Callback when new frame is ready for processing*/
    public Mat onCameraFrame(CvCameraViewFrame inputFrame) {
        if(this.estimating == true && refImage != null) {/*running pose estimation*/
            //p("estimating");
            frame = inputFrame.gray();
            outFrame = inputFrame.rgba();
            /*calculate 2D points using pose estimation*/
            points2D = OpencvNativeClass.nativePoseEstimation(frame.getNativeObjAddr(), refImage.getNativeObjAddr(), outFrame.getNativeObjAddr(), descriptors.getNativeObjAddr(), keypoints, points);
            /*call the onPose callback if we have points to return to the user*/
            if(points2D != null) {
                listener.onPose(points2D, outFrame);
            }
            return outFrame;
        }
        /*user has touched the screen so we need to take a picture*/
        if(this.getRefImage == true) {
            this.refImage = inputFrame.gray();
            this.getRefImage = false;
            this.preview = false;
            if(this.startWhenReady == true) {
                startEstimation();
            }
            return refImage;
        }
        /*User is previewing the camera stream and needs to take picture of reference image*/
        if(this.preview == true) {
            this.frame = inputFrame.gray();
            return frame;
        }
        if(this.preview = false) {
            this.frame = inputFrame.gray();
            return null;
        }
        return null;//never happens
    }

    /*uses boolean value to turn onCameraFrame callback off*/
    public void endEstimation() {
        if(this.cv != null) {
            this.cv.disableView();//this will call the onCameraViewStopped() callback
        }
        this.estimating = false;
        this.stopped = true;
    }

    /*called when disbleView() is called*/
    public void onCameraViewStopped() {
        frame.release();
        refImage.release();
        outFrame.release();
    }

    /*intialize some matrices when we start the camera*/
    public void onCameraViewStarted(int width, int height) {
        frame = new Mat(height, width, CvType.CV_8UC1);
        refImage = new Mat(height, width, CvType.CV_8UC1);
        outFrame = new Mat(height, width, CvType.CV_8UC4);
    }





    }
