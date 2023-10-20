
#include "Motors_Dashboard.h"
#include "Motors_Dashboard_Private.h"

#define		POTONTIOMETER_DcMotor		POTONTIOMETER1
#define		MOTOR_Forward				MOTOR1_Forward
#define		MOTOR_Backward				MOTOR1_backward
#define		STEPPER						STEPPER3

extern void Motors_Dashboard_Init ()
{
	DIO_Init();
	ADC_Init(ADC_VCC, ADC_PRESCALER_8);
	ADC_Enable();
	TIMER0_Init(TIMER0_PRESCALER_8, TIMER0_PHASE_CORRECT_PWM, TIMER0_OC0_SET_ON_COMPARE_MATCH);
	TIMER1_Init(TIMER1_PRESCALER_8, TIMER1_FAST_PWM_ICR1_TOP, TIMER1_OC1A_CLEAR_ON_COMPARE_MATCH, TIMER1_OC1B_DISCONNECTED);
	LCD_Init();
	
	LCD_GoTo(0, 0);
	LCD_WriteString("DC.");
	LCD_GoTo(0, 6);
	LCD_WriteString("SER.");
	LCD_GoTo(0, 12);
	LCD_WriteString("STE.");
}

extern void Motors_Dashboard_Run ()
{
	u8 DcMotorSpeed = POTONTIOMETER_DcMotor ();
	MOTOR1_Forward(DcMotorSpeed);
	LCD_GoTo(1, 0);
	LCD_WriteNumber_4Digit((u16)DcMotorSpeed);
	
	double servoAngle = POTONTIOMETER_Servo ();
	SERVO_setAngle (servoAngle);
	LCD_GoTo(1, 6);
	LCD_WriteNumber_4Digit((u16)servoAngle);
	
	u16 stepperAngle = 30;
	STEPPER (stepperAngle);
	LCD_GoTo(1, 12);
	LCD_WriteNumber_4Digit((u16)stepperAngle);
}

static double POTONTIOMETER_Servo ()
{
	return (POTONTIOMETER2 () * 180.0) / 100.0 - 90.0;
}
