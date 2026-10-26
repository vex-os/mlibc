#ifndef _ABIBITS_FCNTL_H
#define _ABIBITS_FCNTL_H

#include <abi-bits/pid_t.h>
#include <bits/off_t.h>
#include <vex/fcntl.h>

#define O_RDONLY     __k_O_RDONLY
#define O_WRONLY     __k_O_WRONLY
#define O_RDWR       __k_O_RDWR
#define O_CREAT      __k_O_CREAT
#define O_EXCL       __k_O_EXCL
#define O_TRUNC      __k_O_TRUNC
#define O_APPEND     __k_O_APPEND
#define O_NONBLOCK   __k_O_NONBLOCK
#define O_DIRECTORY  __k_O_DIRECTORY
#define O_NOFOLLOW   __k_O_NOFOLLOW
#define O_CLOEXEC    __k_O_CLOEXEC

/* The kernel ignores these flags */
#define O_NOCTTY     0x0400
#define O_DSYNC      0x0800
#define O_ASYNC      0x1000
#define O_DIRECT     0x2000
#define O_LARGEFILE  0x4000
#define O_NOATIME    0x8000
#define O_PATH       0x10000
#define O_TMPFILE    0x20000
#define __O_SYNC     0x40000
#define O_SYNC       (__O_SYNC | O_DSYNC)
#define O_RSYNC      O_SYNC
#define O_EXEC       O_PATH
#define O_SEARCH     O_PATH
#define O_ACCMODE    O_RDWR

#define F_DUPFD          __k_F_DUPFD
#define F_GETFD          __k_F_GETFD
#define F_SETFD          __k_F_SETFD
#define F_GETFL          __k_F_GETFL
#define F_SETFL          __k_F_SETFL
#define F_DUPFD_CLOEXEC  __k_F_DUPFD_CLOEXEC

/* TODO: the kernel has no record locks or file owners yet, so these numbers are the C library's own */
#define F_GETLK       1001
#define F_SETLK       1002
#define F_SETLKW      1003
#define F_SETLK64     F_SETLK
#define F_SETLKW64    F_SETLKW
#define F_SETOWN      1004
#define F_GETOWN      1005
#define F_OFD_GETLK   1006
#define F_OFD_SETLK   1007
#define F_OFD_SETLKW  1008

#define F_RDLCK 0
#define F_WRLCK 1
#define F_UNLCK 2

#define FD_CLOEXEC  __k_FD_CLOEXEC

#define AT_FDCWD             __k_AT_FDCWD
#define AT_SYMLINK_NOFOLLOW  __k_AT_SYMLINK_NOFOLLOW
#define AT_SYMLINK_FOLLOW    __k_AT_SYMLINK_FOLLOW
#define AT_REMOVEDIR         __k_AT_REMOVEDIR
#define AT_EACCESS           __k_AT_EACCESS

#if defined(_GNU_SOURCE)
#define AT_EMPTY_PATH __k_AT_EMPTY_PATH
#endif /* defined(_GNU_SOURCE) */

#define POSIX_FADV_NORMAL 0
#define POSIX_FADV_RANDOM 1
#define POSIX_FADV_SEQUENTIAL 2
#define POSIX_FADV_WILLNEED 3
#define POSIX_FADV_DONTNEED 4
#define POSIX_FADV_NOREUSE 5

struct flock {
	short l_type;
	short l_whence;
	off_t l_start;
	off_t l_len;
	pid_t l_pid;
};

#endif /* _ABIBITS_FCNTL_H */
