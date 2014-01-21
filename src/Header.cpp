/*
 * Header.cpp
 *
 *  Created on: Sep 24, 2013
 *      Author: count zero
 */

#include "Header.h"
#include <iomanip>
#include <iostream>

Header::Header() {
	// TODO Auto-generated constructor stub

}

Header::~Header() {
	// TODO Auto-generated destructor stub
}

void Header::Packup()	{

	int Spare = HeaderTotalLength - (Length_Size + Number_Size + Revision_Size + StationID_Size + SpindleID_Size);
	OutputStream << ""; //clear string stream

	OutputStream << std::setw(Length_Size) << std::setfill('0') << Length;

	OutputStream << std::setw(Number_Size) << std::setfill('0') << Number;

	OutputStream << std::setw(Revision_Size) << std::setfill('0') << Revision;

	OutputStream << std::setw(NoAckFlag_Size) << std::setfill('0') << static_cast<int>(NoAckFlag);

	OutputStream << std::setw(StationID_Size) << std::setfill('0') << StationID;

	OutputStream << std::setw(SpindleID_Size) << std::setfill('0') << SpindleID;

	OutputStream << std::setw(Spare) << std::setfill('0') << 0 << std::endl; // Fill rest of header


}

void Header::Unpack()	{

	InputStream >> std::setw(Length_Size)	>> Length;

	InputStream >> std::setw(Number_Size)	>> Number;

	InputStream >> std::setw(Revision_Size) >> Revision;

	InputStream >> std::setw(NoAckFlag_Size) >> NoAckFlag;

	InputStream >> std::setw(StationID_Size) >> StationID;

	InputStream >> std::setw(SpindleID_Size) >> SpindleID;
}
