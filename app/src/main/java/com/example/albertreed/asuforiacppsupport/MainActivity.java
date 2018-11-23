//Use openCV to open camera and preview images

package com.example.albertreed.asuforiacppsupport;


import org.opencv.android.BaseLoaderCallback;
import org.opencv.android.CameraBridgeViewBase.CvCameraViewFrame;
import org.opencv.android.LoaderCallbackInterface;
import org.opencv.android.OpenCVLoader;
import org.opencv.core.CvType;
import org.opencv.core.Mat;
import org.opencv.android.CameraBridgeViewBase;
import org.opencv.android.CameraBridgeViewBase.CvCameraViewListener2;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.SurfaceView;
import android.view.WindowManager;
import android.support.v7.app.AppCompatActivity;
import android.widget.Toast;


public class MainActivity  extends AppCompatActivity  implements CvCameraViewListener2 {

    static {
        System.loadLibrary("MyOpencvLibs");
    }
    Mat mRgba;
    Mat mGray;

    private static final String TAG = "OCVSample::Activity";

    private CameraBridgeViewBase cv;
    private boolean isJavaCamera = true;
    private MenuItem swCamera = null;

    private BaseLoaderCallback mLoaderCallback = new BaseLoaderCallback(this) {
        @Override
        public void onManagerConnected(int status) {
            switch(status) {
                case LoaderCallbackInterface.SUCCESS:
                {
                    Log.i(TAG, "OpenCV loaded successfully");
                    cv.enableView();
                } break;
                default:
                {
                    super.onManagerConnected(status);
                } break;
            }
        }
    };

    public MainActivity() {Log.i(TAG, "Instantiated new " + this.getClass());}

    @Override
    public void onCreate(Bundle savedInstanceState) {
        Log.i(TAG, "called onCreate");
        super.onCreate(savedInstanceState);
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);

        setContentView(R.layout.main_activity_surface_view);

        cv = (CameraBridgeViewBase) findViewById(R.id.main_activity_java_surface_view);

        cv.setVisibility(SurfaceView.VISIBLE);

        cv.setCvCameraViewListener(this);
    }

    @Override
    public void onPause()
    {
        super.onPause();
        if(cv != null) {
            cv.disableView();
        }

    }

    @Override
    public void onResume()
    {
        super.onResume();
        if(!OpenCVLoader.initDebug()) {
            Log.d(TAG, "Internal OpenCV library not found. Using OpenCV Manager for initialization");
            OpenCVLoader.initAsync(OpenCVLoader.OPENCV_VERSION_3_0_0, this, mLoaderCallback);
        } else {
            Log.d(TAG, "OpenCV library found inside package. Using it!");
            mLoaderCallback.onManagerConnected(LoaderCallbackInterface.SUCCESS);
        }
    }

    public void onDestroy() {
        super.onDestroy();
        if(cv != null) {
            cv.disableView();
        }
    }

    public void onCameraViewStarted(int width, int height) {
        mRgba = new Mat(height, width, CvType.CV_8UC4);
        mGray = new Mat(height, width, CvType.CV_8UC1);

    }

    public void onCameraViewStopped() {
        mRgba.release();
        mGray.release();
    }

    public Mat onCameraFrame(CvCameraViewFrame inputFrame) {
        mRgba = inputFrame.rgba();
        OpencvNativeClass.convertGray(mRgba.getNativeObjAddr(), mGray.getNativeObjAddr());
        return mGray;
    }

   /* interface PoseListener() {
        void onPose() {

        }
    }*/

   // public static native int nativePoseEstimation();


}