/*
 * GPIO.c
 *
 * Created: 1/17/2020 2:10:14 PM
 * Author : Az
 */ 
#include "registers.h"
#include "gpio.h"
#include "softwareDelay.h"
#include "led.h"
#include "pushButton.h"
#include "std_types.h"
#include "sevenSeg.h"
#include "timers.h"
#include "dcMotor.h"
#include "SwICU.h"
#include "ultraSonicConfig.h"
#include "Usart/usart.h"


volatile uint8_t gu_distance_read = LOW;
volatile uint8_t gu_sw_icu = 0;

uint8_t test_c;
volatile uint8_t * c = &test_c;


void gpioReq7(void);
void gpioReq8(void);
void gpioReq9(void);
void test(void);
void testPwm(void);
void car(void);
void swIcuDistanceMeasurement(void);
void carObtsacleDetection(void);
void usartTest(void);


int main(void)
{ 
	/* gpioReq7();*/ 
	/* gpioReq8(); */
	/* gpioReq9(); */
	//test(); 
   /*testPwm();*/
   //car();   
   //swIcuDistanceMeasurement();
   //carObtsacleDetection();
   
   UsartRx(); 
   usartTest(); 
   gpioPortDirection(GPIOB,HIGH);    
   while(1)
   {
      gpioPortWrite(GPIOB,*c);      
      softwareDelayMs(1000); 
   }
   
}

void usartTest()
{
   sei();
   Usart_Init();
}


void swIcuDistanceMeasurement()
{
   /*------------------------------------------------------ Initialization --------------------------------------------------*/   
   /* Initialize Timer2 : Its counts will be used for distance measurement */
   timer2Init(T2_NORMAL_MODE,T2_OC2_CLEAR,T2_PRESCALER_64,0,0,0,T2_INTERRUPT_NORMAL);
   /* Initialize UltraSonicSensor */
   //gpioPinDirection(ULTRA_EN_GPIO,ULTRA_ENABLE_BIT,OUTPUT); /* Triggering pin */
   gpioPinDirection(ULTRA_OUT_GPIO,ULTRA_OUT_BIT,INPUT);    /* Echo pin */
   PORTB_DIR |= BIT3;
   /* Initialize LEDs : will be used to as an output for distance measurement value */
   Led_Init(LED_0);
   Led_Init(LED_1);
   Led_Init(LED_2);
   Led_Init(LED_3);
   /* Set Global Interrupt */
   sei(); 
   /**********/
   /*Disable INT2*/
   GICR &= ~(BIT5);   
   /*Set ISC2 to (1) : that will fire INT2 on rising edge */
   MCUCSR |= BIT6; 
   /*Reset INTF2 flag bit by setting 1*/
   GIFR |= BIT5; 
   /*Enable INT2*/
   GICR |= (BIT5);   
   /*---------------------------------------------------- Trigger the Sensor -----------------------------------------------------*/    
   gpioPinWrite(ULTRA_EN_GPIO,ULTRA_ENABLE_BIT,HIGH);     
   timer0DelayMs(1);   
   gpioPinWrite(ULTRA_EN_GPIO,ULTRA_ENABLE_BIT,LOW);      
   while(1)
   {       
      if(gu_distance_read == HIGH)
      {               
         gpioPortWrite(GPIOB,((68*gu_sw_icu)/1000)<<4);
         timer0DelayMs(100);  
         gu_distance_read = LOW;
         gpioPinWrite(ULTRA_EN_GPIO,ULTRA_ENABLE_BIT,HIGH);         
         timer0DelayMs(1);
         gpioPinWrite(ULTRA_EN_GPIO,ULTRA_ENABLE_BIT,LOW);                     
      }     
   }
   
}

