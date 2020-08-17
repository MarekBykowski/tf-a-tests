/*
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <arch.h>
#include <assert.h>
#include <tftf_lib.h>
#include <drivers/console.h>
#include <drivers/arm/arm_gic.h>
#include <platform.h>
#include <platform_def.h>

static const mmap_region_t mmap[] = {
	MAP_REGION_FLAT(TFTF_BASE, TFTF_SIZE, MT_MEMORY | MT_RW | MT_NS),
	MAP_REGION_FLAT(DEVICE0_BASE, DEVICE0_SIZE, MT_DEVICE | MT_RW | MT_NS),
	{0}
};

void tftf_platform_setup(void)
{
	arm_gic_init(GICC_BASE, GICD_BASE, GICR_BASE);
	arm_gic_setup_global();
	arm_gic_setup_local();
}

void tftf_plat_arch_setup(void)
{
	INFO("mb: %s()\n", __func__);
	tftf_plat_configure_mmu();
}

void tftf_early_platform_setup(void)
{
	console_init(CRASH_CONSOLE_BASE, PL011_UART_CLK_IN_HZ,
		PL011_BAUDRATE);
}

extern void plat_get_nvm_handle(uintptr_t *handle);

static const unsigned char axxia_power_domain_tree_desc[] = {
	/* Number of system nodes */
	PLATFORM_SYSTEM_COUNT,
	/* Number of cluster nodes */
	PLATFORM_CLUSTER_COUNT,
	/* Number of children for the first node */
	PLATFORM_MAX_CPUS_PER_CLUSTER,
#if 0
	/* Number of children for the second node */
	PLATFORM_MAX_CPUS_PER_CLUSTER
	/* Number of children for the third node */
	PLATFORM_MAX_CPUS_PER_CLUSTER,
	/* Number of children for the fourth node */
	PLATFORM_MAX_CPUS_PER_CLUSTER
#endif
};
const unsigned char *tftf_plat_get_pwr_domain_tree_desc(void)
{
	return axxia_power_domain_tree_desc;
}

#if 0
static const struct {
	unsigned cluster_id;
	unsigned cpu_id;
} xlf_cores[] = {
	/* Cortex-A53 Cluster0: 4 cores*/
	{ 0, 0 },
	{ 0, 1 },
	{ 0, 2 },
	{ 0, 3 },
	/* Cortex-A53 Cluster1: 4 cores */
	{ 1, 0 },
	{ 1, 1 },
	{ 1, 2 },
	{ 1, 3 }
#if 0
	/* Cortex-A53 Cluster2: 4 cores */
	{ 2, 0 },
	{ 2, 1 },
	{ 2, 2 },
	{ 2, 3 },
	/* Cortex-A53 Cluster3: 4 cores */
	{ 3, 0 },
	{ 3, 1 },
	{ 3, 2 },
	{ 3, 3 },
#endif
	/* there are 8 of them but this is enough so far */
};
#endif

uint64_t tftf_plat_get_mpidr(unsigned int core_pos)
{
	uint64_t mpid;
	unsigned int coreid, clusterid;

	assert(core_pos < PLATFORM_CORE_COUNT);

	coreid = core_pos % PLATFORM_MAX_CPUS_PER_CLUSTER;
	clusterid = core_pos / PLATFORM_MAX_CPUS_PER_CLUSTER;

	if (clusterid >= PLATFORM_CLUSTER_COUNT)
		return INVALID_MPID;

	mpid = (coreid << MPIDR_AFF0_SHIFT) | (clusterid << MPIDR_AFF1_SHIFT);

	return mpid;
}

const mmap_region_t *tftf_platform_get_mmap(void)
{
	return mmap;
}
