#include "fieldlimits.hpp"

FieldLimits::FieldLimits(){
    cv::FileStorage paramReader("data.yml", cv::FileStorage::READ);

    auto cannyParam = paramReader["canny_parameters"];
    calibrating = cannyParam["Calibrating"];
    _cannythresh1 = cannyParam["Thresh1"];
    _cannythresh2 = cannyParam["Thresh2"];

    largestArea = 0;
    largestAreaIndex = 0;
}

FieldLimits::~FieldLimits(){
    cv::FileStorage paramWriter("data.yml", cv::FileStorage::WRITE);

    paramWriter << "canny_parameters";
    paramWriter << "{" << "Thresh1" << _cannythresh1; 
    paramWriter        << "Thresh2" << _cannythresh2;
    paramWriter        << "Calibrating" << calibrating << "}";
    paramWriter.release();
}

void FieldLimits::calculateProb(cv::Mat input){
    preProcessor(input);
    std::vector<std::vector<cv::Point> > newcontours;
    std::vector<cv::Vec4i> newhierarchy;
    cv::Rect newboundingRect;
    cv::Mat test;
    dst.copyTo(test);
    cv::findContours(test, newcontours, newhierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
    
    for (uint i = 0; i < newcontours.size(); ++i)
    {   
        newboundingRect = cv::boundingRect(newcontours[i]);
        double a = newboundingRect.area();
        if(a > largestArea){
            largestArea = a;
            largestAreaIndex = i;
            boundingRect = newboundingRect;
            contours = newcontours;
            hierarchy = newhierarchy;
        }
    }
    drawGreatLines(input);
}

void FieldLimits::drawGreatLines(cv::Mat input){
    auto color = cv::Scalar::all(133);
    cv::drawContours(input, contours, largestAreaIndex, color, 3, 8, hierarchy);
    cv::rectangle(input, boundingRect, cv::Scalar(0,255,0),1, 8,0); 
}


void FieldLimits::preProcessor(cv::Mat input){
    cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3), cv::Point(1, 1));
    cv::blur(input, input, cv::Size(5,5));
    cv::Canny(input, dst, _cannythresh1, _cannythresh2, 3);
    if(calibrating){
        cv::createTrackbar("Canny Threshold 1", "Canny Out", &_cannythresh1, 1000);
        cv::createTrackbar("Canny Threshold 2", "Canny Out", &_cannythresh2, 1000);
    }
}

cv::Mat FieldLimits::getResult(){
    return dst;
}

