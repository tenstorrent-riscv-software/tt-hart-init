/*
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText:  2025-2026 Tenstorrent AI ULC
 *
 * c2opensbi.c
 *
 * Platform code for jumping to OpenSBI bootloader.
 *
 */

#include <stdint.h>
#include <stddef.h>

#include "helpers.h"
#include "helpers-c.h"
#include "platform.h"


// In CLANG 'naked' functions can't use any non-ASM statement, so in order to
// jump to normal 'main' we have to call setup_stack() first. As this is a
// minimalist example, skip 'main'.
__attribute__((naked, noreturn)) void platform_entry(void) {
    asm volatile(
        "jal pma_init\n"

        "csrr a0, mhartid\n\t"  // hart ID into a0
        "li a1, %0\n\t"         // DTB into a1
        "li t0, %1\n\t"         // OPENSBI addr

        // Workaround for OpenSBI which currently does not recognize the Smrnmi
        // extension:
        // Set mnstatus.NMIE to 1 to enable RNMIs. Without this, a non-maskable
        // event (NME) could jump to an uninitialized RNMI trap vector, causing
        // undefined behavior.
        "csrsi 0x744, 0x8\n\t"

        "jalr x0, t0, 0\n\t"    // jump
        :
        : "i"(DTB), "i"(OPENSBI)
        : "a0","a1","t0"
    );

    C_IDLE();
}
