#ifndef _ABIBITS_LIMITS_H
#define _ABIBITS_LIMITS_H

#include <vex/limits.h>
#include <vex/utsname.h>

#define __MLIBC_IOV_MAX 1024
#define __MLIBC_LOGIN_NAME_MAX 256
#define __MLIBC_HOST_NAME_MAX (__k_UNAME_SIZE - 1)
#define __MLIBC_NAME_MAX __k_NAME_MAX
#define __MLIBC_OPEN_MAX 1024
#define __MLIBC_TTY_NAME_MAX 32

#endif /* _ABIBITS_LIMITS_H */
