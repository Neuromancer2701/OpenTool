/*
 * OpenTool.c
 *
 *  Created on: Dec 10, 2013
 *      Author: root
 */

#include "OpenTool.h"
#include "Mids.h"
#include <exception>
#include <memory>

using std::unique_ptr;

OpenTool::OpenTool()
{
	ip_address = "";
	port = 0;

	timeout = 0;
	retries = 0;

	openToolHeader.SpindleID = 0;
}

OpenTool::OpenTool(int spindleNumber, string _ip, int _port, int _timeout, int _retries)
{
	ip_address = _ip;
	port = _port;
	timeout = _timeout;
	retries = _retries;
	openToolHeader.SpindleID = spindleNumber;
}

OpenTool::~OpenTool()
{

}


Error OpenTool::Connect()
{
	 Error result = Error::UNKNOWN;


	 return result;
}

Error OpenTool::Disconnect()
{
 Error result = Error::UNKNOWN;


 return result;
}

Error OpenTool::Listen()
{
	 Error result = Error::UNKNOWN;


	 return result;
}

bool OpenTool::isTimedOut()
{
	bool timed_out = false;

	return timed_out;
}

bool OpenTool::RetriesReached()
{
	bool retries_occured = false;

	return retries_occured;
}


Error OpenTool::MIDInputAction(Header header)
{
	 Error result = Error::UNKNOWN;

	 switch(header.Number)
	 {
		 case MID_Number::CommunicationStart:
			  break;
		 case MID_Number::CommunicationStartAcknowledge:
			  break;
		 case MID_Number::CommunicationStop:
			  break;
		 case MID_Number::CommandError:
			  break;
		 case MID_Number::CommandAccepted:
			  break;
		 case MID_Number::KeepAliveMessage:
			  break;

		 default:
			  break;
	 }


	 return result;
}

Error OpenTool::MIDOutputAction(Header* header)
{
	 Error result = Error::UNKNOWN;

	 switch(header->Number)
	 {
		 case MID_Number::CommunicationStart:
			  //try{
			  //unique_ptr<Mid_0001> mid001 = dynamic_cast<Mid_0001 *>(header);
			  //	  }catch (exception& my_ex) {cout << "Exception: " << my_ex.what();}
			  break;
		 case MID_Number::CommunicationStartAcknowledge:
			  break;
		 case MID_Number::CommunicationStop:
			  break;
		 case MID_Number::CommandError:
			  break;
		 case MID_Number::CommandAccepted:
			  break;
		 case MID_Number::KeepAliveMessage:
			  break;

		 default:
			  break;
	 }


	 return result;
}
