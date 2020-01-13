#include "lib_ba.h"

// Static varivables

volatile static ba_block_t __baPool[BA_POOL_SIZE];

// Private functions declaration

static ba_err_t __BA_GetFirstFreeBlockIdx(uint16_t * idx);
static ba_err_t __BA_FindBlockIdxByUsedPtr(uint16_t * idx, void ** usedPtr);
static ba_err_t __BA_CheckBlockOverflow(void * data, uint16_t blockSize);

// Private functions definition

/**
 * @brief Find and return index of a free block inside static pool
 * 
 * @param idx [out] index inside static pool of free block
 * @return ba_err_t Return BA_ERR_PARAM in case of NULL pointer. Return BA_ERR_OK in case when 
 * was found a free block. Return BA_ERR_NO_FREE_BLOCKS if no free block available
 */
static ba_err_t __BA_GetFirstFreeBlockIdx(uint16_t * idx)
{
    if (idx == NULL) return BA_ERR_PARAM;

    for (uint16_t i = 0; i < BA_POOL_SIZE; i++)
    {
        if (__baPool[i].usedPtr == NULL)
        {
            *idx = i;
            return BA_ERR_OK;
        }
    }

    return BA_ERR_NO_FREE_BLOCKS;
}

/**
 * @brief Find and return block's index inside static pool based on a user's pointer
 * 
 * @param idx [out] index inside static pool of block
 * @param usedPtr [in] address of the user's pointer
 * @return ba_err_t 
 * - BA_ERR_OK in case when the block was found. 
 * - BA_ERR_PARAM in case of NULL pointer in paramenters. 
 * - BA_ERR_NOT_FOUND if block was not found.
 */
static ba_err_t __BA_FindBlockIdxByUsedPtr(uint16_t * idx, void ** usedPtr)
{
    if (usedPtr == NULL) return BA_ERR_PARAM;

    for (uint16_t i = 0; i < BA_POOL_SIZE; i++)
    {
        if (__baPool[i].usedPtr == usedPtr)
        {
            *idx = i;
            return BA_ERR_OK;
        }
    }

    return BA_ERR_NOT_FOUND;
}

/**
 * @brief Check block corruption in case overflow by usage of allocated blocks
 * 
 * @param data [in] pointer to a block's data
 * @param blockSize [in] size of block
 * @return ba_err_t 
 * - BA_ERR_OK in case if all right
 * - BA_ERR_CORRUPTED in case when there is overflow situation inside static pool
 */
static ba_err_t __BA_CheckBlockOverflow(void * data, uint16_t blockSize)
{
    uint8_t * dataPtr = (uint8_t *) data;

    for (uint16_t i = 0; i < blockSize; i++)
    {
        if (*(dataPtr + i) != BA_DEFAULT_BYTE)
        {
            return BA_ERR_CORRUPTED;
        }
    }

    return BA_ERR_OK;
}

// Public functions definition

/**
 * @brief Init block allocator
 * 
 * @return ba_err_t 
 * - BA_ERR_OK in case if all right
 */
ba_err_t BA_Init(void)
{
    BA_CRITICAL_ENTER();

    for (uint16_t i = 0; i < BA_POOL_SIZE; i++)
    {
        memset(__baPool[i].data, BA_DEFAULT_BYTE, BA_BLOCK_SIZE);
        __baPool[i].usedPtr = NULL;
    } 

    BA_CRITICAL_EXIT();

    return BA_ERR_OK;
}

/**
 * @brief Allocate block
 * 
 * @param destPtr address of pointer for allocated block. 
 * Warning! Pointer will be have NULL value in case of error!
 * @return ba_err_t 
 * - BA_ERR_OK in case of successful allocations. 
 * - BA_ERR_PARAM in case of null reference in parameters.
 * - BA_ERR_NO_FREE_BLOCKS in case when no free blocks found.
 * - BA_ERR_CORRUPTED in case when there is overflow situation inside static pool
 */
ba_err_t BA_Alloc(void ** destPtr)
{
    if (destPtr == NULL) return BA_ERR_PARAM;
    
    ba_err_t err = BA_ERR_OK;
    uint16_t idx = 0;

    BA_CRITICAL_ENTER();
    
    *destPtr = NULL;
    err = __BA_GetFirstFreeBlockIdx(&idx);

    if (err == BA_ERR_OK)
    {
        *destPtr = &__baPool[idx].data;
        __baPool[idx].usedPtr = destPtr;
#if BA_CHECK_OVERFLOW == 1
        err = __BA_CheckBlockOverflow(&__baPool[idx].data, BA_BLOCK_SIZE);
#endif
    }

    BA_CRITICAL_EXIT();
    
    return err;
}

/**
 * @brief Free allocated block
 * 
 * @param destPtr address of pointer for allocated block. 
 * @return ba_err_t 
 * - BA_ERR_OK in case of successful allocations. 
 * - BA_ERR_PARAM in case of NULL value in parameters.
 * - BA_ERR_NOT_FOUND if block was not found.
 */
ba_err_t BA_Free(void ** destPtr)
{
    if (destPtr == NULL) return BA_ERR_PARAM;

    ba_err_t err = BA_ERR_OK;
    uint16_t idx = 0;

    BA_CRITICAL_ENTER();

    err = __BA_FindBlockIdxByUsedPtr(&idx, destPtr);

    if (err == BA_ERR_OK)
    {
        memset(__baPool[idx].data, BA_DEFAULT_BYTE, BA_BLOCK_SIZE);
        __baPool[idx].usedPtr = NULL;
        *destPtr = NULL;
    }

    BA_CRITICAL_EXIT();

    return err;
}

/**
 * @brief Calculate free blocks count and return it
 * 
 * @return uint16_t count of free blocks
 */
uint16_t BA_GetFreeBlocksCount(void)
{
    uint16_t ret = 0;

    BA_CRITICAL_ENTER();

    for (uint16_t i = 0; i < BA_POOL_SIZE; i++)
    {
        if (__baPool[i].usedPtr == NULL)
        {
            ret++;
        }
    }

    BA_CRITICAL_EXIT();

    return ret;
}

/**
 * @brief Calc max usage of a block in bytes
 * @warning This functions can be calc wrong usage according to it's nature.
 * Value of maxUsage may be used for mainly debug and tracing purposes 
 * @param destPtr [in] address of pointer for allocated block.
 * @param maxUsage [out] max usage of the block in bytes
 * @return ba_err_t 
 * - BA_ERR_OK if all ok. 
 * - BA_ERR_PARAM in case of null reference in parameters.
 */
ba_err_t BA_GetBlockMaxUsage(void ** destPtr, uint16_t * maxUsage)
{
    if (destPtr == NULL) return BA_ERR_PARAM;
    if (maxUsage == NULL) return BA_ERR_PARAM;

    ba_err_t err = BA_ERR_OK;
    uint16_t idx = 0;

    BA_CRITICAL_ENTER();

    err = __BA_FindBlockIdxByUsedPtr(&idx, destPtr);

    if (err == BA_ERR_OK)
    {
        *maxUsage = BA_BLOCK_SIZE;

        // TODO - improving. Use binary search
        for (uint16_t i = BA_BLOCK_SIZE; i > 0; i--)
        {
            if (__baPool[idx].data[i - 1] == BA_DEFAULT_BYTE)
            {
                (*maxUsage)--;
            }
        } 
    }

    BA_CRITICAL_EXIT();

    return err;
}
