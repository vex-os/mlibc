#ifndef _ABIBITS_UTSNAME_H
#define _ABIBITS_UTSNAME_H

#include <vex/utsname.h>

struct utsname {
	char sysname[__k_UNAME_SIZE];
	char nodename[__k_UNAME_SIZE];
	char release[__k_UNAME_SIZE];
	char version[__k_UNAME_SIZE];
	char machine[__k_UNAME_SIZE];
#if defined(_GNU_SOURCE)
	char domainname[__k_UNAME_SIZE];
#else
	char __domainname[__k_UNAME_SIZE];
#endif
};

#endif /* _ABIBITS_UTSNAME_H */
