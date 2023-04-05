#include "systemManager.hpp"

#include <string>
#include <iostream>
#include <openssl/sha.h>
#include <cstring>
#include <ctime>
#include <iomanip>

SystemManager::SystemManager(ConfigManager& config_) : config(&config_)
{
    INFO_LOG("Create..");
}

bool SystemManager::PwCompare(std::vector<char> input_pw)
{
    if (input_pw.empty())
    {
        return false;
    }

    /* SHA256 PW */
    std::string get_pw, sha256_pw;
    if ((EncryptionSHA256(reinterpret_cast<char*>(input_pw.data()), sha256_pw) == false))
    {
        return false;
    }

    /* Get PW */
    if ((this->GetPw(get_pw) == false))
    {
        return false;
    }

    /* Check PW */
	if (this->IsEqual(get_pw, sha256_pw)) 
	{
        return true;
	}
    return false;
}

bool SystemManager::GetPw(std::string& pw)
{
    pw = config->GetConfigVal("SYSTEM", "PW", config->str_val);
    return pw.empty() ?  false : true;
}

bool SystemManager::EncryptionSHA256(char* pw, std::string& hash_pw)
{
    SHA256_CTX context;
    unsigned char hash[SHA256_DIGEST_LENGTH];
    std::string salt_ = config->GetConfigVal("SYSTEM", "SALT", config->str_val);

    SHA256_Init(&context);
    SHA256_Update(&context, (unsigned char*)pw, strlen(pw));
    SHA256_Update(&context, (unsigned char*)salt_.c_str(), strlen(salt_.c_str()));
    SHA256_Final(hash, &context);

    std::stringstream hex_hash;
    for (uint8_t i = 0; i < SHA256_DIGEST_LENGTH; i++)
    {
        hex_hash << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(hash[i]);
    }
    hash_pw = hex_hash.str();

    return hash_pw.empty() ?  false : true;
}

char* SystemManager::GetLocalTime(void)
{
    struct tm tm;
    time_t ct;

    ct = time(NULL);
    tm = *localtime(&ct);

    static char cur_time[32];
    memset(cur_time, 0x0, sizeof(cur_time));
    snprintf(cur_time, sizeof(cur_time), "%d-%d-%d %d:%d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min);

    return cur_time;
}

uint64_t SystemManager::GetTick(void)
{
    struct timespec time_probe;
    
    if (clock_gettime(CLOCK_MONOTONIC, &time_probe) == -1)
    {
        ERR_LOG("Clock_gettime error");
    }
    uint64_t time_ms = time_probe.tv_nsec / 1000000UL;
    time_ms = time_ms + (time_probe.tv_sec * 1000);
	
    return time_ms;
}

bool SystemManager::IsTimeDiff(unsigned long now, unsigned long prev, unsigned long goal)
{
    constexpr uint64_t MAX_TIME = 0xFFFFFFFF;

    if (now > prev)
    {
        return ((now - prev) >= goal);
    }
    else
    {
        return (((MAX_TIME - prev) + now + 1 ) >= goal);
    }
}

SystemManager::~SystemManager()
{
    config = nullptr;
    INFO_LOG("Delete..");
}