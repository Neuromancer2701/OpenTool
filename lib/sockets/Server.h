/*
 * Server.h
 *
 *  Created on: Nov 22, 2013
 *      Author: count zero
 */

#ifndef SERVER_H_
#define SERVER_H_

#include "Client.h"
#include <sys/epoll.h>

class Server {
public:
	Server(int _port);
	Server();
	int Available();
	virtual ~Server();



	static const int MAX_CONNECTIONS = 6;
	int active_connections;
	Client client_list[MAX_CONNECTIONS];
protected:
	int server_fd;				//File Descriptor for client connection
	int port;

	Connection_Status  status;
	Error			   error_state;
	Error setBlockingMode(Blocking_Mode mode);

private:
	static const int MAX_EVENTS = 16;
	int event_fd;
	struct epoll_event event;
	struct epoll_event events[MAX_EVENTS];

};

#endif /* SERVER_H_ */
