// ParaSocket.h : header file
//

#pragma once

// for SCH_CREDENTIALS
#define	SCHANNEL_USE_BLACKLISTS
#include <Winternl.h>

// for security functions
#define SECURITY_WIN32
#include <security.h>

#define	SOCK_STATE_IDLE		0
#define	SOCK_STATE_CONNECTED	1
#define	SOCK_STATE_RECEIVED	2
#define	SOCK_STATE_SENT		3
#define	SOCK_STATE_ACCEPTED	4
#define	SOCK_STATE_FAILED	5

class CParaSocket : public CAsyncSocket
{
public:
			CParaSocket( void );
			~CParaSocket( void );

	virtual	BOOL	Connect( LPCTSTR lpszHostAddress, UINT nHostPort );
	virtual	void	Close( void );
	virtual	int	Send(    void* pbData, int cbData, int nFlags = 0 );
	virtual	int	Receive( void* pbData, int cbData, int nFlags = 0 );
	virtual	BOOL	IOCtl( long lCommand, DWORD* lpArgument );

		void	SetNotify( CWnd* pWnd, int nIdClient = 0, int nIdMessage = WM_SOCKET_NOTIFY );
		int	GetLastState( void ){ return m_nState; };

protected:
		CWnd*	m_pWndClient;
		int	m_nIdClient,
			m_iMessage;
		int	m_nState;

	virtual	void	OnAccept(  int nErrorCode );
	virtual	void	OnClose(   int nErrorCode );
	virtual	void	OnConnect( int nErrorCode );
	virtual	void	OnReceive( int nErrorCode );
	virtual	void	OnSend(    int nErrorCode );

		void	NotifyState( void );

		int	m_nPort;
		CString	m_strHost;
		bool	m_bReceiving;
		HANDLE	m_hReceived;
		int	m_iTLS;

	    CredHandle	m_hCred;
	    CtxtHandle	m_hContext;
	SecPkgContext_StreamSizes
			m_cbsContext;

		BYTE*	m_pbDecrypted;
		DWORD	m_cbDecrypted;
		BYTE*	m_pbEncrypted;
		DWORD	m_cbEncrypted;
		DWORD	m_cbPacketMax;

		bool	OnConnectTLS1( void );
		bool	OnConnectTLS2( void );
		void	OnReceiveTLS( void );

		int	ReceiveTLS( BYTE* pbData, DWORD cbData );
		int	SendTLS(    BYTE* pbData, DWORD cbData );
		void	CloseTLS( void );
		void	FinishTLS( SECURITY_STATUS status );

		void	EnqueueData( BYTE* pbData, DWORD cbData );
		BYTE*	DequeueData( DWORD cbData );
};
