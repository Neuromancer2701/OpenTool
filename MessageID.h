/*
 * MessageID.h
 *
 *  Created on: Sep 24, 2013
 *      Author: count zero
 */

#ifndef MESSAGEID_H_
#define MESSAGEID_H_

class MessageID {
public:
	MessageID();
	virtual ~MessageID();
	virtual void Packup(){};
	virtual void Unpack(){};

	enum MID_Number
	{
		enCommunicationStart = 0001,
		enCommunicationStartAcknowledge = 0002,
		enCommunicationStop = 0003,
		enCommandError = 0004,
		enCommandAccepted =  0005,
		enKeepAliveMessage = 9999
	};

	enum MID_Revision
	{
		enRevision1 = 1,
		enRevision2 = 2,
		enRevision3 = 3,
		enRevision4 = 4,
		enRevision5 = 5,
		enRevision6 = 6,
		enRevision500 = 500,
		enRevision998 = 998,
		enRevision999 = 999
	};

};

#endif /* MESSAGEID_H_ */
