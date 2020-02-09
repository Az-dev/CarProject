#include "timers.h"

typedef struct gstr_timerInit_t{
    uint8_t en_mode;
    uint8_t en_OC0;
    uint8_t en_prescal;
    uint8_t u8_initialValue;
    uint8_t u8_outputCompare;
    uint8_t u8_assynchronous;
    uint8_t en_interruptMask;    
}gstr_timerInit_t; 

static gstr_timerInit_t timer0;
static gstr_timerInit_t timer2;

volatile uint32_t gu32_sw_counter = 0;
//volatile uint8_t gu8_duty_ticks = 0;
volatile uint8_t gu8_preloader = 0;
volatile uint16_t gu16_t2_sw_counter  = 0;

volatile uint16_t gu16_t1_en_prescal; /* Timer 1 prescaler */

/*======================================================= Timer 1 Control ==================================================*/
/**
 * Description:
 * @param controlA
 * @param controlB
 * @param initialValue
 * @param outputCompare
 * @param interruptMask
 */
void timer1Init(En_timer1Mode_t en_mode,En_timer1OC_t en_OC,En_timer1perscaler_t en_prescal, uint16_t u16_initialValue, uint16_t u16_outputCompareA, uint16_t u16_outputCompareB,uint16_t u16_inputCapture, En_timer1Interrupt_t en_interruptMask)
{
   TCCR1 |= en_mode; 
   TCCR1 |= 0xA000;  /*COM1A1/COM1B1 = 1 & COM1A0/COM1B0 = 0  -----> Clear OC1A/OC1B on compare match when up-counting. Set OC1A/OC1B on compare match when down counting.*/
   TCNT1 |= u16_initialValue;
   OCR1A |= u16_outputCompareA;
   OCR1B |= u16_outputCompareB;
   ICR1  |= u16_inputCapture;
   gu16_t1_en_prescal = en_prescal;  
}

/**
 * Description:
 */
void timer1Start(void)
{
   TCCR1B |= gu16_t1_en_prescal;
}

/**
 * Description:
 */
void timer1Stop(void)
{
   TCCR1B = T1_NO_CLOCK; 
}

/**
 * Description:
 * @param dutyCycle
 */
void timer1HwPWM(uint8_t u8_dutyCycle)
{  
   /* configure output pins direction and data*/
   gpioPinDirection(GPIOD,(BIT2|BIT3),HIGH);
   gpioPinWrite(GPIOD,(BIT4|BIT5),LOW); 
   /* Calculate initial value for OCR1A and OCR1B */
   uint32_t u32_ocr_initial = u8_dutyCycle * (32000 / 100);   
   /*initialize timer1 : to generate a wave of 250 Hz of frequency , so we initialize TCNT1 to 64000 -which is equal to (65537 - 1537)-*/
   timer1Init(T1_COMP_MODE_ICR1_BOTTOM,0xA000,T1_PRESCALER_NO,0,(uint16_t)u32_ocr_initial,(uint16_t)u32_ocr_initial,32000,T1_POLLING);
   //timer1Init(T1_COMP_MODE_ICR1_TOP,(T1_OC1A_SET|T1_OC1B_SET),T1_PRESCALER_NO,0,1000,1000,1,T1_POLLING);
   /*start timer*/
   timer1Start();
}

/*======================================================= Timer 0 Control ==================================================*/
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
    /***************** T0: OUTPUT COMPARE MODE & Polling CONTROL **************************/
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


/*========================================================== Timer2 Control ================================================*/
/**
 * Description:
 * @param control
 * @param initialValue
 * @param outputCompare
 * @param interruptMask
 */
void timer2Init(En_timer2Mode_t en_mode,En_timer2OC_t en_OC,En_timer2perscaler_t en_prescal, uint8_t u8_initialValue, uint8_t u8_outputCompare, uint8_t u8_assynchronous, En_timer2Interrupt_t en_interruptMask)
{
   /* Initialize TCCR0 */
   timer2.en_mode = en_mode;
   /* set en_OC0*/
   timer2.en_OC0 = en_OC;
   /* Set prescaler */
   timer2.en_prescal = en_prescal;
   /* set u8_initialValue */
   timer2.u8_initialValue = u8_initialValue;
   /*set u8_outputCompare */
   timer2.u8_outputCompare = u8_outputCompare;
   /*set assynchronous*/
   timer2.u8_assynchronous = u8_assynchronous;
   /* set en_interruptMask */
   timer2.en_interruptMask = en_interruptMask;   
}
/**
 * Description:
 * @param value
 */
