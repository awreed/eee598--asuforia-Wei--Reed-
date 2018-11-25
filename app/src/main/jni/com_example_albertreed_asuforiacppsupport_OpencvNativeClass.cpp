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

/*void symmetryTest(const std::vector<std::vector<cv::DMatch> > & matches1,
                    const std::vector<std::vector<cv::DMatch> > & matches2,
                    std::vector<cv::DMatch>& symMatches)
{
    for(vector<vector<DMatch> >::const_iterator
        matchIterator1 = matches1.begin(); matchIterator1 != matches1.end(); ++matchIterator1)
    {
        if(matchIterator1->empty() || matchIterator1->size() < 2) {
            continue;
        }
        for(vector<vector<DMatch> >::const_iterator
            matchIterator2 = matches2.begin(); matchIterator2 != matches2.end(); ++matchIterator2)
        {
            if(matchIterator2->empty() || matchIterator2->size() < 2) {
                continue;
            }
            if((*matchIterator1)[0].queryIdx ==
                (*matchIterator2)[0].trainIdx &&
                (*matchITerator2)[0].queryIdx ==
                (*matchIterator1)[0].trainIdx)
            {
                symMatches.push_back(
                    DMatch((*matchIterator1[0].queryIdx,
                            (*matchIterator)[0].trainIdx,
                            (*matchIterator1)[0].distance));
                break;
            }
        }
    }


 }*/

int ratioTest(std::vector<std::vector<cv::DMatch>> &matches) {
    int removed = 0;

    for(std::vector<std::vector<cv::DMatch> >::iterator
        matchIterator = matches.begin(); matchIterator!=matches.end(); ++matchIterator)
    {
        if(matchIterator->size() > 1) {
            if((*matchIterator)[0].distance/(*matchIterator)[1].distance > 0.7) {
                matchIterator->clear();
                removed++;
            }
        } else {
            matchIterator->clear();
            removed++;
        }
    }
    return removed;
}

void jArray2Vec(jobjectArray akp, vector<KeyPoint>& vkp, JNIEnv *env) {

        int size = env->GetArrayLength(akp);

        jclass cls = env->FindClass("org/opnecv/features2d/KeyPoint");
        jmethodID midInit = env->GetMethodID(cls, "<init>", "(FFFFFII)V");
        jfieldID fidx = env->GetFieldID(cls, "pt.x", "F");
        jfieldID fidy = env->GetFieldID(cls, "pt.y", "F");
        jfieldID fidsize = env->GetFieldID(cls, "size", "F");
        jfieldID fidangle = env->GetFieldID(cls, "angle", "F");
        jfieldID fidresponse = env->GetFieldID(cls, "response", "F");
        jfieldID fidoctave = env->GetFieldID(cls, "octave", "I");
        jfieldID fidclassid = env->GetFieldID(cls, "class_id", "I");


        for(int i = 0; i < size; i++) {
            jobject cur_kp = env->GetObjectArrayElement(akp, i);

            KeyPoint tmp_kp;

            tmp_kp.pt.x = env->GetFloatField(cur_kp, fidx);
            tmp_kp.pt.y = env->GetFloatField(cur_kp, fidy);
            tmp_kp.size = env->GetFloatField(cur_kp, fidsize);
            tmp_kp.angle = env->GetFloatField(cur_kp, fidangle);
            tmp_kp.response = env->GetFloatField(cur_kp, fidresponse);
            tmp_kp.octave = env->GetIntField(cur_kp, fidoctave);
            tmp_kp.class_id = env->GetIntField(cur_kp, fidclassid);

            vkp.push_back(tmp_kp);
        }
}

jobjectArray vec2jArray(vector<KeyPoint>& kp, JNIEnv *env) {
     jclass cls = env->FindClass("org/opencv/features2d/KeyPoint");

     jmethodID midInit = env->GetMethodID(cls, "<init>", "(FFFFFII)V");

     jobjectArray keyPointArray = env->NewObjectArray(kp.size(), cls, NULL);

     for(unsigned int i = 0; i < kp.size(); i++) {
        jobject newKeyPoint = env->NewObject(cls, midInit, kp[i].pt.x, kp[i].pt.y,
            kp[i].size, kp[i].angle, kp[i].response, kp[i].octave,
            kp[i].class_id);
        env->SetObjectArrayElement(keyPointArray, i, newKeyPoint);
     }
     return keyPointArray;
}




JNIEXPORT jint JNICALL Java_com_example_albertreed_asuforiacppsupport_OpencvNativeClass_nativePoseEstimation
    (JNIEnv * env, jclass, jlong addrFrame, jlong descriptorMat, jobjectArray Javakeypoints){
    Mat& frame = * (Mat *) addrFrame;
    Mat& refDescriptors = * (Mat *) descriptorMat;
    int size = env->GetArrayLength(Javakeypoints);
    vector<KeyPoint> keypoints(size);

    jArray2Vec(Javakeypoints, keypoints, env);

    Ptr<flann::IndexParams> indexParams = makePtr<flann::LshIndexParams>(6, 12, 1);
    Ptr<flann::SearchParams> searchParams = cv::makePtr<flann::SearchParams>(50);

    Ptr<DescriptorMatcher> matcher = FlannBasedMatcher::create();

    Ptr<ORB> orb = ORB::create();
    vector<KeyPoint> frameKeypoints;
    Mat newDescriptors;
    orb->detectAndCompute(frame, Mat(), frameKeypoints, newDescriptors);

    std::vector<std::vector<cv::DMatch> > matches12, matches21;

    matcher->knnMatch(newDescriptors, refDescriptors, matches12, 2);
    matcher->knnMatch(refDescriptors, newDescriptors, matches21, 2);



   // symmetryTest(matches12, matches21, good_matches);




    return 0;
}



JNIEXPORT jobjectArray JNICALL Java_com_example_albertreed_asuforiacppsupport_OpencvNativeClass_getReferencePoints
     (JNIEnv * env, jclass, jlong addrFrame, jlong descriptorsAddr){
     Mat& mOrb =  * (Mat *) addrFrame;
     Mat& descriptors = * (Mat *) descriptorsAddr;
     Ptr<ORB> orb = ORB::create();
     vector<KeyPoint> keypoints;
     orb->detectAndCompute(mOrb, Mat(), keypoints, descriptors);

     jobjectArray keyPointArray = vec2jArray(keypoints, env);
     return keyPointArray;
 }





