
# local kernel build dir
KERN_DIR ?= /lib/modules/$(shell uname -r)/build

# users kernel dir

MODULE_NAME = st7796u_fb

all:
	make -C $(KERN_DIR) M=`pwd` modules
	dtc -@ -I dts -O dtb -o rk3588-spi4-m0-cs1-st7796u-fb.dtbo rk3588-spi4-m0-cs1-st7796u-fb.dts
clean:
	make -C $(KERN_DIR) M=`pwd` modules clean

clena: clean

#CFLAGS_$(MODULE_NAME).o := -DDEBUG
obj-m += $(MODULE_NAME).o
