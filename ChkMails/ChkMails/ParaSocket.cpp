// ParaSocket.cpp : implementation file
//

#include "pch.h"
#include "ParaSocket.h"

#include <schannel.h>
#include <shlwapi.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

///////////////////////////////////////////////////////////////////////////////////////
// Constructor & Destructor

CParaSocket::CParaSocket( void )
	: CAsyncSocket()
{
	m_pWndClient = AfxGetMainWnd();
	m_iMessage   = WM_SOCKET_NOTIFY;
	m_nIdClient  = 0;

	m_nState     = SOCK_STATE_IDLE;

	m_nPort      = 0;
	m_bReceiving = false;
	m_hReceived  = CreateEvent( NULL, FALSE, FALSE, NULL );

	m_iTLS        = 0;
	SecInvalidateHandle( &m_hCred );
	SecInvalidateHandle( &m_hContext );
	m_cbsContext  = { 0 };
	m_cbDecrypted = 0;
	m_pbDecrypted = NULL;
	m_pbEncrypted = NULL;
	m_cbEncrypted = 0;
	m_cbPacketMax = 0;
}

CParaSocket::~CParaSocket( void )
{
	if	( m_pbDecrypted )
		delete[] m_pbDecrypted;
	if	( m_pbEncrypted )
		delete[] m_pbEncrypted;
	if	( m_iTLS )
		FinishTLS( 0 );

	CloseHandle( m_hReceived );

	CAsyncSocket::~CAsyncSocket();
}

///////////////////////////////////////////////////////////////////////////////////////
// Public Methods

BOOL
CParaSocket::Connect( LPCTSTR lpszHostAddress, UINT nHostPort )
{
	m_strHost = lpszHostAddress;
	m_nPort   = nHostPort;

	if	( m_nPort == 443 ||	// https
		  m_nPort == 465 ||	// smtps
		  m_nPort == 993 ||	// imaps
		  m_nPort == 995 ){	// pop3s
		m_iTLS = 1;
	}
	else if	( (int)nHostPort < 0 ){
		nHostPort = -(int)nHostPort;
		m_iTLS = 1;
	}
	else
		m_iTLS = 0;

	if	( m_iTLS ){
		m_cbPacketMax = 16384+256;	// according to observed SecPkgContext_StreamSizes
		m_pbEncrypted = new BYTE[m_cbPacketMax];
		m_cbEncrypted = 0;
	}

	return	CAsyncSocket::Connect( lpszHostAddress, nHostPort );
}

void
CParaSocket::Close( void )
{
	if	( m_iTLS ){
		CloseTLS();

		m_iTLS = 0;
		delete[] m_pbEncrypted;
		m_cbPacketMax = 0;
		m_pbEncrypted = NULL;
		m_cbEncrypted = 0;
	}

	CAsyncSocket::Close();
}

int
CParaSocket::Send( void* pbData, int cbData, int nFlags )
{
	if	( m_iTLS )
		return	SendTLS( (BYTE*)pbData, cbData );
	else
		return	CAsyncSocket::Send( pbData, cbData, nFlags );
}

int
CParaSocket::Receive( void* pbData, int cbData, int nFlags )
{
	if	( m_iTLS )
		return	ReceiveTLS( (BYTE*)pbData, cbData );
	else
		return	CAsyncSocket::Receive( pbData, cbData, nFlags );
}

BOOL
CParaSocket::IOCtl( long lCommand, DWORD* lpArgument )
{
	if	( m_iTLS && ( lCommand == FIONREAD ) ){
		*lpArgument = m_cbDecrypted;
		return	TRUE;
	}
	else
		return	CAsyncSocket::IOCtl( lCommand, lpArgument );
}

void
CParaSocket::SetNotify( CWnd* pWnd, int nIdClient, int nIdMessage  )
{
	m_pWndClient = pWnd;
	m_nIdClient  = nIdClient;
	m_iMessage   = nIdMessage;
}

///////////////////////////////////////////////////////////////////////////////////////
// Overridden Functions

void
CParaSocket::OnAccept( int nErrorCode )
{
	m_nState = nErrorCode? SOCK_STATE_FAILED: SOCK_STATE_ACCEPTED;
	NotifyState();
}

void
CParaSocket::OnClose( int nErrorCode )
{
	m_nState = nErrorCode? SOCK_STATE_FAILED: SOCK_STATE_IDLE;
	NotifyState();
}

void
CParaSocket::OnConnect( int nErrorCode )
{
	m_nState = nErrorCode? SOCK_STATE_FAILED: SOCK_STATE_CONNECTED;

	if	( nErrorCode )
		NotifyState();
	else if	( m_iTLS == 1 )
		OnConnectTLS1();
	else
		NotifyState();
}

