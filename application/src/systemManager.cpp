#include "systemManager.hpp"

#include <string>
#include <iostream>
#include <openssl/sha.h>
#include <cstring>
#include <ctime>

SystemManager::SystemManager(ConfigManager& config_) : config(&config_)
{
    INFO_LOG("Create..");
}

bool SystemManager::PwCompare(std::vector<char> input_pw)
{
	std::vector<unsigned char> encryption_input_pw = EncryptionPw(reinterpret_cast<char*>(input_pw.data()));
	if (this->IsEqual(this->GetPw(), encryption_input_pw)) 
	{
        return true;
	}
    return false;
}

std::vector<unsigned char> SystemManager::GetPw()
{
	std::vector<unsigned char> v;
    std::string pw = config->GetConfigVal("SYSTEM", "PW", config->str_val);
    copy(pw.begin(), pw.end(), back_inserter(v));

    return  v;
}

std::vector<unsigned char> SystemManager::EncryptionPw(char* pw)
{
    SHA256_CTX context;
    unsigned char hash[SHA256_DIGEST_LENGTH];
    std::string salt_ = config->GetConfigVal("SYSTEM", "SALT", config->str_val);

    SHA256_Init(&context);
    SHA256_Update(&context, (unsigned char*)pw, strlen(pw));
    SHA256_Update(&context, (unsigned char*)salt_.c_str(), strlen(salt_.c_str()));
    SHA256_Final(hash, &context);

    char hashTransform[SHA256_DIGEST_LENGTH * 2 + 1] = {0,};
    for (uint8_t i = 0; i < SHA256_DIGEST_LENGTH; i++)
    {
        snprintf(&hashTransform[i*2], SHA256_DIGEST_LENGTH * 2 + 1, "%02x", (unsigned int)hash[i]);
    }
    std::vector<unsigned char> encryption_pw(hashTransform, hashTransform + strlen((const char *)hashTransform));
    return encryption_pw;
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
    config = NULL;
    INFO_LOG("Delete..");
}