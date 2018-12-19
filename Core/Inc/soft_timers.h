/*
 * soft_timers.h
 *
 *  Created on: 19.12.2018
 *      Author: Jacek
 */

#ifndef INC_SOFT_TIMERS_H_
#define INC_SOFT_TIMERS_H_

#include "stdint.h"

#define HAL_TICK_MAX_TIMERS	6

typedef enum _tickTimerResult { tRC_OK = 0, tRC_ERR } tickTimerResult;
typedef enum _tickTimerState { tTS_STOP = 0, tTS_PENDING } tickTimerState;

typedef struct tickTimer
{
	uint32_t id;		// timer ID, for identification, if zero - timer is not used
	volatile uint32_t *pTimer;	// counter, decrement by 1 every 1 ms., if is greater than zero
} tickTimer_t;

class softTimers
{
public:
	softTimers();
	~softTimers();
	void update(void);
	tickTimerResult tickTimerAdd(const uint32_t id, volatile uint32_t *pTimer, uint32_t *pIndex);
	tickTimerResult tickTimerRemove(const uint32_t id, volatile uint32_t *pTimer, const uint32_t index);
	tickTimerResult tickTimerSet(const uint32_t id, const uint32_t index, uint32_t value);
	tickTimerResult tickTimerGet(const uint32_t id, const uint32_t index, uint32_t *value);
	tickTimerState tickTimerIsStopped(const uint32_t id, const uint32_t index);
	tickTimerState tickTimerInProgress(const uint32_t id, const uint32_t index);
private:
	volatile tickTimer_t tickTimers[HAL_TICK_MAX_TIMERS];

};

#endif /* INC_SOFT_TIMERS_H_ */
