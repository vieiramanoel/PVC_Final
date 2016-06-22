#ifndef FIND_ROBOTS_H
#define FIND_ROBOTS_H

#include "opencv2/opencv.hpp"
#include "limitsparams.hpp"


class FindRobots
{
  public:
    FindRobots(limitsParameters);
    void ResizeLimits(cv::Mat);
    void find(cv::Mat);
  private:
    void drawRect(cv::Mat);
    void preprocessor(cv::Mat);
    void extractPoints(cv::Mat);
    
    cv::Rect newLimits;
    
    int resizeRatio_;
    
    cv::Mat input_;
    cv::Mat dst;

    int _cannythresh1;
    int _cannythresh2;
    limitsParameters params_;
};
#endif
