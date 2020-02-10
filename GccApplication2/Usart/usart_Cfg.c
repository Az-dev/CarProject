/****************************************    #Includes   ************************************/
#include "usart_Cfg.h"

/* create usart intialization struct */
const gstr_usart_init_t usart_init_config = 
{
   //(uint8_t)ASYNCH_MODE,
   ASYNCH_MODE,
   PARITY_DISABLED,
   STOP_1_BIT,
   UCSRC_SEL,
   (RECIEVE_COMPLETE_EN|TRANSMIT_COMPLETE_EN),
   //(RECIEVE_COMPLETE_EN|DATA_REGISTER_EMPTY_EN),
   (RECIEVE_EN|TRANSMIT_EN),
   U2X_DIS,
};

