// MainWnd.h : header file
//

#pragma once

#include "ParaSocket.h"
#include "NotifyWnd.h"

class	CAccount{
public:
		CString	m_strHost;
		CString	m_strUser;
		CString	m_strPass;
		int	m_nPort;
		int	m_nError;

		CAccount( void )
		{
			m_nPort  = 0;
			m_nError = 0;
		}

		bool
		operator !=( const CAccount& src )
		{
			return
			m_strHost != src.m_strHost ||
			m_strUser != src.m_strUser ||
			m_strPass != src.m_strPass ||
			m_nPort   != src.m_nPort;
		}
};

class	CAttr{
public:
		enum{
			unsupported,
			Text,
			Multipart,
		}	m_iType;
		enum{
			uninterested,
			Plain,
			HTML,
		}	m_iSubType;
		enum{
			unencoded,
			Bit7,
			Bit8,
			Base64,
			Quoted,
			Binary
		}	m_iEncode;
		int	m_iCharset;
	     CStringA	m_strBoundary;
		CTime	m_time;
	     CStringA	m_strFrom,
			m_strSender;
	     CStringA	m_strZone;
		int	m_nAuth;
		DWORD	m_dwReason;
		CString	m_strLinks;
		CString	m_strFile;
		CAttr( void )
		{
			m_iType    = unsupported;
			m_iSubType = uninterested;
			m_iCharset = -1;
			m_iEncode  = Bit7;
			m_nAuth    = 0;
			m_dwReason = 0;
		}
};

class CMainWnd : public CWnd
{
public:
	CMainWnd( void );
	~CMainWnd( void );

protected:
		int	m_nPoll;
		bool	m_bSummary;
		bool	m_bTone;
		CString	m_strToneFile;
		CString	m_strLogPath;
		int	m_nLog;
		bool	m_bLogAll;

		int	m_nAuth;
		DWORD	m_dwCode;
		CString	m_strDomains;
		CString	m_strNames;
		CString	m_strRecipients;
		CString	m_strSenders;
		DWORD	m_dwSender;
		CString	m_strTimes;
		CString	m_strWhites;

		DWORD	m_dwAuthCache;
		CString	m_strTLDCache;

	NOTIFYICONDATA	m_nid;
		CFont	m_fontProp,
			m_fontFix;
		CWnd*	m_pWndModal;

	   CParaSocket*	m_pSocket;
	   CNotifyWnd*	m_pWndNotify;

		int	m_iUser;
		int	m_iPhase;
		int	m_iMessage,
			m_nMessage;
		int	m_nMail;

		HANDLE	m_hSummary;
		CString	m_strSummary,	m_strSummaryLast;
	       CStringA	m_strMail;

		CArray	<CAccount, CAccount&>
			m_aAccount;

	virtual	BOOL	DestroyWindow( void );

	afx_msg	int	OnCreate( LPCREATESTRUCT lpCreateStruct );
	afx_msg	void	OnTimer( UINT_PTR nIDEvent );
	afx_msg	LRESULT	OnGetFont(        WPARAM wParam, LPARAM lParam );
	afx_msg	LRESULT	OnSocketNotify(   WPARAM wParam, LPARAM lParam );
	afx_msg	LRESULT	OnUserTrayNotify( WPARAM wParam, LPARAM lParam );
	afx_msg	LRESULT	OnPopupNotify(    WPARAM wParam, LPARAM lParam );
	afx_msg	LRESULT	OnGetCommonFont(  WPARAM wParam, LPARAM lParam );
	afx_msg	LRESULT	OnGetSender(      WPARAM wParam, LPARAM lParam );
	afx_msg	LRESULT	OnShowHelp(       WPARAM wParam, LPARAM lParam );
	afx_msg	void	OnMenuWeb( void );
	afx_msg	void	OnMenuLicense( void );
	afx_msg	void	OnMenuAbout( void );
	afx_msg	void	OnMenuAccounts( void );
	afx_msg	void	OnMenuFilter( void );
	afx_msg	void	OnMenuLogs( void );
	afx_msg	void	OnMenuSetup( void );
	afx_msg	void	OnMenuExit( void );
	DECLARE_MESSAGE_MAP()

