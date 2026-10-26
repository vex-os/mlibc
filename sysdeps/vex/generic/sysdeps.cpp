#include <abi-bits/errno.h>
#include <bits/ensure.h>
#include <dirent.h>
#include <limits.h>
#include <mlibc/all-sysdeps.hpp>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/statvfs.h>
#include <sys/utsname.h>
#include <termios.h>
#include <time.h>
#include <vex/capability.h>
#include <vex/dirent.h>
#include <vex/fcntl.h>
#include <vex/futex.h>
#include <vex/ioctl.h>
#include <vex/mman.h>
#include <vex/stat.h>
#include <vex/statvfs.h>
#include <vex/syscall.h>
#include <vex/termios.h>
#include <vex/time.h>
#include <vex/utsname.h>

#include "syscall.hpp" // The architecture provides vex::syscall for up to six arguments

// The sysdeps pass these structures between the kernel and the user program without a copy
static_assert(sizeof(struct stat) == sizeof(struct __k_stat));
static_assert(offsetof(struct stat, st_dev) == offsetof(struct __k_stat, st_dev));
static_assert(offsetof(struct stat, st_ino) == offsetof(struct __k_stat, st_ino));
static_assert(offsetof(struct stat, st_mode) == offsetof(struct __k_stat, st_mode));
static_assert(offsetof(struct stat, st_nlink) == offsetof(struct __k_stat, st_nlink));
static_assert(offsetof(struct stat, st_uid) == offsetof(struct __k_stat, st_uid));
static_assert(offsetof(struct stat, st_gid) == offsetof(struct __k_stat, st_gid));
static_assert(offsetof(struct stat, st_rdev) == offsetof(struct __k_stat, st_rdev));
static_assert(offsetof(struct stat, st_size) == offsetof(struct __k_stat, st_size));
static_assert(offsetof(struct stat, st_atim) == offsetof(struct __k_stat, st_atim));
static_assert(offsetof(struct stat, st_mtim) == offsetof(struct __k_stat, st_mtim));
static_assert(offsetof(struct stat, st_ctim) == offsetof(struct __k_stat, st_ctim));
static_assert(offsetof(struct stat, st_blksize) == offsetof(struct __k_stat, st_blksize));
static_assert(offsetof(struct stat, st_blocks) == offsetof(struct __k_stat, st_blocks));

static_assert(sizeof(struct timespec) == sizeof(struct __k_timespec));
static_assert(offsetof(struct timespec, tv_sec) == offsetof(struct __k_timespec, tv_sec));
static_assert(offsetof(struct timespec, tv_nsec) == offsetof(struct __k_timespec, tv_nsec));
static_assert(UTIME_NOW == __k_UTIME_NOW);
static_assert(UTIME_OMIT == __k_UTIME_OMIT);

static_assert(sizeof(struct termios) == sizeof(struct __k_termios));
static_assert(offsetof(struct termios, c_iflag) == offsetof(struct __k_termios, c_iflag));
static_assert(offsetof(struct termios, c_oflag) == offsetof(struct __k_termios, c_oflag));
static_assert(offsetof(struct termios, c_cflag) == offsetof(struct __k_termios, c_cflag));
static_assert(offsetof(struct termios, c_lflag) == offsetof(struct __k_termios, c_lflag));
static_assert(offsetof(struct termios, c_cc) == offsetof(struct __k_termios, c_cc));
static_assert(offsetof(struct termios, c_ispeed) == offsetof(struct __k_termios, c_ispeed));
static_assert(offsetof(struct termios, c_ospeed) == offsetof(struct __k_termios, c_ospeed));
static_assert(NCCS == __k_NCCS);

static_assert(sizeof(struct winsize) == sizeof(struct __k_winsize));
static_assert(offsetof(struct winsize, ws_row) == offsetof(struct __k_winsize, ws_row));
static_assert(offsetof(struct winsize, ws_col) == offsetof(struct __k_winsize, ws_col));
static_assert(offsetof(struct winsize, ws_xpixel) == offsetof(struct __k_winsize, ws_xpixel));
static_assert(offsetof(struct winsize, ws_ypixel) == offsetof(struct __k_winsize, ws_ypixel));

