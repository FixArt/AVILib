#ifndef AVILIB_USED_ITERATOR
#pragma once 

// #include <cmath>
// #include <initializer_list>
// #include <functional>
#include <algorithm>
#include <type_traits>

/**
 * @brief Namespace of Alternative Vector (And pair... and shared_ptr... and other things?) Implementation Library.
 * 
 */
namespace AVIL
{
    template<typename checked, typename = void>
    struct is_forward_iterator : std::false_type {};

    template<typename checked, typename = void>
    struct is_backward_iterator : std::false_type {};

    template<typename checked>
    struct is_forward_iterator<checked, std::void_t<decltype(++std::declval<checked&>()), decltype(*std::declval<checked&>()), decltype(std::declval<checked&>() == std::declval<checked&>())>> : std::true_type {};
    
    template<typename checked>
    struct is_backward_iterator<checked, std::void_t<decltype(--std::declval<checked&>()), decltype(*std::declval<checked&>()), decltype(std::declval<checked&>() == std::declval<checked&>())>> : std::true_type {};

    template<typename checked>
    struct is_complete_iterator
    {
        constexpr const static bool value = is_forward_iterator<checked>::value && is_backward_iterator<checked>::value;
    };

    template<typename checked>
    struct is_directioned_iterator
    {
        constexpr const static bool value = is_forward_iterator<checked>::value || is_backward_iterator<checked>::value;
    };

    template<typename reversed, typename = typename std::enable_if<is_directioned_iterator<reversed>::value>>
    struct reverse_iterator : public reversed
    {
        // private:
        // reversed itself{};
        public:

        reverse_iterator() = default;

        reverse_iterator(const reverse_iterator&) = default;

        reverse_iterator(reverse_iterator&&) = default;

        reverse_iterator& operator=(const reverse_iterator&) = default;

        reverse_iterator& operator=(const reversed& copied)
        {
            reversed::operator=(copied);
            return *this;
        }

        reverse_iterator& operator=(reverse_iterator&&) = default;

        ~reverse_iterator() = default;

        inline bool operator==(const reverse_iterator& compared) const
        {
            return reversed::operator==(compared.itself);
        }

        inline bool operator!=(const reverse_iterator& compared) const
        {
            return reversed::operator!=(compared.itself);
        }

        reverse_iterator& operator++()
        {
            reversed::operator++();
            return *this;
        }

        reverse_iterator& operator--()
        {
            reversed::operator--();
            return *this;
        }

        template<typename type>
        reverse_iterator& operator+=(const type& value)
        {
            reversed::operator+=(value);
            return *this;
        }

        template<typename type>
        reverse_iterator& operator-=(const type& value)
        {
            reversed::operator-=(value);
            return *this;
        }

        template<typename type>
        reverse_iterator operator+(const type& value) const
        {
            reverse_iterator<reversed> returned;
            returned.operator+=(value);
            return returned;
        }

        template<typename type>
        reverse_iterator operator-(const type& value) const
        {
            reverse_iterator<reversed> returned;
            returned.operator-=(value);
            return returned;
        }

        // decltype(*std::declval<reversed&>()) operator*()
        // {
        //     return reversed::operator*();
        // }
    };
};
#define AVILIB_USED_ITERATOR 1
#endif 
