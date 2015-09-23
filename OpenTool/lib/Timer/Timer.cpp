/*
 * Timer.c
 *
 *  Created on: Dec 10, 2013
 *      Author: root
 */

#include "Timer.h"
#include <cstdlib>
#include <cstdio>
#include <cstring>

Timer::Timer(int _seconds, int _milliseconds, bool singleshot)
{
	timerSpecs.it_value.tv_sec = seconds = _seconds;
	milliseconds =_milliseconds;
	timerSpecs.it_value.tv_nsec = MILLISECONDS_TO_NANOSECONDS * milliseconds;

	if(singleshot)
	{
		timerSpecs.it_interval.tv_sec = 0;
		timerSpecs.it_interval.tv_nsec = 0;
	}
	else
	{
		timerSpecs.it_interval.tv_sec = timerSpecs.it_value.tv_sec;
		timerSpecs.it_interval.tv_nsec = timerSpecs.it_value.tv_nsec;
	}

	sigemptyset(&SignalAction.sa_mask);
	SignalAction.sa_flags = SA_SIGINFO;
	SignalAction.sa_sigaction = Timer::signal_handler;
	memset(&signalEvent, 0, sizeof(signalEvent));
	signalEvent.sigev_notify = SIGEV_SIGNAL;
	signalEvent.sigev_value.sival_ptr = (void*) this;
	signalEvent.sigev_signo = SIGALRM;


	if (timer_create(CLOCK_REALTIME, &signalEvent, &timerID)!= 0)
	{
		perror("Could not creat the timer");
	}

	if (sigaction(SIGALRM, &SignalAction, NULL))
	{
		perror("Could not install new signal handler");
	}
}


Timer::Timer()
{

}

Timer::~Timer()
{

}

void Timer::init(int seconds, int milliseconds, bool singleshot)
{
	timerSpecs.it_value.tv_sec = seconds;
	timerSpecs.it_value.tv_nsec = MILLISECONDS_TO_NANOSECONDS * milliseconds;

	if(singleshot)
	{
		timerSpecs.it_interval.tv_sec = 0;
		timerSpecs.it_interval.tv_nsec = 0;
	}
	else
	{
		timerSpecs.it_interval.tv_sec = timerSpecs.it_value.tv_sec;
		timerSpecs.it_interval.tv_nsec = timerSpecs.it_value.tv_nsec;
	}

	sigemptyset(&SignalAction.sa_mask);
	SignalAction.sa_flags = SA_SIGINFO;
	SignalAction.sa_sigaction = Timer::signal_handler;
	memset(&signalEvent, 0, sizeof(signalEvent));
	signalEvent.sigev_notify = SIGEV_SIGNAL;
	signalEvent.sigev_value.sival_ptr = (void*) this;
	signalEvent.sigev_signo = SIGALRM;


	if (timer_create(CLOCK_REALTIME, &signalEvent, &timerID)!= 0)
	{
		perror("Could not creat the timer");
	}

	if (sigaction(SIGALRM, &SignalAction, NULL))
	{
		perror("Could not install new signal handler");
	}
}
void Timer::start()
{
	if (timer_settime(this->timerID, 0, &this->timerSpecs, NULL) == -1)
	{
		perror("Could not start timer:");
	}
}
void Timer::reset()
{
	init( seconds, milliseconds, false);
	start();
}


void Timer::signal_handler(int sigNumb, siginfo_t *si, void *uc)
{
	Timer* LocalTimer = reinterpret_cast<Timer *> (si->si_value.sival_ptr);
	LocalTimer->TimerTask();
}
