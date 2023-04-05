#ifndef IMAGE_SENDER_H
#define IMAGE_SENDER_H

#include <opencv2/opencv.hpp>
#include "utils.hpp"

class ImageSender : public Utils
{
public:
    cv::VideoCapture* cap;
    std::string dest_ip;
    int dest_port;
    int img_width;
    int img_height;

    ImageSender(const std::string& path, const std::string& dest_ip_, int dest_port_, int img_width_, int img_height_);
    cv::Mat CaptureImage(const int width, const int height);
    bool SendImage(const char* dest_ip, const int dest_port, const int width, const int height);
    bool SendOneShotUdp(const char* ip, const int port, const char* data, const size_t size);
    ~ImageSender();
};
#endif
