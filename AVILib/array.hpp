#include <stdexcept>
#ifndef AVILIB_USED_ARRAY
#pragma once 

#include <cmath>
// #include <initializer_list>
#include <functional>
#include <algorithm>
#include <type_traits>
#include <exception>

#include "iterator.hpp"

/**
 * @brief Namespace of Alternative Vector (And pair... and shared_ptr... and other things?) Implementation Library.
 * 
 */
namespace AVIL
{
    template<typename type, const size_t size>
    struct array
    {
        private:

        type itself[size];

        public:

        array() = default;

        ~array() = default;

        template<size_t otherSize>
        array<type, size + otherSize> operator+(const array<type, otherSize>& other)
        {
            array<type, size + otherSize> returned;
            std::copy(begin(), end(), returned.begin());
            std::copy(other.begin(), other.end(), returned.begin() + size);
            return returned;
        }

        // template<typename = typename std::enable_if<size != 1>>
        // array(const type& initializer)
        // {
        //     std::fill(itself[0], itself[size], initializer);
        // }

        array(std::initializer_list<type> from)
        {
            // if(from.size() == 1) std::fill(itself[0], itself[size], from);
            if(from.size() > size) return;
            std::copy(from.begin(), from.end(), begin());
        }

        // array(const type(&from)[size])
        // {
        //     std::copy(from[0], from[size], begin());
        // }

        template<size_t theSize, typename = typename std::enable_if<(theSize <= size)>>
        array(const type(&from)[theSize])
        {
            if(from.size() == 1) std::fill(itself[0], itself[size], from[0]);
            std::copy(from[0], from[theSize], begin());
        }

        // array(const array& copied)
        // {
        //     std::copy(copied.begin(), copied.end(), begin());
        // }

        array(const array& copied) = default;

        array(array&& copied) = default;

        array& operator=(const array& copied) = default;

        array& operator=(array&& copied) = default;

        constexpr inline type& at(const size_t& index)
        {
            if(index >= size) throw((std::string)"Attempted illegal access at " + std::to_string(index) + " in container of size " + std::to_string(size) + ".\n");
            return itself[index];
        }

        constexpr inline const type& at(const size_t& index) const
        {
            if(index >= size) throw((std::string)"Attempted illegal access at " + std::to_string(index) + " in container of size " + std::to_string(size) + ".\n");
            return itself[index];
        }

        constexpr inline type& operator[](const size_t& index)
        {
            return at(index);
        }

        constexpr inline const type& operator[](const size_t& index) const
        {
            return at(index);
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
            
        // reverse_iterator<const type*> rbegin() const
        // {
        //     return &itself[size - 1];
        // }

        // reverse_iterator<const type*> rend() const
        // {
        //     return itself - 1;
        // }

        // reverse_iterator<type*> rbegin()
        // {
        //     return &itself[size - 1];
        // }

        // reverse_iterator<type*> rend()
        // {
        //     return itself - 1;
        // }
            
        const type* cbegin() const
        {
            return itself;
        }

        const type* cend() const
        {
            return itself + size;
        }
            
        reverse_iterator<const type*> crbegin() const
        {
            return &itself[size - 1];
        }

        reverse_iterator<const type*> crend() const
        {
            return itself - 1;
        }
    };

    template<typename type>
    struct array<type, 0>;
}; 
#define AVILIB_USED_ARRAY 1
#endif