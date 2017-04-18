#include "wheel_rotate.h"

void wheel_rotate_init(void)
{
	static GPIO_InitTypeDef  GPIO_InitStruct;

// right wheel : PA2, left wheel : PA3
	GPIO_InitStruct.Pin = GPIO_PIN_2 | GPIO_PIN_3;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	__HAL_RCC_GPIOE_CLK_ENABLE();

	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}



void wheel_forward()
{
	motor_forward(motor_right);
	motor_forward(motor_left);
}

void wheel_back()
{
	motor_backwardward(motor_right);
	motor_backwardward(motor_left);
}

void wheel_turn_right() // then we can use delay time to control the angle
{
	motor_stop(motor_right);
	motor_forward(motor_left);
}

void wheel_turn_left();// then we can use delay time to control the angle
{
	motor_stop(motor_left);
	motor_forward(motor_right);
}

void wheel_stop()
{
	motor_stop(motor_left);
	motor_stop(motor_right);
}

void wheel_turnaround() // how to turn reverse? (not finished)
{
	motor_forward(motor_right);
	motor_backwardward(motor_left);

}