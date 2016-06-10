#ifndef HOUGHLINES_H_
#define HOUGHLINES_H_
#include "opencv2/opencv.hpp"
#include <iostream>
#include <limits>
#include <cstdio>

class HoughLines
{
  public:
    HoughLines();
    ~HoughLines();
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
    std::vector<cv::Vec4i> externalLines;
    int _minlenght;
    int _maxgap;
    int _threshold;
    int _cannythresh1;
    int _cannythresh2;
    int _infLim;
    int _rightLim;
    bool _hasfinishedfor;
};

#endif