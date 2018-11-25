package com.example.albertreed.asuforiacppsupport;

import android.media.Image;
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
import org.opencv.core.Mat;
import org.opencv.android.CameraBridgeViewBase;
import org.opencv.android.CameraBridgeViewBase.CvCameraViewListener2;

public class asuforia extends AppCompatActivity implements CvCameraViewListener2 {
    PoseListener listenerCallback;
    Image refImage;
    Surface cameraSurface;

    private CameraBridgeViewBase cv;
    private boolean isJavaCamera = true;
    private MenuItem swCamera = null;
    private boolean startEstimation = false;

    private static final String TAG = "OCVSample::Activity";

    Mat frame;


    public asuforia(PoseListener listener, Image refImage, Surface cameraSurface) {
        this.listenerCallback = listener;
        this.refImage = refImage;
        this.cameraSurface = cameraSurface;
        Mat image = new Mat(100, 100, CvType.CV_8UC4);

        cv = (CameraBridgeViewBase) findViewById(R.id.main_activity_java_surface_view);

        cv.setVisibility(SurfaceView.VISIBLE);

        cv.setCvCameraViewListener(this);

        OpencvNativeClass.getReferencePoints(image.getNativeObjAddr());
    }
    /*uses boolean value to turn onCameraFrame callback on*/
    public void startEstimation() {
        this.startEstimation = true;
        cv.enableView();//this will call the onCameraViewStarted() callback
    }

    public Mat onCameraFrame(CvCameraViewFrame inputFrame) {//same as onImageAvailable()
        if(this.startEstimation) {
            frame = inputFrame.rgba();
            OpencvNativeClass.nativePoseEstimation(frame.getNativeObjAddr());
            listenerCallback.onPose();
            return frame;
        } else {
            return null;
        }
    }

    /*uses boolean value to turn onCameraFrame callback off*/
    public void endEstimation() {
        if(cv != null) {
            cv.disableView();//this will call the onCameraViewStopped() callback
        }
        this.startEstimation = false;
    }

    public void onCameraViewStopped() {
        frame.release();
    }

    public void onCameraViewStarted(int width, int height) {
        frame = new Mat(height, width, CvType.CV_8UC4);


    }


    }
