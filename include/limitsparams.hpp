#ifndef LIMITS_PARAMS
#define LIMITS_PARAMS

struct limitsParameters
{
    cv::Rect limits;
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    int largestAreaIndex;
};

#endif