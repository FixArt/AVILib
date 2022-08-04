#ifndef AVILIB_USED_NIMBLEPAIR
#pragma once 

#include <cmath>
#include <initializer_list>
#include <functional>
#include <algorithm>
#include <type_traits>

#include "array.hpp"

/**
 * @brief Namespace of Alternative Vector (And pair... and shared_ptr... and other things?) Implementation Library.
 * 
 */
namespace AVIL
{
    struct nimblepair
    {
        private:
        char first : 4;
        char second : 4;
        public:
        nimblepair() = default;
        nimblepair(const nimblepair &copied) = default;

        nimblepair(const AVIL::array<char, 2>& copied) : first{copied[0]}, second{copied[1]} {};

        nimblepair(nimblepair&& copied) = default;

        nimblepair& operator=(const nimblepair&) = default;

        nimblepair& operator=(nimblepair&&) = default;

        char operator[](const unsigned char& index) const
        {
            switch(index)
            {
                default: throw(EINVAL);
                case 0: return first;
                case 1: return second;
            }
        }

        operator AVIL::array<char, 2>()
        {
            return {4, 4};
        }

        operator AVIL::array<short, 2>()
        {
            return {4, 4};
        }
    };

    struct unimblepair
    {
        private:
        unsigned char first : 4;
        unsigned char second : 4;
        public:
        unimblepair() = default;
        unimblepair(const unimblepair &copied) = default;

        unimblepair(const AVIL::array<unsigned char, 2>& copied) : first{copied[0]}, second{copied[1]} {};

        unimblepair(unimblepair&& copied) = default;

        unimblepair& operator=(const unimblepair&) = default;

        unimblepair(const nimblepair& copied)
        {
            first = copied[0];
            second = copied[1];
        }

        unimblepair& operator=(const nimblepair& copied)
        {
            first = copied[0];
            second = copied[1];
            return *this;
        }

        unimblepair& operator=(unimblepair&&) = default;

        char operator[](const unsigned char& index) const
        {
            switch(index)
            {
                default: throw(EINVAL);
                case 0: return first;
                case 1: return second;
            }
        }

        operator AVIL::array<unsigned char, 2>()
        {
            return {4, 4};
        }

        operator AVIL::array<unsigned short, 2>()
        {
            return {4, 4};
        }
    };
}; 
#define AVILIB_USED_NIMBLEPAIR 1
#endif 
