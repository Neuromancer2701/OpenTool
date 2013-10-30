/*
 * Mid0002.h
 *
 *  Created on: Oct 20, 2013
 *      Author: count zero
 */

#ifndef MID0002_H_
#define MID0002_H_

#include "Header.h"
#include <string>

using std::string;

class Mid_0002: public Header {
public:
	Mid_0002();
	virtual ~Mid_0002();

	void PackupMid0002();
	void UnPackMid0002();

	void BuildRevision1();
	void BuildRevision2();
	void BuildRevision3();

	void ParseRevision1();
	void ParseRevision2();
	void ParseRevision3();

	//Revision 1
	int Cell_Id;
	int Channel_Id;
	string Controller_Name;

	static const int Cell_Id_size = 4;
	static const int Channel_Id_size = 2;
	static const int Controller_Name_size = 25;
	static const int Rev1_size = Cell_Id_size + Channel_Id_size + Controller_Name_size + (3 * Index_size);

	//Revision 2
	string Supplier_code;
	static const int Supplier_code_size = 3;
	static const int Rev2_size = Rev1_size + Supplier_code_size + Index_size;

	//Revision 3
	string Open_Protocol_version;
	string Controller_software_version;
	string Tool_software_version;

	static const int Open_Protocol_version_size = 19;
	static const int Controller_software_version_size = 19;
	static const int Tool_software_version_size = 19;
	static const int Rev3_size = Rev2_size + Open_Protocol_version_size + Controller_software_version_size + Tool_software_version_size + (3 * Index_size);
};

#endif /* MID0002_H_ */
