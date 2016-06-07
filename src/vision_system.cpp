#include <opencv2/opencv.hpp>
#include <iostream>
#include "houghlines.hpp"

int main(int argc, char const *argv[])
{
    HoughLines hough;
    cv::VideoCapture video("rgb_video.avi");
    cv::Mat frame;
    cv::Mat output, outputstd;
    while(true){
        video >> frame;
        hough.calculateProb(frame);
        output = hough.getResult();
        cv::imshow("RGB Video", frame);

        if(cv::waitKey(30) == 27) break;
    }

    return 0;
}