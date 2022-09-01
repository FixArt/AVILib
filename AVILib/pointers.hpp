#ifndef AVILIB_USED_POINTERS
#pragma once 

// #include <cmath>
// #include <initializer_list>
// #include <functional>
#include <algorithm>
#include <type_traits>

#include "counter.hpp"

/**
 * @brief Namespace of Alternative Vector (And pair... and shared_ptr... and other things?) Implementation Library.
 * 
 */
namespace AVIL
{
    template<class type>
    struct default_delete
    {
        constexpr inline void operator()(type* deleted)
        {
            delete deleted;
        }
    };

    template<class type>
    struct default_delete<type[]>
    {
        constexpr inline void operator()(type* deleted)
        {
            delete[] deleted;
        }
    };

    template<>
    struct default_delete<void>
    {
        constexpr inline void operator()(void* deleted)
        {
            free(deleted);
        }
    };

    template<class type, class deleter = default_delete<type>>
    struct unique_ptr
    {
        private:

            type* pointed;

        public:

            unique_ptr(type* pointer = nullptr) : pointed{pointer} {}

            // unique_ptr(unique_ptr& copied)
            // {
            //     pointed = copied.pointed;
            //     copied.pointed = nullptr;
            // }

            unique_ptr(unique_ptr&& copied)
            {
                pointed = copied.pointed;
                copied.pointed = nullptr;
            }

            unique_ptr& operator=(unique_ptr&& copied)
            {
                if(pointed != nullptr) { deleter temporary; temporary(pointed); }
                pointed = copied.pointed;
                // copied.pointed = nullptr;
                return *this;
            }

            unique_ptr& operator=(type* copied)
            {
                if(pointed != nullptr) { deleter temporary; temporary(pointed); }
                pointed = copied;
                // deletor = (copied == nullptr)?(nullptr):([](void* removed){ deleter temporary; temporary(removed); });
                return *this;
            }

            // const type& operator[](size_t index)
            // {
            //     return pointed[index];
            // }

            type& operator*()
            {
                return *pointed;
            }

            const type& operator*() const
            {
                return *pointed;
            }

            type& get()
            {
                return *pointed;
            }

            const type& get() const
            {
                return *pointed;
            }
            
            template<typename = typename std::enable_if<std::is_array<type>{}.value>>
            const type& operator[](const size_t& index) const
            {
                return pointed[index];
            }

            template<typename = typename std::enable_if<std::is_array<type>{}.value>>
            type& operator[](const size_t& index)
            {
                return pointed[index];
            }

            type* operator->()
            {
                return pointed;
            }

            const type* const operator->() const
            {
                return pointed;
            }

            operator bool() const
            {
                return pointed != nullptr;
            }

            operator const type* const() const
            {
                return pointed;
            }

            operator type*()
            {
                return pointed;
            }

            template<typename other, typename = typename std::enable_if<std::is_convertible<other*, type*>{}.value>>
            operator const other* const() const
            {
                return (other*)pointed;
            }

            template<typename other, typename = typename std::enable_if<std::is_convertible<other*, type*>{}.value>>
            operator other*()
            {
                return (other*)pointed;
            }

            // operator unique_ptr<type[]>()
            // {
            //     return new type[1]{*pointed};
            // }

            ~unique_ptr()
            {
                // if(pointed != nullptr)  { deleter temporary; temporary(pointed); }
                if(pointed != nullptr) { deleter temporary; temporary(pointed); }
            }
    };

    template<class type, class deleter = default_delete<type>>
    struct weak_ptr;

    template<class type, class deleter = default_delete<type>>
    struct shared_ptr
    {
        private:

            type* pointed;

            counter<size_t>* point;

            void(*destroyment)(void*);

            // template<typename other, typename = typename std::enable_if<std::is_convertible<type*, other*>{}.value>> friend class shared_ptr;
            template<typename other, typename other_deleter> friend class shared_ptr;

            // template<typename other, typename = typename std::enable_if<std::is_convertible<type*, other*>{}.value>> friend class shared_ptr;
            template<typename other, typename other_deleter> friend class weak_ptr;

            counter<size_t>* getCounter()
            {
                return point;
            }

        public:

            // shared_ptr() : pointed{nullptr}, point{new counter<size_t>{1}} {}

