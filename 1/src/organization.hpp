#ifndef ORGANIZATION_HPP
#define ORGANIZATION_HPP

#include <string>
#include <cstddef>
#include "return_type.hpp"

struct Organization {
    std::string name;
    std::string inn;
    int workers{};
    void clear(){
        name = "";
        inn = "";
        workers = 0;
    }
};

ReturnType<std::string>::type
organization_to_yaml(const std::string& instance_name, const Organization& org);

ReturnType<const char*>::type
organization_to_yaml(const char* instance_name, const Organization& org);

ReturnType<std::pair<const char*, std::size_t>>::type
organization_to_yaml(const char* instance_name, std::size_t len, const Organization& org);

Organization organization_from_yaml(const std::string& yaml_str);
Organization organization_from_yaml(const char* yaml_str);
Organization organization_from_yaml(const char* yaml_str, std::size_t size);
bool validate_inn(const std::string& inn);

#endif
