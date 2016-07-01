#ifndef FIND_TEAMS_H_
#define FIND_TEAMS_H_

#include "opencv2/opencv.hpp"

class FindTeams
{
  public:
    FindTeams();
    ~FindTeams();
    void find(std::vector<cv::Rect>, cv::Mat&);
};

#endif