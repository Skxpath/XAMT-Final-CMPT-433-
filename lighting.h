#ifndef LIGHTING_H
#define LIGHTING_H

#define GPIO_EXPORT_PATH "/sys/class/gpio/export"
#define GPIO_NUMBER 49
#define GPIO_VALUE_PATH "/sys/class/gpio/gpio49/value"

void turn_on_Lighting(void);
void turn_off_Lighting(void);
int get_Lighting_state(void);

#endif
