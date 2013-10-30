/*
 * Mid0001.cpp
 *
 *  Created on: Oct 20, 2013
 *      Author: count zero
 */

#include "Mid0001.h"

Mid_0001::Mid_0001() {
	// TODO Auto-generated constructor stub
	Length = HeaderTotalLength;
	Number = CommunicationStart;
}

Mid_0001::~Mid_0001() {
	// TODO Auto-generated destructor stub
}

string Mid_0001::BuildMid0001(MID_Revision Rev, bool NoAck, int Station, int Spindle)
{
	Revision = Rev;
	NoAckFlag = NoAck;
	StationID = Station;
	SpindleID = Spindle;
	Packup();

	return OutputStream.str();
}

void Mid_0001::ParseMid0001(string data)
{
	InputStream << data;
	Unpack();
}
