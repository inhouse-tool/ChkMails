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
		if	( m_nState != SOCK_STATE_IDLE )
			CloseTLS();

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

#define	FLAGS_CRED	(SCH_USE_STRONG_CRYPTO | SCH_CRED_AUTO_CRED_VALIDATION | SCH_CRED_NO_DEFAULT_CREDS)
#define	FLAGS_ISC	(ISC_REQ_ALLOCATE_MEMORY | ISC_REQ_CONFIDENTIALITY | ISC_REQ_REPLAY_DETECT | ISC_REQ_SEQUENCE_DETECT | ISC_REQ_STREAM)

bool
CParaSocket::OnConnectTLS1( void )
{
	// Clear count of the encrypted data.

	m_cbEncrypted = 0;

	// Choice either SCH_CREDENTIALS ( TLS 1.3 ) or SCHANNEL_CRED ( TLS 1.2 ).

	void*	pAuthData = NULL;
	SCH_CREDENTIALS	cred13 = { 0 };
	SCHANNEL_CRED	cred12 = { 0 };

	if	( GetWinVer() >= 11 ){
		cred13.dwVersion = SCH_CREDENTIALS_VERSION;
		cred13.dwFlags = FLAGS_CRED;
		pAuthData = &cred13;
	}
	else{
		cred12.dwVersion = SCHANNEL_CRED_VERSION;
		cred12.dwFlags = FLAGS_CRED;
		pAuthData = &cred12;
	}

	// Acquire a handle to preexisting credentials.

	SECURITY_STATUS	status =
	AcquireCredentialsHandle( NULL, UNISP_NAME, SECPKG_CRED_OUTBOUND, NULL,
						pAuthData, NULL, NULL, &m_hCred, NULL );		
	if	( status != SEC_E_OK ){
		SetLastError( status );
		return	false;
	}

	bool	bDone = false;

	do{
		// Prepare the buffer for the outgoing message.
		// See https://learn.microsoft.com/en-us/windows/win32/secauthn/stream-contexts

		SecBuffer	asbOut[1] = { 0 };
		asbOut[0].BufferType = SECBUFFER_TOKEN;
		SecBufferDesc descOut = { SECBUFFER_VERSION, _countof( asbOut ), asbOut };

		// Initialize the security context.

		DWORD	dwFlags = FLAGS_ISC;
		status =
		InitializeSecurityContext( &m_hCred, NULL, (TCHAR*)m_strHost.GetString(), dwFlags,
						0, 0, NULL, 0, &m_hContext, &descOut, &dwFlags, NULL );
		
		// Continue: Send 'Client Hello'.

		if	( status == SEC_I_CONTINUE_NEEDED )
			bDone = SendBackTLS( asbOut[0].pvBuffer, asbOut[0].cbBuffer );
	}while	( 0 );

	// Done: Go ahead to the next stage to wait 'Server hello'.

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

	int	cbReceived = CAsyncSocket::Receive( m_pbEncrypted +m_cbEncrypted, m_cbPacketMax -m_cbEncrypted );
	if	( cbReceived <= 0 ){
		if	( cbReceived < 0 )
			FinishTLS( cbReceived );
		return	false;
	}

	m_cbEncrypted += cbReceived;

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
		DWORD	dwFlags = FLAGS_ISC;
		status =
		InitializeSecurityContext( &m_hCred, &m_hContext, (TCHAR*)m_strHost.GetString(), dwFlags,
						0, 0, &descIn, 0, NULL, &descOut, &dwFlags, NULL );

		// Done: Continue.

		if	( status == SEC_E_OK ||
			  status == SEC_I_CONTINUE_NEEDED )
			;

		// Imcomplete: Do next time.

		else if	( status == SEC_E_INCOMPLETE_MESSAGE )
			break;

		// Others: It's failed.

		else{
			bDone = false;
			break;
		}
	
		// Consume currently received data.

		ConsumeTLS( &descIn );

		// Completed: Get the sizes of each block of the stream.

		if	( status == SEC_E_OK ){
			QueryContextAttributes( &m_hContext, SECPKG_ATTR_STREAM_SIZES, &m_cbsContext );

			DWORD	cbPacket = m_cbsContext.cbHeader + m_cbsContext.cbMaximumMessage + m_cbsContext.cbTrailer;
			if	( cbPacket > m_cbPacketMax ){
				m_cbPacketMax = cbPacket;
				delete[]	m_pbEncrypted;
				m_pbEncrypted = new BYTE[m_cbPacketMax];
			}

			m_iTLS = 3;
			m_nState = SOCK_STATE_CONNECTED;
			NotifyState();
		}
	}while	( 0 );

	// Done: Send handshke response if any.

	if	( bDone ){
		if	( asbOut[0].cbBuffer > 0 )
			bDone = SendBackTLS( asbOut[0].pvBuffer, asbOut[0].cbBuffer );
	}

	// Failed: Clean up.

	else
		FinishTLS( status );

	return	bDone;
}

