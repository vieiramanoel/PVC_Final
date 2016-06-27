#ifndef BALL_FINDER_H_
#define BALL_FINDER_H_

#include <vector>

#include <opencv2/opencv.hpp>


class BallIdentifier
{
  public:

    BallIdentifier();
    void find(cv::Mat&); 
    cv::Point2f getBallPose();
    bool segment(cv::Mat);
  private:
    std::vector<cv::Point> findLargerBlob(std::vector< std::vector<cv::Point> > contours);
    static const cv::Scalar CIRCLE_COLOR_;
    cv::Point2f ball_pose_;
    int counter_;
    bool issegmented_;
    cv::Mat segmentedImage;
    int hsv_min_h_, hsv_max_h_;
    int hsv_min_s_, hsv_max_s_;
    int hsv_min_v_, hsv_max_v_;

};

#endif
