#include <iostream>
#include <cstdio>
#include <unistd.h>

#include "yaml-cpp/yaml.h"
#include "utils.hpp"
#include "event_driven.hpp"

class Core
{
public:
    Core(int argc, char **argv) : argc_(argc), argv_(argv)
    {
        if (this->OptionHandler() == false)
        {
		std::exit(0);
        }

        YAML::Node& config = const_cast<YAML::Node&>(this->config);
        config = YAML::LoadFile(this->GetConfigPath());

        this->ev_driven = new EventDriven(config);
    }

    ~Core()
    {
        delete this->ev_driven;
    }

    void Start()
    {
        ev_driven->StartEventLoop();
    }

    void SetConfigPath(std::string& config_path)
    {
        this->config_path = config_path;
    }

    std::string GetConfigPath()
    {
        return this->config_path;
    }
    
private:
    EventDriven* ev_driven;
    mutable YAML::Node config;
    std::string config_path;
    int argc_;
    char** argv_;

    void Help(const char* cmd)
    {
        std::fprintf(stderr, "Usage : [-d -f Config File Name]\n");
        std::fprintf(stderr, "  -d,         Background\n");
        std::fprintf(stderr, "  -f,         Config file path\n");
        std::fprintf(stderr, "  -h,         Help\n\n");
    }

    bool OptionHandler()
    {
        if (this->argc_ <= 1)
        {
            return false;
        }

        int opt;
        while ((opt = getopt(this->argc_, this->argv_, "f:dh")) != -1)
        {
            switch (opt)
            {
                case 'd':
		{
                    //daemonize(); Todo
                    break;
		}
                case 'f':
		{
		    std::string str(optarg);
                    this->SetConfigPath(str);
                    break;
		}
		case 'h':
		{
                    this->Help(optarg);
                    return false;
		}
                default:
		{
                    std::fprintf(stderr, "Unknown option: %c\n", optopt);
                    return false;
		}
            }
        }
        return true; 
    }
};

int main(int argc, char** argv)
{	
    std::unique_ptr<Core> core(new Core(argc, argv));
    core->Start();
	
    return 0;
}
