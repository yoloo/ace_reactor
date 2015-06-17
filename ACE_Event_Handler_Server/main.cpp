/*User headers.*/
#include "Client_Acceptor.h"

/*ACE headers.*/
#include "ace/Log_Msg.h"



int main( int argc, char * argv[] )
{
	ACE_INET_Addr addr( 8010 );

	ClientAcceptor * pClientAcceptor = new ClientAcceptor( addr );
	pClientAcceptor->open();

	/*The underlying passive acceptor open to listen.*/
	ACE_DEBUG( ( LM_DEBUG, "The server start to listen...\n" ) );

	while( true )
	{
		ACE_Reactor::instance()->handle_events();
	}

	return 0;
}