/*
 * Mid0005.cpp
 *
 *  Created on: Oct 20, 2013
 *      Author: count zero
 */

#include "Mid0005.h"
#include <iomanip>
#include <iostream>

Mid_0005::Mid_0005() {
	Length = HeaderTotalLength;
	Number = CommandAccepted;
}

Mid_0005::~Mid_0005() {
	// TODO Auto-generated destructor stub
}

void Mid_0005::BuildMid0005()
{
	Length += Number_Size;
	Packup();

	OutputStream << std::setw(Number_Size) << std::setfill('0') << (int)Number;
	OutputStream << '\0';
}
void Mid_0005::ParseMid0005()
{
	int placeholder = 0;
	Unpack();
	InputStream >> std::setw(Number_Size)	>> placeholder;
	Number =  (MID_Number) placeholder;
}
