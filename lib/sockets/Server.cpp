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
			if(error_state != Error::NONE)
			{
				perror("server Nonblock failed.");
				server_fd = -1;
			}
		}
    }
}


Server::~Server() {

}
void Server::Listen() {

	  int event_fd, s;
	  struct epoll_event event;
	  struct epoll_event events[MAX_EVENTS];

	if ( listen(server_fd, MAX_CONNECTIONS) != 0 )
	{
		perror("socket--listen");
		error_state = Error::LISTEN;
		server_fd = -1;
	}
	else
	{
		event_fd = epoll_create1 (0);
		if (event_fd == -1)
		{
			perror ("epoll_create");
			abort ();
		}

		event.data.fd = server_fd;
		event.events = EPOLLIN | EPOLLET;

		s = epoll_ctl (event_fd, EPOLL_CTL_ADD, server_fd, &event);
		if (s == -1)
		{
		  perror ("epoll_ctl");
		  abort ();
		}


		while (1)
		{
		      int n, i;

		      n = epoll_wait (event_fd, events, MAX_EVENTS, -1);
		    for (i = 0; i < n; i++)
			{
			  if (   (events[i].events & EPOLLERR) ||
		             (events[i].events & EPOLLHUP) ||
		           (!(events[i].events & EPOLLIN )  ))
			    {
		              /* An error has occured on this fd, or the socket is not
		                 ready for reading (why were we notified then?) */
			      fprintf (stderr, "epoll error\n");
			      close (events[i].data.fd);
			      continue;
			    }
			}
		}

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
