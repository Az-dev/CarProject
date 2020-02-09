/*
 * dcMotor.c
 *
 * Created: 1/27/2020 4:53:40 PM
 *  Author: Az
 */ 
#include "dcMotor.h"
#include "dcMotorConfig.h"
#include "timers.h"

/**
 * Description: 
 * @param
 */
void MotorDC_Init(En_motorType_t en_motor_number)
{
   switch (en_motor_number)
   {
      case MOT_1:
         /*set directions of motor 1 enable bit*/
         gpioPinDirection(MOTOR_EN_1_GPIO,MOTOR_EN_1_BIT,OUTPUT);
         /*set direction of motor terminal "A" */
         gpioPinDirection(MOTOR_OUT_1A_GPIO,MOTOR_OUT_1A_BIT,OUTPUT);
         /*set direction of motor terminal "B" */
         gpioPinDirection(MOTOR_OUT_1B_GPIO,MOTOR_OUT_1B_BIT,OUTPUT);         
      break;
      case MOT_2:
         /*set directions of motor 2 enable bit*/
         gpioPinDirection(MOTOR_EN_2_GPIO,MOTOR_EN_2_BIT,OUTPUT);
         /*set direction of motor terminal "A" */
         gpioPinDirection(MOTOR_OUT_2A_GPIO,MOTOR_OUT_2A_BIT,OUTPUT);
         /*set direction of motor terminal "B" */
         gpioPinDirection(MOTOR_OUT_2B_GPIO,MOTOR_OUT_2B_BIT,OUTPUT);
      break;
   }
}

/**
 * Description: 
 * @param 
 * @param 
 */
void MotorDC_Dir(En_motorType_t en_motor_number, En_motorDir_t en_motor_dir)
{
   switch(en_motor_number)
   {
      case MOT_1:
         switch(en_motor_dir)
         {
            case STOP:
               /*write 0 on MOTOR_EN_1_BIT*/
               gpioPinWrite(MOTOR_EN_1_GPIO,MOTOR_EN_1_BIT,LOW);               
               /*write on terminals "A" & "B" of MOT_1 : 0 0*/
               gpioPinWrite(MOTOR_OUT_1A_GPIO,MOTOR_OUT_1A_BIT,LOW);
               gpioPinWrite(MOTOR_OUT_1B_GPIO,MOTOR_OUT_1B_BIT,LOW);
            break;
            case FORWARD:
               /*write 1 on MOTOR_EN_1_BIT*/
               gpioPinWrite(MOTOR_EN_1_GPIO,MOTOR_EN_1_BIT,HIGH);
               /*write on terminals "A" & "B" of MOT_1 : 0 1*/
               gpioPinWrite(MOTOR_OUT_1A_GPIO,MOTOR_OUT_1A_BIT,LOW);
               gpioPinWrite(MOTOR_OUT_1B_GPIO,MOTOR_OUT_1B_BIT,HIGH);               
            break;
            case BACKWARD:
               /*write 1 on MOTOR_EN_1_BIT*/
               gpioPinWrite(MOTOR_EN_1_GPIO,MOTOR_EN_1_BIT,HIGH);
               /*write on terminals "A" & "B" of MOT_1 : 1 0*/
               gpioPinWrite(MOTOR_OUT_1A_GPIO,MOTOR_OUT_1A_BIT,HIGH);
               gpioPinWrite(MOTOR_OUT_1B_GPIO,MOTOR_OUT_1B_BIT,LOW);
            break;
         }      
      break;
      case MOT_2:
         switch(en_motor_dir)
         {
            case STOP:
               /*write 0 on MOTOR_EN_2_BIT*/
               gpioPinWrite(MOTOR_EN_2_GPIO,MOTOR_EN_2_BIT,LOW);
               /*write on terminals "A" & "B" of MOT_2 : 0 0*/
               gpioPinWrite(MOTOR_OUT_2A_GPIO,MOTOR_OUT_2A_BIT,LOW);
               gpioPinWrite(MOTOR_OUT_2B_GPIO,MOTOR_OUT_2B_BIT,LOW);
            break;
            case FORWARD:
               /*write 1 on MOTOR_EN_2_BIT*/
               gpioPinWrite(MOTOR_EN_2_GPIO,MOTOR_EN_2_BIT,HIGH);
               /*write on terminals "A" & "B" of MOT_2 : 0 1*/
               gpioPinWrite(MOTOR_OUT_2A_GPIO,MOTOR_OUT_2A_BIT,LOW);
               gpioPinWrite(MOTOR_OUT_2B_GPIO,MOTOR_OUT_2B_BIT,HIGH);
            break;
            case BACKWARD:
               /*write 1 on MOTOR_EN_2_BIT*/
               gpioPinWrite(MOTOR_EN_2_GPIO,MOTOR_EN_2_BIT,HIGH);
               /*write on terminals "A" & "B" of MOT_2 : 1 0*/
               gpioPinWrite(MOTOR_OUT_2A_GPIO,MOTOR_OUT_2A_BIT,HIGH);
               gpioPinWrite(MOTOR_OUT_2B_GPIO,MOTOR_OUT_2B_BIT,LOW);
            break;
         }
      break;
   }
}

/**
 * Description: 
 * @param 
 */
void MotorDC_Speed_PollingWithT0(uint8_t u8_motor_speed)
{
   
   timer0SwPWM(u8_motor_speed,1); // frequency is 250
}

/**
 * Description: set the port value (which is PORT register)
 * @param 
 */
void MotorDC_Speed_HwPWM(uint8_t u8_motor_speed)
{
   
   timer1HwPWM(u8_motor_speed); // frequency is 250
}