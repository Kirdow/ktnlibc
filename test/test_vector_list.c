#include "test.h"
#include "test_vector_list.h"

#include "vector_list.h"

static void test_CapacityAlloc(void)
{
    struct vector_t vec;
    vector_alloc(&vec, 1);

    TEST_ASSERT_EQUAL_UINT64_MESSAGE(1, vec.capacity, "Capacity should equal 1");

    vector_add(&vec, 10);

    TEST_ASSERT_EQUAL_UINT64_MESSAGE(1, vec.capacity, "Capacity should stay 1 after first add");

    vector_add(&vec, 20);

    TEST_ASSERT_EQUAL_UINT64_MESSAGE(2, vec.capacity, "Capacity should equal 2 after second add");

    vector_add(&vec, 30);

    TEST_ASSERT_EQUAL_UINT64_MESSAGE(3, vec.capacity, "Capacity should equal 3 after third add");

    vector_add(&vec, 40);

    TEST_ASSERT_EQUAL_UINT64_MESSAGE(4, vec.capacity, "Capacity should equal 4 after fourth add");

    vector_add(&vec, 50);

    TEST_ASSERT_EQUAL_UINT64_MESSAGE(6, vec.capacity, "Capacity should equal 6 after fifth add");

    vector_add(&vec, 60);

    TEST_ASSERT_EQUAL_UINT64_MESSAGE(6, vec.capacity, "Capacity should equal 6 after sixth add");

    vector_add(&vec, 70);

    TEST_ASSERT_EQUAL_UINT64_MESSAGE(9, vec.capacity, "Capacity should equal 9 after seventh add");

    vector_add(&vec, 80);
    vector_add(&vec, 90);

    TEST_ASSERT_EQUAL_UINT64_MESSAGE(9, vec.capacity, "Capacity should equal 9 after nineth add");

    vector_add(&vec, 100);

    TEST_ASSERT_EQUAL_UINT64_MESSAGE(13, vec.capacity, "Capacity should equal 13 after tenth add");

    vector_add(&vec, 110);
    vector_add(&vec, 120);
    vector_add(&vec, 130);

    TEST_ASSERT_EQUAL_UINT64_MESSAGE(13, vec.capacity, "Capacity should equal 13 after thirteenth add");

    vector_add(&vec, 140);

    TEST_ASSERT_EQUAL_UINT64_MESSAGE(19, vec.capacity, "Capacity should equal 19 after fourteenth add");

    for (usize_t i = 0; i < 14; i++) {
        usize_t value = (i + 1) * 10;

        char buffer[128];
        snprintf(buffer, 127, "Index %lu should equal %lu", i, value);
        buffer[127] = '\0';

        TEST_ASSERT_EQUAL_UINT64_MESSAGE(value, vec.data[i], buffer);
    }

    vector_clear(&vec);

    TEST_ASSERT_EQUAL_UINT64_MESSAGE(19, vec.capacity, "Capacity should equal 13 after clear");

    for (usize_t i = 0; i < 19; i++) {
        usize_t value = 0;

        char buffer[128];
        snprintf(buffer, 127, "Index %lu should equal %lu after clear", i, value);
        buffer[127] = '\0';

        TEST_ASSERT_EQUAL_UINT64_MESSAGE(value, vec.data[i], buffer);
    }

    vector_release(&vec);

    TEST_ASSERT_EQUAL_UINT64_MESSAGE(0, vec.capacity, "Capacity should be zero after release");
    TEST_ASSERT_EQUAL_UINT64_MESSAGE(NULL, vec.data, "Data should be null after release");
}

