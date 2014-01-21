/*
 * MessageID.h
 *
 *  Created on: Sep 24, 2013
 *      Author: count zero
 */

#ifndef MESSAGEID_H_
#define MESSAGEID_H_

#include <sstream>
#include <string>

using std::string;
using std::stringstream;

enum class MID_Number
{
	CommunicationStart = 0001,
	CommunicationStartAcknowledge = 0002,
	CommunicationStop = 0003,
	CommandError = 0004,
	CommandAccepted =  0005,
	KeepAliveMessage = 9999
};

inline std::ostream& operator<<( std::ostream& stream, const MID_Number number )
{
	stream << static_cast<int>(number);
    return stream;
}

inline std::istream & operator>>(std::istream & stream, MID_Number& number )
{
	unsigned placeholder = 0;
	if(stream >> placeholder)
		number = static_cast<MID_Number>(placeholder);
    return stream;
}


enum class MID_Revision
{
	Revision1 = 1,
	Revision2 = 2,
	Revision3 = 3,
	Revision4 = 4,
	Revision5 = 5,
	Revision6 = 6,
	Revision500 = 500,
	Revision998 = 998,
	Revision999 = 999
};

inline std::ostream& operator<<( std::ostream& stream, const MID_Revision revision )
{
	stream << static_cast<int>(revision);
    return stream;
}

inline std::istream & operator>>(std::istream & stream, MID_Revision& revision )
{
	unsigned placeholder = 0;
	if(stream >> placeholder)
		revision = static_cast<MID_Revision>(placeholder);
    return stream;
}

enum class MID_Error
{
	No_Error = 0,
	Invalid_data = 1,
	Application_number_not_present = 2,
	Application_cannot_be_set = 3,
	Application_not_running = 4,
	VIN_upload_subscription_already_exists = 6,
	VIN_upload_subscription_does_not_exists = 7,
	VIN_input_source_not_granted = 8,
	Last_tightening_result_subscription_already_exists = 9,
	Last_tightening_result_subscription_does_not_exist = 10,
	Alarm_subscription_already_exists = 11,
	Alarm_subscription_does_not_exist = 12,
	Application_selection_subscription_already_exists = 13,
	Application_selection_subscription_does_not_exist = 14,
	Tightening_Id_requested_not_found = 15,
	Connection_rejected_protocol_busy = 16,
	Linking_Group_number_not_present = 17,
	Linking_Group_info_subscription_already_exists = 18,
	Linking_Group_info_subscription_does_not_exist = 19,
	Linking_Group_cannot_be_set = 20,
	Linking_Group_not_running = 21,
	Not_possible_to_execute_dynamic_Linking_Group_request = 22,
	Linking_Group_batch_decrement_failed = 23,
	Controller_is_not_a_sync_Master = 30,
	Multi_spindle_status_subscription_already_exists = 31,
	Multi_spindle_status_subscription_does_not_exist = 32,
	Multi_spindle_result_subscription_already_exists = 33,
	Multi_spindle_result_subscription_does_not_exist = 34,
	Linking_Group_line_control_info_subscription_already_exists = 40,
	Linking_Group_line_control_info_subscription_does_not_exist = 41,
	Identifier_input_source_not_granted = 42,
	Multiple_identifiers_work_order_subscription_already_exists = 43,
	Multiple_identifiers_work_order_subscription_does_not_exist = 44,
	Status_external_monitored_inputs_subscription_already_exists = 50,
	Status_external_monitored_inputs_subscription_does_not_exist = 51,
	IO_device_not_connected = 52,
	Faulty_IO_device_number = 53,
	Tool_currently_in_use = 59,
	No_histogram_available = 60,
	Calibration_failed = 70,
	Calibration_failed2 = 79,
	Audi_emergency_status_subscription_exists = 80,
	Audi_emergency_status_subscription_does_not_exist = 81,
	Automatic_Manual_mode_subscribe_already_exist = 82,
	Automatic_Manual_mode_subscribe_does_not_exist = 83,
	The_relay_function_subscription_already_exists = 84,
	The_relay_function_subscription_does_not_exist = 85,
	The_selector_socket_info_subscription_already_exist = 86,
	The_selector_socket_info_subscription_does_not_exist = 87,
	The_digin_info_subscription_already_exist = 88,
	The_digin_info_subscription_does_not_exist = 89,
	Lock_at_batch_done_subscription_already_exist = 90,
	Lock_at_batch_done_subscription_does_not_exist = 91,
	Open_protocol_commands_disabled = 92,
	Open_protocol_commands_disabled_subscription_already_exists = 93,
	Open_protocol_commands_disabled_subscription_does_not_exist = 94,
	Reject_request,_PowerMACS_is_in_manual_mode = 95,
	Client_already_connected = 96,
	MID_revision_unsupported = 97,
	Controller_internal_request_timeout = 98,
	Unknown_MID = 99
};

inline std::ostream& operator<<( std::ostream& stream, const MID_Error error)
{
	stream << static_cast<int>(error);
    return stream;
}

inline std::istream & operator>>(std::istream & stream, MID_Error& error )
{
	unsigned placeholder = 0;
	if(stream >> placeholder)
		error = static_cast<MID_Error>(placeholder);
    return stream;
}


class MessageID {
public:
	MessageID();
	virtual ~MessageID();
	virtual void Packup(){};
	virtual void Unpack(){};

	stringstream InputStream;
	stringstream OutputStream;
	string GetRawOutputString(){return OutputStream.str();}
	void SetRawInputString(const string input){InputStream << input;}

	static const int Index_size = 2;

};

#endif /* MESSAGEID_H_ */
