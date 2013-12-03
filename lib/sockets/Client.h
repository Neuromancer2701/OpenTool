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
					UNKNOWN 	= 0,
					GENERIC 	= 1,
					NONE 		= 2,
					FCNTL_SET 	= 3,
					FCNTL_GET 	= 4,
					READ		= 5,
					WRITE		= 6
				  };

static const int MAX_PACKET_LENGTH = 10000;

class Client {
public:
	Client(string _ip_address,unsigned short _port);
	Client();
	virtual ~Client();
	void init(string _ip_address,unsigned short _port);
	Connection_Status Connect();
	Connection_Status Disconnect();
	Error Read(vector<char>& buffer);
	Error Write(vector<char>  buffer);


protected:
	int client_fd;				//File Descriptor for client connection
	Connection_Status status;
	char retries;				//Number of connect retries
	static const char max_retries = 5;
	Error setBlockingMode(Blocking_Mode mode);

private:
	string ip_address;
	unsigned short port;



};

#endif /* CLIENT_H_ */