package com.example.albertreed.asuforiacppsupport;


import org.opencv.android.CameraBridgeViewBase;
import org.opencv.core.Mat;

public class asuforia {
    static {
        System.loadLibrary("MyOpencvLibs");
    }


    public native static int nativeOrb(long addrFrame,long ShowKeypoints1);

}




/*public asuforia(PoseListener listener, Image refImage, Surface cameraSurface) {


}

public void startEstimation(){
    onImageAvailable();
}

private void onImageAvailable(){

}

public void endEstimation() {}
*/