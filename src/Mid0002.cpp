/*
 * Mid0002.cpp
 *
 *  Created on: Oct 20, 2013
 *      Author: count zero
 */

#include "Mid0002.h"
#include <iomanip>
#include <iostream>

Mid_0002::Mid_0002() {
	Length = HeaderTotalLength;
	Number = CommunicationStartAcknowledge;
}

Mid_0002::~Mid_0002() {
	// TODO Auto-generated destructor stub
}

void Mid_0002::BuildRevision1()
{
	OutputStream << std::setw(Index_size) << std::setfill('0') << 1;
	OutputStream << std::setw(Cell_Id_size) << std::setfill('0') << Cell_Id;
	OutputStream << std::setw(Index_size) << std::setfill('0') << 2;
	OutputStream << std::setw(Channel_Id_size) << std::setfill('0') << Channel_Id;
	OutputStream << std::setw(Index_size) << std::setfill('0') << 3;
	OutputStream << std::setw(Controller_Name_size) << std::setfill(' ') << Controller_Name;
}

void Mid_0002::BuildRevision2()
{
	BuildRevision1();
	OutputStream << std::setw(Index_size) << std::setfill('0') << 4;
	OutputStream << std::setw(Supplier_code_size) << std::setfill(' ') << Supplier_code;

}

void Mid_0002::BuildRevision3()
{
	BuildRevision2();
	OutputStream << std::setw(Index_size) << std::setfill('0') << 5;
	OutputStream << std::setw(Open_Protocol_version_size) << std::setfill('0') << Open_Protocol_version;
	OutputStream << std::setw(Index_size) << std::setfill('0') << 6;
	OutputStream << std::setw(Controller_software_version_size) << std::setfill('0') << Controller_software_version;
	OutputStream << std::setw(Index_size) << std::setfill('0') << 7;
	OutputStream << std::setw(Tool_software_version_size) << std::setfill(' ') << Tool_software_version;
}

void Mid_0002::PackupMid0002()
{

	switch(Revision)
	{
		default:
		case Revision1:
			Length += Rev1_size;
			Packup();
			BuildRevision1();
			OutputStream << '\0';
			break;
		case Revision2:
			Length += Rev2_size;
			Packup();
			BuildRevision2();
			OutputStream << '\0';
			break;
		case Revision3:
			Length += Rev3_size;
			Packup();
			BuildRevision3();
			OutputStream << '\0';
			break;
	}
}

void Mid_0002::ParseRevision1()
{
	int index;
	OutputStream >> std::setw(Index_size) >> index;
	OutputStream >> std::setw(Cell_Id_size) >> Cell_Id;
	OutputStream >> std::setw(Index_size) >> index;
	OutputStream >> std::setw(Channel_Id_size) >> Channel_Id;
	OutputStream >> std::setw(Index_size) >> index;
	OutputStream >> std::setw(Controller_Name_size) >> Controller_Name;

}
void Mid_0002::ParseRevision2()
{
	int index;
	ParseRevision1();
	OutputStream >> std::setw(Index_size) >> index;
	OutputStream >> std::setw(Supplier_code_size) >> Supplier_code;

}
void Mid_0002::ParseRevision3()
{
	int index;
	ParseRevision2();
	OutputStream >> std::setw(Index_size) >> index;
	OutputStream >> std::setw(Open_Protocol_version_size) >> Open_Protocol_version;
	OutputStream >> std::setw(Index_size) >> index;
	OutputStream >> std::setw(Controller_software_version_size) >> Controller_software_version;
	OutputStream >> std::setw(Index_size) >> index;
	OutputStream >> std::setw(Tool_software_version_size) >> Tool_software_version;
}

void Mid_0002::UnPackMid0002()
{
	Unpack();

	switch(Revision)
	{
		default:
		case Revision1:
			ParseRevision1();
			break;
		case Revision2:
			ParseRevision2();
			break;
		case Revision3:
			ParseRevision3();
			break;
	}

}
