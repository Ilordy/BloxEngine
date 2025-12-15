#pragma once
#include "utils/MemHelper.h"
#include "utils/blx_assertions.h"

namespace BlxStl
{
    /// @brief A simple linked list implementation.
    /// @tparam T The type of elements stored in the linked list.
    template <typename T>
    class LinkedList
    {
      public:
        class LinkedNode
        {
          public:
            LinkedNode* next;
            T value;

            LinkedNode(const T* val) : next(nullptr), value(*val) {}
            LinkedNode(const T& val) : next(nullptr), value(val) {}
            LinkedNode(T&& val) : next(nullptr), value(std::move(val)) {}
        };

        /// @brief Pointer to the head node of the linked list.
        LinkedNode* head;
        /// @brief Pointer to the tail node of the linked list.
        LinkedNode* tail;

        LinkedList() : head(nullptr), tail(nullptr) {}
        LinkedList(const T* val) : head(BlxMem::New<LinkedNode>(BLXMEMORY_TAG_LINKEDLIST, val)), tail(head) {}
        LinkedList(const T& val) : head(BlxMem::New<LinkedNode>(BLXMEMORY_TAG_LINKEDLIST, val)), tail(head) {}
        LinkedList(T&& val) : head(BlxMem::New<LinkedNode>(BLXMEMORY_TAG_LINKEDLIST, std::move(val))), tail(head) {}

        /// @brief Appends a new element to the end of the linked list.
        /// @param val The value of the new element to append.
        void Add(const T* val)
        {
            BLXASSERT(val != nullptr);

            LinkedNode* node = BlxMem::New<LinkedNode>(BLXMEMORY_TAG_LINKEDLIST, *val);

            if (!head)
            {
                head = node;
                tail = head;
            }
            else
            {
                tail->next = node;
                tail = node;
            }
        }

        /// @brief Appends a new element to the end of the linked list.
        /// @param val The value of the new element to append.
        void Add(const T& val)
        {
            LinkedNode* node = BlxMem::New<LinkedNode>(BLXMEMORY_TAG_LINKEDLIST, val);

            if (!head)
            {
                head = node;
                tail = head;
            }
            else
            {
                tail->next = node;
                tail = node;
            }
        }

        /// @brief Move version of Add to efficiently add temporary objects.
        /// @param val The value of the new element to append.
        void Add(T&& val)
        {
            LinkedNode* node = BlxMem::New<LinkedNode>(BLXMEMORY_TAG_LINKEDLIST, std::move(val));

            if (!head)
            {
                head = node;
                tail = head;
            }
            else
            {
                tail->next = node;
                tail = node;
            }
        }

        /// @brief Constructs a new element in place at the end of the linked list.
        /// @tparam ...Args The types of arguments to forward to the constructor.
        /// @param ...args The arguments to forward to the constructor of T.
        template <typename... Args>
        void Emplace(Args&&... args)
        {
            tail->next = BlxMem::New<LinkedNode>(BLXMEMORY_TAG_LINKEDLIST, std::forward<Args>(args)...);
            tail = tail->next;
        }

        ~LinkedList()
        {
            LinkedNode* currentNode = head;
            while (currentNode != nullptr)
            {
                LinkedNode* nextNode = currentNode->next;
                BlxMem::DestructAndFree(currentNode, BLXMEMORY_TAG_LINKEDLIST);
                currentNode = nextNode;
            }
        }
    };
} // namespace BlxStl