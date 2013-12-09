/*
 * Client.cpp
 *
 *  Created on: Nov 22, 2013
 *      Author: count zero
 */

#include "Client.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/unistd.h>
#include <sys/epoll.h>
#include <sys/fcntl.h>
#include <errno.h>
#include <cstdlib>
#include <cstdio>
#include <cstring>

#define debug 1

#if debug          // Make non-zero for debug output
#define DEBUG(fmt, args...) printf(fmt, ## args)
#else
#define DEBUG(fmt, args...)
#endif

Client::Client(string _ip_address,unsigned short _port) {

	ip_address = _ip_address;
	port = _port;
	client_fd = -1;
	status = Connection_Status::UNKNOWN;
	retries = 0;
}
Client::Client() {

	ip_address = "";
	port = 0;
	client_fd = -1;
	status = Connection_Status::UNKNOWN;
	retries = 0;
}
void Client::init(string _ip_address,unsigned short _port){
	ip_address = _ip_address;
	port = _port;
}

Error Client::Accept(int _server_fd){
	struct sockaddr in_addr;
	socklen_t in_len;
	int local_client_fd;
	char port_buffer[32];
	char ip_buffer[32];
	Error error_state = Error::NONE;

	DEBUG("Accepting, Server FD: %d\n ",_server_fd);
	in_len = sizeof( in_addr);
	local_client_fd = accept (_server_fd, &in_addr, &in_len);
	if (local_client_fd == -1)
	{
		if ((errno == EAGAIN) || (errno == EWOULDBLOCK))  /* We have processed all incoming connections. */
		{
			status = Connection_Status::UNKNOWN;
			error_state = Error::ACCEPT_DONE;
			client_fd = -1;
		}
		else
		{
			perror ("accept");
			status = Connection_Status::DISCONNECTED;
			error_state = Error::ACCEPT;
			client_fd = -1;
		}
	}
	else
	{
		client_fd = local_client_fd;
		status = Connection_Status::CONNECTED;
		retries = 0;

		DEBUG("Accepted connections, client FD: %d\n ",client_fd);

        int nameinfo = getnameinfo (&in_addr, in_len, ip_buffer, sizeof(ip_buffer), port_buffer, sizeof(port_buffer), NI_NUMERICHOST | NI_NUMERICSERV);
		if (nameinfo == 0)
		{
			ip_address = string(ip_buffer);
			port = atoi(port_buffer);
			DEBUG("Client Ip:%s:%d \n ",ip_address.c_str(), port);
		}

		if(setBlockingMode(Blocking_Mode::NonBlock) != Error::NONE)
		{
			perror("Nonblock failed.");
			status = Connection_Status::DISCONNECTED;
		}
	}

	return error_state;
}

