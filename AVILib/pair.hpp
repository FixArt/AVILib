#ifndef AVILIB_USED_PAIR
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
    template<class first, class second>
    struct pair
    {
        private:
            void* variable = nullptr;
            size_t currentType = 0;

            void resize(const size_t& newSize)
            {
                if(variable != nullptr)
                {
                    if(currentType == typeid(first).hash_code())
                    {
                        ((first*)variable)->~first();
                    }
                    else
                    {
                        ((second*)variable)->~second();
                    }
                    free(variable);
                    variable = nullptr;
                    if(newSize == 0) return;
                }
                variable = malloc(newSize);
                if(variable == nullptr) throw(ENOMEM);
            }

            void clean()
            {
                if(variable != nullptr)
                {
                    if(currentType == typeid(first).hash_code())
                    {
                        ((first*)variable)->~first();
                        // free((first*)variable);
                    }
                    else
                    {
                        ((second*)variable)->~second();
                        // free((second*)variable);
                    }
                    free(variable);
                    variable = nullptr;
                    currentType = 0;
                }
            }

            template<class type>
            void turnTo()
            {
                if(typeid(type) != typeid(first) and typeid(type) != typeid(second))
                {
                    throw(EINVAL);
                }
                resize(sizeof(type));
                new (&variable) type;
                currentType = typeid(type).hash_code();
            }
        public:
            pair<first, second>() : variable{nullptr}, currentType{0} {}

            const size_t& currentHash() const
            {
                return currentType;
            }

            template<class type>
            pair<first, second>(const type& assigned)
            {
                turnTo<type>();
                *((type*)variable) = (type&)assigned;
            }

            template<class alternativeFirst, class alternativeSecond>
            pair<first, second>(const pair<alternativeFirst, alternativeSecond>& copied)
            {
                if(typeid(first).hash_code() != copied.currentHash() and typeid(second).hash_code() != copied.currentHash())
                {
                    throw(EINVAL);
                }
                if(typeid(first).hash_code() == copied.currentHash())
                {
                    turnTo<first>();
                    *((first*)variable) = (first&)copied;
                }
                else
                {
                    turnTo<second>();
                    *((second*)variable) = (second&)copied;
                }
                currentType = copied.currentHash();
            }

            pair<first, second>(const pair<first, second>& copied)
            {
                if(typeid(first).hash_code() != copied.currentHash() and typeid(second).hash_code() != copied.currentHash())
                {
                    throw(EINVAL);
                }
                if(typeid(first).hash_code() == copied.currentHash())
                {
                    turnTo<first>();
                    *((first*)variable) = (first&)copied;
                }
                else
                {
                    turnTo<second>();
                    *((second*)variable) = (second&)copied;
                }
                currentType = copied.currentHash();
            }

            template<class type>
            pair<first, second> operator=(const type& assigned)
            {
                turnTo<type>();
                *((type*)variable) = (type&)assigned;
                return *this;
            }

            template<class alternativeFirst, class alternativeSecond>
            pair<first, second> operator=(const pair<alternativeFirst, alternativeSecond>& copied)
            {
                if(typeid(first).hash_code() != copied.currentHash() and typeid(second).hash_code() != copied.currentHash())
                {
                    throw(EINVAL);
                }
                if(typeid(first).hash_code() == copied.currentHash())
                {
                    turnTo<first>();
                    *((first*)variable) = (first&)copied;
                }
                else
                {
                    turnTo<second>();
                    *((second*)variable) = (second&)copied;
                }
                currentType = copied.currentHash();
                return *this;
            }

            pair<first, second> operator=(const pair<first, second>& copied)
            {
                if(typeid(first).hash_code() != copied.currentHash() and typeid(second).hash_code() != copied.currentHash())
                {
                    throw(EINVAL);
                }
                if(typeid(first).hash_code() == copied.currentHash())
                {
                    turnTo<first>();
                    *((first*)variable) = (first&)copied;
                }
                else
                {
                    turnTo<second>();
                    *((second*)variable) = (second&)copied;
                }
                currentType = copied.currentHash();
                return *this;
            }

            // template<class type>
            // operator type&()
            // {
            //     if(currentHash() != typeid(type).hash_code())
            //     {
            //         throw(EINVAL);
            //     }
            //     return *((type*)variable);
            // }

            // template<class type>
            // operator const type&() const
            // {
            //     if(currentType != typeid(type).hash_code())
            //     {
            //         throw(EINVAL);
            //     }
            //     return *((type*)variable);
            // }

            // template<class type>
            // operator type&()
            // {
            //     if(currentHash() == typeid(first).hash_code())
            //     {
            //         return *((first*)variable);
            //     }
            //     else if(currentHash() == typeid(second).hash_code())
            //     {
            //         return *((second*)variable);
            //     }
            //     else throw(EINVAL);
            // }

            // template<class type>
            // operator type&() const
            // {
            //     if(currentHash() == typeid(first).hash_code())
            //     {
            //         return *((first*)variable);
            //     }
            //     else if(currentHash() == typeid(second).hash_code())
            //     {
            //         return *((second*)variable);
            //     }
            //     else throw(EINVAL);
            // }

            operator first&()
            {
                if(currentHash() != typeid(first).hash_code())
                {
                    throw(EINVAL);
                }
                return *((first*)variable);
            }

            operator const first&() const
            {
                if(currentType != typeid(first).hash_code())
                {
                    throw(EINVAL);
                }
                return *((first*)variable);
            }

            operator second&()
            {
                if(currentHash() != typeid(second).hash_code())
                {
                    throw(EINVAL);
                }
                return *((second*)variable);
            }

            operator const second&() const
            {
                if(currentType != typeid(second).hash_code())
                {
                    throw(EINVAL);
                }
                return *((second*)variable);
            }

            ~pair<first, second>()
            {
                if(variable != nullptr)
                {
                    if(currentType == typeid(first).hash_code())
                    {
                        ((first*)variable)->~first();
                        // free((first*)variable);
                    }
                    else
                    {
                        ((second*)variable)->~second();
                        // free((second*)variable);
                    }
                    free(variable);
                }
            }
    };
};
#define AVILIB_USED_PAIR 1
#endif