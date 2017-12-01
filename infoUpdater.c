#include "infoUpdater.h"
#include "accel_drv.h"
#include "infraRed.h"
#include "sleeping.h"
#include "lighting.h"

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define UPDATE_DELAY 20


#define HIGHEST_FLOOR 2
#define LOWEST_FLOOR -1

#define FLOOR_DIST_THRESHOLD (2.5)


static int num_floors = HIGHEST_FLOOR - LOWEST_FLOOR;
static double *floor_distances;
static float *floor_mapping;

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

static _Bool lightsOn = false;
static _Bool lightsPreferredOn = true;

// Variable for storing distance data read
static double distance;

static float floor_number;

// Pointer to store output of getReading function
static accel_output* reading;


static float distToFloor(double dist)
{
  for(int i=0; i < num_floors; i++){
    if((i == 0) && (dist > (floor_distances[i] - FLOOR_DIST_THRESHOLD))){
      return (floor_mapping[i] - 0.5);
    }else if((dist <= (floor_distances[i] + FLOOR_DIST_THRESHOLD)) && (dist >= (floor_distances[i] - FLOOR_DIST_THRESHOLD))){
      return floor_mapping[i];
    }
    else{
      if(i == num_floors - 1){
        return (floor_mapping[i] + 0.5);
      }else if(dist > (floor_distances[i+1] + FLOOR_DIST_THRESHOLD)){
        if(floor_mapping[i] == -1){
          return 0.5;
        }else{
          return ((floor_mapping[i] + floor_mapping[i+1])/2);
        }
      }
    }
  }

  return 0;
}

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

    floor_number = distToFloor(distance);

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

    for(int attempts = 0; attempts <= 10; attempts++){
      // Check if lights are on according to GPIO files
      lightsOn = (get_Lighting_state() + 1) % 2;

      // If lights are off...
      if(lightsOn != lightsPreferredOn){
        // If already tried 10 consecutive times to turn lights back on without success
        // Inform user of problem
        if(attempts >=10){
          // lighting_error_handling_function(); (not implemented)
          printf("Lights not toggling properly\n");
        }else{
          // If haven't tried 10 times yet, try again
          if(lightsPreferredOn){
            turn_on_Lighting();
          }else{
            turn_off_Lighting();
          }
        }
      }else{
        // If lights already on, continue on with thread
        break;
      }
    }

    // Wait for update_delay ms
    sleep_msec(UPDATE_DELAY);
  }

  // Terminate thread when stop flag is set
  pthread_exit(0);
}

// Initialization of info updater
void Updater_init(void)
{
  if(HIGHEST_FLOOR * LOWEST_FLOOR > 0){
    num_floors++;
  }

  floor_distances = malloc((sizeof(double)) * num_floors);
  floor_mapping = malloc((sizeof(float)) * num_floors);

  // Hard code for floor info to use with demo
  for(int fl = 0; fl < num_floors ; fl++){
    floor_distances[(num_floors - 1 - fl)] = (double) (15 + (fl * 30));
    if((HIGHEST_FLOOR < 0) || ((HIGHEST_FLOOR - fl) > 0)){
      floor_mapping[(num_floors - 1 - fl)] = HIGHEST_FLOOR - fl;
    }else{
      floor_mapping[(num_floors - 1 - fl)] = HIGHEST_FLOOR - fl - 1;
    }
  }

  // Call initialization of AccelDrv functionalities.
  AccelDrv_init();
  IRSensor_init();
  sleep_msec(250);
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

  free(floor_distances);
  free(floor_mapping);
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

_Bool Updater_getLightsState(void)
{
  return lightsOn;
}

void Updater_toggleLighting(_Bool turnOn)
{
  lightsPreferredOn = turnOn;
}

float Updater_getCurrentFloor(void)
{
  return floor_number;
}
