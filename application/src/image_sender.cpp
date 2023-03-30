#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "image_sender.hpp"

ImageSender::ImageSender(const std::string& path, const std::string& dest_ip_, int dest_port_, int img_width_, int img_height_)
            : cap(new cv::VideoCapture(path)), dest_ip(dest_ip_), dest_port(dest_port_), img_width(img_width_), img_height(img_height_)
{
    if (!this->cap->isOpened())
    {
        ERR_LOG("Failed to open camera.");
    }
}

cv::Mat ImageSender::CaptureImage(const int width, const int height)
{
    cv::Mat read_frame;
    if (!cap->read(read_frame))
    {
        std::cerr << "Failed to read frame." << std::endl;
        cv::Mat black_image(width, height, CV_8UC3, cv::Scalar(0, 0, 0)); 
        return black_image;
    }

    return read_frame;
}

bool ImageSender::SendImage(const char* dest_ip, const int dest_port, const int width, const int height)
{
    cv::Mat frame; 
    resize(this->CaptureImage(width, height), frame, cv::Size(width, height));

    std::vector<uchar> buffer;
    cv::imencode(".jpg", frame, buffer);
    
    return SendOneShotUdp(dest_ip, dest_port,
                        reinterpret_cast<const char*>(buffer.data()), buffer.size());
}

bool ImageSender::SendOneShotUdp(const char* ip, const int port, const char* data, const size_t size)
{
    if ((data == NULL) || (*data == '\0'))
    {
        return false;
    }

    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == -1)
    {
        return false;
    }

    struct sockaddr_in server;
    server.sin_family      = AF_INET;
    server.sin_addr.s_addr = inet_addr(ip);
    server.sin_port        = htons(port);

    if (sendto(sock, data, size, 0, (struct sockaddr *)&server, sizeof(server)) == -1)
    {
        perror("sendto");
        goto error;
    }

    close(sock);
    return true;

    error:
    close(sock);
    return false;        
}

ImageSender::~ImageSender()
{
    delete this->cap;
}
    
