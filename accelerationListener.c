//NOTE: main() is only for testing - delete when implementing into a bigger project

#include "fileSys.h"
#include "accelerationListener.h"

#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <stdint.h>
#include <pthread.h>

#define I2CDRV_LINUX_BUS "/dev/i2c-1"
#define I2C_DEVICE 0x1C
#define CTRL_REG 0x2A
#define STATUS_ADDRESS 0x00

#define SLOTS "/sys/devices/platform/bone_capemgr/slots"
#define CAPE_MESSAGE "BB-I2C1"

#define THRESHOLD 200
#define OFFSET 1000
#define NANOSECONDS_DELAY 400000000
#define SECONDS_DELAY 0

static _Bool run;
static int fileDesc;

void listen();
int16_t absolute(int16_t val);
void detectMovement(int16_t x, int16_t y, int16_t z);
static void writeReg(unsigned char addr, unsigned char value);
static void readReg(unsigned char addr, unsigned char *value);
static int setupI2C(char* bus, int addr);

static pthread_t tid;
static void* background_thread(void* args);

int main()
{
	AccelerationListener_init();
	printf("finishing\n");
	long long sec = 2;
	struct timespec thirtySeconds = {sec, 0};
	nanosleep(&thirtySeconds, (struct timespec *) NULL);
	AccelerationListener_cleanup();
	return 0;
}

void AccelerationListener_init(void)
{
	run = 1;
	fileDesc = setupI2C(I2CDRV_LINUX_BUS, I2C_DEVICE);
	writeReg(CTRL_REG,1);
	pthread_create(&tid, NULL, background_thread, NULL);
	listen();
	pthread_join(tid, NULL);
}

static int setupI2C(char* bus, int addr)
{
	int i2cFile_setup = open(bus, O_RDWR);
	if (i2cFile_setup < 0)
	{
		printf("Cannot open bus (%s)\n", bus);
		printf("Trying to initialize cape\n");
		echoToFile(SLOTS, CAPE_MESSAGE);
		i2cFile_setup = open(bus, O_RDWR);
		if (i2cFile_setup < 0)
		{
			printf("Error loading I2C. Please address the issue\n");
			exit(-1);
		}
		printf("Cape loaded successfully\n");
	}
	int result = ioctl(i2cFile_setup, I2C_SLAVE, addr);
	if (result < 0)
	{
		printf("Cannot setup device as slave\n");
		exit(1);
	}
	return i2cFile_setup;
}

static void writeReg(unsigned char addr, unsigned char value)
{
	unsigned char in[2];
	in[0] = addr;
	in[1] = value;
	if (write(fileDesc, in, 2) != 2) {
		printf("Cannot write to register\n");
		exit(-1);
	}
}

static void* background_thread(void* args)
{
	long long sec = 10;
	struct timespec thirtySeconds = {sec, 0};
	nanosleep(&thirtySeconds, (struct timespec *) NULL);
	printf("time's up!\n");
	run = 0;
	return NULL;
}


void AccelerationListener_cleanup(void)
{
	close(fileDesc);
	printf("cleanup\n");
}

void listen()
{
	long long sleepSeconds = SECONDS_DELAY;
	long long sleepNanoSeconds = NANOSECONDS_DELAY;
	struct timespec delay = {sleepSeconds, sleepNanoSeconds};
	while(run)
	{
		unsigned char value[16];
		readReg(STATUS_ADDRESS, value);

		int16_t x = (value[1] << 8) | value[2];
		x = (x >> 4);
		int16_t y = (value[3] << 8) | value[4];
		y = (y >> 4);
		int16_t z = (value[5] << 8) | value[6];
		z = (z >> 4);

		x = absolute(x);
		y = absolute(y);
		z = (z - OFFSET);
		z = absolute(z);

		if(x > THRESHOLD || y > THRESHOLD || z > THRESHOLD){
			detectMovement(x, y, z);
		}

		nanosleep(&delay, (struct timespec *) NULL);
	}
}

static void readReg(unsigned char addr, unsigned char *value)
{
	if(write(fileDesc, &addr, sizeof(addr)) != sizeof(addr))
	{
		printf("Cannot write to reg\n");
	}
	if (read(fileDesc, value, 7*sizeof(value)) != 7*sizeof(value))
	{
		printf("Cannot read from reg\n");
	}
}


int16_t absolute(int16_t val)
{
	if (val < 0) {
		return val * -1;
	}
	return val;
}

void detectMovement(int16_t x, int16_t y, int16_t z)
{
	if (x > y && x > z)
	{
		printf("Moving on X\n");
	}
	else if (y > x && y > z)
	{
		printf("Moving on Y\n");
	}
	else
	{
		printf("Moving on Z\n");
	}
}
