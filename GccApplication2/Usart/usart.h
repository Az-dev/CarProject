#ifndef __USART_H__
#define __USART_H__
/*---------------------------------------- #includes ----------------------------------------*/
#include "../gpio.h"
#include "../registers.h"
#include "usart_Cfg.h"
#include "../std_types.h"
/*---------------------------------------- #defines -----------------------------------------*/
#define BAUDE_RATE 103 /* this value generates baude rate of 9600 at fosc = 16MHz */

/*
*  Description : Intializes USART.
*
*  @param void
*  @return void
*/
void Usart_Init(void);

/*
*  Description : Read a character from RXB.
*
*  @param void
*  @return uint8_t
*/
uint8_t UsartReadRx(void);

/*
*  Description : Write a character to TXB
*
*  @param uint8_t * c
*  @return void
*/
void UsartWriteTx(volatile uint8_t * c);

#endif /* END OF __USART_H__ */