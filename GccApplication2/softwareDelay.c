/*
 * softwareDelay.c
 *
 * Created: 1/17/2020 6:46:54 PM
 *  Author: Az
 */ 
#include "softwareDelay.h"
#include "std_types.h"

/**
 * Description: this delay function operate in a polling manner
 * 				don't use it with RTOSs
 * @param a_delay_in_ms: the milli-seconds
 */
void softwareDelayMs(uint32_t u32_delay_in_ms)
{
	while(u32_delay_in_ms > 0)
	{
		volatile uint16_t i = 360;
		while(i>0)
		{
			i--;
		}
		u32_delay_in_ms--;		
	}
}