static_assert(sizeof(struct dirent) == sizeof(struct __k_dirent));
static_assert(offsetof(struct dirent, d_ino) == offsetof(struct __k_dirent, d_ino));
static_assert(offsetof(struct dirent, d_off) == offsetof(struct __k_dirent, d_off));
static_assert(offsetof(struct dirent, d_reclen) == offsetof(struct __k_dirent, d_reclen));
static_assert(offsetof(struct dirent, d_type) == offsetof(struct __k_dirent, d_type));
static_assert(offsetof(struct dirent, d_name) == offsetof(struct __k_dirent, d_name));

static_assert(DT_UNKNOWN == __k_DT_UNKNOWN);
static_assert(DT_FIFO == __k_DT_FIFO);
static_assert(DT_CHR == __k_DT_CHR);
static_assert(DT_DIR == __k_DT_DIR);
static_assert(DT_BLK == __k_DT_BLK);
static_assert(DT_REG == __k_DT_REG);
static_assert(DT_LNK == __k_DT_LNK);
static_assert(DT_SOCK == __k_DT_SOCK);

// The user structure has a domain name field after these
static_assert(offsetof(struct utsname, sysname) == offsetof(struct __k_utsname, sysname));
static_assert(offsetof(struct utsname, nodename) == offsetof(struct __k_utsname, nodename));
static_assert(offsetof(struct utsname, release) == offsetof(struct __k_utsname, release));
static_assert(offsetof(struct utsname, version) == offsetof(struct __k_utsname, version));
static_assert(offsetof(struct utsname, machine) == offsetof(struct __k_utsname, machine));

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

int mlibc::Sysdeps<Umask>::operator()(mode_t mode, mode_t* old)
{
    intptr_t result;
    int error = vex_syscall_result(&result, __k_SYS_SETUMASK, mode);

    if(error) {
        return error;
    }

    *old = result;
    return 0;
}

// The calls below cannot fail for the calling process
pid_t mlibc::Sysdeps<GetPid>::operator()()
{
    return vex::syscall(__k_SYS_GETPID);
}

pid_t mlibc::Sysdeps<GetPpid>::operator()()
{
    return vex::syscall(__k_SYS_GETPPID);
}

pid_t mlibc::Sysdeps<GetTid>::operator()()
{
    return vex::syscall(__k_SYS_GETTID);
}

// The kernel has no call for the real and
// effective ids alone, so these read the whole triple
uid_t mlibc::Sysdeps<GetUid>::operator()()
{
    uid_t ruid = 0, euid = 0, suid = 0;
    sysdep<GetResuid>(&ruid, &euid, &suid);
    return ruid;
}

uid_t mlibc::Sysdeps<GetEuid>::operator()()
{
    uid_t ruid = 0, euid = 0, suid = 0;
    sysdep<GetResuid>(&ruid, &euid, &suid);
    return euid;
}

gid_t mlibc::Sysdeps<GetGid>::operator()()
{
    gid_t rgid = 0, egid = 0, sgid = 0;
    sysdep<GetResgid>(&rgid, &egid, &sgid);
    return rgid;
}

gid_t mlibc::Sysdeps<GetEgid>::operator()()
{
    gid_t rgid = 0, egid = 0, sgid = 0;
    sysdep<GetResgid>(&rgid, &egid, &sgid);
    return egid;
}

int mlibc::Sysdeps<GetResuid>::operator()(uid_t* ruid, uid_t* euid, uid_t* suid)
{
    return vex_syscall_errno(__k_SYS_GETRESUID, reinterpret_cast<intptr_t>(ruid),
        reinterpret_cast<intptr_t>(euid), reinterpret_cast<intptr_t>(suid));
}

int mlibc::Sysdeps<GetResgid>::operator()(gid_t* rgid, gid_t* egid, gid_t* sgid)
{
    return vex_syscall_errno(__k_SYS_GETRESGID, reinterpret_cast<intptr_t>(rgid),
        reinterpret_cast<intptr_t>(egid), reinterpret_cast<intptr_t>(sgid));
}

int mlibc::Sysdeps<SetResuid>::operator()(uid_t ruid, uid_t euid, uid_t suid)
{
    return vex_syscall_errno(__k_SYS_SETRESUID, ruid, euid, suid);
}

int mlibc::Sysdeps<SetResgid>::operator()(gid_t rgid, gid_t egid, gid_t sgid)
{
    return vex_syscall_errno(__k_SYS_SETRESGID, rgid, egid, sgid);
}

