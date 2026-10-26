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
      ClockGet {};
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
