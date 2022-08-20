#ifndef AVILIB_USED_SERIALIZABLE
#pragma once 

//#include <cstdint>
#include <bitset>
#include <string>
#include <tuple>
#include <array>
#include "boolbyte.hpp"
// #include "vector.hpp"
#include <bit>
#include <cmath>
#include <cstring>
#include <type_traits>
// #include <iostream>
// #include <utility>
// #include <type_traits>
// #include <errno.h>

// #warning Do not use this header, under development and unstable.

/**
 * @brief Namespace of Alternative Vector (And pair... and shared_ptr... and other things?) Implementation Library.
 * 
 */
namespace AVIL
{
    // template<typename first, typename second>
    // struct is_interconvertible
    // {
    //     constexpr static const bool value = (std::is_same<first, second>{}.value) || (std::is_convertible<const first&, second>{}.value && std::is_convertible<const second&, first>{}.value);
    // };

    // template<typename checked, typename... types>
    // struct is_serializable_helper
    // {
    //     constexpr static const bool value = is_serializable_helper<checked>{}.value && is_serializable_helper<types...>{}.value;
    // };

    // template<typename checked>
    // struct is_serializable_helper<checked>
    // {
    //     constexpr static const bool value = std::is_trivially_copyable<checked>{}.value
    //     || is_interconvertible<vector<bool>, checked>{}.value
    //     || is_interconvertible<std::string, checked>{}.value
    //     || (std::is_convertible<checked, std::string>{}.value && std::is_convertible<const char* const, checked>{}.value);
    // };

    // template<typename... types>
    // using is_serializable = is_serializable_helper<types...>;

    template<typename... types>
    struct metaclass
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

        protected:

        template<size_t index>
        typename std::tuple_element<index, decltype(stored)>::type& get()
        {
            return std::get<index>(stored);
        }

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
        constexpr void bitsetConversionHelper(std::bitset<bytesize()>& current, size_t& placement) const
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

        protected:
        operator std::bitset<bytesize()>() const
        {
            std::bitset<bytesize()> returned;
            size_t placement = 0;
            bitsetConversionHelper<0>(returned, placement);
            return returned;
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

        static constexpr inline size_t elements()
        {
            return 0;
        }

        static constexpr inline size_t bytesize()
        {
            return 0;
        }
    };

    template<typename... types>
    struct metastruct
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
        metastruct(other... setElements)
        {
            elementsSetHelper<0, other...>(setElements...);
        }

        metastruct(types... setElements)
        {
            elementsSetHelper<0, types...>(setElements...);
        }

        metastruct() = default;

        metastruct(const metastruct&) = default;

        metastruct(metastruct&&) = default;

        metastruct& operator=(const metastruct&) = default;

        metastruct& operator=(metastruct&&) = default;

        std::string string() const
        {

        }

        inline operator std::string() const
        {
            return string();
        }

        ~metastruct() = default;

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
        constexpr void bitsetConversionHelper(std::bitset<bytesize()>& current, size_t& placement) const
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

        // template<size_t index, typename = typename std::enable_if<std::is_trivially_copyable<typename std::tuple_element<index, decltype(stored)>::type>{}.value>>
        // constexpr void vectorConversionHelper(vector<bool>& current, size_t& placement) const
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
        //     vectorConversionHelper<index + 1>(current, placement + sizeof(std::tuple_element<index, decltype(stored)>::type));
        // }

        // template<size_t index, typename = typename std::enable_if<is_interconvertible<vector<bool>, typename std::tuple_element<index, decltype(stored)>::type>{}.value>,
        // typename = typename std::enable_if<!std::is_trivially_copyable<typename std::tuple_element<index, decltype(stored)>::type>{}.value>>
        // void vectorConversionHelper(vector<bool>& current, size_t& placement) const
        // {
        //     if(placement + sizeof(std::tuple_element<index, decltype(stored)>::type) >= bytesize())
        //     {
        //         return;
        //     }
        //     vector<bool> put = std::get<index>();
        //     std::array<bool, sizeof(size_t)> temporary;
        //     for(size_t i = 0; i < sizeof(size_t) / 8; ++i)
        //     {
        //         temporary[i] = (put.size >> i) & 1;
        //     }
        //     for(bool placed : temporary)
        //     {
        //         current[placement] = placed;
        //         ++placement;
        //     }
        //     for(bool checked : put)
        //     {
        //         current[placement] = checked;
        //         ++placement;
        //     }
        //     vectorConversionHelper<index + 1>(current, placement + sizeof(std::tuple_element<index, decltype(stored)>::type));
        // }

