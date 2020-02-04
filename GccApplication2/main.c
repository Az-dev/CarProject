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

volatile uint8_t * gu_sw_icu = NULL;
volatile uint8_t sw_icu_reset = LOW;



void gpioReq7(void);
void gpioReq8(void);
void gpioReq9(void);
void test(void);
void testPwm(void);
void car(void);
void swIcuTest(void);





int main(void)
{ 
	/* gpioReq7();*/ 
	/* gpioReq8(); */
	/* gpioReq9(); */
	//test(); 
   /*testPwm();*/
   /*car();*/
   swIcuTest();
}

void swIcuTest()
{
   
   /************************************** Intialization *****************************************/
   /*Initialize SWI_CU*/ 
   SwICU_Init(SwICU_EdgeRisiging);  
   /*initialize led*/
   Led_Init(LED_0);
   Led_Init(LED_1);
   Led_Init(LED_2);
   Led_Init(LED_3);
   /*Initialize triggerPins*/
   gpioPinDirection(ULTRA_EN_GPIO,ULTRA_ENABLE_BIT,OUTPUT); /*triggering pin*/
   gpioPinDirection(ULTRA_OUT_GPIO,ULTRA_OUT_BIT,INPUT);
   /**************************************** Interrupts enable **************************************/
   /*set sei()*/
   sei();
   /*Enable INT2*/
   SwICU_Enable();
   /************************************** Start timer and trigger ICU *************************************/
   /*trigger SW_ICU*/
   /*provide a 10-micro seconds pulse*/
   gpioPinWrite(ULTRA_EN_GPIO,ULTRA_ENABLE_BIT,HIGH);
   //_delay_us(50);
   timer0DelayMs(1);   
   gpioPinWrite(ULTRA_EN_GPIO,ULTRA_ENABLE_BIT,LOW);
   /*Start Input Capture*/
   //SwICU_Start();         
   while (1)
   {           
      if(sw_icu_reset == HIGH)
      {
         /*calculate distance and turn led on*/
         //uint8_t distance = (68*(*(gu_sw_icu)))/1000;
         //(((68*(*(gu_sw_icu)))/1000)<<4)
         //PORTB
         gpioPortWrite(GPIOB,(68*(*gu_sw_icu)/1000)<<4);
         /*Delay*/
         timer0DelayMs(1000);
         /*trigger SW_ICU*/
         /*provide a 10-micro seconds pulse*/
         gpioPinWrite(ULTRA_EN_GPIO,ULTRA_ENABLE_BIT,HIGH);
         timer0DelayMs(1);
         gpioPinWrite(ULTRA_EN_GPIO,ULTRA_ENABLE_BIT,LOW);
         /*Start Input Capture again*/
         SwICU_Start();         
         /*set reset flag to low*/
         sw_icu_reset = LOW;    
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

