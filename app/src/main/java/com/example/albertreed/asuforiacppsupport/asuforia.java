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

    KeyPoint[] keypoints;
    Mat descriptors = new Mat();
    MatOfKeyPoint kpMat = new MatOfKeyPoint();
    Point3[] points = new Point3[8];
    Point[] points2D;

    private CameraBridgeViewBase cv;
    PoseListener listener;

    private boolean estimating = false;
    private boolean preview = false;
    public static boolean getRefImage = false;
    private boolean startWhenReady = false;

    Mat frame;
    Mat refImage = null;
    Mat outFrame;

    Mat rVec, tVec;



    public asuforia(CameraBridgeViewBase cv, Point3[] p) {
        this.cv = cv;
        this.cv.setCvCameraViewListener(this);//this class contains the callback
        this.listener = new MainActivity();

        for(int i = 0; i < p.length; i++) {
            this.points[i] = new Point3();
            //p("a");
            this.points[i] = p[i];
            p(Double.toString(this.points[i].x));
            //p("b");
        }
        cv.enableView();
        this.preview = true;

    }




    /*uses boolean value to turn onCameraFrame callback on*/
    public void startEstimation() {
        if(this.refImage != null) {
            this.keypoints = OpencvNativeClass.getReferencePoints(this.refImage.getNativeObjAddr(), descriptors.getNativeObjAddr());
            this.kpMat.fromArray(keypoints);
            this.estimating = true;
        } else {
            this.startWhenReady = true;
        }
    }

    public Mat onCameraFrame(CvCameraViewFrame inputFrame) {//same as onImageAvailable()
        if(this.estimating == true && refImage != null) {
            //p("estimating");
            frame = inputFrame.gray();
            outFrame = inputFrame.rgba();
            points2D = OpencvNativeClass.nativePoseEstimation(frame.getNativeObjAddr(), refImage.getNativeObjAddr(), outFrame.getNativeObjAddr(), descriptors.getNativeObjAddr(), keypoints, points);
            if(points2D != null) {
                listener.onPose(points2D, outFrame);
            }
            return outFrame;
        }
        if(this.getRefImage == true) {
            this.refImage = inputFrame.gray();
            this.getRefImage = false;
            this.preview = false;
            if(this.startWhenReady == true) {
                startEstimation();
            }
            return refImage;
        }
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
    }

    public void onCameraViewStopped() {
        frame.release();
        refImage.release();
        outFrame.release();
    }

    public void onCameraViewStarted(int width, int height) {
        frame = new Mat(height, width, CvType.CV_8UC1);
        refImage = new Mat(height, width, CvType.CV_8UC1);
        outFrame = new Mat(height, width, CvType.CV_8UC4);
    }





    }
