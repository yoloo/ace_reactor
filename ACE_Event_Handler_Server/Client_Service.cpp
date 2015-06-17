/*User headers.*/
#include "Client_Service.h"

/*ACE headers.*/
#include "ace/Log_Msg.h"
#include "ace/OS.h"
#include <ace/Message_Block.h>

ClientService::ClientService()
{
	m_pRecvBuf = NULL;
	m_pSendBuf = NULL;
}


ClientService::~ClientService()
{
}


int ClientService::open()
{
	try
	{
		/*Dynamic-malloc for recv-buffer.*/
		m_pRecvBuf = new ACE_Message_Block( SIZE_OF_RECV );
		ACE_OS::memset( m_pRecvBuf->rd_ptr(), '\0', SIZE_OF_RECV );

		/*Dynamic-malloc for recv-buffer.*/
		m_pSendBuf = new ACE_Message_Block( SIZE_OF_SEND );
		ACE_OS::memset( m_pSendBuf->rd_ptr(), '\0', SIZE_OF_SEND );
	}
	catch( ... )
	{
		m_pRecvBuf = NULL;
		m_pSendBuf = NULL;

		return -1;
	}

	return ACE_Reactor::instance()->register_handler( this, ACE_Event_Handler::READ_MASK | ACE_Event_Handler::WRITE_MASK );
}

int ClientService::handle_input( ACE_HANDLE handle )
{
	ssize_t nBytesRecv = 0;
	nBytesRecv = m_peer.recv( m_pRecvBuf->wr_ptr(), SIZE_OF_RECV );
	if ( nBytesRecv <= 0 )//Connection has closed down or an error has occurred.
	{
		return -1;
	}
	else
	{
		m_pRecvBuf->wr_ptr( nBytesRecv );
	}

	return 0;
}


int ClientService::handle_output( ACE_HANDLE handle )
{
	size_t nActualSendBytes = 0;
	size_t nTotalSendBytes = m_pSendBuf->length();
	while ( nTotalSendBytes > 0 )
	{
		size_t nOnceSendBytes = 0;
		nOnceSendBytes = m_peer.send( m_pSendBuf->rd_ptr() + nActualSendBytes, nTotalSendBytes );

		nActualSendBytes += nOnceSendBytes;
		nTotalSendBytes -= nOnceSendBytes;
	}

	/*Check whether the send-buffer have been sent completely.*/
	if ( nActualSendBytes != m_pSendBuf->length() )
	{
		return -1;
	}

	//size_t nTotalSendBytes = m_pSendBuf->length();
	//if ( 0 == nTotalSendBytes )
	//{
	//	/*Removing WRITE_MASK.*/
	//	ACE_Reactor::instance ()->cancel_wakeup( this, ACE_Event_Handler::WRITE_MASK );
	//	
	//	return 0;
	//}
	//else
	//{

	//}


	return 0;
}


int ClientService::handle_close( ACE_HANDLE handle, ACE_Reactor_Mask close_mask )
{
	/*Release.*/
	if ( m_pRecvBuf != NULL )
	{
		m_pRecvBuf->release();
	}

	if ( m_pSendBuf != NULL )
	{
		m_pSendBuf->release();
	}

	delete this;

	return 0;
}

ACE_HANDLE ClientService::get_handle() const
{
	return m_peer.get_handle();
}

ACE_SOCK_Stream & ClientService::peer()
{
	return m_peer;
}