void
CParaSocket::OnReceiveTLS( void )
{
	// Do nothing if the buffer is full.

	if	( m_cbEncrypted == m_cbPacketMax )
		return;

	// Receive encrypted data.

	int	cbReceived = CAsyncSocket::Receive( m_pbEncrypted + m_cbEncrypted, m_cbPacketMax - m_cbEncrypted, 0 );
	if	( cbReceived <= 0 ){
		if	( cbReceived < 0 )
			FinishTLS( cbReceived );
		return;
	}

	m_cbEncrypted += cbReceived;

	// Consume encrypted data.

	bool	bDone = true;
	while	( m_cbEncrypted ){

		// Prepare the buffers to decrypt the encrypted data.

		SecBuffer asbIn[4] = { 0 };

		asbIn[0].BufferType = SECBUFFER_DATA;
		asbIn[0].pvBuffer   = m_pbEncrypted;
		asbIn[0].cbBuffer   = m_cbEncrypted;
		asbIn[1].BufferType = SECBUFFER_EMPTY;
		asbIn[2].BufferType = SECBUFFER_EMPTY;
		asbIn[3].BufferType = SECBUFFER_EMPTY;

		SecBufferDesc descIn = { SECBUFFER_VERSION, _countof( asbIn ), asbIn };

		// Decrypt the data.

		SECURITY_STATUS status = DecryptMessage( &m_hContext, &descIn, 0, NULL );

		// Completed: Store the decrypted message.

		if	( status == SEC_E_OK ){
			// Check if unprocessed data is implicated.
			// See https://learn.microsoft.com/en-us/windows/win32/secauthn/extra-buffers-returned-by-schannel

			int	i = SeekBufTLS( &descIn, SECBUFFER_DATA );
			if	( i >= 0 && asbIn[i].cbBuffer > 0 )
				EnqueueTLS( (BYTE*)asbIn[i].pvBuffer, asbIn[i].cbBuffer );

			(void)ConsumeTLS( &descIn );
		}

		// Session closed: Abandon encrypted data.

		else if	( status == SEC_I_CONTEXT_EXPIRED ){
			m_cbEncrypted = 0;
			break;
		}

		// Need more data: Wait for the next time.

		else if	( status == SEC_E_INCOMPLETE_MESSAGE )
			break;

		// Another context required: Apply the returned Extra as a token for a new context. ( TLS 1.3 )

		else if	( status == SEC_I_RENEGOTIATE ){
			void*	pbExtra = NULL;
			DWORD	cbExtra = 0;

			int	i = SeekBufTLS( &descIn, SECBUFFER_EXTRA );
			if	( i >= 0 ){
				pbExtra = asbIn[i].pvBuffer;
				cbExtra = asbIn[i].cbBuffer;
			}
			if	( !pbExtra )
				continue;

			asbIn[0].BufferType = SECBUFFER_TOKEN;
			asbIn[0].pvBuffer   = pbExtra;
			asbIn[0].cbBuffer   = cbExtra;
			asbIn[1].BufferType = SECBUFFER_EMPTY;
			asbIn[1].pvBuffer   = NULL;
			asbIn[1].cbBuffer   = 0;
			asbIn[2].BufferType = SECBUFFER_EMPTY;
			asbIn[2].pvBuffer   = NULL;
			asbIn[2].cbBuffer   = 0;
			asbIn[3].BufferType = SECBUFFER_EMPTY;
			asbIn[3].pvBuffer   = NULL;
			asbIn[3].cbBuffer   = 0;

			SecBuffer	asbOut[1] = { 0 };
			asbOut[0].BufferType = SECBUFFER_TOKEN;
			SecBufferDesc descOut = { SECBUFFER_VERSION, _countof( asbOut ), asbOut };

			DWORD	dwFlags = FLAGS_ISC;
			status =
			InitializeSecurityContext( &m_hCred, &m_hContext, (TCHAR*)m_strHost.GetString(), dwFlags,
							0, SECURITY_NATIVE_DREP, &descIn, 0, NULL, &descOut, &dwFlags, NULL );

			if	( status == SEC_E_OK ||
				  status == SEC_I_CONTINUE_NEEDED ){
				ConsumeTLS( &descIn );
				bDone = SendBackTLS( asbOut[0].pvBuffer, asbOut[0].cbBuffer );
			}
			else
				bDone = false;
		}

		// Others: It's failed.

		else
			bDone = false;

		// Failed: Clean up.

		if	( !bDone ){
			FinishTLS( status );
			break;
		}
	}

	if	( bDone ){
		m_nState = SOCK_STATE_RECEIVED;
		NotifyState();
	}
}

