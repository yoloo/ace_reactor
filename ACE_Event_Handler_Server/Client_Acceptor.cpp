/*User headers.*/
#include "Client_Acceptor.h"
#include "Client_Service.h"

/*ACE headers.*/
#include "ace/Log_Msg.h"


ClientAcceptor::ClientAcceptor()
{

}


ClientAcceptor::ClientAcceptor( const ACE_INET_Addr & addr )
{
	m_listenAddr = addr;
}


ClientAcceptor::~ClientAcceptor()
{

}

int ClientAcceptor::open()
{
	/*Bind and listen at the given address.*/
	int ret = m_acceptor.open( m_listenAddr );
	if ( -1 == ret )
	{
		ACE_DEBUG( ( LM_DEBUG, "Error at ACE_SOCK_Acceptor::open().\n" ) );

		return -1;
	}

	/*Register accept-event.*/
	return ACE_Reactor::instance()->register_handler( this, ACE_Event_Handler::ACCEPT_MASK );
}

int ClientAcceptor::handle_input( ACE_HANDLE fd )
{
	/*Create one ClientService object to handle one connection.*/
	ClientService * pClientService = new ClientService();

	/*Accept the connection and give it to the created ClientService to handle.*/
	int ret = m_acceptor.accept( pClientService->peer(), 0, 0, 1 );
	if ( -1 == ret )
	{
		pClientService->handle_close( ACE_INVALID_HANDLE, NULL_MASK );

		ACE_DEBUG( ( LM_DEBUG, "Error at ACE_SOCK_Acceptor::acceptor().\n" ) );
		return -1;
	}

	return pClientService->open();
}

int ClientAcceptor::handle_close( ACE_HANDLE handle, ACE_Reactor_Mask close_mask )
{
	/*Close ACE_SOCK_Acceptor object and stop to listen.*/
	m_acceptor.close();

	delete this;

	return -1;
}


ACE_HANDLE ClientAcceptor::get_handle() const
{
	return m_acceptor.get_handle();
}


void ClientAcceptor::set_listen_addr( const ACE_INET_Addr & addr )
{
	m_listenAddr = addr;
}


ACE_INET_Addr ClientAcceptor::get_listen_addr() const
{
	return m_listenAddr;
}
