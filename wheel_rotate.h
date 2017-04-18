#ifndef WHEEL_ROTATE_H
#define WHEEL_ROTATE_H

#include "hal_common_includes.h"

typedef enum {
	WHEEL_RIGHT,
	WHEEL_LEFT
} wheel;

void wheel_rotate_init(void);
void wheel_forward();
void wheel_back();
void wheel_turn_right();
void wheel_turn_left();
void wheel_stop();
void wheel_turnaround();
#endif

