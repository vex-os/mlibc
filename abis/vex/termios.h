#ifndef _ABIBITS_TERMIOS_H
#define _ABIBITS_TERMIOS_H

#include <vex/termios.h>

typedef unsigned char cc_t;
typedef unsigned int speed_t;
typedef unsigned int tcflag_t;

#define NCCS __k_NCCS

#define VINTR     __k_VINTR
#define VQUIT     __k_VQUIT
#define VERASE    __k_VERASE
#define VKILL     __k_VKILL
#define VEOF      __k_VEOF
#define VEOL      __k_VEOL
#define VEOL2     __k_VEOL2
#define VSTART    __k_VSTART
#define VSTOP     __k_VSTOP
#define VSUSP     __k_VSUSP
#define VREPRINT  __k_VREPRINT
#define VWERASE   __k_VWERASE
#define VLNEXT    __k_VLNEXT
#define VDISCARD  __k_VDISCARD
#define VMIN      __k_VMIN
#define VTIME     __k_VTIME

#define IGNBRK   __k_IGNBRK
#define BRKINT   __k_BRKINT
#define IGNPAR   __k_IGNPAR
#define PARMRK   __k_PARMRK
#define INPCK    __k_INPCK
#define ISTRIP   __k_ISTRIP
#define INLCR    __k_INLCR
#define IGNCR    __k_IGNCR
#define ICRNL    __k_ICRNL
#define IUCLC    __k_IUCLC
#define IXON     __k_IXON
#define IXANY    __k_IXANY
#define IXOFF    __k_IXOFF
#define IMAXBEL  __k_IMAXBEL
#define IUTF8    __k_IUTF8

#define OPOST   __k_OPOST
#define OLCUC   __k_OLCUC
#define ONLCR   __k_ONLCR
#define OCRNL   __k_OCRNL
#define ONOCR   __k_ONOCR
#define ONLRET  __k_ONLRET
#define OFILL   __k_OFILL
#define OFDEL   __k_OFDEL
#define NLDLY   __k_NLDLY
#define NL0     __k_NL0
#define NL1     __k_NL1
#define CRDLY   __k_CRDLY
#define CR0     __k_CR0
#define CR1     __k_CR1
#define CR2     __k_CR2
#define CR3     __k_CR3
#define TABDLY  __k_TABDLY
#define TAB0    __k_TAB0
#define TAB1    __k_TAB1
#define TAB2    __k_TAB2
#define TAB3    __k_TAB3
#define BSDLY   __k_BSDLY
#define BS0     __k_BS0
#define BS1     __k_BS1
#define VTDLY   __k_VTDLY
#define VT0     __k_VT0
#define VT1     __k_VT1
#define FFDLY   __k_FFDLY
#define FF0     __k_FF0
#define FF1     __k_FF1

#define CSIZE    __k_CSIZE
#define CS5      __k_CS5
#define CS6      __k_CS6
#define CS7      __k_CS7
#define CS8      __k_CS8
#define CSTOPB   __k_CSTOPB
#define CREAD    __k_CREAD
#define PARENB   __k_PARENB
#define PARODD   __k_PARODD
#define HUPCL    __k_HUPCL
#define CLOCAL   __k_CLOCAL
#define CRTSCTS  __k_CRTSCTS

#define ISIG     __k_ISIG
#define ICANON   __k_ICANON
#define XCASE    __k_XCASE
#define ECHO     __k_ECHO
#define ECHOE    __k_ECHOE
#define ECHOK    __k_ECHOK
#define ECHONL   __k_ECHONL
#define ECHOPRT  __k_ECHOPRT
#define ECHOCTL  __k_ECHOCTL
#define ECHOKE   __k_ECHOKE
#define NOFLSH   __k_NOFLSH
#define TOSTOP   __k_TOSTOP
#define IEXTEN   __k_IEXTEN

#define NOKERNINFO __k_NOKERNINFO

struct termios {
	tcflag_t c_iflag;
	tcflag_t c_oflag;
	tcflag_t c_cflag;
	tcflag_t c_lflag;
	cc_t c_cc[NCCS];
	speed_t c_ispeed;
	speed_t c_ospeed;
};

#define B0 0
#define B50 50
#define B75 75
#define B110 110
#define B134 134
#define B150 150
#define B200 200
#define B300 300
#define B600 600
#define B1200 1200
#define B1800 1800
#define B2400 2400
#define B4800 4800
#define B9600 9600
#define B19200 19200
#define B38400 38400
#define B57600 57600
#define B115200 115200
#define B230400 230400
#define B460800 460800
#define B500000 500000
#define B576000 576000
#define B921600 921600
#define B1000000 1000000
#define B1152000 1152000
#define B1500000 1500000
#define B2000000 2000000
#define B2500000 2500000
#define B3000000 3000000
#define B3500000 3500000
#define B4000000 4000000
#define EXTA B19200
#define EXTB B38400

#define TCSANOW 0
#define TCSADRAIN 1
#define TCSAFLUSH 2

#define TCIFLUSH   __k_TCIFLUSH
#define TCOFLUSH   __k_TCOFLUSH
#define TCIOFLUSH  __k_TCIOFLUSH
#define TCOOFF     __k_TCOOFF
#define TCOON      __k_TCOON
#define TCIOFF     __k_TCIOFF
#define TCION      __k_TCION

#endif /* _ABIBITS_TERMIOS_H */
