//Use openCV to open camera and preview images

package com.example.albertreed.asuforiacppsupport;


import org.opencv.core.Mat;
import org.opencv.android.CameraBridgeViewBase;

import android.media.Image;
import android.os.Bundle;
import android.util.Log;
import android.view.MenuItem;
import android.view.Surface;
import android.view.WindowManager;
import android.support.v7.app.AppCompatActivity;


public class MainActivity extends AppCompatActivity  implements PoseListener {

    static {
        System.loadLibrary("MyOpencvLibs");
    }
    Mat mRgba;
    Mat mGray;

    private static final String TAG = "OCVSample::Activity";

    private CameraBridgeViewBase cv;
    private boolean isJavaCamera = true;
    private MenuItem swCamera = null;

    PoseListener listener;
    Image refImage;
    Surface surface;

    asuforia ARmanager;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        Log.i(TAG, "called onCreate");
        super.onCreate(savedInstanceState);
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);

        setContentView(R.layout.main_activity_surface_view);

        ARmanager = new asuforia(listener, refImage, surface );

        ARmanager.startEstimation();
    }

    public void onPose()
    {
        //do stuff
    }


    public void onDestroy() {
        super.onDestroy();
        ARmanager.endEstimation();
    }





}