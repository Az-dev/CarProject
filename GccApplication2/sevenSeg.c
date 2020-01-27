/*
 * sevenSeg.c
 *
 * Created: 1/18/2020 12:28:44 AM
 *  Author: Az
 */ 

#include "sevenSeg.h"

/**
 * Description: 
 * @param 
 */
void sevenSegInit(En_SevenSegId_t a_segment_id)
{
	switch(a_segment_id)
	{
		case SEG_0:
			//configure enable pins direction
			gpioPinDirection(SEG_EN1_GPIO,SEG_EN1_BIT,HIGH);			
		break;
		case SEG_1:
			//configure enable pins direction
			gpioPinDirection(SEG_EN2_GPIO,SEG_EN2_BIT,HIGH);			
		break;		
	}
	//configure BCD pins direction
	gpioPinDirection(SEG_BCD_GPIO,SEG_BCD_BITS,HIGH);
	//configure BCD decimal point pins direction
	gpioPinDirection(SEG_BCD_DP_GPIO,SEG_BCD_DP_BIT,HIGH);
}

/**
 * Description: 
 * @param 
 */
void sevenSegEnable(En_SevenSegId_t en_segment_id)
{
	switch(en_segment_id)
	{
		case SEG_0:				
			gpioPinWrite(SEG_EN1_GPIO,SEG_EN1_BIT,HIGH);
		break;
		case SEG_1:		
			gpioPinWrite(SEG_EN2_GPIO,SEG_EN2_BIT,HIGH);
		break;
	}
}

/**
 * Description: 
 * @param 
 */
void sevenSegDisable(En_SevenSegId_t en_segment_id)
{
	switch(en_segment_id)
	{
		case SEG_0:
			gpioPinWrite(SEG_EN1_GPIO,SEG_EN1_BIT,LOW);
		break;
		case SEG_1:
			gpioPinWrite(SEG_EN2_GPIO,SEG_EN2_BIT,LOW);
		break;
	}	
}

/**
 * Description: 
 * @param 
 */
void sevenSegWrite(En_SevenSegId_t en_segment_id, uint8_t u8_number)
{
	switch(en_segment_id)
	{
		case SEG_0:
			sevenSegDisable(SEG_1);
			sevenSegEnable(SEG_0);					
			gpioPinWrite(SEG_BCD_GPIO,u8_number,HIGH);
		break;
		case SEG_1:
			sevenSegDisable(SEG_0);
			sevenSegEnable(SEG_1);
			gpioPinWrite(SEG_BCD_GPIO,u8_number,HIGH);
		break;			
	}
}