            shared_ptr(type* pointer = nullptr) : pointed{pointer}, point{new counter<size_t>[2]}, destroyment{(pointer == nullptr)?((decltype(destroyment))nullptr):([](void* removed){ deleter temporary; temporary((type*)removed); })} { ++(point[0]); }

            shared_ptr(type* pointer, counter<size_t>* otherPoint) : pointed{pointer}, point{otherPoint}, destroyment{(pointer == nullptr)?((decltype(destroyment))nullptr):([](void* removed){ deleter temporary; temporary((type*)removed); })} { ++(point[0]); }

            shared_ptr<type, deleter>(const shared_ptr<type, deleter>& copied)
            {
                pointed = copied.pointed;
                point = copied.point;
                destroyment = copied.destroyment;
                ++(point[0]);
            }

            template<typename other, typename otherDeleter, typename = typename std::enable_if<std::is_convertible<other*, type*>{}.value>>
            shared_ptr(const shared_ptr<other, otherDeleter>& copied)
            {
                pointed = copied.pointed;
                point = copied.point;
                destroyment = copied.destroyment;
                ++(point[0]);
            }

            shared_ptr<type, deleter>(shared_ptr<type, deleter>&& copied)
            {
                pointed = copied.pointed;
                point = copied.point;
                destroyment = copied.destroyment;
                copied.pointed = nullptr;
                copied.point = new counter<size_t>[2];
                ++(copied.point[0]);
            }

            shared_ptr& operator=(const shared_ptr& copied)
            {
                if(&copied == this) return *this;
                --(point[0]);
                if((point[0]) == (size_t)0)
                {
                    if(pointed != nullptr) destroyment(pointed);
                    if((point[1]) == (size_t)0) delete[] point;
                }
                pointed = copied.pointed;
                point = copied.point;
                destroyment = copied.destroyment;
                ++(point[0]);
                return *this;
            }

            template<typename other, typename otherDeleter, typename = typename std::enable_if<std::is_convertible<other*, type*>{}.value>>
            shared_ptr& operator=(const shared_ptr<other, otherDeleter>& copied)
            {
                if(&copied == this) return *this;
                --(point[0]);
                if((point[0]) == (size_t)0)
                {
                    if(pointed != nullptr) destroyment(pointed);
                    if((point[1]) == (size_t)0) delete[] point;
                }
                pointed = copied.pointed;
                point = copied.point;
                destroyment = copied.destroyment;
                ++(point[0]);
                return *this;
            }

            bool unique() const
            {
                return use_count() == 1;
            }

            // const type& operator[](size_t index)
            // {
            //     return pointed[index];
            // }

            shared_ptr& operator=(type* copied)
            {
                --(point[0]);
                if((point[0]) == (size_t)0)
                {
                    if(pointed != nullptr) destroyment(pointed);
                    if((point[1]) == (size_t)0) delete[] point;
                }
                pointed = copied;
                point = new counter<size_t>[2];
                // deletor = (copied == nullptr)?(nullptr):([](void* removed){ deleter temporary; temporary(removed); });
                ++(point[0]);
                return *this;
            }

            template<typename other, typename = typename std::enable_if<std::is_convertible<other*, type*>{}.value>>
            shared_ptr& operator=(other* copied)
            {
                --(point[0]);
                if((point[0]) == (size_t)0)
                {
                    if(pointed != nullptr) destroyment(pointed);
                    if((point[1]) == (size_t)0) delete[] point;
                }
                pointed = (type*)copied;
                point = new counter<size_t>[2];
                destroyment = (copied == nullptr)?(nullptr):([](void* removed){ default_delete<other> temporary; temporary(removed); });
                ++(point[0]);
                return *this;
            }

            operator bool() const
            {
                return pointed != nullptr;
            }

            type& operator*()
            {
                return *pointed;
            }

            const type& operator*() const
            {
                return *pointed;
            }

            type& get()
            {
                return *pointed;
            }
            
            const type& get() const
            {
                return *pointed;
            }
            
            template<typename = typename std::enable_if<std::is_array<type>{}.value>>
            const type& operator[](const size_t& index) const
            {
                return pointed[index];
            }

            template<typename = typename std::enable_if<std::is_array<type>{}.value>>
            type& operator[](const size_t& index)
            {
                return pointed[index];
            }

