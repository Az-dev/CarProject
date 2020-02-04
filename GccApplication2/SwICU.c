/*
 * SwICU.c
 *
 * Created: 1/30/2020 12:11:53 AM
 *  Author: Az
 */ 

/*
 * SwICU.h
 *
 *  Created on: Oct 27, 2019
 *      Author: Sprints
 */

#ifndef SWICU_H_
#define SWICU_H_

#include "interrupt.h"
#include "timers.h"
#include "SwICU.h"
#include "led.h"




typedef enum EN_SwICU_Edge_t{
	SwICU_EdgeFalling = 2,
	SwICU_EdgeRisiging = 3
}EN_SwICU_Edge_t;

extern volatile uint8_t * gu_sw_icu;
extern volatile uint8_t sw_icu_reset;



/*
* Description : Decides on Whether SwICU will be initialized to falling or rising edge
*
* @params : EN_SwICU_Edge_t a_en_inputCaptureEdge
*
*/
void SwICU_Init(EN_SwICU_Edge_t a_en_inputCaptureEdge)
{
   /*****************************************  Initialize *******************************************/
   /*Initialize Timer*/
   timer2Init(T2_NORMAL_MODE,T2_OC2_CLEAR,T2_PRESCALER_64,0,0,0,T2_INTERRUPT_NORMAL);
   /*Initialize SW_ICU*/   
   if (a_en_inputCaptureEdge == SwICU_EdgeFalling)
   {
      /*Initialize ISCR to (0) to work in the falling edge) */
      MCUCSR &= (~BIT6);     
   }
   else if(a_en_inputCaptureEdge == SwICU_EdgeRisiging)
   {
      /*Initialize ISCR to (1) to work in the rising edge) */
      MCUCSR |= BIT6;          
   }            
}

/*
* Description : returns the edge configuration of SwICU
*
* @return : EN_SwICU_Edge_t
*
*/
EN_SwICU_Edge_t SwICU_GetCfgEdge(void)
{   
   if (MCUCSR & (BIT6))
   {
      return SwICU_EdgeRisiging; /*ISCR is (1) : works in the rising edge*/            
   }else{
      return SwICU_EdgeFalling;   /*ISCR is (0) : works in the falling edge*/                  
   }    
}

/*
* Description : Sets the clock edge of the SwICU
*
* @param : EN_SwICU_Edge_t a_en_inputCaptureEdgeedge
*
*/
void SwICU_SetCfgEdge(EN_SwICU_Edge_t a_en_inputCaptureEdgeedge)
{
   
   /*Disable SwICU*/
   //SwICU_Disable();   
   /*Re-initialize*/
   SwICU_Init(a_en_inputCaptureEdgeedge);   
   /*Enable SwICU*/
   //SwICU_Enable();    
}

void SwICU_Enable(void)
{
   /*Enable INT2*/
   GICR |= (BIT5);
   //GICR |= 0xf0; /* because INT2 works only when one or both of the other 2 interrupts is enabled !!!!!?*/
}
void SwICU_Disable(void)
{
   /*Disable INT2*/
   GICR &= ~(BIT5);
}

void SwICU_Read(volatile uint8_t * a_pu8_capt)
{
   /*Read TCNT2*/
   *a_pu8_capt = timer2Read();
   /*Reset TCNT2*/
   timer2Set(0);   
}

void SwICU_Stop(void)
{
   /*Disable INT2*/
   SwICU_Disable();
   /*stop timer2 */
   timer2Stop();      
}

void SwICU_Start(void)
{  
   /*Enable INT2*/
   //SwICU_Enable();     
   /*Start timer2*/
   timer2Start();
}



/**************************** INT2 Control *******************************/
ISR_INT2()
{
   if(SwICU_GetCfgEdge() == SwICU_EdgeFalling)
   {   
      /* Stop SwICU */
      SwICU_Stop();               
      /* Read SW_ICU*/
      SwICU_Read(gu_sw_icu);      
      /*set configuration edge*/
      SwICU_SetCfgEdge(SwICU_EdgeRisiging);
      /**Debuger**/
      gpioPortWrite(GPIOB,0xf0);
      while(1){}
      /****/ 
      /*set reset flag*/
      sw_icu_reset = HIGH;                        
   }
   else if(SwICU_GetCfgEdge() == SwICU_EdgeRisiging)
   {
      /* Stop SwICU */
      SwICU_Start();
      /*set configuration edge*/
      SwICU_SetCfgEdge(SwICU_EdgeFalling);                   
   } 
}

#endif /* SWICU_H_ */

