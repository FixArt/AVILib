#ifndef AVILIB_USED_NUMERIC_IOSTREAM
#pragma once 

//#include <cstdint>
#include <string>
#include <iostream>
// #include <utility>
// #include <type_traits>
// #include <errno.h>

#include "numeric.hpp"

/**
 * @brief Namespace of Alternative Vector (And pair... and shared_ptr... and other things?) Implementation Library.
 * 
 */
namespace AVIL
{
    template<size_t size>
    std::ostream& operator<<(std::ostream& stream, const auint_t<size>& number)
    {
        stream << (std::string)number;
        return stream;
    }

    template<size_t size>
    std::istream& operator>>(std::istream& stream, auint_t<size>& number)
    {
        std::string get;
        stream >> get;
        number = get;
        return stream;
    }
};
#define AVILIB_USED_NUMERIC_IOSTREAM 1
#endif