            type* operator->()
            {
                return pointed;
            }

            const type* const operator->() const
            {
                return pointed;
            }

            size_t use_count() const
            {
                return (size_t)(point[0]);
            }

            operator const type* const() const
            {
                return pointed;
            }

            operator type*()
            {
                return pointed;
            }

            template<typename other, typename = typename std::enable_if<std::is_convertible<other*, type*>{}.value>>
            operator const other* const() const
            {
                return (other*)pointed;
            }

            template<typename other, typename = typename std::enable_if<std::is_convertible<other*, type*>{}.value>>
            operator other*()
            {
                return (other*)pointed;
            }

            // operator shared_ptr<type[]>()
            // {
            //     return new type[1]{*pointed};
            // }

            ~shared_ptr()
            {
                --(point[0]);
                if((point[0]) == (size_t)0)
                {
                    if(pointed != nullptr) destroyment(pointed);
                    if((point[1]) == (size_t)0) delete[] point;
                }
            }
    };

    template<class deleter>
    struct unique_ptr<void, deleter>
    {
        private:

            void* pointed;

        public:

            unique_ptr(void* pointer = nullptr) : pointed{pointer} {}

            // unique_ptr(unique_ptr& copied)
            // {
            //     pointed = copied.pointed;
            //     copied.pointed = nullptr;
            // }

            unique_ptr(unique_ptr&& copied)
            {
                pointed = copied.pointed;
                copied.pointed = nullptr;
            }

            unique_ptr& operator=(unique_ptr&& copied)
            {
                if(pointed != nullptr) { deleter temporary; temporary(pointed); }
                pointed = copied.pointed;
                copied.pointed = nullptr;
                return *this;
            }

            unique_ptr& operator=(void* copied)
            {
                if(pointed != nullptr) { deleter temporary; temporary(pointed); }
                pointed = copied;
                // deletor = (copied == nullptr)?(nullptr):([](void* removed){ deleter temporary; temporary(removed); });
                return *this;
            }

            // const type& operator[](size_t index)
            // {
            //     return pointed[index];
            // }

            void* operator->()
            {
                return pointed;
            }

            const void* const operator->() const
            {
                return pointed;
            }

            operator bool() const
            {
                return pointed != nullptr;
            }

            operator const void* const() const
            {
                return pointed;
            }

            operator void*()
            {
                return pointed;
            }

            template<typename other, typename = typename std::enable_if<std::is_convertible<other*, void*>{}.value>>
            operator const other* const() const
            {
                return (other*)pointed;
            }

            template<typename other, typename = typename std::enable_if<std::is_convertible<other*, void*>{}.value>>
            operator other*()
            {
                return (other*)pointed;
            }

            // operator unique_ptr<type[]>()
            // {
            //     return new type[1]{*pointed};
            // }

            ~unique_ptr()
            {
                // if(pointed != nullptr)  { deleter temporary; temporary(pointed); }
                if(pointed != nullptr) { deleter temporary; temporary(pointed); }
            }
    };

    template<class deleter>
    struct shared_ptr<void, deleter>
    {
        private:

            void* pointed;

            counter<size_t>* point;

            void(*destroyment)(void*);

            counter<size_t>* getCounter()
            {
                return point;
            }

            // template<typename other, typename = typename std::enable_if<std::is_convertible<type*, other*>{}.value>> friend class shared_ptr;
            template<typename other, typename other_deleter> friend class shared_ptr;

            // template<typename other, typename = typename std::enable_if<std::is_convertible<type*, other*>{}.value>> friend class shared_ptr;
            template<typename other, typename other_deleter> friend class weak_ptr;

        public:

            // shared_ptr() : pointed{nullptr}, point{new counter<size_t>{1}} {}

            shared_ptr(void* pointer = nullptr) : pointed{pointer}, point{new counter<size_t>[2]}, destroyment{(pointer == nullptr)?((decltype(destroyment))nullptr):([](void* removed){ deleter temporary; temporary((void*)removed); })} { ++(point[0]); }

            shared_ptr(void* pointer, counter<size_t>* otherPoint) : pointed{pointer}, point{otherPoint}, destroyment{(pointer == nullptr)?((decltype(destroyment))nullptr):([](void* removed){ deleter temporary; temporary((void*)removed); })} { ++(point[0]); }

