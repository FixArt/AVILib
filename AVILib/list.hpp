#ifndef AVILIB_USED_LIST
#pragma once 

// #include <cmath>
// #include <initializer_list>
// #include <functional>
#include <algorithm>
#include <type_traits>

#include "vector.hpp"

/**
 * @brief Namespace of Alternative Vector (And pair... and shared_ptr... and other things?) Implementation Library.
 * 
 */
namespace AVIL
{
    template<class type>
    class list
    {
        private:

        struct node
        {
            type stored;
            node* next;
        };

        node* next;

        node* last()
        {
            if(next == nullptr)
            {
                return nullptr;
            }
            node* processed;
            for(processed = next; processed->next != nullptr; processed = processed->next) {}
            return processed;
        }

        const node* const last() const
        {
            if(next == nullptr)
            {
                return nullptr;
            }
            node* processed;
            for(processed = next; processed != nullptr; processed = processed->next) {}
            return processed;
        }

        node* getNode(const size_t& index)
        {
            size_t current = 0;
            for(node* processed = next; processed != nullptr; processed = processed->next)
            {
                if(current == index)
                {
                    return processed;
                }
                ++current;
            }
            return nullptr;
            // return getNode(index - current);
        }

        const node* const getNode(const size_t& index) const
        {
            size_t current = 0;
            for(node* processed = next; processed != nullptr; processed = processed->next)
            {
                if(current == index)
                {
                    return processed;
                }
                ++current;
            }
            return nullptr;
            // return getNode(index - current);
        }

        node* getNodeC(const size_t& index)
        {
            size_t current = 0;
            for(node* processed = next; processed != nullptr; processed = processed->next)
            {
                if(current == index)
                {
                    return processed;
                }
                ++current;
            }
            // return nullptr;
            return getNodeC(index - current);
        }

        const node* const getNodeC(const size_t& index) const
        {
            size_t current = 0;
            for(node* processed = next; processed != nullptr; processed = processed->next)
            {
                if(current == index)
                {
                    return processed;
                }
                ++current;
            }
            // return nullptr;
            return getNodeC(index - current);
        }

        // void destruct(node* destructed)
        // {
        //     if(destructed != nullptr)
        //     {
        //         destruct(destructed->next);
        //         delete destructed;
        //     }
        // }

        public:
        list() : next{nullptr} {}

        list(const list& copied)
        {
            const node* const current = copied.next;
            if(current != nullptr)
            {
                next = new node{current->stored, nullptr};
                current = current->next;
            }
            node* processed = next;
            for(current; current != nullptr; current = current->next)
            {
                processed->next = new node{current->stored, nullptr};
                processed = processed->next;
            }
        }

        list& operator=(const list& copied)
        {
            const node* const current = copied.next;
            // if(current != nullptr)
            // {
            //     if(next != nullptr)
            //     {
            //         next->value = current->value;
            //     }
            //     else
            //     {
            //         next = new node{current->stored, nullptr};
            //     }
            //     current = current->next;
            // }
            node* processed = next;
            for(current; current != nullptr; current = current->next)
            {
                if(processed != nullptr)
                {
                    processed->value = current->value;
                }
                else
                {
                    processed = new node{current->stored, nullptr};
                }
                processed = processed->next;
            }
            return *this;
        }

        list(list&& moved)
        {
            next = moved.next;
            moved.next = nullptr;
        }

        list& operator=(list&& moved)
        {
            if(next != nullptr)
            {
                node* further = next;
                for(node* processed = next; processed != nullptr; processed = processed->next)
                {
                    further = further->next;
                    delete processed;
                }
            }
            next = moved.next;
            moved.next = nullptr;
            return *this;
        }

        void append(const type& appended)
        {
            node* processed = last();
            if(processed == nullptr)
            {
                next = new node{appended, nullptr};
                return;
            }
            processed->next = new node{appended, nullptr};
        }

        void append(const vector<type>& copied)
        {
            node* processed = last();
            for(const type& added : copied)
            {
                processed = new node{added, nullptr};
                processed = processed->next;
            }
        }

        void push(const type& value, const size_t& index)
        {
            if(index == 0)
            {
                node* previousNext = next;
                next = new node{value, previousNext};
                return;
            }
            node* processed = getNodeC(index - 1);
            if(processed == nullptr)
            {
                processed = last();
            }
            node* previousNext = processed->next;
            processed->next = new node{value, previousNext};
        }

        void pop(const size_t& index)
        {
            if(next == nullptr)
            {
                return;
            }
            if(index == 0)
            {
                node* after = next->next;
                delete next;
                next = after;
                return;
            }
            node* before = getNodeC(index - 1);
            // node* processed = getNode(index);
            node* processed = before->next;
            if(processed == nullptr)
            {
                pop(0);
                return;
            }
            node* after = processed->next;
            delete processed;
            before->next = after;
        }

