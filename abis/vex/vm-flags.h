#ifndef _ABIBITS_VM_FLAGS_H
#define _ABIBITS_VM_FLAGS_H

#include <vex/mman.h>

#define PROT_NONE   __k_PROT_NONE
#define PROT_READ   __k_PROT_READ
#define PROT_WRITE  __k_PROT_WRITE
#define PROT_EXEC   __k_PROT_EXEC

#define MAP_FAILED ((void *)(-1))
#define MAP_FILE 0x00

#define MAP_SHARED     __k_MAP_SHARED
#define MAP_PRIVATE    __k_MAP_PRIVATE
#define MAP_FIXED      __k_MAP_FIXED
#define MAP_ANONYMOUS  __k_MAP_ANONYMOUS
#define MAP_ANON MAP_ANONYMOUS

#define MS_ASYNC       __k_MS_ASYNC
#define MS_INVALIDATE  __k_MS_INVALIDATE
#define MS_SYNC        __k_MS_SYNC

#define MCL_CURRENT 0x01
#define MCL_FUTURE 0x02

#define POSIX_MADV_NORMAL 0
#define POSIX_MADV_RANDOM 1
#define POSIX_MADV_SEQUENTIAL 2
#define POSIX_MADV_WILLNEED 3
#define POSIX_MADV_DONTNEED 4

#endif /* _ABIBITS_VM_FLAGS_H */
