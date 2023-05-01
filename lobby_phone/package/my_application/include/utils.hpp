#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <cstring>
#include "log.hpp"

class Utils
{
public:
    LoggerRegistry::Map::iterator logLevel;
    Utils();
    ~Utils();    
template <typename T>
    bool IsEqual(T v1, T v2)
    {
        DEBUG_LOG("v1:size : {}", v1.size());
        DEBUG_LOG("v2:size : {}", v2.size());
        DEBUG_LOG("v1:data : {}", v1.data());
        DEBUG_LOG("v2:data : {}", v2.data());
        
        return (v1.size() == v2.size() &&
                std::equal(v1.begin(), v1.end(), v2.begin()));
    }
};

#endif