void timer2Set(uint8_t u8_a_value)
{
   switch(timer0.en_mode)
   {
      case T2_NORMAL_MODE:
         //timer is in normal mode
         TCNT2 |= u8_a_value;
      break;
      case T2_COMP_MODE:
         //timer is in CTC mode
         TCNT2 = 0x00;
         OCR2 |= u8_a_value;
      break;
   }
}

/**
 * Description:
 * @return
 */
uint8_t timer2Read(void)
{
   return TCNT2;
}

/**
 * Description:
 */
void timer2Start(void)
{
   TCCR2 |= (timer2.en_mode | timer2.en_prescal);
}

/**
 * Description:
 */
void timer2Stop(void)
{
   TCCR2 &= T2_NO_CLOCK;   
}

/**
 * Description:
 * @param delay
 */
void timer2DelayMs(uint16_t u16_delay_in_ms)
{
    uint32_t ticks_number = 0;
	 uint8_t t2_tov_initial = 0;  /* overflow initial value*/
    uint8_t t2_oc_initial = 0;   /* output compare initial value*/
    
    /* Initialize the timer*/
    timer2Init(T2_NORMAL_MODE,T2_OC2_CLEAR,T2_PRESCALER_NO,0,0,0,T2_INTERRUPT_NORMAL);   

    /* calculate tick number in Milli seconds */    
    ticks_number = (CPU_F / (timer2.en_prescal * MILLI_SECONDS)) * u16_delay_in_ms;
    /* calculate software_counter value in Milli seconds */
    gu16_t2_sw_counter   = ticks_number / T2_OV_VAL;
    if((ticks_number % T2_OV_VAL)) /* if (ticks_number % T2_OV_VAL) != 0*/
    {
         /*increment gu16_sw_counter*/
         gu16_t2_sw_counter++;
         /* set t2_tov_initial value */
         t2_tov_initial = (T2_OV_VAL - (uint8_t)(ticks_number % T2_OV_VAL));
         /* set t2_oc_initial*/
         t2_oc_initial = (uint8_t)(ticks_number % T2_OV_VAL); 
    }else
    {
       /* set t2_tov_initial value */
       t2_tov_initial = 0;
       /* set t2_oc_initial*/
       t2_oc_initial = T2_OCR_MAX;  
    } 
    /********************************************** NORMAL MODE CONTROL **********************************************/
    /* set timer0 : set TOV initial value*/
    timer2Set(t2_tov_initial);
    switch(timer2.en_interruptMask)
    {
       case T2_INTERRUPT_NORMAL:
       /* set Global interrupt bit*/
       sei();
       /* set TOIE0 */
       TIMSK |= timer2.en_interruptMask;
       /* start the timer*/
       timer2Start();
       /* wait for the timer to finish*/
       while(1)
       {
          if(gu16_t2_sw_counter == 0) break;
       }
       /* stop the timer*/
       timer2Stop();
       /* reset TOIE0 */
       TIMSK &= ~(timer2.en_interruptMask);
       /* clear Global interrupt bit*/
       cli();
       break;
   } 
}

/************************************************ Timers ISRs Control **********************************************************/
ISR_TIMER0_OVF(){
   /*reset pins*/
   gpioPinWrite(GPIOD,(BIT4|BIT5),LOW);
   /*reload TCNT0*/
   TCNT0 = gu8_preloader;  //debug point
}

ISR_TIMER0_COMP(){
   /*set pins*/
   gpioPinWrite(GPIOD,(BIT4|BIT5),HIGH);
}

ISR_TIMER2_OVF(){
   /* decrement global software counter*/
   gu16_t2_sw_counter-=1;
}












