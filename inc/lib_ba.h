#ifndef __LIB_BA_H
#define __LIB_BA_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#ifdef BA_TEST
#include "lib_ba_opts.h"
#endif

#ifndef BA_BLOCK_SIZE
#error No BA_BLOCK_SIZE defined. Please define in your code the BA_BLOCK_SIZE from 1 to UINT16_MAX max
#endif

#if BA_BLOCK_SIZE < 1
#error BA_BLOCK_SIZE > UINT16_MAX. BA_BLOCK_SIZE must have size 1 <= BA_BLOCK_SIZE <= UINT16_MAX
#endif

#if BA_BLOCK_SIZE > UINT16_MAX
#error BA_BLOCK_SIZE > UINT16_MAX. BA_BLOCK_SIZE must have size 1 <= BA_BLOCK_SIZE <= UINT16_MAX
#endif

#ifndef BA_POOL_SIZE
#error NO BA_POOL_SIZE defined. Please define in your code the BA_POOL_SIZE from 1 to UINT16_MAX max
#endif

#if BA_POOL_SIZE < 1
#error BA_POOL_SIZE less than 1. Please redefine it from 1 to uin32_t max
#endif

#if BA_POOL_SIZE > UINT16_MAX
#error BA_POOL_SIZE > UINT16_MAX. BA_POOL_SIZE must have size 1 <= BA_POOL_SIZE <= UINT16_MAX
#endif

#ifndef BA_CHECK_OVERFLOW
#error No BA_CHECK_OVERFLOW defined. Please define in your code the BA_CHECK_OVERFLOW to 0 (no overflow check) or to 1 (overflow check exist)
#endif

#if ((BA_CHECK_OVERFLOW != 0) && (BA_CHECK_OVERFLOW != 1))
#error Wrong BA_CHECK_OVERFLOW value. Please define in your code the BA_CHECK_OVERFLOW to 0 (no overflow check) or to 1 (overflow check exist)
#endif

#ifndef BA_CRITICAL_ENTER
#error No BA_CRITICAL_ENTER() defined. Please define it in your code. For non-multitreading evironment you may define it as <#define BA_CRITICAL_ENTER() ;>
#endif // !BA_CRITICAL_ENTER

#ifndef BA_CRITICAL_EXIT
#error No BA_CRITICAL_EXIT() defined. Please define it in your code. For non-multitreading evironment you may define it as <#define BA_CRITICAL_EXIT() ;>
#endif // !BA_CRITICAL_EXIT

#ifndef NULL
#define NULL                ((void *) 0)
#endif

#define BA_DEFAULT_BYTE     ((uint8_t) 0xBD)

typedef enum
{
    BA_ERR_OK = 0x00,
    BA_ERR_PARAM,
    BA_ERR_NO_FREE_BLOCKS,
    BA_ERR_NOT_FOUND,
#if BA_CHECK_OVERFLOW == 1
    BA_ERR_CORRUPTED
#endif
} ba_err_t;

typedef struct __ba_block
{
    uint8_t data[BA_BLOCK_SIZE];
    void * usedPtr;
} ba_block_t;

ba_err_t BA_Init(void);
ba_err_t BA_Alloc(void ** destPtr);
ba_err_t BA_Free(void ** destPtr);
uint16_t BA_GetFreeBlocksCount(void);
ba_err_t BA_GetBlockMaxUsage(void ** destPtr, uint16_t * maxUsage);

#ifdef __cplusplus
}
#endif

#endif //__LIB_BA_H