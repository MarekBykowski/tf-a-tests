#
# SPDX-License-Identifier: BSD-3-Clause
#

PLAT_INCLUDES	:=	-Iplat/axxia/axm5600/include/

PLAT_SOURCES	:=	plat/axxia/axm5600/axm5600_setup.c		\
			plat/axxia/axm5600/axm5600_pwr_state.c		\
			plat/axxia/axm5600/aarch64/plat_helpers.S		\
			plat/axxia/axm5600/axm5600_timers.c			\
			drivers/arm/pl011/${ARCH}/pl011_console.S	\
			drivers/arm/gic/arm_gic_v2v3.c			\
			drivers/arm/gic/gic_v2.c			\
			drivers/arm/gic/gic_v3.c			\
			drivers/arm/gic/gic_common.c			\
			drivers/arm/timer/private_timer.c		\
			drivers/arm/timer/sp804.c

TFTF_CFLAGS		+= -Wno-maybe-uninitialized

# Be bit more spitfull
LOG_LEVEL := 40

ifeq ($(USE_NVM),1)
$(error "axm5600 port of TFTF doesn't currently support USE_NVM=1")
endif
