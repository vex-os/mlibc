#ifndef _ABIBITS_STATVFS_H
#define _ABIBITS_STATVFS_H

#include <abi-bits/fsblkcnt_t.h>
#include <abi-bits/fsfilcnt_t.h>
#include <vex/statvfs.h>

#define ST_RDONLY __k_ST_RDONLY
#define ST_NOSUID __k_ST_NOSUID

/* The kernel does not have this flag */
#define ST_MANDLOCK 64

/* The kernel field widths differ, so the statvfs sysdep copies the fields */
struct statvfs {
	unsigned long f_bsize;
	unsigned long f_frsize;
	fsblkcnt_t f_blocks;
	fsblkcnt_t f_bfree;
	fsblkcnt_t f_bavail;
	fsfilcnt_t f_files;
	fsfilcnt_t f_ffree;
	fsfilcnt_t f_favail;
	unsigned long f_fsid;
	unsigned long f_flag;
	unsigned long f_namemax;
	char f_basetype[80];
};

#endif /* _ABIBITS_STATVFS_H */
