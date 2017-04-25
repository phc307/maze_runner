// main for test

#include "sensor_input.h"
#include "cpu.h"
#include "board_led.h"
#include "foo.h"



int main()
{

	cpu_init();
	board_led_init();
	sensor_input_init();
 	int i = 0;
 	while(1)
	{
    	if(TEST_FOO(i, i+1) < 0)
    	{
      		while(1); // We probably have had a radiation hit or a major malfunction on the ALU of the processor...
    	}
    	else
    	{
    		//Right sensor : red
    		if(Read_Distance(SENSOR_RIGHT)>=5)
    		{
      			board_led_on(LED1);
      			board_led_off(LED2);
      			board_led_off(LED3);
      			cpu_sw_delay(100U);
    		}
    		if(Read_Distance(SENSOR_RIGHT)<5)
    		{
    			board_led_on(LED1);
      			board_led_off(LED2);
      			board_led_off(LED3);
      			cpu_sw_delay(20U);
    		}
    		// Left sensor: orange
    		if(Read_Distance(SENSOR_LEFT)>=5)
    		{
      			board_led_off(LED1);
      			board_led_on(LED2);
      			board_led_off(LED3);
      			cpu_sw_delay(100U);
    		}
    		if(Read_Distance(SENSOR_LEFT)<5)
    		{
    			board_led_off(LED1);
      			board_led_on(LED2);
      			board_led_off(LED3);
      			cpu_sw_delay(20U);
    		}

    		// Front sensor : green
    		if(Read_Distance(SENSOR_FRONT)>=5)
    		{
      			board_led_off(LED1);
      			board_led_off(LED2);
      			board_led_on(LED3);
      			cpu_sw_delay(100U);
    		}
    		if(Read_Distance(SENSOR_FRONT)<5)
    		{
    			board_led_off(LED1);
      			board_led_off(LED2);
      			board_led_on(LED3);
      			cpu_sw_delay(20U);
    		}


		    ++i; // Increment i for the next test iteration...
    	}
  	}

  	return 0;
}