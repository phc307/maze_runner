#ifndef MOTOR_H
#define MOTOR_H		1

#include "hal_common_includes.h"

typedef enum {
	motor_right;
	motor_left
} motor;

void motor_init(void);
void motor_forward(motor motor_id);
void motor_backwardward(motor motor_id);
void motor_stop(motor motor_id);

#endif