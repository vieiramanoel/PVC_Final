#include <opencv2/opencv.hpp>
#include <iostream>
#include "fieldlimits.hpp"
#include "findrobots.hpp"
#include "limitsparams.hpp"
#include "findball.hpp"
#include "findteams.hpp"

int main(int argc, char const *argv[])
{
    FieldLimits field;
    cv::VideoCapture video("rgb_video.avi");
    cv::Mat frame;

    while(true){
        video >> frame;
        
        field.calculateProb(frame);
        cv::imshow("RGB Video", frame);
       
        if(cv::waitKey(30) == 27 or field.isStable()) break;
    }    
    

    auto output = field.getResult();
    cv::destroyWindow("Canny Out");
    FindRobots robotFinder(output);
    BallIdentifier ballFinder;
    FindTeams teamFinder;
    int framecounter = 0;
    while(true){
        video >> frame;
        ballFinder.find(frame);
        robotFinder.find(frame);
        robotFinder.ResizeLimits(frame);
        auto robots = robotFinder.getRobots();
        
        cv::imshow("RGB Video", frame);

        framecounter++;
        if (cv::waitKey(30) == 'q') break;
    }

    return 0;
}