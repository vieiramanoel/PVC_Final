#include "opencv2/opencv.hpp"

class FindRobots
{
  public:
    FindRobots(cv::Rect);
    void ResizeLimits(cv::Mat input);
    cv::Rect getResizedRect();

  private:
    void drawRect(cv::Mat input);
    static void onLimitsChanged(int, void*);
    cv::Rect limits_;
    int resizeRatio_;
    cv::Mat input_;
};
