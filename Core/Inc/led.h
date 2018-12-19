/*
 * led.h
 *
 *  Created on: 19.12.2018
 *      Author: Jacek
 */

#ifndef INC_LED_H_
#define INC_LED_H_

class infoLed
{
public:
	infoLed();
	void start(void);
	void task(void);
private:
	volatile uint32_t timer;
	uint32_t timerIndex;
	static const uint32_t timerId = 0x4c454454; // "LEDT" -> 4C 45 44 54
	bool run;
};

#endif /* INC_LED_H_ */
