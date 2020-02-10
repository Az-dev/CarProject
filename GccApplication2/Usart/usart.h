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
*  Description : Read character from UDR.
*
*  @param void
*  @return uint8_t
*/
void UsartTx(void);


/*
*  Description : Write character to UDR
*
*  @param uint8_t c
*  @return void
*/
void UsartRx(void);


#endif /* END OF __USART_H__ */