static void test_EraseAt(void)
{
    struct vector_t vec;
    vector_alloc(&vec, 8);

    for (usize_t i = 0; i < 8; i++) {
        vector_add(&vec, (i + 1) * 10);
    }

    vector_erase_at(&vec, 5);

    usize_t j = 0;
    for (usize_t i = 0; i < 8; i++) {
        if (i == 5) {
            continue;
        }

        usize_t value = (i + 1) * 10;

        char buffer[128];
        snprintf(buffer, 127, "Index %lu should equal %lu after vector_erase_at(&vec, 5)", j, value);
        buffer[127] = '\0';

        TEST_ASSERT_EQUAL_UINT64_MESSAGE(value, vec.data[j], buffer);

        ++j;
    }

    vector_release(&vec);
}

static void test_EraseAll(void)
{
    struct vector_t vec;
    vector_alloc(&vec, 8);

    for (usize_t i = 0; i < 2; i++) {
        vector_add(&vec, 20);
    }

    for (usize_t i = 0; i < 3; i++) {
        vector_add(&vec, 50);
    }

    for (usize_t i = 0; i < 3; i++) {
        vector_add(&vec, 40);
    }

    for (usize_t i = 0; i < 8; i++) {
        usize_t value = i < 2 ? 20 : i < 5 ? 50 : 40;

        char buffer[128];
        snprintf(buffer, 127, "Index %lu should equal %lu after vector_add x8", i, value);
        buffer[127] = '\0';

        TEST_ASSERT_EQUAL_UINT64_MESSAGE(value, vec.data[i], buffer);
    }

    vector_erase_all(&vec, 50);

    for (usize_t i = 0; i < 8; i++) {
        usize_t value = i < 2 ? 20 : i < 5 ? 40 : 0;

        char buffer[128];
        snprintf(buffer, 127, "Index %lu should equal %lu after vector_erase_all(&vec, 50)", i, value);
        buffer[127] = '\0';

        TEST_ASSERT_EQUAL_UINT64_MESSAGE(value, vec.data[i], buffer);
    }

    TEST_ASSERT_EQUAL_UINT64_MESSAGE(5, vec.len, "Length should equal 5 after removing 3 equal elements");

    vector_release(&vec);
}

static void test_Find(void)
{
    struct vector_t vec;
    vector_alloc(&vec, 8);

    for (usize_t i = 0; i < 8; i++) {
        vector_add(&vec, (i + 1) * 10);
    }

    usize_t index;
    bool_t result = vector_find(&vec, 50, &index);

    TEST_ASSERT_EQUAL_UINT64_MESSAGE(true, result, "Finding 50 should yield a successful result");
    TEST_ASSERT_EQUAL_UINT64_MESSAGE(4, index, "Finding 50 should yield an index of 4");

    result = vector_find(&vec, 55, &index);

    TEST_ASSERT_EQUAL_UINT64_MESSAGE(false, result, "Finding 55 should yield an unsuccessful result");
    TEST_ASSERT_EQUAL_UINT64_MESSAGE(4, index, "Unsuccessful results should leave output untouched");

    vector_release(&vec);
}

static void test_At(void)
{
    struct vector_t vec;
    vector_alloc(&vec, 8);

    for (usize_t i = 0; i < 8; i++) {
        vector_add(&vec, (i + 1) * 10);
    }

    usize_t item;
    bool_t result = vector_at(&vec, 4, &item);

    TEST_ASSERT_EQUAL_UINT64_MESSAGE(true, result, "At 4 should yield a successful result");
    TEST_ASSERT_EQUAL_UINT64_MESSAGE(50, item, "At 4 should yild an item of 50");
    
    result = vector_at(&vec, 8, &item);

    TEST_ASSERT_EQUAL_UINT64_MESSAGE(false, result, "At 8 should yield an unsuccessful result");
    TEST_ASSERT_EQUAL_UINT64_MESSAGE(50, item, "Unsuccessful results should leave output untouched");

    vector_release(&vec);
}

void _test_vector_list(void)
{
    RUN_TEST(test_CapacityAlloc);
    RUN_TEST(test_EraseAt);
    RUN_TEST(test_EraseAll);
    RUN_TEST(test_Find);
    RUN_TEST(test_At);
}