            shared_ptr<void, deleter>(const shared_ptr<void, deleter>& copied)
            {
                pointed = copied.pointed;
                point = copied.point;
                destroyment = copied.destroyment;
                ++(point[0]);
            }

            template<typename other, typename otherDeleter, typename = typename std::enable_if<std::is_convertible<other*, void*>{}.value>>
            shared_ptr(const shared_ptr<other, otherDeleter>& copied)
            {
                pointed = copied.pointed;
                point = copied.point;
                destroyment = copied.destroyment;
                ++(point[0]);
            }

            shared_ptr<void, deleter>(shared_ptr<void, deleter>&& copied)
            {
                pointed = copied.pointed;
                point = copied.point;
                destroyment = copied.destroyment;
                copied.pointed = nullptr;
                copied.point = new counter<size_t>[2];
                ++(copied.point[0]);
            }

            shared_ptr& operator=(const shared_ptr& copied)
            {
                if(&copied == this) return *this;
                --(point[0]);
                if((point[0]) == (size_t)0)
                {
                    if(pointed != nullptr) destroyment(pointed);
                    if((point[1]) == (size_t)0) delete[] point;
                }
                pointed = copied.pointed;
                point = copied.point;
                destroyment = copied.destroyment;
                ++(point[0]);
                return *this;
            }

            template<typename other, typename otherDeleter, typename = typename std::enable_if<std::is_convertible<other*, void*>{}.value>>
            shared_ptr& operator=(const shared_ptr<other, otherDeleter>& copied)
            {
                if(&copied == this) return *this;
                --(point[0]);
                if((point[0]) == (size_t)0)
                {
                    if(pointed != nullptr) destroyment(pointed);
                    if((point[1]) == (size_t)0) delete[] point;
                }
                pointed = copied.pointed;
                point = copied.point;
                destroyment = copied.destroyment;
                ++(point[0]);
                return *this;
            }

            bool unique() const
            {
                return use_count() == 1;
            }

            // const type& operator[](size_t index)
            // {
            //     return pointed[index];
            // }

            shared_ptr& operator=(void* copied)
            {
                --(point[0]);
                if((point[0]) == (size_t)0)
                {
                    if(pointed != nullptr) destroyment(pointed);
                    if((point[1]) == (size_t)0) delete[] point;
                }
                pointed = copied;
                point = new counter<size_t>[2];
                // deletor = (copied == nullptr)?(nullptr):([](void* removed){ deleter temporary; temporary(removed); });
                ++(point[0]);
                return *this;
            }

            template<typename other, typename = typename std::enable_if<std::is_convertible<other*, void*>{}.value>>
            shared_ptr& operator=(other* copied)
            {
                --(point[0]);
                if((point[0]) == (size_t)0)
                {
                    if(pointed != nullptr) destroyment(pointed);
                    if((point[1]) == (size_t)0) delete[] point;
                }
                pointed = (void*)copied;
                point = new counter<size_t>[2];
                destroyment = (copied == nullptr)?(nullptr):([](void* removed){ default_delete<other> temporary; temporary(removed); });
                ++(point[0]);
                return *this;
            }

            operator bool() const
            {
                return pointed != nullptr;
            }

            void* operator->()
            {
                return pointed;
            }

            const void* const operator->() const
            {
                return pointed;
            }

            size_t use_count() const
            {
                return (size_t)(point[0]);
            }

            operator const void* const() const
            {
                return pointed;
            }

            operator void*()
            {
                return pointed;
            }

            template<typename other, typename = typename std::enable_if<std::is_convertible<other*, void*>{}.value>>
            operator const other* const() const
            {
                return (other*)pointed;
            }

            template<typename other, typename = typename std::enable_if<std::is_convertible<other*, void*>{}.value>>
            operator other*()
            {
                return (other*)pointed;
            }

            // operator shared_ptr<type[]>()
            // {
            //     return new type[1]{*pointed};
            // }

            ~shared_ptr()
            {
                --(point[0]);
                if((point[0]) == (size_t)0)
                {
                    if(pointed != nullptr) destroyment(pointed);
                    if((point[1]) == (size_t)0) delete[] point;
                }
            }
    };

