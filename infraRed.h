// I2C driver module
// Create a i2c_device_t struct for the device you want to access,
// Then use the functions to initialize it, use it, and cleanup.
#ifndef INFRARED_H
#define INFRARED_H

// Initialize IR Sensor
void IRSensor_init(void);

// Get raw voltage data
int IRSensor_getVoltage0Reading(void);

// Convert voltage data to distance
double IRSensor_readingToDistance(double reading);

// Get converted distance data
double IRSensor_getDistance(void);

#endif
