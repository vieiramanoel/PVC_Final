#include <opencv2/opencv.hpp>
#include <iostream>
#include "fieldlimits.hpp"
#include <findrobots.hpp>

int main(int argc, char const *argv[])
{
    FieldLimits field;
    cv::VideoCapture video("rgb_video.avi");
    cv::Mat frame;
    cv::Rect output;
    

    while(true){
        video >> frame;
        
        field.calculateProb(frame);
        output = field.getResult();
        

        cv::imshow("RGB Video", frame);
       
        if(cv::waitKey(30) == 27 or field.isStable()) break;
    }    
    cv::destroyWindow("Canny Out");
    FindRobots founder(output);
    while(true){
        video >> frame;
        founder.find(frame);
        founder.ResizeLimits(frame);
        cv::imshow("RGB Video", frame);
        if (cv::waitKey(30) == 27) break;
    }

    return 0;
}