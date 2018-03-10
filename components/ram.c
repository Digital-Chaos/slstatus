/* See LICENSE file for copyright and license details. */
#include <stdio.h>
#include <sys/sysctl.h>
#include <unistd.h>
#include "../util.h"

#define DISPLAY_FORMAT              "%2.1fGB"
#define GIGABYTE                    (1024 * 1024 * 1024)

#define SYSCTL_VM_PAGE_COUNT        "vm.stats.vm.v_page_count"
#define SYSCTL_VM_FREE_COUNT        "vm.stats.vm.v_free_count"
#define SYSCTL_VM_INACTIVE_COUNT    "vm.stats.vm.v_inactive_count"


static const char *bytes_to_string(long bytes);
static long vm_stats(const char *name);
static long vm_available_bytes();
static long vm_total_bytes();
static long vm_used_bytes();


const char *
ram_free(void)
{
	return bytes_to_string(vm_available_bytes());
}

const char *
ram_perc(void)
{
	int percent_used = ((100 * vm_used_bytes()) / vm_total_bytes());
	return bprintf("%d",  percent_used);
}

const char *
ram_total(void)
{
	return bytes_to_string(vm_total_bytes());
}

const char *
ram_used(void)
{
	return bytes_to_string(vm_used_bytes());
}


static const char *
bytes_to_string(long bytes)
{
	return (bytes > -1) ? (bprintf(DISPLAY_FORMAT, (float) bytes / GIGABYTE)) : (NULL);
}

static long
vm_available_bytes()
{
	long available_bytes = -1;
	long free_bytes = vm_stats(SYSCTL_VM_FREE_COUNT);
	if (free_bytes > -1) {
		long inactive_bytes = vm_stats(SYSCTL_VM_INACTIVE_COUNT);
		if (inactive_bytes > -1) {
			available_bytes = free_bytes + inactive_bytes;
		}
	}
	return available_bytes;
}

static long
vm_total_bytes()
{
	return vm_stats(SYSCTL_VM_PAGE_COUNT);
}

static long
vm_used_bytes()
{
	return (vm_total_bytes() - vm_available_bytes());
}

static long
vm_stats(const char *name)
{
	long bytes = -1;
	int page_count;
	size_t page_count_size = sizeof(page_count);
	if (sysctlbyname(name, &page_count, &page_count_size, NULL, 0) == 0) {
		bytes = (long) page_count * getpagesize();
	}
	return bytes;
}
