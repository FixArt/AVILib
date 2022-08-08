#ifndef AVILIB_USED_OPTIONAL
#pragma once 

#include <utility>
#include <type_traits>
#include <errno.h>

/**
 * @brief Namespace of Alternative Vector (And pair... and shared_ptr... and other things?) Implementation Library.
 * 
 */
namespace AVIL
{
    template<class type>
    struct optional
    {
        private:
        type value;
        bool present : 1;
        public:
        optional() noexcept : present{false} {}

        optional(const type& copied) : value{copied}, present{true} {}

        optional& operator=(const type& copied)
        {
            value = copied;
            present = true;
            return *this;
        }

        optional& operator=(type&& copied)
        {
            value = std::move(copied);
            present = true;
            return *this;
        }

        bool exists() const noexcept
        {
            return present;
        }

        type& ensure(const type& ensured = {})
        {
            if(!present)
            {
                value = ensured;
                present = true;
            }
            return value;
        }

        type& value_or(const type& ensured = {})
        {
            return ensure(ensured);
        }

        optional(const optional&) = default;

        optional& operator=(const optional&) = default;

        optional(optional&& moved)
        {
            if(moved.present == true) value = std::move(moved.value);
            present = moved.present;
            moved.present = false;
        }

        optional& operator=(optional&& moved)
        {
            if(moved.present == true) value = std::move(moved.value);
            present = moved.present;
            moved.present = false;
            return *this;
        }

        ~optional() = default;

        explicit operator type() const noexcept
        {
            if(!present) return {};
            return value;
        }

        explicit operator type&()
        {
            if(!present) throw(EINVAL);
            return value;
        }

        explicit operator const type&() const
        {
            if(!present) throw(EINVAL);
            return value;
        }

        operator bool() const
        {
            return present;
        }
    };
};
#define AVILIB_USED_OPTIONAL 1
#endif
