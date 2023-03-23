#ifndef OLED_H
#define OLED_H

#include <mutex>
#include "ioctl_oled.h"

#include <vector>
class Oled
{
public:
    Oled(const char* dev_path);
    void WriteDisplay(std::vector<char>, const uint8_t, const uint8_t);
    void WriteDisplay(const char *, const uint8_t, const uint8_t);
    void ClearDisplay(void);
    ~Oled();

private:
    std::mutex oled_mutex;
    int fd;
    ioctl_oled_t oled_ioctl;
};
#endif