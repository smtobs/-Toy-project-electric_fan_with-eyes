#include "utils.hpp"

Utils::Utils()
{
    this->logLevel = LoggerRegistry::getInstance().registerLogger(basename((char *)__FILE__));
}

Utils::~Utils()
{

}