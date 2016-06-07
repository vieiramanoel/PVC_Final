#ifndef HOUGHLINES_H_
#define HOUGHLINES_H_
#include "opencv2/opencv.hpp"

class HoughLines
{
  public:
    void calculateProb(cv::Mat);
    void calculateStd(cv::Mat);
    cv::Mat getResult();

  private:
    void preProcessor(cv::Mat);
    cv::Mat dst, cdst;
    std::vector<cv::Vec2f> stdlines;
    std::vector<cv::Vec4i> lines;

};


#endif