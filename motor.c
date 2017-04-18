#include "motor.h"

void motor_forward(motor motor_id);
void motor_backwardward(motor motor_id);
void motor_stop(motor motor_id);


void motor_init(void)
{
	static GPIO_InitTypeDef  GPIO_InitStruct;

// GPIO_PIN_6 | GPIO_PIN_7 : right motor 1
// GPIO_PIN_3 | GPIO_PIN_4 : left motor 2
	GPIO_InitStruct.Pin = GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_3 | GPIO_PIN_4;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	__HAL_RCC_GPIOE_CLK_ENABLE();

	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
}

void motor_forward(motor motor_id)
{
	switch(motor_id)
	{
		case motor_right:
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_6, Low);
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, High);
			break;
		case motor_left:
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3, Low);
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_4, High);
			break;
	}
}

void motor_backward(motor motor_id)
{
	switch(motor_id)
	{
		case motor_right:
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_6, High);
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, Low);
			break;
		case motor_left:
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3, High);
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_4, Low);
			break;
	}
}

void motor_stop(motor motor_id)
{
	switch(motor_id)
	{
		case motor_right:
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_6, Low);
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, Low);
			break;
		case motor_left:
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3, Low);
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_4, Low);
			break;
	}
}



