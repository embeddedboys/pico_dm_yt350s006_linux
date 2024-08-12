// SPDX-License-Identifier: GPL-2.0+
/*
 * DRM driver for display panels connected to a Sitronix ST7715R or ST7735R
 * display controller in SPI mode.
 *
 * Copyright 2017 David Lechner <david@lechnology.com>
 * Copyright (C) 2019 Glider bvba
 */

#include <linux/backlight.h>
#include <linux/delay.h>
#include <linux/dma-buf.h>
#include <linux/gpio/consumer.h>
#include <linux/module.h>
#include <linux/property.h>
#include <linux/spi/spi.h>
#include <video/mipi_display.h>

#include <drm/drm_atomic_helper.h>
#include <drm/drm_drv.h>
#include <drm/drm_fbdev_generic.h>
#include <drm/drm_gem_atomic_helper.h>
#include <drm/drm_gem_dma_helper.h>
#include <drm/drm_managed.h>
#include <drm/drm_mipi_dbi.h>

#define ST7796U_FRMCTR1		0xb1
#define ST7796U_FRMCTR2		0xb2
#define ST7796U_FRMCTR3		0xb3
#define ST7796U_INVCTR		0xb4
#define ST7796U_PWCTR1		0xc0
#define ST7796U_PWCTR2		0xc1
#define ST7796U_PWCTR3		0xc2
#define ST7796U_PWCTR4		0xc3
#define ST7796U_PWCTR5		0xc4
#define ST7796U_VMCTR1		0xc5
#define ST7796U_GAMCTRP1	0xe0
#define ST7796U_GAMCTRN1	0xe1

#define ST7796U_MY	BIT(7)
#define ST7796U_MX	BIT(6)
#define ST7796U_MV	BIT(5)
#define ST7796U_ML	BIT(4)
#define ST7796U_RGB	BIT(3)
#define ST7796U_MH	BIT(2)

struct st7796u_cfg {
	const struct drm_display_mode mode;
	unsigned int left_offset;
	unsigned int top_offset;
	unsigned int write_only:1;
	unsigned int rgb:1;		/* RGB (vs. BGR) */
};

struct st7796u_priv {
	struct mipi_dbi_dev dbidev;	/* Must be first for .release() */
	const struct st7796u_cfg *cfg;
	struct gpio_desc *led;
};

static void st7796u_pipe_enable(struct drm_simple_display_pipe *pipe,
				struct drm_crtc_state *crtc_state,
				struct drm_plane_state *plane_state)
{
	struct mipi_dbi_dev *dbidev = drm_to_mipi_dbi_dev(pipe->crtc.dev);
	struct st7796u_priv *priv = container_of(dbidev, struct st7796u_priv,
						 dbidev);
	struct mipi_dbi *dbi = &dbidev->dbi;
	int ret, idx;
	u8 addr_mode;

	if (!drm_dev_enter(pipe->crtc.dev, &idx))
		return;

	DRM_DEBUG_KMS("\n");

	ret = mipi_dbi_poweron_reset(dbidev);
	if (ret)
		goto out_exit;

#if TFT_MODEL_YT350S006
	mipi_dbi_command(dbi, MIPI_DCS_EXIT_SLEEP_MODE);
	msleep(120);

	mipi_dbi_command(dbi, 0xf0, 0xc3);
	mipi_dbi_command(dbi, 0xf0, 0x96);

	switch (dbidev->rotation) {
	default:
		addr_mode = 0;
		break;
	case 90:
		addr_mode = ST7796U_MV;
		break;
	case 180:
		addr_mode = ST7796U_MX | ST7796U_MY;
		break;
	case 270:
		addr_mode = ST7796U_MX | ST7796U_MV;
		break;
	}

	if (priv->cfg->rgb)
		addr_mode |= ST7796U_RGB;

	mipi_dbi_command(dbi, MIPI_DCS_SET_ADDRESS_MODE, addr_mode);
	mipi_dbi_command(dbi, 0xb4, 0x01);
	mipi_dbi_command(dbi, MIPI_DCS_SET_PIXEL_FORMAT,
			 MIPI_DCS_PIXEL_FMT_16BIT);

