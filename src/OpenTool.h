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
#include <string>

using std::string;

using std::function;
using std::unique_ptr;


class OpenTool {
public:
	OpenTool();
	OpenTool(int spindleNumber, string _ip, int _port, int _timeout, int _retries, MID_Revision _rev);
	virtual ~OpenTool();

	Error Connect();
	Error Listen();
	Error Send(string data);
	Error Disconnect();
	Error MIDInputAction(Header* message);
	Error MIDOutputAction(Header* message);

	bool isTimedOut();
	bool RetriesReached();
	void SetStationID(int station_id = 0);
	void SetAckStatus(bool ack = true);

	Connection_Type type;

	string ip_address;
	int port;
	MID_Revision version;

	int timeout;
	int retries;


	Header openToolHeader;

	function<void(Header* message)>MID0001Received;
	function<void(Header* message)>MID0002Received;
	function<void(Header* message)>MID0003Received;
	function<void(Header* message)>MID0004Received;
	function<void(Header* message)>MID0005Received;
	function<void(Header* message)>MID9999Received;

private:
	Client client;
	Server server;
	long connection_time;


};

#endif /* OPENTOOL_H_ */