		bool	AddNI( void );
		void	DelNI( void );
		void	ModNI( UINT uIcon, CString strTip );

		void	SetMenu( void );

		bool	Introduce( void );

		void	LoadAccounts( void );
		void	SaveAccounts( void );
		void	LoadSettings( void );
		void	SaveSettings( void );
		void	LoadFilters( void );
		void	SaveFilters( void );
		void	LoadTLDs( void );
		void	SaveTLDs( void );

		HANDLE	m_hPower;
		int	m_nWakeDelay;
	static	ULONG	CALLBACK
			OnPower( void* pContext, ULONG uType, void* pSetting );

		void	PollNow( void );
		void	PollMails( void );
		bool	ParseMail( CStringA strMail, LPCTSTR pchFile = NULL );

		CAttr	GetAttr(   CStringA strMail );
		void	GetAuth(   CStringA strMail, CAttr& attr );
		void	GetFrom(   CStringA strMail, CAttr& attr );
		void	GetSender( CStringA strMail, CAttr& attr );
		void	GetType(   CStringA strMail, CAttr& attr );
		void	GetEncode( CStringA strMail, CAttr& attr );
		void	GetDate(   CStringA strMail, CAttr& attr );
		CTime	GetTime(   CStringA strDate, CAttr& attr );
		int	GetCodePage(   CStringA strMail );
		void	CheckToCc(     CStringA strMail, CAttr& attr );
		void	CheckMID(      CStringA strMail, CAttr& attr );
		void	CheckReceived( CStringA strMail, CAttr& attr );
		void	CheckBlackList( CStringA strSender, CAttr& attr );
	       CStringA	GetHeaderField( CStringA strMail, int iField );

		CString	MakeLog( CStringA strMail, CAttr& attr );
		void	SaveLog( CStringA strMail, CString strLog, CAttr& attr );
		bool	IsDuplicated( CString strFile, CTime time );
		CString	AddSuffix( CString strFile );
		void	TrimFiles( CString strPath, CString strFile, int nFile );
		void	MoveFiles( CString strPathFrom, CString strPathTo );

		CString	MakeSummary( CString strLog, CAttr attr );
		void	ShowSummary(  CString strSummary );
		void	ShareSummary( CString strSummary );

		CString	StringFromHeader( CStringA strIn, CAttr& attr );
		CString	StringFromBody(   CStringA strIn, CAttr& attr );
	       CStringA	DecodeBase64( CStringA strEncoded );
	       CStringA	DecodeQuoted( CStringA strEncoded );
		CString	StringFromCodePage( CStringA strIn, CAttr& attr );
	       CStringA	EscapeFromJIS(      CStringA strIn, CAttr& attr );
	       CStringA	StringToUTF8( CStringW strIn, CAttr& attr );
		void	LFtoCRLF(     CString& strLines );
		void	HexToUnicode( CString& strLines );

		void	CheckUnicode(   CString& strLog,   CAttr& attr );
		void	CheckAlias(     CString  strLog,   CAttr& attr );
		void	CheckSubject(   CString  strLog,   CAttr& attr );
		int	CompareWild( CString strWild, CString strName );
		void	NormalizeAlias( CString& strName );
		TCHAR	EvasiveToASCII( TCHAR ch );
		void	CheckWhiteList( CString  strLog,   CAttr& attr );
		void	CheckLink(      CString& strLog,   CAttr& attr );
		bool	GetLinkInHTML(  CString  strLog, int& xLines, CString& strLink, CString& strDisplay );
		void	GetLinkInText(  CString  strLog, int& xLines, CString& strLink );
		bool	IsEvasiveCode(  CString  strLog, int& xLines );
		void	SetLinkVisible( CString& strLink,  CAttr& attr );

		void	FilterError( UINT uIdError, CAttr& attr );

		void	ConnectPOP( void );
		void	RespondPOP( CStringA strMessage );
		void	ClosePOP( int nwError );

		bool	ReadFromEML( LPCTSTR pchFolder );
		void	ReadEML( CString strFile );
};
