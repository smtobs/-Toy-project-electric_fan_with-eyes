#ifndef OLED_H
#define OLED_H

#include <mutex>
#include <vector>

#include "utils.hpp"
#include "ioctl_oled.h"

class Oled : public Utils
{
public:
    Oled(const char* dev_path);
    void WriteDisplay(const char *, const uint8_t, const uint8_t);
    void ClearDisplay(void);
    ~Oled();

private:
    std::mutex oled_mutex;
    int fd;
    ioctl_oled_t oled_ioctl;
};
#endif