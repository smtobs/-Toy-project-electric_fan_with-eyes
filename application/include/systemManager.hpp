#ifndef SYS_MANAGER_H
#define SYS_MANAGER_H

#include <vector>
#include <iostream>

#include "utils.hpp"
#include "log.hpp"

class Utils;
class SystemManager : public Utils
{
public:
    SystemManager();
    bool PwCompare(std::vector<char>);
    bool FindUnitNumber(std::vector<char>);
    char* GetLocalTime(void);
    uint64_t GetTick();
    bool IsTimeDiff(unsigned long now, unsigned long prev, unsigned long goal);
    ~SystemManager();

private:
    std::vector<unsigned char> pw;
    int MAX_PW;
    
    std::vector<unsigned char> EncryptionPw(char* pw);
    std::vector<unsigned char> GetPw();
};
#endif