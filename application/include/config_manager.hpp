#ifndef CONFIG_MANAGER_H
#define CONFIG_MANAGER_H

#include "yaml-cpp/yaml.h"

class ConfigManager
{
public:
    int int_val;
    float float_val;
    std::string str_val;

    ConfigManager(const std::string& path_);
    void FinishSetConfig();
    bool UpdateConfig();
    ~ConfigManager();
    
template <typename T> 
    T GetConfigVal(const std::string& first_tag, const std::string& second_tag, T type)
    {
        if (first_tag.empty() || second_tag.empty())
        {
            //return null;
        }

        YAML::Node& config = const_cast<YAML::Node&>(this->config); 
        auto c = config[first_tag][second_tag].as<T>(); 
        return c;
    }

template <typename T>
    bool SetConfigVal(const std::string& first_tag, const std::string& second_tag, T value)
    {
        if (first_tag.empty() || second_tag.empty())
        {
            return false;
        }

        this->config[first_tag][second_tag] = value;

        return true;
    }
    
private:
    mutable YAML::Node config;
    std::string path;
};
#endif
