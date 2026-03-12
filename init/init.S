/* 
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText:  2025-2026 Tenstorrent AI ULC
 */

#include "helpers.h"

#include "platform_defaults.h"

#ifdef HAVE_PLATFORM_H
#include "platform.h"   // optional customer overrides
#endif

// Non-Maskable Exception (NME) entry (per Smrnmi extension, when
// mnstatus.NMIE = 0)
// Safety idle-loop trap until platform software installs a proper handler
.align 4
.weak nmevec
nmevec:
    // Trap all 16 exception vectors
.option push
.option norvc
    .rept 16
        nop
    .endr
.option pop
    IDLE()

set_nmevec:
    # t0 = base of NME MMR region
    li      t0, NMEVEC_MMR

    # t1 = hart id
    csrr    t1, mhartid

    # t2 = hart-specific MMR address = base + (hart_id * STRIDE)
    li      t4, NMEVEC_MMR_HART_STRIDE
    mul     t2, t1, t4       # t2 = hart_id * stride
    add     t2, t2, t0       # t2 = base + hart_id*stride

    # t3 = address of nmevec handler
    la      t3, nmevec

    # store handler into hart-specific MMR
    sd      t3, 0(t2)

    # optional: memory fence to ensure write completes
    fence   iorw, iorw

    ret

// Generic PMA init, might be redefined by platform
.weak pma_init
pma_init:
    // pmacfg: size<<58 | addr<<12 | coh<<8 | cache<<7 | amo<<5 | type<<3 | perm<<0
    li    t0, 0x7c0000000000000f
    csrw  0x7e0, t0
    li    t1, 0x7c000000800001e7
    csrw  0x7e1, t1
    sfence.vma
    ret

// Floating-point init
.global fmv_init
fmv_init:
    // Save original mstatus
    csrr  x2, mstatus
    li    x3, 0x2000        // mstatus.FS = 1 (enable FPU)
    or    x4, x2, x3
    csrw  mstatus, x4

    // Save original misa
    csrr  x31, misa
    li    x3, 0x20          // misa.F = 1
    or    x4, x3, x31
    csrw  misa, x4

    // Zero FP registers
    fmv.w.x f0,  x0
    fmv.w.x f1,  x0
    fmv.w.x f2,  x0
    fmv.w.x f3,  x0
    fmv.w.x f4,  x0
    fmv.w.x f5,  x0
    fmv.w.x f6,  x0
    fmv.w.x f7,  x0
    fmv.w.x f8,  x0
    fmv.w.x f9,  x0
    fmv.w.x f10, x0
    fmv.w.x f11, x0
    fmv.w.x f12, x0
    fmv.w.x f13, x0
    fmv.w.x f14, x0
    fmv.w.x f15, x0
    fmv.w.x f16, x0
    fmv.w.x f17, x0
    fmv.w.x f18, x0
    fmv.w.x f19, x0
    fmv.w.x f20, x0
    fmv.w.x f21, x0
    fmv.w.x f22, x0
    fmv.w.x f23, x0
    fmv.w.x f24, x0
    fmv.w.x f25, x0
    fmv.w.x f26, x0
    fmv.w.x f27, x0
    fmv.w.x f28, x0
    fmv.w.x f29, x0
    fmv.w.x f30, x0
    fmv.w.x f31, x0

    // Restore
    csrw    mstatus, x2
    csrw    misa, x31
    ret

// Vector unit init
.global vec_init
vec_init:
    // Save original mstatus
    csrr  x2, mstatus
    li    x3, 0x200        // mstatus.VS = 1 (enable vector state)
    or    x4, x2, x3
    csrw  mstatus, x4

    // Save original misa
    csrr  x31, misa
    li    x3, 0x200000     // misa.V = 1
    or    x4, x3, x31
    csrw  misa, x4

    // Set vector configuration: vtype/vl
    li    x4, 0x0
    li    x5, 0x4
    li    x6, 0xd8
    vsetvl x4, x5, x6       // configure vector unit

    // Zero vector registers
    vmv.v.x v0,  x0
    vmv.v.x v1,  x0
    vmv.v.x v2,  x0
    vmv.v.x v3,  x0
    vmv.v.x v4,  x0
    vmv.v.x v5,  x0
    vmv.v.x v6,  x0
    vmv.v.x v7,  x0
    vmv.v.x v8,  x0
    vmv.v.x v9,  x0
    vmv.v.x v10, x0
    vmv.v.x v11, x0
    vmv.v.x v12, x0
    vmv.v.x v13, x0
    vmv.v.x v14, x0
    vmv.v.x v15, x0
    vmv.v.x v16, x0
    vmv.v.x v17, x0
    vmv.v.x v18, x0
    vmv.v.x v19, x0
    vmv.v.x v20, x0
    vmv.v.x v21, x0
    vmv.v.x v22, x0
    vmv.v.x v23, x0
    vmv.v.x v24, x0
    vmv.v.x v25, x0
    vmv.v.x v26, x0
    vmv.v.x v27, x0
    vmv.v.x v28, x0
    vmv.v.x v29, x0
    vmv.v.x v30, x0
    vmv.v.x v31, x0

    // Force VILL = 1
    li    t0, -1         // t0 = illegal VTYPE
    li    t1, 0
    vsetvl x0, t1, t0 // VL is set according to VTYPE

    // Restore mstatus and misa
    csrw mstatus, x2
    csrw misa, x31

    ret

// CPU-specific initialization dispatcher
// Not a standard callable function
// Makes nested calls without stack - cannot preserve ra
// Jumps to cpu_init_done label in _start instead of returning
// Reads marchid and calls appropriate init based on CPU type
cpu_init:
    csrr    t0, marchid

    // Mask bits [63:56] (commercial bit + IP family)
    li      t1, MARCHID_MASK
    and     t0, t0, t1

    // Check for Ascalon
    li      t1, MARCHID_ASCALON
    bne     t0, t1, skip_ascalon

    // Ascalon-specific init
    call    fmv_init
    call    vec_init
    j       cpu_init_ret

skip_ascalon:

    // Check for Babylon
    li      t1, MARCHID_BABYLON
    bne     t0, t1, skip_babylon

    // No Babylon-specific init is needed yet
    j       cpu_init_ret

skip_babylon:
#ifndef RELAXED_MARCHID
    // Unknown CPU - hang (aggressive mode by default)
    IDLE()
#endif

cpu_init_ret:
    j cpu_init_done

// Platform entry point. If defined in another source file, will get overwritten
.weak platform_entry
platform_entry:
    IDLE()

// Entry point
.section .text.start,"ax",@progbits
.global _start
.align 4
_start:
    GPR_INIT_DESTRUCTIVE()

    // Setup NME vector for safety if SETUP_NMEVEC is defined
#ifdef SETUP_NMEVEC
    call    set_nmevec
#endif

    j       cpu_init
cpu_init_done:

    jal     x0, platform_entry

    // Unreachable
    IDLE()
