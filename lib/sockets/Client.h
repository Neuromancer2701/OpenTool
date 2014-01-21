/*
 * Client.h
 *
 *  Created on: Nov 22, 2013
 *      Author: count zero
 */

#ifndef CLIENT_H_
#define CLIENT_H_

#include <string>
#include <vector>
using std::string;
using std::vector;


enum class Connection_Status {DISCONNECTED = -1, UNKNOWN = 0, CONNECTED = 1};
enum class Blocking_Mode {Block = 0, NonBlock = 1};
enum class Error {
					UNKNOWN 	  ,
					GENERIC 	  ,
					NONE 		  ,
					FCNTL_SET 	  ,
					FCNTL_GET 	  ,
					READ		  ,
					WRITE		  ,
					SOCKET        ,
					BIND	      ,
					ACCEPT		  ,
					LISTEN		  ,
					EPOLL_CREATE  ,
					EPOLL_CTL     ,
					EPOLL_WAIT 	  ,
					EPOLL_UNKNOWN ,
					EPOLL_CLOSE	  ,
					ACCEPT_DONE   ,
					RETRIES   	  ,
					TIMEOUT
				  };

enum class Connection_Type {Client, Server};

static const int MAX_PACKET_LENGTH = 10000;

class Client {

public:
	Client(string _ip_address,unsigned short _port);
	Client();
	virtual ~Client();
	void init(string _ip_address,unsigned short _port);

	Connection_Status Connect();
	Connection_Status Disconnect();
	Error Accept(int _server_fd);
	Error EventRead(vector<char>& buffer);
	Error Read(vector<char>& buffer);
	Error Write(vector<char>  buffer);
	Connection_Status Status(){return status;}
	int get_fd(){return client_fd;}

protected:
	Connection_Status status;
	int client_fd;				//File Descriptor for client connection
	char retries;				//Number of connect retries
	static const char max_retries = 5;
	Error setBlockingMode(Blocking_Mode mode);

private:
	string ip_address;
	unsigned short port;
	static const int MAX_EVENTS = 16;




};

#endif /* CLIENT_H_ */
