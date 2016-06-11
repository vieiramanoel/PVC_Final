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
        std::vector<cv::Mat> channels(3);
        cv::split(hsvFrame, channels);
        hough.calculateProb(channels[2]);
        output = hough.getResult();
        cv::imshow("RGB Video", channels[2]);
        cv::imshow("Canny Out", output);
        if(cv::waitKey(30) == 27) break;
    }

    return 0;
}