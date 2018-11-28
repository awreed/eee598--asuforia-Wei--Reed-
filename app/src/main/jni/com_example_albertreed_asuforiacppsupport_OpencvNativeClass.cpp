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

void computePlaneEquation(const Point3f &p0, const Point3f &p1, const Point3f &p2, float &a, float &b, float &c, float &d) {
  //Vector p0_p1
  Point3f p0_p1;
  p0_p1.x = p0.x - p1.x;
  p0_p1.y = p0.y - p1.y;
  p0_p1.z = p0.z - p1.z;

  //Vector p0_p2
  Point3f p0_p2;
  p0_p2.x = p0.x - p2.x;
  p0_p2.y = p0.y - p2.y;
  p0_p2.z = p0.z - p2.z;

  //Normal vector
  Point3f n = p0_p1.cross(p0_p2);

  a = n.x;
  b = n.y;
  c = n.z;
  d = -(a*p0.x + b*p0.y + c*p0.z);

  float norm =  sqrt(a*a + b*b + c*c);
  a /= norm;
  b /= norm;
  c /= norm;
  d /= norm;
}

Point3f compute3DOnPlaneFrom2D(const Point2f &imagePt, const Mat &cameraMatrix, const float a, const float b, const float c, const float d) {
  double fx = cameraMatrix.at<double>(0,0);
  double fy = cameraMatrix.at<double>(1,1);
  double cx = cameraMatrix.at<double>(0,2);
  double cy = cameraMatrix.at<double>(1,2);

  Point2f normalizedImagePt;
  normalizedImagePt.x = (imagePt.x - cx) / fx;
  normalizedImagePt.y = (imagePt.y - cy) / fy;

  float s = -d / (a*normalizedImagePt.x + b*normalizedImagePt.y + c);

  cv::Point3f pt;
  pt.x = s*normalizedImagePt.x;
  pt.y = s*normalizedImagePt.y;
  pt.z = s;

  return pt;
}



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

Point2f backproject3DPoint(const Point3f &point3d, Mat _A_matrix,Mat _P_matrix )
{
    // 3D point vector [x y z 1]'
    Mat point3d_vec = Mat(4, 1, CV_64FC1);
    point3d_vec.at<double>(0) = point3d.x;
    point3d_vec.at<double>(1) = point3d.y;
    point3d_vec.at<double>(2) = point3d.z;
    point3d_vec.at<double>(3) = 1;
    // 2D point vector [u v 1]'
    Mat point2d_vec = Mat(4, 1, CV_64FC1);
    point2d_vec = _A_matrix * _P_matrix * point3d_vec;
    // Normalization of [u v]'
    Point2f point2d;
    point2d.x = point2d_vec.at<double>(0) / point2d_vec.at<double>(2);
    point2d.y = point2d_vec.at<double>(1) / point2d_vec.at<double>(2);
    return point2d;
}




