/*
 * Mid0001.h
 *
 *  Created on: Oct 20, 2013
 *      Author: count zero
 */

#ifndef MID0001_H_
#define MID0001_H_

#include "Header.h"
#include <string>

using std::string;

class Mid_0001: public Header {
public:
	Mid_0001();
	virtual ~Mid_0001();

	string BuildMid0001(MID_Revision Rev, bool NoAckFlag, int StationID, int SpindleID);
	void ParseMid0001(string data);
};

#endif /* MID0001_H_ */
