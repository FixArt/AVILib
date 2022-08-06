#ifndef AVILIB_USED_RAWHEAP
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
    /**
     * @brief Raw heap storage.
     * 
     */
    struct rawheap
    {
        private:
        void* managed;
        size_t size;
        public:
        rawheap() : managed{nullptr}, size{0} {}

        template<class type>
        type& operator[](size_t index)
        {
            if((index + sizeof(type)) >= size)
            {
                throw(EINVAL);
            }
            return *((type*)((char*)managed + index));
        }

        ~rawheap()
        {
            if(managed != nullptr) free(managed);
        }
    };
};
#define AVILIB_USED_RAWHEAP 1
#endif 
