#ifndef __LIB_BA_OPTS_H
#define __LIB_BA_OPTS_H

#ifdef __cplusplus
extern "C" {
#endif

#define BA_BLOCK_SIZE           256         // size of each block in bytes
#define BA_POOL_SIZE            8           // size of pool of blocks

#define BA_CHECK_OVERFLOW       1           // check overflow option. 0 - no check, 1 - check enabled  

#ifdef __cplusplus
}
#endif

#endif //__LIB_BA_OPTS_H