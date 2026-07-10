/*
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText:  2025-2026 Tenstorrent USA, Inc.
 *
 * c2opensbi.ld - linker script for c2opensbi platform
 */

SECTIONS
{
    /* Optional: create a room between CPU init and c2opensbi data sections if needed */
    PLATFORM_BASE = INIT_BASE;
    . = PLATFORM_BASE + 0x1000;
    .data : { *(.data) }
    .bss : { *(.bss) }
}
