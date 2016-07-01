#ifndef FIND_TEAMS_H_
#define FIND_TEAMS_H_

class FindTeams
{
  public:
    FindTeams();
    ~FindTeams();
    void find(std::vector<cv::Rect>, cv::Mat&);
};

#endif