        list(const vector<type>& copied)
        {
            node* processed = next;
            for(const type& added : copied)
            {
                processed->next = new node{added, nullptr};
                processed = processed->next;
            }
        }

        size_t size() const
        {
            size_t size = 0;
            node* processed = next;
            while(processed != nullptr)
            {
                ++size;
                processed = processed->next;
            }
            return size;
        }

        type& operator[](const size_t& index)
        {
            size_t current = 0;
            for(node* processed = next; processed != nullptr; processed = processed->next)
            {
                if(current == index)
                {
                    return processed->stored;
                }
                ++current;
            }
            // throw(EINVAL);
            return operator[](index - current);
        }

        const type& operator[](const size_t& index) const
        {
            size_t current = 0;
            for(node* processed = next; processed != nullptr; processed = processed->next)
            {
                if(current == index)
                {
                    return processed->stored;
                }
                ++current;
            }
            // throw(EINVAL);
            return operator[](index - current);
        }

        size_t where(const type& searched) const
        {
            size_t current = 0;
            for(node* processed = next; processed != nullptr; processed = processed->next)
            {
                if(processed->stored == searched)
                {
                    return current;
                }
                ++current;
            }
            return current; // Size returned.
        }

        type sum() const
        {
            type sum = 0;
            for(node* processed = next; processed != nullptr; processed = processed->next)
            {
                sum += processed->stored;
            }
            return sum;
        }

        size_t where(const type* const searched) const
        {
            size_t current = 0;
            for(node* processed = next; processed != nullptr; processed = processed->next)
            {
                if(&processed->stored == searched)
                {
                    return current;
                }
                ++current;
            }
            return current; // Size returned.
        }

        operator vector<type>()
        {
            vector<type> returned;
            for(node* processed = next; processed != nullptr; processed = processed->next)
            {
                returned.append(processed->stored);
            }
            return returned;
        }

        ~list()
        {
            // destruct(next);
            if(next == nullptr)
            {
                return;
            }
            node* further = next;
            for(node* processed = next; processed != nullptr; processed = further)
            {
                further = processed->next;
                delete processed;
            }
        }
    };

    template<class type>
    class dlist
    {
        protected:

        struct node
        {
            type stored;
            node* next;
            node* previous;
        };

        private:

        node* next;

        node* last()
        {
            if(next == nullptr)
            {
                return nullptr;
            }
            node* processed;
            for(processed = next; processed->next != nullptr; processed = processed->next) {}
            return processed;
        }

        const node* const last() const
        {
            if(next == nullptr)
            {
                return nullptr;
            }
            node* processed;
            for(processed = next; processed != nullptr; processed = processed->next) {}
            return processed;
        }

        node* getNode(const size_t& index)
        {
            size_t current = 0;
            for(node* processed = next; processed != nullptr; processed = processed->next)
            {
                if(current == index)
                {
                    return processed;
                }
                ++current;
            }
            return nullptr;
            // return getNode(index - current);
        }

        const node* const getNode(const size_t& index) const
        {
            size_t current = 0;
            for(node* processed = next; processed != nullptr; processed = processed->next)
            {
                if(current == index)
                {
                    return processed;
                }
                ++current;
            }
            return nullptr;
            // return getNode(index - current);
        }

        node* getNodeC(const size_t& index)
        {
            size_t current = 0;
            for(node* processed = next; processed != nullptr; processed = processed->next)
            {
                if(current == index)
                {
                    return processed;
                }
                ++current;
            }
            // return nullptr;
            return getNodeC(index - current);
        }

        const node* const getNodeC(const size_t& index) const
        {
            size_t current = 0;
            for(node* processed = next; processed != nullptr; processed = processed->next)
            {
                if(current == index)
                {
                    return processed;
                }
                ++current;
            }
            // return nullptr;
            return getNodeC(index - current);
        }

        // void destruct(node* destructed)
        // {
        //     if(destructed != nullptr)
        //     {
        //         destruct(destructed->next);
        //         delete destructed;
        //     }
        // }

        public:
        dlist() : next{nullptr} {}

        dlist(const dlist& copied)
        {
            const node* const current = copied.next;
            if(current != nullptr)
            {
                next = new node{current->stored, nullptr, nullptr};
                current = current->next;
            }
            node* processed = next;
            for(current; current != nullptr; current = current->next)
            {
                processed->next = new node{current->stored, nullptr, &processed};
                processed = processed->next;
            }
        }

        dlist& operator=(const dlist& copied)
        {
            const node* const current = copied.next;
            if(current != nullptr)
            {
                if(next != nullptr)
                {
                    next->value = current->value;
                }
                else
                {
                    next = new node{current->stored, nullptr, nullptr};
                }
                current = current->next;
            }
            // if(current != nullptr)
            // {
            //     if(next != nullptr)
            //     {
            //         next->value = current->value;
            //     }
            //     else
            //     {
            //         next = new node{current->stored, nullptr};
            //     }
            //     current = current->next;
            // }
            node* processed = next;
            for(current; current != nullptr; current = current->next)
            {
                if(processed->next != nullptr)
                {
                    processed->next->value = current->value;
                }
                else
                {
                    processed->next = new node{current->stored, nullptr, processed};
                }
                processed = processed->next;
            }
            return *this;
        }