// An id of -1 stands for "leave as is" in the setres calls
static constexpr intptr_t vex_id_unchanged = -1;

// POSIX asks for the effective capability here, not for a user id of zero
static int vex_capable(unsigned capability, bool* capable)
{
    struct __k_caps caps;
    int error = vex_syscall_errno(__k_SYS_CAPGET, reinterpret_cast<intptr_t>(&caps));

    if(error) {
        return error;
    }

    *capable = caps.effective & __k_CAP_TO_MASK(capability);
    return 0;
}

// A privileged caller sets all three ids,
// any other caller sets the effective one only
int mlibc::Sysdeps<SetUid>::operator()(uid_t uid)
{
    bool capable;
    int error = vex_capable(__k_CAP_SETUID, &capable);

    if(error) {
        return error;
    }

    if(capable) {
        return sysdep<SetResuid>(uid, uid, uid);
    }

    return sysdep<SetResuid>(vex_id_unchanged, uid, vex_id_unchanged);
}

int mlibc::Sysdeps<SetEuid>::operator()(uid_t euid)
{
    return sysdep<SetResuid>(vex_id_unchanged, euid, vex_id_unchanged);
}

int mlibc::Sysdeps<SetGid>::operator()(gid_t gid)
{
    bool capable;
    int error = vex_capable(__k_CAP_SETGID, &capable);

    if(error) {
        return error;
    }

    if(capable) {
        return sysdep<SetResgid>(gid, gid, gid);
    }

    return sysdep<SetResgid>(vex_id_unchanged, gid, vex_id_unchanged);
}

int mlibc::Sysdeps<SetEgid>::operator()(gid_t egid)
{
    return sysdep<SetResgid>(vex_id_unchanged, egid, vex_id_unchanged);
}

int mlibc::Sysdeps<GetPgid>::operator()(pid_t pid, pid_t* pgid)
{
    intptr_t result;
    int error = vex_syscall_result(&result, __k_SYS_GETPGID, pid);

    if(error) {
        return error;
    }

    *pgid = result;
    return 0;
}

int mlibc::Sysdeps<SetPgid>::operator()(pid_t pid, pid_t pgid)
{
    return vex_syscall_errno(__k_SYS_SETPGID, pid, pgid);
}

int mlibc::Sysdeps<GetSid>::operator()(pid_t pid, pid_t* sid)
{
    intptr_t result;
    int error = vex_syscall_result(&result, __k_SYS_GETSID, pid);

    if(error) {
        return error;
    }

    *sid = result;
    return 0;
}

int mlibc::Sysdeps<SetSid>::operator()(pid_t* sid)
{
    intptr_t result;
    int error = vex_syscall_result(&result, __k_SYS_SETSID);

    if(error) {
        return error;
    }

    *sid = result;
    return 0;
}

int mlibc::Sysdeps<GetGroups>::operator()(size_t size, gid_t* list, int* ret)
{
    intptr_t result;
    int error = vex_syscall_result(&result, __k_SYS_GETGROUPS, size,
        reinterpret_cast<intptr_t>(list));

    if(error) {
        return error;
    }

    *ret = result;
    return 0;
}

int mlibc::Sysdeps<SetGroups>::operator()(size_t size, const gid_t* list)
{
    return vex_syscall_errno(__k_SYS_SETGROUPS, size, reinterpret_cast<intptr_t>(list));
}

int mlibc::Sysdeps<Stat>::operator()(fsfd_target target, int fd, const char* path, int flags,
    struct stat* statbuf)
{
    switch(target) {
        case fsfd_target::fd:
            return vex_syscall_errno(__k_SYS_FSTAT, fd, reinterpret_cast<intptr_t>(statbuf));
        case fsfd_target::path:
            return vex_syscall_errno(__k_SYS_FSTATAT, __k_AT_FDCWD,
                reinterpret_cast<intptr_t>(path), reinterpret_cast<intptr_t>(statbuf), flags);
        case fsfd_target::fd_path:
            return vex_syscall_errno(__k_SYS_FSTATAT, fd, reinterpret_cast<intptr_t>(path),
                reinterpret_cast<intptr_t>(statbuf), flags);
        default:
            return EINVAL;
    }
}

