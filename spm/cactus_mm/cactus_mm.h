/*
 * Copyright (c) 2017-2019, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __CACTUS_H__
#define __CACTUS_H__

#include <secure_partition.h>
#include <stdint.h>

/* Linker symbols used to figure out the memory layout of Cactus. */
extern uintptr_t __TEXT_START__, __TEXT_END__;
#define CACTUS_TEXT_START	((uintptr_t)&__TEXT_START__)
#define CACTUS_TEXT_END		((uintptr_t)&__TEXT_END__)

extern uintptr_t __RODATA_START__, __RODATA_END__;
#define CACTUS_RODATA_START	((uintptr_t)&__RODATA_START__)
#define CACTUS_RODATA_END	((uintptr_t)&__RODATA_END__)

extern uintptr_t __RWDATA_START__, __RWDATA_END__;
#define CACTUS_RWDATA_START	((uintptr_t)&__RWDATA_START__)
#define CACTUS_RWDATA_END	((uintptr_t)&__RWDATA_END__)

extern uintptr_t __BSS_START__, __BSS_END__;
#define CACTUS_BSS_START	((uintptr_t)&__BSS_START__)
#define CACTUS_BSS_END		((uintptr_t)&__BSS_END__)

/*
 * Once Cactus has finished its initialisation, this is the function it will
 * jump to to handle runtime services for the rest of its lifetime.
 */
__dead2 void secure_services_loop(void);

#endif /* __CACTUS_H__ */
