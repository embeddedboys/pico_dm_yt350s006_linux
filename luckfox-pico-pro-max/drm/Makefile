
# local kernel build dir
# KERN_DIR ?= /lib/modules/$(shell uname -r)/build

# users kernel dir
ARCH := arm
CROSS_COMPILE := /home/developer/sources/luckfox-pico/tools/linux/toolchain/arm-rockchip830-linux-uclibcgnueabihf/bin/arm-rockchip830-linux-uclibcgnueabihf-
KERN_DIR := /home/developer/sources/luckfox-pico/sysdrv/source/kernel

MODULE_NAME = st7796u_drm

all:
	make ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) -C $(KERN_DIR) M=`pwd` modules
clean:
	make ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) -C $(KERN_DIR) M=`pwd` modules clean

clena: clean

CFLAGS_st7796u.o := -DTFT_MODEL_YT350S006=0 -DTFT_MODEL_HP35006_D=1
obj-m += $(MODULE_NAME).o
$(MODULE_NAME)-y += drm_mipi_dbi.o
$(MODULE_NAME)-y += st7796u.o

