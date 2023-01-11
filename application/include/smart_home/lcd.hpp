#ifndef LCD_H
#define LCD_H

#include <vector>
class Lcd
{
public:
    Lcd();
    void Write(std::vector<char>);
    void Write(const char *);
    ~Lcd();
};
#endif