// Note to Matt: Do NOT include this file in project Makefile
// but do read over it to see how to use functions

#include "accel_drv.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#include <unistd.h>

// Thread control variables
static _Bool stopFlag = false;
static pthread_t tid;

// Variables for storing values read from getReading function
static double x;
static double y;
static double z;
static double total;
static double angle;
static _Bool accelerating;

// Pointer to store output of getReading function
static accel_output* reading;


// Thread for reader
void* accel_reader_thread(void* args)
{
  printf("Thread initialzing\n");

  // Loop while stop flag is not set
  while(!stopFlag){

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
  }

  // Terminate thread when stop flag is set
  printf("Thread terminating\n");
  pthread_exit(0);
}

int main()
{

  // For testing purposes; Don't need to include i
  int i = 1;

  // Call initialization of AccelDrv functionalities.
  AccelDrv_init();

  // Create thread for looping check of info
  // Note that after creating the thread, you may need to wait for a short
  // amount of time (a hundred ms or so?) before any data is accessible
  pthread_create(&tid, NULL, accel_reader_thread, NULL);

  // Below is code to output readings to console, don't need to include in server
  // Function loops for 60 seconds before setting stop flag and terminating
  printf("Beginning output of accelerometer readings\n");
  while(i <= 60){
    sleep(1);
    printf("t= %d:", i);
    printf("x = %lf, ", x);
    printf("y = %lf, ", y);
    printf("z = %lf, ", z);
    printf("total = %lf, ", total);
    printf("angle = %lf, ", angle);
    if(accelerating){
      printf("accelerating = true\n");
    }else{
      printf("accelerating = false\n");
    }
    i++;
  }

  // Set stop flag (Note that this shouldn't happen in our server)
  stopFlag = true;

  // Wait for reading thread to finish
  pthread_join(tid, NULL);

  // Call cleanup function after code is done
  AccelDrv_cleanup();
  return 0;
}
