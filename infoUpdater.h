// I2C driver module
// Create a i2c_device_t struct for the device you want to access,
// Then use the functions to initialize it, use it, and cleanup.
#ifndef INFO_UPDATER_H
#define INFO_UPDATER_H

#include <stdbool.h>

void Updater_init(void);
void Updater_cleanup(void);

// Get functions for X, Y, and Z axis acceleration
double Updater_getAccelX(void);
double Updater_getAccelY(void);
double Updater_getAccelZ(void);

// Get function for total acceleration
double Updater_getAccelTotal(void);

// Get function for tilt angle
// Note that this may be unreliable when elevator is accelerating
double Updater_getAngle(void);

// Get whether or not the system is accelerating
_Bool Updater_isAccelerating(void);

// Get function for distance
// Two options for raw reading (fast)
// or stable reading (may be slightly inaccurate due to delay)
double Updater_getDistanceRaw(void);
double Updater_getDistanceStable(void);

// Get current state of lights
_Bool Updater_getLightsState(void);

// Function to toggle lights on/off
void Updater_toggleLighting(_Bool turnOn);

// Get current floor number of elevator
float Updater_getCurrentFloor(void);

#endif
