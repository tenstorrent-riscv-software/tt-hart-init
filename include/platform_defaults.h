/* 
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText:  2025-2026 Tenstorrent USA, Inc.
 */

#ifndef PLATFORM_DEFAULTS_H
#define PLATFORM_DEFAULTS_H

// CPU architecture IDs
// Bit [63]: 1 = commercial core
// Bits [62:56]: IP family (0=Ascalon, 1=Babylon)
#define MARCHID_MASK                0xFF00000000000000
#define MARCHID_ASCALON             0x8000000000000000
#define MARCHID_BABYLON             0x8100000000000000

// Internal CSRs
#define C_FECFG2                    0xBC2
#define C_FECFG2_DIS_CSR_VXRM_OPT   (1 << 24)

// Setup early NME vector trap
#define SETUP_NMEVEC

// Base address of NMEVEC MMR is necessary to setup an early NME handler
#define NMEVEC_MMR                  0x42000028

// Per-hart distance between NMEVEC MMRs
#define NMEVEC_MMR_HART_STRIDE      0x10000

// Used to link ELF
#define INIT_BASE_ADDR              0x0

// Stack location and size per each core
#ifndef STACK_TOP
#define STACK_TOP                   0x10000000LU
#endif
#ifndef STACK_SIZE
#define STACK_SIZE                  0x1000LU
#endif

#endif /* PLATFORM_DEFAULTS_H */
