#ifndef _KTNLIBC_VECTOR_STACK_H
#define _KTNLIBC_VECTOR_STACK_H

#include "type.h"
#include "vector_list.h"

struct stack_t {
    struct vector_t vec;
};

bool_t stack_alloc(struct stack_t *stack);
bool_t stack_release(struct stack_t *stack);
bool_t stack_push(struct stack_t *stack, usize_t item);
bool_t stack_empty(struct stack_t *stack);
bool_t stack_can_pop(struct stack_t *stack);
usize_t stack_pop(struct stack_t *stack, usize_t _default);
usize_t stack_peek(struct stack_t *stack, usize_t _default);
bool_t stack_try_pop(struct stack_t *stack, usize_t *item);
bool_t stack_try_peek(struct stack_t *stack, usize_t *item);
usize_t stack_size(struct stack_t *stack);

#endif //_KTNLIBC_VECTOR_STACK_H
