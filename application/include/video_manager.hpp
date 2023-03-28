
#include <opencv2/opencv.hpp>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

Video::Video()
{
    cv::VideoCapture cap(0);
    if (!this->cap.isOpened())
    {
        std::cerr << "Failed to open camera." << std::endl;
        return false;
    }
}

cv::Mat Video::CaptureImage(const int width, const int height)
{
    cv::Mat read_frame;
    if (!cap.read(read_frame))
    {
        std::cerr << "Failed to read frame." << std::endl;
        Mat black_image(width, height, CV_8UC3, Scalar(0, 0, 0)); 
        return black_image;
    }
    frame = read_frame;

    return frame;
}

bool Video::SendImage(const char* dest_ip, const int dest_port, const int width, const int height)
{
    cv::Mat frame; 
    resize(this->CaptureImage(width, height), frame, cv::Size(width, height));

    std::vector<uchar> buffer;
    cv::imencode(".jpg", frame, buffer);
    
    return SendOneShotUdp(dest_ip, dest_port,
                        reinterpret_cast<const char*>(buffer.data()), buffer.size());
}

bool Video::SendOneShotUdp(const char* ip, const int port, const char* data, const size_t size)
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);
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
