#ifndef AVILIB_USED_SETS
#pragma once 

#include <cmath>
#include <initializer_list>
#include <functional>
#include <algorithm>
#include <type_traits>

#include "vector.hpp"

/// THIS ONE IN DEVELOPMENT

/**
 * @brief Namespace of Alternative Vector (And pair... and shared_ptr... and other things?) Implementation Library.
 * 
 */
namespace AVIL
{
    template<class type>
    struct unordered_set
    {
        private:
        AVIL::vector<type> itself;

        public:

        unordered_set(){}

        unordered_set(const unordered_set& copied) : itself{copied.itself} {}

        unordered_set(unordered_set&& moved) : itself{std::move(moved.itself)} {}

        unordered_set(std::initializer_list<type> copied) : itself{copied} {}

        unordered_set operator=(const std::initializer_list<type>& copied)
        {
            itself = copied;
        }

        unordered_set operator=(const unordered_set& copied)
        {
            itself = copied.itself;
        }

        unordered_set operator=(unordered_set&& copied)
        {
            itself = std::move(copied.itself);
        }

        bool contains(const type& key)
        {
            for(size_t i = 0; i < itself.size; ++i)
            {
                if(itself[i] == key)
                {
                    return true;
                }
            }
            return false;
        }

        void append(const type& key)
        {
            itself.append(key);
        }

        void remove(const type& key)
        {
            itself.fremove([key](const type& processed)
            {
                return processed == key;
            });
        }

        size_t where(const type& key)
        {
            for(size_t i = 0; i < itself.size; ++i)
            {
                if(itself[i] == key)
                {
                    return i;
                }
            }
            return itself.size;
        }

        type& operator[](const type& key)
        {
            size_t placement = where(key);
            if(placement == itself.size)
            {
                append(key);
            }
            return itself[placement];
        }

        const type& operator[](const type& key) const
        {
            size_t placement = where(key);
            if(placement == itself.size)
            {
                append(key);
            }
            return itself[placement].value;
        }

        const type* begin() const
        {
            return itself.begin();
        }

        const type* end() const
        {
            return itself.end();
        }

        type* begin()
        {
            return itself.begin();
        }

        type* end()
        {
            return itself.end();
        }
        
        const type* rbegin() const
        {
            return itself.rbegin();
        }

        const type* rend() const
        {
            return itself.rend();
        }

        type* rbegin()
        {
            return itself.rbegin();
        }

        type* rend()
        {
            return itself.rend();
        }
        
        const type* cbegin() const
        {
            return itself.cbegin();
        }

        const type* cend() const
        {
            return itself.cend();
        }
        
        const type* crbegin() const
        {
            return itself.crbegin();
        }
        
        const type* crend() const
        {
            return itself.crend();
        }
    };
};
#define AVILIB_USED_SETS 1
#endif 
