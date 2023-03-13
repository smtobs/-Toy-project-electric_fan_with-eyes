#ifndef OLED_H
#define OLED_H

#include "ioctl_oled.h"

#include <vector>
class Oled
{
public:
    Oled();
    void WriteDisplay(std::vector<char>, const uint8_t, const uint8_t);
    void WriteDisplay(const char *, const uint8_t, const uint8_t);
    void ClearDisplay(void);
    ~Oled();

private:
    int fd;
    static constexpr const char* dev_path = "/dev/oled";
    ioctl_oled_t oled_ioctl;
};
#endif