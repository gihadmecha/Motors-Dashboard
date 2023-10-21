
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

static u8 DcMotorSpeed = 0;
static u8 DcMotorSpeed_POTONTIOMETER_previous = 0;
static u8 DcMotorSpeed_POTONTIOMETER_present = 0;
static s16 DcMotorSpeed_keypad_previous = 0;
static s16 DcMotorSpeed_keypad_present = 0;
static s16 servoAngle = 0;
static s16 ServoAngle_POTONTIOMETER_previous = 0;
static s16 ServoAngle_POTONTIOMETER_present = 0;
static s16 ServoAngle_keypad_previous = 0;
static s16 ServoAngle_keypad_present = 0;
static s32 stepperAngle = 0;
static s32 Stepper_position = 0;

static u8 firstkey = 0;

static u8 value[6] = {0};
static u8 valueIndex = 0;

static s32 number = 0;

extern void Motors_Dashboard_Run ()
{
	Motors_Dashboard_keypadGetValue ();
	
	DcMotorSpeed_POTONTIOMETER_present = POTONTIOMETER_DcMotor ();
	if (DcMotorSpeed_POTONTIOMETER_present != DcMotorSpeed_POTONTIOMETER_previous)
	{
		DcMotorSpeed = DcMotorSpeed_POTONTIOMETER_present;
		DcMotorSpeed_POTONTIOMETER_previous = DcMotorSpeed_POTONTIOMETER_present;
	}
	
	if (DcMotorSpeed_keypad_present != DcMotorSpeed_keypad_previous)
	{
		if (DcMotorSpeed_keypad_present >= 0 && DcMotorSpeed_keypad_present <= 100)
		{
			DcMotorSpeed = DcMotorSpeed_keypad_present;
			DcMotorSpeed_keypad_previous = DcMotorSpeed_keypad_present;
		}
	}
	
	MOTOR1_Forward(DcMotorSpeed);
	
	if (firstkey != '1')
	{
		LCD_GoTo(1, 0);
		LCD_WriteNumber_4Digit((u16)DcMotorSpeed);
	}
	
	ServoAngle_POTONTIOMETER_present = POTONTIOMETER_Servo ();
	if (ServoAngle_POTONTIOMETER_present != ServoAngle_POTONTIOMETER_previous)
	{
		servoAngle = ServoAngle_POTONTIOMETER_present;
		ServoAngle_POTONTIOMETER_previous = ServoAngle_POTONTIOMETER_present;
		
		if (firstkey != '2')
		{
			LCD_GoToClear(1, 6, 4);
		}
	}
	
	if (ServoAngle_keypad_present != ServoAngle_keypad_previous)
	{
		if (ServoAngle_keypad_present >= -90 && ServoAngle_keypad_present <= 90)
		{
			servoAngle = ServoAngle_keypad_present;
			ServoAngle_keypad_previous = ServoAngle_keypad_present;
			
			LCD_GoToClear(1, 6, 4);
		}
	}

	SERVO_setAngle (servoAngle);
	
	if (firstkey != '2')
	{
		LCD_GoTo(1, 6);
		if (servoAngle >= 0)
		{
			LCD_WriteNumber_4Digit((u16)servoAngle);
		}
		else
		{
			LCD_WriteNumber((s32)servoAngle);
		}
	}
	
	
	//if (StepperAngle_keypad_present != 0)
	//{
		//stepperAngle = StepperAngle_keypad_present;
		STEPPER (stepperAngle); 
		Stepper_position += stepperAngle;
			
		//StepperAngle_keypad_present = 0;
		//LCD_GoToClear(1, 12, 4);
	//}
	
	if (firstkey != '3')
	{
		if (Stepper_position >= 0)
		{
			LCD_GoTo(1, 12);
			LCD_WriteNumber_4Digit((u16)Stepper_position);
		}
		else
		{
			LCD_GoTo(1, 12);
			LCD_WriteNumber((s32)Stepper_position);
		}
	}
	
	stepperAngle = 0;
}


static double POTONTIOMETER_Servo ()
{
	return (POTONTIOMETER2 () * 180.0) / 100.0 - 90.0;
}


