
#include "Motors_Dashboard.h"
#include "Motors_Dashboard_Private.h"

extern void Motors_Dashboard_Init ()
{
	DIO_Init();
	ADC_Init(ADC_VCC, ADC_PRESCALER_8);
	ADC_Enable();
	TIMER0_Init(TIMER0_PRESCALER_8, TIMER0_PHASE_CORRECT_PWM, TIMER0_OC0_SET_ON_COMPARE_MATCH);
	TIMER1_Init(TIMER1_PRESCALER_8, TIMER1_PHASE_CORRECT_PWM_ICR1_TOP, TIMER1_OC1A_SET_ON_COMPARE_MATCH, TIMER1_OC1B_SET_ON_COMPARE_MATCH);
	LCD_Init();
}

extern void Motors_Dashboard_Run ()
{
	MOTOR1_Forward(POTONTIOMETER1 ());
	
	double servoAngle = (POTONTIOMETER2() * 180.0) / 100.0 - 90.0;
	LCD_GoTo(0, 0);
	LCD_WriteNumber_4Digit(POTONTIOMETER2());
	LCD_GoTo(1, 0);
	LCD_WriteNumber_4Digit((u16)servoAngle);
	SERVO_setAngle (servoAngle);
	
}
