#ifndef _ABIBITS_AUXV_H
#define _ABIBITS_AUXV_H

#include <vex/auxv.h>

#define AT_CLKTCK __k_AT_CLKTCK
#define AT_SECURE __k_AT_SECURE
#define AT_SYSINFO_EHDR __k_AT_SYSINFO_EHDR

/* The kernel does not put these entries in the vector yet */
#define AT_PLATFORM 15
#define AT_HWCAP 16
#define AT_FPUCW 18
#define AT_RANDOM 25
#define AT_HWCAP2 26
#define AT_HWCAP3 29
#define AT_HWCAP4 30
#define AT_EXECFN 31

#endif /* _ABIBITS_AUXV_H */
