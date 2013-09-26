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

	int Length;
	MID_Number Number;
	MID_Revision Revision;
	bool NoAckFlag;
	int StationID;
	int SpindleID;
};

#endif /* HEADER_H_ */
