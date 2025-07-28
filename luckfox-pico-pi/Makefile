
# local kernel build dir
# KERN_DIR ?= /lib/modules/$(shell uname -r)/build

# users kernel dir
ARCH := arm
CROSS_COMPILE := ${HOME}/luckfox/pico/tools/linux/toolchain/arm-rockchip830-linux-uclibcgnueabihf/bin/arm-rockchip830-linux-uclibcgnueabihf-
KERN_DIR := ${HOME}/luckfox/pico/sysdrv/source/objs_kernel

MODULE_NAME = panel-mipi-dbi

all:
	make ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) -C $(KERN_DIR) M=`pwd` modules
	./mipi-dbi-cmd panel-mipi-dbi-spi.bin rpi-dm-cl35bc219-40a.txt
clean:
	make ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) -C $(KERN_DIR) M=`pwd` modules clean

clena: clean

obj-m += $(MODULE_NAME).o
$(MODULE_NAME)-y += drm_modes.o
$(MODULE_NAME)-y += drm_mipi_dbi.o
$(MODULE_NAME)-y += panel_mipi_dbi.o
