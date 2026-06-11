/* ascalon.ld - linker script for Ascalon CPU initialization */

#include "platform_defaults.h"

#ifdef HAVE_PLATFORM_H
#include "platform.h"
#endif

/* INIT_BASE is a mandatory variable and initialized with INIT_BASE_ADDR constant.
 * This is needed in case if next stage bootloader needs to allocate sections
 * in predefined offsets from the beginning of the init code.
 */
INIT_BASE = INIT_BASE_ADDR;

ENTRY(_start)

SECTIONS
{
    /* Ensure .text.start is the first in the binary and located at address 0.
     * As code is relocatable, it just helps to avoid it to be located in other
     * sections.*/
    . = INIT_BASE;
    .text.start : {
        KEEP(*(.text.start))
    }

    . = ALIGN(16);

    /* The rest of the text. Collect only the default .text section: all
     * general code lands here once per-function splitting is disabled.
     * Explicitly named .text.* sub-sections are intentionally NOT matched,
     * so a platform linker script can place them at fixed addresses itself. */
    .text : {
        *(.text)
        *(.rodata*)
    }

    . = ALIGN(16);   /* align for the following extra sections */
}
