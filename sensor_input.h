#ifndef SENSOR_INPUT_H
#define SENSOR_INPUT_H

#include "hal_common_includes.h"

typedef enum {
	SENSOR_RIGHT;
	SENSOR_LEFT;
	SENSOR_MID;
} sensor;

/*
Left: Sensor #2
Front: Sensor #1
Right: Sensor #0
*/

void sensor_input_init(void);
static sensor_right_input();
static sensor_left_input();
static sensor_mid_input();
void sensor_stop();

#endif