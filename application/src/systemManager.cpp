#include "systemManager.hpp"
#include <string>
#include <iostream>
#include <openssl/sha.h>
#include <cstring>

SystemManager::SystemManager()
{
    MAX_PW = 128;
}

bool SystemManager::FindUnitNumber(std::vector<char> input_unit_num)
{
    std::vector<char> get_unit_num;

    get_unit_num.push_back('1');
    get_unit_num.push_back('2');
    get_unit_num.push_back('3');
    get_unit_num.push_back('4');
    if (this->IsEqual(get_unit_num, input_unit_num))
    {
        return true;
    }
    return false;
}

bool SystemManager::PwCompare(std::vector<char> input_pw)
{
    if (this->GetPw(this->pw))
    {
        std::vector<unsigned char> encryption_input_pw = EncryptionPw(reinterpret_cast<char*>(input_pw.data()));
        if (IsEqual(this->pw, encryption_input_pw)) 
        {
            return true;
        }
    }
    else
    {
        std::cout << "시스템 패스워드를 획득 할 수 없습니다. !!!!!!!!!!!!" << std::endl;
    }
    return false;
}

bool SystemManager::GetPw(std::vector<unsigned char>& sys_pw)
{
    unsigned char t[] = "ef797c8118f02dfb649607dd5d3f8c7623048c9c063d532cc95c5ed7a898a64f"; // 1 2 3 4
    sys_pw.insert(sys_pw.begin(), std::begin(t), std::end(t));

    return true;
}

std::vector<unsigned char> SystemManager::EncryptionPw(char* pw)
{
    SHA256_CTX context;
    unsigned char hash[SHA256_DIGEST_LENGTH];
    char* salt = "5678";

    SHA256_Init(&context);
    SHA256_Update(&context, (unsigned char*)pw, strlen(pw));
    SHA256_Update(&context, (unsigned char*)salt, strlen(salt));
    SHA256_Final(hash, &context);

    char hashTransform[SHA256_DIGEST_LENGTH * 2 + 1] = {0,};
    for (uint8_t i = 0; i < SHA256_DIGEST_LENGTH; i++)
    {
        snprintf(&hashTransform[i*2], SHA256_DIGEST_LENGTH * 2 + 1, "%02x", (unsigned int)hash[i]);
    }
    std::vector<unsigned char> encryption_pw(hashTransform, hashTransform + strlen((const char *)hashTransform) + 1);
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

SystemManager::~SystemManager()
{

}