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

    private CameraBridgeViewBase cv;

    private boolean estimating = false;
    private boolean preview = false;
    public static boolean getRefImage = false;

    Mat frame;
    Mat refImage;
    Mat refImageGray = new Mat();
    Mat testFrame;



    public asuforia(CameraBridgeViewBase cv) {
        this.cv = cv;
        this.cv.setCvCameraViewListener(this);//this class contains the callback
        cv.enableView();
    }




    /*uses boolean value to turn onCameraFrame callback on*/
    public void startEstimation() {

        /*Save the keypoints of the reference image*/
        p("1");
        this.keypoints = OpencvNativeClass.getReferencePoints(this.refImage.getNativeObjAddr(), descriptors.getNativeObjAddr());
        this.kpMat.fromArray(keypoints);
        p("here");
        this.estimating = true;
    }

    public Mat onCameraFrame(CvCameraViewFrame inputFrame) {//same as onImageAvailable()
        if(this.estimating == true) {
            //p("estimating");
            frame = inputFrame.gray();
            OpencvNativeClass.nativePoseEstimation(frame.getNativeObjAddr(), descriptors.getNativeObjAddr(), keypoints);;
            return frame;
        }
        if(this.getRefImage == true) {
            //p("getRefImage");
            this.refImage = inputFrame.gray();
            this.getRefImage = false;
            startEstimation();
            return refImage;
        }
        //p("1");
        frame = inputFrame.rgba();
        //p("2");
        return frame;
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
    }

    public void onCameraViewStarted(int width, int height) {
        frame = new Mat(height, width, CvType.CV_8UC1);
        refImage = new Mat(height, width, CvType.CV_8UC1);

    }





    }
