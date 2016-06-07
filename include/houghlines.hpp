#ifndef HOUGHLINES_H_
#define HOUGHLINES_H_
#include "opencv2/opencv.hpp"
#include <iostream>

class HoughLines
{
  public:
    HoughLines();
    void calculateProb(cv::Mat);
    void calculateStd(cv::Mat);
    cv::Mat getResult();

  private:
    void preProcessor(cv::Mat);
    void drawGreatLines(cv::Mat);
    static void onHoughTrackbar(int, void*);
    static void onCannyTrackbar(int, void*);
    cv::Mat dst, cdst;
    std::vector<cv::Vec2f> stdlines;
    std::vector<cv::Vec4i> lines;
    int _minlenght = 33;
    int _maxgap = 104;
    int _threshold = 125;
    int _cannythresh1 = 0;
    int _cannythresh2 = 0;
};


#endif