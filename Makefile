SRC_C = fileSys.c accelerationListener.c

PUBDIR = $(HOME)/cmpt433/public/myApps
OUTDIR = $(PUBDIR)
CROSS_TOOL = arm-linux-gnueabihf-
CC_CPP = $(CROSS_TOOL)g++
CC_C = $(CROSS_TOOL)gcc

CFLAGS = -Wall -std=c99 -D _POSIX_C_SOURCE=200809L -Werror

all:
	$(CC_C) $(CFLAGS) $(SRC_C) -o $(OUTDIR)/elevator -lpthread -lm
