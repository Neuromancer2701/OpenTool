/*
 * Header.h
 *
 *  Created on: Sep 24, 2013
 *      Author: count zero
 */

#ifndef HEADER_H_
#define HEADER_H_

#include "MessageID.h"

class Header: public MessageID {
public:
	Header();
	virtual ~Header();
	void Packup();
	void Unpack();

	const static char HeaderTotalLength = 20;

	//Open Protocol 3.0 Header
	int Length;
	const static char Length_Size = 4;

	MID_Number Number;
	const static char Number_Size = 4;

	MID_Revision Revision;
	const static char Revision_Size = 3;

	//Open Protocol 5.1 and 7.0 Header
	bool NoAckFlag;
	const static char NoAckFlag_Size = 1;

	//Open Protocol 10.0 and 10.3 Header
	int StationID;
	const static char StationID_Size = 1;

	int SpindleID;
	const static char SpindleID_Size = 2;

	// rest of the header packed with 0s

};

#endif /* HEADER_H_ */
