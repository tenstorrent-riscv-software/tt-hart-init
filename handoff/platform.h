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

// Payload start addresses
#ifndef PAYLOAD
#define PAYLOAD     0x80000000
#endif

#endif /* PLATFORM_H */
