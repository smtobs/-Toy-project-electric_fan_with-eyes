#ifndef UTILS_H
#define UTILS_H

//#include "systemManager.hpp"

#include <string>
#include <iostream>
#include <cstring>
#include <vector>

class Utils
{
public:
    Utils();
    bool IsEqual(std::vector<char> const &v1, std::vector<char> const &v2);
    bool IsEqual(std::vector<unsigned char> const &v1, std::vector<unsigned char> const &v2);
    void test();
    
    ~Utils();
};
#endif
