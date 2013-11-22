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
#include <sys/unistd.h>
#include <sys/fcntl.h>
#include <errno.h>
#include <cstdio>
#include <cstring>

Client::Client(string _ip_address,unsigned short _port) {

	ip_address = _ip_address;
	port = _port;
	client_fd = -1;
	status = Connection_Status::UNKNOWN;
	retries = 0;
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
