// Demo application to read analog input voltage 0 on the BeagleBone
// Assumes ADC cape already loaded:
// root@beaglebone:/# echo BB-ADC > /sys/devices/platform/bone_capemgr/slots
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>
#define A2D_FILE_VOLTAGE1 "/sys/bus/iio/devices/iio:device0/in_voltage1_raw"
#define A2D_VOLTAGE_REF_V 3.15
#define A2D_MAX_READING 4095
#define REGULATOR 0

int getVoltage0Reading()
{
	// Open file
	FILE *f = fopen(A2D_FILE_VOLTAGE1, "r");
	if (!f) {
		printf("ERROR: Unable to open voltage input file. Cape loaded?\n");
		printf("try: echo BB-ADC > /sys/devices/platform/bone_capemgr/slots\n");
		exit(-1);
	}

	// Get reading
	int a2dReading = 0;
	int itemsRead = fscanf(f, "%d", &a2dReading);
	if (itemsRead <= 0) {
		printf("ERROR: Unable to read values from voltage input file.\n");
		exit(-1);
	}	

	// Close file
	fclose(f);
	return a2dReading;
}

static void busyWait(long nanoseconds){
	long seconds = 0;
	struct timespec reqDelay = {seconds, nanoseconds};
	nanosleep(&reqDelay, (struct timespec *) NULL);
}

static double reading_to_distance(double reading){
	double voltage = (reading / A2D_MAX_READING) * A2D_VOLTAGE_REF_V;
	printf(" %5.2fv =====>",voltage);
	double distance = 0;
	if (voltage > 3.15 || voltage <= 0.44){
		//error
		printf("out of range\n");
	}
	else if (voltage <= 3.15 && voltage > 2.25){
		//piece wise 1
		//voltage = distance * (-0.2) + 4.4;
		distance = (4.4-voltage)/0.2;
	}
	else if (voltage <= 2.25 && voltage > 1.75){
		//piece wise 2
		// voltage = distance * (-0.1) + 3.4;
		distance = (3.4-voltage)/0.1;
	}
	else if (voltage <= 1.75 && voltage > 1.3){
		//piece wise 3	
		//voltage = distance * (-0.1) + 3.2;
		distance = (3.2-voltage)/0.1;
	}
	else if (voltage <= 1.3 && voltage > 1.08){
		//piece wise 4
		//voltage = distance * (-0.04) + 2.2;
		distance = (2.2-voltage)/0.04;
	}
	else if (voltage <= 1.08 && voltage > 0.92){
		//piece wise 5
		//voltage = distance * (-0.03) + 1.9;
		distance = (1.9-voltage)/0.03;
	}
	else if (voltage <= 0.92 && voltage > 0.73){
		//piece wise 6
		//voltage = distance * (-0.02) + 1.5;
		distance = (1.5-voltage)/0.02;
	}
	else if (voltage <= 0.73 && voltage > 0.6){
		//piece wise 7
		//voltage = distance * (-0.01) + 1.3;
		distance = (1.3-voltage)/0.01;
	}
	else if (voltage <= 0.6 && voltage > 0.44){
		//piece wise 8 + 9
		//distance = (1-voltage)/0.01;
		//error
		printf("out of range\n");
	}
	return distance - REGULATOR;
}

int main()
{
	while (true) {
		int reading = getVoltage0Reading();
		double distance;
		distance = reading_to_distance((double)reading);
		printf("Value %5d ==> %5.2fcm\n", reading, distance);
		busyWait(300000000);
	}
	return 0;
}
