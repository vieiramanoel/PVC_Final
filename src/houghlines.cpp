#include "houghlines.hpp"

void HoughLines::onTrackbar(int, void* param){
    HoughLines* thisptr = (HoughLines*) param;
    cv::HoughLinesP(thisptr->dst, thisptr->lines, 1, CV_PI/180, thisptr->_threshold, thisptr->_minlenght, thisptr->_maxgap);
}

void HoughLines::calculateProb(cv::Mat input){
    preProcessor(input);
    cv::createTrackbar( "Hough Gap", "RGB Video", &_maxgap, 50, onTrackbar, (void*) this);
    cv::createTrackbar( "Hough Lenght", "RGB Video", &_minlenght, 200, onTrackbar, (void*) this);
    cv::createTrackbar( "Hough Thresh", "RGB Video", &_threshold, 200, onTrackbar, (void*) this);
    onTrackbar(_minlenght, (void*)this);
    drawGreatLines(input);
}

void HoughLines::drawGreatLines(cv::Mat input){
    std::vector<cv::Vec4i> bestLines;

    for (size_t i = 0; i < lines.size(); ++i)
        if(norm(lines[i]) > 100)
            bestLines.push_back(lines[i]);

    for(size_t i = 0; i < bestLines.size(); i++)
        line(input, cv::Point(bestLines[i][0], bestLines[i][1]),
            cv::Point(bestLines[i][2], bestLines[i][3]), cv::Scalar(139,0,0), 3, 8 );
}

void HoughLines::preProcessor(cv::Mat input){
    cv::Canny(input, dst, 50, 200, 3);
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