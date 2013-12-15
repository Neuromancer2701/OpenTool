/*
 * Server.h
 *
 *  Created on: Nov 22, 2013
 *      Author: count zero
 */

#ifndef SERVER_H_
#define SERVER_H_

#include "Client.h"
#include "Timer.h"
#include <sys/epoll.h>

class ServerTimer : public Timer
{
public:
	ServerTimer():Timer(){};
	virtual ~ServerTimer(){};
	void TimerTask() {};
};

class Server {
public:
	Server(int _port);
	Server();
	int Available();
	void ServerLoop();
	virtual void Task();

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
	ServerTimer server_timer;
};



#endif /* SERVER_H_ */
