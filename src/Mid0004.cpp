/*
 * Mid0004.cpp
 *
 *  Created on: Oct 20, 2013
 *      Author: count zero
 */

#include "Mid0004.h"
#include <iomanip>
#include <iostream>

Mid_0004::Mid_0004() {
	Length = HeaderTotalLength;
	Number = MID_Number::CommandError;
}

Mid_0004::~Mid_0004() {
	// TODO Auto-generated destructor stub
}


void Mid_0004::BuildMid0004()
{
	Length += Number_Size;
	Length += Error_Size;
	Packup();

	OutputStream << std::setw(Number_Size) << std::setfill('0') << (int)Number;
	OutputStream << std::setw(Error_Size) << std::setfill('0') << (int)Error;
	OutputStream << '\0';
}
void Mid_0004::ParseMid0004()
{
	int placeholder = 0;
	Unpack();
	InputStream >> std::setw(Number_Size)	>> placeholder;
	Number =  (MID_Number) placeholder;

	InputStream >> std::setw(Error_Size) >> placeholder;
	Error = (MID_Error) placeholder;
}

