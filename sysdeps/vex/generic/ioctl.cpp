#include <errno.h>
#include <stdarg.h>
#include <stdint.h>
#include <sys/ioctl.h>
#include <vex/syscall.h>

#include "syscall.hpp" // The architecture provides vex::syscall for up to six arguments

// The argument is a pointer or an integer
// and the kernel takes either as a word
int ioctl(int fd, unsigned long request, ...)
{
    va_list args;
    va_start(args, request);
    void* argument = va_arg(args, void*);
    va_end(args);

    intptr_t result = vex::syscall(__k_SYS_IOCTL, fd, request,
        reinterpret_cast<intptr_t>(argument));

    if(result < 0 && result >= -4095) {
        errno = static_cast<int>(-result);
        return -1;
    }

    return static_cast<int>(result);
}
