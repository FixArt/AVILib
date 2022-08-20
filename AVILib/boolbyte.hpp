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

        boolbyte(const bool first, const bool second = false, const bool third = false, const bool fourth = false, const bool fifth = false, const bool sixth = false, const bool seventh = false, const bool eight = false)
        : b1{first}, b2{second}, b3{third}, b4{fourth},
        b5{fifth}, b6{sixth}, b7{seventh}, b8{eight}
        {}

        boolbyte(const boolbyte& copied) = default;

        boolbyte(const AVIL::array<bool, 8>& copied) : b1{copied[0]}, b2{copied[1]}, b3{copied[2]}, b4{copied[3]}, b5{copied[4]}, b6{copied[5]}, b7{copied[6]}, b8{copied[7]} {}

        boolbyte(boolbyte&& copied) = default;

        boolbyte(const char& copied)
        {
            b1 = (copied) & 1;
            b2 = (copied >> 1) & 1;
            b3 = (copied >> 2) & 1;
            b4 = (copied >> 3) & 1;
            b5 = (copied >> 4) & 1;
            b6 = (copied >> 5) & 1;
            b7 = (copied >> 6) & 1;
            b8 = (copied >> 7) & 1;
        }

        bool get(const unsigned char& index) const
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

        struct reference
        {
            protected:

            friend boolbyte;

            boolbyte* const controlled;
            unsigned char choosen = 0;

            reference(boolbyte* encontrolled, const unsigned char& enchoosen) : controlled{encontrolled}, choosen{enchoosen} {}

            public:

            reference(const reference& copied) : controlled{copied.controlled}, choosen{copied.choosen} {}

            reference& operator=(const bool& copied)
            {
                if(choosen > 7) return *this;
                controlled->set(choosen, copied);
                return *this;
            }

            reference& operator=(const reference& copied)
            {
                choosen = copied.choosen;
                return *this;
            }

            reference& operator++()
            {
                choosen += 1;
                if(choosen > 7) choosen = 0;
                return *this;
            }

            reference& operator--()
            {
                if(choosen == 0) return *this;
                choosen -= 1;
                return *this;
            }

            bool operator*() const
            {
                if(choosen > 7) return false;
                return controlled->get(choosen);
            }

            operator bool() const
            {
                if(choosen > 7) return false;
                return controlled->get(choosen);
            }

            ~reference(){}
        };

        reference operator[](const unsigned char& index)
        {
            // switch(index)
            // {
            //     default: throw(EINVAL);
            //     case 0: return b1;
            //     case 1: return b2;
            //     case 2: return b3;
            //     case 3: return b4;
            //     case 4: return b5;
            //     case 5: return b6;
            //     case 6: return b7;
            //     case 7: return b8;
            // }
            if(index > 7) throw(EINVAL);
            else return {(boolbyte* const)this, index};
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