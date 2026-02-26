/* 
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText:  2025-2026 Tenstorrent AI ULC
 */

#ifndef HELPERS_H
#define HELPERS_H

// A two-level variadic macro that converts preprocessor tokens into a
// single C string literal. This is essential for turning raw assembly macros
// (which may contain commas) into a string for `asm()`. The two levels force
// inner macros to expand *before* being stringified.
#define STRINGIFY_IMPL(...) #__VA_ARGS__
#define STRINGIFY(...) STRINGIFY_IMPL(__VA_ARGS__)

// A two-level macro that concatenates two tokens into a single one.
// The two levels ensure that macro arguments (like `__LINE__`) are expanded
// before the concatenation happens.
#define GLUE_IMPL(a, b) a##b
#define GLUE(a, b) GLUE_IMPL(a, b)

// Destructive (zeroes ra and sp) reset of GPR registers
#define GPR_INIT_DESTRUCTIVE() \
    li x1, 0;  /* ra */         \
    li x2, 0;  /* sp */         \
    li x3, 0;  li x4, 0;  li x5, 0;  li x6, 0;  li x7, 0; \
    li x8, 0;  li x9, 0;  li x10, 0; li x11, 0; li x12, 0; li x13, 0; \
    li x14, 0; li x15, 0; li x16, 0; li x17, 0; li x18, 0; li x19, 0; \
    li x20, 0; li x21, 0; li x22, 0; li x23, 0; li x24, 0; li x25, 0; \
    li x26, 0; li x27, 0; li x28, 0; li x29, 0; li x30, 0; li x31, 0

// A C wrapper to execute the GPR_INIT_DESTRUCTIVE assembly
#define C_GPR_INIT_DESTRUCTIVE() asm volatile (STRINGIFY(GPR_INIT_DESTRUCTIVE()))

// An idle loop that uses the __LINE__ macro to create a unique label,
// allowing it to be called multiple times within the same function.
#define IDLE() \
    GLUE(idle_loop_, __LINE__): \
    wfi; \
    j GLUE(idle_loop_, __LINE__);

// A C wrapper to execute the IDLE assembly loop
#define C_IDLE() asm volatile (STRINGIFY(IDLE()))


#endif /* HELPERS_H */
