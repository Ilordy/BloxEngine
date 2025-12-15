#pragma once
#include "MemHelper.h"
#include "core/blx_defines.h"
#include "core/blx_logger.h"
#include "core/blx_memory.h"
#include "utils/MemHelper.h"

#include <functional>
#include <utility>

#ifndef BLX_DEFAULT_HASH_TABLE_SIZE
#define BLX_DEFAULT_HASH_TABLE_SIZE 97
#endif

namespace BlxStl
{
    template <typename T>
    struct Hash
    {
        uint64 operator()(const T& key)
        {
            return std::hash<T>()(key);
        }
    };

    template <typename T>
    struct EqualTo
    {
        bool operator()(const T& a, const T& b)
        {
            return a == b;
        }
    };

    /**
     * @brief A simple hashmap (hash table) implementation.
     *
     * @tparam K Key type.
     * @tparam V Value type.
     * @tparam Hasher Hashing functor.
     * @tparam Equal Equality comparison functor.
     * @note This implementation uses separate chaining for collision resolution.
     */
    template <typename K, typename V, typename Hasher = BlxStl::Hash<K>, typename Equal = BlxStl::EqualTo<K>>
    class Map
    {

      public:
        /**
         * @brief Key-value pair structure for map use.
         *
         */
        struct KeyValuePair
        {
            K key;
            V value;
            KeyValuePair* next;
            KeyValuePair() : next(nullptr) {}
            KeyValuePair(const K& k, const V& v) : key(k), value(v), next(nullptr) {}
            KeyValuePair(K&& k, V&& v) : key(std::move(k)), value(std::move(v)), next(nullptr) {}
        };

      private:
        KeyValuePair** buckets;
        uint64 bucketCount;
        Hasher hasher;
        Equal equalTo;

      public:
        Map(uint64 size = BLX_DEFAULT_HASH_TABLE_SIZE) : bucketCount(size)
        {
            buckets = BlxMem::NewArray<KeyValuePair*>(bucketCount, BLXMEMORY_TAG_DICT);
        }

        ~Map()
        {
            for (uint64 i = 0; i < bucketCount; i++)
            {
                KeyValuePair* current = buckets[i];
                while (current)
                {
                    KeyValuePair* toDelete = current;
                    current = current->next;
                    BlxMem::DestructAndFree(toDelete, BLXMEMORY_TAG_DICT);
                }
            }

            BlxMem::DeleteArray(buckets, bucketCount, BLXMEMORY_TAG_DICT);
        }

        const V* Find(const K& key) const
        {
            uint64 index = hasher(key) % bucketCount;
            KeyValuePair* current = buckets[index];
            while (current)
            {
                if (equalTo(current->key, key))
                {
                    return &current->value;
                }
                current = current->next;
            }
            return nullptr;
        }

        const V* Find(const K* key) const
        {
            uint64 index = hasher(*key) % bucketCount;
            KeyValuePair* current = buckets[index];
            while (current)
            {
                if (equalTo(current->key, *key))
                {
                    return &current->value;
                }
                current = current->next;
            }
            return nullptr;
        }

        V* Find(const K& key)
        {
            uint64 index = hasher(key) % bucketCount;
            KeyValuePair* current = buckets[index];
            while (current)
            {
                if (equalTo(current->key, key))
                {
                    return &current->value;
                }
                current = current->next;
            }
            return nullptr;
        }

        V* Find(const K* key)
        {
            uint64 index = hasher(*key) % bucketCount;
            KeyValuePair* current = buckets[index];
            while (current)
            {
                if (equalTo(current->key, *key))
                {
                    return &current->value;
                }
                current = current->next;
            }
            return nullptr;
        }

        inline bool Contains(const K& key)
        {
            return Find(key) != nullptr;
        }

        inline bool Contains(const K* key)
        {
            return Find(key) != nullptr;
        }

