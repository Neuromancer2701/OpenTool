/*
 * Mid0004.h
 *
 *  Created on: Oct 20, 2013
 *      Author: count zero
 */

#ifndef MID0004_H_
#define MID0004_H_

#include "Header.h"

class Mid_0004: public Header {
public:
	Mid_0004();
	virtual ~Mid_0004();

	MID_Number Number;
	const static char Number_Size = 4;

	MID_Error Error;
	const static char Error_Size = 2;

	void BuildMid0004();
	void ParseMid0004();
};

#endif /* MID0004_H_ */