int mlibc::Sysdeps<Openat>::operator()(int dirfd, const char* path, int flags, mode_t mode, int* fd)
{
    intptr_t result;
    int error = vex_syscall_result(&result, __k_SYS_OPENAT, dirfd, reinterpret_cast<intptr_t>(path),
        flags, mode);

    if(error) {
        return error;
    }

    *fd = result;
    return 0;
}

int mlibc::Sysdeps<Dup>::operator()(int fd, int flags, int* newfd)
{
    if(flags & ~O_CLOEXEC) {
        return EINVAL;
    }

    intptr_t result;
    int error = vex_syscall_result(&result, __k_SYS_FCNTL, fd,
        (flags & O_CLOEXEC) ? F_DUPFD_CLOEXEC : F_DUPFD, 0);

    if(error) {
        return error;
    }

    *newfd = result;
    return 0;
}

// The call without flags does not fail when both descriptors are the same one
int mlibc::Sysdeps<Dup2>::operator()(int fd, int flags, int newfd)
{
    if(flags == 0) {
        return vex_syscall_errno(__k_SYS_DUP2, fd, newfd);
    }

    return vex_syscall_errno(__k_SYS_DUP3, fd, newfd, flags);
}

// Only the requests that the kernel knows take an argument, and it is an integer
int mlibc::Sysdeps<Fcntl>::operator()(int fd, int request, va_list args, int* result)
{
    intptr_t argument = 0;

    switch(request) {
        case F_DUPFD:
        case F_DUPFD_CLOEXEC:
        case F_SETFD:
        case F_SETFL:
            argument = va_arg(args, int);
            break;
        case F_GETFD:
        case F_GETFL:
            break;
        default:
            return EINVAL;
    }

    intptr_t value;
    int error = vex_syscall_result(&value, __k_SYS_FCNTL, fd, request, argument);

    if(error) {
        return error;
    }

    *result = value;
    return 0;
}

int mlibc::Sysdeps<Ioctl>::operator()(int fd, unsigned long request, void* arg, int* result)
{
    intptr_t value;
    int error = vex_syscall_result(&value, __k_SYS_IOCTL, fd, request,
        reinterpret_cast<intptr_t>(arg));

    if(error) {
        return error;
    }

    *result = value;
    return 0;
}

int mlibc::Sysdeps<Ftruncate>::operator()(int fd, size_t size)
{
    return vex_syscall_errno(__k_SYS_FTRUNCATE, fd, size);
}

int mlibc::Sysdeps<Fsync>::operator()(int fd)
{
    return vex_syscall_errno(__k_SYS_FSYNC, fd);
}

// The kernel has no call that skips the metadata
int mlibc::Sysdeps<Fdatasync>::operator()(int fd)
{
    return vex_syscall_errno(__k_SYS_FSYNC, fd);
}

int mlibc::Sysdeps<Mkdirat>::operator()(int dirfd, const char* path, mode_t mode)
{
    return vex_syscall_errno(__k_SYS_MKDIRAT, dirfd, reinterpret_cast<intptr_t>(path), mode);
}

int mlibc::Sysdeps<Mkdir>::operator()(const char* path, mode_t mode)
{
    return sysdep<Mkdirat>(__k_AT_FDCWD, path, mode);
}

int mlibc::Sysdeps<Unlinkat>::operator()(int dirfd, const char* path, int flags)
{
    return vex_syscall_errno(__k_SYS_UNLINKAT, dirfd, reinterpret_cast<intptr_t>(path), flags);
}

int mlibc::Sysdeps<Rmdir>::operator()(const char* path)
{
    return sysdep<Unlinkat>(__k_AT_FDCWD, path, __k_AT_REMOVEDIR);
}

int mlibc::Sysdeps<Renameat>::operator()(int olddirfd, const char* old_path, int newdirfd,
    const char* new_path)
{
    return vex_syscall_errno(__k_SYS_RENAMEAT, olddirfd, reinterpret_cast<intptr_t>(old_path),
        newdirfd, reinterpret_cast<intptr_t>(new_path));
}

int mlibc::Sysdeps<Rename>::operator()(const char* path, const char* new_path)
{
    return sysdep<Renameat>(__k_AT_FDCWD, path, __k_AT_FDCWD, new_path);
}