JNIEXPORT jint JNICALL Java_com_example_albertreed_asuforiacppsupport_OpencvNativeClass_nativePoseEstimation
    (JNIEnv * env, jclass, jlong addrFrame, jlong aresFrame){
      Mat _A_matrix = Mat::zeros(3, 3, CV_64FC1);   // intrinsic camera parameters
      Mat _P_matrix = cv::Mat::zeros(3, 4, CV_64FC1);   // rotation-translation matrix

       LOGD("a");
       double f = 2.95;                           // focal length in mm (2.95)
       double sx = 3.67, sy = 2.74;             // sensor size
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
       Mat inliers;
       Mat distCoeffs = Mat::zeros(4, 1, CV_64FC1);    // vector of distortion coefficients


       _A_matrix.at<double>(0, 0) = params[0];       //      [ fx   0  cx ]
       _A_matrix.at<double>(1, 1) = params[1];       //      [  0  fy  cy ]
       _A_matrix.at<double>(0, 2) = params[2];       //      [  0   0   1 ]
       _A_matrix.at<double>(1, 2) = params[3];
       _A_matrix.at<double>(2, 2) = 1;
       LOGD("b");


       Mat& frame = * (Mat *) addrFrame;
       Mat& resFrame = * (Mat *) aresFrame;

       LOGD("ba2");
       Ptr<ORB> orb = ORB::create();
       LOGD("baa");
       vector<KeyPoint> frameKeypoints;
       Mat newDescriptors;
       vector<KeyPoint> keypoints;
       Mat refDescriptors;
       orb->detectAndCompute(frame, Mat(), frameKeypoints, newDescriptors);
       orb->detectAndCompute(resFrame, Mat(), keypoints, refDescriptors);

       LOGD("b3");
       const float minRatio = 1.f / 2.f;
       const int k = 2;
       vector<vector<DMatch>> Knnmatches;
       vector<DMatch> matches;

       BFMatcher matcher(NORM_L2);
       matcher.knnMatch(refDescriptors, newDescriptors, Knnmatches, 2);

        for (size_t i = 0; i < Knnmatches.size(); i++) {
        const DMatch& bestMatch = Knnmatches[i][0];
        const DMatch& betterMatch = Knnmatches[i][1];
        float  distanceRatio = bestMatch.distance / betterMatch.distance;
        if (distanceRatio < minRatio)
            matches.push_back(bestMatch);
        }
       /*
       BFMatcher matcher(NORM_L2);
       vector<DMatch> matches;
       matcher.match(refDescriptors, newDescriptors, matches);
       */

       LOGD("b4");
      if(matches.size() >= 4) // OpenCV requires solvePnPRANSAC to minimally have 4 set of points
       {
       Mat rvec = Mat::zeros(3, 1, CV_64FC1);          // output rotation vector
       Mat tvec = Mat::zeros(3, 1, CV_64FC1);          // output translation vector
       vector<Point3f> list_points3d_model_match; // container for the model 3D coordinates found in the scene
       vector<Point2f> list_points2d_scene_match; // container for the model 2D coordinates found in the scene
       vector<Point2f> imageFramePoints;

       vector<Point3f> axes;
       int x_offset=0;
       int y_offset=0;
       int z_offset=0;

       axes.push_back(Point3f(0+x_offset, 0+x_offset, 0+z_offset));
       axes.push_back(Point3f(20+x_offset, 0+x_offset, 0+z_offset));
       axes.push_back(Point3f(0+x_offset, 20+x_offset, 0+z_offset));
       axes.push_back(Point3f(20+x_offset,20+x_offset, 0+z_offset));
       axes.push_back(Point3f(0+x_offset, 0+x_offset, 20+z_offset));
       axes.push_back(Point3f(20+x_offset, 0+x_offset, 20+z_offset));
       axes.push_back(Point3f(0+x_offset, 20+x_offset, 20+z_offset));
       axes.push_back(Point3f(20+x_offset, 20+x_offset, 20+z_offset));
       vector<Point3f> refModel;
       vector<Point2f> matchedFramePoints;
       vector<Point2f> axes2d;
       vector<KeyPoint> matchedKeypoints;
//calculate the 3D coordinates of 2D
       float a,b,c,d;
      computePlaneEquation(axes[2],axes[1],axes[4],a,b,c,d);
       LOGD("%f\n",a);
       LOGD("%f\n",b);
       LOGD("%f\n",c);
       LOGD("%f\n",d);


       for(unsigned int match_index = 0; match_index < matches.size(); ++match_index) {
            double x = keypoints[matches[match_index].queryIdx].pt.x;
            double y = keypoints[matches[match_index].queryIdx].pt.y;
            //refModel.push_back(Point3f(x, y, 0.0));
            refModel.push_back(compute3DOnPlaneFrom2D(keypoints[matches[match_index].queryIdx].pt, _A_matrix,a,b,c,d));
            matchedKeypoints.push_back(frameKeypoints[matches[match_index].trainIdx]);
            Point2f tmp =  frameKeypoints[matches[match_index].trainIdx].pt;
            matchedFramePoints.push_back(tmp);
       }

        LOGD("C1");
       // drawKeypoints(frame, matchedKeypoints, frame);//right
       // solvePnP(refModel, matchedFramePoints, _A_matrix, noArray(), rvec, tvec, false,SOLVEPNP_ITERATIVE );
        solvePnPRansac(refModel, matchedFramePoints, _A_matrix,noArray(), rvec, tvec, false, iterationsCount, reprojectionError, confidence, inliers, SOLVEPNP_ITERATIVE);

        Mat _R_matrix;
        Mat _t_matrix;

        _R_matrix = cv::Mat::zeros(3, 3, CV_64FC1);   // rotation matrix
        _t_matrix = cv::Mat::zeros(3, 1, CV_64FC1);   // translation matrix


        Rodrigues(rvec,_R_matrix);      // converts Rotation Vector to Matrix
        _t_matrix = tvec;       // set translation matrix
        LOGD("C1");
        _P_matrix.at<double>(0,0) = _R_matrix.at<double>(0,0);
        _P_matrix.at<double>(0,1) = _R_matrix.at<double>(0,1);
        _P_matrix.at<double>(0,2) = _R_matrix.at<double>(0,2);
        _P_matrix.at<double>(1,0) = _R_matrix.at<double>(1,0);
        _P_matrix.at<double>(1,1) = _R_matrix.at<double>(1,1);
        _P_matrix.at<double>(1,2) = _R_matrix.at<double>(1,2);
        _P_matrix.at<double>(2,0) = _R_matrix.at<double>(2,0);
        _P_matrix.at<double>(2,1) = _R_matrix.at<double>(2,1);
        _P_matrix.at<double>(2,2) = _R_matrix.at<double>(2,2);
        _P_matrix.at<double>(0,3) = _t_matrix.at<double>(0);
        _P_matrix.at<double>(1,3) = _t_matrix.at<double>(1);
        _P_matrix.at<double>(2,3) = _t_matrix.at<double>(2);

       axes2d.push_back(backproject3DPoint(axes[0],_A_matrix,_P_matrix));
       axes2d.push_back(backproject3DPoint(axes[1],_A_matrix,_P_matrix));
       axes2d.push_back(backproject3DPoint(axes[2],_A_matrix,_P_matrix));
       axes2d.push_back(backproject3DPoint(axes[3],_A_matrix,_P_matrix));
       axes2d.push_back(backproject3DPoint(axes[4],_A_matrix,_P_matrix));
       axes2d.push_back(backproject3DPoint(axes[5],_A_matrix,_P_matrix));
       axes2d.push_back(backproject3DPoint(axes[6],_A_matrix,_P_matrix));
       axes2d.push_back(backproject3DPoint(axes[7],_A_matrix,_P_matrix));




       line(frame, axes2d[0], axes2d[1], CV_RGB(255,0,0), 2 );
       line(frame, axes2d[0], axes2d[2], CV_RGB(0,255,0), 2 );
       line(frame, axes2d[2], axes2d[3], CV_RGB(0,0,255), 2 );
       line(frame, axes2d[3], axes2d[1], CV_RGB(255,0,0), 2 );

       line(frame, axes2d[4], axes2d[5], CV_RGB(0,255,0), 2 );
       line(frame, axes2d[4], axes2d[6], CV_RGB(0,0,255), 2 );
       line(frame, axes2d[6], axes2d[7], CV_RGB(255,0,0), 2 );
       line(frame, axes2d[7], axes2d[5], CV_RGB(0,255,0), 2 );

       line(frame, axes2d[2], axes2d[6], CV_RGB(0,0,255), 2 );
       line(frame, axes2d[3], axes2d[7], CV_RGB(0,0,255), 2 );
       line(frame, axes2d[1], axes2d[5], CV_RGB(0,0,255), 2 );
       line(frame, axes2d[0], axes2d[4], CV_RGB(0,0,255), 2 );


/*
       projectPoints(axes, rvec, tvec, _A_matrix, noArray(), axes2d);
        LOGD("d");
        LOGD("e");

   	    line(frame, axes2d[0], axes2d[1], CV_RGB(255,0,0), 2 );
   	    line(frame, axes2d[0], axes2d[2], CV_RGB(0,255,0), 2 );
   	    line(frame, axes2d[2], axes2d[3], CV_RGB(0,0,255), 2 );
   	    line(frame, axes2d[3], axes2d[1], CV_RGB(255,0,0), 2 );

        line(frame, axes2d[4], axes2d[5], CV_RGB(0,255,0), 2 );
        line(frame, axes2d[4], axes2d[6], CV_RGB(0,0,255), 2 );
        line(frame, axes2d[6], axes2d[7], CV_RGB(255,0,0), 2 );
        line(frame, axes2d[7], axes2d[5], CV_RGB(0,255,0), 2 );

        line(frame, axes2d[2], axes2d[6], CV_RGB(0,0,255), 2 );
        line(frame, axes2d[3], axes2d[7], CV_RGB(0,0,255), 2 );
        line(frame, axes2d[1], axes2d[5], CV_RGB(0,0,255), 2 );
        line(frame, axes2d[0], axes2d[4], CV_RGB(0,0,255), 2 );*/
        }
       LOGD("g");
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





