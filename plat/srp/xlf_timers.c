/*
 * Copyright (c) 2018, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <assert.h>
#include <drivers/arm/sp804.h>
#include <platform.h>
#include <platform_def.h>
#include <stddef.h>
#include <timer.h>

static const plat_timer_t plat_timers = {
	.program = sp804_timer_program,
	.cancel = sp804_timer_cancel,
	.handler = sp804_timer_handler,
	.timer_step_value = 2,
	.timer_irq = GPTIMER0_IRQ 
};

int plat_initialise_timer_ops(const plat_timer_t **timer_ops)
{
	assert(timer_ops != NULL);
	*timer_ops = &plat_timers;

	/* Initialise the system timer */
	sp804_timer_init(GPTIMER0_BASE, GPTIMER0_FREQ);

	return 0;
}
