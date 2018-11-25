#include <com_example_albertreed_asuforiacppsupport_OpencvNativeClass.h>

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

int toGray(Mat img, Mat& gray) {
    cvtColor(img, gray, CV_RGBA2GRAY);
    return 1;
}

JNIEXPORT jint JNICALL Java_com_example_albertreed_asuforiacppsupport_OpencvNativeClass_nativePoseEstimation
    (JNIEnv *, jclass, jlong addrFrame){
    Mat& frame = * (Mat *) addrFrame;
    return 0;



    }

JNIEXPORT jint JNICALL Java_com_example_albertreed_asuforiacppsupport_OpencvNativeClass_getReferencePoints
     (JNIEnv *, jclass, jlong addrFrame){
     Mat& mOrb =  * (Mat *) addrFrame;
     Ptr<ORB> orb = ORB::create();
     vector<KeyPoint> keypoints;
     Mat descriptors;
     orb->detectAndCompute(mOrb, Mat(), keypoints, descriptors);
     /*Now we need to keep going*/
     return 1;
}
