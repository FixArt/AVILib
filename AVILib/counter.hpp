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
    /**
     * Counter for pointers, which only can be sent by reference. Non explicit copying disallowed.
     */
    template<class countType = size_t>
    class counter
    {
        private:
            countType count = 0;

        public:
            // counter() : count(0) {}

            counter(countType settedCount = 0) : count(settedCount) {}

            explicit counter<countType>(const counter<countType>& copied)
            {
                count = copied.count;
            }

            counter& operator+=(const countType& number)
            {
                count += number;
                return *this;
            }

            counter& operator-=(const countType& number)
            {
                if(std::is_unsigned<countType>() and number >= count) count = 0;
                else count -= number;
                count -= number;
                return *this;
            }

            counter& operator*=(const countType& number)
            {
                count *= number;
                return *this;
            }

            counter& operator/=(const countType& number)
            {
                count /= number;
                return *this;
            }

            counter& operator=(const countType& number)
            {
                count = number;
                return *this;
            }

            bool operator==(const countType& number)
            {
                return count == number;
            }

            counter& operator++()
            {
                ++count;
                return *this;
            }

            counter& operator--()
            {
                if(!std::is_unsigned<countType>() or count != 0)
                {
                    --count;
                }
                return *this;
            }

            // operator countType()
            // {
            //     return count;
            // }

            operator const countType&()
            {
                return count;
            }
    };
}; 
