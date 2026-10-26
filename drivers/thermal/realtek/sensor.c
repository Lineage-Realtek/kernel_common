// SPDX-License-Identifier: GPL-2.0-only
/*
 * sensor.c - Realtek generic thermal sensor driver
 *
 * Copyright (C) 2017-2018,2020 Realtek Semiconductor Corporation
 * Author: Cheng-Yu Lee <cylee12@realtek.com>
 */

#include <linux/delay.h>
#include <linux/of.h>
#include <linux/platform_device.h>
#include <linux/pm.h>
#include <linux/slab.h>
#include <linux/thermal.h>
#include <linux/module.h>
#include <linux/mfd/syscon.h>
#include <linux/of_device.h>
#include <linux/of_address.h>
#include "sensor.h"

static inline int thermal_sensor_hw_init(struct thermal_sensor_device *tdev)
{
	const struct thermal_sensor_desc *desc = tdev->desc;
	int ret;

	if (!desc->hw_ops->init)
		return 0;

	ret = desc->hw_ops->init(tdev);
	tdev->last_reset_time = ktime_get();
	return ret;
}

static inline void thermal_sensor_hw_exit(struct thermal_sensor_device *tdev)
{
	const struct thermal_sensor_desc *desc = tdev->desc;

	if (!desc->hw_ops->exit)
		return;

	desc->hw_ops->exit(tdev);
}

static inline void thermal_sensor_hw_reset(struct thermal_sensor_device *tdev)
{
	const struct thermal_sensor_desc *desc = tdev->desc;

	if (!desc->hw_ops->reset)
		return;

	desc->hw_ops->reset(tdev);
	tdev->last_reset_time = ktime_get();
}

static inline int thermal_sensor_hw_get_temp(struct thermal_sensor_device *tdev,
					     int *temp)
{
	const struct thermal_sensor_desc *desc = tdev->desc;

	if (!desc->hw_ops->get_temp)
		return -EINVAL;

	if (desc->reset_time_ms) {
		s64 t;

		t = ktime_ms_delta(ktime_add_ms(tdev->last_reset_time, desc->reset_time_ms),
				ktime_get());
		if (t > 0) {
			dev_info(tdev->dev, "wait %lldms to be ready\n", t);
			msleep(t);
		}
	}

	return desc->hw_ops->get_temp(tdev, temp);
}

static inline
void thermal_sensor_hw_dump_status(struct thermal_sensor_device *tdev)
{
	const struct thermal_sensor_desc *desc = tdev->desc;
	unsigned int val;
	int ret;
	int i;

	if (!desc->status_cnt)
		return;

	for (i = 0; i < desc->status_cnt; i++) {
		ret = thermal_sensor_device_reg_read(tdev, desc->status_ofs + i * 4, &val);
		dev_warn(tdev->dev, "status%d, ofs=%04x, val=%08x, ret=%d\n",
			 i, desc->status_ofs + i * 4, val, ret);
	}
}

static inline int is_vaild_temp(int temp, const struct thermal_sensor_desc *desc)
{
        int max_temp = 150000;
        int min_temp = -3000;

        if (desc->has_valid_temp) {
                max_temp = desc->valid_max_temp;
                min_temp = desc->valid_min_temp;
        }

        if (temp < min_temp || temp > max_temp)
                return 0;
        return 1;
}

static int thermal_sensor_get_temp(void *data, int *temp)
{
	struct thermal_sensor_device *tdev = data;
	int ret;

	mutex_lock(&tdev->lock);
	ret = thermal_sensor_hw_get_temp(tdev, temp);
	mutex_unlock(&tdev->lock);

	if (ret || !is_vaild_temp(*temp, tdev->desc)) {
		dev_warn(tdev->dev, "invalid temp=%d\n", *temp);
		thermal_sensor_hw_dump_status(tdev);

		mutex_lock(&tdev->lock);
		thermal_sensor_hw_reset(tdev);
		ret = thermal_sensor_hw_get_temp(tdev, temp);
		mutex_unlock(&tdev->lock);
	}

	dev_dbg(tdev->dev, "temp=%d\n", *temp);
	return ret;
}

static const struct thermal_zone_of_device_ops thermal_sensor_ops = {
	.get_temp  = thermal_sensor_get_temp,
};

