#include "fieldlimits.hpp"

FieldLimits::FieldLimits(){
    cv::FileStorage paramReader("data.yml", cv::FileStorage::READ);

    auto cannyParam = paramReader["canny_parameters"];
    calibrating = cannyParam["Calibrating"];
    _cannythresh1 = cannyParam["Thresh1"];
    _cannythresh2 = cannyParam["Thresh2"];

    counterAdjust_ = 0;
    largestArea = 0;
    largestAreaIndex = 0;
    hasRectSetted_ = false;
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
    if(not hasRectSetted_){
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
                counterAdjust_ = 0;
                largestArea = a;
                largestAreaIndex = i;
                boundingRect = newboundingRect;
                contours = newcontours;
                hierarchy = newhierarchy;
            }
            else
                counterAdjust_++;
        }
    }
    drawGreatLines(input);
}


void FieldLimits::drawGreatLines(cv::Mat input){
    auto color = cv::Scalar::all(133);
    cv::drawContours(input, contours, largestAreaIndex, color, 3, 8, hierarchy);
    cv::rectangle(input, boundingRect, cv::Scalar(0,255,0),3, 8,0); 
}


void FieldLimits::preProcessor(cv::Mat input){
    cv::Mat cannyMat;
    std::vector<cv::Mat> channels(3);

    input.copyTo(cannyMat);
    
    cv::cvtColor(cannyMat, cannyMat, CV_BGR2HSV);
    cv::split(cannyMat, channels);
    
    cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3), cv::Point(1, 1));
    
    cv::blur(cannyMat, cannyMat, cv::Size(5,5));
    cv::Canny(cannyMat, dst, _cannythresh1, _cannythresh2, 3);
    cv::imshow("Canny Out", dst);
    if(calibrating){
        cv::createTrackbar("Canny Threshold 1", "Canny Out", &_cannythresh1, 1000);
        cv::createTrackbar("Canny Threshold 2", "Canny Out", &_cannythresh2, 1000);
    }
}

cv::Rect FieldLimits::getResult(){
    return boundingRect;
}

void FieldLimits::setNewRect(cv::Rect newlimits){
    hasRectSetted_ = true;
    boundingRect = newlimits;
}

bool FieldLimits::isStable(){
    return counterAdjust_ > 4000 ? true : false;
}