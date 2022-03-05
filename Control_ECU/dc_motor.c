/******************************************************************************************************
File Name	: dc_motor.c
Author		: Sherif Beshr
Description : header file for the DC Motor driver
*******************************************************************************************************/

#include "dc_motor.h"
#include "gpio.h"

/***************************************************************************************************
 *                                		Definitions                                  			   *
 ***************************************************************************************************/

/*
 * Description :
 * Initialize the DC Motor:
 * 1. Setup the DC Motor pins directions by using the GPIO driver.
 * 2. Initialize the DC Motor to STOP.
 */
void DcMotor_Init(void)
{
	GPIO_setupPinDirection(DC_MOTOR_PORT_ID, DC_MOTOR_PIN1_ID, PIN_OUTPUT);
	GPIO_setupPinDirection(DC_MOTOR_PORT_ID, DC_MOTOR_PIN2_ID, PIN_OUTPUT);
	GPIO_writePin(DC_MOTOR_PORT_ID, DC_MOTOR_PIN1_ID, LOGIC_LOW);
	GPIO_writePin(DC_MOTOR_PORT_ID, DC_MOTOR_PIN2_ID, LOGIC_LOW);
}

/*
 * Description :
 * Set the DC Motor State and speed:
 * 1. Set the Motor State ( STOP --> PIN1=0 & PIN2=0  ,  CW --> PIN1=0 & PIN2=1 ,  ACW --> PIN1=1 & PIN2=0 )
 * 2. Set the Motor Speed by passing duty cycle to the PWM function.
 * Input: State ( CW or ACW or STOP ) , Speed = duty cycle
 */
void DcMotor_Rotate(DcMotor_State state)
{
	if (state == STOP)
	{
		GPIO_writePin(DC_MOTOR_PORT_ID, DC_MOTOR_PIN1_ID, LOGIC_LOW);
		GPIO_writePin(DC_MOTOR_PORT_ID, DC_MOTOR_PIN2_ID, LOGIC_LOW);
	}
	else if (state == ACW)
	{
		GPIO_writePin(DC_MOTOR_PORT_ID, DC_MOTOR_PIN1_ID, LOGIC_LOW);
		GPIO_writePin(DC_MOTOR_PORT_ID, DC_MOTOR_PIN2_ID, LOGIC_HIGH);

	}
	else if (state == CW)
	{
		GPIO_writePin(DC_MOTOR_PORT_ID, DC_MOTOR_PIN1_ID, LOGIC_HIGH);
		GPIO_writePin(DC_MOTOR_PORT_ID, DC_MOTOR_PIN2_ID, LOGIC_LOW);
	}

}
