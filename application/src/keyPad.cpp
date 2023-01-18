
#include "keyPad.hpp"
#include <cstring>
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

KeyPad::KeyPad()
{
    if ((fd = open(dev_path, O_RDWR)) == -1)
    {
        std::cout << dev_path << " Open Failed [" << strerror(errno) << "]" <<  std::endl;
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
    }
}

char KeyPad::Scan()
{
    char push_button[2] = {0,};
	read(fd, push_button, sizeof(push_button));
	std::cout << "buff : " << push_button << std::endl;

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
    // Key Pad 디바이스 드라이버 close
}




