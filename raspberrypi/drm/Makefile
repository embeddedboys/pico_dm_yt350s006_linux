
ARCH := arm64
CROSS_COMPILE := aarch64-linux-gnu-
KERN_DIR := /lib/modules/`uname -r`/build

MODULE_NAME = st7796u_drm

all:
	make ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) -C $(KERN_DIR) M=`pwd` modules
clean:
	make ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) -C $(KERN_DIR) M=`pwd` modules clean

dtb:
	dtc -@ -I dts -O dtb -o spi0-st7796u-drm.dtbo spi0-st7796u-drm.dts
	dtc -@ -I dts -O dtb -o i2c1-gt911.dtbo i2c1-gt911.dts

clena: clean

# CFLAGS_$(MODULE_NAME).o := -DDEBUG
CFLAGS_st7796u.o := -DTFT_MODEL_YT350S006=0 -DTFT_MODEL_HP35006_D=1
obj-m += $(MODULE_NAME).o
$(MODULE_NAME)-y += drm_mipi_dbi.o
$(MODULE_NAME)-y += st7796u.o

