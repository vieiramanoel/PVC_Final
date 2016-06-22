#include "opencv2/opencv.hpp"

class FindRobots
{
  public:
    FindRobots(cv::Rect, std::vector<std::vector<cv::Point>>);
    void ResizeLimits(cv::Mat);
    cv::Rect getResizedRect();
    void find(cv::Mat);
  private:
    void drawRect(cv::Mat);
    void preprocessor(cv::Mat);
    void extractPoints(cv::Mat);
    cv::Rect limits_;
    cv::Rect newLimits;
    int resizeRatio_;
    cv::Mat input_;
    cv::Mat dst;
    int _cannythresh1;
    int _cannythresh2;
    std::vector<std::vector<cv::Point>> contours_;
};
