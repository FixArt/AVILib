#ifndef AVILIB_USED_MAPS
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
    template<class keytype, class valuetype>
    struct map_pair
    {
        keytype key;
        valuetype value;

        bool operator==(const map_pair& compared)
        {
            return key == compared.key and value == compared.value;
        }

        bool operator==(const keytype& compared)
        {
            return key == compared;
        }

        bool operator!=(const keytype& compared)
        {
            return key != compared;
        }

        bool operator>(const keytype& compared)
        {
            return key > compared;
        }

        bool operator<(const keytype& compared)
        {
            return key < compared;
        }

        bool operator>=(const keytype& compared)
        {
            return key >= compared;
        }

        bool operator<=(const keytype& compared)
        {
            return key <= compared;
        }
    };

    template<class keytype, class valuetype>
    struct unordered_map
    {
        private:
        AVIL::vector<map_pair<keytype, valuetype>> itself;

        public:

        unordered_map(){}

        unordered_map(const unordered_map& copied)
        {
            itself = copied.itself;
        }

        unordered_map(unordered_map&& moved)
        {
            itself = std::move(moved.itself);
        }

        unordered_map(std::initializer_list<map_pair<keytype, valuetype>> copied)
        {
            itself = copied;
        }

        unordered_map operator=(const std::initializer_list<map_pair<keytype, valuetype>>& copied)
        {
            itself = copied;
        }

        bool contains(const keytype& key)
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

        void append(const keytype& key, const valuetype& value)
        {
            itself.append({key, value});
        }

        void append(const keytype& key)
        {
            itself.append({key});
        }

        void remove(const keytype& key)
        {
            itself.fremove([key](const map_pair<keytype, valuetype>& processed)
            {
                return processed.key == key;
            });
        }

        void remove(const valuetype& value)
        {
            itself.fremove([value](const map_pair<keytype, valuetype>& processed)
            {
                return processed.value == value;
            });
        }

        size_t where(const keytype& key)
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

        valuetype& operator[](const keytype& key)
        {
            size_t placement = where(key);
            if(placement == itself.size)
            {
                append(key);
            }
            return itself[placement].value;
        }

        const valuetype& operator[](const keytype& key) const
        {
            size_t placement = where(key);
            if(placement == itself.size)
            {
                append(key);
            }
            return itself[placement].value;
        }

        const map_pair<keytype, valuetype>* begin() const
        {
            return itself.begin();
        }

        const map_pair<keytype, valuetype>* end() const
        {
            return itself.end();
        }

        map_pair<keytype, valuetype>* begin()
        {
            return itself.begin();
        }

        map_pair<keytype, valuetype>* end()
        {
            return itself.end();
        }
        
        const map_pair<keytype, valuetype>* rbegin() const
        {
            return itself.rbegin();
        }

        const map_pair<keytype, valuetype>* rend() const
        {
            return itself.rend();
        }

        map_pair<keytype, valuetype>* rbegin()
        {
            return itself.rbegin();
        }

        map_pair<keytype, valuetype>* rend()
        {
            return itself.rend();
        }
        
        const map_pair<keytype, valuetype>* cbegin() const
        {
            return itself.cbegin();
        }

        const map_pair<keytype, valuetype>* cend() const
        {
            return itself.cend();
        }
        
        const map_pair<keytype, valuetype>* crbegin() const
        {
            return itself.crbegin();
        }
        
        const map_pair<keytype, valuetype>* crend() const
        {
            return itself.crend();
        }
    };
};
#define AVILIB_USED_MAPS 1
#endif 
