#ifndef AVILIB_USED_HEAP
#pragma once 

// #include <cmath>
// #include <initializer_list>
// #include <functional>
// #include <algorithm>
// #include <type_traits>
#include <cstring>
#include <cstdlib>
#include <errno.h>

#include "vector.hpp"

/**
 * @brief Namespace of Alternative Vector (And pair... and shared_ptr... and other things?) Implementation Library.
 * 
 */
namespace AVIL
{
    /**
     * @brief Heap realization based on vector.
     * 
     * @tparam type 
     */
    template<class type>
    struct heap
    {
        private:
        AVIL::vector<type> itself;
        public:
        heap() : itself{} {}

        heap(const AVIL::vector<type>& copied) : itself{copied} {}

        heap(const std::initializer_list<type>& copied) : itself{copied} {}

        /**
         * @brief Delete elements from heap.
         * 
         * @param pushedSize How much elements to delete.
         */
        void push(size_t pushedSize = 1)
        {
            itself.reduce(pushedSize); // All bound checks done in function, no need to twice checking.
        }

        /**
         * @brief Gives you control over current last element in heap;
         * 
         * @return type& Last variable reference.
         */
        type& get()
        {
            if(itself.size == 0) throw(EINVAL);
            return itself[0];
        }

        /**
         * @brief Gives you variable and removes it from heap.
         * 
         * @return type Variable returned.
         */
        type possess()
        {
            if(itself.size == 0) throw(EINVAL);
            type returned = std::move(itself[0]);
            itself.pop(0);
            return returned;
        }

        /**
         * @brief Get unchangeable reference to variable.
         * 
         * @return const type& The reference.
         */
        const type& get() const
        {
            if(itself.size == 0) throw(EINVAL);
            return itself[0];
        }

        /**
         * @brief Gets current size of heap.
         * 
         * @return size_t 
         */
        size_t size()
        {
            return itself.size;
        }

        /**
         * @brief Put element on heap.
         * 
         * @param placed Element placed on heap.
         */
        void put(const type& placed)
        {
            itself.append(placed);
        }

        ~heap(){ /*Nothing must be done, vector will do everything itself.*/ }

        operator type&()
        {
            return get();
        }

        operator const type&() const
        {
            return get();
        }

        operator AVIL::vector<type>()
        {
            return itself;
        }
    };
};
#define AVILIB_USED_HEAP 1
#endif 
