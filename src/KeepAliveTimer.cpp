/*
 * KeepAliveTimer.cpp
 *
 *  Created on: Feb 9, 2014
 *      Author: root
 */

#include <KeepAliveTimer.h>

KeepAliveTimer::KeepAliveTimer()
{
	seconds = milliseconds =  0;
}
KeepAliveTimer::KeepAliveTimer(int seconds, int milliseconds)
{
		Timer(seconds, milliseconds, false);
		timer_counter = 0;
}

KeepAliveTimer::~KeepAliveTimer()
{
	// TODO Auto-generated destructor stub
}

void KeepAliveTimer::TimerTask()
{
	timer_counter++;
}


