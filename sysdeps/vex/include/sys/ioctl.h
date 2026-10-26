#ifndef _SYS_IOCTL_H
#define _SYS_IOCTL_H

#include <vex/ioctl.h>

/* The requests are in the headers of the kernel, such as vex/serial.h and vex/termios.h */

#ifdef __cplusplus
extern "C" {
#endif

int ioctl(int __fd, unsigned long __request, ...);

#ifdef __cplusplus
}
#endif

#endif /* _SYS_IOCTL_H */
