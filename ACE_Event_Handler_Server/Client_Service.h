#ifndef _CLIENT_SERVICE_H_
#define _CLIENT_SERVICE_H_

/*ACE headers.*/
#include "ace/Reactor.h"
#include "ace/SOCK_Stream.h"
#include "ace/Message_Block.h"

enum
{
	SIZE_OF_RECV = 1024,
	SIZE_OF_SEND = 2048,
};

class ClientService : public ACE_Event_Handler
{
public:/*Constructor.*/
	ClientService();
	~ClientService();

public:/*Interface.*/
	int open();
	/*Get the reference of the communication peer.*/
	ACE_SOCK_Stream & peer();

public:/*Override.*/
	/*Called by the reactor when an input event happens.*/
	virtual int handle_input( ACE_HANDLE handle = ACE_INVALID_HANDLE );
	virtual int handle_output( ACE_HANDLE handle = ACE_INVALID_HANDLE );
	virtual int handle_close( ACE_HANDLE handle, ACE_Reactor_Mask close_mask );
	/*Called by the reactor to determine the underlying handle, such as the peer handle.*/
	virtual ACE_HANDLE get_handle() const;

private:/*Data members.*/
	ACE_SOCK_Stream m_peer;/*Communication peer.*/

	ACE_Message_Block * m_pRecvBuf;
	ACE_Message_Block * m_pSendBuf;
};

#endif