int
CParaSocket::ReceiveTLS( BYTE* pbData, DWORD cbData )
{
	int	cbRecv = 0;

	if	( cbData != 0 )
		if	( m_cbDecrypted ){
			int	cbToGet = min( cbData, m_cbDecrypted );
			BYTE*	pbGet = DequeueTLS( cbToGet );
			CopyMemory( pbData, pbGet, cbToGet );
			delete[] pbGet;

			pbData += cbToGet;
			cbData -= cbToGet;
			cbRecv += cbToGet;
		}

	return	cbRecv;
}

int
CParaSocket::SendTLS( BYTE* pbData, DWORD cbData )
{
	int	cchSent = cbData;

	while	( cbData > 0 ){
		int	cbToPut = min( cbData, m_cbsContext.cbMaximumMessage );

		if	( m_cbPacketMax ){
			char*	pbSend = new char[m_cbPacketMax];

			// Prepare the buffers to encrypt the message.

			SecBuffer asbOut[3];
			asbOut[0].BufferType = SECBUFFER_TOKEN;
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

			// Done: Send the encrypted data.

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
				if	( cbSent != SOCKET_ERROR ){
					pbData += cbToPut;
					cbData -= cbToPut;
				}
			}

			delete[] pbSend;

			// Failed: Clean up.

			if	( status != SEC_E_OK ){
				FinishTLS( status );
				return	SOCKET_ERROR;
			}
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

	DWORD	dwFlags = FLAGS_ISC;
	status =
	InitializeSecurityContext( &m_hCred, &m_hContext, (TCHAR*)m_strHost.GetString(), dwFlags,
					0, 0, NULL, 0, NULL, &descOut, &dwFlags, NULL );

	// Done: Send 'Close Notify'.

	if	( status == SEC_E_OK )
		(void)SendBackTLS( asbOut[0].pvBuffer, asbOut[0].cbBuffer );

	// Clean up anyway.

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

bool
CParaSocket::SendBackTLS( void* pbData, DWORD cbData )
{
	bool	bDone = true;

	if	( pbData ){
		BYTE*	pbSend = (BYTE*)pbData;

		while	( cbData > 0 ){
			int	cbSent = CAsyncSocket::Send( pbSend, cbData );
			if	( cbSent <= 0 ){
				bDone = false;
				break;
			}

			cbData -= cbSent;
			pbSend += cbSent;
		}

		FreeContextBuffer( pbData );
	}

	return	bDone;
}

bool
CParaSocket::ConsumeTLS( SecBufferDesc* pDesc )
{
	bool		bDone = true;
	SecBuffer*	pBuffers = pDesc->pBuffers;

	int	cbExtra = 0;
	int	i = SeekBufTLS( pDesc, SECBUFFER_EXTRA );
	if	( i >= 0 )
		cbExtra = pBuffers[i].cbBuffer;

	{
		int	cbConsumed = m_cbEncrypted - cbExtra;
		MoveMemory( m_pbEncrypted, m_pbEncrypted + cbConsumed, cbExtra );
		m_cbEncrypted = cbExtra;
	}

	return	bDone;
}

int
CParaSocket::SeekBufTLS( SecBufferDesc* pDesc, DWORD dwType )
{
	SecBuffer*	pBuffers = pDesc->pBuffers;
	DWORD		cBuffers = pDesc->cBuffers;

	for	( int i = 0; i < (int)cBuffers; i++ )
		if	( pBuffers[i].BufferType == dwType )
			return	i;

	return	-1;
}

void
CParaSocket::EnqueueTLS( BYTE* pbData, DWORD cbData )
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
CParaSocket::DequeueTLS( DWORD cbData )
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

// Consider using a ring buffer
// if you think that physical memory copying is expensive.
}

DWORD
CParaSocket::GetWinVer( void )
{
	DWORD	dwMajor = 0, dwMinor = 0;
	DWORD	dwBuild = 0;

	HMODULE	hNT = LoadLibraryEx( _T("ntdll.dll"), NULL, LOAD_LIBRARY_SEARCH_SYSTEM32 );
	if	( hNT ){
		void	(WINAPI *RtlGetNtVersionNumbers)( LPDWORD major, LPDWORD minor, LPDWORD build ) =
			(void (WINAPI *)( LPDWORD major, LPDWORD minor, LPDWORD build ))
			GetProcAddress( hNT, "RtlGetNtVersionNumbers" );
		
		RtlGetNtVersionNumbers( &dwMajor, &dwMinor, &dwBuild );
		dwBuild &= ~0xF0000000;	// Mask 'build type'.
		FreeLibrary( hNT );
	}

	DWORD	dwWinVer = dwMajor;

	if	( dwMajor == 10 )
		if	( dwBuild >= 22000 )
			dwWinVer = 11;

	return	dwWinVer;

// Use RtlGetVersion (wdm.h)
// or apply GetFileVersionInfo (winver.h) to kernel32.dll
// if you don't want to use undocumented API like RtlGetNtVersionNumbers.
}
