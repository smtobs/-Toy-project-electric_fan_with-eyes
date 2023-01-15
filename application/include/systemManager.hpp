#ifndef SYS_MANAGER_H
#define SYS_MANAGER_H

#include <vector>

class SystemManager
{
public:
    SystemManager();
    bool PwCompare(std::vector<char>);
    bool FindUnitNumber(std::vector<char>);
    ~SystemManager();

private:
    std::vector<char> pw;
    int MAX_PW;
    
    bool GetPw(std::vector<char>&);
};
#endif