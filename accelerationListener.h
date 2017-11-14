#ifndef ACCELERATION_LISTENER_H
#define ACCELERATION_LISTENER_H

typedef struct{
  int x;
  int y;
  int z;
}accel_output;

void AccelerationListener_init(void);
void AccelerationListener_cleanup(void);

accel_output* AccelerationListener_getAccelOutput(void);
double AccelerationListener_getAngle(void);

#endif
