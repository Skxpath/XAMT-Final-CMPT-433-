SRC_C = main.c fileSys.c infoUpdater.c i2c_drv.c accel_drv.c infraRed.c udpListener.c sleeping.c

PUBDIR = $(HOME)/cmpt433/public/myApps
OUTDIR = $(PUBDIR)
TARGET= elevator
CROSS_TOOL = arm-linux-gnueabihf-
CC_CPP = $(CROSS_TOOL)g++
CC_C = $(CROSS_TOOL)gcc

CFLAGS = -Wall -std=c99 -D _POSIX_C_SOURCE=200809L -Werror

all:
	$(CC_C) $(CFLAGS) $(SRC_C) -o $(OUTDIR)/$(TARGET) -lpthread -lm

clean:
	rm -f $(OUTDIR)/$(TARGET)
