# Makefile for building embedded application.
# by Brian Fraser

# Edit this file to compile extra C files into their own programs.

# Note to Matt: Do not include accel_test.c in the new Makefile.
# That one is only there for testing purposes.
TARGET= accelreader
SOURCES= accel_test.c accel_drv.c i2c_drv.c


PUBDIR = $(HOME)/cmpt433/public/myApps
OUTDIR = $(PUBDIR)
CROSS_TOOL = arm-linux-gnueabihf-
CC_CPP = $(CROSS_TOOL)g++
CC_C = $(CROSS_TOOL)gcc

CFLAGS = -Wall -g -std=c99 -D _POSIX_C_SOURCE=200809L -Werror


# Asound Library
# - See the AudioGuide for steps to copy library from target to host.
LFLAGS = -L$(HOME)/cmpt433/public/asound_lib_BBB

# -pg for supporting gprof profiling.
#CFLAGS += -pg



all:
	$(CC_C) $(CFLAGS) $(SOURCES) -o $(OUTDIR)/$(TARGET)  $(LFLAGS) -lpthread -lm


clean:
	rm -f $(OUTDIR)/$(TARGET)

# Copy the sound files, and the nodeJS server to the public directory.