void
CParaSocket::OnReceive( int nErrorCode )
{
	m_nState = nErrorCode? SOCK_STATE_FAILED: SOCK_STATE_RECEIVED;

	if	( nErrorCode )
		NotifyState();
	else if	( m_iTLS == 2 )
		OnConnectTLS2();
	else if	( m_iTLS >= 3 )
		OnReceiveTLS();
	else
		NotifyState();
}

void
CParaSocket::OnSend( int nErrorCode )
{
	m_nState = nErrorCode? SOCK_STATE_FAILED: SOCK_STATE_SENT;
	NotifyState();
}

///////////////////////////////////////////////////////////////////////////////////////
// Internal Functions

void
CParaSocket::NotifyState( void ) 
{
	if	( m_pWndClient )
		m_pWndClient->PostMessage( m_iMessage, MAKEWPARAM( m_nIdClient, m_nState ), (LPARAM)this );
}

#pragma comment( lib, "Secur32.lib" )

bool
CParaSocket::OnConnectTLS1( void )
{
	bool	bDone = true;

	// Acquire a handle to preexisting credentials.

	SCH_CREDENTIALS	cred = { 0 };
	cred.dwVersion = SCH_CREDENTIALS_VERSION;
	cred.dwFlags = SCH_USE_STRONG_CRYPTO | SCH_CRED_AUTO_CRED_VALIDATION | SCH_CRED_NO_DEFAULT_CREDS;

	SECURITY_STATUS	status =
	AcquireCredentialsHandle( NULL, UNISP_NAME, SECPKG_CRED_OUTBOUND, NULL,
						&cred, NULL, NULL, &m_hCred, NULL );		
	if	( status != SEC_E_OK ){
		SetLastError( status );
		return	false;
	}

	do{
		// Prepare the buffer for the outgoing message.
		// See https://learn.microsoft.com/en-us/windows/win32/secauthn/stream-contexts

		SecBuffer	asbOut[1] = { 0 };
		asbOut[0].BufferType = SECBUFFER_TOKEN;
		SecBufferDesc descOut = { SECBUFFER_VERSION, _countof( asbOut ), asbOut };

		// Initialize the security context.

		DWORD	dwFlags = ISC_REQ_USE_SUPPLIED_CREDS | ISC_REQ_ALLOCATE_MEMORY | ISC_REQ_CONFIDENTIALITY |
				ISC_REQ_REPLAY_DETECT | ISC_REQ_SEQUENCE_DETECT | ISC_REQ_STREAM;
		status =
		InitializeSecurityContext( &m_hCred, NULL, m_strHost.GetBuffer(), dwFlags,
						0, 0, NULL, 0, &m_hContext, &descOut, &dwFlags, NULL );
		
		// Completed, but must do it again: Send 'Client Hello'.

		if	( status == SEC_I_CONTINUE_NEEDED ){
			char*	pbBuffer = (char*)asbOut[0].pvBuffer;
			int	cbBuffer  =       asbOut[0].cbBuffer;

			while	( cbBuffer > 0 ){
				m_cbEncrypted = 0;
				int	cbSent = CAsyncSocket::Send( pbBuffer, cbBuffer );
				if	( cbSent <= 0 ){
					bDone = false;
					break;
				}
				cbBuffer -= cbSent;
				pbBuffer += cbSent;
			}

			if	( asbOut[0].pvBuffer )
				FreeContextBuffer( asbOut[0].pvBuffer );

			if	( !bDone )
				break;
		}

		// Others ( failed ): Quit.

		else{
			bDone = false;
			break;
		}
	}while	( 0 );

	// Done: Go ahead to the next stage to wait 'Server hello".

	if	( bDone )
		m_iTLS = 2;

	// Failed: Clean up.

	else
		FinishTLS( status );

	return	bDone;
}