    // template<> struct unique_ptr<void>{};

    // template<> struct shared_ptr<void>{};
    
    // template<class type>
    // struct unique_ptr<type[]>
    // {
    //     private:

    //         type* pointed;

    //     public:

    //         unique_ptr(type* pointer = nullptr) : pointed{pointer} {}

    //         // unique_ptr(unique_ptr& copied)
    //         // {
    //         //     pointed = copied.pointed;
    //         //     copied.pointed = nullptr;
    //         // }

    //         unique_ptr(unique_ptr&& copied)
    //         {
    //             pointed = copied.pointed;
    //             // copied.pointed = nullptr;
    //         }

    //         unique_ptr& operator=(unique_ptr&& copied)
    //         {
    //             if(pointed != nullptr) delete[] pointed;
    //             pointed = copied.pointed;
    //             // copied.pointed = nullptr;
    //             return *this;
    //         }

    //         const type& operator[](const size_t& index) const
    //         {
    //             return pointed[index];
    //         }

    //         type& operator[](const size_t& index)
    //         {
    //             return pointed[index];
    //         }

    //         type& operator*()
    //         {
    //             return *pointed;
    //         }

    //         const type& operator*() const
    //         {
    //             return *pointed;
    //         }

    //         type& get()
    //         {
    //             return *pointed;
    //         }

    //         const type& get() const
    //         {
    //             return *pointed;
    //         }

    //         type* operator->()
    //         {
    //             return pointed;
    //         }

    //         const type* const operator->() const
    //         {
    //             return pointed;
    //         }

    //         operator bool() const
    //         {
    //             return pointed != nullptr;
    //         }

    //         operator const type* const() const
    //         {
    //             return pointed;
    //         }

    //         operator type*()
    //         {
    //             return pointed;
    //         }

    //         // template<typename other, typename = typename std::enable_if<std::is_convertible<other*, type*>{}.value>>
    //         // operator const other* const() const
    //         // {
    //         //     return (other*)pointed;
    //         // }

    //         // template<typename other, typename = typename std::enable_if<std::is_convertible<other*, type*>{}.value>>
    //         // operator other*()
    //         // {
    //         //     return (other*)pointed;
    //         // }

    //         operator unique_ptr<type>()
    //         {
    //             return new type{pointed[0]};
    //         }

    //         ~unique_ptr()
    //         {
    //             if(pointed != nullptr) delete[] pointed;
    //         }
    // };

    // template<class type>
    // struct shared_ptr<type[]>
    // {
    //     private:

    //         type* pointed;

    //         counter<size_t>* point;

    //         // template<typename other, typename = typename std::enable_if<std::is_convertible<type*, other*>{}.value>> friend class shared_ptr;

    //     public:

    //         // shared_ptr() : pointed{nullptr}, point{new counter<size_t>{1}} {}

    //         shared_ptr(type* pointer = nullptr) : pointed{pointer}, point{new counter<size_t>[2]} { ++(point[0]); }

    //         shared_ptr(type* pointer, counter<size_t>* otherPoint) : pointed{pointer}, point{otherPoint} { ++(point[0]); }

    //         shared_ptr<type[]>(const shared_ptr<type[]>& copied)
    //         {
    //             pointed = copied.pointed;
    //             point = copied.point;
    //             ++(point[0]);
    //         }

    //         // template<typename other, typename = typename std::enable_if<std::is_convertible<other*, type*>{}.value>>
    //         // shared_ptr<type[]>(const shared_ptr<other[]>& copied)
    //         // {
    //         //     pointed = copied.pointed;
    //         //     point = copied.point;
    //         //     ++(point[0]);
    //         // }

    //         shared_ptr<type[]>(shared_ptr<type[]>&& copied)
    //         {
    //             pointed = copied.pointed;
    //             point = copied.point;
    //             // copied.pointed = nullptr;
    //             // copied.point = new counter<size_t>[2];
    //             ++(copied.point[0]);
    //         }

    //         shared_ptr& operator=(const shared_ptr& copied)
    //         {
    //             if(&copied == this) return *this;
    //             --(point[0]);
    //             if((point[0]) == (size_t)0)
    //             {
    //                 if(pointed != nullptr) delete[] pointed;
    //                 if((point[1]) == (size_t)0) delete[] point;
    //             }
    //             pointed = copied.pointed;
    //             point = copied.point;
    //             ++(point[0]);
    //             return *this;
    //         }

