// SPDX-License-Identifier: GPL-2.0-only
/*
 * Realtek Thermal IPA Manual Reset Module
 *
 * The power allocator (IPA) thermal governor accumulates the temperature
 * error in err_integral. Writing 1 to the trigger_reset parameter clears
 * the accumulated error of the CPU thermal zone.
 *
 * Usage:
 *   echo 1 > /sys/module/rtk_ipa_reset/parameters/trigger_reset
 *
 * The parameter is write-only; reading it is not supported.
 *
 * Reconstructed from the RTD1325 device kernel; no source was released.
 *
 * Author: Hao-Wen Ting <haowen.ting@realtek.com>
 */

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/err.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/mutex.h>
#include <linux/thermal.h>

#define TARGET_TZ_NAME	"cpu-thermal"

/*
 * Private data of the power allocator governor, see
 * drivers/thermal/gov_power_allocator.c. Must be kept in sync.
 */
struct power_allocator_params {
	bool allocated_tzp;
	s64 err_integral;
	s32 prev_err;
	int trip_switch_on;
	int trip_max_desired_temperature;
	u32 sustainable_power;
};

static bool trigger_reset;

static int reset_ipa_integral_store(const char *val,
				    const struct kernel_param *kp)
{
	struct thermal_zone_device *tz;
	struct power_allocator_params *params;
	bool trigger = false;

	if (kstrtobool(val, &trigger))
		return -EINVAL;

	if (!trigger)
		return -EINVAL;

	tz = thermal_zone_get_zone_by_name(TARGET_TZ_NAME);
	if (IS_ERR_OR_NULL(tz)) {
		pr_warn("Target thermal zone '%s' not found.\n",
			TARGET_TZ_NAME);
		return -ENODEV;
	}

	mutex_lock(&tz->lock);

	params = tz->governor_data;
	if (!params) {
		pr_warn("Governor data is null, cannot reset.\n");
		goto unlock;
	}

	params->err_integral = 0;
	params->prev_err = 0;

unlock:
	mutex_unlock(&tz->lock);

	return 0;
}

static const struct kernel_param_ops reset_ops = {
	.set = reset_ipa_integral_store,
};

static int __init rtk_ipa_reset_init(void)
{
	return 0;
}

static void __exit rtk_ipa_reset_exit(void)
{
}

module_param_cb(trigger_reset, &reset_ops, &trigger_reset, 0200);
MODULE_PARM_DESC(trigger_reset, "Write 1 to safely reset IPA err_integral");

module_init(rtk_ipa_reset_init);
module_exit(rtk_ipa_reset_exit);

MODULE_AUTHOR("Hao-Wen Ting <haowen.ting@realtek.com>");
MODULE_DESCRIPTION("Realtek Thermal IPA Manual Reset Module");
MODULE_LICENSE("GPL v2");
