 /*---------------------------------------- #Include & #Define ---------------------------------*/
#include "usart.h"

/* Create a new Usart instance in the main -Dont forget - */
//Usart s ={/*put your initial params here and then s will be passed to functions*/};
extern volatile uint8_t * c;   

/*
*  Description : Intializes USART.
*
*  @param void
*  @return void
*/
void Usart_Init(void)
{
   /*----------------- initializing (UCSRA) -----------------*/   
   UCSRC |= (usart_init_config.usart_mode_sel|usart_init_config.stop_bit_sel|usart_init_config.reg_sel_mode|0x05); /*0x05 becuase we want UCSZ0:1 to be set to 1 1 ---> in order to obtain 8-bit width character*/
   //UCSRC |= 0x85; /*works fine*/
   /*----------------- initializing (UCSRA) -----------------*/
   UCSRB |= (usart_init_config.interrupt_mode_sel|usart_init_config.usart_dir_sel);
   /*----------------- initializing (UCSRA) -----------------*/
   UCSRA &= ~(0x1C); /* Check that FE & DOR & PE is set to zero*/
   UCSRA |= (usart_init_config.double_speed_select);
   /*------------------ initialize baude rate ---------------*/
   UBRRL = BAUDE_RATE;
   /*------------------ Character size select  ---------------*/
   /*---- setting it to 8-bit -----*/
   //UCSRC |= 0x05;
   UCSRB &= ~(0x04); /* ----> check this if it will work or not */
   /*----- clear RXB8 and TXB8 ----*/
   UCSRB &= ~(0x03);
}

/*
*  Description : Read a character from RXB.
*
*  @param void
*  @return uint8_t
*/
uint8_t UsartReadRx(void)
{
   return UDR;
}

/*
*  Description : Write a character to TXB
*
*  @param uint8_t * c
*  @return void
*/
void UsartWriteTx(volatile uint8_t * c)
{
   //UDR = *c;
}

/*------------------------------------- Interrupt handlers  -----------------------------------*/

/*
* Usart on successful character receive completion interrupt handler 
*/
ISR_USART_RX()
{
   /*
   * on successful character reception ..> write the new character
   */
   //UsartWriteTx(&c);
   return 0;   
}

/*
* Usart on successful character transmit completion interrupt handler
*/
ISR_USART_TX()
{
   /*
   * on successful character transmission ..> read the new character 
   */
   //*c = (volatile uint8_t)UsartReadRx();
   return 0;   
}