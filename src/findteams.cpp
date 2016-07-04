#include "findteams.hpp"

FindTeams::FindTeams(){

}

FindTeams::~FindTeams(){
    
}

void FindTeams::find(std::vector<cv::Rect>robots, cv::Mat& input){
    std::vector<cv::Mat> channels(3);
    for (uint i = 0; i < robots.size() ; ++i)
    {
        cv::Mat robotRoi(input, robots[3]);
        cv::split(robotRoi, channels);

        int histSize = 256;

        float range[] = {0, 256};
        const float *histRange = { range };
        bool uniform = true, accumulate = true;
        cv::Mat b_hist, g_hist, r_hist;

        calcHist( &channels[0], 1, 0, cv::Mat(), b_hist, 1, &histSize, &histRange, uniform, accumulate );
        calcHist( &channels[1], 1, 0, cv::Mat(), g_hist, 1, &histSize, &histRange, uniform, accumulate );
        calcHist( &channels[2], 1, 0, cv::Mat(), r_hist, 1, &histSize, &histRange, uniform, accumulate );

        cv::imshow("agsdaydsg", robotRoi);


        // Draw the histograms for B, G and R
        int hist_w = 512; int hist_h = 400;
        int bin_w = cvRound( (double) hist_w/histSize );

        cv::Mat histImage( hist_h, hist_w, CV_8UC3, cv::Scalar( 0,0,0) );

        /// Normalize the result to [ 0, histImage.rows ]
        normalize(b_hist, b_hist, 0, histImage.rows, cv::NORM_MINMAX, -1, cv::Mat() );
        normalize(g_hist, g_hist, 0, histImage.rows, cv::NORM_MINMAX, -1, cv::Mat() );
        normalize(r_hist, r_hist, 0, histImage.rows, cv::NORM_MINMAX, -1, cv::Mat() );

        /// Draw for each channel
        for( int i = 1; i < histSize; i++ )
        {
            cv::line( histImage, cv::Point( bin_w*(i-1), hist_h - cvRound(b_hist.at<float>(i-1)) ) ,
                cv::Point( bin_w*(i), hist_h - cvRound(b_hist.at<float>(i)) ),
                cv::Scalar( 255, 0, 0), 2, 8, 0  );
            line( histImage, cv::Point( bin_w*(i-1), hist_h - cvRound(g_hist.at<float>(i-1)) ) ,
                cv::Point( bin_w*(i), hist_h - cvRound(g_hist.at<float>(i)) ),
                cv::Scalar( 0, 255, 0), 2, 8, 0  );
            line( histImage, cv::Point( bin_w*(i-1), hist_h - cvRound(r_hist.at<float>(i-1)) ) ,
                cv::Point( bin_w*(i), hist_h - cvRound(r_hist.at<float>(i)) ),
                cv::Scalar( 0, 0, 255), 2, 8, 0  );
        }

        /// Display
        cv::namedWindow("calcHist Demo", CV_WINDOW_AUTOSIZE );
        cv::imshow("calcHist Demo", histImage );
    }
}