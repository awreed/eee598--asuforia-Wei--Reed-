package com.example.albertreed.asuforiacppsupport;

import org.opencv.core.KeyPoint;

public class OpencvNativeClass {
    public native static int convertGray(long matAddrRgba, long mat);

    public native static int nativePoseEstimation(long addrFrame, long descriptorMat, KeyPoint[] Javakeypoints);

    public native static KeyPoint[] getReferencePoints(long addrFrame, long descriptorsAddr );

    public native static KeyPoint[] testKeyPoints(long addrFrame);
}
