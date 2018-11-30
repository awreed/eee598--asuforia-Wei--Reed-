package com.example.albertreed.asuforiacppsupport;

import org.opencv.core.KeyPoint;
import org.opencv.core.Point;
import org.opencv.core.Point3;

public class OpencvNativeClass {
    /*not used*/
    public native static int convertGray(long matAddrRgba, long mat);

    /*main algorithm*/
    public native static Point[] nativePoseEstimation(long addrFrame, long refFrame, long outFrame, long descriptorMat, KeyPoint[] Javakeypoints,
    Point3[] points3D);

    /*finds keypoints in the reference image*/
    public native static KeyPoint[] getReferencePoints(long addrFrame, long descriptorsAddr );

    /*not used*/
    public native static KeyPoint[] testKeyPoints(long addrFrame);
}
