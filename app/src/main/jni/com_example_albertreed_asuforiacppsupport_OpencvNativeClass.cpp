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




JNIEXPORT jint JNICALL Java_com_example_albertreed_asuforiacppsupport_OpencvNativeClass_nativePoseEstimation
    (JNIEnv * env, jclass, jlong addrFrame, jlong descriptorMat, jobjectArray Javakeypoints){

       LOGD("a");
       double f = 55;                           // focal length in mm
       double sx = 22.3, sy = 14.9;             // sensor size
       double width = 1280, height = 720;        // image size
       double params[] = { width*f/sx,   // fx
                              height*f/sy,  // fy
                              width/2,      // cx
                              height/2};    // cy
       int iterationsCount = 500;      // number of Ransac iterations.
       float reprojectionError = 2.0;  // maximum allowed distance to consider it an inlier.
       double confidence = 0.95;        // ransac successful confidence.
       int pnpMethod = SOLVEPNP_ITERATIVE;
       bool useExtrinsicGuess = false;
       Mat inliers_idx;
       Mat distCoeffs = Mat::zeros(4, 1, CV_64FC1);    // vector of distortion coefficients
       Mat rvec = Mat::zeros(3, 1, CV_64FC1);          // output rotation vector
       Mat tvec = Mat::zeros(3, 1, CV_64FC1);          // output translation vector
       Mat _A_matrix = Mat::zeros(3, 3, CV_64FC1);   // intrinsic camera parameters
       _A_matrix.at<double>(0, 0) = params[0];       //      [ fx   0  cx ]
       _A_matrix.at<double>(1, 1) = params[1];       //      [  0  fy  cy ]
       _A_matrix.at<double>(0, 2) = params[2];       //      [  0   0   1 ]
       _A_matrix.at<double>(1, 2) = params[3];
       _A_matrix.at<double>(2, 2) = 1;
       LOGD("b");
   // pnp ///

       Mat& frame = * (Mat *) addrFrame;
       //LOGD("b1");
       Mat& refDescriptors = * (Mat *) descriptorMat;
       //LOGD("b2");
       int size = env->GetArrayLength(Javakeypoints);
       LOGD("ba");
       vector<KeyPoint> keypoints(size);
       LOGD("ba1");
       jArray2Vec(Javakeypoints, keypoints, env);
       LOGD("ba2");
       Ptr<ORB> orb = ORB::create();
       LOGD("baa");
       vector<KeyPoint> frameKeypoints;
       Mat newDescriptors;
       orb->detectAndCompute(frame, Mat(), frameKeypoints, newDescriptors);
       LOGD("b3");
       BFMatcher matcher(NORM_L2);
       vector<DMatch> matches;
       matcher.match(refDescriptors, newDescriptors, matches);
       LOGD("b4");
       vector<Point2f> list_points3d_model_match; // container for the model 3D coordinates found in the scene
       vector<Point2f> list_points2d_scene_match; // container for the model 2D coordinates found in the scene
       vector<Point2f> imageFramePoints;
   	vector<Point3d> framePoints;
       LOGD("C");
       LOGD("matches size is %lu\n", matches.size());
       for(unsigned int match_index = 0; match_index < matches.size(); ++match_index)
           {
             Point2f point3d_model = keypoints[ matches[match_index].queryIdx ].pt;  // 3D point from model
             Point2f point2d_scene = frameKeypoints[ matches[match_index].queryIdx ].pt; // 2D point from the scene
             list_points3d_model_match.push_back(point3d_model);         // add 3D point
             list_points2d_scene_match.push_back(point2d_scene);         // add 2D point
           }

       if(matches.size() >= 4) // OpenCV requires solvePnPRANSAC to minimally have 4 set of points
           {
             // Estimate the pose using RANSAC approach
            solvePnPRansac( list_points3d_model_match, list_points2d_scene_match, _A_matrix, distCoeffs, rvec, tvec,useExtrinsicGuess, iterationsCount, reprojectionError, confidence,inliers_idx, pnpMethod );
           }
           LOGD("d");
   	framePoints.push_back( Point3d( 0.0, 0.0, 0.0 ) );
   	framePoints.push_back( Point3d( 5.0, 0.0, 0.0 ) );
   	framePoints.push_back( Point3d( 0.0, 5.0, 0.0 ) );
   	framePoints.push_back( Point3d( 5.0, 5.0, 0.0 ) );
       framePoints.push_back( Point3d( 0.0, 0.0, 5.0 ) );
       framePoints.push_back( Point3d( 5.0, 0.0, 5.0 ) );
       framePoints.push_back( Point3d( 0.0, 5.0, 5.0 ) );
       framePoints.push_back( Point3d( 5.0, 5.0, 5.0 ) );
       LOGD("e");
   	projectPoints(framePoints, rvec, tvec, _A_matrix, distCoeffs, imageFramePoints );
   	LOGD("f");
   	line(frame, imageFramePoints[0], imageFramePoints[1], CV_RGB(255,0,0), 2 );
   	line(frame, imageFramePoints[0], imageFramePoints[2], CV_RGB(0,255,0), 2 );
   	line(frame, imageFramePoints[2], imageFramePoints[3], CV_RGB(0,0,255), 2 );
   	line(frame, imageFramePoints[3], imageFramePoints[1], CV_RGB(255,0,0), 2 );

       line(frame, imageFramePoints[4], imageFramePoints[5], CV_RGB(0,255,0), 2 );
       line(frame, imageFramePoints[4], imageFramePoints[6], CV_RGB(0,0,255), 2 );
       line(frame, imageFramePoints[6], imageFramePoints[7], CV_RGB(255,0,0), 2 );
       line(frame, imageFramePoints[7], imageFramePoints[5], CV_RGB(0,255,0), 2 );

       line(frame, imageFramePoints[2], imageFramePoints[6], CV_RGB(0,0,255), 2 );
       line(frame, imageFramePoints[3], imageFramePoints[7], CV_RGB(0,0,255), 2 );
       line(frame, imageFramePoints[1], imageFramePoints[5], CV_RGB(0,0,255), 2 );
       line(frame, imageFramePoints[0], imageFramePoints[4], CV_RGB(0,0,255), 2 );


        LOGD("g");


   // symmetryTest(matches12, matches21, good_matches);
    return 0;
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





