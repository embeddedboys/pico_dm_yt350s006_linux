
# local kernel build dir

MODULE_NAME = st7796u_fb

all:
	./mipi-dbi-cmd rpi-dm-hp35006.bin rpi-dm-hp35006.txt
	dtc -@ -I dts -O dtb -o sun8i-h3-spi-st7796u-drm.dtbo sun8i-h3-spi-st7796u-drm.dts
clean:
	rm -rf *.bin *.dtbo

install: all
	sudo cp rpi-dm-hp35006.bin /lib/firmware/
	sudo cp sun8i-h3-spi-st7796u-drm.dtbo /boot/dtb/overlays/

clena: clean