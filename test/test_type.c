#include "test.h"
#include "test_type.h"

#include "type.h"

static void test_BoolSize(void)
{
    TEST_ASSERT_EQUAL_UINT8_MESSAGE(sizeof(char), sizeof(bool_t), "sizeof(bool_t) must equal sizeof(char)");
}

static void test_SSizeTSize(void)
{
    TEST_ASSERT_EQUAL_UINT8_MESSAGE(sizeof(int64_t), sizeof(ssize_t), "sizeof(ssize_t) must equal sizeof(int64_t)");
}

static void test_USizeTSize(void)
{
    TEST_ASSERT_EQUAL_UINT8_MESSAGE(sizeof(uint64_t), sizeof(usize_t), "sizeof(usize_t) must equal sizeof(uint64_t)");
}

void _test_type(void)
{
    RUN_TEST(test_BoolSize);
    RUN_TEST(test_SSizeTSize);
    RUN_TEST(test_USizeTSize);
}
