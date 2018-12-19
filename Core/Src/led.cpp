/*
 * led.cpp
 *
 *  Created on: 19.12.2018
 *      Author: Jacek
 */

#include "soft_timers.h"
#include "led.h"
#include "main.h"

extern softTimers sTimers;

infoLed::infoLed():
	run(false)
{
	sTimers.tickTimerAdd(timerId, &timer, &timerIndex);
}

void infoLed::task(void)
{
	if( run )
	{
		if( sTimers.tickTimerIsStopped(timerId, timerIndex) == tTS_STOP )
		{
			sTimers.tickTimerSet(timerId,timerIndex, 500);
			HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
		}
	}
}

void infoLed::start(void)
{
	run = true;
}
