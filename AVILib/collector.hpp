#ifndef AVILIB_USED_COLLECTOR
// #include <type_traits>
#pragma once
#include <cstdint>
#include <cstdlib>
#include <errno.h>
#include "vector.hpp"

/**
 * @brief Namespace of Alternative Vector (And pair... and shared_ptr... and other things?) Implementation Library.
 * 
 */
namespace AVIL
{
    class collector
    {
        private:
            AVIL::vector<void*> allocated;
        public:

            /**
             * @brief Checks whenever specific pointer is part of garbage collector.
             * 
             * @param checkedPointer Pointer to check.
             * @return true This pointer allocated in garbage collector.
             * @return false This pointer isn't allocated in garbage collector.
             */
            bool contains(const void* const checkedPointer)
            {
                for(size_t i = 0; i < allocated.size; ++i)
                {
                    if(checkedPointer == allocated[i])
                    {
                        return true;
                    }
                }
                return false;
            }

            /**
             * @brief Regain control over a pointer.
             * 
             * @param index Index of regained block.
             * @return void* Regained block pointer.
             */
            void* regain(const size_t& index)
            {
                if(index >= allocated.size) return nullptr;
                return allocated[index];
            }

            size_t collectionSize()
            {
                return allocated.size;
            }

            collector(){}

            /**
             * @brief Malloc for garbage collector.
             * 
             * @param size Size of block.
             * @return void* Returned block.
             */
            void* cmalloc(size_t size)
            {
                if(size == 0)
                {
                    throw(EINVAL);
                    return nullptr;
                }
                void* processed = malloc(size);
                if(processed == nullptr)
                {
                    throw(ENOMEM);
                    return nullptr;
                }
                allocated.append(processed);
                return processed;
            }

            /**
             * @brief Calloc for garbage collector.
             * 
             * @param member Fill with which value.
             * @param size Size of block.
             * @return void* Returned block.
             */
            void* ccalloc(const size_t& member, const size_t& size)
            {
                if(size == 0)
                {
                    throw(EINVAL);
                    return nullptr;
                }
                void* processed = calloc(member, size);
                if(processed == nullptr)
                {
                    throw(ENOMEM);
                    return nullptr;
                }
                allocated.append(processed);
                return processed;
            }

            /**
            * @brief Realloc for garbage collector.
            * 
            * @param reallocated Reallocated pointer.
            * @param newSize New size for pointer.
            * @return void* Returned pointer. Nullptr if errors.
            */
            void* crealloc(void* reallocated, const size_t& newSize)
            {
                if(!contains(reallocated))
                {
                    throw(EINVAL);
                    return nullptr;
                }
                if(newSize == 0)
                {
                    free(reallocated);
                    allocated.fremove(reallocated);
                    return nullptr;
                }
                void* processed = realloc(reallocated, newSize);
                return processed;
            }

            /**
             * @brief Free for garbage collector.
             * 
             * @param freed Freed pointer.
             */
            void cfree(void* freed)
            {
                if(!contains(freed))
                {
                    throw(EINVAL);
                    return;
                }
                free(freed);
                allocated.fremove(freed);
            }

            ~collector()
            {
                for(size_t i = 0; i < allocated.size; ++i)
                {
                    if(allocated[i] != nullptr) free(allocated[i]);
                }
            }
    };

    /**
     * @brief Default garbage collector.
     * 
     */
    AVIL::collector default_collector;

    /**
     * @brief Malloc for default garbage collector.
     * 
     * @param size Size of block.
     * @return void* Returned block.
     */
    inline void* cmalloc(size_t size)
    {
        return default_collector.cmalloc(size);
    }

    /**
     * @brief Calloc for default garbage collector.
     * 
     * @param member Fill with which value.
     * @param size Size of block.
     * @return void* Returned block.
     */
    inline void* ccalloc(size_t member, size_t size)
    {
        return default_collector.ccalloc(member, size);
    }

    /**
     * @brief Realloc for default garbage collector.
     * 
     * @param reallocated Reallocated pointer.
     * @param newSize New size for pointer.
     * @return void* Returned pointer. Nullptr if errors.
     */
    inline void* crealloc(void* reallocated, size_t newSize)
    {
        return default_collector.crealloc(reallocated, newSize);
    }

    /**
     * @brief Free for default garbage collector.
     * 
     * @param freed Freed pointer.
     */
    inline void cfree(void* freed)
    {
        default_collector.cfree(freed);
    }

    /**
     * @brief Regain control over a pointer.
     * 
     * @param index Index of regained block.
     * @return void* Regained block pointer.
     */
    inline void* regain(const size_t& index)
    {
        return default_collector.regain(index);
    }
}; 
#define AVILIB_USED_COLLECTOR 1
#endif
