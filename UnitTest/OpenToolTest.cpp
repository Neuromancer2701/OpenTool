//============================================================================
// Name        : OpenToolTest.cpp
// Author      : 
// Version     :
// Copyright   : 
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "Server.h"
#include "Timer.h"
#include <unistd.h>
#include <iostream>

using namespace std;


class TestTimer : public Timer
{
public:
	TestTimer(int seconds, int milliseconds):Timer(seconds, milliseconds, false){};
	virtual ~TestTimer(){};
	void TimerTask() {cout << "Woot Timer Task went off!!!" << endl;};
};

int main() {

	bool listening = true;
	vector<char>  read_data;

	cout << "This is a Test." << endl;
	Server Test_Server(12000);

	while(listening)
	{
		sleep(1);
		//Test.Available();
		//for(int i = 0;i < Test.active_connections; i++)
		//{

		//}
	}

	return 0;
}


