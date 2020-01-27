/*
 * gpio.c
 *
 * Created: 1/17/2020 3:40:53 PM
 *  Author: Az
 */ 
#include "gpio.h"
#include "registers.h"
#include "std_types.h"

/*===========================PIN Control===============================*/
/**
 * Description: set selected pins (more than one pin [ORed]) direction
 *
 * @param port: portID and takes the following values
 * 				- 0 -> GPIOA
 * 				- 1 -> GPIOB
 * 				- 2 -> GPIOC
 * 				- 3 -> GPIOD
 * @param pins: the selected pins the user need to set its direction and takes
 * 				any member from the enum EN_bits:
 * 				- BIT0
 * 				- BIT1
 * 				- BIT2
 * 				- BIT3
 * 				- BIT4
 * 				- BIT5
 * 				- BIT6
 * 				- BIT7
 *
 * @param direction: set the pins direction and takes the following values
 * 				- 0x00 -> Input
 * 				- 0xff -> Output
 */
void gpioPinDirection(uint8_t u8_port, uint8_t u8_pins, uint8_t u8_direction)
{
	/*check the direction whether input or output*/
	if(!u8_direction)
	{		
		/*direction is input*/
		/*invert u8_pins*/
		u8_pins = ~u8_pins;	
		switch(u8_port)
		{
			case 0: /* GPIOA */			
			PORTA_DIR &= u8_pins;			
			break;
			case 1: /* GPIOB */			
			PORTB_DIR &= u8_pins;
			break;
			case 2: /* GPIOC */
			PORTC_DIR &= u8_pins;			
			break;
			case 3: /* GPIOD */
			PORTD_DIR &= u8_pins;			
			break;			
		}		
	}else{
		/*direction is output*/
		switch(u8_port)
		{
			case 0: /* GPIOA */
			PORTA_DIR |= u8_pins;
			break;
			case 1: /* GPIOB */
			PORTB_DIR |= u8_pins;
			break;
			case 2: /* GPIOC */
			PORTC_DIR |= u8_pins;
			break;
			case 3: /* GPIOD */
			PORTD_DIR |= u8_pins;
			break;			
		}		
	}	
}

/**
 * Description: set selected pins (more than one pin [ORed]) values
 * @param port: portID and takes the following values
 * 				- 0 -> GPIOA
 * 				- 1 -> GPIOB
 * 				- 2 -> GPIOC
 * 				- 3 -> GPIOD
 * @param pins: the selected pins the user need to write its values and takes
 * 				any member from the enum EN_bits:
 * 				- BIT0
 * 				- BIT1
 * 				- BIT2
 * 				- BIT3
 * 				- BIT4
 * 				- BIT5
 * 				- BIT6
 * 				- BIT7
 * @param value
 */
void gpioPinWrite(uint8_t u8_port, uint8_t u8_pins, uint8_t u8_value)
{
	if(!u8_value)
	{
		u8_pins = ~u8_pins;
		switch(u8_port)
		{
			case 0: /* GPIOA */
			PORTA_DATA &= u8_pins;		/* clear pins*/
			break;
			case 1: /* GPIOB */
			PORTB_DATA &= u8_pins;		/* clear pins*/
			break;
			case 2: /* GPIOC */
			PORTC_DATA &= u8_pins;		/* clear pins*/
			break;
			case 3: /* GPIOD */
			PORTD_DATA &= u8_pins;		/* clear pins*/
			break;
		}		
	}else
	{
		switch(u8_port)
		{
			case 0: /* GPIOA */
			PORTA_DATA |= u8_pins;		/* set pins*/
			break;
			case 1: /* GPIOB */
			PORTB_DATA |= u8_pins;		/* set pins*/
			break;
			case 2: /* GPIOC */
			PORTC_DATA |= u8_pins;		/* set pins*/
			break;
			case 3: /* GPIOD */
			PORTD_DATA |= u8_pins;		/* set pins*/
			break;
		}
	}
		
}

/**
 * Description: toggle selected pin (only one pin) direction
 * @param port: portID and takes the following values
 * 				- 0 -> GPIOA
 * 				- 1 -> GPIOB
 * 				- 2 -> GPIOC
 * 				- 3 -> GPIOD
 * @param pins: the selected pin the user need to toggle it's value and takes
 * 				only one member from the enum EN_bits:
 * 				- BIT0
 * 				- BIT1
 * 				- BIT2
 * 				- BIT3
 * 				- BIT4
 * 				- BIT5
 * 				- BIT6
 * 				- BIT7
 */
void gpioPinToggle(uint8_t u8_port, uint8_t u8_pins)
{
	switch(u8_port)
	{
		case 0: /* GPIOA */		
		PORTA_DATA ^= u8_pins;		/* toggle pins values*/
		break;
		case 1: /* GPIOB */		
		PORTB_DATA ^= u8_pins;		/* toggle pins values*/
		break;
		case 2: /* GPIOC */		
		PORTC_DATA ^= u8_pins;		/* toggle pins values*/
		break;
		case 3: /* GPIOD */		
		PORTD_DATA ^= u8_pins;		/* toggle pins values*/
		break;
	}	
}

