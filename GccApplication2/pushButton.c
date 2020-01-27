/*
 * pushButton.c
 *
 * Created: 1/17/2020 11:03:36 PM
 *  Author: Az
 */ 
#include "pushButton.h"
#include "gpio.h"


/**
 * Description: Initialize the BTN_x Pin state (where x 0, 1, 2, 3) to Input
 * @param btn_id: The btn to be initialized and it takes
 * 				  one of the enum (En_buttonId) parameters
 *
 */
void pushButtonInit(En_buttonId_t en_butotn_id)
{
	switch(en_butotn_id)
	{
		case BTN_0:
			gpioPinDirection(BTN_0_GPIO,BTN_0_BIT,LOW);			
		break;
		case BTN_1:
			gpioPinDirection(BTN_1_GPIO,BTN_1_BIT,LOW);
		break;
		case BTN_2:
			gpioPinDirection(BTN_2_GPIO,BTN_2_BIT,LOW);
		break;
		case BTN_3:
			gpioPinDirection(BTN_3_GPIO,BTN_3_BIT,LOW);
		break;		
	}
	
}

/**
 * Description: read BTN_x (where x 0, 1, 2, 3) state which is stored in the program
 * 				in the update function
 *
 */
En_buttonStatus_t pushButtonGetStatus(En_buttonId_t en_butotn_id)
{
	En_buttonStatus_t btn_state_latch = Released;  
	switch(en_butotn_id)
	{
		case BTN_0:
			btn_state_latch = gpioPinRead(BTN_0_GPIO,BTN_0_BIT);
			softwareDelayMs(40);
			btn_state_latch = (btn_state_latch == gpioPinRead(BTN_0_GPIO,BTN_0_BIT))? btn_state_latch : !(btn_state_latch);						
		break;
		case BTN_1:
			btn_state_latch = gpioPinRead(BTN_1_GPIO,BTN_1_BIT);
			softwareDelayMs(40);
			btn_state_latch = (btn_state_latch == gpioPinRead(BTN_1_GPIO,BTN_1_BIT))? btn_state_latch : !(btn_state_latch);			
		break;
		case BTN_2:
			btn_state_latch = gpioPinRead(BTN_2_GPIO,BTN_2_BIT);
			softwareDelayMs(40);
			btn_state_latch = (btn_state_latch == gpioPinRead(BTN_2_GPIO,BTN_2_BIT))? btn_state_latch : !(btn_state_latch);			
		break;
		case BTN_3:
			btn_state_latch = gpioPinRead(BTN_3_GPIO,BTN_3_BIT);
			softwareDelayMs(40);
			btn_state_latch = (btn_state_latch == gpioPinRead(BTN_3_GPIO,BTN_3_BIT))? btn_state_latch : !(btn_state_latch);			
		break;
	}
	return btn_state_latch;	
}
