/* See LICENSE file for copyright and license details. */
#include <stdio.h>
#include <sys/sysctl.h>
#include <sys/dkstat.h>
#include "../util.h"


#define ZERO_CELSIUS         273.15
#define SYSCTL_CPU_FREQ     "dev.cpu.0.freq"
#define SYSCTL_CPU_TEMP     "dev.cpu.0.temperature"
#define SYSCTL_CPU_TIMES    "kern.cp_time"

const char *
cpu_freq(void)
{
    int freq;
    size_t freq_size = sizeof(freq);
    if (sysctlbyname(SYSCTL_CPU_FREQ, &freq, &freq_size, NULL, 0) == -1) {
        freq = -1;
    }
    return (freq > -1) ? (bprintf("%1.1fGHz", (float) freq / 1000)) : (NULL);
}

const char *
cpu_perc(void)
{
    int percent = -1;
    static long previous_total_time;
    static long previous_idle_time;

    long cpu_time[CPUSTATES];
    size_t cpu_time_size = sizeof(cpu_time);
    if (sysctlbyname(SYSCTL_CPU_TIMES, &cpu_time, &cpu_time_size, NULL, 0) == 0) {

        // current idle time
        long idle_time = cpu_time[CP_IDLE];

        // current total accumulated time for all cpu states
        long total_time = 0;
        for (int i = 0; i < CPUSTATES; i++) {
            total_time += cpu_time[i];
        }

        // calc cpu usage as total time minus idle time
        long delta_idle_time  = idle_time  - previous_idle_time;
        long delta_total_time = total_time - previous_total_time;
        percent = (100 * (delta_total_time - delta_idle_time)) / delta_total_time;

        // store current times as previous times for next call
        previous_total_time = total_time;
        previous_idle_time = idle_time;
    }

    return (percent > -1) ? (bprintf("%d", percent)) : (NULL);
}

const char *
cpu_temp(void)
{
    int temp;
    size_t temp_size = sizeof(temp);
    if (sysctlbyname(SYSCTL_CPU_TEMP, &temp, &temp_size, NULL, 0) == -1) {
        temp = -1;
    }
    return (temp > -1) ? (bprintf("%1.1fC", ((float) temp / 10) - ZERO_CELSIUS)) : (NULL);
}
