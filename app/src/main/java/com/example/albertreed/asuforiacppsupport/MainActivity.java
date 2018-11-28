//Use openCV to open camera and preview images

package com.example.albertreed.asuforiacppsupport;


import org.opencv.android.Utils;
import org.opencv.core.Mat;
import org.opencv.android.CameraBridgeViewBase;
import org.opencv.android.BaseLoaderCallback;
import org.opencv.android.CameraBridgeViewBase.CvCameraViewFrame;
import org.opencv.android.LoaderCallbackInterface;
import org.opencv.android.OpenCVLoader;
import org.opencv.core.CvType;
import org.opencv.core.Mat;
import org.opencv.android.CameraBridgeViewBase;
import org.opencv.android.CameraBridgeViewBase.CvCameraViewListener2;
import org.opencv.photo.Photo;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.media.Image;
import android.os.Bundle;
import android.util.Log;
import android.view.MenuItem;
import android.view.Surface;
import android.view.SurfaceView;
import android.view.View;
import android.view.WindowManager;
import android.support.v7.app.AppCompatActivity;
import android.widget.FrameLayout;

import static com.example.albertreed.asuforiacppsupport.asuforia.p;


public class MainActivity extends AppCompatActivity  implements PoseListener {

    static {
        System.loadLibrary("MyOpencvLibs");
    }
    Mat mRgba;
    Mat mGray;
    Mat refImage = null;



    private BaseLoaderCallback mLoaderCallback = new BaseLoaderCallback(this) {
        @Override
        public void onManagerConnected(int status) {
            switch(status) {
                case LoaderCallbackInterface.SUCCESS:
                {

                } break;
                default:
                {
                    super.onManagerConnected(status);
                } break;
            }
        }
    };



    private CameraBridgeViewBase cv;


    asuforia ARmanager;

    @Override
    public void onCreate(Bundle savedInstanceState) {

        super.onCreate(savedInstanceState);
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
        setContentView(R.layout.main_activity_surface_view);

        cv = (CameraBridgeViewBase) findViewById(R.id.main_activity_java_surface_view);

        cv.setVisibility(SurfaceView.VISIBLE);
        org.opencv.android.JavaCameraView jcv = (org.opencv.android.JavaCameraView) findViewById(R.id.main_activity_java_surface_view);
        jcv.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                asuforia.getRefImage = true;
            }
        });


        ARmanager = new asuforia(cv);



        //ARmanager.startEstimation();
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