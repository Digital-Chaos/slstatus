/* See LICENSE file for copyright and license details. */
#include <stdio.h>
#include <sys/sysctl.h>
#include "../util.h"

#define SYSCTL_BATTERY_LIFE     "hw.acpi.battery.life"
#define SYSCTL_BATTERY_STATE    "hw.acpi.battery.state"
#define SYSCTL_BATTERY_TIME     "hw.acpi.battery.time"

#define DISCHARGING_SYMBOL      "-"
#define CHARGING_SYMBOL         "+"
#define CHARGED_SYMBOL          "="

static int sysctl_fetch(const char *name);

const char *
battery_perc(void)
{
	int battery_life = sysctl_fetch(SYSCTL_BATTERY_LIFE);
	return (battery_life > -1) ? (bprintf("%d", battery_life)) : (NULL);
}

const char *
battery_power(void)
{
	return NULL;
}

const char *
battery_state(void)
{
	int battery_state = sysctl_fetch(SYSCTL_BATTERY_STATE);
	return  (battery_state == 1) ? (DISCHARGING_SYMBOL) :
	       ((battery_state == 2) ? (CHARGING_SYMBOL) :
	       (CHARGED_SYMBOL));
}

const char *
battery_time(void)
{
	int battery_time = sysctl_fetch(SYSCTL_BATTERY_TIME);
	return (battery_time > -1) ? (bprintf("%dh:%dm", battery_time / 60, battery_time % 60)) : (NULL);
}

static int
sysctl_fetch(const char *name)
{
	int value;
	size_t value_size = sizeof(value);
	return (sysctlbyname(name, &value, &value_size, NULL, 0) == 0) ? (value) : (-1);
}
