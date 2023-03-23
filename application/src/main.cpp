#include <iostream>
#include "yaml-cpp/yaml.h"
#include "utils.hpp"
#include "event_driven.hpp"

class Core
{
public:
    Core(int argc, char **argv) : argc_(argc), argv_(argv)
    {
        std::string config_path = "../config/core_config.yaml";
	YAML::Node& config = const_cast<YAML::Node&>(this->config);
        config = YAML::LoadFile(config_path);
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

private:
	EventDriven* ev_driven;
	mutable YAML::Node config;
	//const YAML::Node& config;
	
	int argc_;
	char** argv_;
};

int main(int argc, char** argv)
{	
Core* core_ = new Core(argc, argv);

	core_->Start();
	delete core_;
	
    return 0;
}
