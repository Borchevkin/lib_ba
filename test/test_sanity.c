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

void test_SanityCheck(void)
{ 
    void * allocBlock = NULL;
    ba_err_t baErr = BA_ERR_OK;

    baErr = BA_Init();
    TEST_ASSERT_EQUAL(BA_ERR_OK, baErr);
    
    baErr = BA_Alloc(NULL);
    TEST_ASSERT_EQUAL(BA_ERR_PARAM, baErr);

    baErr = BA_Alloc(&allocBlock);
    TEST_ASSERT_EQUAL(BA_ERR_OK, baErr);

    baErr = BA_Free(NULL);
    TEST_ASSERT_EQUAL(BA_ERR_PARAM, baErr);

    baErr = BA_Free(&allocBlock + 1);
    TEST_ASSERT_EQUAL(BA_ERR_NOT_FOUND, baErr);
}