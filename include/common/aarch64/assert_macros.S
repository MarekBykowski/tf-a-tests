/*
 * Copyright (c) 2018, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __ASSERT_MACROS_S__
#define __ASSERT_MACROS_S__

	/*
	 * Assembler macro to enable asm_assert. Use this macro wherever
	 * assert is required in assembly. Please note that the macro makes
	 * use of label '300' to provide the logic and the caller
	 * should make sure that this label is not used to branch prior
	 * to calling this macro.
	 */
#define ASM_ASSERT(_cc) \
.ifndef .L_assert_filename ;\
	.pushsection .rodata.str1.1, "aS" ;\
	.L_assert_filename: ;\
			.string	__FILE__ ;\
	.popsection ;\
.endif ;\
	b._cc	300f ;\
	adr	x0, .L_assert_filename ;\
	mov	x1, __LINE__ ;\
	b	asm_assert ;\
300:

#endif /* __ASSERT_MACROS_S__ */
