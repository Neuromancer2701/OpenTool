/*
 * Mid0005.h
 *
 *  Created on: Oct 20, 2013
 *      Author: count zero
 */

#ifndef MID0005_H_
#define MID0005_H_

#include "Header.h"

class Mid_0005: public Header {
public:
	Mid_0005();
	virtual ~Mid_0005();

	MID_Number Number;
	const static char Number_Size = 4;

	void BuildMid0005();
	void ParseMid0005();
};

#endif /* MID0005_H_ */