bool
CParaSocket::OnConnectTLS2( void )
{
	bool	bDone = true;

	// Receive 'Server Hello'.

	m_cbEncrypted += CAsyncSocket::Receive( m_pbEncrypted +m_cbEncrypted, m_cbPacketMax -m_cbEncrypted );

	// Prepare the buffers for the incoming message.

	SecBuffer	asbIn[4] = { 0 };
	asbIn[0].BufferType = SECBUFFER_TOKEN;
	asbIn[0].pvBuffer   = m_pbEncrypted;
	asbIn[0].cbBuffer   = m_cbEncrypted;
	asbIn[1].BufferType = SECBUFFER_EMPTY;
	asbIn[2].BufferType = SECBUFFER_EMPTY;
	asbIn[3].BufferType = SECBUFFER_EMPTY;
	SecBufferDesc descIn  = { SECBUFFER_VERSION, _countof( asbIn  ), asbIn  };

	// Prepare the buffer for the outgoing message.

	SecBuffer	asbOut[1] = { 0 };
	asbOut[0].BufferType = SECBUFFER_TOKEN;

	SecBufferDesc descOut = { SECBUFFER_VERSION, _countof( asbOut ), asbOut };

	// Initialize the security context.

	SECURITY_STATUS	status;

	do{
		DWORD	dwFlags = ISC_REQ_USE_SUPPLIED_CREDS | ISC_REQ_ALLOCATE_MEMORY | ISC_REQ_CONFIDENTIALITY |
				ISC_REQ_REPLAY_DETECT | ISC_REQ_SEQUENCE_DETECT | ISC_REQ_STREAM;
		status =
		InitializeSecurityContext( &m_hCred, &m_hContext, NULL, dwFlags,
						0, 0, &descIn, 0, NULL, &descOut, &dwFlags, NULL );

		// If any extra data has received, keep it in the buffer.

		if	( asbIn[1].BufferType == SECBUFFER_EXTRA ){
			int	cbExtra = asbIn[1].cbBuffer;
			MoveMemory( m_pbEncrypted, m_pbEncrypted + (m_cbEncrypted -cbExtra), cbExtra );
			m_cbEncrypted = cbExtra;
		}
		else
			m_cbEncrypted = 0;

		// Completed: Get the size of each block of the stream,
		//		then go ahead to the next stage to wait the app do something.

		if	( status == SEC_E_OK ){
			QueryContextAttributes( &m_hContext, SECPKG_ATTR_STREAM_SIZES, &m_cbsContext );

			m_iTLS = 3;
			m_nState = SOCK_STATE_CONNECTED;
			NotifyState();

			break;
		}

		// Incomplete: Continue.

		else if	( status == SEC_E_INCOMPLETE_MESSAGE )
			break;

		// Completed, but must do it again: Send the response.

		else if	( status == SEC_I_CONTINUE_NEEDED ){
			char*	pbBuffer = (char*)asbOut[0].pvBuffer;
			int	cbBuffer  =       asbOut[0].cbBuffer;

			if	( pbBuffer ){
				while	( cbBuffer ){
					int	cbSent = CAsyncSocket::Send( pbBuffer, cbBuffer );
					if	( cbSent <= 0 ){
						bDone = false;
						break;
					}
					cbBuffer -= cbSent;
					pbBuffer += cbSent;
				}
				FreeContextBuffer( asbOut[0].pvBuffer );
			}

			if	( !bDone )				
				break;
		}

		// Others ( failed ): Quit.

		else{
			bDone = false;
			TRACE( "OnConnectTLS2 failed in error %08x\n", status );
			break;
		}
	}while	( 0 );

	// Done: If some message has received, decrypt it.

	if	( bDone ){
		if	( m_cbEncrypted ){

			// Prepare the buffers for decrypting the message.

			asbIn[0].BufferType = SECBUFFER_DATA;
			asbIn[0].pvBuffer   = m_pbEncrypted;
			asbIn[0].cbBuffer   = m_cbEncrypted;
			asbIn[1].BufferType = SECBUFFER_EMPTY;
			asbIn[2].BufferType = SECBUFFER_EMPTY;
			asbIn[3].BufferType = SECBUFFER_EMPTY;

			// Decrypt the message.

			status = DecryptMessage( &m_hContext, &descIn, 0, NULL );

			// Store the decrypted message.

			if	( status == SEC_E_OK ){
				EnqueueData( (BYTE*)asbIn[1].pvBuffer, asbIn[1].cbBuffer );

				int	cbUnprocessd = 0;
				for	( int i = 0; i < _countof( asbIn ); i++ )
					if	( asbIn[i].BufferType == SECBUFFER_EXTRA ){
						cbUnprocessd = asbIn[i].cbBuffer;
						break;
					}
				int	cbValid = m_cbEncrypted - cbUnprocessd;

				if	( !cbUnprocessd ){
					MoveMemory( m_pbEncrypted, m_pbEncrypted +cbValid, m_cbEncrypted -cbValid );
					m_cbEncrypted -= cbValid;
				}

				m_nState = SOCK_STATE_RECEIVED;
				NotifyState();
				if	( m_bReceiving ){
					m_bReceiving = false;
					SetEvent( m_hReceived );
				}
			}
		}
	}

	// Failed: Clean up.

	else
		FinishTLS( status );

	return	bDone;
}

