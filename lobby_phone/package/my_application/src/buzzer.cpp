#include <iostream>
#include <cstring>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <sys/ioctl.h>

#include "buzzer.hpp"

Buzzer::Buzzer(const char* dev_path)
{
    if ((this->fd = open(dev_path, O_RDWR)) < 0)
    {
       // ERR_LOG("{} {}", dev_path, strerror(errno));
	std::cout << "open fail" << std::endl;
    }
    else
    {   
        //INFO_LOG("{} open success", dev_path);
	std::cout << "open success" << std::endl;
    	SuccessSound();
    }


}

void Buzzer::SuccessSound(void)
{
    this->buzzer_mutex.lock();
    ioctl(this->fd, IOCTL_CMD_SUCCESS_SOUND_BUZZER);
    this->buzzer_mutex.unlock();
}

void Buzzer::FailSound(void)
{
    this->buzzer_mutex.lock();
    ioctl(this->fd, IOCTL_CMD_FAIL_SOUND_BUZZER);
    this->buzzer_mutex.unlock();
}

void Buzzer::ButtonPushSound(void)
{
    this->buzzer_mutex.lock();
    ioctl(this->fd, IOCTL_CMD_BUTTON_PUSH_SOUND_BUZZER);
    this->buzzer_mutex.unlock();
}

Buzzer::~Buzzer()
{
    FailSound();
    if (this->fd)
    {
        close(this->fd);
        INFO_LOG("close success");
    }
}
