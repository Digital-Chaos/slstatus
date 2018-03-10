# slstatus version
VERSION = 0

# customize below to fit your system

# components
COM =\
	components/datetime\
	components/battery\
	components/ram\
	components/cpu\
#	components/disk\
#	components/entropy\
#	components/hostname\
#	components/ip\
#	components/kernel_release\
#	components/keyboard_indicators\
#	components/load_avg\
#	components/num_files\
#	components/run_command\
#	components/swap\
#	components/temperature\
#	components/uptime\
#	components/user\
#	components/volume\
#	components/wifi

# paths
PREFIX = /usr/local
MANPREFIX = ${PREFIX}/man

X11INC = /usr/local/include
X11LIB = /usr/local/lib

# flags
CPPFLAGS = -I$(X11INC) -D_DEFAULT_SOURCE
CFLAGS   = -std=c99 -pedantic -Wall -Wextra -Os
LDFLAGS  = -L$(X11LIB) -s
LDLIBS   = -lX11

# compiler and linker
CC = cc
