/*
 * OpenTool.h
 *
 *  Created on: Dec 10, 2013
 *      Author: root
 */

#ifndef OPENTOOL_H_
#define OPENTOOL_H_

#include "Client.h"

class OpenTool {
public:
	OpenTool();
	virtual ~OpenTool();

	Error Connect();
	Error Listen();

	bool isTimedOut();
	bool RetriesReached();

	Connection_Type type;

	string ip_address;
	int port;

	int timeout;
	int retries;

private:
	Client client;
	Server server;


};

#endif /* OPENTOOL_H_ */
