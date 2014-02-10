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
#include <netdb.h>
#include <errno.h>
#include <cstdio>
#include <cstring>


#ifndef SO_REUSEPORT
#define SO_REUSEPORT	15
#endif

#define debug 1

#if debug          // Make non-zero for debug output
#define DEBUG(fmt, args...) printf(fmt, ## args)
#else
#define DEBUG(fmt, args...)
#endif



Server::Server() {

	port = 0;
}
Server::Server(int _port)	{

	active_connections = 0;
	server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if( server_fd < 0 )
	{
		perror("socket");
		error_state = Error::SOCKET;
	}
    else
    {
	    int optval = 1;
		struct sockaddr_in local_address;
		memset((char *)&local_address, 0, sizeof(local_address));
		local_address.sin_family = AF_INET;
		local_address.sin_addr.s_addr = htonl(INADDR_ANY);
		local_address.sin_port = htons(_port);

	    setsockopt(server_fd, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(optval));

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
				if ( listen(server_fd, MAX_CONNECTIONS) != 0 )
				{
					perror("socket--listen");
					error_state = Error::LISTEN;
					server_fd = -1;
				}
				else
				{
					int epoll_status;
					event_fd = epoll_create1 (0);
					if (event_fd == -1)
					{
						perror ("epoll_create");
						error_state = Error::EPOLL_CREATE;
						server_fd = -1;
					}
					else
					{
						event.data.fd = server_fd;
						event.events = EPOLLIN;

						epoll_status = epoll_ctl (event_fd, EPOLL_CTL_ADD, server_fd, &event);
						if (epoll_status == -1)
						{
							perror ("epoll_create");
							error_state = Error::EPOLL_CTL;
							server_fd = -1;
						}
						else
						{
							//DEBUG("Created server object, bind, setblocking mode, Listening, Server FD: %d\n ",server_fd);
							char port_buffer[32];
							char ip_buffer[32];

							socklen_t len = sizeof( local_address);
							int nameinfo = getnameinfo ((const sockaddr *)&local_address, len, ip_buffer, sizeof(ip_buffer), port_buffer, sizeof(port_buffer), NI_NUMERICHOST | NI_NUMERICSERV);
							if (nameinfo == 0)
							{
								string ip_address = string(ip_buffer);
								port = atoi(port_buffer);
								DEBUG("Server Ip:%s:%d \n ",ip_address.c_str(), port);
							}
						}
					}
				}
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
int Server::Available() {

	Client client;

	if(error_state == Error::NONE)
	{
		int epoll_events = epoll_wait (event_fd, events, MAX_EVENTS, -1);
		DEBUG("Epoll events, %d\n ",epoll_events);

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
					error_state = client_list[active_connections].Accept(server_fd);
					if(error_state == Error::NONE)
					{
						AddClientToEvent(client_list[active_connections]);
						active_connections++;
					}
				}
				error_state = Error::NONE;
			}
			else if(isClient(events[i].data.fd, client))
			{
				client.Read(read_buffer);
			}
			else
			{
				perror ("Unknown epoll event");
				error_state = Error::EPOLL_UNKNOWN;
			}
		}
	}
	else
	{
	  perror ("Setup did not work so can't wait for a connection.");
	}

	DEBUG("Active Connections:%d and Error State:%d\n", active_connections, error_state);
	return active_connections;
}
Error Server::AddClientToEvent(Client client)
{
	error_state = Error::UNKNOWN;
	int epoll_status;
	struct epoll_event event;

	event.data.fd = client.get_fd();
	event.events = EPOLLIN | EPOLLET | EPOLLRDHUP;

	epoll_status = epoll_ctl(event_fd, EPOLL_CTL_ADD, event.data.fd, &event);

	if (epoll_status == -1)
	{
		perror ("epoll_create");
		error_state = Error::EPOLL_CTL;
	}
	else
	{
		error_state = Error::NONE;
	}

	return error_state;
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

bool Server::isClient(int fd, Client& client)
{
	bool found = false;
	for(auto local:client_list)
	{
		if(local.get_fd() == fd)
		{
			client = local;
			found = true;
		}
	}

	return found;
}