/**
 * Description: read selected pin (only one pin) direction
 * @param port: portID and takes the following values
 * 				- 0 -> GPIOA
 * 				- 1 -> GPIOB
 * 				- 2 -> GPIOC
 * 				- 3 -> GPIOD
 *
 * @param pin: the selected pin the user need to read it's value and takes
 * 				only one member from the enum EN_bits:
 * 				- BIT0
 * 				- BIT1
 * 				- BIT2
 * 				- BIT3
 * 				- BIT4
 * 				- BIT5
 * 				- BIT6
 * 				- BIT7
 * @return
 */
uint8_t gpioPinRead(uint8_t u8_port, uint8_t u8_pin)
{
	uint8_t pin_val = 0x00;
	switch(u8_port)
	{
		case 0: /* GPIOA */
		pin_val = PORTA_PIN & u8_pin ;		/* set pins*/
		break;
		case 1: /* GPIOB */
		pin_val = PORTB_PIN & u8_pin;		/* set pins*/
		break;
		case 2: /* GPIOC */
		pin_val = PORTC_PIN & u8_pin;		/* set pins*/
		break;
		case 3: /* GPIOD */
		pin_val = PORTD_PIN & u8_pin;		/* set pins*/
		break;
	}
	if(!pin_val) return 0x00;
	else return 0x01;
}

/*===========================PORT Control===============================*/
/**
 * Description: set port direction (which is DDR register)
 * @param port: portID and takes the following values
 * 				- 0 -> GPIOA
 * 				- 1 -> GPIOB
 * 				- 2 -> GPIOC
 * 				- 3 -> GPIOD
 *
 * @param direction: set the port direction and takes the following values
 * 				- 0x00 -> Input
 * 				- 0xff -> Output
 */
void gpioPortDirection(uint8_t u8_port, uint8_t u8_direction)
{
	switch(u8_port)
	{
		case 0: /* GPIOA */
		PORTA_DIR = u8_direction;
		break;
		case 1: /* GPIOB */
		PORTB_DIR = u8_direction;
		break;
		case 2: /* GPIOC */
		PORTC_DIR = u8_direction;
		break;
		case 3: /* GPIOD */
		PORTD_DIR = u8_direction;
		break;
	}	
}

/**
 * Description: set the port value (which is PORT register)
 * @param port: portID and takes the following values
 * 				- 0 -> GPIOA
 * 				- 1 -> GPIOB
 * 				- 2 -> GPIOC
 * 				- 3 -> GPIOD
 * @param value: set the port value and takes the following values
 * 				- 0x00 -> Low
 * 				- 0xff -> High
 */
void gpioPortWrite(uint8_t u8_port, uint8_t u8_value)
{
	switch(u8_port)
	{
		case 0: /* GPIOA */
		PORTA_DATA = u8_value;
		break;
		case 1: /* GPIOB */
		PORTB_DATA = u8_value;
		break;
		case 2: /* GPIOC */
		PORTC_DATA = u8_value;
		break;
		case 3: /* GPIOD */
		PORTD_DATA = u8_value;
		break;
	}
	
}

/**
 * Description: toggle the port value (which is PORT register)
 * @param port: portID and takes the following values
 * 				- 0 -> GPIOA
 * 				- 1 -> GPIOB
 * 				- 2 -> GPIOC
 * 				- 3 -> GPIOD
 */
void gpioPortToggle(uint8_t u8_port)
{
	switch(u8_port)
	{
		case 0: /* GPIOA */
		PORTA_DATA ^= 0xff;
		break;
		case 1: /* GPIOB */
		PORTB_DATA ^= HIGH;
		break;
		case 2: /* GPIOC */
		PORTC_DATA ^= HIGH;
		break;
		case 3: /* GPIOD */
		PORTD_DATA ^= HIGH;
		break;
	}
}

/**
 * Description: read the current port value
 * @param port: portID and takes the following values
 * 				- 0 -> GPIOA
 * 				- 1 -> GPIOB
 * 				- 2 -> GPIOC
 * 				- 3 -> GPIOD
 * @return
 */
uint8_t gpioPortRead(uint8_t u8_port)
{
	uint8_t port_val = 0x00;
	switch(u8_port)
	{
		case 0: /* GPIOA */
		port_val = PORTA_PIN;
		break;
		case 1: /* GPIOB */
		port_val = PORTB_PIN;
		break;
		case 2: /* GPIOC */
		port_val = PORTC_PIN;
		break;
		case 3: /* GPIOD */
		port_val = PORTD_PIN;
		break;
	}
	return port_val;
}