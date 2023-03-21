#ifndef BUZZER_H
#define BUZZER_H

#include <mutex>
#include "ioctl_buzzer.h"

class Buzzer
{
public:
    Buzzer();
    void SuccessSound(void);
    void ButtonPushSound(void);
    ~Buzzer();

private:
    std::mutex buzzer_mutex;
    int fd;
    static constexpr const char* dev_path = "/dev/buzzer";
};
#endif