void
CParaSocket::OnReceiveTLS( void )
{
	// Do nothing if the buffer was full.

	if	( m_cbEncrypted == m_cbPacketMax )
		return;

	// Receive encrypted data.

	int	cbReceived = CAsyncSocket::Receive( m_pbEncrypted + m_cbEncrypted, m_cbPacketMax - m_cbEncrypted, 0 );
	if	( cbReceived <= 0 )
		return;

	m_cbEncrypted += cbReceived;

	while	( m_cbEncrypted ){

		// Prepare the buffers for decrypting the message.

		SecBuffer asbIn[4] = {};
		asbIn[0].BufferType = SECBUFFER_DATA;
		asbIn[0].pvBuffer   = m_pbEncrypted;
		asbIn[0].cbBuffer   = m_cbEncrypted;
		asbIn[1].BufferType = SECBUFFER_EMPTY;
		asbIn[2].BufferType = SECBUFFER_EMPTY;
		asbIn[3].BufferType = SECBUFFER_EMPTY;
		SecBufferDesc descIn = { SECBUFFER_VERSION, _countof( asbIn ), asbIn };

		// Decrypt the message.

		SECURITY_STATUS status = DecryptMessage( &m_hContext, &descIn, 0, NULL );

		// Completed: Store the decrypted message.

		if	( status == SEC_E_OK ){
			EnqueueData( (BYTE*)asbIn[1].pvBuffer, asbIn[1].cbBuffer );

			// Check if unprocessed data is implicated.
			// See https://learn.microsoft.com/en-us/windows/win32/secauthn/extra-buffers-returned-by-schannel

			int	cbUnprocessd = 0;
			if	( asbIn[3].BufferType == SECBUFFER_EXTRA )
				cbUnprocessd = asbIn[3].cbBuffer;

			int	cbValid = m_cbEncrypted - cbUnprocessd;

			MoveMemory( m_pbEncrypted, m_pbEncrypted + cbValid, m_cbEncrypted - cbValid );
			m_cbEncrypted -= cbValid;
			if	( cbUnprocessd )
				continue;
			else{
				m_nState = SOCK_STATE_RECEIVED;
				NotifyState();
				break;
			}
		}

		// Session closed: Abandon encrypted data.

		else if	( status == SEC_I_CONTEXT_EXPIRED ){
			m_cbEncrypted = 0;
			break;
		}

		// Need more data: Wait for the next OnReceive.

		else if	( status == SEC_E_INCOMPLETE_MESSAGE ){
			break;
		}

		// Others: Quit.

		else{
			FinishTLS( status );
			break;
		}
	}
}

int
CParaSocket::ReceiveTLS( BYTE* pbData, DWORD cbData )
{
	int	cbRecv = 0;

	while	( cbData != 0 )
		if	( m_cbDecrypted ){
			int	cbToGet = min( cbData, m_cbDecrypted );
			BYTE*	pbGet = DequeueData( cbToGet );
			CopyMemory( pbData, pbGet, cbToGet );
			delete[] pbGet;

			pbData += cbToGet;
			cbData -= cbToGet;
			cbRecv += cbToGet;
		}
		else{
			m_bReceiving = true;
			WaitForSingleObject( m_hReceived, INFINITE );
			continue;
		}

	return	cbRecv;
}

int
CParaSocket::SendTLS( BYTE* pbData, DWORD cbData )
{
	int	cchSent = cbData;

	while	( cbData > 0 ){
		int	cbToPut = min( cbData, m_cbsContext.cbMaximumMessage );

		char*	pbSend = new char[m_cbPacketMax];

		// Prepare the buffers for encrypting the message.

		SecBuffer asbOut[3];
		asbOut[0].BufferType = SECBUFFER_STREAM_HEADER;
		asbOut[0].pvBuffer   = pbSend;
		asbOut[0].cbBuffer   = m_cbsContext.cbHeader;
		asbOut[1].BufferType = SECBUFFER_DATA;
		asbOut[1].pvBuffer   = pbSend +m_cbsContext.cbHeader;
		asbOut[1].cbBuffer   = cbToPut;
		asbOut[2].BufferType = SECBUFFER_STREAM_TRAILER;
		asbOut[2].pvBuffer   = pbSend +m_cbsContext.cbHeader +cbToPut;
		asbOut[2].cbBuffer   = m_cbsContext.cbTrailer;
		SecBufferDesc	descOut = { SECBUFFER_VERSION, _countof( asbOut ), asbOut };

		// Encrypt the message.

		CopyMemory( asbOut[1].pvBuffer, pbData, cbToPut );
		SECURITY_STATUS	status = EncryptMessage( &m_hContext, 0, &descOut, 0 );

		// Send the encrypted message.

		if	( status == SEC_E_OK ){
			int	cbTotal = asbOut[0].cbBuffer + asbOut[1].cbBuffer + asbOut[2].cbBuffer;
			int	cbDone = 0;
			int	cbSent = -1;
			while	( cbDone != cbTotal ){
				cbSent = CAsyncSocket::Send( pbSend+cbDone, cbTotal-cbDone, 0 );
				if	( cbSent == SOCKET_ERROR )
					break;
				else
					cbDone += cbSent;
			}
			if	( cbSent != -1 ){
				pbData += cbToPut;
				cbData -= cbToPut;
			}
		}

		delete[] pbSend;

		if	( status != SEC_E_OK ){
			FinishTLS( status );
			return	SOCKET_ERROR;
		}
	}

	return	cchSent;
}

