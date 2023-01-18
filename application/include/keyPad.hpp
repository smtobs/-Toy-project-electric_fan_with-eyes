#ifndef KEYPAD_H
#define KEYPAD_H
#include <vector>

class KeyPad
{
public:
    KeyPad();
    std::vector<char> BuffCpy();
    void BuffCler();
    char Scan();
    ~KeyPad();

private:
    std::vector<char> buff;
    std::vector<char> test;
    int fd;
    static constexpr const char* dev_path = "/dev/keyPad";
};
#endif
