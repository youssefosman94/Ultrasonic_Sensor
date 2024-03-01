/******************************************************************************
 *
 * Module: Ultrasonic Sensor
 *
 * File Name: ultrasonic_sensor.c
 *
 * Description: Source file for the Ultrasonic Sensor driver
 *
 * Author: Youssef Osman
 *
 *******************************************************************************/

#include"ultrasonic.h"
#include"gpio.h"
#include"icu.h"
#include"util/delay.h"

uint8 g_edgeCount = 0;
uint16 TIMER_VALUE = 0; /* the time required by the sensor to detect an object and return back from an object*/
/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/
void Ultrasonic_init(void) {
	/*setup Icu to with f_cpu_clock and to detect Rising Edge initially*/
	ICU_ConfigType Icu_Config = { F_CPU_8, RAISING };
	ICU_init(&Icu_Config);
	/*CallBack function Becasue Icu is in lower layer than Ultrasonic*/
	ICU_setCallBack(Ultrasonic_edgeProcessing);
	GPIO_setupPinDirection(TRIGGER_PORT, TRIGGER_PIN, PIN_OUTPUT);
	GPIO_writePin(TRIGGER_PORT, TRIGGER_PIN, LOGIC_LOW);
}
void Ultrasonic_Trigger(void) {
	GPIO_writePin(TRIGGER_PORT, TRIGGER_PIN, LOGIC_HIGH);
	_delay_us(10); /*Min Trigger pulse 10 micro Seconds */
	GPIO_writePin(TRIGGER_PORT, TRIGGER_PIN, LOGIC_LOW);
}

uint16 Ultrasonic_readDistance(void) {
	Ultrasonic_Trigger();
	return (((TIMER_VALUE) / 58.8));
}
void Ultrasonic_edgeProcessing(void) {
	g_edgeCount++;
	if (g_edgeCount == 1) {
		ICU_clearTimerValue();
		ICU_setEdgeDetectionType(FALLING);
	}
	else if (g_edgeCount == 2) {
		TIMER_VALUE = ICU_getInputCaptureValue();
		ICU_clearTimerValue();
		ICU_setEdgeDetectionType(RAISING);
		g_edgeCount = 0;
	}
}