	mipi_dbi_command(dbi, 0xe8, 0x40, 0x82, 0x07, 0x18, 0x27, 0x0a, 0xb6, 0x33);
	mipi_dbi_command(dbi, 0xc5, 0x23);
	mipi_dbi_command(dbi, 0xc2, 0xa7);
	mipi_dbi_command(dbi, ST7796U_GAMCTRP1, 0xF0, 0x01, 0x06, 0x0F, 0x12, 0x1D, 0x36, 0x54, 0x44, 0x0C, 0x18, 0x16, 0x13, 0x15);
	mipi_dbi_command(dbi, ST7796U_GAMCTRN1, 0xF0, 0x01, 0x05, 0x0A, 0x0B, 0x07, 0x32, 0x44, 0x44, 0x0C, 0x18, 0x17, 0x13, 0x16);

	mipi_dbi_command(dbi, 0xf0, 0x3c);
	mipi_dbi_command(dbi, 0xf0, 0x69);

	mipi_dbi_command(dbi, MIPI_DCS_SET_DISPLAY_ON);
#elif TFT_MODEL_HP35006_D
	mipi_dbi_command(dbi, 0xf0, 0xc3);
	mipi_dbi_command(dbi, 0xf0, 0x96);

	switch (dbidev->rotation) {
	default:
		addr_mode = 0;
		break;
	case 90:
		addr_mode = ST7796U_MV;
		break;
	case 180:
		addr_mode = ST7796U_MX | ST7796U_MY;
		break;
	case 270:
		addr_mode = ST7796U_MX | ST7796U_MV;
		break;
	}

	if (priv->cfg->rgb)
		addr_mode |= ST7796U_RGB;

	mipi_dbi_command(dbi, MIPI_DCS_SET_ADDRESS_MODE, addr_mode);
	mipi_dbi_command(dbi, MIPI_DCS_SET_PIXEL_FORMAT, MIPI_DCS_PIXEL_FMT_16BIT);
	mipi_dbi_command(dbi, 0xb4, 0x01);
	mipi_dbi_command(dbi, 0xb1, 0x80, 0x01);
	mipi_dbi_command(dbi, 0xb5, 0x1f, 0x50, 0x00, 0x20);
	mipi_dbi_command(dbi, 0xb6, 0x8A, 0x07, 0x3b);

	mipi_dbi_command(dbi, 0xc0, 0x80, 0x64);
	mipi_dbi_command(dbi, 0xc1, 0x13);
	mipi_dbi_command(dbi, 0xc2, 0xa7);
	mipi_dbi_command(dbi, 0xc5, 0x09);

	mipi_dbi_command(dbi, 0xe8, 0x40, 0x8a, 0x00, 0x00, 0x29, 0x19, 0xA5, 0x33);
	mipi_dbi_command(dbi, ST7796U_GAMCTRP1, 0xF0, 0x06, 0x0B, 0x07, 0x06, 0x05, 0x2E, 0x33, 0x47, 0x3A, 0x17, 0x16, 0x2E, 0x31);
	mipi_dbi_command(dbi, ST7796U_GAMCTRN1, 0xF0, 0x09, 0x0D, 0x09, 0x08, 0x23, 0x2E, 0x33, 0x46, 0x38, 0x13, 0x13, 0x2C, 0x32);

	mipi_dbi_command(dbi, 0xf0, 0x3c);
	mipi_dbi_command(dbi, 0xf0, 0x69);

	mipi_dbi_command(dbi, 0x35, 0x00);
	mipi_dbi_command(dbi, MIPI_DCS_EXIT_SLEEP_MODE);
	msleep(120);

	mipi_dbi_command(dbi, MIPI_DCS_SET_DISPLAY_ON);
	msleep(50);
	mipi_dbi_command(dbi, 0x21);
#else
	#error "At least one model must be defined"
#endif

	mipi_dbi_enable_flush(dbidev, crtc_state, plane_state);
out_exit:
	drm_dev_exit(idx);
}

static const struct drm_simple_display_pipe_funcs st7796u_pipe_funcs = {
	DRM_MIPI_DBI_SIMPLE_DISPLAY_PIPE_FUNCS(st7796u_pipe_enable),
};

static const struct st7796u_cfg yt350s006_cfg = {
	.mode		= { DRM_SIMPLE_MODE(320, 480, 49, 73) },
	/* Cannot read from Adafruit 1.8" display via SPI */
	.write_only	= false,
	.rgb = true,
};

DEFINE_DRM_GEM_DMA_FOPS(st7796u_fops);

