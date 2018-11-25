#include <com_example_albertreed_asuforiacppsupport_OpencvNativeClass.h>
#include <jni.h>
#include <android/bitmap.h>
#include "opencv2/opencv.hpp"
#include "opencv2/core.hpp"
#include <android/log.h>
#include "features.h"
#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include <iostream>
#include <fstream>
#include "opencv2/highgui/highgui.hpp"
#include <vector>
using namespace cv;
using namespace std;
int toGray(Mat img, Mat& gray) {
    cvtColor(img, gray, CV_RGBA2GRAY);
    return 1;
}
int Keypoints(Mat mOrb,vector<KeyPoint>  Keypoints1, Mat& ShowKeypoints1) {
    drawKeypoints(mOrb,Keypoints1,ShowKeypoints1);
    return 1;
}


extern "C" {

JNIEXPORT jint JNICALL Java_com_example_albertreed_asuforiacppsupport_OpencvNativeClass_convertGray
  (JNIEnv *, jclass, jlong addrRGBA, jlong addrGray){
    Mat& mRgb = * (Mat *) addrRGBA;
    Mat& mGray = * (Mat *) addrGray;

    int conv;
    jint retval;
    conv = toGray(mRgb, mGray);

    retval = (jint) conv;
    return retval;
}



JNIEXPORT jint JNICALL Java_com_example_albertreed_asuforiacppsupport_OpencvNativeClass_nativePoseEstimation
    (JNIEnv *, jclass, jlong addrFrame){
    Mat& frame = * (Mat *) addrFrame;
 }

JNIEXPORT jint JNICALL Java_com_example_albertreed_asuforiacppsupport_asuforia_nativeOrb
(JNIEnv *, jclass, jlong addrFrame,  jlong ShowKeypoints1) {
     Mat& mOrb =  * (Mat *) addrFrame;
     Mat& mGray = * (Mat *) ShowKeypoints1;
     int conv;
     jint retval;
     Ptr<ORB> orb = ORB::create();
     vector<KeyPoint> Keypoints1;
     Mat descriptors1;
     orb->detectAndCompute(mOrb, Mat(), Keypoints1, descriptors1);
     conv = Keypoints(mOrb,Keypoints1, mGray);
     retval = (jint) conv;


     return 1;
}



}



