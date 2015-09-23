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

int main() {

	bool listening = true;
	cout << "This is a Test." << endl;
	Server Test_Server(12000);

	while(listening)
	{
		sleep(1);
		//Test.Available();

	}

	return 0;
}