static const struct drm_driver st7796u_driver = {
	.driver_features	= DRIVER_GEM | DRIVER_MODESET | DRIVER_ATOMIC,
	.fops			= &st7796u_fops,
	DRM_GEM_DMA_DRIVER_OPS_VMAP,
	.debugfs_init		= mipi_dbi_debugfs_init,
	.name			= "st7796u",
	.desc			= "Sitronix ST7796U",
	.date			= "20240712",
	.major			= 1,
	.minor			= 0,
};

static const struct of_device_id st7796u_of_match[] = {
	{ .compatible = "yaoyuanhong,yt350s006", .data = &yt350s006_cfg },
	{ },
};
MODULE_DEVICE_TABLE(of, st7796u_of_match);

static const struct spi_device_id st7796u_id[] = {
	{ "yt350s006", (uintptr_t)&yt350s006_cfg },
	{ },
};
MODULE_DEVICE_TABLE(spi, st7796u_id);

static int st7796u_probe(struct spi_device *spi)
{
	struct device *dev = &spi->dev;
	const struct st7796u_cfg *cfg;
	struct mipi_dbi_dev *dbidev;
	struct st7796u_priv *priv;
	struct drm_device *drm;
	struct mipi_dbi *dbi;
	struct gpio_desc *dc;
	u32 rotation = 0;
	int ret;

	cfg = device_get_match_data(&spi->dev);
	if (!cfg)
		cfg = (void *)spi_get_device_id(spi)->driver_data;

	priv = devm_drm_dev_alloc(dev, &st7796u_driver,
				  struct st7796u_priv, dbidev.drm);
	if (IS_ERR(priv))
		return PTR_ERR(priv);

	dbidev = &priv->dbidev;
	priv->cfg = cfg;

	dbi = &dbidev->dbi;
	drm = &dbidev->drm;

	dbi->reset = devm_gpiod_get(dev, "reset", GPIOD_OUT_HIGH);
	if (IS_ERR(dbi->reset))
		return dev_err_probe(dev, PTR_ERR(dbi->reset), "Failed to get GPIO 'reset'\n");

	dc = devm_gpiod_get(dev, "dc", GPIOD_OUT_LOW);
	if (IS_ERR(dc))
		return dev_err_probe(dev, PTR_ERR(dc), "Failed to get GPIO 'dc'\n");

	// dbidev->backlight = devm_of_find_backlight(dev);
	// if (IS_ERR(dbidev->backlight))
	// 	return PTR_ERR(dbidev->backlight);

	priv->led = devm_gpiod_get(dev, "led", GPIOD_OUT_LOW);
	if (IS_ERR(priv->led))
		return dev_err_probe(dev, PTR_ERR(priv->led), "Failed to get GPIO 'led'\n");
	gpiod_set_value_cansleep(priv->led, 1);

	device_property_read_u32(dev, "rotation", &rotation);

	ret = mipi_dbi_spi_init(spi, dbi, dc);
	if (ret)
		return ret;

	if (cfg->write_only)
		dbi->read_commands = NULL;

	dbidev->left_offset = cfg->left_offset;
	dbidev->top_offset = cfg->top_offset;

	ret = mipi_dbi_dev_init(dbidev, &st7796u_pipe_funcs, &cfg->mode,
				rotation);
	if (ret)
		return ret;

	drm_mode_config_reset(drm);

	ret = drm_dev_register(drm, 0);
	if (ret)
		return ret;

	spi_set_drvdata(spi, drm);

	drm_fbdev_generic_setup(drm, 0);

	return 0;
}

static void st7796u_remove(struct spi_device *spi)
{
	struct drm_device *drm = spi_get_drvdata(spi);

	drm_dev_unplug(drm);
	drm_atomic_helper_shutdown(drm);
}

static void st7796u_shutdown(struct spi_device *spi)
{
	drm_atomic_helper_shutdown(spi_get_drvdata(spi));
}

static struct spi_driver st7796u_spi_driver = {
	.driver = {
		.name = "st7796u",
		.of_match_table = st7796u_of_match,
	},
	.id_table = st7796u_id,
	.probe = st7796u_probe,
	.remove = st7796u_remove,
	.shutdown = st7796u_shutdown,
};
module_spi_driver(st7796u_spi_driver);

MODULE_DESCRIPTION("Sitronix ST7796U DRM driver");
MODULE_AUTHOR("Zheng Hua <writeforever@foxmail.com>");
MODULE_LICENSE("GPL");
