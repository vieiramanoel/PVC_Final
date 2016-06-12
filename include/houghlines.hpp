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
    cv::Mat getResult();

  private:
    void preProcessor(cv::Mat);
    void drawGreatLines(cv::Mat);
    static void onHoughTrackbar(int, void*);
    static void onCannyTrackbar(int, void*);
    cv::Mat dst;
    cv::Rect boundingRect;
    std::vector<std::vector<cv::Point> > contours;
    std::vector<cv::Vec4i> hierarchy;

    int _cannythresh1;
    int _cannythresh2;
   
    int largestArea, largestAreaIndex;
    bool _hasfinishedfor;
};

#endif