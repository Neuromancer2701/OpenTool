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
	Server Test(12000);

	//TestTimer test_timer(20,0);

	//test_timer.start();
	while(listening)
	{
		sleep(1);
		Test.Available();
		for(int i = 0;i < Test.active_connections; i++)
		{
			read_data.clear();
			if(Test.client_list[i].EventRead(read_data) == Error::EPOLL_CLOSE)
			{
				cout << "Client Closed: " << i << endl;
				Test.client_list[i].Disconnect();
				Test.active_connections--;
			}
			else
			{
				string str(read_data.begin(),read_data.end());
				cout << "Read Data:"<< str << "from client number " << i << endl;
			}

		}
	}

	return 0;
}


