#ifndef _SYS_MOUNT_H
#define _SYS_MOUNT_H

/* POSIX has no mount interface, so these functions are an extension of Vex.
 * Their prototypes are the ones of Linux and BSD that programs expect. */

#include <vex/mount.h>

#ifdef __cplusplus
extern "C" {
#endif

/* The flags are for what every filesystem has in common */
#define MS_RDONLY  __k_MNT_RDONLY
#define MNT_RDONLY __k_MNT_RDONLY

/* The source is a block device, or null for a filesystem that has none.
 * The type may be null if there is a device, as the kernel finds the type then.
 * The data is a string with the options of the filesystem, or null. */
int mount(const char* __source, const char* __target, const char* __fstype, unsigned long __flags,
    const void* __data);
int umount(const char* __target);

#ifdef __cplusplus
}
#endif

#endif /* _SYS_MOUNT_H */
