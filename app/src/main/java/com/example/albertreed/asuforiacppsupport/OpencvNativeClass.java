package com.example.albertreed.asuforiacppsupport;

import org.opencv.core.KeyPoint;
import org.opencv.core.Point;
import org.opencv.core.Point3;

public class OpencvNativeClass {
    public native static int convertGray(long matAddrRgba, long mat);

    public native static Point[] nativePoseEstimation(long addrFrame, long refFrame, long outFrame, long descriptorMat, KeyPoint[] Javakeypoints,
    Point3[] points3D);

    public native static KeyPoint[] getReferencePoints(long addrFrame, long descriptorsAddr );

    public native static KeyPoint[] testKeyPoints(long addrFrame);
}
