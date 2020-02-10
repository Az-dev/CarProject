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
   UCSRA &= ~(0x1C); /* Check that FE & DOR & PE is set to zero*/
   UCSRC |= (usart_init_config.usart_mode_sel|usart_init_config.stop_bit_sel|usart_init_config.reg_sel_mode);
   /*----------------- initializing (UCSRA) -----------------*/
   UCSRB |= (usart_init_config.interrupt_mode_sel|usart_init_config.usart_dir_sel);
   /*----------------- initializing (UCSRA) -----------------*/
   UCSRA |= (usart_init_config.double_speed_select);
   /*------------------ initialize baude rate ---------------*/
   UBRRL = BAUDE_RATE;
   /*------------------ Character size select  ---------------*/
   /*---- setting it to 8-bit -----*/
   UCSRC |= 0x05;
   UCSRB &= ~(0x04);
   /*----- reset RXB8 and TXB8 ----*/
   UCSRB &= ~(0x03);
}

/*
*  Description : Read character from UDR.
*
*  @param void
*  @return uint8_t
*/
void UsartRx(void)
{
   //*c = (uint8_t)UDR;
   *c = UDR;
   //gpioPortWrite(GPIOB, UDR);
}

/*
*  Description : Write character to UDR
*
*  @param uint8_t c
*  @return void
*/
void UsartTx(void)
{
   UDR = *c;
}

/*------------------------------------- Interrupt handleres  -----------------------------------*/
ISR_USART_RX()
{
   //UCSRA &= ~(0x1C); /* Check that FE & DOR & PE is set to zero*/
   UsartRx();
}

ISR_USART_TX()
{
   //UCSRA &= ~(0x1C); /* Check that FE & DOR & PE is set to zero*/
   UsartTx();
}