/*
 * soft_timers.cpp
 *
 *  Created on: 19.12.2018
 *      Author: Jacek
 */

// *****************************************
// ***          Time Management          ***
// *****************************************

// *****************************************
// ***          Include section          ***
// *****************************************
#include "stm32f3xx_hal.h"
#include "string.h"
#include "soft_timers.h"

// *****************************************
// ***          Local definition         ***
// *****************************************
#define tickNULL (uint32_t*)(0)
#define TICK_TIMER_NOT_USED 0

softTimers::softTimers()
{
	// clear all data (id & timer content)
	HAL_SuspendTick();
	memset((tickTimer_t*)&tickTimers, 0x00, sizeof(tickTimers));
	HAL_ResumeTick();
}

softTimers::~softTimers()
{
	HAL_SuspendTick();
	memset((tickTimer_t*)&tickTimers, 0x00, sizeof(tickTimers));
	HAL_ResumeTick();
}

void softTimers::update(void)
{
	for( uint32_t index = 0; index < HAL_TICK_MAX_TIMERS; index++)
		if(( tickTimers[index].id != TICK_TIMER_NOT_USED ) && ( tickTimers[index].pTimer != tickNULL))
		{
			if( *(tickTimers[index].pTimer) )
				(*(tickTimers[index].pTimer))--;
		}
}

tickTimerResult softTimers::tickTimerAdd(const uint32_t id, volatile uint32_t *pTimer, uint32_t *pIndex)
{
	tickTimerResult rc = tRC_ERR;

	if(( id == 0 ) || ( pTimer == tickNULL ) || (pIndex == tickNULL)) return rc;

	for( uint32_t index = 0; index < HAL_TICK_MAX_TIMERS; index++)
	{
		if(( tickTimers[index].id == TICK_TIMER_NOT_USED ) && ( tickTimers[index].pTimer == tickNULL ))
		{
			HAL_SuspendTick();
			//
			tickTimers[index].id = id;
			tickTimers[index].pTimer = pTimer;
			*(tickTimers[index].pTimer) = 0;
			*pIndex = index;
			//
			HAL_ResumeTick();
			//
			rc = tRC_OK;
			break;
		}
	}

	return rc;
}

tickTimerResult softTimers::tickTimerRemove(const uint32_t id, volatile uint32_t *pTimer, const uint32_t index)
{
	tickTimerResult rc = tRC_ERR;

	if(( id == 0 ) || ( pTimer == tickNULL ) || !( index < HAL_TICK_MAX_TIMERS)) return rc;

	if(( tickTimers[index].id == id ) && ( tickTimers[index].pTimer == pTimer ))
	{
		HAL_SuspendTick();
		//
		tickTimers[index].id = 0;
		tickTimers[index].pTimer = tickNULL;
		//
		HAL_ResumeTick();
		//
		rc = tRC_OK;
	}

	return rc;
}

tickTimerResult softTimers::tickTimerSet(const uint32_t id, const uint32_t index, uint32_t value)
{
	tickTimerResult rc = tRC_ERR;

	if(( id == 0 ) || !(index < HAL_TICK_MAX_TIMERS)) return rc;

	if(( tickTimers[index].id == id) && (tickTimers[index].pTimer != tickNULL))
	{
		HAL_SuspendTick();
		//
		*(tickTimers[index].pTimer) = value;
		//
		HAL_ResumeTick();
		//
		rc = tRC_OK;
	}
	return rc;
}

tickTimerResult softTimers::tickTimerGet(const uint32_t id, const uint32_t index, uint32_t *value)
{
	tickTimerResult rc = tRC_ERR;

	if(( id == 0 ) || !(index < HAL_TICK_MAX_TIMERS) || (value == tickNULL)) return rc;

	if(( tickTimers[index].id == id) && (tickTimers[index].pTimer != tickNULL))
	{
		HAL_SuspendTick();
		//
		*value = *(tickTimers[index].pTimer);
		//
		HAL_ResumeTick();
		//
		rc = tRC_OK;
	}
	return rc;
}

tickTimerState softTimers::tickTimerIsStopped(const uint32_t id, const uint32_t index)
{
	if(( id == 0 ) || !(index < HAL_TICK_MAX_TIMERS)) return tTS_STOP;

	if(( tickTimers[index].id == id) && (tickTimers[index].pTimer != tickNULL))
	{
		HAL_SuspendTick();
		//
		uint32_t value = *(tickTimers[index].pTimer);
		//
		HAL_ResumeTick();
		//
		return (value == 0 ? tTS_STOP : tTS_PENDING);
	}

	return tTS_STOP;
}

tickTimerState softTimers::tickTimerInProgress(const uint32_t id, const uint32_t index)
{
	if(( id == 0 ) || !(index < HAL_TICK_MAX_TIMERS)) return tTS_STOP;

	if(( tickTimers[index].id == id) && (tickTimers[index].pTimer != tickNULL))
	{
		HAL_SuspendTick();
		//
		uint32_t value = *(tickTimers[index].pTimer);
		//
		HAL_ResumeTick();
		//
		return (value ? tTS_PENDING : tTS_STOP );
	}

	return tTS_STOP;
}
