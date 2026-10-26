#ifndef _ABIBITS_WAIT_H
#define _ABIBITS_WAIT_H

#include <mlibc-config.h>
#include <vex/wait.h>

/* TODO: the kernel has no job control in waitpid yet, so stopped and continued never show up */
#define WNOHANG __k_WNOHANG
#define WUNTRACED 2
#define WSTOPPED 2
#define WEXITED 4
#define WCONTINUED 8
#define WNOWAIT 0x01000000

#define __WCOREFLAG 0x80

#define WEXITSTATUS(x) __k_WEXITSTATUS(x)
#define WTERMSIG(x) __k_WTERMSIG(x)
#define WSTOPSIG(x) WEXITSTATUS(x)
#define WIFEXITED(x) __k_WIFEXITED(x)
#define WIFSIGNALED(x) (((signed char) (((x) & 0x7f) + 1) >> 1) > 0)
#define WIFSTOPPED(x) (((x) & 0xff) == 0x7f)
#define WIFCONTINUED(x) ((x) == 0xffff)

#if defined(_DEFAULT_SOURCE)
#define WCOREFLAG __WCOREFLAG
#endif

#if defined(_DEFAULT_SOURCE) || __MLIBC_POSIX2024
#define WCOREDUMP(x) ((x) & __WCOREFLAG)
#endif /* defined(_DEFAULT_SOURCE) || __MLIBC_POSIX2024 */

#if defined(_DEFAULT_SOURCE)
#define W_EXITCODE(ret, sig) __k_W_EXITCODE(ret, sig)
#endif /* defined(_DEFAULT_SOURCE) */

#endif /* _ABIBITS_WAIT_H */
