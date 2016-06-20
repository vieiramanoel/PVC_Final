#include "findrobots.hpp"


FindRobots::FindRobots(cv::Rect limits){
    limits_ = limits;
    resizeRatio_ = 0;
    cv::FileStorage paramReader("data.yml", cv::FileStorage::READ);

    auto cannyParam = paramReader["canny_parameters"];
    _cannythresh1 = cannyParam["Thresh1"];
    _cannythresh2 = cannyParam["Thresh2"];
}


void FindRobots::ResizeLimits(cv::Mat input){
    input_ = input;
    cv::createTrackbar("Resize Percentage", "RGB Video", &resizeRatio_, 100);
    drawRect(input);
}

void FindRobots::find(cv::Mat input){
    preprocessor(input);
    uchar* p;
    int zeroscounter;
    for (int i = newLimits.x; i < newLimits.y + newLimits.height; ++i)
    {
        p = dst.ptr<uchar>(i);
        for (int j = newLimits.y; j < newLimits.x + newLimits.width; ++j)
        {
            if (((int)p[j] == 0 and (int)p[j+1] == 255) or 
                ((int)p[j] == 255 and (int)p[j+1] == 0 ))
            {
                auto robot = cv::Point(j, i);
                cv::circle(input, robot, 2, cv::Scalar::all(-1));
            }
        }
    }

}

void FindRobots::preprocessor(cv::Mat input){
    cv::Mat cannyMat;
    std::vector<cv::Mat> channels(3);

    input.copyTo(cannyMat);
    
    cv::cvtColor(cannyMat, cannyMat, CV_BGR2HSV);
    cv::split(cannyMat, channels);
    
    cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3), cv::Point(1, 1));
    
    cv::Canny(channels[2], dst, _cannythresh1, _cannythresh2, 3);
    cv::imshow("Canny Out", dst);
    
}

void FindRobots::drawRect(cv::Mat input){

    newLimits = limits_;
    auto widthpc = newLimits.width * (float)resizeRatio_/100;
    auto heightpc = newLimits.height * (float)resizeRatio_/100;
    newLimits.x += widthpc/2;
    newLimits.y += heightpc/2;
    newLimits.width *= 1 - ((float)resizeRatio_/100);
    newLimits.height *= 1 - ((float)resizeRatio_/100);
    cv::rectangle(input, newLimits, cv::Scalar(0,255,0),3, 8,0);
}

cv::Rect FindRobots::getResizedRect(){
    return limits_;
}