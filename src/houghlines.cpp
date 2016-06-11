#include "houghlines.hpp"

HoughLines::HoughLines(){
    cv::FileStorage paramReader("data.yml", cv::FileStorage::READ);
    auto houghParam = paramReader["hough_parameters"];
    _maxgap = houghParam["Gap"];
    _minlenght = houghParam["Lenght"];
    _threshold = houghParam["Thresh"];
    auto cannyParam = paramReader["canny_parameters"];
    _cannythresh1 = cannyParam["Thresh1"];
    _cannythresh2 = cannyParam["Thresh2"];
    auto limitParam = paramReader["limit_parameters"];
    _lenLim = limitParam["Norm"];
    _leftLim = limitParam["left"];
    _leftDst = limitParam["left_Distance"];
    _bottomLim = limitParam["Bottom"];
    _bottomDst = limitParam["Bottom_Distance"];
    _hasfinishedfor = false;
}

HoughLines::~HoughLines(){
    cv::FileStorage paramWriter("data.yml", cv::FileStorage::WRITE);
    paramWriter << "hough_parameters";
    paramWriter << "{" << "Gap"  << _maxgap;
    paramWriter        << "Lenght" << _minlenght;
    paramWriter        << "Thresh" << _threshold << "}";
    paramWriter << "canny_parameters";
    paramWriter << "{" << "Thresh1" << _cannythresh1; 
    paramWriter        << "Thresh2" << _cannythresh2 << "}";
    paramWriter << "limit_parameters";
    paramWriter << "{" << "Norm" << _lenLim;
    paramWriter        << "Left" << _leftLim;
    paramWriter        << "Left_Distance" << _leftDst;
    paramWriter        << "Bottom" << _bottomLim;
    paramWriter        << "Bottom_Distance" << _bottomDst << "}";
    paramWriter.release();
}

void HoughLines::calculateProb(cv::Mat input){
    preProcessor(input);
    /*cv::createTrackbar( "Hough Gap", "RGB Video", &_maxgap, 1000);
    cv::createTrackbar( "Hough Lenght", "RGB Video", &_minlenght, 200);
    cv::createTrackbar( "Hough Thresh", "RGB Video", &_threshold, 200);*/
    cv::HoughLinesP(dst, lines, 1, CV_PI/180, _threshold, _minlenght, _maxgap);
    drawGreatLines(input);
}

void HoughLines::drawGreatLines(cv::Mat input){
    std::vector<cv::Vec4i> bestLines;
    cv::createTrackbar("Left Lim", "RGB Video", &_leftLim, 300);
    cv::createTrackbar("LDistance", "RGB Video", &_leftDst, 50);
    cv::createTrackbar("Bottom Lim", "RGB Video", &_bottomLim, 100);
    cv::createTrackbar("Distance", "RGB Video", &_bottomDst, 20);
    for(auto line : lines){
        if(cv::norm(line) > _lenLim)
            if(line[0] < _leftLim and (std::abs(line[2] - line[0]) < _leftDst))
                bestLines.push_back(line);
            if (line[0] < _bottomLim and (std::abs(line[1] - line[3]) < _bottomDst))
                bestLines.push_back(line);

    }
    
    for(auto bestLine : lines)
        cv::line(input, cv::Point(bestLine[0], bestLine[1]),
            cv::Point(bestLine[2], bestLine[3]), cv::Scalar(139,0,0), 3, 8 );
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

void HoughLines::calculateStd(cv::Mat input){
    preProcessor(input);
    cv::HoughLines(dst, stdlines, 1, CV_PI/180, 100, 0, 0);

    for (size_t i = 0; i < lines.size(); ++i)
    {
        float rho = lines[i][0], theta = lines[i][1];
        cv::Point pt1, pt2;
        double a = cv::cos(theta), b = cv::sin(theta);
        double x0 = a*rho, y0 = b*rho;
        pt1.x = cvRound(x0 + 1000*(-b));
        pt1.y = cvRound(y0 + 1000*(a));
        pt2.x = cvRound(x0 - 1000*(-b));
        pt2.y = cvRound(x0 - 1000*(a));
        line(cdst, pt1, pt2, cv::Scalar(255,255,255), 3, CV_AA);
    }
}