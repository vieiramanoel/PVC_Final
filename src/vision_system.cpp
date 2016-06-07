#include <opencv2/opencv.hpp>
#include <iostream>

int main(int argc, char const *argv[])
{
    cv::VideoCapture video("rgb_video.avi");
    cv::Mat frame;
    while(true){
        video >> frame;
        cv::imshow("RGB Video", frame);
        if(cv::waitKey(30) == 27) break;
    }

    return 0;
}