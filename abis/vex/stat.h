#ifndef _ABIBITS_STAT_H
#define _ABIBITS_STAT_H

#include <abi-bits/blkcnt_t.h>
#include <abi-bits/blksize_t.h>
#include <abi-bits/dev_t.h>
#include <abi-bits/gid_t.h>
#include <abi-bits/ino_t.h>
#include <abi-bits/mode_t.h>
#include <abi-bits/nlink_t.h>
#include <abi-bits/uid_t.h>
#include <bits/ansi/time_t.h>
#include <bits/ansi/timespec.h>
#include <bits/off_t.h>
#include <vex/stat.h>

#define S_IFMT    __k_S_IFMT
#define S_IFBLK   __k_S_IFBLK
#define S_IFCHR   __k_S_IFCHR
#define S_IFIFO   __k_S_IFIFO
#define S_IFREG   __k_S_IFREG
#define S_IFDIR   __k_S_IFDIR
#define S_IFLNK   __k_S_IFLNK
#define S_IFSOCK  __k_S_IFSOCK
#define S_IRWXU   __k_S_IRWXU
#define S_IRUSR   __k_S_IRUSR
#define S_IWUSR   __k_S_IWUSR
#define S_IXUSR   __k_S_IXUSR
#define S_IRWXG   __k_S_IRWXG
#define S_IRGRP   __k_S_IRGRP
#define S_IWGRP   __k_S_IWGRP
#define S_IXGRP   __k_S_IXGRP
#define S_IRWXO   __k_S_IRWXO
#define S_IROTH   __k_S_IROTH
#define S_IWOTH   __k_S_IWOTH
#define S_IXOTH   __k_S_IXOTH
#define S_ISUID   __k_S_ISUID
#define S_ISGID   __k_S_ISGID
#define S_ISVTX   __k_S_ISVTX

#define S_IREAD S_IRUSR
#define S_IWRITE S_IWUSR
#define S_IEXEC S_IXUSR

#ifdef __cplusplus
extern "C" {
#endif

struct stat {
	dev_t st_dev;
	ino_t st_ino;
	mode_t st_mode;
	nlink_t st_nlink;
	uid_t st_uid;
	gid_t st_gid;
	dev_t st_rdev;
	off_t st_size;
	struct timespec st_atim;
	struct timespec st_mtim;
	struct timespec st_ctim;
	blksize_t st_blksize;
	blkcnt_t st_blocks;
};

#define stat64 stat

#ifdef __cplusplus
}
#endif

#endif /* _ABIBITS_STAT_H */
