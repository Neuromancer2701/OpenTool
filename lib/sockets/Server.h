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
	Server();
	virtual ~Server();

	Client client_list[6];

};

#endif /* SERVER_H_ */
