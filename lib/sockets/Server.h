/*
 * Server.h
 *
 *  Created on: Nov 22, 2013
 *      Author: count zero
 */

#ifndef SERVER_H_
#define SERVER_H_

#include "Client.h"
#include <array>

using std::array;

class Server {
public:
	Server();
	virtual ~Server();

	array<Client, 6> client_list;

};

#endif /* SERVER_H_ */
