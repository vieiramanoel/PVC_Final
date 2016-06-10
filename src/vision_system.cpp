#include <opencv2/opencv.hpp>
#include <iostream>
#include "houghlines.hpp"

int main(int argc, char const *argv[])
{
    HoughLines hough;
    cv::VideoCapture video("rgb_video.avi");
    cv::Mat frame, hsvFrame;
    cv::Mat output, outputstd;
    while(true){
        video >> frame;
        cv::cvtColor(frame, hsvFrame, CV_BGR2HSV);
        hough.calculateProb(hsvFrame);
        output = hough.getResult();
        cv::imshow("RGB Video", hsvFrame);
        //cv::imshow("Canny Out", output);
        if(cv::waitKey(30) == 27) break;
    }

    return 0;
}