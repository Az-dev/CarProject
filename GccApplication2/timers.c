#include "timers.h"

typedef struct gstr_timerInit_t{
    uint8_t en_mode;
    uint8_t en_OC0;
    uint8_t en_prescal;
    uint8_t u8_initialValue;
    uint8_t u8_outputCompare;
    uint8_t en_interruptMask;    
}gstr_timerInit_t; 

static gstr_timerInit_t timer0;
volatile uint32_t gu32_sw_counter = 0;
//volatile uint8_t gu8_duty_ticks = 0;
volatile uint8_t gu8_preloader = 0;

/**
 * Description:
 * @param control
 * @param initialValue
 * @param outputCompare
 * @param interruptMask
 */
void timer0Init(En_timer0Mode_t en_mode,En_timer0OC_t en_OC0,En_timer0perscaler_t en_prescal, uint8_t u8_initialValue, uint8_t u8_outputCompare, En_timer0Interrupt_t en_interruptMask)
{    
    /* Initialize TCCR0 */    
    timer0.en_mode = en_mode;
	/* set en_OC0*/
	timer0.en_OC0 = en_OC0;
    /* Set prescaler */    
    timer0.en_prescal = en_prescal; 
    /* set u8_initialValue */
    timer0.u8_initialValue = u8_initialValue;
    /*set u8_outputCompare */
    timer0.u8_outputCompare = u8_outputCompare; 
    /* set en_interruptMask */
    timer0.en_interruptMask = en_interruptMask;      
}

/**
 * Description:
 * @param value
 */
void timer0Set(uint8_t u8_value)
{
   switch(timer0.en_mode)
   {
      case T0_NORMAL_MODE:
         //timer is in normal mode
         TCNT0 |= u8_value;
      break;
      case T0_COMP_MODE:
         //timer is in CTC mode
         TCNT0 = 0x00;
         OCR0 |= u8_value;
      break;      
   }    
}

/**
 * Description:
 * @return
 */
uint8_t timer0Read(void)
{
    return TCNT0;
}

/**
 * Description:
 */
void timer0Start(void)
{
    TCCR0 |= (timer0.en_mode | timer0.en_prescal);
}

/**
 * Description:
 */
void timer0Stop(void)
{
    TCCR0 &= T0_NO_CLOCK;
}

/**
 * Description:
 * @param delay
 */
void timer0DelayMs(uint16_t u16_delay_in_ms)
{
    uint32_t ticks_number = 0;
	 //uint8_t t0_tov_initial = 0;  /* overflow initial value*/
    uint8_t t0_oc_initial = 0;   /* output compare initial value*/
    uint16_t i = 0;         
    /* initialize timer 0*/    
    timer0Init(T0_COMP_MODE,T0_OC0_CLEAR,T0_PRESCALER_NO,0,0,T0_POLLING);	/* Compare Mode & Polling*/    
    /* calculate tick number in Milli seconds */    
    ticks_number = (CPU_F / (timer0.en_prescal * MILLI_SECONDS)) * u16_delay_in_ms;
    /* calculate software_counter value in Milli seconds */
    gu32_sw_counter   = ticks_number / T0_OV_VAL;
    if((ticks_number % T0_OV_VAL)) /* if (ticks_number % T0_OV_VAL) != 0*/
    {
         /*increment gu32_sw_counter*/
         gu32_sw_counter++;
         /* set t0_tov_initial value */
         //t0_tov_initial = (T0_OV_VAL - (uint8_t)(ticks_number % T0_OV_VAL));
         /* set t0_oc_initial*/
         t0_oc_initial = (uint8_t)(ticks_number % T0_OV_VAL); 
    }else
    {
       /* set t0_tov_initial value */
       //t0_tov_initial = 0;
       /* set t0_oc_initial*/
       t0_oc_initial = T0_OCR_MAX;  
    }    
    /********************************************** T0: OUTPUT COMPARE MODE & Polling CONTROL **********************************************/
    /*set timer0 : set OC initial value*/
    timer0Set(t0_oc_initial);    
    /* start the timer*/
    timer0Start();
    while(i < gu32_sw_counter)
    {
       i++;
       while(1)
       {
          if((TIFR & 0x02))
          {
             /*reset OCF0*/
             TIFR |= 0x02;
             /*reinitialize OCR0 */
             OCR0 = T0_OCR_MAX;
             /*break while loop*/
             break;
          };
       }
    }
    /* stop the timer*/
    timer0Stop();	   
}

/**
 * Description:
 * @param dutyCycle
 */
void timer0SwPWM(uint8_t u8_dutyCycle,uint8_t u8_frequency)
{
   uint32_t u8_frequency_ticks = (((CPU_F / 1000) / 64) / u8_frequency);  
   uint32_t u16_duty_ticks = ((100-u8_dutyCycle) * (uint8_t)u8_frequency_ticks) / 100;
   gu8_preloader = (255-(u8_frequency_ticks-1));
   /* configure output pins direction and data*/
   gpioPinDirection(GPIOD,(BIT4|BIT5),HIGH);
   gpioPinWrite(GPIOD,(BIT4|BIT5),LOW); 
   /*enable interrupts*/
   sei();                              /* enable global mask */
   TIMSK |= timer0.en_interruptMask;   /*enable T0_INTERRUPT_CMP mask*/           
   /* initialize counter */
   timer0Init(T0_NORMAL_MODE,T0_OC0_CLEAR,T0_PRESCALER_1024,gu8_preloader,(gu8_preloader + u16_duty_ticks),(T0_INTERRUPT_CMP|T0_INTERRUPT_NORMAL));
   
   /*initialize TCNT0*/
   TCNT0 = timer0.u8_initialValue;
   /*initialize OCR0*/
   OCR0 = timer0.u8_outputCompare;    
   /*Initialize OCR0*/  
   //timer0Set(gu8_duty_ticks);         
   /*start the timer*/
   timer0Start();       
}

/************************************************ Timers ISRs **********************************************************/
/***************  T0 ISRs *************/
ISR_TIMER0_OVF()
{  
   /*reset pins*/ 
   gpioPinWrite(GPIOD,(BIT4|BIT5),LOW);
   /*reload TCNT0*/
   TCNT0 = gu8_preloader;  //debug point  
}

ISR_TIMER0_COMP()
{
   /*set pins*/
   gpioPinWrite(GPIOD,(BIT4|BIT5),HIGH);      
}









