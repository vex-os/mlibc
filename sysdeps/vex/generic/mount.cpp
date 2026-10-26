#include <errno.h>
#include <stdint.h>
#include <sys/mount.h>
#include <vex/syscall.h>

#include "syscall.hpp" // The architecture provides vex::syscall for up to six arguments

static int vex_mount_result(intptr_t result)
{
    if(result < 0 && result >= -4095) {
        errno = static_cast<int>(-result);
        return -1;
    }

    return 0;
}

int mount(const char* source, const char* target, const char* fstype, unsigned long flags,
    const void* data)
{
    return vex_mount_result(vex::syscall(__k_SYS_MOUNT, reinterpret_cast<intptr_t>(source),
        reinterpret_cast<intptr_t>(target), reinterpret_cast<intptr_t>(fstype), flags,
        reinterpret_cast<intptr_t>(data)));
}

int umount(const char* target)
{
    return vex_mount_result(vex::syscall(__k_SYS_UMOUNT, reinterpret_cast<intptr_t>(target)));
}
