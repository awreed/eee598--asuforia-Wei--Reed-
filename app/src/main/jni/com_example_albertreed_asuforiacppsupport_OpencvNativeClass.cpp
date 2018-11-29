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

void jPoints2cPoints(jobjectArray jP, vector<Point3f>& model3D, JNIEnv * env) {
    int size = env->GetArrayLength(jP);
    jclass cls = env->FindClass("org/opencv/core/Point3");
    jfieldID x = env->GetFieldID(cls, "x", "D");
    jfieldID y = env->GetFieldID(cls, "y", "D");
    jfieldID z = env->GetFieldID(cls, "z", "D");
    for(int i = 0; i < size; i++) {
        jobject pt = env->GetObjectArrayElement(jP, i);
        model3D.push_back(Point3f(env->GetDoubleField(pt, x),env->GetDoubleField(pt, y),env->GetDoubleField(pt, z)));
    }
}

void jArray2Vec(jobjectArray akp, vector<KeyPoint>& vkp, JNIEnv *env) {

      //  LOGD("j1");

        int size = env->GetArrayLength(akp);
     //   LOGD("j2");
        jclass cls = env->FindClass("org/opencv/core/KeyPoint");
        jclass clsPoint = env->FindClass("org/opencv/core/Point");


        jfieldID fidx = env->GetFieldID(clsPoint, "x", "D");
        jfieldID fidy = env->GetFieldID(clsPoint, "y", "D");

        jfieldID fidpt = env->GetFieldID(cls, "pt", "Lorg/opencv/core/Point;");
        jfieldID fidsize = env->GetFieldID(cls, "size", "F");

        jfieldID fidangle = env->GetFieldID(cls, "angle", "F");

        jfieldID fidresponse = env->GetFieldID(cls, "response", "F");

        jfieldID fidoctave = env->GetFieldID(cls, "octave", "I");

        jfieldID fidclassid = env->GetFieldID(cls, "class_id", "I");
       // LOGD("j3");


        for(int i = 0; i < 100; i++) {
            //LOGD("i is %d\n", i);
        //    LOGD("i is %d\n", i);

            jobject cur_kp = env->GetObjectArrayElement(akp, i);
            //LOGD("j5");
            KeyPoint tmp_kp;
            //LOGD("j6");
            jobject pt = env->GetObjectField(cur_kp, fidpt);

            //LOGD("j61");

            tmp_kp.pt.x = env->GetDoubleField(pt, fidx);
            //LOGD("j62");
            tmp_kp.pt.y = env->GetDoubleField(pt, fidy);
            //LOGD("j7");
            tmp_kp.size = env->GetFloatField(cur_kp, fidsize);
            tmp_kp.angle = env->GetFloatField(cur_kp, fidangle);
            tmp_kp.response = env->GetFloatField(cur_kp, fidresponse);
            tmp_kp.octave = env->GetIntField(cur_kp, fidoctave);
            tmp_kp.class_id = env->GetIntField(cur_kp, fidclassid);

            vkp.push_back(tmp_kp);
        }

}

jobjectArray pvec2jarray(vector<Point2f>& p2D, JNIEnv *env) {
    jclass cls = env->FindClass("org/opencv/core/Point");

    jmethodID midInit = env->GetMethodID(cls, "<init>", "(DD)V");

    jobjectArray pointArray = env->NewObjectArray(p2D.size(), cls, NULL);

    for(int i = 0; i < p2D.size(); i++) {
        jobject p = env->NewObject(cls, midInit, p2D[i].x, p2D[i].y);
        env->SetObjectArrayElement(pointArray, i, p);
    }
    return pointArray;

}



