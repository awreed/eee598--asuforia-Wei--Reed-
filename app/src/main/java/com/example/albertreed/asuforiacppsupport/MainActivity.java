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
import org.opencv.core.CvType;
import org.opencv.android.CameraBridgeViewBase;
import org.opencv.android.CameraBridgeViewBase.CvCameraViewListener2;
import org.opencv.core.Point;
import org.opencv.core.Point3;
import org.opencv.core.Scalar;
import org.opencv.imgproc.Imgproc;
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

    /*Responsible for syncing OpenCV libraries, ideally should be in Asuforia class but it segfaults*/
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

    /*Points to draw a 3D cube*/
    Point3[] model = new Point3[8];
    double[] p1 = {0, 0, 0};
    double[] p2 = {100, 0, 0};
    double[] p3 = {0, 100, 0};
    double[] p4 = {100, 100, 0};
    double[] p5 = {0, 0, 100};
    double[] p6 = {100, 0, 100};
    double[] p7 = {0, 100, 100};
    double[] p8 = {100, 100, 100};

    /*2D points corresponding to 3D points*/
    Point[] model2D = new Point[16];



    @Override
    public void onCreate(Bundle savedInstanceState) {

        super.onCreate(savedInstanceState);
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
        setContentView(R.layout.main_activity_surface_view);

        cv = (CameraBridgeViewBase) findViewById(R.id.main_activity_java_surface_view);

        cv.setVisibility(SurfaceView.VISIBLE);

        /*set up click listener so user can take picture by tapping screen*/
        org.opencv.android.JavaCameraView jcv = (org.opencv.android.JavaCameraView) findViewById(R.id.main_activity_java_surface_view);
        jcv.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                asuforia.getRefImage = true;
            }
        });
        /*initialize arrays*/
        for(int i = 0; i < model.length; i++) {
            model[i] = new Point3();
        }
        for(int i = 0; i < model2D.length; i++) {
            model2D[i] = new Point();
        }
        this.model[0].set(this.p1);
        this.model[1].set(this.p2);
        this.model[2].set(this.p3);
        this.model[3].set(this.p4);
        this.model[4].set(this.p5);
        this.model[5].set(this.p6);
        this.model[6].set(this.p7);
        this.model[7].set(this.p8);

        /*start asuforia library by passing in cameraView and 3D model*/
        ARmanager = new asuforia(cv, model);


        /*Begin estimtation*/
        ARmanager.startEstimation();
    }
    /*callback that returns 2D points for user to draw on frame, points are pose correct*/
    @Override
    public void onPose(Point[] p, Mat frame)
    {
        /*draw 2D points on the screen (This draws a cube)*/
        Imgproc.line(frame, p[0], p[1], new Scalar(255,0,0), 2 );
        Imgproc.line(frame, p[0], p[2], new Scalar(0,255,0), 2 );
        Imgproc.line(frame, p[2], p[3], new Scalar(0,0,255), 2 );
        Imgproc.line(frame, p[3], p[1], new Scalar(255,0,0), 2 );

        Imgproc.line(frame, p[4], p[5], new Scalar(0,255,0), 2 );
        Imgproc.line(frame, p[4], p[6], new Scalar(0,0,255), 2 );
        Imgproc.line(frame, p[6], p[7], new Scalar(255,0,0), 2 );
        Imgproc.line(frame, p[7], p[5], new Scalar(0,255,0), 2 );

        Imgproc.line(frame, p[2], p[6], new Scalar(0,0,255), 2 );
        Imgproc.line(frame, p[3], p[7], new Scalar(0,0,255), 2 );
        Imgproc.line(frame, p[1], p[5], new Scalar(0,0,255), 2 );
        Imgproc.line(frame, p[0], p[4], new Scalar(0,0,255), 2 );
        //do stuff
    }


    public void onDestroy() {
        super.onDestroy();
        ARmanager.endEstimation();
    }





}