int mlibc::Sysdeps<Linkat>::operator()(int olddirfd, const char* old_path, int newdirfd,
    const char* new_path, int flags)
{
    return vex_syscall_errno(__k_SYS_LINKAT, olddirfd, reinterpret_cast<intptr_t>(old_path),
        newdirfd, reinterpret_cast<intptr_t>(new_path), flags);
}

int mlibc::Sysdeps<Link>::operator()(const char* old_path, const char* new_path)
{
    return sysdep<Linkat>(__k_AT_FDCWD, old_path, __k_AT_FDCWD, new_path, 0);
}

int mlibc::Sysdeps<Symlinkat>::operator()(const char* target_path, int dirfd, const char* link_path)
{
    return vex_syscall_errno(__k_SYS_SYMLINKAT, reinterpret_cast<intptr_t>(target_path), dirfd,
        reinterpret_cast<intptr_t>(link_path));
}

int mlibc::Sysdeps<Symlink>::operator()(const char* target_path, const char* link_path)
{
    return sysdep<Symlinkat>(target_path, __k_AT_FDCWD, link_path);
}

int mlibc::Sysdeps<Readlinkat>::operator()(int dirfd, const char* path, void* buffer,
    size_t max_size, ssize_t* length)
{
    intptr_t result;
    int error = vex_syscall_result(&result, __k_SYS_READLINKAT, dirfd,
        reinterpret_cast<intptr_t>(path), reinterpret_cast<intptr_t>(buffer), max_size);

    if(error) {
        return error;
    }

    *length = result;
    return 0;
}

int mlibc::Sysdeps<Readlink>::operator()(const char* path, void* buffer, size_t max_size,
    ssize_t* length)
{
    return sysdep<Readlinkat>(__k_AT_FDCWD, path, buffer, max_size, length);
}

int mlibc::Sysdeps<Fchmodat>::operator()(int dirfd, const char* path, mode_t mode, int flags)
{
    return vex_syscall_errno(__k_SYS_FCHMODAT, dirfd, reinterpret_cast<intptr_t>(path), mode,
        flags);
}

int mlibc::Sysdeps<Chmod>::operator()(const char* path, mode_t mode)
{
    return sysdep<Fchmodat>(__k_AT_FDCWD, path, mode, 0);
}

int mlibc::Sysdeps<Fchmod>::operator()(int fd, mode_t mode)
{
    return sysdep<Fchmodat>(fd, "", mode, __k_AT_EMPTY_PATH);
}

int mlibc::Sysdeps<Fchownat>::operator()(int dirfd, const char* path, uid_t owner, gid_t group,
    int flags)
{
    return vex_syscall_errno(__k_SYS_FCHOWNAT, dirfd, reinterpret_cast<intptr_t>(path), owner,
        group, flags);
}

// A null path stands for the descriptor itself, as in futimens
int mlibc::Sysdeps<Utimensat>::operator()(int dirfd, const char* path,
    const struct timespec times[2], int flags)
{
    if(path == nullptr) {
        path = "";
        flags |= __k_AT_EMPTY_PATH;
    }

    return vex_syscall_errno(__k_SYS_UTIMENSAT, dirfd, reinterpret_cast<intptr_t>(path),
        reinterpret_cast<intptr_t>(times), flags);
}

int mlibc::Sysdeps<Chdir>::operator()(const char* path)
{
    return vex_syscall_errno(__k_SYS_CHDIR, reinterpret_cast<intptr_t>(path));
}

int mlibc::Sysdeps<Fchdir>::operator()(int fd)
{
    return vex_syscall_errno(__k_SYS_FCHDIR, fd);
}

// The kernel does not report a domain name, so that field stays empty
int mlibc::Sysdeps<Uname>::operator()(struct utsname* buf)
{
    struct __k_utsname name;
    int error = vex_syscall_errno(__k_SYS_UNAME, reinterpret_cast<intptr_t>(&name));

    if(error) {
        return error;
    }

    memset(buf, 0, sizeof(*buf));
    memcpy(buf->sysname, name.sysname, sizeof(name.sysname));
    memcpy(buf->nodename, name.nodename, sizeof(name.nodename));
    memcpy(buf->release, name.release, sizeof(name.release));
    memcpy(buf->version, name.version, sizeof(name.version));
    memcpy(buf->machine, name.machine, sizeof(name.machine));

    // TODO: get this from the kernel?
    strncpy(buf->domainname, "skibidisigma6767", sizeof(buf->domainname));

    return 0;
}

