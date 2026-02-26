/* 
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText:  2025-2026 Tenstorrent AI ULC
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

// OpenSBI and DTB start addresses
#ifndef OPENSBI
#define OPENSBI     0x80000000LU
#endif

#ifndef DTB
#define DTB         0x84000000LU
#endif

#endif /* PLATFORM_H */
