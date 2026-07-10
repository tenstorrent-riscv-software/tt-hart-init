/* 
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText:  2025-2026 Tenstorrent USA, Inc.
 *
 * c-runtime-example.c
 *
 * Demonstrates how to setup C runtime.
 *
 */

#include <stdint.h>
#include <stddef.h>

#include "helpers.h"
#include "helpers-c.h"
#include "platform.h"


// In CLANG 'naked' functions can't use any non-ASM statements, so perform
// minimum setup and jump to normal 'main'
__attribute__((naked, noreturn)) void platform_entry(void) {
    asm volatile(
        "jal pma_init\n"
        "jal setup_stack\n"
        "jal main\n"
    );

    C_IDLE();
}

#define DATA_SIZE 32

// src goes to .bss section
uint8_t src[DATA_SIZE] = {0};

// dst goes to .data section
uint8_t dst[DATA_SIZE] = {
    0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 0x20, 0x21,
    0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f, 0x30, 0x31,
    0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f, 0x40, 0x41,
    0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f, 0x50, 0x51,
};

// Normal C function
void memcpy(void* src, void* dst, size_t n)
{
    // Use stack
    uint8_t* s = src;
    uint8_t* d = dst;

    while(n--) {
        *d++ = *s++;
    };
}

__attribute__((noreturn)) void main(void)
{
    // Relocatable access to global variables
    ACCESS_GLOBAL_VAR_PROLOGUE();

    // Dereference GET_PTR() because it returns a pointer to the global array
    // and dereferencing gives the address of its first element.
    uint8_t *src_p = *GET_PTR(src);
    uint8_t *dst_p = *GET_PTR(dst);

    // Access memory
    for (int i = 0; i < DATA_SIZE; i++) {
        src_p[i] = i;
    }

    // Call normal C function
    memcpy(src_p, dst_p, DATA_SIZE);

    // Cleanup GPRs just as an example to make sure almost everything is clean
    C_GPR_INIT_DESTRUCTIVE();

    // Done

    C_IDLE();

    __builtin_unreachable(); /* tells compiler: never returns */
}