static int thermal_sensor_resume(struct device *dev)
{
	struct thermal_sensor_device *tdev = dev_get_drvdata(dev);

	dev_info(dev, "enter %s\n", __func__);
	mutex_lock(&tdev->lock);
	thermal_sensor_hw_reset(tdev);
	mutex_unlock(&tdev->lock);
	dev_info(dev, "exit %s\n", __func__);
	return 0;
}

static const struct dev_pm_ops thermal_sensor_pm_ops = {
	.resume = thermal_sensor_resume,
};

static int thermal_sensor_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct device_node *np = dev->of_node;
	struct thermal_sensor_device *tdev;
	int ret = 0;
	struct resource res;
	const struct thermal_sensor_desc *desc;
	u32 num, i;

	desc = of_device_get_match_data(dev);
	if (!desc)
		return -EINVAL;

	tdev = devm_kzalloc(dev, sizeof(*tdev), GFP_KERNEL);
	if (!tdev)
		return -ENOMEM;

	ret = of_address_to_resource(np, 0, &res);
	if (ret)
		return ret;

	tdev->base = devm_ioremap(dev, res.start, resource_size(&res));
	if (!tdev->base)
		return -ENOMEM;

	if (of_find_property(np, "realtek,scpu-wrapper", NULL)) {
		tdev->regmap = syscon_regmap_lookup_by_phandle(np, "realtek,scpu-wrapper");
		if (IS_ERR(tdev->regmap))
			return PTR_ERR(tdev->regmap);
	}

	mutex_init(&tdev->lock);
	tdev->dev = dev;
	tdev->desc = desc;
	platform_set_drvdata(pdev, tdev);

	ret = thermal_sensor_hw_init(tdev);
	if (ret)
		return ret;

	if (of_property_read_u32(np, "realtek,sensor-instances", &num))
		num = 1;

	for (i = 0; i < num; i++) {
		struct thermal_zone_device *tz;

		tz = devm_thermal_zone_of_sensor_register(dev, i, tdev,
							  &thermal_sensor_ops);
		if (IS_ERR(tz)) {
			thermal_sensor_hw_exit(tdev);
			dev_err(dev, "failed to add thermal sensor: %d\n", ret);
			return PTR_ERR(tz);
		}
	}
	return 0;
}

static int thermal_sensor_remove(struct platform_device *pdev)
{
	struct thermal_sensor_device *tdev = platform_get_drvdata(pdev);

	thermal_sensor_hw_exit(tdev);
	platform_set_drvdata(pdev, NULL);
	return 0;
}

static const struct of_device_id thermal_sensor_of_match[] = {
	{ .compatible = "realtek,rtd129x-thermal-sensor", .data = &rtd129x_sensor_desc, },
	{ .compatible = "realtek,rtd139x-thermal-sensor", .data = &rtd139x_sensor_desc, },
	{ .compatible = "realtek,rtd161x-thermal-sensor", .data = &rtd1619_sensor_desc, },
	{ .compatible = "realtek,rtd131x-thermal-sensor", .data = &rtd1319_sensor_desc, },
	{ .compatible = "realtek,rtd1619b-thermal-sensor", .data = &rtd1619b_sensor_desc, },
	{ .compatible = "realtek,rtd1312c-thermal-sensor", .data = &rtd1312c_sensor_desc, },
	{ .compatible = "realtek,rtd1319d-thermal-sensor", .data = &rtd1319d_sensor_desc, },
	{ .compatible = "realtek,rtd1315e-thermal-sensor", .data = &rtd1315e_sensor_desc, },
	{ .compatible = "realtek,rtd1625-sc-wrap-thermal-sensor", .data = &rtd1625_sc_wrap_sensor_desc, },
	{ .compatible = "realtek,rtd1625-sys-thermal-sensor", .data = &rtd1625_sys_sensor_desc, },
	{}
};
MODULE_DEVICE_TABLE(of, thermal_sensor_of_match);

static struct platform_driver thermal_sensor_drv = {
	.driver = {
		.name           = "rtk-thermal-sensor",
		.owner          = THIS_MODULE,
		.of_match_table = of_match_ptr(thermal_sensor_of_match),
		.pm             = &thermal_sensor_pm_ops,
	},
	.probe    = thermal_sensor_probe,
	.remove   = thermal_sensor_remove,
};
module_platform_driver(thermal_sensor_drv);

MODULE_DESCRIPTION("Realtek Thermal Sensor Driver");
MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("Cheng-Yu Lee <cylee12@realtek.com>");
