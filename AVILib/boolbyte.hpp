#ifndef AVILIB_USED_BOOLBYTE
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
    struct boolbyte
    {
        private:
        bool b1 : 1;
        bool b2 : 1;
        bool b3 : 1;
        bool b4 : 1;
        bool b5 : 1;
        bool b6 : 1;
        bool b7 : 1;
        bool b8 : 1;
        public:
        boolbyte() = default;

        boolbyte(const bool* const boolArray) : b1{boolArray[0]}, b2{boolArray[1]}, b3{boolArray[2]}, b4{boolArray[3]}, b5{boolArray[4]}, b6{boolArray[5]}, b7{boolArray[6]}, b8{boolArray[7]} {}

        boolbyte(const boolbyte& copied) = default;

        boolbyte(boolbyte&& copied) = default;

        bool get(const unsigned char& index, const bool& value)
        {
            switch(index)
            {
                default: throw(EINVAL);
                case 0: return b1;
                case 1: return b2;
                case 2: return b3;
                case 3: return b4;
                case 4: return b5;
                case 5: return b6;
                case 6: return b7;
                case 7: return b8;
            }
        }

        void set(const unsigned char& index, const bool& value)
        {
            switch(index)
            {
                default: throw(EINVAL);
                case 0: b1 = value; break;
                case 1: b2 = value; break;
                case 2: b3 = value; break;
                case 3: b4 = value; break;
                case 4: b5 = value; break;
                case 5: b6 = value; break;
                case 6: b7 = value; break;
                case 7: b8 = value; break;
            }
        }

        bool operator[](const unsigned char& index) const
        {
            switch(index)
            {
                default: throw(EINVAL);
                case 0: return b1;
                case 1: return b2;
                case 2: return b3;
                case 3: return b4;
                case 4: return b5;
                case 5: return b6;
                case 6: return b7;
                case 7: return b8;
            }
        }

        // #ifdef AVILIB_USED_ARRAY
        operator AVIL::array<bool, 8>()
        {
            AVIL::array<bool, 8> returned{b1, b2, b3, b4, b5, b6, b7, b8};
            return returned;
        }
        // #endif
    };
};
#define AVILIB_USED_BOOLBYTE 1
#endif