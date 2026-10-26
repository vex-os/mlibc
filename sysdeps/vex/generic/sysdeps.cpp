#include <abi-bits/errno.h>
#include <bits/ensure.h>
#include <limits.h>
#include <mlibc/all-sysdeps.hpp>
#include <stdint.h>
#include <string.h>
#include <vex/fcntl.h>
#include <vex/futex.h>
#include <vex/ioctl.h>
#include <vex/mman.h>
#include <vex/syscall.h>
#include <vex/termios.h>
#include <vex/time.h>

#include "syscall.hpp" // The architecture provides vex::syscall for up to six arguments

static int vex_errno(intptr_t result)
{
    if(result < 0 && result >= -4095)
        return static_cast<int>(-result);
    return 0;
}

static int vex_syscall_errno(intptr_t syscall_number, intptr_t arg_1 = 0, intptr_t arg_2 = 0,
    intptr_t arg_3 = 0, intptr_t arg_4 = 0, intptr_t arg_5 = 0, intptr_t arg_6 = 0)
{
    intptr_t result = vex::syscall(syscall_number, arg_1, arg_2, arg_3, arg_4, arg_5, arg_6);

    return vex_errno(result);
}

static int vex_syscall_result(intptr_t* result, intptr_t syscall_number, intptr_t arg_1 = 0,
    intptr_t arg_2 = 0, intptr_t arg_3 = 0, intptr_t arg_4 = 0, intptr_t arg_5 = 0,
    intptr_t arg_6 = 0)
{
    *result = vex::syscall(syscall_number, arg_1, arg_2, arg_3, arg_4, arg_5, arg_6);

    return vex_errno(*result);
}

void mlibc::Sysdeps<LibcLog>::operator()(const char* message)
{
    vex::syscall(__k_SYS_WRITE, 2, reinterpret_cast<intptr_t>(message), strlen(message));
    vex::syscall(__k_SYS_WRITE, 2, reinterpret_cast<intptr_t>("\n"), 1);
}

[[noreturn]] void mlibc::Sysdeps<LibcPanic>::operator()()
{
    sysdep<LibcLog>("mlibc panic!");
    vex_syscall_errno(__k_SYS_EXIT, 1);
    __builtin_unreachable();
}

[[noreturn]] void mlibc::Sysdeps<Exit>::operator()(int status)
{
    vex_syscall_errno(__k_SYS_EXIT, status);
    __builtin_unreachable();
}

int mlibc::Sysdeps<TcbSet>::operator()(void* pointer)
{
    return vex_syscall_errno(__k_SYS_TCB_SET, reinterpret_cast<intptr_t>(pointer));
}

int mlibc::Sysdeps<AnonAllocate>::operator()(size_t size, void** pointer)
{
    intptr_t result;
    int error = vex_syscall_result(&result, __k_SYS_MMAP, 0, size, __k_PROT_READ | __k_PROT_WRITE,
        __k_MAP_PRIVATE | __k_MAP_ANONYMOUS, -1, 0);

    if(error) {
        return error;
    }

    *pointer = reinterpret_cast<void*>(result);
    return 0;
}

int mlibc::Sysdeps<AnonFree>::operator()(void* pointer, size_t size)
{
    return vex_syscall_errno(__k_SYS_MUNMAP, reinterpret_cast<intptr_t>(pointer), size);
}

int mlibc::Sysdeps<VmMap>::operator()(void* hint, size_t size, int prot, int flags, int fd,
    off_t offset, void** window)
{
    intptr_t result;
    int error = vex_syscall_result(&result, __k_SYS_MMAP, reinterpret_cast<intptr_t>(hint), size,
        prot, flags, fd, offset);

    if(error) {
        return error;
    }

    *window = reinterpret_cast<void*>(result);
    return 0;
}

int mlibc::Sysdeps<VmUnmap>::operator()(void* pointer, size_t size)
{
    return vex_syscall_errno(__k_SYS_MUNMAP, reinterpret_cast<intptr_t>(pointer), size);
}

int mlibc::Sysdeps<ClockGet>::operator()(int clock, time_t* secs, long* nanos)
{
    struct __k_timespec time;
    int error = vex_syscall_errno(__k_SYS_GETTIME, clock, reinterpret_cast<intptr_t>(&time));

    if(error) {
        return error;
    }

    *secs = time.tv_sec;
    *nanos = time.tv_nsec;
    return 0;
}

// The kernel takes a relative timeout, as Linux does.
// A futex word may live in shared memory, so every wait and wake asks for the shared kind
int mlibc::Sysdeps<FutexWait>::operator()(int* pointer, int expected, const struct timespec* time)
{
    return vex_syscall_errno(__k_SYS_FUTEX_WAIT, reinterpret_cast<intptr_t>(pointer), expected,
        reinterpret_cast<intptr_t>(time), __k_FUTEX_SHARED);
}

int mlibc::Sysdeps<FutexWake>::operator()(int* pointer, bool all)
{
    return vex_syscall_errno(__k_SYS_FUTEX_WAKE, reinterpret_cast<intptr_t>(pointer),
        all ? INT_MAX : 1, __k_FUTEX_SHARED);
}

int mlibc::Sysdeps<Open>::operator()(const char* pathname, int flags, mode_t mode, int* fd)
{
    intptr_t result;
    int error = vex_syscall_result(&result, __k_SYS_OPENAT, __k_AT_FDCWD,
        reinterpret_cast<intptr_t>(pathname), flags, mode);

    if(error) {
        return error;
    }

    *fd = result;
    return 0;
}

int mlibc::Sysdeps<Close>::operator()(int fd)
{
    return vex_syscall_errno(__k_SYS_CLOSE, fd);
}

int mlibc::Sysdeps<Read>::operator()(int fd, void* buf, size_t count, ssize_t* bytes_read)
{
    intptr_t result;
    int error = vex_syscall_result(&result, __k_SYS_READ, fd, reinterpret_cast<intptr_t>(buf),
        count);

    if(error) {
        return error;
    }

    *bytes_read = result;
    return 0;
}

int mlibc::Sysdeps<Write>::operator()(int fd, const void* buf, size_t count, ssize_t* bytes_written)
{
    intptr_t result;
    int error = vex_syscall_result(&result, __k_SYS_WRITE, fd, reinterpret_cast<intptr_t>(buf),
        count);

    if(error) {
        return error;
    }

    *bytes_written = result;
    return 0;
}

int mlibc::Sysdeps<Seek>::operator()(int fd, off_t offset, int whence, off_t* new_offset)
{
    intptr_t result;
    int error = vex_syscall_result(&result, __k_SYS_SEEK, fd, offset, whence);

    if(error) {
        return error;
    }

    *new_offset = result;
    return 0;
}

// Only a terminal answers to the termios request
int mlibc::Sysdeps<Isatty>::operator()(int fd)
{
    struct __k_termios termios;
    int error = vex_syscall_errno(__k_SYS_IOCTL, fd, __k_TCGETS,
        reinterpret_cast<intptr_t>(&termios));

    if(error == 0 || error == EBADF) {
        return error;
    }

    return ENOTTY;
}
