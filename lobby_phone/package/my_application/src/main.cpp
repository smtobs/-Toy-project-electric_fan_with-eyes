#include <iostream>
#include <cstdio>
#include <unistd.h>
#include <cassert>

#include "utils.hpp"
#include "event_driven.hpp"
#include "config_manager.hpp"

class Core : public Utils
{
public:
    Core(int argc, char **argv) : argc_(argc), argv_(argv)
    {
        this->OptionHandler();
        config = new ConfigManager(this->GetConfigPath());
        this->ev_driven = new EventDriven(*config);
    }

    ~Core()
    {
        delete this->config;
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
    ConfigManager* config;
    std::string config_path;
    int argc_;
    char** argv_;

    void Help()
    {
        std::fprintf(stderr, "Usage : [-d -f Config File Name]\n");
        std::fprintf(stderr, "  -d,         Background\n");
        std::fprintf(stderr, "  -f,         Config file path\n");
        std::fprintf(stderr, "  -h,         Help\n\n");
        
    }

    bool OptionHandler()
    {
	    std::cout << "OptionHandler start" << std::endl;
        if (this->argc_ <= 1)
        {
	    std::cout << "argc < 1" << std::endl;
            return false;
        }


        char opt;
		do
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
                    this->Help();
                    return false;
                }

                default:
                {
                    std::fprintf(stderr, "Unknown option: %c\n", optopt);
                    return false;
                }
            }
		} while ((opt = getopt(this->argc_, this->argv_, "f:dh")) != -1);
        return true; 
    }
};

int main(int argc, char** argv)
{
    std::cout << "core start !" << std::endl;
    std::unique_ptr<Core> core(new Core(argc, argv));
    core->Start();

    return 0;
}

