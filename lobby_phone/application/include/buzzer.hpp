#ifndef BUZZER_H
#define BUZZER_H

#include <mutex>
#include "ioctl_buzzer.h"
#include "utils.hpp"

class Buzzer : public Utils
{
public:
    Buzzer(const char* dev_path);
    void SuccessSound(void);
    void FailSound(void);
    void ButtonPushSound(void);
    ~Buzzer();

private:
    std::mutex buzzer_mutex;
    int fd;
};
#endif
