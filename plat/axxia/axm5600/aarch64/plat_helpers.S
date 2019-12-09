/*
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <arch.h>
#include <asm_macros.S>
#include <platform_def.h>

	.globl	plat_crash_console_init
	.globl	plat_crash_console_putc
	.globl	plat_crash_console_flush
	.globl	platform_get_core_pos
	.globl	plat_flush_dcache_l1
	.globl	plat_flush_dcache_l2
	.globl	plat_flush_dcache


	/* Define a crash console for the plaform */
#define AXXIA_CRASH_CONSOLE_BASE	PL011_UART0_BASE

/*
 * Return 0 to 3 for the Cortex-A53 cores and 4 to 5 for the Cortex-A57
 * cores.
 */
func platform_get_core_pos
	and	x1, x0, #MPIDR_CPU_MASK
	and	x0, x0, #MPIDR_CLUSTER_MASK
	/* Swap Cortex-A53/Cortex-A57 order. */
	eor	x0, x0, #(1 << MPIDR_AFF1_SHIFT)
	add	x0, x1, x0, LSR #6
	ret
endfunc platform_get_core_pos

/*---------------------------------------------
 * int plat_crash_console_init(void)
 *
 * Function to initialize the crash console
 * without a C Runtime to print crash report.
 * Clobber list : x0, x1, x2
 *---------------------------------------------
 */
func plat_crash_console_init
	mov_imm	x0, AXXIA_CRASH_CONSOLE_BASE
	mov_imm	x1, PL011_UART_CLK_IN_HZ
	mov_imm	x2, PL011_BAUDRATE
	b	console_core_init
endfunc plat_crash_console_init

/*---------------------------------------------
 * int plat_crash_console_putc(int c)
 *
 * Function to print a character on the crash
 * console without a C Runtime.
 * Clobber list : x1, x2
 *---------------------------------------------
 */
func plat_crash_console_putc
	mov_imm	x1, AXXIA_CRASH_CONSOLE_BASE
	b	console_core_putc
endfunc plat_crash_console_putc

/* ---------------------------------------------
 * int plat_crash_console_flush()
 * Function to force a write of all buffered
 * data that hasn't been output.
 * Out : return -1 on error else return 0.
 * Clobber list : r0 - r1
 * ---------------------------------------------
 */
func plat_crash_console_flush
	mov_imm	x1, AXXIA_CRASH_CONSOLE_BASE
	b	console_core_flush
endfunc plat_crash_console_flush
/*-----------------------------------------------------
 * void plat_flush_dcache_l1
 *
 * clean and invalidate one level cache.
 *
 * x0: cache level
 * x1: 0 flush & invalidate, 1 invalidate only
 * x2~x9: clobbered
 *-----------------------------------------------------
 */
func plat_flush_dcache_l1
	mov	x16, x30
	mov	x0, #0
	mov x1, #0
	bl	plat_flush_dcache
	mov	x30, x16
	ret
endfunc plat_flush_dcache_l1

/*-----------------------------------------------------
 * void plat_flush_dcache_l2
 *
 * clean and invalidate one level cache.
 *
 * x0: cache level
 * x1: 0 flush & invalidate, 1 invalidate only
 * x2~x9: clobbered
 *-----------------------------------------------------
 */
func plat_flush_dcache_l2
	mov	x16, x30
	mov	x0, #1
	mov x1, #0
	bl	plat_flush_dcache
	mov	x30, x16
	ret
endfunc plat_flush_dcache_l2

/*
 * void plat_flush_dcache(int invalidate_only)
 *
 * x0: cache level
 * x1: 0 flush & invalidate, 1 invalidate only
 *
 * clean and invalidate all data cache by SET/WAY.
 */
func plat_flush_dcache
	dsb	sy
	mrs	x10, clidr_el1		/* read clidr_el1 */
	lsr	x11, x10, #24
	and	x11, x11, #0x7		/* x11 <- loc */
	cbz	x11, finished		/* if loc is 0, exit */
	mov	x15, x30
	/* x0  <- cache level */
	/* x10 <- clidr_el1 */
	/* x11 <- loc */
	/* x15 <- return address */

	lsl	x12, x0, #1
	add	x12, x12, x0		/* x0 <- tripled cache level */
	lsr	x12, x10, x12
	and	x12, x12, #7		/* x12 <- cache type */
	cmp	x12, #2
	b.lt	cont			/* skip if no cache or icache */
	bl	plat_flush_dcache_level	/* x1 = 0 flush, 1 invalidate */

cont:
	mov	x0, #0
	msr	csselr_el1, x0		/* restore csselr_el1 */
	dsb	sy
	isb
	mov	x30, x15

finished:
	ret
endfunc plat_flush_dcache

/*
 * void plat_flush_dcache_level(level)
 *
 * clean and invalidate one level cache.
 *
 * x0: cache level
 * x1: 0 flush & invalidate, 1 invalidate only
 * x2~x9: clobbered
 */
func plat_flush_dcache_level
	lsl	x12, x0, #1
	msr	csselr_el1, x12		/* select cache level */
	isb				/* sync change of cssidr_el1 */
	mrs	x6, ccsidr_el1		/* read the new cssidr_el1 */
	and	x2, x6, #7		/* x2 <- log2(cache line size)-4 */
	add	x2, x2, #4		/* x2 <- log2(cache line size) */
	mov	x3, #0x3ff
	and	x3, x3, x6, lsr #3	/* x3 <- max number of #ways */
	clz	w5, w3			/* bit position of #ways */
	mov	x4, #0x7fff
	and	x4, x4, x6, lsr #13	/* x4 <- max number of #sets */
	/* x12 <- cache level << 1 */
	/* x2 <- line length offset */
	/* x3 <- number of cache ways - 1 */
	/* x4 <- number of cache sets - 1 */
	/* x5 <- bit position of #ways */

loop_set:
	mov	x6, x3			/* x6 <- working copy of #ways */
loop_way:
	lsl	x7, x6, x5
	orr	x9, x12, x7		/* map way and level to cisw value */
	lsl	x7, x4, x2
	orr	x9, x9, x7		/* map set number to cisw value */
	tbz	w1, #0, 1f
	dc	isw, x9
	b	2f
1:	dc	cisw, x9		/* clean & invalidate by set/way */
2:	subs	x6, x6, #1		/* decrement the way */
	b.ge	loop_way
	subs	x4, x4, #1		/* decrement the set */
	b.ge	loop_set

	ret
endfunc plat_flush_dcache_level
