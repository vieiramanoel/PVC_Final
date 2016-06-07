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
    std::cout << _maxgap << std::endl;
}


void HoughLines::onHoughTrackbar(int, void* param){
}

void HoughLines::onCannyTrackbar(int, void* param){
}

void HoughLines::calculateProb(cv::Mat input){
    preProcessor(input);
    cv::createTrackbar( "Hough Gap", "RGB Video", &_maxgap, 1000, onHoughTrackbar, (void*) this);
    cv::createTrackbar( "Hough Lenght", "RGB Video", &_minlenght, 200, onHoughTrackbar, (void*) this);
    cv::createTrackbar( "Hough Thresh", "RGB Video", &_threshold, 200, onHoughTrackbar, (void*) this);
    cv::HoughLinesP(dst, lines, 1, CV_PI/180, _threshold, _minlenght, _maxgap);
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
    cv::Canny(input, dst, _cannythresh1, _cannythresh2, 3);
    cv::createTrackbar("Canny Threshold 1", "Canny Out", &_cannythresh1, 1000, onCannyTrackbar, (void*) this);
    cv::createTrackbar("Canny Threshold 2", "Canny Out", &_cannythresh2, 1000, onCannyTrackbar, (void*) this);
    cv::cvtColor(dst, cdst, CV_GRAY2BGR);
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