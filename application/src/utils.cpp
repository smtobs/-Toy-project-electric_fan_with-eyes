#include "utils.hpp"
#include <vector>

#include <iostream>

Utils::Utils()
{
}

bool Utils::IsEqual(std::vector<char> const &v1, std::vector<char> const &v2)
{
    std::cout << "v1:size : " << v1.size() << std::endl;
    std::cout << "v2:size : " << v2.size() << std::endl;

    std::cout << "v1:data : " << v1.data() << std::endl;
    std::cout << "v2:data : " << v2.data() << std::endl;

    return (v1.size() == v2.size() &&
            std::equal(v1.begin(), v1.end(), v2.begin()));
}

bool Utils::IsEqual(std::vector<unsigned char> const &v1, std::vector<unsigned char> const &v2)
{
    std::cout << "v1:size : " << v1.size() << std::endl;
    std::cout << "v2:size : " << v2.size() << std::endl;

    std::cout << "v1:data : " << v1.data() << std::endl;
    std::cout << "v2:data : " << v2.data() << std::endl;

    return (v1.size() == v2.size() &&
            std::equal(v1.begin(), v1.end(), v2.begin()));
}

bool Utils::IsEqual(const std::string& v1, const std::string& v2)
{
    std::cout << "v1:size : " << v1.size() << std::endl;
    std::cout << "v2:size : " << v2.size() << std::endl;

    std::cout << "v1:data : " << v1.data() << std::endl;
    std::cout << "v2:data : " << v2.data() << std::endl;

    return (v1.size() == v2.size() &&
            std::equal(v1.begin(), v1.end(), v2.begin()));
}

Utils::~Utils()
{

}