#include "lcd.hpp"
#include <iostream>

Lcd::Lcd()
{
    //LCD 드라이버 오픈
}

void Lcd::Write(std::vector<char> data)
{
    for (auto ir = data.begin(); ir != data.end(); ++ir)
    {
        std::cout << *ir << " ";
    }
    std::cout << std::endl;
}

void Lcd::Write(const char *data)
{
    std::cout << data << std::endl;
}

Lcd::~Lcd()
{

}