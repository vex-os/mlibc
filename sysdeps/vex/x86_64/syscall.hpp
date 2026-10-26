#pragma once

#include <stdint.h>

// The number goes in rax and the arguments in rdi, rsi, rdx, r10, r8 and r9.
// The result is in rax and a negative errno stands for an error.
// SYSCALL itself clobbers rcx and r11, and the kernel keeps every other register.
// A call passes zero for the arguments that it does not name, and the kernel ignores them.

namespace vex
{
inline intptr_t syscall(intptr_t syscall_number, intptr_t arg_1 = 0, intptr_t arg_2 = 0,
    intptr_t arg_3 = 0, intptr_t arg_4 = 0, intptr_t arg_5 = 0, intptr_t arg_6 = 0);
} // namespace vex

inline intptr_t vex::syscall(intptr_t syscall_number, intptr_t arg_1, intptr_t arg_2,
    intptr_t arg_3, intptr_t arg_4, intptr_t arg_5, intptr_t arg_6)
{
    register intptr_t amd64_arg_4 asm("r10") = arg_4;
    register intptr_t amd64_arg_5 asm("r8") = arg_5;
    register intptr_t amd64_arg_6 asm("r9") = arg_6;

    intptr_t result;

    asm volatile("syscall"
        : "=a"(result)
        : "a"(syscall_number), "D"(arg_1), "S"(arg_2), "d"(arg_3), "r"(amd64_arg_4),
        "r"(amd64_arg_5), "r"(amd64_arg_6)
        : "rcx", "r11", "memory");
    return result;
}
