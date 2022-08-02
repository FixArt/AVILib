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
     * @brief Variable without specified type only for trivial types.
     * 
     */
    struct untypizedtrivial
    {
        private:
            void* variable = nullptr;
            size_t capacity = 0;

            void resize(const size_t& newCapacity)
            {
                if(newCapacity == capacity) return;
                if(newCapacity == 0)
                {
                    free(variable);
                    variable = nullptr;
                    capacity = 0;
                    return;
                }

                if(variable != nullptr)
                {
                    void* newVariable = realloc(variable, newCapacity);
                    if(newVariable == nullptr)
                    {
                        // free(variable);
                        variable = malloc(newCapacity);
                        if(variable == nullptr) throw(ENOMEM);
                    }
                    else
                    {
                        variable = newVariable;
                    }
                }
                else
                {
                    // free(variable);
                    variable = malloc(newCapacity);
                    if(variable == nullptr) throw(ENOMEM);
                }
                capacity = newCapacity;
            }
        public:

            const size_t& size = capacity;
            
            untypizedtrivial()
            {
                capacity = 0;
                variable = nullptr;
                resize(0);
            }

            template<class type>
            untypizedtrivial(const type& value)
            {
                if(!std::is_trivial<type>().value) throw(EINVAL);
                resize(sizeof(value));
                *((type*)variable) = (type&)value;
            }

            template<class type>
            untypizedtrivial(type&& value)
            {
                if(!std::is_trivial<type>().value) throw(EINVAL);
                resize(sizeof(value));
                *((type*)variable) = std::move(value);
            }

            untypizedtrivial(const untypizedtrivial& value)
            {
                resize(value.size);
                if(value.size != 0) *((char*)variable) = (char&)value;
            }

            untypizedtrivial(untypizedtrivial&& value) : variable{value.variable}, capacity{value.capacity}
            {
                value.variable = nullptr;
                value.capacity = 0;
            }

            // untypizedtrivial(untypizedtrivial&& value)
            // {
            //     variable = value.variable;
            //     capacity = value.capacity;
            //     value.variable = nullptr;
            //     value.capacity = 0;
            // }

            untypizedtrivial& operator=(const untypizedtrivial& value)
            {
                if(&value == this) return *this;
                resize(value.size);
                if(value.size != 0) *((char*)variable) = (char&)value;
                return *this;
            }

            untypizedtrivial(const untypizedtrivial&& value)
            {
                resize(value.size);
                if(value.size != 0) *((char*)variable) = (char&)value;
            }

            untypizedtrivial& operator=(const untypizedtrivial&& value)
            {
                if(&value == this) return *this;
                resize(value.size);
                if(value.size != 0) *((char*)variable) = (char&)value;
                return *this;
            }

            template<class type>
            untypizedtrivial& operator=(const type& value)
            {
                if(!std::is_trivial<type>().value) throw(EINVAL);
                resize(sizeof(type));
                *((type*)variable) = value;
                return *this;
            }

            template<class type>
            operator type&() const
            {
                if(!std::is_trivial<type>().value or (sizeof(type) != capacity)) throw(EINVAL);
                return *((type*)variable);
            }

            // template<class type>
            // operator type()
            // {
            //     return *((type*)variable);
            // }

            ~untypizedtrivial()
            {
                if(variable != nullptr) free(variable);
            }
    };

    /**
     * @brief Due to the compatibility with POD. (any POD type is also trivial type)
     * 
     */
    typedef untypizedtrivial untypizedpod;

    /**
     * @brief Store any type here.
     *
     */
    struct untypized
    {
        private:

        /**
         * @brief Data stored.
         *
         */
        void* data = nullptr;

        /**
         * @brief Current type.
         *
         */
        const std::type_info& (*current)() = nullptr;

        void*(*copy)(void* other) = nullptr;

        void*(*move)(void* other) = nullptr;

        void(*destroy)(void* data) = nullptr;

        public:
        untypized() : data{nullptr}, copy{nullptr}, move{nullptr}, destroy{nullptr}, current{nullptr} {}

        const std::type_info& id()
        {
            if(current == nullptr) return typeid(void);
            return current();
        }

        template<class type>
        untypized(const type& variable)
        {
            data = (void*)(new type{variable});
            copy = [](void* data){ return (void*)(new type{*((type*)data)}); };
            move = [](void* data){ return (void*)(new type{std::move(*((type*)data))}); };
            destroy = [](void* data){ delete ((type*)data); };
            current = []() -> const std::type_info& { return typeid(type); };
        }

        template<class type>
        untypized(type&& variable)
        {
            data = (void*)(new type{std::move(variable)});
            copy = [](void* data){ return (void*)(new type{*((type*)data)}); };
            move = [](void* data){ return (void*)(new type{std::move(*((type*)data))}); };
            destroy = [](void* data){ delete ((type*)data); };
            current = []() -> const std::type_info& { return typeid(type); };
        }

        untypized(const untypized& other) : copy{other.copy}, move{other.move}, current{other.current}
        {
            if(destroy != nullptr) destroy(data);
            destroy = other.destroy;
            data = other.copy(other.data);
        }

        untypized& operator=(const untypized& other)
        {
            copy = other.copy;
            move = other.move;
            current = other.current;
            if(destroy != nullptr) destroy(data);
            destroy = other.destroy;
            data = other.copy(other.data);
            return *this;
        }

        untypized(untypized&& other) : copy{other.copy}, move{other.move}, current{other.current}
        {
            if(destroy != nullptr) destroy(data);
            destroy = other.destroy;
            data = other.move(other.data);
            other.data = nullptr;
            other.clean(); // Invoke cleaning.
        }

        void clean()
        {
            if(destroy != nullptr and data != nullptr) destroy(data);
            data = nullptr;

            copy = nullptr;
            move = nullptr;
            destroy = nullptr;
            current = nullptr;
        }

        template<class type>
        operator type&()
        {
            if(data == nullptr or typeid(type) != current())
            {
                throw(EINVAL);
            }
            return *((type*)data);
        }

        ~untypized()
        {
            if(destroy != nullptr and data != nullptr) destroy(data);
        }
    };
};