        void Add(const K& key, const V& value)
        {
            if (Contains(key))
            {
                // TODO: Add stack trace here.
                BLXWARNING("A Key that already exist has tried to be added again in this hashmap.");
                return;
            }

            uint64 index = hasher(key) % bucketCount;
            KeyValuePair* newNode = BlxMem::New<KeyValuePair>(BLXMEMORY_TAG_DICT, key, value);
            if (!buckets[index])
            {
                buckets[index] = newNode;
            }
            else
            {
                KeyValuePair* current = buckets[index];
                while (current->next)
                {
                    current = current->next;
                }
                current->next = newNode;
            }
        }

        void Add(K&& key, V&& value)
        {
            if (Contains(key))
            {
                // TODO: Add stack trace here.
                BLXWARNING("A Key that already exist has tried to be added again in this hashmap.");
                return;
            }

            uint64 index = hasher(key) % bucketCount;
            KeyValuePair* newNode = BlxMem::New<KeyValuePair>(BLXMEMORY_TAG_DICT, std::move(key), std::move(value));
            if (!buckets[index])
            {
                buckets[index] = newNode;
            }
            else
            {
                KeyValuePair* current = buckets[index];
                while (current->next)
                {
                    current = current->next;
                }
                current->next = newNode;
            }
        }

        V& operator[](const K& key)
        {
            V* value = Find(key);
            if (value)
            {
                return *value;
            }
            else
            {
                // If key does not exist, add a default-constructed value.
                Add(key, V());
                return *Find(key);
            }
        }

        V& operator[](const K* key)
        {
            V* value = Find(key);
            if (value)
            {
                return *value;
            }
            else
            {
                // If key does not exist, add a default-constructed value.
                Add(*key, V());
                return *Find(key);
            }
        }

        // TODO: In the future add support to emplace values using your own BlxStl::MakePair.

        // template <typename... ValueArgs>
        // void Emplace(const K& key, ValueArgs&&... valueArgs)
        // {
        //     if (Contains(key))
        //     {
        //         // TODO: Add stack trace here.
        //         BLXWARNING("A Key that already exist has tried to be added again in this hashmap.");
        //         return;
        //     }

        //     V value(std::forward<ValueArgs>(valueArgs)...);
        //     uint64 index = hasher(key) % bucketCount;

        //     MapNode* newNode = blxAllocate(sizeof(MapNode), BLXMEMORY_TAG_DICT);
        //     BlxMem::ConstructAt(newNode, key, std::move(value));
        //     //MapNode* newNode = BlxMem::New<MapNode>(BLXMEMORY_TAG_DICT, std::move(key), std::move(value));
        //     if (!buckets[index])
        //     {
        //         buckets[index] = newNode;
        //     }
        //     else
        //     {
        //         MapNode* current = buckets[index];
        //         while (current->next)
        //         {
        //             current = current->next;
        //         }
        //         current->next = newNode;
        //     }
        // }

        void Remove(const K& key)
        {
            uint64 index = hasher(key) % bucketCount;
            KeyValuePair* current = buckets[index];
            KeyValuePair* prev = nullptr;
            while (current)
            {
                if (equalTo(current->key, key))
                {
                    if (prev)
                    {
                        prev->next = current->next;
                    }
                    else
                    {
                        buckets[index] = current->next;
                    }

                    BlxMem::DestructAndFree(current, BLXMEMORY_TAG_DICT);
                    return;
                }

                prev = current;
                current = current->next;
            }
        }

        void Remove(const K* key)
        {
            uint64 index = hasher(*key) % bucketCount;
            KeyValuePair* current = buckets[index];
            KeyValuePair* prev = nullptr;
            while (current)
            {
                if (equalTo(current->key, *key))
                {
                    if (prev)
                    {
                        prev->next = current->next;
                    }
                    else
                    {
                        buckets[index] = current->next;
                    }

                    BlxMem::DestructAndFree(current, BLXMEMORY_TAG_DICT);
                    return;
                }

                prev = current;
                current = current->next;
            }
        }
    };
} // namespace BlxStl