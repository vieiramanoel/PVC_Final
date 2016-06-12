#include "houghlines.hpp"

HoughLines::HoughLines(){
    cv::FileStorage paramReader("data.yml", cv::FileStorage::READ);

    auto cannyParam = paramReader["canny_parameters"];
    _cannythresh1 = cannyParam["Thresh1"];
    _cannythresh2 = cannyParam["Thresh2"];

    _hasfinishedfor = false;
    largestArea = 0;
    largestAreaIndex = 0;
}

HoughLines::~HoughLines(){
    cv::FileStorage paramWriter("data.yml", cv::FileStorage::WRITE);

    paramWriter << "canny_parameters";
    paramWriter << "{" << "Thresh1" << _cannythresh1; 
    paramWriter        << "Thresh2" << _cannythresh2 << "}";
    paramWriter.release();
}

void HoughLines::calculateProb(cv::Mat input){
    preProcessor(input);
    std::vector<std::vector<cv::Point> > newcontours;
    std::vector<cv::Vec4i> newhierarchy;

    cv::findContours(dst, newcontours, newhierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
    for (uint i = 0; i < newcontours.size(); ++i)
    {
        double a = cv::contourArea(newcontours[i], false);
        if(a > largestArea){
            largestArea = a;
            largestAreaIndex = i;
            boundingRect = cv::boundingRect(newcontours[i]);
            contours = newcontours;
            hierarchy = newhierarchy;
        }
    }
    drawGreatLines(input);
}

void HoughLines::drawGreatLines(cv::Mat input){
    auto color = cv::Scalar::all(133);
    cv::drawContours(input, contours, largestAreaIndex, color, 3, 8, hierarchy);
    cv::rectangle(input, boundingRect, cv::Scalar(0,255,0),1, 8,0); 
}


void HoughLines::preProcessor(cv::Mat input){
    cv::blur(input, input, cv::Size(5,5));
    cv::Canny(input, dst, _cannythresh1, _cannythresh2, 3);
    cv::createTrackbar("Canny Threshold 1", "Canny Out", &_cannythresh1, 1000);
    cv::createTrackbar("Canny Threshold 2", "Canny Out", &_cannythresh2, 1000);
}

cv::Mat HoughLines::getResult(){

    return dst;
}

