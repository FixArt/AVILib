#ifndef AVILIB_USED_ARRAY
#pragma once 

#include <cmath>
#include <initializer_list>
#include <functional>
#include <algorithm>
#include <type_traits>

/**
 * @brief Namespace of Alternative Vector (And pair... and shared_ptr... and other things?) Implementation Library.
 * 
 */
namespace AVIL
{
    template<class type, const size_t size>
    struct array
    {
        private:

        type itself[size];

        public:

        array(){}

        array(std::initializer_list<type> from)
        {
            if(from.size() > size) return;
            std::copy(from.begin(), from.end(), begin());
        }

        array(const array& copied)
        {
            std::copy(copied.begin(), copied.end(), begin());
        }

        type& operator[](const size_t& index)
        {
            if(index >= size) throw(EINVAL);
            return itself[index];
        }
            
        const type* begin() const
        {
            return itself;
        }

        const type* end() const
        {
            return itself + size;
        }

        type* begin()
        {
            return itself;
        }

        type* end()
        {
            return itself + size;
        }
    };
}; 
#define AVILIB_USED_ARRAY 1
#endif