    //         bool unique()
    //         {
    //             return use_count() == 1;
    //         }

    //         const type& operator[](const size_t& index) const
    //         {
    //             return pointed[index];
    //         }

    //         type& operator[](const size_t& index)
    //         {
    //             return pointed[index];
    //         }

    //         shared_ptr& operator=(type* copied)
    //         {
    //             --(point[0]);
    //             if((point[0]) == (size_t)0)
    //             {
    //                 if(pointed != nullptr) delete[] pointed;
    //                 if((point[1]) == (size_t)0) delete[] point;
    //             }
    //             pointed = copied;
    //             point = new counter<size_t>[2];
    //             ++(point[0]);
    //             return *this;
    //         }

    //         operator bool() const
    //         {
    //             return pointed != nullptr;
    //         }

    //         type& operator*()
    //         {
    //             return *pointed;
    //         }

    //         type& get()
    //         {
    //             return *pointed;
    //         }
            
    //         const type& get() const
    //         {
    //             return *pointed;
    //         }

    //         type* operator->()
    //         {
    //             return pointed;
    //         }

    //         const type* const operator->() const
    //         {
    //             return pointed;
    //         }

    //         size_t use_count()
    //         {
    //             return (size_t)(point[0]);
    //         }

    //         counter<size_t>* getCounter()
    //         {
    //             return point;
    //         }

    //         const type& operator*() const
    //         {
    //             return *pointed;
    //         }

    //         operator const type* const() const
    //         {
    //             return pointed;
    //         }

    //         operator type*()
    //         {
    //             return pointed;
    //         }

    //         template<typename other, typename = typename std::enable_if<std::is_convertible<other*, type*>{}.value>>
    //         operator const other* const() const
    //         {
    //             return (other*)pointed;
    //         }

    //         template<typename other, typename = typename std::enable_if<std::is_convertible<other*, type*>{}.value>>
    //         operator other*()
    //         {
    //             return (other*)pointed;
    //         }

    //         operator shared_ptr<type>()
    //         {
    //             return new type{pointed[0]};
    //         }

    //         ~shared_ptr()
    //         {
    //             --(point[0]);
    //             if((point[0]) == (size_t)0)
    //             {
    //                 if(pointed != nullptr) delete[] pointed;
    //                 if((point[1]) == (size_t)0) delete[] point;
    //             }
    //         }
    // };

    template<class type, class deleter = default_delete<type>, typename = typename std::enable_if<std::is_default_constructible<type>::value>>
    unique_ptr<type, deleter> make_unique()
    {
        return unique_ptr<type, deleter>{new type{}};
    }

    template<class type, class deleter = default_delete<type>, typename = typename std::enable_if<std::is_copy_constructible<type>::value>>
    unique_ptr<type, deleter> make_unique(const type& copied)
    {
        return unique_ptr<type, deleter>{new type{copied}};
    }

    template<class type, class deleter = default_delete<type>, class... arguments, typename = typename std::enable_if<std::is_constructible<type, arguments...>::value>>
    unique_ptr<type, deleter> make_unique(arguments... constructor_arguments)
    {
        return unique_ptr<type, deleter>{new type{constructor_arguments...}};
    }

    template<class type, class deleter = default_delete<type>, typename = typename std::enable_if<std::is_default_constructible<type>::value>>
    shared_ptr<type, deleter> make_shared()
    {
        return shared_ptr<type, deleter>{new type{}};
    }

    template<class type, class deleter = default_delete<type>, typename = typename std::enable_if<std::is_copy_constructible<type>::value>>
    shared_ptr<type, deleter> make_shared(const type& copied)
    {
        return shared_ptr<type, deleter>{new type{copied}};
    }

    template<class type, class deleter = default_delete<type>, class... arguments, typename = typename std::enable_if<std::is_constructible<type, arguments...>::value>>
    shared_ptr<type, deleter> make_shared(arguments... constructor_arguments)
    {
        return shared_ptr<type, deleter>{new type{constructor_arguments...}};
    }
    template<class type, class deleter = default_delete<type[]>>
    unique_ptr<type[], deleter> make_unique(const size_t& size)
    {
        return unique_ptr<type[], deleter>{new type[size]};
    }

