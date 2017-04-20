#include "sensor_input.h"
#include "cpu.h"

//  use "Prototype" interface board

void sensor_input_init(void)
{
	static GPIO_InitTypeDef  GPIO_InitStruct;
// for sensor

	//trigger, output, #0:PE10	#1:PA12	#2:PA11
	GPIO_InitStruct.Pin = GPIO_PIN_10 | GPIO_PIN_12 | GPIO_PIN_11;
	GPIO_InitStruct.Mode = GPIO_Mode_OUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOE_CLK_ENABLE();
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	//echo, input, #0:PE11	#1:PF6	#2:PA13
	GPIO_InitStruct.Pin = GPIO_PIN_11 | GPIO_PIN_6 | GPIO_PIN_13;
	GPIO_InitStruct.Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOE_CLK_ENABLE();
	__HAL_RCC_GPIOF_CLK_ENABLE();
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);
}


static Read_Distance(sensor sensor_id)
{
	/*
	Left: Sensor #2
	Front: Sensor #1
	Right: Sensor #0
	*/
	float disTime = 0;
	float flag = 0;
	float distance =0;
	if (sensor_id == SENSOR_RIGHT)
	{
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_10, GPIO_PIN_RESET);
		cpu_sw_delay(2);
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_10, GPIO_PIN_SET);
		cpu_sw_delay(10);
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_10, GPIO_PIN_RESET);
		while(flag == 0)
		{ 
			while(GPIO_ReadInputDataBit(GPIOE, GPIO_PIN_11) == SET)
			{
				disTime++;
			}
		flag = 1;
		}
		distance = (disTime * 340)/2.0;
		printf("SENSOR_RIGHT-The distance is : %f cm", distance);
	 	return distance;

	}


	if (sensor_id == SENSOR_LEFT)
	{
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_RESET);
		cpu_sw_delay(2);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_SET);
		cpu_sw_delay(10);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_RESET);
		while(flag == 0)
		{ 
			while(GPIO_ReadInputDataBit(GPIOA, GPIO_PIN_13) == SET)
			{
				disTime++;
			}
		flag = 1;
		}
		distance = (disTime * 340)/2.0;
		printf("SENSOR_LEFT-The distance is : %f cm", distance);
	 	return distance;

	}

	if (sensor_id == SENSOR_FRONT)
	{
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_RESET);
		cpu_sw_delay(2);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_SET);
		cpu_sw_delay(10);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_RESET);
		while(flag == 0)
		{ 
			while(GPIO_ReadInputDataBit(GPIOF, GPIO_PIN_6) == SET)
			{
				disTime++;
			}
		flag = 1;
		}
		distance = (disTime * 340)/2.0;
		printf("SENSOR_FRONT-The distance is : %f cm", distance);
	 	return distance;

	}




	void sensor_stop()
	{
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_10, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_11, GPIO_PIN_RESET);

		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOF, GPIO_PIN_6, GPIO_PIN_RESET);

		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_13, GPIO_PIN_RESET);

	}