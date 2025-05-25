#ifndef _KTNLIBC_HASHSET_H 
#define _KTNLIBC_HASHSET_H

#include "type.h"

typedef usize_t (*hashset_hash_func_t)(const usize_t key);
typedef void (*hashset_free_func_t)(const usize_t key);
typedef ssize_t (*hashset_cmp_func_t)(const usize_t key, const usize_t stored);

struct hashset_entry_t {
    usize_t key;
    struct hashset_entry_t *next;
};

struct hashset_t {
    hashset_hash_func_t hash_func;
    hashset_free_func_t free_func;
    hashset_cmp_func_t cmp_func;
    struct hashset_entry_t **buckets;
    usize_t size;
};

struct hashset_t * hashset_create(hashset_hash_func_t hash_fn, hashset_free_func_t free_fn, hashset_cmp_func_t cmp_fn);
bool_t hashset_destroy(struct hashset_t *hashset);
bool_t hashset_add(struct hashset_t *hashset, const usize_t key);
bool_t hashset_exist(struct hashset_t *hashset, const usize_t key);
bool_t hashset_remove(struct hashset_t *hashset, const usize_t key);
const usize_t hashset_get(struct hashset_t *hashset, const usize_t key);

#endif //_KTNLIBC_HASHSET_H
