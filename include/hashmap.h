#ifndef _KTNLIBC_HASHMAP_H
#define _KTNLIBC_HASHMAP_H

#include "type.h"

typedef void (*hashmap_free_func_t)(const usize_t value);
typedef usize_t (*hashmap_copy_func_t)(const usize_t value);

struct hashmap_entry_t {
    char *key;
    usize_t value;
    struct hashmap_entry_t *next;
};

struct hashmap_t {
    hashmap_free_func_t free_func;
    hashmap_copy_func_t copy_func;

    struct hashmap_entry_t **buckets;
    usize_t size;
};

struct hashmap_t * hashmap_create(hashmap_free_func_t free_fn, hashmap_copy_func_t copy_fn);
bool_t hashmap_destroy(struct hashmap_t *hashmap);
bool_t hashmap_put(struct hashmap_t *hashmap, const char *key, const usize_t value);
const usize_t hashmap_get(struct hashmap_t *hashmap, const char *key, const usize_t _default);
bool_t hashmap_remove(struct hashmap_t *hashmap, const char *key);

void hashmap_string_free(const usize_t value);
usize_t hashmap_string_copy(const usize_t value);

#define hashmap_string_create() hashmap_create(hashmap_string_free, hashmap_string_copy)
#define hashmap_T_put(T, H, K, V) hashmap_put((H), (K), ((const usize_t)(V)))
#define hashmap_T_get(T, H, K, D) (T)hashmap_get((H), (K), ((const usize_t)(D)))
#define hashmap_T_remove(T, H, K) hashmap_remove((H), (K))

#endif //_KTNLIBC_HASHMAP_H
