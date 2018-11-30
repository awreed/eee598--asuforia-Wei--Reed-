#include <com_example_albertreed_asuforiacppsupport_OpencvNativeClass.h>

/*converts RBGA to grayscale, not uesd tho*/
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

/*not used*/
int toGray(Mat img, Mat& gray) {
    cvtColor(img, gray, CV_RGBA2GRAY);
    return 1;
}

/*not used*/
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


/*Method for filtering out matches*/
int ratioTest(std::vector<std::vector<cv::DMatch>> &matches) {
    int removed = 0;

    /*cycle over all matches*/
    for(std::vector<std::vector<cv::DMatch> >::iterator
        matchIterator = matches.begin(); matchIterator!=matches.end(); ++matchIterator)
    {
        /*perform the ratio test between matches*/
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
    /*return the number of removed matches*/
    return removed;
}

/*For converting java Point3 to vector<Point3f>*/
void jPoints2cPoints(jobjectArray jP, vector<Point3f>& model3D, JNIEnv * env) {
    int size = env->GetArrayLength(jP);/*length of java Point3 array*/
    jclass cls = env->FindClass("org/opencv/core/Point3");/*class identifier*/
    jfieldID x = env->GetFieldID(cls, "x", "D");/*grab x value*/
    jfieldID y = env->GetFieldID(cls, "y", "D");/*grab y value*/
    jfieldID z = env->GetFieldID(cls, "z", "D");/*grab z value*/
    for(int i = 0; i < size; i++) {/*now copy to vector*/
        jobject pt = env->GetObjectArrayElement(jP, i);
        model3D.push_back(Point3f(env->GetDoubleField(pt, x),env->GetDoubleField(pt, y),env->GetDoubleField(pt, z)));
    }
}

/*convert jobject array of keypoints to vector<keypoints>*/
void jArray2Vec(jobjectArray akp, vector<KeyPoint>& vkp, JNIEnv *env) {
        int size = env->GetArrayLength(akp);/*get size of array*/
        jclass cls = env->FindClass("org/opencv/core/KeyPoint");/*name of keypoint class*/
        jclass clsPoint = env->FindClass("org/opencv/core/Point");/*needed to grab point within
        keypoint class (Fancy pancy)*/

        jfieldID fidx = env->GetFieldID(clsPoint, "x", "D");/*used to grab x and y point values*/
        jfieldID fidy = env->GetFieldID(clsPoint, "y", "D");

        jfieldID fidpt = env->GetFieldID(cls, "pt", "Lorg/opencv/core/Point;");/*point identifier*/
        jfieldID fidsize = env->GetFieldID(cls, "size", "F");/*grab the keypoint values*/

        jfieldID fidangle = env->GetFieldID(cls, "angle", "F");

        jfieldID fidresponse = env->GetFieldID(cls, "response", "F");

        jfieldID fidoctave = env->GetFieldID(cls, "octave", "I");

        jfieldID fidclassid = env->GetFieldID(cls, "class_id", "I");


        for(int i = 0; i < 100; i++) {/*only save 100 keypoints so we dont run out of memory*/
            jobject cur_kp = env->GetObjectArrayElement(akp, i);/*get current keypoint from java array*/
            KeyPoint tmp_kp;
            jobject pt = env->GetObjectField(cur_kp, fidpt);/*copy all values to c++ keypoint type*/


            tmp_kp.pt.x = env->GetDoubleField(pt, fidx);
            tmp_kp.pt.y = env->GetDoubleField(pt, fidy);
            tmp_kp.size = env->GetFloatField(cur_kp, fidsize);
            tmp_kp.angle = env->GetFloatField(cur_kp, fidangle);
            tmp_kp.response = env->GetFloatField(cur_kp, fidresponse);
            tmp_kp.octave = env->GetIntField(cur_kp, fidoctave);
            tmp_kp.class_id = env->GetIntField(cur_kp, fidclassid);

            vkp.push_back(tmp_kp);/*put c++ keypoint into vector*/
        }

}

/*used to convert vector<Point2f> to java Point[] type*/
jobjectArray pvec2jarray(vector<Point2f>& p2D, JNIEnv *env) {
    jclass cls = env->FindClass("org/opencv/core/Point");/*get the class ID*/

    jmethodID midInit = env->GetMethodID(cls, "<init>", "(DD)V");/*find the constructor*/

    jobjectArray pointArray = env->NewObjectArray(p2D.size(), cls, NULL);/*create a new Point()*/

    for(int i = 0; i < p2D.size(); i++) {/*Initialize new points and copy in c++ data to java*/
        jobject p = env->NewObject(cls, midInit, p2D[i].x, p2D[i].y);
        env->SetObjectArrayElement(pointArray, i, p);
    }
    return pointArray;/*will pass this back to the user*/

}

/*used to convert vector<Keypoint> to java Keypoint[]*/
jobjectArray vec2jArray(vector<KeyPoint>& kp, JNIEnv *env) {
     jclass cls = env->FindClass("org/opencv/core/KeyPoint");/*Find the class*/
     jmethodID midInit = env->GetMethodID(cls, "<init>", "(FFFFFII)V");/*find constructor by id*/
     jobjectArray keyPointArray = env->NewObjectArray(kp.size(), cls, NULL);/*create new array*/
     for(unsigned int i = 0; i < kp.size(); i++) {/*copy in c++ data to new Keypoint()*/
        jobject newKeyPoint = env->NewObject(cls, midInit, kp[i].pt.x, kp[i].pt.y,
            kp[i].size, kp[i].angle, kp[i].response, kp[i].octave,
            kp[i].class_id);
        env->SetObjectArrayElement(keyPointArray, i, newKeyPoint);
     }
     return keyPointArray;
}

/*Was used to check if estimated 2D points fit on the screen, not very useful though*/
int checkPoints(vector<Point2f> &v) {
    for(int i = 0; i < v.size(); i++) {
        if(v[i].x < 0 || v[i].x > 1280 || v[i].y <0 || v[i].y > 720) {
            return 0;
        }
    }
    return 1;
}


/*This where all the magic happens. Except its broken so actually this is where crap happens.
Crap that took a very long time to code :)*/
JNIEXPORT jobjectArray JNICALL Java_com_example_albertreed_asuforiacppsupport_OpencvNativeClass_nativePoseEstimation
    (JNIEnv * env, jclass, jlong addrFrame, jlong refFrame, jlong outFrame, jlong descriptorMat, jobjectArray Javakeypoints, jobjectArray points3D){
     Mat Amat = Mat::zeros(3, 3, CV_64FC1);

     double f = 2.95;/*Camera intrinsics from the interwebs*/
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

     Mat distCoeffs = Mat::zeros(8, 1, CV_32FC1);/*Assume no camera distortion...*/

     Mat& frame = * (Mat *) addrFrame;/*convert Java Mats to C++ Mat*/
     Mat& refDescriptors = * (Mat *) descriptorMat;
     Mat& refImage = * (Mat *) refFrame;
     Mat& outImage = * (Mat *) outFrame;
     vector<Point3f> refMatches3D;/*Matches from reference image to be converted to 3D (z = 0)*/
     vector<KeyPoint> refMatches2D;/*Matches from reference image kept 2D*/
     vector<Point2f> frameMatches2D;/*Matches on new frame*/
     vector<KeyPoint> KpframeMatches2D;/*Matched keypoints from frame*/
     vector<vector<DMatch>> knnmatches;/*unfilterd matches*/
     vector<DMatch> matches;/*filtered matches*/
     vector<KeyPoint> frameKeypoints;/*non-matched keypoints from frame*/
     vector<Point3f> model3D;/*3D model that will be loaded from user defined Points*/
     vector<Point2f> model2D;/*Converted 3D to 2D model to be returned to user*/

     /*Convert Java Point3[] to vector<Point3f> WOW SO COOL!*/
     jPoints2cPoints(points3D, model3D, env);

     /*will hold reference keypoints*/
     vector<KeyPoint> refKeypoints;

     /*Convert Java Keypoints[] to vector<keypoint>*/
     jArray2Vec(Javakeypoints, refKeypoints, env);

     /*Create new feature extractor*/
     Ptr<ORB> orb = ORB::create();
     Mat frameDescriptors;
     /*Find keypoints and descriptors from new input frame*/
     orb->detectAndCompute(frame, Mat(), frameKeypoints, frameDescriptors);
     const float minRatio = 1.f/2.f;

     /*Create a new mather*/
     BFMatcher matcher(NORM_L2);
     if(frameKeypoints.size() > 4) {/*If we have some matches*/
         matcher.knnMatch(refDescriptors, frameDescriptors, knnmatches, 2);/*Match keypoints between
         our reference image and our new frame*/

         for(size_t i = 0; i < knnmatches.size(); i++) {/*filter our matches with the ratio test*/
            const DMatch& bestMatch = knnmatches[i][0];
            const DMatch& betterMatch = knnmatches[i][1];
            float distanceRatio = bestMatch.distance/betterMatch.distance;
            if(distanceRatio < minRatio) {
                matches.push_back(bestMatch);
            }
         }

         Mat rMat, rVec = Mat::zeros(3, 1, CV_64FC1);/*create new mat for rotation and translation*/
         Mat tvec = Mat::zeros(3, 1, CV_64FC1);

         jPoints2cPoints(points3D, model3D, env);/*convert user defined model
         of Point3[] to Vector<Point3f>*/


         for(unsigned int i = 0; i < matches.size(); ++i) {/*save the matched keypoints of the reference
            and new frame*/
            double x = refKeypoints[matches[i].queryIdx].pt.x;
            double y = refKeypoints[matches[i].queryIdx].pt.y;
            refMatches3D.push_back(Point3f(x, y, 0.0));/*Convert reference keypoints to 3D planar object*/
            refMatches2D.push_back(refKeypoints[i]);
            Point2f tmp;
            tmp.x = frameKeypoints[matches[i].trainIdx].pt.x;
            tmp.y = frameKeypoints[matches[i].trainIdx].pt.y;


            frameMatches2D.push_back(tmp);
            KpframeMatches2D.push_back(frameKeypoints[matches[i].trainIdx]);
         }
         drawKeypoints(frame, KpframeMatches2D, outImage);/*draw some keypoints WOW*/


         if(frameMatches2D.size() > 4) {
            solvePnP(refMatches3D, frameMatches2D, Amat, distCoeffs, rMat, tvec);/*solve pnp problem between
            our planar reference model and new frame, will populate rotation at and translation vec*/
            Rodrigues(rMat, rVec);/*convert rotation mat to vector for project points*/

            /*pass in model of 3D points, rotation vec, and translation vec to get back 2D model of points
            for the user to draw on the screen*/
            projectPoints(model3D, rVec, tvec, Amat, distCoeffs, model2D);


            /*Delete some vectors so we don't run out of memory*/
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

            /*Convert c++ vector<Point2f> to java Point[] array so user can draw points in MainActivity*/
            jobjectArray array = pvec2jarray(model2D, env);
            return array;
         }

         return NULL;
    }

    return NULL;


}

/*used to find keypoints and descriptors from the reference image*/
JNIEXPORT jobjectArray JNICALL Java_com_example_albertreed_asuforiacppsupport_OpencvNativeClass_getReferencePoints
     (JNIEnv * env, jclass, jlong addrFrame, jlong descriptorsAddr){
     Mat& mOrb =  * (Mat *) addrFrame;/*get Mats*/
     Mat& descriptors = * (Mat *) descriptorsAddr;
     Ptr<FeatureDetector> detector = ORB::create();/*New ORB detector*/
     Ptr<DescriptorExtractor> descriptor = ORB::create();

     vector<KeyPoint> keypoints;
     detector->detect(mOrb, keypoints);/*Find some reference image keypoints*/
     descriptor->compute(mOrb, keypoints, descriptors);/*Find some reference image descriptors*/
     //drawKeypoints(mOrb, keypoints, tf);
     //LOGD("keypoints size is %d\n", keypoints.size());
     jobjectArray keyPointArray = vec2jArray(keypoints, env);/*convert vector<keypoints> to java
     Keypoint[] so we can save keypoints in the java environment*/

     return keyPointArray;
 }





