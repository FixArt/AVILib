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

        array() = default;

        array(std::initializer_list<type> from)
        {
            if(from.size() > size) return;
            std::copy(from.begin(), from.end(), begin());
        }

        // array(const array& copied)
        // {
        //     std::copy(copied.begin(), copied.end(), begin());
        // }

        array(const array& copied) = default;

        array(array&& copied) = default;

        type& operator[](const size_t& index)
        {
            if(index >= size) throw(EINVAL);
            return itself[index];
        }

        const type& operator[](const size_t& index) const
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
            
        const type* rbegin() const
        {
            return &itself[size - 1];
        }

        const type* rend() const
        {
            return itself - 1;
        }

        type* rbegin()
        {
            return &itself[size - 1];
        }

        type* rend()
        {
            return itself - 1;
        }
            
        const type* cbegin() const
        {
            return itself;
        }

        const type* cend() const
        {
            return itself + size;
        }
            
        const type* crbegin() const
        {
            return &itself[size - 1];
        }

        const type* crend() const
        {
            return itself - 1;
        }
    };
}; 
#define AVILIB_USED_ARRAY 1
#endif