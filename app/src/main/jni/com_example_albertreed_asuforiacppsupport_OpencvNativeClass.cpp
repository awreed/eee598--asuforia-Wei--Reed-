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
    (JNIEnv * env, jclass, jlong addrFrame, jlong descriptorMat, jobjectArray Javakeypoints){
    Mat& frame = * (Mat *) addrFrame;
    Mat& descriptors = * (Mat *) descriptorMat;
    jsize size = env->GetArrayLength(Javakeypoints);
    vector<KeyPoint> keypoints((int) size);

  //  for(unsigned int i = 0; i < (unsigned int) size; i++) {
  //      keypoints.push_back((jobject) env->GetObjectArrayElement(Javakeypoints, i));
  //  }

    return 0;
}

JNIEXPORT jobjectArray JNICALL Java_com_example_albertreed_asuforiacppsupport_OpencvNativeClass_getReferencePoints
     (JNIEnv * env, jclass, jlong addrFrame, jlong descriptorsAddr){
     Mat& mOrb =  * (Mat *) addrFrame;
     Mat& descriptors = * (Mat *) descriptorsAddr;
     Ptr<ORB> orb = ORB::create();
     vector<KeyPoint> keypoints;
     orb->detectAndCompute(mOrb, Mat(), keypoints, descriptors);

     jclass cls = env->FindClass("org/opencv/features2d/KeyPoint");

     jmethodID midInit = env->GetMethodID(cls, "<init>", "(FFFFFII)V");

     jobjectArray keyPointArray = env->NewObjectArray(keypoints.size(), cls, NULL);

     for(unsigned int i = 0; i < keypoints.size(); i++) {
        jobject newKeyPoint = env->NewObject(cls, midInit, keypoints[i].pt.x, keypoints[i].pt.y,
            keypoints[i].size, keypoints[i].angle, keypoints[i].response, keypoints[i].octave,
            keypoints[i].class_id);
        env->SetObjectArrayElement(keyPointArray, i, newKeyPoint);
      }
      return keyPointArray;
 }

   JNIEXPORT jobjectArray JNICALL Java_com_example_albertreed_asuforiacppsupport_OpencvNativeClass_testKeyPoints
         (JNIEnv * env, jclass, jlong addrFrame)
         {
         Mat& mOrb =  * (Mat *) addrFrame;
         Mat descriptors;

         Ptr<ORB> orb = ORB::create();
         vector<KeyPoint> keypoints;
         orb->detectAndCompute(mOrb, Mat(), keypoints, descriptors);
         //access the class from the java environment
         jclass cls = env->FindClass("org/opencv/core/KeyPoint");
         //call the constructor of the class (Floag, float, float, float, float, integer, integer)
         jmethodID midInit = env->GetMethodID(cls, "<init>", "(FFFFFII)V");
         jobjectArray keyPointArray = env->NewObjectArray(keypoints.size(), cls, NULL);

         for(unsigned int i = 0; i < keypoints.size(); i++) {
             jobject newKeyPoint = env->NewObject(cls, midInit, keypoints[i].pt.x, keypoints[i].pt.y,
                 keypoints[i].size, keypoints[i].angle, keypoints[i].response, keypoints[i].octave,
                 keypoints[i].class_id);
             env->SetObjectArrayElement(keyPointArray, i, newKeyPoint);
         }
         return keyPointArray;
  }




    /* Ptr<flann::IndexParams> indexParams = makePtr<flann::LshIndexParams>(6, 12, 1);
     Ptr<flann::SearchParams> searchParams = cv::makePtr<flann::SearchParams>(50);

     Ptr<DescriptorMatcher> matcher = FlannBasedMatcher::create();*/
     /*Now we need to keep going*/