int mlibc::Sysdeps<Fstatvfs>::operator()(int fd, struct statvfs* out)
{
    struct __k_statvfs info;
    int error = vex_syscall_errno(__k_SYS_STATVFS, fd, reinterpret_cast<intptr_t>(&info));

    if(error) {
        return error;
    }

    memset(out, 0, sizeof(*out));
    out->f_bsize = info.f_bsize;
    out->f_frsize = info.f_frsize;
    out->f_blocks = info.f_blocks;
    out->f_bfree = info.f_bfree;
    out->f_bavail = info.f_bavail;
    out->f_files = info.f_files;
    out->f_ffree = info.f_ffree;
    out->f_favail = info.f_favail;
    out->f_fsid = info.f_fsid;
    out->f_flag = info.f_flag;
    out->f_namemax = info.f_namemax;
    return 0;
}

// The kernel takes a descriptor only, so the path goes through an open file
int mlibc::Sysdeps<Statvfs>::operator()(const char* path, struct statvfs* out)
{
    int fd;
    int error = sysdep<Openat>(__k_AT_FDCWD, path, O_RDONLY | O_CLOEXEC, 0, &fd);

    if(error) {
        return error;
    }

    error = sysdep<Fstatvfs>(fd, out);
    sysdep<Close>(fd);
    return error;
}

void mlibc::Sysdeps<Sync>::operator()()
{
    vex::syscall(__k_SYS_SYNC);
}

// The kernel returns zero when the child still runs and the call does not block
int mlibc::Sysdeps<Waitpid>::operator()(pid_t pid, int* status, int flags, struct rusage* ru,
    pid_t* ret_pid)
{
    intptr_t result;
    int error = vex_syscall_result(&result, __k_SYS_WAITPID, pid,
        reinterpret_cast<intptr_t>(status), flags);

    if(error) {
        return error;
    }

    // The kernel does not count resources yet
    if(ru) {
        memset(ru, 0, sizeof(*ru));
    }

    *ret_pid = result;
    return 0;
}

int mlibc::Sysdeps<GetCwd>::operator()(char* buffer, size_t size)
{
    return vex_syscall_errno(__k_SYS_GETCWD, reinterpret_cast<intptr_t>(buffer), size);
}

int mlibc::Sysdeps<Faccessat>::operator()(int dirfd, const char* pathname, int mode, int flags)
{
    return vex_syscall_errno(__k_SYS_FACCESSAT, dirfd, reinterpret_cast<intptr_t>(pathname), mode,
        flags);
}

int mlibc::Sysdeps<Access>::operator()(const char* path, int mode)
{
    return sysdep<Faccessat>(__k_AT_FDCWD, path, mode, 0);
}

int mlibc::Sysdeps<OpenDir>::operator()(const char* path, int* handle)
{
    return sysdep<Openat>(__k_AT_FDCWD, path, O_RDONLY | O_DIRECTORY | O_CLOEXEC, 0, handle);
}

// The records are those of struct dirent, so they go to the caller as they are
int mlibc::Sysdeps<ReadEntries>::operator()(int handle, void* buffer, size_t max_size,
    size_t* bytes_read)
{
    intptr_t result;
    int error = vex_syscall_result(&result, __k_SYS_GETDENTS, handle,
        reinterpret_cast<intptr_t>(buffer), max_size);

    if(error) {
        return error;
    }

    *bytes_read = result;
    return 0;
}

// The child gets zero from the kernel
int mlibc::Sysdeps<Fork>::operator()(pid_t* child)
{
    intptr_t result;
    int error = vex_syscall_result(&result, __k_SYS_FORK);

    if(error) {
        return error;
    }

    *child = result;
    return 0;
}

// The call returns only when it fails
int mlibc::Sysdeps<Execve>::operator()(const char* path, char* const argv[], char* const envp[])
{
    return vex_syscall_errno(__k_SYS_EXEC, reinterpret_cast<intptr_t>(path),
        reinterpret_cast<intptr_t>(argv), reinterpret_cast<intptr_t>(envp));
}