/************ INT2 ISR ***********/
ISR_INT2()
{
   if(MCUCSR & BIT6)  /* if ISC2 is set --> Rising edge */
   {
      /*start timer2*/
      timer2Start();
      /*change edge*/
      /**********/
      /*Disable INT2*/
      GICR &= ~(BIT5);
      /*Set ISC2 to (0) : that will fire INT2 on falling edge */
      MCUCSR &= (~BIT6);      
      /*Reset INTF2 flag bit by setting 1*/
      GIFR |= BIT5;
      /*Enable INT2*/
      GICR |= (BIT5);            
   }
   else if(!(MCUCSR & BIT6)) /* if ISC2 is set to (0) --> Falling Edge */ /* assert its value !!!??*/
   {
      /*stop timer counter*/
      timer2Stop();
      /*read TCNT2*/
      gu_sw_icu = timer2Read();
      /*Reset TCNT2*/
      timer2Set(0);
      /*change edge*/
      /**********/
      /*Disable INT2*/
      GICR &= ~(BIT5);
      /*Set ISC2 to (1) : that will fire INT2 on rising edge */
      MCUCSR |= (BIT6);      
      /*Reset INTF2 flag bit by setting 1*/
      GIFR |= BIT5;
      /*Enable INT2*/
      GICR |= (BIT5);
      /*set read flag*/
      gu_distance_read = HIGH;
   }    
   
}

void carObtsacleDetection()
{
   /*------------------------------------------------------ Initialization --------------------------------------------------*/
   /* Initialize Timer2 : Its counts will be used for distance measurement */
   timer2Init(T2_NORMAL_MODE,T2_OC2_CLEAR,T2_PRESCALER_64,0,0,0,T2_INTERRUPT_NORMAL);
   /*Initialize motors*/
   MotorDC_Init(MOT_1);
   MotorDC_Init(MOT_2);
   /* Initialize UltraSonicSensor */
   //gpioPinDirection(ULTRA_EN_GPIO,ULTRA_ENABLE_BIT,OUTPUT); /* Triggering pin */
   gpioPinDirection(ULTRA_OUT_GPIO,ULTRA_OUT_BIT,INPUT);    /* Echo pin */
   PORTB_DIR |= BIT3;
   /* Initialize LEDs : will be used to as an output for distance measurement value */
   Led_Init(LED_0);
   Led_Init(LED_1);
   Led_Init(LED_2);
   Led_Init(LED_3);
   /* Set Global Interrupt */
   sei();
   /**********/
   /*Disable INT2*/
   GICR &= ~(BIT5);
   /*Set ISC2 to (1) : that will fire INT2 on rising edge */
   MCUCSR |= BIT6;
   /*Reset INTF2 flag bit by setting 1*/
   GIFR |= BIT5;
   /*Enable INT2*/
   GICR |= (BIT5);
   /*---------------------------------------------------- Trigger the Sensor -----------------------------------------------------*/
   gpioPinWrite(ULTRA_EN_GPIO,ULTRA_ENABLE_BIT,HIGH);
   timer0DelayMs(1);
   gpioPinWrite(ULTRA_EN_GPIO,ULTRA_ENABLE_BIT,LOW);
   while(1)
   {
      if(gu_distance_read == HIGH)
      {
         gpioPortWrite(GPIOB,((68*gu_sw_icu)/1000)<<4);
         timer0DelayMs(100);
         gu_distance_read = LOW;
         gpioPinWrite(ULTRA_EN_GPIO,ULTRA_ENABLE_BIT,HIGH);
         timer0DelayMs(1);
         gpioPinWrite(ULTRA_EN_GPIO,ULTRA_ENABLE_BIT,LOW);
         if(((68*gu_sw_icu)/1000) > 5)
         {
            /* start moving forward*/
            MotorDC_Dir(MOT_1,FORWARD);
            MotorDC_Dir(MOT_2,FORWARD);
            MotorDC_Speed_HwPWM(70);
            softwareDelayMs(300);
         }else{
            MotorDC_Dir(MOT_1,STOP);
            MotorDC_Dir(MOT_2,STOP);
            timer1Stop();
         }
         
      }
      
      
   }         
}