    template<class type, class deleter = default_delete<type[]>>
    shared_ptr<type[], deleter> make_shared(const size_t& size)
    {
        return shared_ptr<type[], deleter>{new type[size]};
    }

    template<class type, class deleter>
    struct weak_ptr
    {
        private:
            type* stored;

            counter<size_t>* point;

            void(*destroyment)(void*);

            // template<typename other, typename other_deleter> friend class shared_ptr;

            counter<size_t>* getCounter()
            {
                return point;
            }

        public:
            template<class other_deleter>
            weak_ptr(shared_ptr<type, other_deleter>& store) : stored{store.pointed}, point{store.point}, destroyment{store.destroyment} { ++(point[1]); }

            template<class other_deleter>
            weak_ptr(shared_ptr<type, other_deleter>* store) : stored{store->pointed}, point{store->point}, destroyment{store->destroyment} { ++(point[1]); }

            template<class other_deleter>
            weak_ptr(weak_ptr<type, other_deleter>& store) : stored{store.stored}, point{store.point}, destroyment{store.destroyment} { ++(point[1]); }

            template<class other_deleter>
            weak_ptr(weak_ptr<type, other_deleter>* store) : stored{store->stored}, point{store->point}, destroyment{store->destroyment} { ++(point[1]); }

            template<typename other, class other_deleter, typename = typename std::enable_if<std::is_convertible<other*, type*>{}.value>>
            weak_ptr(shared_ptr<other, other_deleter>& store) : stored{store.pointed}, point{store.point}, destroyment{store.destroyment} { ++(point[1]); }

            template<typename other, class other_deleter, typename = typename std::enable_if<std::is_convertible<other*, type*>{}.value>>
            weak_ptr(shared_ptr<other, other_deleter>* store) : stored{store->pointed}, point{store->point}, destroyment{store->destroyment} { ++(point[1]); }

            weak_ptr& operator=(weak_ptr& copied)
            {
                if(&copied == this)
                {
                    return *this;
                }
                --(point[1]);
                if((point[0]) == (size_t)0 and (point[1]) == (size_t)0)
                {
                    delete[] point;
                }
                stored = copied.stored;
                point = copied.point;
                destroyment = copied.destroyment;
                return *this;
            }

            template<typename other, typename = typename std::enable_if<std::is_convertible<other*, type*>{}.value>>
            weak_ptr& operator=(weak_ptr<other>& copied)
            {
                if(&copied == this)
                {
                    return *this;
                }
                --(point[1]);
                if((point[0]) == (size_t)0 and (point[1]) == (size_t)0)
                {
                    delete[] point;
                }
                stored = copied.stored;
                point = copied.point;
                destroyment = copied.destroyment;
                return *this;
            }

            weak_ptr& operator=(shared_ptr<type>& copied)
            {
                --(point[1]);
                if((point[0]) == (size_t)0 and (point[1]) == (size_t)0)
                {
                    delete[] point;
                }
                stored = copied.pointed;
                point = copied.point;
                return *this;
            }

            template<typename other, typename = typename std::enable_if<std::is_convertible<other*, type*>{}.value>>
            weak_ptr& operator=(shared_ptr<other>& copied)
            {
                --(point[1]);
                if((point[0]) == (size_t)0 and (point[1]) == (size_t)0)
                {
                    delete[] point;
                }
                stored = copied.pointed;
                point = copied.point;
                return *this;
            }

            weak_ptr& operator=(weak_ptr* copied)
            {
                if(copied == this)
                {
                    return *this;
                }
                --(point[1]);
                if((point[0]) == (size_t)0 and (point[1]) == (size_t)0)
                {
                    delete[] point;
                }
                stored = copied->stored;
                point = copied->point;
                return *this;
            }

            template<typename other, typename = typename std::enable_if<std::is_convertible<other*, type*>{}.value>>
            weak_ptr& operator=(weak_ptr<other>* copied)
            {
                if(copied == this)
                {
                    return *this;
                }
                --(point[1]);
                if((point[0]) == (size_t)0 and (point[1]) == (size_t)0)
                {
                    delete[] point;
                }
                stored = copied->stored;
                point = copied->point;
                return *this;
            }

