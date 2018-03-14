/* See LICENSE file for copyright and license details. */
#include <err.h>
#include <stdio.h>
#include <sys/mount.h>

#include "../util.h"

#define GIGABYTE    (1024 * 1024 * 1024)

/* Disk space currently available for non-superuser */
const char *
disk_avail(const char *path)
{
    long avail_bytes = -1;
    struct statfs stats;
    if (statfs(path, &stats) == 0) {
        avail_bytes = stats.f_bavail * stats.f_bsize;
    }
    return (avail_bytes > -1) ? (bprintf("%ldG", avail_bytes / GIGABYTE)) : (NULL);
}

/* Disk space currently free (includes space reserved for superuser) */
const char *
disk_free(const char *path)
{
    long free_bytes = -1; 
    struct statfs stats;
    if (statfs(path, &stats) == 0) {
        free_bytes = stats.f_bfree * stats.f_bsize;
    }
    return (free_bytes > -1) ? (bprintf("%ldG", free_bytes / GIGABYTE)) : (NULL);
}

/* Percentage of available (non reserved) disk space currently used */
const char *
disk_perc(const char *path)
{
    int percent_full = -1;
    struct statfs stats;
    if (statfs(path, &stats) == 0) {

        long reserved_blocks = stats.f_bfree - stats.f_bavail;
        long usable_blocks = stats.f_blocks - reserved_blocks;
        long used_blocks = stats.f_blocks - stats.f_bfree;

        percent_full = 100 * used_blocks / usable_blocks;
    }
    return (percent_full > -1) ? (bprintf("%d", percent_full)) : (NULL);
}

/* Total size of disk */
const char *
disk_total(const char *path)
{
    long total_bytes = -1;
    struct statfs stats;
    if (statfs(path, &stats) == 0) {
        total_bytes = stats.f_blocks * stats.f_bsize;
    }
    return (total_bytes > -1) ? (bprintf("%ldG", total_bytes / GIGABYTE)) : (NULL);
}

/* Amount of space on disk currently used */
const char *
disk_used(const char *path)
{
    long used_bytes = -1;
    struct statfs stats;
    if (statfs(path, &stats) == 0) {

        used_bytes = (stats.f_blocks - stats.f_bfree) * stats.f_bsize;
    }
    return (used_bytes > -1) ? (bprintf("%ldG", used_bytes / GIGABYTE)) : (NULL);
}
