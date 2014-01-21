/*
 * OpenTool.h
 *
 *  Created on: Dec 10, 2013
 *      Author: root
 */

#ifndef OPENTOOL_H_
#define OPENTOOL_H_

#include "Server.h"
#include "Header.h"

#include <memory>
#include <functional>

using std::function;
using std::unique_ptr;


class OpenTool {
public:
	OpenTool();
	OpenTool(int spindleNumber, string _ip, int _port, int _timeout, int _retries);
	virtual ~OpenTool();

	Error Connect();
	Error Listen();
	Error Disconnect();
	Error MIDInputAction(Header header);
	Error MIDOutputAction(unique_ptr<void> message);

	bool isTimedOut();
	bool RetriesReached();

	Connection_Type type;

	string ip_address;
	int port;

	int timeout;
	int retries;

	Header openToolHeader;

	function<void()>MID0001Received;
	function<void()>MID0002Received;
	function<void()>MID0003Received;
	function<void()>MID0004Received;
	function<void()>MID0005Received;
	function<void()>MID9999Received;

private:
	Client client;
	Server server;


};

#endif /* OPENTOOL_H_ */
