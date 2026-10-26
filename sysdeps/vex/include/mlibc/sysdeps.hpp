#pragma once

#include <mlibc/sysdep-signatures.hpp>

namespace mlibc
{
struct VexSysdepTags
    : LibcPanic,
      LibcLog,
      Isatty,
      Write,
      TcbSet,
      AnonAllocate,
      AnonFree,
      Seek,
      Exit,
      Close,
      FutexWake,
      FutexWait,
      Read,
      Open,
      VmMap,
      VmUnmap,
      ClockGet,
      GetPid,
      GetPpid,
      GetTid,
      GetUid,
      GetEuid,
      GetGid,
      GetEgid,
      GetResuid,
      GetResgid,
      SetResuid,
      SetResgid,
      SetUid,
      SetEuid,
      SetGid,
      SetEgid,
      GetPgid,
      SetPgid,
      GetSid,
      SetSid,
      GetGroups,
      SetGroups,
      Umask,
      Stat,
      Openat,
      Dup,
      Dup2,
      Fcntl,
      Ioctl,
      Ftruncate,
      Fsync,
      Fdatasync,
      Mkdir,
      Mkdirat,
      Unlinkat,
      Rmdir,
      Rename,
      Renameat,
      Link,
      Linkat,
      Symlink,
      Symlinkat,
      Readlink,
      Readlinkat,
      Chmod,
      Fchmod,
      Fchmodat,
      Fchownat,
      Utimensat,
      Chdir,
      Fchdir,
      Uname,
      Statvfs,
      Fstatvfs,
      Sync,
      Waitpid,
      GetCwd,
      Access,
      Faccessat,
      OpenDir,
      ReadEntries,
      Fork,
      Execve,
      Yield,
      Kill,
      Tgkill {};
} // namespace mlibc

namespace mlibc
{
template<typename Tag>
using Sysdeps = SysdepOf<VexSysdepTags, Tag>;
} // namespace mlibc

namespace mlibc
{
struct SysdepTraits {
    static constexpr bool usesRtNetlink = false;
};
} // namespace mlibc