        dlist(dlist&& moved)
        {
            next = moved.next;
            moved.next = nullptr;
        }

        dlist& operator=(dlist&& moved)
        {
            if(next != nullptr)
            {
                node* further = next;
                for(node* processed = next; processed != nullptr; processed = processed->next)
                {
                    further = further->next;
                    delete processed;
                }
            }
            next = moved.next;
            moved.next = nullptr;
            return *this;
        }

        void append(const type& appended)
        {
            node* processed = last();
            if(processed == nullptr)
            {
                next = new node{appended, nullptr, nullptr};
                return;
            }
            processed->next = new node{appended, nullptr, processed};
        }

        void append(const vector<type>& copied)
        {
            node* processed = last();
            if(processed == nullptr)
            {
                next = new node{copied[0], nullptr, nullptr};
                for(size_t i = 1; i < copied.size; ++i)
                {
                    processed->next = new node{copied[i], nullptr, processed};
                    processed = processed->next;
                }
                return;
            }
            for(const type& added : copied)
            {
                processed->next = new node{added, nullptr, processed};
                processed = processed->next;
            }
        }

        void push(const type& value, const size_t& index)
        {
            if(index == 0)
            {
                node* previousNext = next;
                next = new node{value, previousNext, nullptr};
                previousNext->previous = next;
                return;
            }
            node* processed = getNodeC(index - 1);
            if(processed == nullptr)
            {
                processed = last();
            }
            node* previousNext = processed->next;
            processed->next = new node{value, previousNext, processed};
        }

        void pop(const size_t& index)
        {
            if(next == nullptr)
            {
                return;
            }
            if(index == 0)
            {
                node* after = next->next;
                delete next;
                next = after;
                next->previous = nullptr;
                return;
            }
            node* before = getNodeC(index - 1);
            // node* processed = getNode(index);
            node* processed = before->next;
            if(processed == nullptr)
            {
                pop(0);
                return;
            }
            node* after = processed->next;
            delete processed;
            before->next = after;
            after->previous = before;
        }

        dlist(const vector<type>& copied)
        {
            node* processed = next;
            if(processed == nullptr)
            {
                next = new node{copied[0], nullptr, nullptr};
                for(size_t i = 1; i < copied.size; ++i)
                {
                    processed->next = new node{copied[i], nullptr, processed};
                    processed = processed->next;
                }
                return;
            }
            for(const type& added : copied)
            {
                processed->next = new node{added, nullptr, processed};
                processed = processed->next;
            }
        }

        size_t size() const
        {
            size_t size = 0;
            node* processed = next;
            while(processed != nullptr)
            {
                ++size;
                processed = processed->next;
            }
            return size;
        }

        type& operator[](const size_t& index)
        {
            size_t current = 0;
            for(node* processed = next; processed != nullptr; processed = processed->next)
            {
                if(current == index)
                {
                    return processed->stored;
                }
                ++current;
            }
            // throw(EINVAL);
            return operator[](index - current);
        }

        const type& operator[](const size_t& index) const
        {
            size_t current = 0;
            for(node* processed = next; processed != nullptr; processed = processed->next)
            {
                if(current == index)
                {
                    return processed->stored;
                }
                ++current;
            }
            // throw(EINVAL);
            return operator[](index - current);
        }

        size_t where(const type& searched) const
        {
            size_t current = 0;
            for(node* processed = next; processed != nullptr; processed = processed->next)
            {
                if(processed->stored == searched)
                {
                    return current;
                }
                ++current;
            }
            return current; // Size returned.
        }

        type sum() const
        {
            type sum = 0;
            for(node* processed = next; processed != nullptr; processed = processed->next)
            {
                sum += processed->stored;
            }
            return sum;
        }

        size_t where(const type* const searched) const
        {
            size_t current = 0;
            for(node* processed = next; processed != nullptr; processed = processed->next)
            {
                if(&processed->stored == searched)
                {
                    return current;
                }
                ++current;
            }
            return current; // Size returned.
        }

        operator vector<type>()
        {
            vector<type> returned;
            for(node* processed = next; processed != nullptr; processed = processed->next)
            {
                returned.append(processed->stored);
            }
            return returned;
        }

        ~dlist()
        {
            // destruct(next);
            if(next == nullptr)
            {
                return;
            }
            node* further = next;
            for(node* processed = next; processed != nullptr; processed = further)
            {
                further = processed->next;
                delete processed;
            }
        }
    };
};
#define AVILIB_USED_LIST 1
#endif
