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
      Umask {};
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