            weak_ptr& operator=(shared_ptr<type>* copied)
            {
                --(point[1]);
                if((point[0]) == (size_t)0 and (point[1]) == (size_t)0)
                {
                    delete[] point;
                }
                stored = copied->pointed;
                point = copied->point;
                return *this;
            }

            template<typename other, typename = typename std::enable_if<std::is_convertible<other*, type*>{}.value>>
            weak_ptr& operator=(shared_ptr<other>* copied)
            {
                --(point[1]);
                if((point[0]) == (size_t)0 and (point[1]) == (size_t)0)
                {
                    delete[] point;
                }
                stored = copied->pointed;
                point = copied->point;
                return *this;
            }

            bool expired()
            {
                return point[0] == (size_t)0;
            }

            shared_ptr<type> lock()
            {
                // return expired() ? shared_ptr<type, deleter>{} : shared_ptr<type, deleter>{stored, point};
                if(expired())
                {
                    shared_ptr<type, default_delete<type>> returned{}; // To ensure no problems default used.
                    return returned;
                }
                else
                {
                    shared_ptr<type, deleter> returned{stored, point}; // Good returned.
                    returned.destroyment = destroyment;
                    return returned;
                }
            }

            size_t use_count() const
            {
                return (size_t)(point[0]);
            }

            ~weak_ptr()
            {
                --(point[1]);
                if((point[0]) == (size_t)0 and (point[1]) == (size_t)0)
                {
                    delete[] point;
                }
            }
    };

    /**
     * @brief Simple object pointers.
     * 
     * @tparam type Returned type.
     */
    template<class type>
    struct objptr
    {
        private:
            type* pointedObject = nullptr;
        public:
            objptr<type>() : pointedObject{new type} {}

            objptr<type>(const type& copied)
            {
                if(pointedObject != nullptr) delete pointedObject;
                pointedObject = new type;
                *pointedObject = copied;
            }

            objptr<type>(const objptr<type>& copied)
            {
                if(pointedObject != nullptr) delete pointedObject;
                pointedObject = new type;
                *pointedObject = (const type&)copied;
            }

            objptr<type>(objptr<type>&& copied)
            {
                pointedObject = copied.pointedObject;
                copied.pointedObject = nullptr;
            }

            // objptr<type>(type* assigned)
            // {
            //     if(pointedObject != nullptr) delete pointedObject;
            //     pointedObject = assigned;
            // }

            objptr<type> operator=(const objptr<type>& copied)
            {
                if(pointedObject != nullptr) delete pointedObject;
                pointedObject = new type;
                *pointedObject = (const type&)copied;
                return *this;
            }

            objptr<type> operator=(const type& copied)
            {
                if(pointedObject != nullptr) delete pointedObject;
                pointedObject = new type;
                *pointedObject = copied;
            }

            // objptr<type> operator=(type* assigned)
            // {
            //     if(pointedObject != nullptr) delete pointedObject;
            //     pointedObject = assigned;
            // }

            ~objptr<type>()
            {
                if(pointedObject != nullptr) delete pointedObject;
            }

            operator type*()
            {
                return pointedObject;
            }

            operator type&()
            {
                if(pointedObject == nullptr)
                {
                    pointedObject = new type;
                }
                return *pointedObject;
            }

            type& operator*()
            {
                return *pointedObject;
            }

            const type& operator*() const
            {
                return *pointedObject;
            }

            type& get()
            {
                return *pointedObject;
            }

            const type& get() const
            {
                return *pointedObject;
            }

            type* operator->()
            {
                return pointedObject;
            }

            const type* const operator->() const
            {
                return pointedObject;
            }

            operator const type* const() const
            {
                return pointedObject;
            }

            operator const type&() const
            {
                if(pointedObject == nullptr)
                {
                    throw(EFAULT);
                }
                return *pointedObject;
            }
            
            const type* begin() const
            {
                return pointedObject;
            }

            const type* end() const
            {
                return pointedObject + 1;
            }

            type* begin()
            {
                return pointedObject;
            }

            type* end()
            {
                return pointedObject + 1;
            }
    };
};
#define AVILIB_USED_POINTERS 1
#endif
