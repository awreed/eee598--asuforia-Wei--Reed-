package com.example.albertreed.asuforiacppsupport;

import org.opencv.core.Mat;
import org.opencv.core.Point;

/*Interface implemented by MainActivity*/
public interface PoseListener {
    void onPose(Point[] points, Mat frame);
}
