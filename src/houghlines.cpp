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
    _infLim = limitParam["Norm"];
    _rightLim = limitParam["Right"];
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
    paramWriter << "{" << "Norm" << _infLim;
    paramWriter        << "Right" << _rightLim << "}";
    paramWriter.release();
}

void HoughLines::calculateProb(cv::Mat input){
    preProcessor(input);
    cv::createTrackbar( "Hough Gap", "RGB Video", &_maxgap, 1000);
    cv::createTrackbar( "Hough Lenght", "RGB Video", &_minlenght, 200);
    cv::createTrackbar( "Hough Thresh", "RGB Video", &_threshold, 200);
    cv::HoughLinesP(dst, lines, 1, CV_PI/180, _threshold, _minlenght, _maxgap);
    drawGreatLines(input);
}

void HoughLines::drawGreatLines(cv::Mat input){
    std::vector<cv::Vec4i> bestLines;
    cv::createTrackbar("Right Lim", "RGB Video", &_rightLim, 100);
    
    for(auto line : lines){
        if(cv::norm(line) > _infLim)
            if(line[0] < _rightLim)
                bestLines.push_back(line);
    }

    
    for(auto bestLine : bestLines)
        cv::line(input, cv::Point(bestLine[0], bestLine[1]),
            cv::Point(bestLine[2], bestLine[3]), cv::Scalar(139,0,0), 3, 8 );
    
    /*
    //max values for vertical comparison
    int vmaxValue = std::numeric_limits<int>::max();
    int vminValue = std::numeric_limits<int>::min();

    //max values for horizontal comparison
    int hmaxValue = std::numeric_limits<int>::max();
    int hminValue = std::numeric_limits<int>::min();

    int leftLine, rightLine, 
        bottomLine, topLine;

    for (size_t i = 0; i < lines.size() and (not _hasfinishedfor); ++i)
    {
        std::cout << lines[i][0] << std::endl;
        //left edge line
        if (lines[i][0] < vmaxValue)
            leftLine = i;

        //right edge line
        if (lines[i][2] > vminValue)
            rightLine = i;

        //bottom edge line
        if (lines[i][1] < hmaxValue)
            bottomLine = i;

        if (lines[i][3] > hminValue)
            topLine = i;
        _hasfinishedfor = true;
    }
    if (_hasfinishedfor)
    {
        externalLines.push_back(bestLines[leftLine]);
        externalLines.push_back(bestLines[rightLine]);
        externalLines.push_back(bestLines[bottomLine]);
        externalLines.push_back(bestLines[topLine]);

        for(auto externalLine : externalLines)
            line(input, cv::Point(externalLine[0], externalLine[1]),
                cv::Point(externalLine[2], externalLine[3]), cv::Scalar(0,0,139), 3, 8 );
    }
    else{
        for(auto bestLine : bestLines)
        cv::line(input, cv::Point(bestLine[0], bestLine[1]),
            cv::Point(bestLine[2], bestLine[3]), cv::Scalar(139,0,0), 3, 8 );
    }*/
}


void HoughLines::preProcessor(cv::Mat input){
    cv::blur(input, input, cv::Size(5,5));
    cv::Canny(input, dst, _cannythresh1, _cannythresh2, 3);
    //cv::createTrackbar("Canny Threshold 1", "Canny Out", &_cannythresh1, 1000, onCannyTrackbar, (void*) this);
    //cv::createTrackbar("Canny Threshold 2", "Canny Out", &_cannythresh2, 1000, onCannyTrackbar, (void*) this);
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