void mlibc::Sysdeps<Yield>::operator()()
{
    vex::syscall(__k_SYS_YIELD);
}

int mlibc::Sysdeps<Kill>::operator()(pid_t pid, int sig)
{
    return vex_syscall_errno(__k_SYS_KILL, pid, sig);
}

int mlibc::Sysdeps<Tgkill>::operator()(int tgid, int tid, int sig)
{
    return vex_syscall_errno(__k_SYS_TGKILL, tgid, tid, sig);
}

// The time left comes back in the arguments, which is zero unless a signal ended the sleep
int mlibc::Sysdeps<Sleep>::operator()(time_t* secs, long* nanos)
{
    struct __k_timespec request;
    request.tv_sec = *secs;
    request.tv_nsec = *nanos;

    struct __k_timespec remaining = {};
    int error = vex_syscall_errno(__k_SYS_SLEEP, reinterpret_cast<intptr_t>(&request),
        reinterpret_cast<intptr_t>(&remaining));

    *secs = remaining.tv_sec;
    *nanos = remaining.tv_nsec;
    return error;
}

static int vex_ioctl(int fd, unsigned long request, intptr_t argument)
{
    return vex_syscall_errno(__k_SYS_IOCTL, fd, request, argument);
}

int mlibc::Sysdeps<Tcgetattr>::operator()(int fd, struct termios* attr)
{
    return vex_ioctl(fd, __k_TCGETS, reinterpret_cast<intptr_t>(attr));
}

int mlibc::Sysdeps<Tcsetattr>::operator()(int fd, int optional_action, const struct termios* attr)
{
    switch(optional_action) {
        case TCSANOW:
            return vex_ioctl(fd, __k_TCSETS, reinterpret_cast<intptr_t>(attr));
        case TCSADRAIN:
            return vex_ioctl(fd, __k_TCSETSW, reinterpret_cast<intptr_t>(attr));
        case TCSAFLUSH:
            return vex_ioctl(fd, __k_TCSETSF, reinterpret_cast<intptr_t>(attr));
        default:
            return EINVAL;
    }
}

int mlibc::Sysdeps<Tcsendbreak>::operator()(int fd, int duration)
{
    __k_int32_t value = duration;
    return vex_ioctl(fd, __k_TCSENDBREAK, reinterpret_cast<intptr_t>(&value));
}

int mlibc::Sysdeps<Tcflow>::operator()(int fd, int action)
{
    __k_int32_t value = action;
    return vex_ioctl(fd, __k_TCXONC, reinterpret_cast<intptr_t>(&value));
}

int mlibc::Sysdeps<Tcflush>::operator()(int fd, int queue)
{
    __k_int32_t value = queue;
    return vex_ioctl(fd, __k_TCFLSH, reinterpret_cast<intptr_t>(&value));
}

int mlibc::Sysdeps<Tcdrain>::operator()(int fd)
{
    return vex_ioctl(fd, __k_TCDRAIN, 0);
}

int mlibc::Sysdeps<Tcgetwinsize>::operator()(int fd, struct winsize* winsz)
{
    return vex_ioctl(fd, __k_TIOCGWINSZ, reinterpret_cast<intptr_t>(winsz));
}

int mlibc::Sysdeps<Tcsetwinsize>::operator()(int fd, const struct winsize* winsz)
{
    return vex_ioctl(fd, __k_TIOCSWINSZ, reinterpret_cast<intptr_t>(winsz));
}

// The callers of these three take a negative number for an error
pid_t mlibc::Sysdeps<Tcgetpgrp>::operator()(int fd)
{
    __k_pid_t group;
    int error = vex_ioctl(fd, __k_TIOCGPGRP, reinterpret_cast<intptr_t>(&group));

    if(error) {
        return -error;
    }

    return group;
}

int mlibc::Sysdeps<Tcsetpgrp>::operator()(int fd, pid_t group)
{
    __k_pid_t value = group;
    return -vex_ioctl(fd, __k_TIOCSPGRP, reinterpret_cast<intptr_t>(&value));
}

pid_t mlibc::Sysdeps<Tcgetsid>::operator()(int fd)
{
    __k_pid_t session;
    int error = vex_ioctl(fd, __k_TIOCGSID, reinterpret_cast<intptr_t>(&session));

    if(error) {
        return -error;
    }

    return session;
}
