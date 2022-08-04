#ifndef AVILIB_USED_ARRAY
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
            // void** allocated = nullptr;
            // size_t allocated.size = 0;
            
            // template<class type>
            // void addToList(type* &list, size_t size, type newElement)
            // {
            //     type* extendedList = (type*)malloc(sizeof(type) * (size + 1));
            //     for(size_t i = 0; i < size; ++i)
            //     {
            //         extendedList[i] = list[i];
            //     }
            //     free(list);
            //     extendedList[size] = newElement;
            //     list = extendedList;
            // }

            // template<class type>
            // void deleteFromList(type* &list, size_t size, type deletedElement)
            // {
            //     if(size == 0) return;
            //     if(size == 1)
            //     {
            //         if(list[0] == deletedElement)
            //         {
            //             free(list);
            //             list = nullptr;
            //         }
            //         return;
            //     }
            //     size_t j = 0;
            //     for(size_t i = 0; i < size; ++i)
            //     {
            //         if(deletedElement == list[i])
            //         {
            //             continue;
            //         }
            //         list[j] = list[i];
            //         ++j;
            //     }
            //     type* checkedList = (type*)realloc(list, sizeof(type) * (size - 1));
            //     if(checkedList == nullptr)
            //     {
            //         checkedList = (type*)malloc(sizeof(type) * (size - 1));
            //         for(size_t i = 0; i < size; ++i)
            //         {
            //             checkedList[i] = list[i];
            //         }
            //         free(list);
            //     }
            //     list = checkedList;
            // }
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
                return allocated[allocated.size - 1];
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
                return allocated[allocated.size - 1];
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
    AVIL::collector standart;

    /**
     * @brief Malloc for default garbage collector.
     * 
     * @param size Size of block.
     * @return void* Returned block.
     */
    inline void* cmalloc(size_t size)
    {
        return standart.cmalloc(size);
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
        return standart.ccalloc(member, size);
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
        return standart.crealloc(reallocated, newSize);
    }

    /**
     * @brief Free for default garbage collector.
     * 
     * @param freed Freed pointer.
     */
    inline void cfree(void* freed)
    {
        standart.cfree(freed);
    }

    /**
     * @brief Regain control over a pointer.
     * 
     * @param index Index of regained block.
     * @return void* Regained block pointer.
     */
    inline void* regain(const size_t& index)
    {
        return standart.regain(index);
    }
}; 
#define AVILIB_USED_ARRAY 1
#endif
