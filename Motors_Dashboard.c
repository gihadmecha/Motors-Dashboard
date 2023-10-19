
#include "Motors_Dashboard.h"
#include "Motors_Dashboard_Private.h"

extern void Motors_Dashboard_Init ()
{
	DIO_Init();
	ADC_Init(ADC_VCC, ADC_PRESCALER_8);
	ADC_Enable();
	TIMER1_Init(TIMER1_PRESCALER_8, TIMER1_PHASE_CORRECT_PWM_ICR1_TOP, TIMER1_OC1A_SET_ON_COMPARE_MATCH, TIMER1_OC1B_SET_ON_COMPARE_MATCH);
	MOTOR1_Init();
	LCD_Init();
}

extern void Motors_Dashboard_Run ()
{
	MOTOR1_Forward(POTONTIOMETER1 ());
	SERVO_setAngle (POTONTIOMETER2 ());
}
