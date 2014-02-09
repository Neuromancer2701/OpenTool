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
#include "KeepAliveTimer.h"

#include <memory>
#include <functional>
#include <string>
#include <chrono>

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

	double timeout;
	int retries;


	Header openToolHeader;

	function<Error(Header* message)>MID0001Received;
	function<Error(Header* message)>MID0002Received;
	function<Error(Header* message)>MID0003Received;
	function<Error(Header* message)>MID0004Received;
	function<Error(Header* message)>MID0005Received;
	function<Error(Header* message)>MID9999Received;

private:
	void SendKeepAlive();
	Client client;
	Server server;
	long connection_time;
	std::chrono::time_point<std::chrono::system_clock> lastSentTime;
	int timer_count;
	KeepAliveTimer keep_alive_timer;
};

#endif /* OPENTOOL_H_ */
