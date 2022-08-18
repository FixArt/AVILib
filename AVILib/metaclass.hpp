#include <cmath>
#ifndef AVILIB_USED_NUMERIC
#pragma once 

//#include <cstdint>
#include <bitset>
#include <string>
#include <tuple>
#include <array>
#include "boolbyte.hpp"
#include "vector.hpp"
// #include <iostream>
// #include <utility>
// #include <type_traits>
// #include <errno.h>

#error Do not use this header, under development and unstable.

/**
 * @brief Namespace of Alternative Vector (And pair... and shared_ptr... and other things?) Implementation Library.
 * 
 */
namespace AVIL
{
    // namespace metaclass_parts
    // {
    //     template<size_t index, typename... types>
    //     struct getType
    //     {
    //         using type = char;
    //         static_assert(true, "Index out of bounds");
    //     };

    //     template<size_t index, typename processedType, typename... types>
    //     struct getType<index, std::tuple<processedType, types...>>
    //     {
    //         using type = typename getType<index - 1, std::tuple<types...>>::type;
    //     };

    //     template<typename processedType, typename... types>
    //     struct getType<0, std::tuple<processedType, types...>>
    //     {
    //         using type = processedType;
    //     };
    // };

    template<typename... types>
    class metaclass
    {
        private:

        std::tuple<types...> stored;

        template<size_t index, typename processedType>
        constexpr inline void elementsSetHelper(const processedType& currentElement)
        {
            std::get<index>(stored) = currentElement;
        }

        template<size_t index, typename processedType, typename... processedTypes>
        constexpr void elementsSetHelper(const processedType& currentElement, processedTypes... nextElements)
        {
            std::get<index>(stored) = currentElement;
            elementsSetHelper<index + 1, processedTypes...>(nextElements...);
        }

        template<typename processedType>
        static constexpr inline void bytesizeHelper(size_t& controlled)
        {
            controlled += sizeof(processedType);
        }

        template<typename processedType, typename additionalType, typename... processedTypes>
        static constexpr void bytesizeHelper(size_t& controlled)
        {
            controlled += sizeof(processedType);
            bytesizeHelper<additionalType, processedTypes...>(controlled);
        }

        // static constexpr inline size_t static_bytesize()
        // {
        //     size_t returned = 0;
        //     bytesizeHelper<types...>(returned);
        //     return returned;
        // }

        public:

        // template<size_t index>
        // typename metaclass_parts::getType<index, types...>::type& get()
        // {
        //     return std::get<index>(stored);
        // }

        template<size_t index>
        typename std::tuple_element<index, decltype(stored)>::type& get()
        {
            return std::get<index>(stored);
        }

        // template<size_t index>
        // typename const std::tuple_element<index, decltype(stored)>::type& get() const
        // {
        //     return std::get<index>(stored);
        // }

        template<typename... other>
        metaclass(other... setElements)
        {
            elementsSetHelper<0, other...>(setElements...);
        }

        metaclass(types... setElements)
        {
            elementsSetHelper<0, types...>(setElements...);
        }

        metaclass() = default;

        metaclass(const metaclass&) = default;

        metaclass(metaclass&&) = default;

        metaclass& operator=(const metaclass&) = default;

        metaclass& operator=(metaclass&&) = default;

        std::string string() const
        {

        }

        inline operator std::string() const
        {
            return string();
        }

        ~metaclass() = default;

        static constexpr inline size_t elements()
        {
            return std::tuple_size<decltype(stored)>().value;
        }

        static constexpr inline size_t bytesize()
        {
            size_t returned = 0;
            bytesizeHelper<types...>(returned);
            return returned;
        }

        private:

        template<size_t index>
        constexpr void bitsetConversionHelper(std::bitset<bytesize()> current, size_t& placement)
        {
            if(placement + sizeof(std::tuple_element<index, decltype(stored)>::type) >= bytesize())
            {
                return;
            }
            std::array<boolbyte, sizeof(std::tuple_element<index, decltype(stored)>::type)> put;
            std::copy((boolbyte*)(&std::get<index>(stored)), (boolbyte*)(&std::get<index>(stored)) + 1, put.begin());
            for(boolbyte checked : put)
            {
                for(size_t i = 0; i < 8; ++i)
                {
                    current[placement + i] = checked[i];
                }
                ++placement;
            }
            bitsetConversionHelper<index + 1>(current, placement + sizeof(std::tuple_element<index, decltype(stored)>::type));
        }

        // template<size_t index, typename = typename std::enable_if<std::is_trivial<typename std::tuple_element<index, decltype(stored)>::type>{}.value>>
        // constexpr void bitsetConversionHelper(std::bitset<bytesize()> current, size_t& placement)
        // {
        //     if(placement + sizeof(std::tuple_element<index, decltype(stored)>::type) >= bytesize())
        //     {
        //         return;
        //     }
        //     std::array<boolbyte, sizeof(std::tuple_element<index, decltype(stored)>::type)> put;
        //     std::copy((boolbyte*)(&std::get<index>(stored)), (boolbyte*)(&std::get<index>(stored)) + 1, put.begin());
        //     for(boolbyte checked : put)
        //     {
        //         for(size_t i = 0; i < 8; ++i)
        //         {
        //             current[placement + i] = checked[i];
        //         }
        //         ++placement;
        //     }
        //     bitsetConversionHelper<index + 1>(current, placement + sizeof(std::tuple_element<index, decltype(stored)>::type));
        // }

        public:
        operator std::bitset<bytesize()>() const
        {
            std::bitset<bytesize()> returned;
            return returned;
        }

        operator vector<bool>()
        {

        }
    };

    template<>
    struct metaclass<>
    {
        metaclass() = default;

        metaclass(const metaclass&) = default;

        metaclass(metaclass&&) = default;

        metaclass& operator=(const metaclass&) = default;

        metaclass& operator=(metaclass&&) = default;

        ~metaclass() = default;

        inline std::string string() const
        {
            return "";
        }

        inline operator std::string() const
        {
            return string();
        }

        static constexpr inline size_t elements()
        {
            return 0;
        }

        static constexpr inline size_t bytesize()
        {
            return 0;
        }

        operator vector<bool>()
        {
            return {};
        }
    };
};
#define AVILIB_USED_NUMERIC 1
#endif
