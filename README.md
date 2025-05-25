# ktnlibc
C library with shared code I tend to use across my C projects, released and made available under [MIT License](https://github.com/Kirdow/ktnlibc/blob/master/LICENSE).

## Usage
To use **ktnlibc**, assuming you use CMake, you should be able to just include it into your project's CMakeLists.txt file like this:
```cmake
# Include the package
set(KTNLIBC_VERSION 1.1)
include(FetchContent)
FetchContent_Declare(
    ktnlibc
    GIT_REPOSITORY https://github.com/Kirdow/ktnlibc.git
    GIT_TAG v${KTNLIBC_VERSION}
)

FetchContent_MakeAvailable(ktnlibc)

# Add link target
target_link_libraries(${PROJECT_NAME} PRIVATE ktnlibc)
```

## Features
Includes 1 definiton header and 4 collection headers.

### Definition headers
- **type.h**
    - Contains standard ints, as well as declaring `usize_t` and `ssize_t` typedef based on the 64-bit counterpart.
    - Defined boolean type as `bool_t` based off of `char`. Also re-defined `true`, `false` and `NULL` with the definitions I tend to use in my projects.

### Collection headers
- **array_set.h**
    - This is a non-dynamic non-hashed set which stores values for later lookup and manipulation.
    - Currently only stores `usize_t` as defined in `type.h`.
    - You initialize it using an initial capacity, then you can add and remove up until that capacity.
        - Removing values within the set, rather than shifting the entire set, it moves the last element into its place.
- **vector_list.h**
    - This is a dynamic array/list which stores an indexed list of elements in series.
    - Currently only stores `usize_t` as defined in `type.h`.
    - You can initialize it with a capacity. Once it reaches the capacity, it's automatically resized (increasing capacity by `max(1, 50%)`).
    - **NOTE:** Due to the usage of `realloc`, never store pointers to elements stored in the `vector_list`. If `realloc` ever moves the pointer due to the block not being large enough, your pointers will now point to memory not currently owned by your program, potentially.
    - When an element is removed inside of the `vector_list`, the entire `vector_list` is shifted by 1.
- **vector_stack.h**
    - This is a wrapper of `vector_list.h` which implements your typical Stack functionality like `push`, `pop`, `peek`, `length`, `empty`etc.
- **hashmap.h**
    - This is a dynamic map that uses a hashing function to store and look-up values efficiently and fast.
    - Supports only `char *` as Key. Keys stored in `hashmap` are copied and owned by `hashmap`.
    - Supports any types for as Value. Values stored in `hashmap` are copied and owned by `hashmap` using user-provided `free` and `copy` functions.
        - If `hashmap` ownership isn't desired, simply make the provided function not copy nor free, or simply pass `NULL` functions and let `hashmap` omit the copy/free for you.
    - Currently uses 32 buckets for storage.
- **hashset.h**
    - This is a dynamic set that uses a hashing function to store and look-up values efficiently and fast.
    - Supports any types using user-provided `hash`, `free` and `cmp` functions.
    - Currently uses 32 buckets for storage.

## License
**ktnlibc** is released and the source is available under [MIT License](https://github.com/Kirdow/ktnlibc/blob/master/LICENSE).
