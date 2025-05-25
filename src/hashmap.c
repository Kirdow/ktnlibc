#include "hashmap.h"

#include <stdlib.h>
#include <string.h>

static void default_free_fn(const usize_t value)
{}

static usize_t default_copy_fn(const usize_t value)
{
    return value;
}

struct hashmap_t * hashmap_create(hashmap_free_func_t free_fn, hashmap_copy_func_t copy_fn)
{
    struct hashmap_t *hashmap = (struct hashmap_t *)malloc(sizeof(struct hashmap_t));

    hashmap->size = 32;
    hashmap->buckets = (struct hashmap_entry_t **)malloc(sizeof(struct hashmap_entry_t *) * hashmap->size);
    memset(hashmap->buckets, 0, sizeof(struct hashmap_entry_t *) * hashmap->size);

    hashmap->free_func = (NULL == free_fn) ? default_free_fn : free_fn;
    hashmap->copy_func = (NULL == copy_fn) ? default_copy_fn : copy_fn;

    return hashmap;
}

static usize_t hash(const char *key)
{
    usize_t hash = 5381;
    ssize_t c;
    while ((c = *key++)) {
        hash = ((hash << 5) + hash) + c;
    }

    return hash;
}

bool_t hashmap_put(struct hashmap_t *hashmap, const char *key, const usize_t value)
{
    if (NULL == hashmap) {
        return false;
    }

    usize_t index = hash(key) % hashmap->size;
    struct hashmap_entry_t *current = hashmap->buckets[index];
    
    while (NULL != current) {
        if (strcmp(current->key, key) == 0) {
            hashmap->free_func(current->value);
            current->value = hashmap->copy_func(value);
            return true;
        }

        current = current->next;
    }

    struct hashmap_entry_t *new_entry = (struct hashmap_entry_t *)malloc(sizeof(struct hashmap_entry_t));
    if (NULL == new_entry) {
        return false;
    }

    new_entry->key = strdup(key);
    new_entry->value = hashmap->copy_func(value);

    new_entry->next = hashmap->buckets[index];
    hashmap->buckets[index] = new_entry;

    return true;
}

const usize_t hashmap_get(struct hashmap_t *hashmap, const char *key, const usize_t _default)
{
    if (NULL == hashmap) {
        return _default;
    }

    usize_t index = hash(key) % hashmap->size;
    struct hashmap_entry_t *current = hashmap->buckets[index];

    while (NULL != current) {
        if (strcmp(current->key, key) == 0) {
            return current->value;
        }

        current = current->next;
    }

    return _default;
}

bool_t hashmap_remove(struct hashmap_t *hashmap, const char *key)
{
    if (NULL == hashmap) {
        return false;
    }

    usize_t index = hash(key) % hashmap->size;
    struct hashmap_entry_t *current = hashmap->buckets[index];
    struct hashmap_entry_t *prev = NULL;

    while (NULL != current) {
        if (strcmp(current->key, key) == 0) {
            if (NULL == prev) {
                hashmap->buckets[index] = current->next;
            } else {
                prev->next = current->next;
            }

            free(current->key);
            hashmap->free_func(current->value);
            free(current);
            return true;
        }

        prev = current;
        current = current->next;
    }

    return false;
}

bool_t hashmap_destroy(struct hashmap_t *hashmap)
{
    if (NULL == hashmap) {
        return true;
    }

    if (NULL != hashmap->buckets) {
        for (usize_t index = 0; index < hashmap->size; index++) {
            struct hashmap_entry_t *current = hashmap->buckets[index];

            while (NULL != current) {
                struct hashmap_entry_t *next = current->next;
                free(current->key);
                hashmap->free_func(current->value);
                free(current);
                current = next;
            }
        }

        free(hashmap->buckets);
        hashmap->buckets = NULL;
    }

    free(hashmap);

    return true;
}

void hashmap_string_free(const usize_t value)
{
    char *str = (char *)value;
    free(str);
}

usize_t hashmap_string_copy(const usize_t value)
{
    char *str = (char *)value;
    return (usize_t)strdup(str);
}
