#include "findrobots.hpp"


FindRobots::FindRobots(cv::Rect limits){
    limits_ = limits;
    resizeRatio_ = 0;
}

void FindRobots::onLimitsChanged(int, void* param){
    FindRobots *thisptr = (FindRobots*) param;
    thisptr->limits_.width *= (1 - thisptr->resizeRatio_);
    thisptr->limits_.height *= (1 - thisptr->resizeRatio_);
    thisptr->drawRect(thisptr->input_);
}

void FindRobots::ResizeLimits(cv::Mat input){
    input_ = input;
    cv::createTrackbar("Resize Percentage", "RGB Video", &resizeRatio_, 100, onLimitsChanged, (void*)this);
}

void FindRobots::drawRect(cv::Mat input){
    cv::rectangle(input, limits_, cv::Scalar(0,255,0),3, 8,0);
}

cv::Rect FindRobots::getResizedRect(){
    return limits_;
}