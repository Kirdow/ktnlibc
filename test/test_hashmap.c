#include "test.h"
#include "test_hashmap.h"

#include "hashmap.h"

static void test_Create(void)
{
    struct hashmap_t *hashmap = hashmap_create(NULL, NULL);

    TEST_ASSERT_NOT_NULL_MESSAGE(hashmap, "Hashmap creation should not fail");

    hashmap_destroy(hashmap);
}

static void test_Put(void)
{
    struct hashmap_t *hashmap = hashmap_create(NULL, NULL);
    
    // Make sure we can put 5 fields
    TEST_ASSERT_TRUE_MESSAGE(hashmap_put(hashmap, "Alice", 7), "Put should not fail");
    TEST_ASSERT_TRUE_MESSAGE(hashmap_put(hashmap, "Alice", 15), "Put should not fail");
    TEST_ASSERT_TRUE_MESSAGE(hashmap_put(hashmap, "Bob", 1337), "Put should not fail");
    TEST_ASSERT_TRUE_MESSAGE(hashmap_put(hashmap, "Peter", 17), "Put should not fail");
    TEST_ASSERT_TRUE_MESSAGE(hashmap_put(hashmap, "Gwen", 999999), "Put should not fail");
    TEST_ASSERT_TRUE_MESSAGE(hashmap_put(hashmap, "David", 111732), "Put should not fail");

    // Make sure we can override fields
    TEST_ASSERT_TRUE_MESSAGE(hashmap_put(hashmap, "Bob", 1972), "Put-override should not fail");
    TEST_ASSERT_TRUE_MESSAGE(hashmap_put(hashmap, "Peter", 1958), "Put-override should not fail");

    hashmap_destroy(hashmap);
}

static void test_Get(void)
{
    struct hashmap_t *hashmap = hashmap_create(NULL, NULL);

    hashmap_put(hashmap, "Alice", 15);
    hashmap_put(hashmap, "Bob", 1337);
    hashmap_put(hashmap, "Peter", 17);
    hashmap_put(hashmap, "Gwen", 999999);
    hashmap_put(hashmap, "David", 111732);

    const char *result;

    // Test getting existing value
    result = hashmap_T_get(char *, hashmap, "Bob", NULL);
    TEST_ASSERT_NOT_NULL_MESSAGE(result, "Getting existing value should not yield null");
    TEST_ASSERT_EQUAL_UINT64_MESSAGE(1337, result, "Getting existing value should match expected value");

    // Test another existing value
    result = hashmap_T_get(char *, hashmap, "Peter", NULL);
    TEST_ASSERT_NOT_NULL_MESSAGE(result, "Getting existing value should not yield null");
    TEST_ASSERT_EQUAL_UINT64_MESSAGE(17, result, "Getting existing value should match expected value");

    // Test non-existing value
    result = hashmap_T_get(char *, hashmap, "John", NULL);
    TEST_ASSERT_NULL_MESSAGE(result, "Getting non-existing value should yield null");

    // Modify values
    hashmap_put(hashmap, "Bob", 1972);
    hashmap_put(hashmap, "Peter", 1958);

    // Test getting overwritten value
    result = hashmap_T_get(char *, hashmap, "Bob", NULL);
    TEST_ASSERT_NOT_NULL_MESSAGE(result, "Getting overwritten value should not yield null");
    TEST_ASSERT_EQUAL_UINT64_MESSAGE(1972, result, "Getting overwritten value should match expected value");

    // Test getting another overwritten value
    result = hashmap_T_get(char *, hashmap, "Peter", NULL);
    TEST_ASSERT_NOT_NULL_MESSAGE(result, "Getting overwritten value should not yield null");
    TEST_ASSERT_EQUAL_UINT64_MESSAGE(1958, result, "Getting overwritten value should match expected value");

    // Test getting non-overwritten value
    result = hashmap_T_get(char *, hashmap, "David", NULL);
    TEST_ASSERT_NOT_NULL_MESSAGE(result, "Getting non-overwritten value should not yield null");
    TEST_ASSERT_EQUAL_UINT64_MESSAGE(111732, result, "Getting non-overwritten value should match expected value");

    // Test getting non-existing value
    result = hashmap_T_get(char *, hashmap, "John", NULL);
    TEST_ASSERT_NULL_MESSAGE(result, "Getting non-existing value should yield null");

    hashmap_destroy(hashmap);
}

static void test_Remove(void)
{
    struct hashmap_t *hashmap = hashmap_create(NULL, NULL);

    hashmap_put(hashmap, "Alice", 15);
    hashmap_put(hashmap, "Bob", 1972);

    const char *result;
    bool_t success;

    // Test getting existing value
    result = hashmap_T_get(char *, hashmap, "Bob", NULL);
    TEST_ASSERT_NOT_NULL_MESSAGE(result, "Getting existing value should not yield null");
    TEST_ASSERT_EQUAL_UINT64_MESSAGE(1972, result, "Getting existing value should match expected value");

    // Remove value
    success = hashmap_remove(hashmap, "Bob");

    // Assert removal
    TEST_ASSERT_TRUE_MESSAGE(success, "Removing existing value should not fail");

    // Test getting removed value
    result = hashmap_T_get(char *, hashmap, "Bob", NULL);
    TEST_ASSERT_NULL_MESSAGE(result, "Getting removed value should yield null"); 

    // Test getting not-removed value
    result = hashmap_T_get(char *, hashmap, "Alice", NULL);
    TEST_ASSERT_NOT_NULL_MESSAGE(result, "Getting not-removed value should not yield null");
    TEST_ASSERT_EQUAL_UINT64_MESSAGE(15, result, "Getting not-removed value should match expected value");

    // Re-add value
    success = hashmap_put(hashmap, "Bob", 1337);

    // Assert re-adding
    TEST_ASSERT_TRUE_MESSAGE(success, "Re-adding value should not fail");

    // Test getting re-added value
    result = hashmap_T_get(char *, hashmap, "Bob", NULL);
    TEST_ASSERT_NOT_NULL_MESSAGE(result, "Getting re-added value should not yield null");
    TEST_ASSERT_EQUAL_UINT64_MESSAGE(1337, result, "Getting re-added value should match expected value");

    // Modify re-added value
    success = hashmap_put(hashmap, "Bob", 1972);

    // Assert modification
    TEST_ASSERT_TRUE_MESSAGE(success, "Modifying re-added value should not fail");

    // Test getting modified value
    result = hashmap_T_get(char *, hashmap, "Bob", NULL);
    TEST_ASSERT_NOT_NULL_MESSAGE(result, "Getting modified re-added value should not yield null");
    TEST_ASSERT_EQUAL_UINT64_MESSAGE(1972, result, "Getting modified re-added value should match expected value");

    // Try removing non-existing value
    success = hashmap_remove(hashmap, "David");

    // Assert removing non-existing value fails
    TEST_ASSERT_FALSE_MESSAGE(success, "Removing non-existing value should fail");

    hashmap_destroy(hashmap);
}

void _test_hashmap(void)
{
    RUN_TEST(test_Create);
    RUN_TEST(test_Put);
    RUN_TEST(test_Get);
    RUN_TEST(test_Remove);
}
