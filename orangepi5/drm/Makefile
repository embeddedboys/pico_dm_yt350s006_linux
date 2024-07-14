
# local kernel build dir
KERN_DIR ?= /lib/modules/$(shell uname -r)/build

# users kernel dir

MODULE_NAME = st7796u_drm

all:
	make -C $(KERN_DIR) M=`pwd` modules
clean:
	make -C $(KERN_DIR) M=`pwd` modules clean
dtb:
	dtc -@ -I dts -O dtb -o rk3588-spi4-m0-cs1-st7796u-drm.dtbo rk3588-spi4-m0-cs1-st7796u-drm.dts

clena: clean

#CFLAGS_$(MODULE_NAME).o := -DDEBUG
obj-m += $(MODULE_NAME).o
$(MODULE_NAME)-y += drm_mipi_dbi.o
$(MODULE_NAME)-y += st7796u.o

