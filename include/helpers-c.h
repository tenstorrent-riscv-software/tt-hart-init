/* 
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText:  2025-2026 Tenstorrent AI ULC
 */

#ifndef HELPERS_C_H
#define HELPERS_C_H

#include "platform_defaults.h"

#ifdef HAVE_PLATFORM_H
#include "platform.h"   // optional customer overrides
#endif

// -------- Stack allocation --------

// Setup stack based on current hart ID
__attribute__((naked)) void setup_stack(void)
{
    __asm__ volatile(

        // Get HartID
        "csrr t0, mhartid\n\t"

        // Current hart stack offset = STACK_SIZE * HartID
        "li   t1, %1\n\t"
        "mul  t0, t0, t1\n\t"

        // SP = STACK_TOP - current hart stack offset
        "li   t1, %0\n\t"
        "sub  sp, t1, t0\n\t"

        "ret\n\t"
        :
        : "i"(STACK_TOP), "i"(STACK_SIZE)
        : "t0", "t1" // Clobber list: tells the compiler we used t0 and t1.
    );
}

// -------- Accessing global variables --------

// Calculates relocation delta for position-independent global access.
uintptr_t get_anchor_address(void)
{
    uintptr_t delta;
    __asm__ volatile (
        ".global __anchor_symbol\n"
        "__anchor_symbol:\n\t"
        "auipc  %0, 0\n\t"                  // runtime address (PC)
        "lui    t0, %%hi(__anchor_symbol)\n\t"
        "addi   t0, t0, %%lo(__anchor_symbol)\n\t"
        "sub    %0, t0, %0\n\t"             // delta = link_time - runtime
        : "=&r"(delta)
        :
        : "t0"
    );
    return delta;
}

// Stores relocation delta into a local variable.
// Must be used once if function uses GET_PTR().
// Stack must already be set up.
#define ACCESS_GLOBAL_VAR_PROLOGUE()                   \
    uintptr_t __reloc_delta = get_anchor_address()

// Resolves the runtime pointer for a given global symbol.
#define GET_PTR(sym)                                   \
    ((typeof(&(sym)))((uintptr_t)(&(sym)) - __reloc_delta))
// --------------------------------------------

#endif /* HELPERS_C_H */
