# lib_ba - Static memory block allocator

## License 

BSD 2-Clause. Please check the LICENSE file.

## Testing

Based on [Unity](http://www.throwtheswitch.org/unity) and [Ceedling](http://www.throwtheswitch.org/ceedling)

Install [Ceedling](http://www.throwtheswitch.org/ceedling) and run ```ceedling test:all```

For test used the BA_TEST macro (check out ***project.yml***)

## How to used it

```C
// You should define in your code the following macroses
// Or include lib_ba_opts.h from this repo
#define BA_BLOCK_SIZE           256         // size of each block in bytes
#define BA_POOL_SIZE            8           // size of pool of blocks

#define BA_CHECK_OVERFLOW       1           // check overflow option. 0 - no check, 1 - check enabled  

// For non-multithreading environments you can set theese macroses as ;
// For RTOSes - it's you own charge how to set theese
#define BA_CRITICAL_ENTER()     ;
#define BA_CRITICAL_EXIT()      ;

...

void * allocBlock = NULL;
ba_err_t baErr = BA_ERR_OK;

// You should one time invoke BA_Init() for right work of the allocator
baErr = BA_Init();

// Allocate block
baErr = BA_Alloc(&allocBlock);

...

// Use block
if (allocBlock != NULL)
{
    memset(allocBlock, 0xAA, BA_BLOCK_SIZE);
}

...

// Get free blocks count
uint16_t freeBlocks = BA_GetFreeBlocksCount()

...

// Check block usage
uint16_t maxUsage = 0;
baErr = BA_GetBlockMaxUsage(&allocBlock, &maxUsage);

...

// Free block
baErr = BA_Free(&allocBlock);
```

For additional info please check tests and ***lib_ba.c***

## Multi-thread environment notes

For proper working with this block allocator you should set the following macroses to you own critical section routines. It may be a mutex , a semaphore, IRQ disable/enable, context switching disable/enable, etc.

```C
// For RTOSes - it's you own charge how to set theese
#define BA_CRITICAL_ENTER()     ;
#define BA_CRITICAL_EXIT()      ;
```

## Feedback

Danil Borchevkin - [danilborchevkin](https://github.com/DanilBorchevkin/)