static void Motors_Dashboard_keypadGetValue ()
{
	if (firstkey == 0)
	{
		firstkey = KEYPAD_GetKey();
		
		if ( firstkey == '1' || firstkey == '2' || firstkey == '3')
		{
			if (firstkey == '1')
			{
				LCD_GoToClear( 1, 0, 4);
			}
			else if (firstkey == '2')
			{
				LCD_GoToClear( 1, 6, 4);
			}
			else if (firstkey == '3')
			{
				LCD_GoToClear( 1, 12, 4);
			}
		}
	}
	else
	{
		u8 key;
		
		if (valueIndex <= 4 && value[valueIndex] != '=')
		{
			key = KEYPAD_GetKey();
			
			if (key != NULL)
			{
				value[valueIndex] = key;
				
				if ( valueIndex <= 3 &&  ( (value[valueIndex] >= '0' && value[valueIndex] <= '9') || (value[valueIndex] == '-') ) )
				{
					if (firstkey == '1')
					{
						LCD_GoTo(1, valueIndex);
					}
					else if (firstkey == '2')
					{
						LCD_GoTo(1, 6 + valueIndex);
					}
					else if (firstkey == '3')
					{
						LCD_GoTo(1, 12 + valueIndex);
					}
					
					LCD_WriteChar(value[valueIndex]);
					
					if (value[valueIndex] >= '0' && value[valueIndex] <= '9')
					{
						number = number * 10 + (value[valueIndex] - '0');
					}
				}
				
				if (value[valueIndex] != '=')
				{
					valueIndex++;
				}
			}
			
			if (key == 'C')
			{
				if (firstkey == '1')
				{
					LCD_GoToClear( 1, 0, 4);
				}
				else if (firstkey == '2')
				{
					LCD_GoToClear( 1, 6, 4);
				}
				else if (firstkey == '3')
				{
					LCD_GoToClear( 1, 12, 4);
				}
				
				value[1] = 0;
				value[2] = 0;
				value[3] = 0;
				value[4] = 0;
				valueIndex = 0;
				
				number = 0;
			}
			
			if (value[0] == 'C')
			{
				value[0] = 0;
				value[1] = 0;
				value[2] = 0;
				value[3] = 0;
				value[4] = 0;
				valueIndex = 0;
				
				
				if (firstkey == '1')
				{
					number = DcMotorSpeed;
					LCD_GoToClear(1, 0, 4);
				}
				else if (firstkey == '2')
				{
					number = servoAngle;
					LCD_GoToClear(1, 6, 4);
				}
				else if (firstkey == '3')
				{
					number = stepperAngle;
					LCD_GoToClear(1, 12, 4);
				}
				
				number = 0;
				
				firstkey = 0;
			}
			
			if (value[valueIndex] == '=')
			{
				if (value[0] == '-')
				{
					number = -number;
				}
				
				if (firstkey == '1')
				{
					DcMotorSpeed_keypad_present = number;
				}
				else if (firstkey == '2')
				{
					ServoAngle_keypad_present = number;
				}
				else if (firstkey == '3')
				{
					stepperAngle = number;
				}
				
				value[0] = 0;
				value[1] = 0;
				value[2] = 0;
				value[3] = 0;
				value[4] = 0;
				valueIndex = 0;
				
				number = 0;
				
				if (firstkey == '1')
				{
					LCD_GoToClear(1, 0, 4);
				}
				else if (firstkey == '2')
				{
					LCD_GoToClear(1, 6, 4);
				}
				else if (firstkey == '3')
				{
					LCD_GoToClear(1, 12, 4);
				}
				
				firstkey = 0;
			}
			
			if (valueIndex == 5)
			{
				value[1] = 0;
				value[2] = 0;
				value[3] = 0;
				value[4] = 0;
				valueIndex = 0;
				
				number = 0;
				
				if (firstkey == '1')
				{
					LCD_GoToClear(1, 0, 4);
				}
				else if (firstkey == '2')
				{
					LCD_GoToClear(1, 6, 4);
				}
				else if (firstkey == '3')
				{
					LCD_GoToClear(1, 12, 4);
				}
				
				firstkey = 0;
			}
		}
	}
}
