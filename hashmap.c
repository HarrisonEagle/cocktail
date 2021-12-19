#include "cocktail.h"
#include "hashmap.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

#define HASHMAP_DEFAULT_CAPACITY 20
#define HASHMAP_MAX_LOAD 0.75f
#define HASHMAP_RESIZE_FACTOR 2
#define DEFAULT_KEYSIZE 25

hashmap* hashmap_create()
{
    hashmap* m = malloc(sizeof(hashmap));
    m->capacity = HASHMAP_DEFAULT_CAPACITY;
    m->count = 0;
    m->buckets = calloc(HASHMAP_DEFAULT_CAPACITY, sizeof(bucket));
    m->first = NULL;

    // this prevents branching in hashmap_set.
    // m->first will be treated as the "next" pointer in an imaginary bucket.
    // when the first item is added, m->first will be set to the correct address.
    m->last = (bucket*)&m->first;
    return m;
}

void hashmap_free(hashmap* m)
{
    free(m->buckets);
    free(m);
}

bucket* resize_entry(hashmap* m, bucket* old_entry)
{
    uint32_t index = old_entry->hash % m->capacity;
    while (true)
    {
        bucket* entry = &m->buckets[index];

        if (entry->key == NULL)
        {
            *entry = *old_entry; // copy data from old entry
            return entry;
        }

        index = (index + 1) % m->capacity;
    }
}

void hashmap_resize(hashmap* m)
{
    bucket* old_buckets = m->buckets;

    m->capacity *= HASHMAP_RESIZE_FACTOR;
    m->buckets = calloc(m->capacity, sizeof(bucket));
    m->last = (bucket*)&m->first;
    do
    {

        m->last->next = resize_entry(m, m->last->next);
        m->last = m->last->next;
    } while (m->last->next != NULL);

    free(old_buckets);
}

#define HASHMAP_HASH_INIT 2166136261u

// FNV-1a hash function
static inline uint32_t hash_data(char* data, size_t size)
{
    size_t nblocks = size / 8;
    uint64_t hash = HASHMAP_HASH_INIT;
    for (size_t i = 0; i < nblocks; ++i)
    {
        hash ^= (uint64_t)data[0] << 0 | (uint64_t)data[1] << 8 |
                (uint64_t)data[2] << 16 | (uint64_t)data[3] << 24 |
                (uint64_t)data[4] << 32 | (uint64_t)data[5] << 40 |
                (uint64_t)data[6] << 48 | (uint64_t)data[7] << 56;
        hash *= 0xbf58476d1ce4e5b9;
        data += 8;
    }

    uint64_t last = size & 0xff;
    switch (size % 8)
    {
        case 7:
            last |= (uint64_t)data[6] << 56; /* fallthrough */
        case 6:
            last |= (uint64_t)data[5] << 48; /* fallthrough */
        case 5:
            last |= (uint64_t)data[4] << 40; /* fallthrough */
        case 4:
            last |= (uint64_t)data[3] << 32; /* fallthrough */
        case 3:
            last |= (uint64_t)data[2] << 24; /* fallthrough */
        case 2:
            last |= (uint64_t)data[1] << 16; /* fallthrough */
        case 1:
            last |= (uint64_t)data[0] << 8;
            hash ^= last;
            hash *= 0xd6e8feb86659fd93;
    }

    // compress to a 32-bit result.
    // also serves as a finalizer.
    return hash ^ hash >> 32;
}

bucket* find_entry(hashmap* m, char* key, size_t ksize, uint32_t hash)
{
    uint32_t index = hash % m->capacity;

    while (true)
    {
        bucket* entry = &m->buckets[index];
        // kind of a thicc condition;
        // I didn't want this to span multiple if statements or functions.
        if (entry->key == NULL ||
            // compare sizes, then hashes, then key data as a last resort.
            (entry->ksize == ksize &&
             entry->hash == hash &&
             memcmp(entry->key, key, ksize) == 0))
        {
            // return the entry if a match or an empty bucket is found
            return entry;
        }
        index = (index + 1) % m->capacity;
    }
}

void routing_set(hashmap* m, char* key, ctRoute* val)
{
    if (m->count + 1 > HASHMAP_MAX_LOAD * m->capacity)
        hashmap_resize(m);
    uint32_t hash = hash_data(key, DEFAULT_KEYSIZE);
    bucket* entry = find_entry(m, key, DEFAULT_KEYSIZE, hash);
    if (entry->key == NULL)
    {
        m->last->next = entry;
        m->last = entry;
        entry->next = NULL;

        ++m->count;

        entry->key = key;
        entry->ksize = DEFAULT_KEYSIZE;
        entry->hash = hash;
    }
    entry->routing_value = val;
}

ctRoute* routing_get(hashmap* m, char* key)
{
    uint32_t hash = hash_data(key, DEFAULT_KEYSIZE);
    bucket* entry = find_entry(m, key, DEFAULT_KEYSIZE, hash);

    // if there is no match, output val will just be NULL
    return entry->routing_value;
}