Client::~Client() {

	if(client_fd > 0)
		close(client_fd);
}
Connection_Status Client::Connect()
{

	struct sockaddr_in local_address;
	memset((char *)&local_address, 0, sizeof(local_address));
	local_address.sin_family = AF_INET;
	local_address.sin_addr.s_addr = htonl(INADDR_ANY);
	local_address.sin_port = htons(0);

	status = Connection_Status::UNKNOWN;
	client_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (client_fd < 0 )
	{
		perror("cannot create socket");
		status = Connection_Status::DISCONNECTED;
	}
	else
	{
		if(bind(client_fd, (struct sockaddr *)&local_address, sizeof(local_address)) < 0)
		{
			perror("cannot bind socket");
			status = Connection_Status::DISCONNECTED;
		}
		else
		{
			if(setBlockingMode(Blocking_Mode::NonBlock) != Error::NONE)
			{
				perror("Nonblock failed.");
				status = Connection_Status::DISCONNECTED;
			}
			else
			{
				bool connected = false;
				bool first = true;
				int result = 0;
				struct sockaddr_in remote_address;
				memset((char *)&remote_address, 0, sizeof(remote_address));
				remote_address.sin_family = AF_INET;
				inet_pton(AF_INET, ip_address.c_str(), &(remote_address.sin_addr));
				remote_address.sin_port = htons(port);

				status = Connection_Status::DISCONNECTED;
				while(!connected)
				{
				    result = connect(client_fd, (struct sockaddr *)&remote_address, sizeof(remote_address));
			        if (result < 0)
			        {
			            if (errno == EINPROGRESS || errno == EALREADY)
			            {
			                if (first && errno == EINPROGRESS)
			                {
			                    first = false;
			                    usleep(50000);  // 50 ms
			                    continue;
			                }

			                usleep(1000000);    // 1 second
			                retries++;
			                if (retries == max_retries)
			                {
				            	char error_buffer[64];
				            	sprintf(error_buffer,"%s: Unable to connect to service(reached max retries) at '%s:%d': %s\n", __FUNCTION__, ip_address.c_str(), port, strerror(errno));
				            	perror(error_buffer);
				            	break;
			                }
			                continue;
			            }
			            else
			            {
			            	char error_buffer[64];
			            	sprintf(error_buffer,"%s: Unable to connect to service at '%s:%d': %s\n", __FUNCTION__, ip_address.c_str(), port, strerror(errno));
			            	perror(error_buffer);
			                close(client_fd);
			                break;
			            }
			        }
			        else
			        {
			        	connected = true;
			        	status = Connection_Status::CONNECTED;
			        }
				}
			}
		}
	}
	return status;
}
Connection_Status Client::Disconnect()
{
	status = Connection_Status::UNKNOWN;

	    if (client_fd >= 0)
	    {
	        close(client_fd);
	        client_fd = -1;
	        status = Connection_Status::DISCONNECTED;
	    }

	return status;
}
Error Client::setBlockingMode(Blocking_Mode mode)
{
	Error status = Error::UNKNOWN;
	int old_flags = fcntl(client_fd,F_GETFL, 0);

	if(old_flags < 0 )
	{
		perror("failed to read old flags.");
		status = Error::FCNTL_GET;
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

		old_flags = fcntl(client_fd,F_SETFL, old_flags);
		if(old_flags < 0 )
		{
			perror("failed to set non-block flags.");
			status = Error::FCNTL_SET;
		}
		else
		{
			status = Error::NONE;
		}
	}

	return status;
}

Error Client::EventRead(vector<char>& buffer)
{
	Error error_state = Error::UNKNOWN;
	int event_fd, epoll_status;
	struct epoll_event event;
	struct epoll_event events[MAX_EVENTS];

	event_fd = epoll_create1(0);
	if (event_fd == -1)
	{
		perror ("epoll_create");
		error_state = Error::EPOLL_CREATE;
	}
	else
	{
		event.data.fd = client_fd;
		event.events = EPOLLIN | EPOLLET;

		epoll_status = epoll_ctl (event_fd, EPOLL_CTL_ADD, client_fd, &event);
		if (epoll_status == -1)
		{
			perror ("epoll_create");
			error_state = Error::EPOLL_CTL;
		}
		else
		{
		    int epoll_events = epoll_wait (event_fd, events, MAX_EVENTS, -1);
		    for (int i = 0; i < epoll_events; i++)
			{
				if((events[i].events & EPOLLERR) || (events[i].events & EPOLLHUP) || (!(events[i].events & EPOLLIN ))) /*Epoll error occured*/
				{
				  perror("epoll wait error\n");
				  error_state = Error::EPOLL_WAIT;
				  close (events[i].data.fd);

				}
				else if(client_fd == events[i].data.fd)
				{
					error_state = Read(buffer);
				}
			}
		}
	}

	return error_state;
}

Error Client::Read(vector<char>& buffer)
{
	Error status = Error::UNKNOWN;
	int read_bytes = 0;
	char read_buffer[MAX_PACKET_LENGTH];

	read_bytes = read(client_fd, read_buffer, MAX_PACKET_LENGTH);
	if(read_bytes > 0)
	{
		buffer.resize(read_bytes);
		std::copy ( read_buffer, read_buffer + read_bytes, buffer.begin() );
		status = Error::NONE;
	}
	else
	{
		perror("failed to Read.");
		status = Error::READ;
	}

	return status;
}
Error Client::Write(vector<char>  buffer)
{
	Error status = Error::UNKNOWN;
	int written_bytes = 0;
	char write_buffer[MAX_PACKET_LENGTH];
	int size = 0;

	for(unsigned int i = 0; i < buffer.size(); i ++)
	{
		write_buffer[i] = buffer[i];
	}

	written_bytes = write(client_fd, write_buffer, size);

	if(written_bytes > 0)
	{
		status = Error::NONE;
	}
	else
	{
		perror("failed to Write.");
		status = Error::WRITE;
	}

	return status;
}
