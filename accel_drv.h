// Accelerometer module for reading accelerations from the Zen cape.
#ifndef _ACCEL_DRV_H_
#define _ACCEL_DRV_H_

#include <stdbool.h>

typedef struct{
  double x;
  double y;
  double z;
  double accel_total;
  double angle;
  _Bool accelerating;
}accel_output;

void AccelDrv_init();
void AccelDrv_cleanup();

accel_output* AccelDrv_getReading(void);

#endif
