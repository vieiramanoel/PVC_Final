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
    std::vector<cv::Rect> getRobots();
  private:
    cv::Mat derivate(cv::Mat);
    void drawRect(cv::Mat);
    cv::Mat preprocessor(cv::Mat);
    void extractPoints(cv::Mat);
    
    cv::Rect newLimits;
    cv::Mat input_;
    std::vector<cv::Rect> robots;
    int resizeRatio_;
    int area_;

    int _cannythresh1;
    int _cannythresh2;

    bool hasclosed_;
    
    limitsParameters params_;
};
#endif
