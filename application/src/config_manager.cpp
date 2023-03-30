#include <cassert>
#include <iostream>
#include <fstream>
#include <unistd.h>

#include "config_manager.hpp"

ConfigManager::ConfigManager(const std::string& path_) : path(path_)
{
    YAML::Node& config = const_cast<YAML::Node&>(this->config);
    config = YAML::LoadFile(this->path);
}

void ConfigManager::FinishSetConfig()
{
    if (this->UpdateConfig() == false)
    {
        std:: cout << "Config update failed"<< std::endl;
    }
    else
    {
        std:: cout << "Config update success"<< std::endl;
    }
}

bool ConfigManager::UpdateConfig()
{
    if (access(this->path.c_str(), W_OK) != 0)
    {
        std:: cout << "Config write failed"<< std::endl;
        return false;
    }

    std::ofstream fout(this->path);
    fout << this->config;
    fout.close();
    
    return ((fout.fail()) == true ? false : true );
}

ConfigManager:: ~ConfigManager()
{

}
