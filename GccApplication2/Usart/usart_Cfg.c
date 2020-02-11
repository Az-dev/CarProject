/****************************************    #Includes   ************************************/
#include "usart_Cfg.h"

/* create usart intialization struct */
const gstr_usart_init_t usart_init_config = 
{
   ASYNCH_MODE,
   PARITY_DISABLED,
   STOP_1_BIT,
   UCSRC_SEL,
   //(RECIEVE_COMPLETE_EN|TRANSMIT_COMPLETE_EN),
   INTERRUPT_DIS,
   //(RECIEVE_COMPLETE_EN|DATA_REGISTER_EMPTY_EN),
   RECIEVE_EN,
   U2X_DIS,
};

