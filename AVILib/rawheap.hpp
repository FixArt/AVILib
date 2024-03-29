#ifndef AVILIB_USED_RAWHEAP
#pragma once 

// #include <cmath>
// #include <initializer_list>
// #include <functional>
// #include <algorithm>
// #include <type_traits>
#include <cstring>
#include <cstdlib>
#include <errno.h>

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

        rawheap(const rawheap& copied)
        {
            resize(copied.size);
            if(copied.managed != nullptr)
            {
                memcpy(managed, copied.managed, (copied.size > size)?(size):(copied.size));
            }
        }

        rawheap(rawheap&& moved) : managed{moved.managed}, size{moved.size}
        {
            moved.managed = nullptr;
            moved.size = 0;
        }

        void resize(size_t newSize)
        {
            if(newSize == 0)
            {
                free(managed);
                managed = nullptr;
                size = 0;
                return;
            }
            void* processed = nullptr;
            if(managed != nullptr) processed = realloc(managed, newSize);
            if(processed == nullptr)
            {
                processed = malloc(newSize);
                if(processed == nullptr)
                {
                    throw(ENOMEM);
                }
                if(managed != nullptr)
                {
                    memcpy(processed, managed, (newSize > size)?(size):(newSize));
                    free(managed);
                }
            }
            managed = processed;
            size = newSize;
        }

        template<class type>
        type& operator[](size_t index)
        {
            if((index + sizeof(type)) >= size)
            {
                // throw(EINVAL);
                resize(index + sizeof(type));
                return *((type*)((char*)managed + index));
            }
            return *((type*)((char*)managed + index));
        }

        template<class type>
        type& operator[](size_t index) const
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
