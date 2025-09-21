#include "organization.hpp"
#include <sstream>
#include <algorithm>
#include <cstring>
#include <stdexcept>
#include <regex>


bool validate_inn(const std::string& inn) {
    return inn.length() == 10 && std::all_of(inn.begin(), inn.end(), [](unsigned char c) { 
               return std::isdigit(c); 
           });
}

ReturnType<std::string>::type
organization_to_yaml(const std::string& instance_name, const Organization& org) {
    return instance_name + ":\n" +
           "  name: " + org.name + "\n" +
           "  inn: " + org.inn + "\n" +
           "  workers: " + std::to_string(org.workers) + "\n";
}


ReturnType<const char*>::type
organization_to_yaml(const char* instance_name, const Organization& org) {
    if (!instance_name) throw std::runtime_error("Null instance_name");
    std::string result = std::string(instance_name) + ":\n" +
                         "  name: " + org.name + "\n" +
                         "  inn: " + org.inn + "\n" +
                         "  workers: " + std::to_string(org.workers) + "\n";
    char* out = new char[result.size() + 1];
    std::memcpy(out, result.c_str(), result.size() + 1);
    return out;
}


ReturnType<std::pair<const char*, std::size_t>>::type
organization_to_yaml(const char* instance_name, std::size_t len, const Organization& org) {
    if (!instance_name) throw std::runtime_error("Null instance_name");
    std::string result = std::string(instance_name, len) + ":\n" +
                         "  name: " + org.name + "\n" +
                         "  inn: " + org.inn + "\n" +
                         "  workers: " + std::to_string(org.workers) + "\n";
    char* out = new char[result.size() + 1];
    std::memcpy(out, result.c_str(), result.size() + 1);
    return out;
}

Organization organization_from_yaml(const std::string& yaml_str) {
    Organization org;
    std::istringstream stream(yaml_str);
    std::string line;
    while (std::getline(stream, line)) {
        if (line.empty() || line.find("  ") != 0) continue;
        std::string trimmed = line.substr(2);
        size_t colon_pos = trimmed.find(':');
        if (colon_pos == std::string::npos) continue;
        std::string key = trimmed.substr(0, colon_pos);
        std::string value = trimmed.substr(colon_pos + 1);
        value.erase(0, value.find_first_not_of(" \t"));
        value.erase(value.find_last_not_of(" \t") + 1);
        if (key == "name") {
            org.name = value;
        } else if (key == "inn") {
            if (!validate_inn(value)) {
                throw std::runtime_error("Invalid INN in YAML");
            }
            org.inn = value;
        } else if (key == "workers") {
            try {
                org.workers = std::stoi(value);
            } catch (const std::exception&) {
                throw std::runtime_error("Invalid workers in YAML");
            }
        }
    }
    
    if (org.name.empty() || org.inn.empty()) {
        throw std::runtime_error("Invalid YAML: missing required fields");
    }
    
    return org;
}

Organization organization_from_yaml(const char* yaml_str) {
    if (!yaml_str) throw std::runtime_error("Null YAML string");
    return organization_from_yaml(std::string(yaml_str));
}

Organization organization_from_yaml(const char* yaml_str, std::size_t size) {
    if (!yaml_str) throw std::runtime_error("Null YAML string");
    return organization_from_yaml(std::string(yaml_str, size));
}
