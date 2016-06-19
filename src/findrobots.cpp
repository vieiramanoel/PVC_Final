#include "findrobots.hpp"


FindRobots::FindRobots(cv::Rect limits){
    limits_ = limits;
    resizeRatio_ = 0;
}


void FindRobots::ResizeLimits(cv::Mat input){
    input_ = input;
    cv::createTrackbar("Resize Percentage", "RGB Video", &resizeRatio_, 100);
    drawRect(input);
}

void FindRobots::drawRect(cv::Mat input){

    cv::Rect newLimits = limits_;
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