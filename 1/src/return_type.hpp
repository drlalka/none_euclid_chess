#ifndef RETURN_TYPE_HPP
#define RETURN_TYPE_HPP

#include <string>
#include <utility> 


template<typename T>
struct ReturnType;


template<>
struct ReturnType<std::string> {
    using type = std::string;
    static type default_value;
};


template<>
struct ReturnType<const char*> {
    using type = char*;
    static type default_value;
};


template<>
struct ReturnType<std::pair<const char*, std::size_t>> {
    using type = char*;
    static type default_value;
};

#endif
