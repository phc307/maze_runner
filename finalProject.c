/*
	This is the main user software application
	In this case, we have below the code necessary to toggle a set of LEDs on a	STM32 F3 Discovery PCB at 20 Hz.

	(c) Abhimanyu Ghosh, 2017
 */

#include "cpu.h"
#include "wheel_rotate.h"
#include "sensor_input.h"
#include "foo.h"

int main() // always turns right
{
  int last_direction = 0; // 1 = forward, 2 = backward, 3 = right, 4 = left, 99 = turn around
  cpu_init();
  wheel_rotate_init();
  GPIO_InitTypeDef GPIO_InitStructure;
  int i = 0;

  /*
    In an infinite loop, keep toggling the LEDs in an alternating pattern:
   */
  while(1)
  {
    /*
      Carry out a simple unit test of foo() declared in foo.h:
     */
    if(TEST_FOO(i, i+1) < 0)
    {
      /*
        If the above fails there is either a hardware, code or other undefined error.
        Now we're in an undefined state regarding processor core behavior...
       */
      while(1); // We probably have had a radiation hit or a major malfunction on the ALU of the processor...
    }
    else
    {
      //go forward
      if (!sensor_right_input() && !sensor_left_input() && !sensor_mid_input() \
          || sensor_right_input() && !nsor_left_input() && !sensor_mid_input() \
          || sensor_right_input() && !nsor_left_input() && !sensor_mid_input() \
          || sensor_right_input() && sensor_left_input() && !sensor_mid_input())
      {
        wheel_forward();
        cpu_sw_delay(500U);
        last_direction = 1;
      }

      //turnaround
      else if (sensor_right_input() && sensor_left_input() && sensor_mid_input())
      {
        wheel_turnaround();
        cpu_sw_delay(500U);
        last_direction = 99;
      }
      // turn right
      else if (!sensor_right_input() && sensor_left_input() && sensor_mid_input() \
              || !sensor_right_input() && !nsor_left_input() && sensor_mid_input())
      {
        wheel_turn_right();
        cpu_sw_delay(500U);
        last_direction = 3;
      }
      // turn left
      else if (sensor_right_input() && !sensor_left_input() && sensor_mid_input())
      {
        wheel_turn_left();
        cpu_sw_delay(500U);
        last_direction = 4;
      } 
      


      ++i; // Increment i for the next test iteration...
    }
  }

  return 0;
}
