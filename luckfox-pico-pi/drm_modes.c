/*
 * Copyright © 1997-2003 by The XFree86 Project, Inc.
 * Copyright © 2007 Dave Airlie
 * Copyright © 2007-2008 Intel Corporation
 *   Jesse Barnes <jesse.barnes@intel.com>
 * Copyright 2005-2006 Luc Verhaegen
 * Copyright (c) 2001, Andy Ritger  aritger@nvidia.com
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE COPYRIGHT HOLDER(S) OR AUTHOR(S) BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * Except as contained in this notice, the name of the copyright holder(s)
 * and author(s) shall not be used in advertising or otherwise to promote
 * the sale, use or other dealings in this Software without prior written
 * authorization from the copyright holder(s) and author(s).
 */

#include <linux/ctype.h>
#include <linux/list.h>
#include <linux/list_sort.h>
#include <linux/export.h>
#include <linux/fb.h>

#include <video/of_display_timing.h>
#include <video/of_videomode.h>
#include <video/videomode.h>

#include <drm/drm_crtc.h>
#include <drm/drm_device.h>
#include <drm/drm_edid.h>
#include <drm/drm_modes.h>
#include <drm/drm_print.h>

#ifdef CONFIG_OF
/**
 * of_get_drm_panel_display_mode - get a panel-timing drm_display_mode from devicetree
 * @np: device_node with the panel-timing specification
 * @dmode: will be set to the return value
 * @bus_flags: information about pixelclk, sync and DE polarity
 *
 * The mandatory Device Tree properties width-mm and height-mm
 * are read and set on the display mode.
 *
 * Returns:
 * Zero on success, negative error code on failure.
 */
int of_get_drm_panel_display_mode(struct device_node *np,
				  struct drm_display_mode *dmode, u32 *bus_flags)
{
	u32 width_mm = 0, height_mm = 0;
	struct display_timing timing;
	struct videomode vm;
	int ret;

	ret = of_get_display_timing(np, "panel-timing", &timing);
	if (ret)
		return ret;

	videomode_from_timing(&timing, &vm);

	memset(dmode, 0, sizeof(*dmode));
	drm_display_mode_from_videomode(&vm, dmode);
	if (bus_flags)
		drm_bus_flags_from_videomode(&vm, bus_flags);

	ret = of_property_read_u32(np, "width-mm", &width_mm);
	if (ret)
		return ret;

	ret = of_property_read_u32(np, "height-mm", &height_mm);
	if (ret)
		return ret;

	dmode->width_mm = width_mm;
	dmode->height_mm = height_mm;

	drm_mode_debug_printmodeline(dmode);

	return 0;
}
EXPORT_SYMBOL_GPL(of_get_drm_panel_display_mode);
#endif /* CONFIG_OF */