//
// Created by hidetomo8111f on 2021/12/19.
//

#ifndef COCKTAIL_HASHMAP_H
#define COCKTAIL_HASHMAP_H


#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

typedef struct bucket
{
    // `next` must be the first struct element.
    // changing the order will break multiple functions
    struct bucket* next;

    // key, key size, key hash, and associated value
    char* key;
    size_t ksize;
    uint32_t hash;
    ctRoute* routing_value;
}bucket;

typedef struct hashmap
{
    bucket* buckets;
    int capacity;
    int count;
    // a linked list of all valid entries, in order
    bucket* first;
    // lets us know where to add the next element
    bucket* last;
}hashmap;


hashmap* hashmap_create();

void routing_set(hashmap* m, char* key, ctRoute* val);

ctRoute* routing_get(hashmap* m, char* key);

#endif //COCKTAIL_HASHMAP_H
