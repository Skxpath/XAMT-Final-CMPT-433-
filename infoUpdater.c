// Note to Matt: Do NOT include this file in project Makefile
// but do read over it to see how to use functions

#include "infoUpdater.h"
#include "accel_drv.h"
#include "infraRed.h"
#include "sleeping.h"

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define UPDATE_DELAY 20

// Thread control variables
static _Bool stopFlag = false;
static pthread_t update_tid;

// Variables for storing values read from getReading function
static double x;
static double y;
static double z;
static double total;
static double angle;
static _Bool accelerating;

// Keep track of last 10 iterations of distance data obtained
// and grab an average from said data
static int IR_readings[10] = {0};
static int IR_index = 0;

// Variable for storing distance data read
static double distance;

// Pointer to store output of getReading function
static accel_output* reading;


// Thread for reader
static void* update_thread(void* args)
{
  // Record first 10 iterations of sensor reading data
  for (int i = 0; i < 10; i++){
    IR_readings[i] = IRSensor_getVoltage0Reading();
  }

  // Loop while stop flag is not set
  while(!stopFlag){
    // Record IR distance sensor reading
    IR_readings[IR_index] = IRSensor_getVoltage0Reading();
    IR_index = (IR_index + 1) % 10;

    // Record distance data collected
    distance = IRSensor_getDistance();

    // getReading returns an accel_output type pointer
    reading = AccelDrv_getReading();

    // Store contents of reading into static global variables
    x = reading -> x;
    y = reading -> y;
    z = reading -> z;
    total = reading -> accel_total;
    angle = reading -> angle;
    accelerating = reading -> accelerating;

    // Free the reading pointer since malloc() was called in getReading
    free(reading);

    // Wait for update_delay ms
    sleep_msec(UPDATE_DELAY);
  }

  // Terminate thread when stop flag is set
  pthread_exit(0);
}

// Initialization of info updater
void Updater_init(void)
{
  // Call initialization of AccelDrv functionalities.
  AccelDrv_init();
  IRSensor_init();
  sleep_msec(100);
  // Create thread for looping check of info
  // Note that after creating the thread, you may need to wait for a short
  // amount of time (a hundred ms or so?) before any data is accessible
  pthread_create(&update_tid, NULL, update_thread, NULL);
}

// Cleaup function for updater
// Should never be called in practical application, but here for testing purposes
void Updater_cleanup(void)
{
  // Set stop flag
  stopFlag = true;

  // Wait for reading thread to finish
  pthread_join(update_tid, NULL);

  // Call cleanup function after code is done
  AccelDrv_cleanup();
}

// Returns X-axis acceleration
double Updater_getAccelX(void)
{
  return x;
}

// Returns Y-axis acceleration
double Updater_getAccelY(void)
{
  return y;
}

// Returns Z-axis acceleration
double Updater_getAccelZ(void)
{
  return z;
}

// Returns total acceleration
double Updater_getAccelTotal(void){
  return total;
}

// Returns angle of tilt in degrees
double Updater_getAngle(void)
{
  return angle;
}

// Returns whether or not the system is accelerating
_Bool Updater_isAccelerating(void)
{
  return accelerating;
}

// Returns distance
double Updater_getDistanceRaw(void)
{
  return distance;
}

// Second option for returning distance, which takes the average
// of 10 readings before converting to distance
double Updater_getDistanceStable(void)
{
  int totalReadings = 0;

  // Add up all readings recorded
  // Should technically be blocking,
  // but would probably be too slow performance wise
  for(int i = 0; i < 10; i++){
    totalReadings += IR_readings[i];
  }

  // Take the average of the last 10 readings,
  // then convert it to distance
  double avgReading = ((double)totalReadings)/10;
  return IRSensor_readingToDistance(avgReading);
}
