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

void test_CheckInnerOverflow(void)
{ 
    void * firstBlock = NULL;
    void * secondBlock = NULL;
    ba_err_t baErr = BA_ERR_OK;

    baErr = BA_Init();
    TEST_ASSERT_EQUAL(BA_ERR_OK, baErr);
    
    baErr = BA_Alloc(&firstBlock);
    TEST_ASSERT_EQUAL(BA_ERR_OK, baErr);
    TEST_ASSERT_NOT_EQUAL(NULL, firstBlock);

    if (firstBlock != NULL)
    {
        memset(firstBlock, 0xAA, BA_BLOCK_SIZE + 24);
    }

    baErr = BA_Alloc(&secondBlock);
    TEST_ASSERT_EQUAL(BA_ERR_CORRUPTED, baErr);
}

