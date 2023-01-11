#include "../include/smart_home/keyPad.hpp"
#include <cstring>
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

KeyPad::KeyPad()
{
    if ((fd = open("/dev/keyPad", O_RDWR)) == -1)
    {
        std::cout << "Open /dev/keyPad Failed [" << strerror(errno) << "]" <<  std::endl;
    }
	read(fd, 0, 0);
    this->test.push_back('1');
    this->test.push_back('2');
    this->test.push_back('3');
    this->test.push_back('4');
    //this->test.push_back('5');
    //this->test.push_back('@');
    this->test.push_back('&');
    // Key Pad 디바이스 드라이버 오픈
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
    if (this->test.empty())
    {
        return 0;
    }
    else
    {
	read(fd, 0, 0);
        char push_button = this->test.front();

        /* Check for Call button and password button */
        if ((push_button !='@') && (push_button != '&'))
        {
            this->buff.push_back(push_button);
        }
        this->test.erase(this->test.begin());
        return push_button;
    }
    // key pad 디바이스 드라이버 상태 체크
}

KeyPad::~KeyPad()
{
    // Key Pad 디바이스 드라이버 close
}



