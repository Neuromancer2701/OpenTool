/*
 * Timer.h
 *
 *  Created on: Dec 10, 2013
 *      Author: root
 */

#ifndef TIMER_H_
#define TIMER_H_

#include <signal.h>
#include <time.h>

class Timer {
public:
	Timer(int seconds, int milliseconds, bool singleshot);
	virtual ~Timer();

	void start();

protected:
	virtual void TimerTask() {};


private:
	static void signal_handler(int sigNumb, siginfo_t *si, void *uc);

	timer_t timerID;
	sigset_t SigBlockSet;
	struct sigevent signalEvent;
	struct sigaction SignalAction;
	struct itimerspec timerSpecs;

	static const int MILLISECONDS_TO_NANOSECONDS = 1000000;
	static const int SECONDS_TO_MILLISECONDS     = 1000;

};

#endif /* TIMER_H_ */
