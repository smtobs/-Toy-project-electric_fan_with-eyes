
#include "keyPad.hpp"
#include <cstring>
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

KeyPad::KeyPad(const char* dev_path)
{
    if ((this->fd = open(dev_path, O_RDWR)) < 0)
    {
        ERR_LOG("{} {}", dev_path, strerror(errno));
    }
    else
    {   
        INFO_LOG("{} open success", dev_path);
    }
}

std::vector<char> KeyPad::BuffCpy()
{
    std::vector<char> v(this->buff.begin(), this->buff.end());
    return v;
}

void KeyPad::BuffCler()
{
    if (this->buff.size())
    {
        this->buff.clear();
		this->buff.shrink_to_fit();
    }
}

char KeyPad::Scan()
{
    char push_button[2] = {0,};

    this->keypad_mutex.lock();
    read(fd, push_button, sizeof(push_button));
    this->keypad_mutex.unlock();
    
    if (push_button[0] == '\0')
    {
        return '\0';
    }
    
    DEBUG_LOG("Keypad Input : {}", push_button);

    if ((push_button[0] !='A') &&
        (push_button[0] != 'B') &&
        (push_button[0] != 'C') &&
        (push_button[0] != 'D'))
    {
        this->buff.push_back(push_button[0]);
    }
    return push_button[0];
}

KeyPad::~KeyPad()
{
    if (this->fd)
    {
        close(this->fd);
        INFO_LOG("close success");
    }
}
