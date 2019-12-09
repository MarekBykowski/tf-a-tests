/*
 * Copyright (c) 2018, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdint.h>
#include <tftf.h>
#include <debug.h>
#include <psci.h>
#include <platform.h>

smc_ret_values asm_tftf_smc64(uint32_t fid,
			      u_register_t arg1,
			      u_register_t arg2,
			      u_register_t arg3,
			      u_register_t arg4,
			      u_register_t arg5,
			      u_register_t arg6,
			      u_register_t arg7);

smc_ret_values tftf_smc(const smc_args *args)
{
	smc_ret_values ret_vals;

	INFO("mb: CPU#%u: fid %x "
	     "arg[1..4] %lx %lx %lx %lx\n",
	     platform_get_core_pos(read_mpidr_el1()),
	     args->fid, args->arg1,
	     args->arg2, args->arg3,
	     args->arg4);

	ret_vals = asm_tftf_smc64(args->fid,
			      args->arg1,
			      args->arg2,
			      args->arg3,
			      args->arg4,
			      args->arg5,
			      args->arg6,
			      args->arg7);

	INFO("\tret.ret0 %lx\n", ret_vals.ret0);

	return ret_vals;
}
