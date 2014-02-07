/*
 * OpenTool.c
 *
 *  Created on: Dec 10, 2013
 *      Author: root
 */

#include "OpenTool.h"
#include "Mids.h"

#include <exception>
#include <sstream>


using std::stringstream;
using std::exception;


stringstream Log;


OpenTool::OpenTool()
{
	ip_address = "";
	port = 0;

	timeout = 0;
	retries = 0;

	openToolHeader.SpindleID = 0;
}

OpenTool::OpenTool(int spindleNumber, string _ip, int _port, int _timeout, int _retries, MID_Revision _rev)
{
	ip_address = _ip;
	port = _port;
	timeout = static_cast<double>(_timeout);
	retries = _retries;
	openToolHeader.SpindleID = spindleNumber;
	openToolHeader.Revision = version = _rev;
	SetStationID();
	SetAckStatus();
	lastSentTime = std::chrono::system_clock::now();
}

OpenTool::~OpenTool()
{

}
void OpenTool::SetStationID(int station_id)
{
	openToolHeader.StationID = station_id;
}
void OpenTool::SetAckStatus(bool ack)
{
	openToolHeader.NoAckFlag = !ack;
}

Error OpenTool::Connect()
{
	 Error result = Error::UNKNOWN;
	 client.init(ip_address,port);

	 if(client.Connect() == Connection_Status::CONNECTED)
	 {
		 openToolHeader.Number = MID_Number::CommunicationStart;
		 MIDOutputAction(&openToolHeader);
	 }

	 return result;
}

Error OpenTool::Disconnect()
{
	 Error result = Error::UNKNOWN;

	 client.Disconnect();
	 if(client.Status() == Connection_Status::DISCONNECTED)
		 result = Error::NONE;
	 else
		 result = Error::DISCONNECT;

	 return result;
}

Error OpenTool::Listen()
{
	 Error result = Error::UNKNOWN;
	 Server server;

	 return result;
}
Error OpenTool::Send(string data)
{
	 Error result = Error::UNKNOWN;
	 vector<char>  buffer(data.begin(), data.end());
	 result = client.Write(buffer);

	 return result;
}

bool OpenTool::isTimedOut()
{
	bool timed_out = false;
	std::chrono::time_point<std::chrono::system_clock> current_time;
	current_time = std::chrono::system_clock::now();

	std::chrono::duration<double> seconds = current_time - lastSentTime;

	if(seconds.count() > timeout)
		timed_out = true;

	return timed_out;
}

bool OpenTool::RetriesReached()
{
	bool retries_occured = false;

	return retries_occured;
}


Error OpenTool::MIDInputAction(Header* message)
{
	 Error result = Error::UNKNOWN;

	 switch(message->Number)
	 {
		 case MID_Number::CommunicationStart:
			  try{
				  Mid_0001* mid0001 = dynamic_cast<Mid_0001*>(message);
				  mid0001->Unpack();
				  result = MID0001Received(mid0001);
			  }
			  catch(exception& my_ex)
			  {
				  Log << "Exception: " << my_ex.what();
			  }
			  break;
		 case MID_Number::CommunicationStartAcknowledge:
			  try{
				  Mid_0002* mid0002 = dynamic_cast<Mid_0002*>(message);
				  mid0002->Unpack();
				  result = MID0002Received(mid0002);
			  }
			  catch(exception& my_ex)
			  {
				  Log << "Exception: " << my_ex.what();
			  }
			  break;
		 case MID_Number::CommunicationStop:
			  try{
				  Mid_0003* mid0003 = dynamic_cast<Mid_0003*>(message);
				  mid0003->Unpack();
				  result = MID0003Received(mid0003);
			  }
			  catch(exception& my_ex)
			  {
				  Log << "Exception: " << my_ex.what();
			  }
			  break;
		 case MID_Number::CommandError:
			  try{
				  Mid_0004* mid0004 = dynamic_cast<Mid_0004*>(message);
				  mid0004->Unpack();
				  result = MID0004Received(mid0004);
			  }
			  catch(exception& my_ex)
			  {
				  Log << "Exception: " << my_ex.what();
			  }
			  break;
		 case MID_Number::CommandAccepted:
			  try{
				  Mid_0005* mid0005 = dynamic_cast<Mid_0005*>(message);
				  mid0005->Unpack();
				  result = MID0005Received(mid0005);
			  }
			  catch(exception& my_ex)
			  {
				  Log << "Exception: " << my_ex.what();
			  }
			  break;
		 case MID_Number::KeepAliveMessage:
			  try{
				  Mid_9999* mid9999 = dynamic_cast<Mid_9999*>(message);
				  mid9999->Unpack();
				  result = MID9999Received(mid9999);
			  }
			  catch(exception& my_ex)
			  {
				  Log << "Exception: " << my_ex.what();
			  }
			  break;

		 default:
			  break;
	 }

	 if(result == Error::NONE)
		 lastSentTime = std::chrono::system_clock::now();

	 return result;
}

