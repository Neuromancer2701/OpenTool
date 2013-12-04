/*
 * Server.h
 *
 *  Created on: Nov 22, 2013
 *      Author: count zero
 */

#ifndef SERVER_H_
#define SERVER_H_

#include "Client.h"

class Server {
public:
	Server(int _port);
	Server();
	void Listen();
	virtual ~Server();



	static const int MAX_CONNECTIONS = 6;
	int active_connections = 0;
	Client client_list[MAX_CONNECTIONS];
protected:
	int server_fd;				//File Descriptor for client connection
	int port;

	Connection_Status  status;
	Error			   error_state;
	Error setBlockingMode(Blocking_Mode mode);

private:
	static const int MAX_EVENTS = 16;

};

#endif /* SERVER_H_ */
