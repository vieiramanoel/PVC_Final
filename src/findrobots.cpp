#include "findrobots.hpp"


FindRobots::FindRobots(limitsParameters params){
    params_ = params;
    resizeRatio_ = 0;
    cv::FileStorage paramReader("data.yml", cv::FileStorage::READ);

    hasclosed_ = false;

    auto cannyParam = paramReader["canny_parameters"];
    _cannythresh1 = cannyParam["Thresh1"];
    _cannythresh2 = cannyParam["Thresh2"];
}


void FindRobots::ResizeLimits(cv::Mat input){
    input_ = input;
    cv::createTrackbar("Resize Percentage", "RGB Video", &resizeRatio_, 100);
    drawRect(input);
}

void FindRobots::find(cv::Mat input){
    auto upperPoints = preprocessor(input);
    extractPoints(upperPoints);

    std::vector<std::vector<cv::Point>> newcontours;
    std::vector<cv::Vec4i> newhierarchy;
    cv::Rect newboundingRect;
    cv::Mat test;

    upperPoints.copyTo(test);

    cv::createTrackbar("Area", "white image", &area_, 2000);

    uchar* p;
    cv::dilate(upperPoints, upperPoints, cv::Mat());
    for (int i = 0; i < upperPoints.rows; ++i)
    {
        p = upperPoints.ptr<uchar>(i);
        for (int j = 0; j < upperPoints.cols; ++j)
        {
            if ((int)p[j] == 255)
            {
                int area = 10;
                bool islost = true;
                
                for (int k = 0; k < 20 and j+k < upperPoints.cols; ++k)
                    if(p[j+k] == 255)
                        islost = false;

                if (islost)
                    p[j] = 0;

                for (int k = 5; k < area and p[j+1] != 255; ++k, ++j)
                    p[j] = 255;
            }else if ((int)p[j] != 0)
            {
                p[j] = 0;
            }
        }
    }

    cv::findContours(upperPoints, newcontours, newhierarchy, cv::RETR_CCOMP, cv::CHAIN_APPROX_SIMPLE );

    auto color = cv::Scalar::all(255);
    for (uint i = 0; i < newcontours.size(); ++i)
    {
        auto newboundingRect = cv::boundingRect(newcontours[i]);

        if (newboundingRect.area() > area_)
        {
            cv::drawContours(upperPoints, newcontours, i, color, 3, 8, newhierarchy);
            if (newboundingRect.height < 2*newboundingRect.width  or newboundingRect.height < newboundingRect.width/2)
            {
                cv::rectangle(input, newboundingRect, cv::Scalar(255,255,255),3, 8,0); 
            }
        }
    }

    if (not hasclosed_){
        cv::imshow("white image", upperPoints);
        if (cv::waitKey(30) == 'w'){
          cv::destroyWindow("white image");
          hasclosed_ = true;  
        } 
    }
}

cv::Mat FindRobots::derivate(cv::Mat dst){
    cv::Mat upperPoints(480, 640, CV_8UC1, cv::Scalar(0));
    uchar* p;

    for (int i = newLimits.x; i < newLimits.y + newLimits.height; ++i)
    {
        p = dst.ptr<uchar>(i);
        for (int j = newLimits.y; j < newLimits.x + newLimits.width; ++j)
        {
            if (((int)p[j] == 0 and (int)p[j+1] == 255) or 
                ((int)p[j] == 255 and (int)p[j+1] == 0 ))
            {
                upperPoints.at<uchar>(i, j) = 255;
            }
        }
    }
    return upperPoints;
}

void FindRobots::extractPoints(cv::Mat upperPoints){
    auto color = cv::Scalar::all(0);
    cv::drawContours(upperPoints, params_.contours, params_.largestAreaIndex, color, 6, 10, params_.hierarchy);
}

cv::Mat FindRobots::preprocessor(cv::Mat input){
    cv::Mat cannyMat;
    std::vector<cv::Mat> channels(3);

    input.copyTo(cannyMat);
    
    cv::cvtColor(cannyMat, cannyMat, CV_BGR2HSV);
    cv::split(cannyMat, channels);
    
    cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3), cv::Point(1, 1));
    
    cv::Canny(channels[2], cannyMat, _cannythresh1, _cannythresh2, 3);
    cannyMat = derivate(cannyMat);
    return cannyMat;
}

void FindRobots::drawRect(cv::Mat input){

    newLimits = params_.limits;
    auto widthpc = newLimits.width * (float)resizeRatio_/100;
    auto heightpc = newLimits.height * (float)resizeRatio_/100;
    newLimits.x += widthpc/2;
    newLimits.y += heightpc/2;
    newLimits.width *= 1 - ((float)resizeRatio_/100);
    newLimits.height *= 1 - ((float)resizeRatio_/100);
    cv::rectangle(input, newLimits, cv::Scalar(0,255,0),3, 8,0);
}
