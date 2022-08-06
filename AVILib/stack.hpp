#ifndef AVILIB_USED_STACK
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
     * @brief Stack realization based on vector.
     * 
     * @tparam type 
     */
    template<class type>
    struct stack
    {
        private:
        AVIL::vector<type> itself;
        public:
        stack() : itself{} {}

        /**
         * @brief Delete elements from stack.
         * 
         * @param pushedSize How much elements to delete.
         */
        void push(size_t pushedSize = 1)
        {
            itself.reduce(pushedSize); // All bound checks done in function, no need to twice checking.
        }

        /**
         * @brief Gives you control over current last element in stack;
         * 
         * @return type& Last variable reference.
         */
        type& get()
        {
            if(itself.size == 0) throw(EINVAL);
            return itself[itself.size - 1];
        }

        /**
         * @brief Gives you variable and removes it from stack.
         * 
         * @return type Variable returned.
         */
        type possess()
        {
            if(itself.size == 0) throw(EINVAL);
            type returned = std::move(itself[itself.size - 1]);
            itself.reduce(1);
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
            return itself[itself.size - 1];
        }

        /**
         * @brief Gets current size of stack.
         * 
         * @return size_t 
         */
        size_t size()
        {
            return itself.size;
        }

        /**
         * @brief Put element on stack.
         * 
         * @param placed Element placed on stack.
         */
        void put(const type& placed)
        {
            itself.append(placed);
        }

        ~stack(){ /*Nothing must be done, vector will do everything itself.*/ }

        operator type&()
        {
            return get();
        }

        operator const type&() const
        {
            return get();
        }
    };
};
#define AVILIB_USED_STACK 1
#endif 