        // template<size_t index, typename = typename std::enable_if<is_interconvertible<std::string, typename std::tuple_element<index, decltype(stored)>::type>{}.value || (std::is_convertible<typename std::tuple_element<index, decltype(stored)>::type, std::string>{}.value && std::is_convertible<const char* const, typename std::tuple_element<index, decltype(stored)>::type>{}.value)>,
        // typename = typename std::enable_if<!is_interconvertible<vector<bool>, typename std::tuple_element<index, decltype(stored)>::type>{}.value>,
        // typename = typename std::enable_if<!std::is_trivially_copyable<typename std::tuple_element<index, decltype(stored)>::type>{}.value>>
        // void vectorConversionHelper(vector<bool>& current, size_t& placement) const
        // {
        //     if(placement + sizeof(std::tuple_element<index, decltype(stored)>::type) >= bytesize())
        //     {
        //         return;
        //     }
        //     std::string put = std::get<index>();
        //     std::array<bool, sizeof(size_t)> temporary;
        //     for(size_t i = 0; i < sizeof(size_t) / 8; ++i)
        //     {
        //         temporary[i] = (put.size() >> i) & 1;
        //     }
        //     for(bool placed : temporary)
        //     {
        //         current[placement] = placed;
        //         ++placement;
        //     }
        //     for(char checked : put)
        //     {
        //         if(checked == '\0') break;
        //         for(size_t i = 0; i < 8; ++i)
        //         {
        //             current[placement + i] = ((boolbyte)checked)[i];
        //         }
        //         placement += 8;
        //     }
        //     vectorConversionHelper<index + 1>(current, placement + sizeof(std::tuple_element<index, decltype(stored)>::type));
        // }

        // template<size_t index, typename = typename std::enable_if<!(is_interconvertible<std::string, typename std::tuple_element<index, decltype(stored)>::type>{}.value || (std::is_convertible<typename std::tuple_element<index, decltype(stored)>::type, std::string>{}.value && std::is_convertible<const char* const, typename std::tuple_element<index, decltype(stored)>::type>{}.value))>,
        // typename = typename std::enable_if<!is_interconvertible<vector<bool>, typename std::tuple_element<index, decltype(stored)>::type>{}.value>,
        // typename = typename std::enable_if<!std::is_trivially_copyable<typename std::tuple_element<index, decltype(stored)>::type>{}.value>,
        // typename = typename std::enable_if<(is_interconvertible<const char* const, typename std::tuple_element<index, decltype(stored)>::type>{}.value && (typeid(const char* const) != typeid(typename std::tuple_element<index, decltype(stored)>::type))) || (std::is_convertible<typename std::tuple_element<index, decltype(stored)>::type, const char* const>{}.value && std::is_convertible<const std::string&, typename std::tuple_element<index, decltype(stored)>::type>{}.value)>>
        // void vectorConversionHelper(vector<bool>& current, size_t& placement) const
        // {
        //     if(placement + sizeof(std::tuple_element<index, decltype(stored)>::type) >= bytesize())
        //     {
        //         return;
        //     }
        //     const char* const put = std::get<index>();
        //     std::array<bool, sizeof(size_t)> temporary;
        //     for(size_t i = 0; i < sizeof(size_t) / 8; ++i)
        //     {
        //         temporary[i] = (strlen(put) >> i) & 1;
        //     }
        //     for(bool placed : temporary)
        //     {
        //         current[placement] = placed;
        //         ++placement;
        //     }
        //     for(size_t i = 0; i < strlen(put); ++i)
        //     {
        //         char checked = put[i];
        //         for(size_t j = 0; j < 8; ++j)
        //         {
        //             current[placement + j] = ((boolbyte)checked)[j];
        //         }
        //         placement += 8;
        //     }
        //     vectorConversionHelper<index + 1>(current, placement + sizeof(std::tuple_element<index, decltype(stored)>::type));
        // }

        public:
        operator std::bitset<bytesize()>() const
        {
            std::bitset<bytesize()> returned;
            size_t placement = 0;
            bitsetConversionHelper<0>(returned, placement);
            return returned;
        }

        // constexpr bool reformable() const
        // {
        //     return;
        // }

        // template<typename = typename std::enable_if<is_serializable<types...>{}.value>>
        // operator vector<bool>() const
        // {
        //     if(std::is_trivially_copyable<decltype(*this)>())
        //     {
        //         std::bitset<bytesize()> converted = operator std::bitset<bytesize()>();
        //         vector<bool> returned;
        //         returned.resize(bytesize());
        //         for(size_t i = 0; i < bytesize(); ++i)
        //         {
        //             returned[i] = converted[i];
        //         }
        //         return returned;
        //     }
        //     else
        //     {
        //         vector<bool> returned;
        //         size_t placement = 0;
        //         // vectorConversionHelper<0>(returned, placement);
        //         return returned;
        //     }
        // }
    };

    template<>
    struct metastruct<>
    {
        metastruct() = default;

        metastruct(const metastruct&) = default;

        metastruct(metastruct&&) = default;

        metastruct& operator=(const metastruct&) = default;

        metastruct& operator=(metastruct&&) = default;

        ~metastruct() = default;

        static constexpr inline size_t elements()
        {
            return 0;
        }

        static constexpr inline size_t bytesize()
        {
            return 0;
        }
    };
};
#define AVILIB_USED_SERIALIZABLE 1
#endif
