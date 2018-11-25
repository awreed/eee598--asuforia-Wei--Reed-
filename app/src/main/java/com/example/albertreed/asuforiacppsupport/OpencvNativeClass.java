package com.example.albertreed.asuforiacppsupport;

public class OpencvNativeClass {
    public native static int convertGray(long matAddrRgba, long mat);

    public native static int nativePoseEstimation(long addrFrame);
}
