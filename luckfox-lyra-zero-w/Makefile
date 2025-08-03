
# local kernel build dir
# KERN_DIR ?= /lib/modules/$(shell uname -r)/build

# users kernel dir
ARCH := arm
CROSS_COMPILE := ${HOME}/luckfox/lyra/prebuilts/gcc/linux-x86/arm/gcc-arm-10.3-2021.07-x86_64-arm-none-linux-gnueabihf/bin/arm-none-linux-gnueabihf-
KERN_DIR := ${HOME}/luckfox/lyra/kernel

MODULE_NAME = panel-mipi-dbi

all:
	make ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) -C $(KERN_DIR) M=`pwd` modules
	./mipi-dbi-cmd panel-mipi-dbi-spi.bin rpi-dm-cl35bc219-40a.txt
clean:
	make ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) -C $(KERN_DIR) M=`pwd` modules clean

clena: clean

obj-m += $(MODULE_NAME).o
$(MODULE_NAME)-y += drm_mipi_dbi.o
$(MODULE_NAME)-y += panel_mipi_dbi.o
