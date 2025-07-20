# OrangePi One panel-mipi-dbi driver for Raspberry Pi Display HATs

rpi-dm-hp35006 is the default display module for this project.

![pic](./assets/rpi-dm-on-orangepi-one.jpg)

lists of compatible hardware:

- [rpi-dm-yt350s006]()
- [rpi-dm-cl35bc219-40a]()

## TODO

- [ ] add backlight node in dt overlay

## Get started

... nothing yet.

## References

https://github.com/notro/panel-mipi-dbi/wiki
https://github.com/armbian/sunxi-DT-overlays
https://github.com/embeddedboys/rpi_dm_yt350s006_software

## More

Error log during development:


```c
[    3.344267] panel-mipi-dbi-spi spi0.0: /soc/spi@1c68000/st7796u@0: failed to get panel-timing (error=-2)
```

```c
[    3.341665] panel-mipi-dbi-spi spi0.0: /soc/spi@1c68000/st7796u@0: panel-timing out of bounds
```

```c
[    3.350494] panel-mipi-dbi-spi spi0.0: Direct firmware load for panel-mipi-dbi-spi.bin failed with error -2
[    3.350538] panel-mipi-dbi-spi spi0.0: Falling back to sysfs fallback for: panel-mipi-dbi-spi.bin
[   64.499684] panel-mipi-dbi-spi spi0.0: No config file found for compatible 'panel-mipi-dbi-spi' (error=-110)
[   64.499725] panel-mipi-dbi-spi spi0.0: probe with driver panel-mipi-dbi-spi failed with error -110
```

```c
[  101.558427] panel-mipi-dbi-spi spi0.0: supply power not found, using dummy regulator
[  101.558866] panel-mipi-dbi-spi spi0.0: supply io not found, using dummy regulator
[  101.568591] [drm] Initialized panel-mipi-dbi 1.0.0 for spi0.0 on minor 2
[  102.004418] panel-mipi-dbi-spi spi0.0: [drm] fb0: panel-mipi-dbid frame buffer device
```