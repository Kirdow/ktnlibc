#ifndef _KTNLIBC_TYPE_H
#define _KTNLIBC_TYPE_H

#include <stdint.h>

typedef uint64_t usize_t;

#ifdef _WIN32
    typedef int64_t ssize_t;
#else
    #include <sys/types.h>
#endif

typedef char bool_t;

#ifdef true
#undef true
#endif
#define true 1

#ifdef false
#undef false
#endif
#define false 0

#ifdef NULL
#undef NULL
#endif
#define NULL ((void*)0)

#endif // _KTNLIBC_TYPE_H
