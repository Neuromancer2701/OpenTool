/*
 * Server.cpp
 *
 *  Created on: Nov 22, 2013
 *      Author: count zero
 */

#include "Server.h"

#include <sys/socket.h>
#include <resolv.h>
#include <arpa/inet.h>
#include <sys/unistd.h>
#include <sys/fcntl.h>
#include <sys/epoll.h>
#include <errno.h>
#include <cstdio>
#include <cstring>


#define debug 1

#if debug
#define DebPrint(fmt, args...) printf(fmt, ## args)
#endif



Server::Server() {

	port = 0;
}

Server::Server(int _port)	{

	server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if( server_fd < 0 )
	{
		perror("socket");
		error_state = Error::SOCKET;
	}
    else
    {
		struct sockaddr_in local_address;
		memset((char *)&local_address, 0, sizeof(local_address));
		local_address.sin_family = AF_INET;
		local_address.sin_addr.s_addr = htonl(INADDR_ANY);
		local_address.sin_port = htons(_port);

		if ( bind(server_fd, (struct sockaddr*)&local_address, sizeof(local_address)) != 0 )
		{
			perror("socket--bind");
			error_state = Error::BIND;
			server_fd = -1;
		}
		else
		{
			error_state = setBlockingMode(Blocking_Mode::NonBlock);
			active_connections = 0;
			if(error_state != Error::NONE)
			{
				perror("server Nonblock failed.");
				server_fd = -1;
			}
			else
			{
				DebPrint("Created server object, bind, setblocking mode, Server FD: %d\n ",server_fd);
			}
		}
    }
}


Server::~Server() {

	if(server_fd > 0)
		close(server_fd);

	for(int i = 0;i < MAX_CONNECTIONS; i++)
	{
		client_list[i].Disconnect();
	}
}
void Server::Listen() {

	  int event_fd, epoll_status;
	  struct epoll_event event;
	  struct epoll_event events[MAX_EVENTS];

	  if(error_state == Error::NONE)
	  {
		if ( listen(server_fd, MAX_CONNECTIONS) != 0 )
		{
			perror("socket--listen");
			error_state = Error::LISTEN;
			server_fd = -1;
		}
		else
		{
			DebPrint("Listening, Server FD: %d\n ",server_fd);
			event_fd = epoll_create1 (0);
			if (event_fd == -1)
			{
				perror ("epoll_create");
				error_state = Error::EPOLL_CREATE;
				server_fd = -1;
				return;
			}

			event.data.fd = server_fd;
			event.events = EPOLLIN | EPOLLET;

			epoll_status = epoll_ctl (event_fd, EPOLL_CTL_ADD, server_fd, &event);
			if (epoll_status == -1)
			{
				perror ("epoll_create");
				error_state = Error::EPOLL_CTL;
				server_fd = -1;
				return;
			}

			DebPrint("Waiting for Epoll, Server FD: %d\n ",server_fd);
			int epoll_events = epoll_wait (event_fd, events, MAX_EVENTS, -1);
			DebPrint("Epoll events, %d\n ",epoll_events);

			for (int i = 0; i < epoll_events; i++)
			{
				if((events[i].events & EPOLLERR) || (events[i].events & EPOLLHUP) || (!(events[i].events & EPOLLIN ))) /*Epoll error occured*/
				{
				  perror("epoll wait error\n");
				  error_state = Error::EPOLL_WAIT;
				  close (events[i].data.fd);

				}
				else if(server_fd == events[i].data.fd)
				{
					if(active_connections >= MAX_CONNECTIONS)
					{
						break;
					}

					while (error_state != Error::ACCEPT_DONE)
					{
						DebPrint("Accept\n ");
						error_state = client_list[active_connections].Accept(server_fd);
						if(error_state == Error::NONE)
						{
							active_connections++;
						}
					}
				}
				else
				{
					perror ("Unknown epoll event");
					error_state = Error::EPOLL_UNKNOWN;
				}
			}
		}
	  }
	  else
	  {
		  perror ("Error unable to listen");
	  }

}

Error Server::setBlockingMode(Blocking_Mode mode)
{
	error_state = Error::UNKNOWN;
	int old_flags = fcntl(server_fd,F_GETFL, 0);

	if(old_flags < 0 )
	{
		perror("failed to read old flags.");
		error_state = Error::FCNTL_GET;
	}
	else
	{
		if(mode == Blocking_Mode::NonBlock)
		{
			old_flags |= O_NONBLOCK;
		}
		else
		{
			old_flags &= ~O_NONBLOCK;
		}

		old_flags = fcntl(server_fd,F_SETFL, old_flags);
		if(old_flags < 0 )
		{
			perror("failed to set non-block flags.");
			error_state = Error::FCNTL_SET;
		}
		else
		{
			error_state = Error::NONE;
		}
	}

	return error_state;
}
