#ifndef AVILIB_USED_SQUEUE
#pragma once 

#include <cmath>
#include <initializer_list>
#include <functional>
#include <algorithm>
#include <type_traits>

#include "vector.hpp"

/**
 * @brief Namespace of Alternative Vector (And pair... and shared_ptr... and other things?) Implementation Library.
 * 
 */
namespace AVIL
{
    template<class type>
    struct squeue
    {
        private:

        struct squeuePair
        {
            size_t priority = 0;
            type stored;

            bool operator==(const squeuePair& processed) const
            {
                return stored == processed.stored;
            }

            bool operator!=(const squeuePair& processed) const
            {
                return stored != processed.stored;
            }

            bool operator>(const squeuePair& processed) const
            {
                return priority > processed.priority;
            }

            bool operator>=(const squeuePair& processed) const
            {
                return priority >= processed.priority;
            }

            bool operator<(const squeuePair& processed) const
            {
                return priority < processed.priority;
            }

            bool operator<=(const squeuePair& processed) const
            {
                return priority <= processed.priority;
            }
        };
        vector<squeuePair> itself;

        public:
        // Defaults appropriate. (AVIL::vector constructors called)
        squeue() = default;

        squeue(const squeue&) = default;

        squeue(squeue&&) = default;

        squeue& operator=(const squeue&) = default;

        squeue& operator=(squeue&&) = default;

        // ~squeue() = default; // This one placed in end of struct.

        void push(size_t pushedSize = 1)
        {
            itself.reduce(pushedSize); // All bound checks done in function, no need to twice checking.
        }

        squeue(const vector<type>& copied)
        {
            for(type placed : copied)
            {
                itself.append(squeuePair{0, placed});
            }
            // itself.stableSort();
            itself.sort();
        }

        operator vector<type>()
        {
            vector<type> returned;
            for(squeuePair processed : itself)
            {
                returned.append(processed.stored);
            }
            return returned;
        }

        squeue& operator=(const vector<type>& copied)
        {
            for(type placed : copied)
            {
                itself.append(squeuePair{0, placed});
            }
            // itself.stableSort();
            itself.sort();
            return *this;
        }

        void put(size_t priority, const type& placed)
        {
            itself.append(squeuePair{priority, placed});
            itself.selectionSort();
        }

        template<class... arguments>
        void put(size_t priority, const type& placed, arguments... other)
        {
            itself.append(squeuePair{priority, placed});
            itself.selectionSort();
            put(other...);
        }

        void put(const type& placed)
        {
            //itself.append({0, placed});
            itself.push(squeuePair{0, placed}, 0);
            // itself.selectionSort();
        }

        template<class... arguments>
        void put(const type& placed, arguments... other)
        {
            //itself.append({0, placed});
            itself.push(squeuePair{0, placed}, 0);
            // itself.selectionSort();
            put(other...);
        }

        type& getl()
        {
            return itself[0].stored;
        }

        type& geth()
        {
            return itself[itself.size - 1].stored;
        }

        const type& getl() const
        {
            return itself[0].stored;
        }

        const type& geth() const
        {
            return itself[itself.size - 1].stored;
        }

        type possessl()
        {
            if(itself.size == 0) throw(EINVAL);
            // type returned = std::move(itself[0]);
            type returned = itself[0];
            itself.reduce(1);
            return returned.stored;
        }

        type possessh()
        {
            if(itself.size == 0) throw(EINVAL);
            // type returned = std::move(itself[itself.size - 1]);
            type returned = itself[itself.size - 1];
            itself.reduce(1);
            return returned;
        }

        size_t size() const
        {
            return itself.size;
        }

        ~squeue() = default;
    };
}; 
#define AVILIB_USED_SQUEUE 1
#endif