void
CParaSocket::CloseTLS( void )
{
	// Prepare the buffers for shut down.

	DWORD	type = SCHANNEL_SHUTDOWN;
	SecBuffer asbDown[1];
	asbDown[0].BufferType = SECBUFFER_TOKEN;
	asbDown[0].pvBuffer   = &type;
	asbDown[0].cbBuffer   = sizeof(type);

	SECURITY_STATUS	status;

	// Shut down the Schanel.

	SecBufferDesc descDown = { SECBUFFER_VERSION, _countof( asbDown ), asbDown };
	status = ApplyControlToken( &m_hContext, &descDown );

	// Prepare the buffer for the outgoing message.

	SecBuffer asbOut[1];
	asbOut[0].BufferType = SECBUFFER_TOKEN;
	SecBufferDesc descOut = { SECBUFFER_VERSION, _countof( asbOut ), asbOut };

	// Initialize the security context.

	DWORD	dwFlags = ISC_REQ_ALLOCATE_MEMORY | ISC_REQ_CONFIDENTIALITY |
				ISC_REQ_REPLAY_DETECT | ISC_REQ_SEQUENCE_DETECT | ISC_REQ_STREAM;
	status =
	InitializeSecurityContext( &m_hCred, &m_hContext, NULL, dwFlags,
					0, 0, &descOut, 0, NULL, &descOut, &dwFlags, NULL );

	// Send 'Close Notify'.

	if	( status == SEC_E_OK ){
		char*	pbBuffer = (char*)asbOut[0].pvBuffer;
		int	cbBuffer = asbOut[0].cbBuffer;
		while	( cbBuffer ){
			int	cbSent = CAsyncSocket::Send( pbBuffer, cbBuffer, 0 );
			if	( cbSent <= 0 )
				break;
			pbBuffer += cbSent;
			cbBuffer -= cbSent;

			// Insert a time to receive a TCP ACK. ( just for a clean capture log :-)
			Sleep( 50 );
		}
		FreeContextBuffer( asbOut[0].pvBuffer );
	}

	FinishTLS( status );
}

void
CParaSocket::FinishTLS( SECURITY_STATUS status )
{
	if	( SecIsValidHandle( &m_hContext ) ){
		DeleteSecurityContext( &m_hContext );
		SecInvalidateHandle( &m_hContext );
	}
	if	( SecIsValidHandle( &m_hCred ) ){
		FreeCredentialsHandle( &m_hCred );
		SecInvalidateHandle( &m_hCred );
	}

	if	( status != SEC_E_OK ){
		SetLastError( status );
		m_nState = SOCK_STATE_FAILED;
		NotifyState();
	}
}

void
CParaSocket::EnqueueData( BYTE* pbData, DWORD cbData )
{
	int	cbNew = m_cbDecrypted +cbData;
	BYTE*	pbNew = new BYTE[cbNew];

	CopyMemory( pbNew, m_pbDecrypted, m_cbDecrypted );
	CopyMemory( pbNew +m_cbDecrypted, pbData, cbData );

	delete[] m_pbDecrypted;
	m_pbDecrypted = pbNew;
	m_cbDecrypted = cbNew;
}

BYTE*
CParaSocket::DequeueData( DWORD cbData )
{
	int	cbNew = m_cbDecrypted -cbData;
	BYTE*	pbNew = cbNew? new BYTE[cbNew]: NULL;

	BYTE*	pbData = new BYTE[cbData];
	CopyMemory( pbData, m_pbDecrypted, cbData );
	if	( pbNew )
		CopyMemory( pbNew,  m_pbDecrypted +cbData, cbNew );

	delete[] m_pbDecrypted;
	m_pbDecrypted = pbNew;
	m_cbDecrypted = cbNew;

	return	pbData;
}
