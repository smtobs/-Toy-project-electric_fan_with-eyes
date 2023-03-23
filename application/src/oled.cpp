#include <iostream>
#include <cstring>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <sys/ioctl.h>

#include "oled.hpp"

Oled::Oled(const char* dev_path)
{
    if ((this->fd = open(dev_path, O_RDWR)) < 0)
    {
        std::cout << dev_path << " Open Failed [" << strerror(errno) << "]" <<  std::endl;
    }

    memset(&oled_ioctl, 0x0, sizeof(ioctl_oled_t));
}

void Oled::WriteDisplay(std::vector<char> data, const uint8_t line_no, const uint8_t cursor_pos)
{
    for (auto ir = data.begin(); ir != data.end(); ++ir)
    {
        std::cout << *ir << " ";
    }
    std::cout << std::endl;
}

void Oled::WriteDisplay(const char* data, const uint8_t line_no, const uint8_t cursor_pos)
{
    this->oled_mutex.lock();
    memset(&oled_ioctl.str, 0x0, sizeof(oled_ioctl.str));

    std::strncpy(oled_ioctl.str, data, strlen(data));
    oled_ioctl.line_no    = line_no;
    oled_ioctl.cursor_pos = cursor_pos;

    ioctl(this->fd, IOCTL_OLED_DISPLAY_WRITE, &oled_ioctl);
    this->oled_mutex.unlock();
}

void Oled::ClearDisplay(void)
{
    this->oled_mutex.lock();
    ioctl(this->fd, IOCTL_OLED_CLEAR);
    this->oled_mutex.unlock();
}

Oled::~Oled()
{
    if (this->fd)
    {
        close(this->fd);
    }
}
