#ifndef SYS_MANAGER_H
#define SYS_MANAGER_H

#include <vector>
#include <iostream>

#include "utils.hpp"
#include "config_manager.hpp"

class SystemManager : public Utils
{
public:
    SystemManager(ConfigManager& config_);
    bool PwCompare(std::vector<char>);
    char* GetLocalTime(void);
    uint64_t GetTick();
    bool IsTimeDiff(unsigned long now, unsigned long prev, unsigned long goal);
    ~SystemManager();

private:
    ConfigManager* config;
    std::vector<unsigned char> pw;
    
    bool EncryptionSHA256(char* pw, std::string& hash_pw);
    bool GetPw(std::string& get_pw);
};

#endif