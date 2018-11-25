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

    Mat frame;


    public asuforia(CameraBridgeViewBase cv) {
        this.cv = cv;
        this.cv.setCvCameraViewListener(this);

        //this.keypoints = OpencvNativeClass.getReferencePoints(image.getNativeObjAddr(), descriptors.getNativeObjAddr());
        //this.kpMat.fromArray(keypoints);

    }
    /*uses boolean value to turn onCameraFrame callback on*/
    public void startEstimation() {
        this.startEstimation = true;
        this.cv.enableView();//this will call the onCameraViewStarted() callback
    }

    public Mat onCameraFrame(CvCameraViewFrame inputFrame) {//same as onImageAvailable()
        if(this.startEstimation) {
            frame = inputFrame.gray();
            this.keypoints = OpencvNativeClass.testKeyPoints(frame.getNativeObjAddr());
            this.kpMat.fromArray(this.keypoints);
            //OpencvNativeClass.nativePoseEstimation(frame.getNativeObjAddr(), descriptors.getNativeObjAddr(), keypoints);
            //listenerCallback.onPose();
            Mat outputImage = new Mat();
            Scalar color = new Scalar(0, 0, 255);
            int flags = Features2d.DRAW_RICH_KEYPOINTS;
            Features2d.drawKeypoints(frame, this.kpMat, outputImage, color, flags);
            return outputImage;

        } else {
            return null;
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
