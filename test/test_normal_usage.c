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

void test_NormalUsage(void)
{ 
    void * allocBlock = NULL;
    ba_err_t baErr = BA_ERR_OK;

    baErr = BA_Init();
    TEST_ASSERT_EQUAL(BA_ERR_OK, baErr);
    TEST_ASSERT_EQUAL(BA_POOL_SIZE, BA_GetFreeBlocksCount());
    
    baErr = BA_Alloc(&allocBlock);
    TEST_ASSERT_EQUAL(BA_ERR_OK, baErr);
    TEST_ASSERT_NOT_EQUAL(NULL, allocBlock);
    TEST_ASSERT_EQUAL(BA_POOL_SIZE - 1, BA_GetFreeBlocksCount());

    if (allocBlock != NULL)
    {
        memset(allocBlock, 0xAA, BA_BLOCK_SIZE);
    }

    baErr = BA_Free(&allocBlock);
    TEST_ASSERT_EQUAL(BA_ERR_OK, baErr);
    TEST_ASSERT_EQUAL(NULL, allocBlock);
    TEST_ASSERT_EQUAL(BA_POOL_SIZE, BA_GetFreeBlocksCount());
}

void test_MaxBlocksAlloc(void)
{
    void * blocks[BA_POOL_SIZE * 2];
    ba_err_t baErr = BA_ERR_OK;
    uint16_t counter = 0;

    baErr = BA_Init();
    TEST_ASSERT_EQUAL(BA_ERR_OK, baErr);
    TEST_ASSERT_EQUAL(BA_POOL_SIZE, BA_GetFreeBlocksCount());

    while ((baErr == BA_ERR_OK) && (counter < (BA_POOL_SIZE *2)))
    {
        baErr = BA_Alloc(&blocks[counter]);
        
        if ((baErr == BA_ERR_OK) && (blocks[counter] != NULL))
        {
            memset(blocks[counter], 0xAA, BA_BLOCK_SIZE);
            counter++;
            TEST_ASSERT_EQUAL(BA_POOL_SIZE - counter, BA_GetFreeBlocksCount());
        }
    }

    TEST_ASSERT_EQUAL(BA_POOL_SIZE, counter);
}

void test_AllocMaxBlocksFreeAndReuseOne(void)
{
    void * blocks[BA_POOL_SIZE];
    void * oneBlock = NULL;
    void * blockToSanityCheck = NULL;
    ba_err_t baErr = BA_ERR_OK;

    baErr = BA_Init();
    TEST_ASSERT_EQUAL(BA_ERR_OK, baErr);
    TEST_ASSERT_EQUAL(BA_POOL_SIZE, BA_GetFreeBlocksCount());

    for (uint16_t i = 0; i < BA_POOL_SIZE; i++)
    {
        TEST_ASSERT_EQUAL(BA_POOL_SIZE - i, BA_GetFreeBlocksCount());
        baErr = BA_Alloc(&blocks[i]);
        TEST_ASSERT_EQUAL(BA_ERR_OK, baErr);
    }

    blockToSanityCheck = blocks[3];

    // At this pount we have no free blocks
    oneBlock = blockToSanityCheck;
    baErr = BA_Alloc(&oneBlock);
    TEST_ASSERT_EQUAL(BA_ERR_NO_FREE_BLOCKS, baErr);
    TEST_ASSERT_EQUAL(NULL, oneBlock);
    TEST_ASSERT_EQUAL(0, BA_GetFreeBlocksCount());

    // Free block[3] and get it to one block
    baErr = BA_Free(&blocks[3]);
    TEST_ASSERT_EQUAL(BA_ERR_OK, baErr);
    TEST_ASSERT_EQUAL(NULL, blocks[3]);
    TEST_ASSERT_EQUAL(1, BA_GetFreeBlocksCount());

    // Alloc oneBlock and check it address
    baErr = BA_Alloc(&oneBlock);
    TEST_ASSERT_EQUAL(BA_ERR_OK, baErr);
    TEST_ASSERT_NOT_EQUAL(NULL, oneBlock);
    TEST_ASSERT_EQUAL(blockToSanityCheck, oneBlock);
    TEST_ASSERT_EQUAL(0, BA_GetFreeBlocksCount());
}
