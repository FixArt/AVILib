#include <new>
#ifndef AVILIB_USED_MEMUTILITARY
#pragma once 

#include <cstdlib>
#include <cstring>
#include <type_traits>

/**
 * @brief Namespace of Alternative Vector (And pair... and shared_ptr... and other things?) Implementation Library.
 * 
 */
namespace AVIL
{
    template<typename type, typename... arguments>
    inline type* altnew(arguments... initializers)
    {
        type* returned = (type*)malloc(sizeof(type));
        if(returned == nullptr)
        {
            throw(std::bad_alloc{});
        }
        if(!std::is_trivially_constructible<type>::value) new (returned) type{initializers...};
        return returned;
    }

    template<typename type, typename... arguments>
    inline type* altnewarray(const size_t& size, arguments... initializers)
    {
        type* returned = (type*)malloc(size * sizeof(type));
        if(returned == nullptr)
        {
            throw(std::bad_alloc{});
        }
        if(!std::is_trivially_constructible<type>::value) 
            for(size_t i = 0; i < size; ++i) new (returned + i) type{initializers...};
        return returned;
    }

    template<typename type>
    inline void altdelete(type* managed, const size_t& size)
    {
        for(size_t i = 0; i < size; ++i) managed[i].~type();
        free(managed);
    }

    template<typename type>
    inline void altresize(type*& managed, const size_t& from, const size_t& to)
    {
        if(from == to) return;
        if(from < to)
            for(size_t i = from; i < to; ++i) managed[i].~type();
        type* checked = (type*)realloc(managed, sizeof(type) * to);
        if(checked == nullptr)
        {
            checked = (type*)malloc(to * sizeof(type));
            if(checked == nullptr)
            {
                throw(std::bad_alloc{});
            }
            else
            {
                // memcpy(checked, managed, ((from < to)?(from):(to)) * sizeof(type));
                if(from < to)
                {
                    memcpy(checked, managed, from * sizeof(type));
                    for(size_t i = from; i < to; ++i) new (checked + i) type;
                    altdelete(managed, from);
                }
                else
                {
                    memcpy(checked, managed, to * sizeof(type));
                    // for(size_t i = from; i < to; ++i) new (checked + i) type;
                    free(managed);
                }
            }
            // altdelete(managed, from);
        }
        else if(from < to)
        {
            for(size_t i = from; i < to; ++i) new (checked + i) type;
        }
        managed = checked;
        // return returned;
    }

    template<typename from, typename to, typename... arguments>
    inline to* altreapply(from* transferred, arguments... initializers)
    {
        transferred->~from();
        to* returned = (to*)realloc(transferred, sizeof(to));
        if(returned == nullptr)
        {
            returned = (to*)malloc(sizeof(to));
            if(returned == nullptr)
            {
                throw(std::bad_alloc{});
            }
            free(transferred);
        }
        new (returned) to{initializers...};
        return returned;
    }
};
#define AVILIB_USED_MEMUTILITARY 1
#endif 
 