Error OpenTool::MIDOutputAction(Header* message)
{
	 Error result = Error::UNKNOWN;
	 string data("");

	 switch(message->Number)
	 {
		 case MID_Number::CommunicationStart:
			  try{
				  Mid_0001* mid0001 = dynamic_cast<Mid_0001*>(message);
				  mid0001->Packup();
				  data = mid0001->GetRawOutputString();
			  }
			  catch(exception& my_ex)
			  {
				  Log << "Exception: " << my_ex.what();
			  }
			  break;
		 case MID_Number::CommunicationStartAcknowledge:
			  try{
				  Mid_0002* mid0002 = dynamic_cast<Mid_0002*>(message);
				  mid0002->Packup();
				  data = mid0002->GetRawOutputString();
			  }
			  catch(exception& my_ex)
			  {
				  Log << "Exception: " << my_ex.what();
			  }
			  break;
		 case MID_Number::CommunicationStop:
			  try{
				  Mid_0003* mid0003 = dynamic_cast<Mid_0003*>(message);
				  mid0003->Packup();
				  data = mid0003->GetRawOutputString();
			  }
			  catch(exception& my_ex)
			  {
				  Log << "Exception: " << my_ex.what();
			  }
			  break;
		 case MID_Number::CommandError:
			  try{
				  Mid_0004* mid0004 = dynamic_cast<Mid_0004*>(message);
				  mid0004->Packup();
				  data = mid0004->GetRawOutputString();
			  }
			  catch(exception& my_ex)
			  {
				  Log << "Exception: " << my_ex.what();
			  }
			  break;
		 case MID_Number::CommandAccepted:
			  try{
				  Mid_0005* mid0005 = dynamic_cast<Mid_0005*>(message);
				  mid0005->Packup();
				  data = mid0005->GetRawOutputString();
			  }
			  catch(exception& my_ex)
			  {
				  Log << "Exception: " << my_ex.what();
			  }
			  break;
		 case MID_Number::KeepAliveMessage:
			  try{
				  Mid_9999* mid9999 = dynamic_cast<Mid_9999*>(message);
				  mid9999->Packup();
				  data = mid9999->GetRawOutputString();
			  }
			  catch(exception& my_ex)
			  {
				  Log << "Exception: " << my_ex.what();
			  }
			  break;

		 default:
			  break;
	 }

	 if(data.length() >= Header::HeaderTotalLength )
	 {
		 result = Send(data);
	 }
	 else
	 {
		 Log << "Opps data shorter than default header.";
	 }

	 return result;
}

class KeepAliveTimer : public Timer
{
public:
	KeepAliveTimer(int seconds, int milliseconds):Timer(seconds, milliseconds, false){};
	virtual ~KeepAliveTimer(){};
	void TimerTask()
	{

	}
};