jobjectArray vec2jArray(vector<KeyPoint>& kp, JNIEnv *env) {
     jclass cls = env->FindClass("org/opencv/core/KeyPoint");
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

int checkPoints(vector<Point2f> &v) {
    for(int i = 0; i < v.size(); i++) {
        if(v[i].x < 0 || v[i].x > 1280 || v[i].y <0 || v[i].y > 720) {
            return 0;
        }
    }
    return 1;
}



JNIEXPORT jobjectArray JNICALL Java_com_example_albertreed_asuforiacppsupport_OpencvNativeClass_nativePoseEstimation
    (JNIEnv * env, jclass, jlong addrFrame, jlong refFrame, jlong outFrame, jlong descriptorMat, jobjectArray Javakeypoints, jobjectArray points3D){
     Mat Amat = Mat::zeros(3, 3, CV_64FC1);

     double f = 2.95;
     double sx = 3.67, sy = 2.74;
     double width = 1280, height = 720;
     double params[] = {width * f/sx,
                        height * f/sy,
                        width/2,
                        height/2};
     Amat.at<double>(0,0) = params[0];
     Amat.at<double>(1,1) = params[1];
     Amat.at<double>(0,2) = params[2];
     Amat.at<double>(1,2) = params[3];
     Amat.at<double>(2,2) = 1;

     Mat distCoeffs = Mat::zeros(8, 1, CV_32FC1);

     Mat& frame = * (Mat *) addrFrame;
     Mat& refDescriptors = * (Mat *) descriptorMat;
     Mat& refImage = * (Mat *) refFrame;
     Mat& outImage = * (Mat *) outFrame;
     vector<Point3f> refMatches3D;
     vector<KeyPoint> refMatches2D;
     vector<Point2f> frameMatches2D;
     vector<KeyPoint> KpframeMatches2D;
     vector<vector<DMatch>> knnmatches;
     vector<DMatch> matches;
     vector<KeyPoint> frameKeypoints;
     vector<Point3f> model3D;
     vector<Point2f> model2D;

     jPoints2cPoints(points3D, model3D, env);


     vector<KeyPoint> refKeypoints;


     jArray2Vec(Javakeypoints, refKeypoints, env);

     Ptr<ORB> orb = ORB::create();
     Mat frameDescriptors;
     orb->detectAndCompute(frame, Mat(), frameKeypoints, frameDescriptors);
     LOGD("3");
     const float minRatio = 1.f/2.f;



     BFMatcher matcher(NORM_L2);
     if(frameKeypoints.size() > 4) {
         matcher.knnMatch(refDescriptors, frameDescriptors, knnmatches, 2);

         for(size_t i = 0; i < knnmatches.size(); i++) {
            const DMatch& bestMatch = knnmatches[i][0];
            const DMatch& betterMatch = knnmatches[i][1];
            float distanceRatio = bestMatch.distance/betterMatch.distance;
            if(distanceRatio < minRatio) {
                matches.push_back(bestMatch);
            }
         }

         Mat rMat, rVec = Mat::zeros(3, 1, CV_64FC1);
         Mat tvec = Mat::zeros(3, 1, CV_64FC1);

         jPoints2cPoints(points3D, model3D, env);


         for(unsigned int i = 0; i < matches.size(); ++i) {
            double x = refKeypoints[matches[i].queryIdx].pt.x;
            double y = refKeypoints[matches[i].queryIdx].pt.y;
            refMatches3D.push_back(Point3f(x, y, 0.0));
            refMatches2D.push_back(refKeypoints[i]);
            Point2f tmp;
            tmp.x = frameKeypoints[matches[i].trainIdx].pt.x;
            tmp.y = frameKeypoints[matches[i].trainIdx].pt.y;


            frameMatches2D.push_back(tmp);
            KpframeMatches2D.push_back(frameKeypoints[matches[i].trainIdx]);
         }
         drawKeypoints(frame, KpframeMatches2D, outImage);


         if(frameMatches2D.size() > 4) {
            solvePnP(refMatches3D, frameMatches2D, Amat, distCoeffs, rMat, tvec);
            Rodrigues(rMat, rVec);

            projectPoints(model3D, rVec, tvec, Amat, distCoeffs, model2D);

            if(frameKeypoints.size() > 0) {
              frameKeypoints.clear();
            }
            if(refMatches3D.size() > 0) {
              refMatches3D.clear();
            }
            if(refMatches2D.size() > 0) {
              refMatches2D.clear();
            }
            if(frameMatches2D.size() > 0) {
              frameMatches2D.clear();
            }
            if(KpframeMatches2D.size() > 0) {
              KpframeMatches2D.clear();
            }
            if(matches.size() > 0) {
              matches.clear();
            }

            jobjectArray array = pvec2jarray(model2D, env);
            return array;
         }

         return NULL;
    }

    return NULL;


}



JNIEXPORT jobjectArray JNICALL Java_com_example_albertreed_asuforiacppsupport_OpencvNativeClass_getReferencePoints
     (JNIEnv * env, jclass, jlong addrFrame, jlong descriptorsAddr){
     Mat& mOrb =  * (Mat *) addrFrame;
     Mat& descriptors = * (Mat *) descriptorsAddr;
     Ptr<FeatureDetector> detector = ORB::create();
     Ptr<DescriptorExtractor> descriptor = ORB::create();

     vector<KeyPoint> keypoints;
     detector->detect(mOrb, keypoints);
     descriptor->compute(mOrb, keypoints, descriptors);
     //drawKeypoints(mOrb, keypoints, tf);
     //LOGD("keypoints size is %d\n", keypoints.size());
     jobjectArray keyPointArray = vec2jArray(keypoints, env);

     return keyPointArray;
 }





