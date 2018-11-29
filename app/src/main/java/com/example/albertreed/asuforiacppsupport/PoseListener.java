package com.example.albertreed.asuforiacppsupport;

import org.opencv.core.Mat;
import org.opencv.core.Point;

public interface PoseListener {
    void onPose(Point[] points, Mat frame);
}
