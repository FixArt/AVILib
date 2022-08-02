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
     * @brief Variant implementation.
     * 
     * @tparam types Types of variant.
     */
    template<class... types>
    struct variant
    {
        private:
            void* variable = nullptr;
            size_t currentType = 0;

            // template<class checkedType>
            // bool availableIterator() const
            // {
            //     return false;
            // }

            template<class checkedType, class processedType>
            bool availableIterator() const
            {
                return typeid(processedType) == typeid(checkedType);
            }

            template<class checkedType, class processedType, class additionalType, class... availableTypes>
            bool availableIterator() const
            {
                //if(sizeof...(availableTypes) == 0) return typeid(processedType) == typeid(checkedType);
                return (typeid(processedType) == typeid(checkedType))?(true):(availableIterator<checkedType, additionalType, availableTypes...>());
            }

            template<class processedType>
            bool availableIteratorHash(const size_t& checkedHash) const
            {
                return typeid(processedType).hash_code() == checkedHash;
            }

            template<class processedType, class additionalType, class... availableTypes>
            bool availableIteratorHash(const size_t& checkedHash) const
            {
                // if(sizeof...(availableTypes) == 0) return typeid(processedType).hash_code() == checkedHash;
                return (typeid(processedType).hash_code() == checkedHash)?(true):(availableIteratorHash<additionalType, availableTypes...>(checkedHash));
            }

            // template<class processedType>
            // void assignIterator(const variant<types...>& copied)
            // {
            //     if(copied.currentHash() == typeid(processedType).hash_code())
            //     {
            //         turnTo<processedType>();
            //         *((processedType*)variable) = (processedType&)copied;
            //     }
            // }

            // template<class processedType, class additionalType, class... assignedTypes>
            // void assignIterator(const variant<types...>& copied)
            // {
            //     if(copied.currentHash() == typeid(processedType).hash_code())
            //     {
            //         turnTo<processedType>();
            //         *((processedType*)variable) = (processedType&)copied;
            //     }
            //     else
            //     {
            //         // if(sizeof...(assignedTypes) == 0) return;
            //         assignIterator<additionalType, assignedTypes...>(copied);
            //     }
            // }

            template<class processedType>
            void assignIterator(const variant& copied)
            {
                if(copied.currentHash() == typeid(processedType).hash_code())
                {
                    turnTo<processedType>();
                    *((processedType*)variable) = (processedType)copied;
                }
            }

            template<class processedType, class additionalType, class... assignedTypes>
            void assignIterator(const variant& copied)
            {
                if(copied.currentHash() == typeid(processedType).hash_code())
                {
                    turnTo<processedType>();
                    *((processedType*)variable) = (processedType)copied;
                }
                else
                {
                    // if(sizeof...(assignedTypes) == 0) return;
                    assignIterator<additionalType, assignedTypes...>(copied);
                }
            }

            // template<class... otherTypes, class processedType>
            // void assignIteratorOther(const variant<otherTypes...>& copied)
            // {
            //     if(copied.currentHash() == typeid(processedType).hash_code())
            //     {
            //         turnTo<processedType>();
            //         *((processedType*)variable) = (processedType&)copied;
            //     }
            // }

            // template<class... otherTypes, class processedType, class additionalType, class... assignedTypes>
            // void assignIteratorOther(const variant<otherTypes...>& copied)
            // {
            //     if(copied.currentHash() == typeid(processedType).hash_code())
            //     {
            //         turnTo<processedType>();
            //         *((processedType*)variable) = (processedType&)copied;
            //     }
            //     else
            //     {
            //         // if(sizeof...(assignedTypes) == 0) return; //So no conflict happened.
            //         assignIteratorOther<otherTypes..., additionalType, assignedTypes...>(copied);
            //     }
            // }

            template<class processedType>
            void cleanIterator()
            {
                if(typeid(processedType) == typeid(currentType))
                {
                    ((processedType*)variable)->~processedType();
                }
            }

            template<class processedType, class additionalOne, class... cleanedTypes>
            void cleanIterator()
            {
                if(typeid(processedType) == typeid(currentType))
                {
                    ((processedType*)variable)->~processedType();
                }
                else
                {
                    // if(sizeof...(cleanedTypes) == 0) return;
                    cleanIterator<additionalOne, cleanedTypes...>();
                }
            }

            void clean()
            {
                if(variable != nullptr)
                {
                    cleanIterator<types...>();
                    free(variable);
                    variable = nullptr;
                }
            }

            void resize(const size_t& newSize)
            {
                if(variable != nullptr)
                {
                    clean();
                    if(newSize == 0) return;
                }
                variable = malloc(newSize);
                if(variable == nullptr) throw(ENOMEM);
            }

            template<class type>
            void turnTo()
            {
                if(!isAvailable<type>())
                {
                    throw(EINVAL);
                }
                resize(sizeof(type));
                new (&variable) type;
                currentType = typeid(type).hash_code();
            }

            // void assign(const variant<types...>& copied)
            // {
            //     if(!isAvailable(copied.currentHash()))
            //     {
            //         throw(EINVAL);
            //     }
            //     assignIterator<types...>(copied);
            //     currentType = copied.currentHash();
            // }

            // template<class... otherTypes>
            // void assign(const variant<otherTypes...>& copied)
            // {
            //     if(!isAvailable(copied.currentHash()))
            //     {
            //         throw(EINVAL);
            //     }
            //     // assignIteratorOther<otherTypes..., types...>(copied);
            //     assignIterator<types...>(copied);
            //     currentType = copied.currentHash();
            // }

            void assign(const variant& copied)
            {
                if(!isAvailable(copied.currentHash()))
                {
                    throw(EINVAL);
                }
                assignIterator<types...>(copied);
                currentType = copied.currentHash();
            }

        public:
            template<class type>
            bool isAvailable() const
            {
                return availableIterator<type, types...>();
            }

            bool isAvailable(const size_t& checkedHash) const
            {
                return availableIteratorHash<types...>(checkedHash);
            }

            variant<types...>() : variable{nullptr}, currentType{0} {}

            const size_t& currentHash() const
            {
                return currentType;
            }

            template<class type>
            variant<types...>(const type& assigned)
            {
                turnTo<type>();
                *((type*)variable) = (type&)assigned;
            }

            // template<class... otherTypes>
            // variant<types...>(const variant<otherTypes...>& copied)
            // {
            //     // assign<otherTypes...>(copied);
            //     assign(copied);
            //     // currentType = copied.currentHash();
            // }

            // variant<types...>(const variant<types...>& copied)
            // {
            //     assign(copied);
            //     // currentType = copied.currentHash();
            // }

            variant(const variant& copied)
            {
                assign(copied);
                // currentType = copied.currentHash();
            }

            variant(variant&& assigned) : variable{assigned.variable}, currentType{assigned.currentType}
            {
                assigned.variable = nullptr;
                assigned.currentType = 0;
            }

            // variant(variant&& assigned)
            // {
            //     // assign(assigned);
            //     // assigned.clean();
            //     // currentType = copied.currentHash();
            //     variable = assigned.variable;
            //     currentType = assigned.currentType;
            //     assigned.variable = nullptr;
            //     assigned.currentType = 0;
            // }

            template<class type>
            variant<types...> operator=(const type& assigned)
            {
                turnTo<type>();
                *((type*)variable) = (type&)assigned;
                return *this;
            }

            // template<class... otherTypes>
            // variant<types...> operator=(const variant<otherTypes...>& copied)
            // {
            //     // assign<otherTypes...>(copied);
            //     assign(copied);
            //     // currentType = copied.currentHash();
            //     return *this;
            // }

            // variant<types...> operator=(const variant<types...>& copied)
            // {
            //     assign(copied);
            //     // currentType = copied.currentHash();
            //     return *this;
            // }

            variant operator=(const variant& copied)
            {
                assign(copied);
                // currentType = copied.currentHash();
                return *this;
            }

            template<class type>
            operator type&()
            {
                if(currentHash() != typeid(type).hash_code())
                {
                    throw(EINVAL);
                }
                return *((type*)variable);
            }

            template<class type>
            operator const type&() const
            {
                if(currentType != typeid(type).hash_code())
                {
                    throw(EINVAL);
                }
                return *((type*)variable);
            }

            ~variant<types...>()
            {
                clean();
            }
    };
};
