#include "../include/smart_home/systemManager.hpp"
#include <string>
#include <iostream>

/* 코드 옮겨야 함 */
template<typename T>
bool IsEqual(std::vector<T> const &v1, std::vector<T> const &v2)
{
    std::cout << "v1:size : " << v1.size() << std::endl;
    std::cout << "v2:size : " << v2.size() << std::endl;
    return (v1.size() == v2.size() &&
            std::equal(v1.begin(), v1.end(), v2.begin()));
}

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
    if (IsEqual(get_unit_num, input_unit_num))
    {
        return true;
    }
    return false;
}

bool SystemManager::PwCompare(std::vector<char> input_pw)
{
    if (this->GetPw(this->pw))
    {
        if (IsEqual(this->pw, input_pw))
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

bool SystemManager::GetPw(std::vector<char>& sys_pw)
{
    sys_pw.push_back('1');
    sys_pw.push_back('2');
    sys_pw.push_back('3');
    sys_pw.push_back('4');
}

SystemManager::~SystemManager()
{

}