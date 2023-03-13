#ifndef SYS_MANAGER_H
#define SYS_MANAGER_H

#include "utils.hpp"
#include <vector>

//template<typename T>
class Utils;
class SystemManager : public Utils
{
public:
    SystemManager();
    bool PwCompare(std::vector<char>);
    bool FindUnitNumber(std::vector<char>);
    char* GetLocalTime(void);
    ~SystemManager();

private:
    std::vector<unsigned char> pw;
    int MAX_PW;
    
    std::vector<unsigned char> EncryptionPw(char* pw);
    bool GetPw(std::vector<unsigned char>&);
};
#endif