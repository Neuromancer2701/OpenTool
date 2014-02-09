/*
 * KeepAliveTimer.h
 *
 *  Created on: Feb 9, 2014
 *      Author: root
 */

#ifndef KEEPALIVETIMER_H_
#define KEEPALIVETIMER_H_

#include <Timer.h>


class KeepAliveTimer: public Timer
{
public:
	KeepAliveTimer();
	KeepAliveTimer(int seconds, int milliseconds);
	virtual ~KeepAliveTimer();

	void TimerTask();
	int GetCount(){ return timer_counter;}


private:
	volatile int timer_counter;

};

#endif /* KEEPALIVETIMER_H_ */
