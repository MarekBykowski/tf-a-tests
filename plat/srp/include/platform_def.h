/*
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __PLATFORM_DEF_H__
#define __PLATFORM_DEF_H__

/* Memory map related regions */
#define DEVICE0_BASE		ULL(0x0400000000)
#define DEVICE0_SIZE		ULL(0xC0000000)

#define CACHE_WRITEBACK_GRANULE		0x40

/*******************************************************************************
 * Platform binary types for linking
 ******************************************************************************/
#ifndef AARCH32
#define PLATFORM_LINKER_FORMAT		"elf64-littleaarch64"
#define PLATFORM_LINKER_ARCH		aarch64
#else
#define PLATFORM_LINKER_FORMAT		"elf32-littlearm"
#define PLATFORM_LINKER_ARCH		arm
#endif

/*******************************************************************************
 * Generic platform constants
 ******************************************************************************/
/* See atf's ./plat/intel/axxia/include/platform_def.h for some defs */
/* Size of cacheable stacks */
#define PLATFORM_STACK_SIZE		0x800
/* Size of coherent stack */
#define PCPU_DV_MEM_STACK_SIZE          0x100

/* EGC is only one (ARM) Cluster */
#define PLATFORM_SYSTEM_COUNT		1
#define PLATFORM_CLUSTER_COUNT		1
#define PLATFORM_MAX_CPUS_PER_CLUSTER	4
#define PLATFORM_CORE_COUNT		(PLATFORM_CLUSTER_COUNT * \
						PLATFORM_MAX_CPUS_PER_CLUSTER)
#define PLATFORM_NUM_AFFS		(PLATFORM_CORE_COUNT + \
						PLATFORM_CLUSTER_COUNT + 1)
#define PLATFORM_MAX_AFFLVL		MPIDR_AFFLVL2
#define PLAT_MAX_PWR_LEVEL		PLATFORM_MAX_AFFLVL
#define PLAT_MAX_PWR_STATES_PER_LVL	2

#define PLAT_MAX_NR_SPI			491
#define PLAT_MAX_SPI_OFFSET_ID		(PLAT_MAX_NR_SPI - 32)

/* TODO: verify. Local state bit width for each level in the state-ID field of
 * power state
 */
#define PLAT_LOCAL_PSTATE_WIDTH		4
/* TODO: verify. All the platforms set 2 so do we */
#define PLAT_MAX_PWR_STATES_PER_LVL	2

/*******************************************************************************
 * Run-time address of the TFTF image.
 * It has to match the location where the Trusted Firmware-A loads the BL33
 * image.
 ******************************************************************************/
#define DRAM_BASE			ULL(0x0)
#define DRAM_SIZE			ULL(0x40000000)
#define TFTF_BASE			DRAM_BASE
#define TFTF_SIZE			(DRAM_BASE + DRAM_SIZE)

/* Memory mapped General-Purpose Timer 1 interfaces
 * For implementation in Linux kernel search for compatible string "arm,sp804"
 */
/*TODO: gp timer for SRP*/
#define GPTIMER0_BASE			ULL(0x8080220000)
/*******************************************************************************
 * Shared-Peripheral-Interrupt (SPI) ID. As per porting-guide.rst:
 * "SPI numbers are mapped onto GIC interrupt IDs, starting from interrupt
 * ID 32 In other words, the offset ID corresponds to the SPI number, to
 * which 32 must be added to get the GIC IRQ ID".
 * Note, that this is in contrast to Linux kernel that offsets SPI IDs from 0,
 * that is Linux' SPI ID 0 maps to GIC interrupt ID 32(=0+32).
 ******************************************************************************/
#define GPTIMER0_IRQ			62
#define GPTIMER0_FREQ			125000000

/*******************************************************************************
 * Generic Timers. ARM requires Generic Timers to be memory-mapped but there
 * are systems that fail so. Examples are Axxia Gen2 (5600, and XLF) in which
 * the Timer hasn't been mapped. For details see "ARM® Architecture Reference
 * Manual ARMv8, for ARMv8-A architecture profile Beta, "I 1.1 About
 * the Generic Timer specification".
 ******************************************************************************/
#define GEN_TIMERS_MEM_MAPPED		0

/*******************************************************************************
 * Non-Secure Software Generated Interupts IDs
 ******************************************************************************/
#define IRQ_NS_SGI_0		0
#define IRQ_NS_SGI_1		1
#define IRQ_NS_SGI_2		2
#define IRQ_NS_SGI_3		3
#define IRQ_NS_SGI_4		4
#define IRQ_NS_SGI_5		5
#define IRQ_NS_SGI_6		6
#define IRQ_NS_SGI_7		7

/* Per-CPU Non-Secure Timer Interrupt ID */
#define IRQ_PCPU_NS_TIMER		30
/* Per-CPU Hyperisor Timer Interrupt ID */
#define IRQ_PCPU_HP_TIMER		26
/* Per-CPU Non-Secure Timer Interrupt ID */
#define IRQ_PCPU_NS_TIMER		30

/*
 * TFTF_NVM_OFFSET/SIZE correspond to the NVM partition in the partition table
 * TODO: Not supported for now!
 */
#define TFTF_NVM_SIZE			ULL(0x8080100000)
#define TFTF_NVM_OFFSET			0x10000


/* ARM PL011 UART */
#define PL011_UART0_BASE                ULL(0x0480340000)
#define CRASH_CONSOLE_BASE              PL011_UART0_BASE
#define CRASH_CONSOLE_SIZE              0x10000
#define PL011_BAUDRATE                  9600
#define SIM_PER_CLOCK_HZ		ULL(125000000)
#define PL011_UART_CLK_IN_HZ		SIM_PER_CLOCK_HZ

/*
 * GIC-500. Intel decided to not use the legacy support for this GIC-500
 * configuration. This means the GIC-500 will run only in GICv3 compliant mode.
 */
#define GICC_BASE			ULL(0x8001000000)
#define GICD_BASE			ULL(0x8010000000)
#define GICR_BASE			ULL(0x8010400000)

/*******************************************************************************
 * Platform specific page table and MMU setup constants
 ******************************************************************************/
#if AARCH64
#define PLAT_PHY_ADDR_SPACE_SIZE	(ULL(1) << 40)
#define PLAT_VIRT_ADDR_SPACE_SIZE	(ULL(1) << 40)
#else
#define PLAT_PHY_ADDR_SPACE_SIZE	(ULL(1) << 32)
#define PLAT_VIRT_ADDR_SPACE_SIZE	(ULL(1) << 32)
#endif

#if IMAGE_TFTF
/* For testing xlat tables lib v2 */
#define MAX_XLAT_TABLES			20
#define MAX_MMAP_REGIONS		50
#else
#define MAX_XLAT_TABLES			5
#define MAX_MMAP_REGIONS		16
#endif

/*
 * Times(in ms) used by test code for completion of different events. Kept the
 * same as in FVP.
 */
#define PLAT_SUSPEND_ENTRY_TIME		15
#define PLAT_SUSPEND_ENTRY_EXIT_TIME	30

#endif /* __PLATFORM_DEF_H__ */
