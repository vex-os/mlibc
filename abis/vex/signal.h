#ifndef _ABIBITS_SIGNAL_H
#define _ABIBITS_SIGNAL_H

#include <abi-bits/pid_t.h>
#include <abi-bits/sigevent.h>
#include <abi-bits/sigset_t.h>
#include <abi-bits/uid_t.h>
#include <bits/size_t.h>
#include <vex/signal.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	int si_signo;
	int si_code;
	int si_errno;
	int si_status;
	pid_t si_pid;
	uid_t si_uid;
	void *si_addr;
	union sigval si_value;
} siginfo_t;

typedef void (*__sighandler)(int);

#define SIG_ERR ((__sighandler)(void *)(-1))
#define SIG_DFL ((__sighandler)(void *)(__k_SIG_DFL))
#define SIG_IGN ((__sighandler)(void *)(__k_SIG_IGN))

#define SIGHUP     __k_SIGHUP
#define SIGINT     __k_SIGINT
#define SIGQUIT    __k_SIGQUIT
#define SIGILL     __k_SIGILL
#define SIGTRAP    __k_SIGTRAP
#define SIGABRT    __k_SIGABRT
#define SIGBUS     __k_SIGBUS
#define SIGFPE     __k_SIGFPE
#define SIGKILL    __k_SIGKILL
#define SIGSEGV    __k_SIGSEGV
#define SIGPIPE    __k_SIGPIPE
#define SIGUSR1    __k_SIGUSR1
#define SIGUSR2    __k_SIGUSR2
#define SIGALRM    __k_SIGALRM
#define SIGTERM    __k_SIGTERM
#define SIGCHLD    __k_SIGCHLD
#define SIGCONT    __k_SIGCONT
#define SIGSTOP    __k_SIGSTOP
#define SIGTSTP    __k_SIGTSTP
#define SIGTTIN    __k_SIGTTIN
#define SIGTTOU    __k_SIGTTOU
#define SIGURG     __k_SIGURG
#define SIGXCPU    __k_SIGXCPU
#define SIGXFSZ    __k_SIGXFSZ
#define SIGVTALRM  __k_SIGVTALRM
#define SIGPROF    __k_SIGPROF
#define SIGWINCH   __k_SIGWINCH
#define SIGSYS     __k_SIGSYS
#define SIGINFO    __k_SIGINFO
#define SIGIO      __k_SIGIO
#define SIGPWR     __k_SIGPWR

#define SIGIOT SIGABRT
#define SIGPOLL SIGIO

/* The first real-time signals belong to the C library: SIGCANCEL and a spare one */
#define SIGCANCEL __k_SIGRTMIN
#define SIGRTMIN (__k_SIGRTMIN + 2)
#define SIGRTMAX __k_SIGRTMAX

#define NSIG (__k_SIGRTMAX + 1)
#define _NSIG NSIG

#define SIG_BLOCK    __k_SIG_BLOCK
#define SIG_UNBLOCK  __k_SIG_UNBLOCK
#define SIG_SETMASK  __k_SIG_SETMASK

#define SA_NOCLDSTOP  __k_SA_NOCLDSTOP
#define SA_NOCLDWAIT  __k_SA_NOCLDWAIT
#define SA_SIGINFO    __k_SA_SIGINFO
#define SA_ONSTACK    __k_SA_ONSTACK
#define SA_RESTART    __k_SA_RESTART
#define SA_NODEFER    __k_SA_NODEFER
#define SA_RESETHAND  __k_SA_RESETHAND
#define SA_RESTORER   __k_SA_RESTORER

#define SA_NOMASK SA_NODEFER
#define SA_ONESHOT SA_RESETHAND

#define MINSIGSTKSZ __k_MINSIGSTKSZ
#define SIGSTKSZ __k_SIGSTKSZ
#define SS_ONSTACK __k_SS_ONSTACK
#define SS_DISABLE __k_SS_DISABLE

#define SIGEV_NONE 1
#define SIGEV_SIGNAL 2
#define SIGEV_THREAD 3

#define SI_USER     __k_SI_USER
#define SI_QUEUE    __k_SI_QUEUE
#define SI_TIMER    __k_SI_TIMER
#define SI_MESGQ    __k_SI_MESGQ
#define SI_ASYNCIO  __k_SI_ASYNCIO
#define SI_KERNEL   __k_SI_KERNEL
#define SI_TKILL    __k_SI_TKILL

#define ILL_ILLOPC     __k_ILL_ILLOPC
#define ILL_ILLOPN     __k_ILL_ILLOPN
#define ILL_ILLADR     __k_ILL_ILLADR
#define ILL_ILLTRP     __k_ILL_ILLTRP
#define ILL_PRVOPC     __k_ILL_PRVOPC
#define ILL_PRVREG     __k_ILL_PRVREG
#define ILL_COPROC     __k_ILL_COPROC
#define ILL_BADSTK     __k_ILL_BADSTK
#define SEGV_MAPERR    __k_SEGV_MAPERR
#define SEGV_ACCERR    __k_SEGV_ACCERR
#define BUS_ADRALN     __k_BUS_ADRALN
#define BUS_ADRERR     __k_BUS_ADRERR
#define BUS_OBJERR     __k_BUS_OBJERR

struct sigaction {
	union {
		void (*sa_handler)(int);
		void (*sa_sigaction)(int, siginfo_t *, void *);
	};
	unsigned long sa_flags;
	void (*sa_restorer)(void);
	sigset_t sa_mask;
};

typedef struct __k_stack stack_t;
typedef struct __k_mcontext mcontext_t;
typedef struct __k_ucontext ucontext_t;

#ifdef __cplusplus
}
#endif

#endif /* _ABIBITS_SIGNAL_H */
