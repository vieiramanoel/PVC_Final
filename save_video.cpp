#include <opencv2/opencv.hpp>
#include <iostream>

int main(int argc, char const *argv[])
{
    cv::VideoWriter rgb_writer;
    bool is_open_rgb;
    
    std::istringstream video_sourceCmd(argv[1]);
    int video_source;

    if(!(video_sourceCmd >> video_source)) return 1;

    cv::VideoCapture cap(video_source);
    cv::Mat frame;
    cap >> frame;
    rgb_writer.open("rgb_video.avi", CV_FOURCC('P','I','M','1'),
                        25, cv::Size(frame.cols, frame.rows),true);

    while(true){
        cap >> frame;
        rgb_writer << frame;
    }
    return 0;
}