package com.example.albertreed.asuforiacppsupport;

import android.media.Image;
import android.nfc.Tag;
import android.view.MenuItem;
import android.view.Surface;
import android.view.SurfaceView;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.SurfaceView;
import android.view.WindowManager;
import android.support.v7.app.AppCompatActivity;
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

import java.util.Vector;

public class asuforia extends AppCompatActivity implements CvCameraViewListener2 {

    public static void p(String s) {
        Log.d("FUCK", s);

    }

    KeyPoint[] keypoints;
    Mat descriptors;
    MatOfKeyPoint kpMat = new MatOfKeyPoint();

    private CameraBridgeViewBase cv;

    private boolean startEstimation = false;
    private boolean getRefImage = false;

    Mat frame;
    Mat refImage;


    public asuforia(CameraBridgeViewBase cv) {
        this.cv = cv;
        this.cv.setCvCameraViewListener(this);//this class contains the callback
        this.getRefImage = true;
    }


    /*uses boolean value to turn onCameraFrame callback on*/
    public void startEstimation() {
        this.startEstimation = true;

        this.cv.enableView();//this will call the onCameraViewStarted() callback

        /*Save the keypoints of the reference image*/
        this.keypoints = OpencvNativeClass.getReferencePoints(refImage.getNativeObjAddr(), descriptors.getNativeObjAddr());
        this.kpMat.fromArray(keypoints);
    }

    public Mat onCameraFrame(CvCameraViewFrame inputFrame) {//same as onImageAvailable()
        if(this.startEstimation == true) {
            frame = inputFrame.gray();
            OpencvNativeClass.nativePoseEstimation(frame.getNativeObjAddr(), descriptors.getNativeObjAddr(), keypoints);;
            return frame;
        } else if(this.getRefImage == true){
            refImage = inputFrame.gray();
            this.getRefImage = false;
            startEstimation();//can start estimation now that we have reference image
            return refImage;
        } else {
            return null;//impossible
        }
    }

    /*uses boolean value to turn onCameraFrame callback off*/
    public void endEstimation() {
        if(this.cv != null) {
            this.cv.disableView();//this will call the onCameraViewStopped() callback
        }
        this.startEstimation = false;
    }

    public void onCameraViewStopped() {
        frame.release();
    }

    public void onCameraViewStarted(int width, int height) {
        frame = new Mat(height, width, CvType.CV_8UC1);


    }


    }
