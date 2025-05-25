#include "hashset.h"

#include <stdlib.h>
#include <string.h>

static usize_t default_hash_fn(const usize_t key)
{
    usize_t hash = 5381;
    char *key8 = (char *)&key;
    for (int i = 0; i < 8; i++) {
        hash = ((hash << 5) + hash) + key8[i];
    }

    return hash;
}

static void default_free_fn(const usize_t key)
{
    // nothing to do for usize_t
}

static ssize_t default_cmp_fn(const usize_t key, const usize_t stored)
{
    return ((ssize_t)key) - ((ssize_t)stored);
}

struct hashset_t * hashset_create(hashset_hash_func_t hash_fn, hashset_free_func_t free_fn, hashset_cmp_func_t cmp_fn)
{
    struct hashset_t *hashset = (struct hashset_t *)malloc(sizeof(struct hashset_t));

    hashset->size = 32;
    hashset->buckets = (struct hashset_entry_t **)malloc(sizeof(struct hashset_entry_t *) * hashset->size);
    memset(hashset->buckets, 0, sizeof(struct hashset_entry_t *) * hashset->size);
    hashset->free_func = (NULL == free_fn) ? default_free_fn : free_fn;
    hashset->hash_func = (NULL == hash_fn) ? default_hash_fn : hash_fn;
    hashset->cmp_func = (NULL == cmp_fn) ? default_cmp_fn : cmp_fn;

    return hashset;
}

bool_t hashset_add(struct hashset_t *hashset, const usize_t key)
{
    if (NULL == hashset) {
        return false;
    }

    usize_t index = hashset->hash_func(key) % hashset->size;
    struct hashset_entry_t *current = hashset->buckets[index];

    while (NULL != current) {
        if (hashset->cmp_func(key, current->key) == 0) {
            return true;
        }

        current = current->next;
    }

    struct hashset_entry_t *new_entry = (struct hashset_entry_t *)malloc(sizeof(struct hashset_entry_t));
    if (NULL == new_entry) {
        return false;
    }

    new_entry->key = key;

    new_entry->next = hashset->buckets[index];
    hashset->buckets[index] = new_entry;

    return true;
}

const usize_t hashset_get(struct hashset_t *hashset, const usize_t key)
{
    if (NULL == hashset) {
        return 0;
    }

    usize_t index = hashset->hash_func(key) % hashset->size;
    struct hashset_entry_t *current = hashset->buckets[index];

    while (NULL != current) {
        if (hashset->cmp_func(key, current->key) == 0) {
            return current->key;
        }

        current = current->next;
    }

    return 0;
}

bool_t hashset_exist(struct hashset_t *hashset, const usize_t key)
{
    if (NULL == hashset) {
        return false;
    }

    usize_t index = hashset->hash_func(key) % hashset->size;
    struct hashset_entry_t *current = hashset->buckets[index];

    while (NULL != current) {
        if (hashset->cmp_func(key, current->key) == 0) {
            return true;
        }

        current = current->next;
    }

    return false;
}

bool_t hashset_remove(struct hashset_t *hashset, const usize_t key)
{
    if (NULL == hashset) {
        return false;
    }

    usize_t index = hashset->hash_func(key) % hashset->size;
    struct hashset_entry_t *current = hashset->buckets[index];
    struct hashset_entry_t *prev = NULL;

    while (NULL != current) {
        if (hashset->cmp_func(key, current->key) == 0) {
            if (NULL == prev) {
                hashset->buckets[index] = current->next;
            } else {
                prev->next = current->next;
            }

            hashset->free_func(current->key);
            free(current);
            return true;
        }

        prev = current;
        current = current->next;
    }

    return false;
}

bool_t hashset_destroy(struct hashset_t *hashset)
{
    if (NULL == hashset) {
        return true;
    }

    if (NULL != hashset->buckets) {
        for (usize_t index = 0; index < hashset->size; index++) {
            struct hashset_entry_t *current = hashset->buckets[index];

            while (NULL != current) {
                struct hashset_entry_t *next = current->next;
                hashset->free_func(current->key);
                free(current);
                current = next;
            }
        }

        free(hashset->buckets);
        hashset->buckets = NULL;
    }

    free(hashset);

    return true;
}
