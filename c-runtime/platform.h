/* 
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText:  2025-2026 Tenstorrent USA, Inc.
 */

#ifndef PLATFORM_H
#define PLATFORM_H

// -------- Redefine default constants --------

// Use default NMEVEC_MMR

// Use default NMEVEC_MMR_HART_STRIDE

// Redefine INIT_BASE_ADDR
#undef  INIT_BASE_ADDR
#define INIT_BASE_ADDR              0x10000

// -------- Define project specific constants --------

// Stack location and size per each core
#undef STACK_TOP
#define STACK_TOP                   0x20000000LU

#undef STACK_SIZE
#define STACK_SIZE                  0x2000LU

#endif /* PLATFORM_H */
