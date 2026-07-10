/* 
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText:  2025-2026 Tenstorrent USA, Inc.
 */

#include "helpers.h"
#include "platform.h"


.global platform_entry
.align 4
platform_entry:
    call pma_init

    // Cleanup GPRs
    GPR_INIT_DESTRUCTIVE()

    // Load PAYLOAD addr into t0 and jump
    li      t0, PAYLOAD
    jalr    x0, t0, 0   # jump to t0, x0 discard return

    // Unreachable
    IDLE()
