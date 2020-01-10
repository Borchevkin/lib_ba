#include "unity.h"

#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "lib_ba.h"

void setUp(void)
{
    //TEST_IGNORE_MESSAGE("Start Test");
}

void tearDown(void)
{
    //TEST_IGNORE_MESSAGE("End Test");
}

void test_BlockUsage(void)
{ 
    void * allocBlock = NULL;
    ba_err_t baErr = BA_ERR_OK;
    uint16_t maxUsage = 0;

    baErr = BA_Init();
    TEST_ASSERT_EQUAL(BA_ERR_OK, baErr);
    
    baErr = BA_Alloc(&allocBlock);
    TEST_ASSERT_EQUAL(BA_ERR_OK, baErr);

    baErr = BA_GetBlockMaxUsage(NULL, &maxUsage);
    TEST_ASSERT_EQUAL(BA_ERR_PARAM, baErr);

    baErr = BA_GetBlockMaxUsage(&allocBlock + 2, &maxUsage);
    TEST_ASSERT_EQUAL(BA_ERR_NOT_FOUND, baErr);

    baErr = BA_GetBlockMaxUsage(&allocBlock, NULL);
    TEST_ASSERT_EQUAL(BA_ERR_PARAM, baErr);

    baErr = BA_GetBlockMaxUsage(&allocBlock, &maxUsage);
    TEST_ASSERT_EQUAL(BA_ERR_OK, baErr);
    TEST_ASSERT_EQUAL(0, maxUsage);

    memset(allocBlock, 0x13, BA_BLOCK_SIZE / 4);
    baErr = BA_GetBlockMaxUsage(&allocBlock, &maxUsage);
    TEST_ASSERT_EQUAL(BA_ERR_OK, baErr);
    TEST_ASSERT_EQUAL(BA_BLOCK_SIZE / 4, maxUsage);

    memset(allocBlock, 0x98, BA_BLOCK_SIZE / 2);
    baErr = BA_GetBlockMaxUsage(&allocBlock, &maxUsage);
    TEST_ASSERT_EQUAL(BA_ERR_OK, baErr);
    TEST_ASSERT_EQUAL(BA_BLOCK_SIZE / 2, maxUsage);

    memset(allocBlock, 0xFF, BA_BLOCK_SIZE);
    baErr = BA_GetBlockMaxUsage(&allocBlock, &maxUsage);
    TEST_ASSERT_EQUAL(BA_ERR_OK, baErr);
    TEST_ASSERT_EQUAL(BA_BLOCK_SIZE, maxUsage);

    baErr = BA_Free(&allocBlock);
    TEST_ASSERT_EQUAL(BA_ERR_OK, baErr);
}