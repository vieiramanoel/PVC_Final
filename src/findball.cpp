#include "findball.hpp"

const cv::Scalar BallIdentifier::CIRCLE_COLOR_(0, 0, 255);

BallIdentifier::BallIdentifier(){
    ball_pose_ = cv::Point2f(320, 240);
    counter_ = 0;
    issegmented_ = false;
}

cv::Point2f BallIdentifier::getBallPose()
{
    return ball_pose_;
}

std::vector<cv::Point> BallIdentifier::findLargerBlob(std::vector< std::vector<cv::Point> > contours)
{
    std::vector<cv::Point> comparison_contour;
    comparison_contour = contours[0];
    for (uint i = 0; i < contours.size() - 1; ++i)
    {
        if (cv::contourArea(comparison_contour) < cv::contourArea(contours[i+1]))
            comparison_contour = contours[i+1];
    }
    return comparison_contour;
}

bool BallIdentifier::segment(cv::Mat input){
    auto window_name = "Ball Segmentation";
    cv::Mat hsv;
    cv::cvtColor(input, hsv, CV_BGR2HSV);

    cv::namedWindow(window_name);
    cv::createTrackbar("HMIN", window_name, &hsv_min_h_, 360);
    cv::createTrackbar("HMAX", window_name, &hsv_max_h_, 360);
    cv::createTrackbar("SMIN", window_name, &hsv_min_s_, 256);
    cv::createTrackbar("SMAX", window_name, &hsv_max_s_, 256);
    cv::createTrackbar("VMIN", window_name, &hsv_min_v_, 256);
    cv::createTrackbar("VMAX", window_name, &hsv_max_v_, 256);
    cv::inRange(hsv, cv::Scalar(hsv_min_h_, hsv_min_s_, hsv_min_v_),
                     cv::Scalar(hsv_max_h_, hsv_max_s_, hsv_max_v_), segmentedImage);
    cv::imshow(window_name, segmentedImage);
    if (cv::waitKey(30) == 27)
    {
        cv::destroyWindow(window_name);
        return true;
    }
    return false;
}

void BallIdentifier::find(cv::Mat &rgb_frame)
{
    
    if(issegmented_){
        std::vector<std::vector<cv::Point>> contours;
        std::vector<cv::Point> larger_contour;
        cv::Moments moments;
        cv::Mat hsv;
        
        cv::cvtColor(rgb_frame, hsv, CV_BGR2HSV);

        cv::inRange(hsv, cv::Scalar(hsv_min_h_, hsv_min_s_, hsv_min_v_),
                     cv::Scalar(hsv_max_h_, hsv_max_s_, hsv_max_v_), segmentedImage);
        
        cv::findContours(segmentedImage, contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);
        if (contours.size() != 0)
        {
            larger_contour = findLargerBlob(contours);
            moments = cv::moments(larger_contour, true);
            ball_pose_ = cv::Point2f(moments.m10/moments.m00, moments.m01/moments.m00);
        }


        cv::circle(rgb_frame, ball_pose_, 10, CIRCLE_COLOR_);
    }
    else
        issegmented_ = segment(rgb_frame);
};

