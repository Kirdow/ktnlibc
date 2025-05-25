#include "test.h"
#include "test_hashset.h"

#include "hashset.h"

static void test_Create(void)
{
    struct hashset_t *hashset = hashset_create(NULL, NULL, NULL);

    TEST_ASSERT_NOT_NULL_MESSAGE(hashset, "Hashmap creation should not fail");

    hashset_destroy(hashset);
}

static void test_Put(void)
{
    struct hashset_t *hashset = hashset_create(NULL, NULL, NULL);
    
    // Make sure we can put 5 fields
    TEST_ASSERT_TRUE_MESSAGE(hashset_add(hashset, 7), "Put should not fail");
    TEST_ASSERT_TRUE_MESSAGE(hashset_add(hashset, 15), "Put should not fail");
    TEST_ASSERT_TRUE_MESSAGE(hashset_add(hashset, 1337), "Put should not fail");
    TEST_ASSERT_TRUE_MESSAGE(hashset_add(hashset, 17), "Put should not fail");
    TEST_ASSERT_TRUE_MESSAGE(hashset_add(hashset, 999999), "Put should not fail");
    TEST_ASSERT_TRUE_MESSAGE(hashset_add(hashset, 111732), "Put should not fail");

    // Make sure we can override fields
    TEST_ASSERT_TRUE_MESSAGE(hashset_add(hashset, 1972), "Put-override should not fail");
    TEST_ASSERT_TRUE_MESSAGE(hashset_add(hashset, 1958), "Put-override should not fail");

    hashset_destroy(hashset);
}

static void test_Get(void)
{
    struct hashset_t *hashset = hashset_create(NULL, NULL, NULL);


    hashset_add(hashset, 15);
    hashset_add(hashset, 1337);
    hashset_add(hashset, 17);
    hashset_add(hashset, 999999);
    hashset_add(hashset, 111732);

    usize_t result;

    // Test getting existing value
    result = hashset_get(hashset, 1337);
    TEST_ASSERT_EQUAL_UINT64_MESSAGE(1337, result, "Getting existing value should match expected value");

    // Test another existing value
    result = hashset_get(hashset, 17);
    TEST_ASSERT_EQUAL_UINT64_MESSAGE(17, result, "Getting existing value should match expected value");

    // Test non-existing value
    result = hashset_get(hashset, 9);
    TEST_ASSERT_EQUAL_UINT64_MESSAGE(0, result, "Getting non-existing value should yield null");

    // Add new values
    hashset_add(hashset, 1972);
    hashset_add(hashset, 1958);

    // Test getting overwritten value
    result = hashset_get(hashset, 1972);
    TEST_ASSERT_EQUAL_UINT64_MESSAGE(1972, result, "Getting overwritten value should match expected value");

    // Test getting another overwritten value
    result = hashset_get(hashset, 1958);
    TEST_ASSERT_EQUAL_UINT64_MESSAGE(1958, result, "Getting overwritten value should match expected value");

    // Test getting existing-overwritten value
    result = hashset_get(hashset, 111732);
    TEST_ASSERT_EQUAL_UINT64_MESSAGE(111732, result, "Getting non-overwritten value should match expected value");

    // Test getting non-existing value
    result = hashset_get(hashset, 9);
    TEST_ASSERT_EQUAL_UINT64_MESSAGE(0, result, "Getting non-existing value should yield null");

    hashset_destroy(hashset);
}

static void test_Remove(void)
{
    struct hashset_t *hashset = hashset_create(NULL, NULL, NULL);

    hashset_add(hashset, 15);
    hashset_add(hashset, 1972);

    usize_t result;
    bool_t success;

    // Test getting existing value
    result = hashset_get(hashset, 1972);
    TEST_ASSERT_EQUAL_UINT64_MESSAGE(1972, result, "Getting existing value should match expected value");

    // Remove value
    success = hashset_remove(hashset, 1972);

    // Assert removal
    TEST_ASSERT_TRUE_MESSAGE(success, "Removing existing value should not fail");

    // Test getting removed value
    result = hashset_get(hashset, 9);
    TEST_ASSERT_EQUAL_UINT64_MESSAGE(0, result, "Getting removed value should yield null"); 

    // Test getting not-removed value
    result = hashset_get(hashset, 15);
    TEST_ASSERT_EQUAL_UINT64_MESSAGE(15, result, "Getting not-removed value should match expected value");

    // Re-add value
    success = hashset_add(hashset, 1337);

    // Assert re-adding
    TEST_ASSERT_TRUE_MESSAGE(success, "Re-adding value should not fail");

    // Test getting re-added value
    result = hashset_get(hashset, 1337);
    TEST_ASSERT_EQUAL_UINT64_MESSAGE(1337, result, "Getting re-added value should match expected value");

    // Modify re-added value
    success = hashset_add(hashset, 1972);

    // Assert modification
    TEST_ASSERT_TRUE_MESSAGE(success, "Modifying re-added value should not fail");

    // Test getting modified value
    result = hashset_get(hashset, 1972);
    TEST_ASSERT_EQUAL_UINT64_MESSAGE(1972, result, "Getting modified re-added value should match expected value");

    // Try removing non-existing value
    success = hashset_remove(hashset, 32);

    // Assert removing non-existing value fails
    TEST_ASSERT_FALSE_MESSAGE(success, "Removing non-existing value should fail");

    hashset_destroy(hashset);
}

void _test_hashset(void)
{
    RUN_TEST(test_Create);
    RUN_TEST(test_Put);
    RUN_TEST(test_Get);
    RUN_TEST(test_Remove);
}
