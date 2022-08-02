#ifndef AVILIB_USED_POINTERS
#pragma once 

#include <cmath>
#include <initializer_list>
#include <functional>
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
    struct unique_ptr
    {
        private:

            type* pointed;

        public:

            unique_ptr(type* pointer = nullptr) : pointed{pointer} {}

            unique_ptr(unique_ptr& copied)
            {
                pointed = copied.pointed;
                copied.pointed = nullptr;
            }

            unique_ptr(unique_ptr&& copied)
            {
                pointed = copied.pointed;
                copied.pointed = nullptr;
            }

            // const type& operator[](size_t index)
            // {
            //     return pointed[index];
            // }

            type& operator*()
            {
                return *pointed;
            }

            type& get()
            {
                return *pointed;
            }

            type* operator->()
            {
                return pointed;
            }

            const type& operator*() const
            {
                return *pointed;
            }

            const type& get() const
            {
                return *pointed;
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

            ~unique_ptr()
            {
                if(pointed != nullptr) delete pointed;
            }
    };

    template<class type>
    struct shared_ptr
    {
        private:

            type* pointed;

            counter<size_t>* point;

        public:

            // shared_ptr() : pointed{nullptr}, point{new counter<size_t>{1}} {}

            shared_ptr(type* pointer = nullptr) : pointed{pointer}, point{new counter<size_t>[2]} { ++(point[0]); }

            shared_ptr(type* pointer, counter<size_t>* otherPoint) : pointed{pointer}, point{otherPoint} { ++(point[0]); }

            shared_ptr<type>(const shared_ptr<type>& copied)
            {
                pointed = copied.pointed;
                point = copied.point;
                ++(point[0]);
            }

            shared_ptr<type>(shared_ptr<type>&& copied)
            {
                pointed = copied.pointed;
                point = copied.point;
                copied.pointed = nullptr;
                copied.point = new counter<size_t>[2];
                ++(copied.point[0]);
            }

            shared_ptr& operator=(shared_ptr& copied)
            {
                if(copied == *this) return *this;
                --(point[0]);
                if((point[0]) == (size_t)0)
                {
                    if(pointed != nullptr) delete pointed;
                    if((point[1]) == (size_t)0) delete[] point;
                }
                pointed = copied.pointed;
                point = copied.point;
                ++(point[0]);
                return *this;
            }

            bool unique()
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
                    if(pointed != nullptr) delete pointed;
                    if((point[1]) == (size_t)0) delete[] point;
                }
                pointed = copied;
                point = new counter<size_t>[2];
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

            type& get()
            {
                return *pointed;
            }

            type* operator->()
            {
                return pointed;
            }

            size_t use_count()
            {
                return (size_t)(point[0]);
            }

            counter<size_t>* getCounter()
            {
                return point;
            }

            const type& operator*() const
            {
                return *pointed;
            }

            const type& get() const
            {
                return *pointed;
            }

            // const type* const operator->() const
            // {
            //     return pointed;
            // }

            operator const type* const() const
            {
                return pointed;
            }

            operator type*()
            {
                return pointed;
            }

            ~shared_ptr()
            {
                --(point[0]);
                if((point[0]) == (size_t)0)
                {
                    if(pointed != nullptr) delete pointed;
                    if((point[1]) == (size_t)0) delete[] point;
                }
            }
    };

    template<class type>
    struct weak_ptr
    {
        private:
            type* stored;

            counter<size_t>* point;

        public:
            weak_ptr(shared_ptr<type>& store) : stored{(type*)store}, point{store.getCounter()} { ++(point[1]); }

            weak_ptr(shared_ptr<type>* store) : stored{(type*)(*store)}, point{store->getCounter()} { ++(point[1]); }

            weak_ptr(weak_ptr<type>& store) : stored{store.stored}, point{store.point} { ++(point[1]); }

            weak_ptr(weak_ptr<type>* store) : stored{store->stored}, point{store->point} { ++(point[1]); }

            bool expired()
            {
                return point[0] == (size_t)0;
            }

            shared_ptr<type> lock()
            {
                return expired() ? shared_ptr<type>{} : shared_ptr<type>{stored, point};
            }

            counter<size_t>* getCounter()
            {
                return point;
            }

            size_t use_count()
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