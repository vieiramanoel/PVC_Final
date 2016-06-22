#ifndef FIELD_LIMITS_H
#define FIELD_LIMITS_H
#include "opencv2/opencv.hpp"
#include <iostream>
#include <limits>
#include <cstdio>
#include "limitsparams.hpp"

class FieldLimits
{
  public:
    FieldLimits();
    ~FieldLimits();
    void calculateProb(cv::Mat);
    bool isStable();
    std::vector<std::vector<cv::Point>> getContours();
    limitsParameters getResult();

  private:
    void preProcessor(cv::Mat);
    void drawGreatLines(cv::Mat);
    static void onHoughTrackbar(int, void*);
    static void onCannyTrackbar(int, void*);
    cv::Mat dst;
    cv::Rect boundingRect;
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    limitsParameters params;

    int _cannythresh1;
    int _cannythresh2;
   
    int largestArea, largestAreaIndex;
    int calibrating;
    int counterAdjust_;

    bool hasRectSetted_;
};

#endif