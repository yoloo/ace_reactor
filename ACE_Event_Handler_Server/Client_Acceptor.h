#ifndef _CLIENT_ACCEPTOR_H_
#define _CLIENT_ACCEPTOR_H_

#include "Client_Service.h"

/*ACE headers.*/
#include "ace/Reactor.h"
#include "ace/SOCK_Acceptor.h"

class ClientAcceptor : public ACE_Event_Handler
{
public:
	ClientAcceptor();
	explicit ClientAcceptor( const ACE_INET_Addr & addr );
	
	virtual ~ClientAcceptor();

public:/*Interface.*/
	/*Complete some initializations of server.*/
	int open();

public:/*Tools.*/
	/*Set listen address.*/
	void set_listen_addr( const ACE_INET_Addr & addr );
	/*Get listen address.*/
	ACE_INET_Addr get_listen_addr() const;

public:/*Override.*/
	/*Called by the reactor when an input event happens.*/
	virtual int handle_input( ACE_HANDLE handle = ACE_INVALID_HANDLE );
	virtual int handle_close( ACE_HANDLE handle, ACE_Reactor_Mask close_mask );

	/*Called by the reactor to determine the underlying handle, such as the peer handle.*/
	virtual ACE_HANDLE get_handle() const;

private:/*Data members.*/
	ACE_SOCK_Acceptor m_acceptor;/*Passive connection object.*/
	ACE_INET_Addr m_listenAddr;/*Listen address.*/


};


#endif