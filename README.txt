RISC-V HART INITIALIZATION FRAMEWORK
=====================================

OVERVIEW
--------
This project is a lightweight, relocatable HART initialization framework for
Tenstorrent RISC-V platforms. It is designed to be relocatable, customizable,
and serves as a minimal foundation for initializing CPU cores before handing
off to platform-specific code (bootloaders, firmware, etc.).

The core principle is CPU initialization code in init/ folder that performs
initial hardware setup (with runtime CPU detection) before jumping to
platform-defined entry point.


FEATURES
--------
* Relocatable Code: Generated .bin file can be loaded and run from any memory
  address.
* Runtime CPU Detection: Single binary supports multiple CPU types via marchid
  CSR detection.
* GCC & Clang Support: Build with selected RISC-V toolchain.
* Default Safety Handler: Includes a default, overridable Non-Maskable Exception
  (NME) handler.


BOOT FLOW
---------
The boot process consists of CPU initialization followed by platform code:

CPU Initialization (init/):
    The init folder contains hardware initialization code. At runtime, the
    marchid CSR is read to detect the CPU type and dispatch to the appropriate
    init routine. This code performs critical hardware pre-initialization (FPU,
    vector unit, NME setup) and then jumps to the `platform_entry` symbol,
    which must be defined by the platform code.

Platform Code (handoff/, etc.):
    Platform-specific folders define the `platform_entry` symbol as their entry
    point. The default `platform_entry` is a weak symbol that simply halts the
    HART, so platform code will automatically override it. Platform code can
    include its own linker script (.ld) for memory layout control and a
    `platform.h` to override default configuration constants.


PROJECT ORGANIZATION
--------------------
init/:
    Contains CPU initialization code with runtime detection. The init.S file
    reads the marchid CSR to determine the CPU type and performs appropriate
    CPU-specific initialization (FPU, vector unit, etc.). Also includes the
    generic linker script (init.ld).

include/:
    Shared headers available to all build targets.

Root-level folders:
    Build targets (platforms, examples, tests).

Root-level source files:
    Any .c or .S files in the root directory are compiled into the default
    target: hart-init.elf and hart-init.bin.


HOW TO BUILD
------------
Prerequisites: A RISC-V toolchain (GCC or Clang).

You can configure the build using the `CROSS_COMPILE` and `COMPILER` make
variables.
    - `CROSS_COMPILE`: The toolchain prefix (e.g., `riscv64-unknown-elf-`).
    - `COMPILER`: The compiler to use (`gcc` or `clang`).

Build Commands:
    - `make`: Compiles the root project.
        Output: `hart-init.elf` and `hart-init.bin`
    - `make <PLATFORM>`: Compiles a specific platform.
        Example: `make handoff` produces `handoff/handoff.bin`
    - `make clean`: Cleans the root project build artifacts.
    - `make clean-<PLATFORM>`: Cleans a specific platform folder.
        Example: `make clean-handoff`.
    - `make clean-all`: Cleans all build artifacts from all folders.

    Binary naming convention: <platform>.bin (e.g., handoff.bin)


CONFIGURATION AND CUSTOMIZATION
-------------------------------
Platform Constants:
    You can override default hardware constants by creating a `platform.h`
file inside your platform directory. The default values are in
`include/platform_defaults.h`:

    #define SETUP_NMEVEC
        Setup early NME vector trap.

    #define NMEVEC_MMR           0x42000028
        NMEVEC MMR address to setup early exception handler for safety.

    #define NMEVEC_MMR_HART_STRIDE 0x10000
        Per-hart stride value to find the corresponding NMEVEC MMR.

    #define INIT_BASE_ADDR            0x0
        Used to link the ELF file.

Linker and Relocatability:
    The code is designed to be fully relocatable. The `INIT_BASE` variable in
the linker script (`init/init.ld`) is set to `0x0` by default. While the code
itself is position-independent, this variable allows a next-stage linker to
offset sections relative to the start of the init code if needed.


WRITING RELOCATABLE CODE
------------------------
To ensure your C code remains position-independent and works correctly in a
relocatable bootloader, you should never access global variables or string
literals directly.

Instead, use the helper macros provided in include/helpers-c.h:

ACCESS_GLOBAL_VAR_PROLOGUE() – Stores the relocation delta in a local variable.
Must be called once at the beginning of a function if you want to access globals
via GET_PTR().

GET_PTR(symbol) – Returns a runtime pointer to the specified global variable or
literal, accounting for relocation. Use this instead of directly referencing the
symbol.


EXCEPTION HANDLING
------------------
A default Non-Maskable Exception (NME) vector (`nmevec`) is provided as a weak
symbol. By default, it halts the HART that triggered the exception. You can
provide your own implementation of `nmevec` in your platform code to define
custom behavior.


EXAMPLES
--------
c-runtime:
    The `c-runtime` directory provides an example demonstrating how to:
    1. Set up a basic C environment (e.g., stack pointer).
    2. Implement `platform_entry` using C-style functions.
    3. Access global variables and string literals in relocatable code.
    4. Use normal C functions (like an embedded `memcpy`).
    5. Alternatively, override constants dynamically at build time using
       `CFLAGS`, e.g., `make c-runtime CFLAGS="-DSTACK_TOP=0x70001000LU -DSTACK_SIZE=0x2000LU"`.
    The compiled output will halt the HART after running the demonstration code.

c2opensbi:
    The `c2opensbi` directory provides an example demonstrating how to:
    1. Implement `platform_entry` using C-style functions.
    2. Jump to a next-stage payload, such as OpenSBI.
    3. Control memory locations and sizes (OpenSBI, DTB, etc.) by defining
       constants in `platform.h`.
    4. Alternatively, override these constants dynamically at build time using
       `CFLAGS`, e.g., `make c2opensbi CFLAGS="-DOPENSBI=0x80010000LU -DDTB=0x83205000LU"`.

handoff:
    The `handoff` directory provides a minimalist, assembly-only platform
    implementation. Its purpose is to immediately hand off control to another
    payload.
    It demonstrates how to:
    1. Perform basic platform initialization.
    2. Clean the general-purpose registers (not required but mimics fresh HW state).
    3. Jump to a generic payload address.
    4. Define the `PAYLOAD` address in `platform.h`, which can also be
       overridden at build time, e.g., `make handoff CFLAGS="-DPAYLOAD=0x90000000"`.

LICENSE
-------
This project is licensed under the Apache License, Version 2.0.

Copyright 2025-2026 Tenstorrent AI ULC.