void car()
{
   /********************************************** Initialization ********************************************************/
   /*Initialize motors*/
   MotorDC_Init(MOT_1);
   MotorDC_Init(MOT_2);
   /********************************************* Move from speed 0-100 for 5 seconds ************************************/   
   /* start moving forward*/
   MotorDC_Dir(MOT_1,FORWARD);
   MotorDC_Dir(MOT_2,FORWARD);      
   uint8_t i =1;
   for(; i<=100;i++)  // step size = 100 / 5 where 100 represents 100 of full speed 5 is the 5 second
   {
      MotorDC_Speed_PollingWithT0(i);  
      softwareDelayMs(90);       
   }
   /********************************************* Move from speed 100-0 for 5 seconds ************************************/
   //Reinitialize i
   i =100;     
   for (;i>=1;i--)
   {
      MotorDC_Speed_PollingWithT0(i); 
      softwareDelayMs(90);        
   }   
   MotorDC_Dir(MOT_1,STOP);
   MotorDC_Dir(MOT_2,STOP);
   timer0Stop();
   /********************************************* Rotate 90 degree ******************************************************/
   MotorDC_Dir(MOT_1,FORWARD);
   MotorDC_Dir(MOT_2,BACKWARD);
   softwareDelayMs(350);
   MotorDC_Dir(MOT_1,STOP);
   MotorDC_Dir(MOT_2,STOP);
}

void testPwm()
{
   while (1)
   {
      uint8_t i =0;
      while(i<100)
      {
         timer0SwPWM(i,1);
         softwareDelayMs(500);
         i += 10;
      }
         
   }   
}

void test(void)
{
	/*initialize led*/
	Led_Init(LED_0);
   Led_Init(LED_1);
   Led_Init(LED_2);
   Led_Init(LED_3);   
	while(1)
	{
		/*Go state*/
		Led_On(LED_0);
		softwareDelayMs(100); //delay 1000 ms
		Led_Off(LED_0);	
		softwareDelayMs(100); //delay 1000 ms	
      Led_On(LED_1);
      softwareDelayMs(100); //delay 1000 ms
      Led_Off(LED_1);
      softwareDelayMs(100); //delay 1000 ms
      Led_On(LED_2);
      softwareDelayMs(100); //delay 1000 ms
      Led_Off(LED_2);
      softwareDelayMs(100); //delay 1000 ms
      Led_On(LED_3);
      softwareDelayMs(100); //delay 1000 ms
      Led_Off(LED_3);
      softwareDelayMs(100); //delay 1000 ms
      
	}
}


void gpioReq7(void)
{
	/*initialize segments*/
	sevenSegInit(SEG_0);
	sevenSegInit(SEG_1);
	while(1)
	{
		uint8_t counter = 0;
		for(;counter < 100;counter++)
		{
			sevenSegWrite(SEG_0,(counter % 10));
			softwareDelayMs(10);
			sevenSegWrite(SEG_1,(counter / 10));
			softwareDelayMs(10);
		}
	}	
}

void gpioReq9(void)
{
	/*initialize led*/
	Led_Init(LED_1);
	Led_Init(LED_2);
	Led_Init(LED_3);
	while(1)
	{
		/*Go state*/
		Led_On(LED_1);
		softwareDelayMs(1000);		
		Led_Off(LED_1);
		/*Stop state*/
		Led_On(LED_3);
		softwareDelayMs(1000);
		Led_Off(LED_3);
		/*Get Ready state*/
		Led_On(LED_2);
		softwareDelayMs(1000);
		Led_Off(LED_2);		
	}	
}

void gpioReq8(void)
{
	uint8_t u8_press_counter = 0;
	uint16_t u16_time_delay_acc = 0; /* time delay accumulator*/
	uint8_t u8_polling_delay = 10; /*time delay (in Milli seconds) value for polling switch button status*/
	//initialize led_1 and btn_1 (sets direction)
	Led_Init(LED_1);
	pushButtonInit(BTN_1);
	while (1)
	{
		/*checking switch state*/
		if(pushButtonGetStatus(BTN_1) == Pressed)
		{
			/*increment u8_press_counter*/
			u8_press_counter++;
			/*check u16_time_delay_acc*/
			if(u16_time_delay_acc > 1000)
			{
				/*reset u8_press_counter*/
				u8_press_counter = 0;
				/*reset u16_time_delay_acc*/
				u16_time_delay_acc = 0;
			}
			Led_On(LED_1);
			softwareDelayMs(1000);
			if(u8_press_counter == 2 && u16_time_delay_acc <= 1000) softwareDelayMs(1000);
			Led_Off(LED_1);
		}
		/*delay and add to accumulator*/
		softwareDelayMs(u8_polling_delay);
		u16_time_delay_acc += u8_polling_delay;
	}	
}

