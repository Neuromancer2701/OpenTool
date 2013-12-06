//============================================================================
// Name        : OpenToolTest.cpp
// Author      : 
// Version     :
// Copyright   : 
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "Server.h"
#include <unistd.h>
#include <iostream>

using namespace std;

int main() {

	bool listening = true;
	vector<char>  read_data;

	cout << "This is a Test." << endl;
	Server Test(15005);

	while(listening)
	{
		sleep(1);
		Test.Available();
		for(int i = 0;i < Test.active_connections; i++)
		{
			Test.client_list[i].EventRead(read_data);
			string str(read_data.begin(),read_data.end());
			cout << "Read Data:"<< str << "from client number " << i << endl;
		}
	}

	return 0;
}


