#include "houghlines.hpp"

void HoughLines::calculateProb(cv::Mat input){
    preProcessor(input);
    cv::HoughLinesP(dst, lines, 1, CV_PI/180, 50, 50, 10);
}

void HoughLines::preProcessor(cv::Mat input){
    //cv::cvtColor(dst, cdst, CV_BGR2HSV);
    cv::Canny(input, dst, 50, 200, 3);
    cv::imshow("lala", dst);

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