// MainWnd.cpp : implementation file
//

#include "pch.h"
#include "ChkMails.h"
#include "MainWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#ifdef	UNICODE
#define	strcpy_s	wcscpy_s
#endif//UNICODE

#define	TID_INIT	1
#define	TID_READY	2
#define	TID_START	3
#define	TID_POLL	4
#define	TID_CLOSE	5

///////////////////////////////////////////////////////////////////////////////////////
// Constructor & Destructor

CMainWnd::CMainWnd( void )
	: CWnd()
{
	LoadSettings();
	LoadFilters();
	LoadTLDs();

	MakeBlob();

	m_pWndModal = NULL;

	m_pSocket  = NULL;
	m_pWndNotify = NULL;

	m_iPhase   = 0;
	m_iMessage = 0;
	m_nMessage = 0;
	m_iUser    = 0;
	m_nMail    = 0;

	m_hSummary = NULL;

	m_hPower     = NULL;
	m_nWakeDealy = 0;
}

CMainWnd::~CMainWnd( void )
{
	if	( m_hSummary )
		CloseHandle( m_hSummary );

	delete[] m_pbBlob;
}

///////////////////////////////////////////////////////////////////////////////////////
// Overridden Functions

BOOL
CMainWnd::DestroyWindow( void )
{
	DelNI();

	if	( m_pWndNotify ){
		m_pWndNotify->DestroyWindow();
		delete	m_pWndNotify;
		m_pWndNotify = NULL;
	}

	return	CWnd::DestroyWindow();
}

///////////////////////////////////////////////////////////////////////////////////////
// Message Handlers

BEGIN_MESSAGE_MAP( CMainWnd, CWnd )
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_MESSAGE( WM_GETFONT,          OnGetFont )
	ON_MESSAGE( WM_SOCKET_NOTIFY,    OnSocketNotify )
	ON_MESSAGE( WM_USER_TRAY_NOTIFY, OnUserTrayNotify )
	ON_MESSAGE( WM_POPUP_NOTIFY,     OnPopupNotify )
	ON_MESSAGE( WM_GET_COMMONFONT,   OnGetCommonFont )
	ON_MESSAGE( WM_GET_SENDER,       OnGetSender )
	ON_MESSAGE( WM_SHOW_HELP,        OnShowHelp )
	ON_COMMAND( ID_HELP_INDEX,    OnMenuWeb )
	ON_COMMAND( ID_HELP_USING,    OnMenuLicense )
	ON_COMMAND( ID_APP_ABOUT,     OnMenuAbout )
	ON_COMMAND( ID_MENU_ACCOUNTS, OnMenuAccounts )
	ON_COMMAND( ID_MENU_SETUP,    OnMenuSetup )
	ON_COMMAND( ID_MENU_FILTER,   OnMenuFilter )
	ON_COMMAND( ID_MENU_LOGS,     OnMenuLogs )
	ON_COMMAND( ID_MENU_EXIT,     OnMenuExit )
END_MESSAGE_MAP()

#include <powrprof.h>
#pragma comment( lib, "Powrprof.lib" )

int
CMainWnd::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
	if	( CWnd::OnCreate( lpCreateStruct ) == -1 )
		return	-1;

	// Make a font to share with PropertySheet.

	LOGFONT	lf = {};
	lf.lfHeight         = 9;
	lf.lfWeight         = FW_REGULAR;
	lf.lfCharSet        = ANSI_CHARSET;
	lf.lfPitchAndFamily = VARIABLE_PITCH | FF_SWISS;
	lf.lfQuality        = CLEARTYPE_QUALITY;
	strcpy_s( lf.lfFaceName, _T("Arial") );
	m_fontProp.CreateFontIndirect( &lf );

	// and a fixed-width font for edit boxes.

	lf.lfHeight         = 16;
	lf.lfWeight         = FW_REGULAR;
	lf.lfCharSet        = ANSI_CHARSET;
	lf.lfPitchAndFamily = FIXED_PITCH | FF_MODERN;
	lf.lfQuality        = CLEARTYPE_QUALITY;
	strcpy_s( lf.lfFaceName, _T("Consolas") );
	m_fontFix.CreateFontIndirect( &lf );

	// Create the notify window.

	m_pWndNotify = new CNotifyWnd;

	m_nWakeDealy = AfxGetApp()->GetProfileInt( _T("Settings"), _T("WakeDelay"),   5 );
	DEVICE_NOTIFY_SUBSCRIBE_PARAMETERS	dnsp = { OnPower, this };
	PowerRegisterSuspendResumeNotification( DEVICE_NOTIFY_CALLBACK, &dnsp, &m_hPower );

	// Set the first stage timer.

	SetTimer( TID_INIT, 0, NULL );

	return	0;
}

void
CMainWnd::OnTimer( UINT_PTR nIDEvent )
{
	if	( nIDEvent == TID_INIT ){
		KillTimer( nIDEvent );
		SetTimer( TID_READY, 0, NULL );
		if	( !Introduce() )
			PostMessage( WM_CLOSE );
	}
	else if	( nIDEvent == TID_READY ){
		KillTimer( nIDEvent );
		if	( AddNI() )
			SetTimer( TID_START,      0, NULL );
		else
			SetTimer( TID_READY, 2*1000, NULL );
	}
	else if	( nIDEvent == TID_START ){
		KillTimer( nIDEvent );
		PollNow();
	}
	else if	( nIDEvent == TID_POLL ){
		PollMails();
	}
	else if	( nIDEvent == TID_CLOSE ){
		KillTimer( nIDEvent );
		RespondPOP( NULL );
	}
	else
		CWnd::OnTimer( nIDEvent );
}

LRESULT
CMainWnd::OnGetFont( WPARAM wParam, LPARAM lParam )
{
	return	(LRESULT)m_fontProp.m_hObject;
}

LRESULT
CMainWnd::OnSocketNotify( WPARAM wParam, LPARAM lParam )
{
	CParaSocket*	pSocket = (CParaSocket*)lParam;
	int		nState = HIWORD( wParam );
	int		nCode  = LOWORD( wParam );

	if	( nState == SOCK_STATE_FAILED ||
		  nState == SOCK_STATE_IDLE ){
		SetTimer( TID_CLOSE, 0, NULL );
	}
	else if	( nState == SOCK_STATE_RECEIVED ){
		UINT	nMessage = 0;
		pSocket->IOCtl( FIONREAD, (ULONG*)&nMessage );

		if	( nMessage > 0 ){
#pragma warning(disable: 26451)
			char*	pMessage  = new char[nMessage+1];
			pSocket->Receive( pMessage, nMessage );
			pMessage[nMessage] = '\0';
			CStringA	strMessage = pMessage;
			delete[] pMessage;

			RespondPOP( strMessage );
#pragma warning(default: 26451)
		}
	}

	return	0;
}

LRESULT
CMainWnd::OnUserTrayNotify( WPARAM wParam, LPARAM lParam )
{
	switch	( lParam ){

		case	WM_LBUTTONUP:
			PollNow();
			break;

		case	WM_RBUTTONUP:
			if	( m_pWndModal ){
				m_pWndModal->FlashWindowEx( FLASHW_ALL, 3, 50 );
				m_pWndModal->SetForegroundWindow();
			}
			else
				SetMenu();
			break;
	}

	return	0;
}

#pragma comment( lib, "Dwmapi.lib" )

LRESULT
CMainWnd::OnPopupNotify( WPARAM wParam, LPARAM lParam )
{
	HWND	hWnd = (HWND)lParam;
	CWnd*	pWnd = FromHandlePermanent( hWnd );
	m_pWndModal = pWnd;

	if	( !pWnd )
		return	0;

	CRect	rectPopup;
	pWnd->GetWindowRect( &rectPopup );
	int	cx = rectPopup.Width();
	int	cy = rectPopup.Height();

	HMONITOR hMonitor = MonitorFromPoint( CPoint( 0, 0 ), MONITOR_DEFAULTTOPRIMARY );
	MONITORINFO mi = {};
	mi.cbSize = sizeof( mi );
	GetMonitorInfo( hMonitor, &mi );

	enum{
		unknown, left, top, right, bottom
	}	ePos = bottom;

	if	( mi.rcMonitor.left   < mi.rcWork.left )
		ePos = left;
	else if	( mi.rcMonitor.top    < mi.rcWork.top )
		ePos = top;
	else if	( mi.rcMonitor.right  > mi.rcWork.right )
		ePos = right;
	else if	( mi.rcMonitor.bottom > mi.rcWork.bottom )
		ePos = bottom;

	int	x, y;

	if	( ePos == left ){
		x = mi.rcWork.left;
		y = mi.rcWork.bottom - cy;
	}
	else if	( ePos == top ){
		x = mi.rcWork.right  - cx;
		y = mi.rcWork.top;
	}
	else if	( ePos == right ){
		x = mi.rcWork.right  - cx;
		y = mi.rcWork.bottom - cy;
	}
	else if	( ePos == bottom ){
		x = mi.rcWork.right  - cx;
		y = mi.rcWork.bottom - cy;
	}

	RECT	rcFrame = {};
	HRESULT	hr = DwmGetWindowAttribute( hWnd, DWMWA_EXTENDED_FRAME_BOUNDS, &rcFrame, sizeof( rcFrame ) );

	if	( hr == S_OK ){
		int	dx = rcFrame.left     - rectPopup.left;
		int	dy = rectPopup.bottom - rcFrame.bottom;

		if	( ePos == left ){
			x -= dx;
			y += dy;
		}
		else if	( ePos == top ){
			x += dx;
			y -= dy;
		}
		else if	( ePos == right ){
			x += dx;
			y += dy;
		}
		else if	( ePos == bottom ){
			x += dx;
			y += dy;
		}
	}

	pWnd->SetWindowPos( NULL, x, y, 0, 0, SWP_NOZORDER | SWP_NOSIZE );

	if	( m_pWndNotify->IsWindowVisible() )
		m_pWndNotify->ShowWindow( SW_HIDE );

	return	0;
}

LRESULT
CMainWnd::OnGetCommonFont( WPARAM wParam, LPARAM lParam )
{
	return	(LRESULT)&m_fontFix;
}

LRESULT
CMainWnd::OnGetSender( WPARAM wParam, LPARAM lParam )
{
	CStringA strMail( (char*)lParam );

	CAttr	attr = GetAttr( strMail );
	if	( attr.m_dwReason )
		return	(LRESULT)0;

	if	( attr.m_strSender.IsEmpty() )
		attr.m_strSender = attr.m_strFrom;

	CStringA strPair;
	strPair += attr.m_strFrom;
	strPair += "\n";
	strPair += attr.m_strSender;
	strPair += "\n";

	char*	pchAnswer = new char[strPair.GetLength()+1];
	memcpy( pchAnswer, strPair.GetBuffer(), strPair.GetLength()+1 );

	return	(LRESULT)pchAnswer;
}

LRESULT
CMainWnd::OnShowHelp( WPARAM wParam, LPARAM lParam )
{
	HWND	hWnd = (HWND)lParam;
	CWnd*	pWnd = FromHandle( hWnd );
	CString	strURL;
	pWnd->GetWindowText( strURL );
	strURL.MakeLower();
	strURL.Replace( ' ', '-' );

	CString	strPath = _T("https://github.com/inhouse-tool/ChkMails/blob/main/README.md");
	strURL.Insert( 0, _T("#") );
	strURL.Insert( 0, strPath );
	ShellExecute( NULL, _T("open"), strURL, NULL, NULL, SW_SHOWNORMAL );

	return	0;
}

void
CMainWnd::OnMenuWeb( void )
{
	CString	strPath = _T("https://github.com/inhouse-tool/ChkMails/blob/main/README.md");
	ShellExecute( NULL, _T("open"), strPath, NULL, NULL, SW_SHOWNORMAL );
}

void
CMainWnd::OnMenuLicense( void )
{
	TCHAR*	pchText =
		_T("MIT License\n")
		_T("\n")
		_T("Copyright (c) 2025 In-house Tool\n")
		_T("\n")
		_T("Permission is hereby granted, free of charge, to any person obtaining a copy\n")
		_T("of this software and associated documentation files (the \"Software\"), to deal\n")
		_T("in the Software without restriction, including without limitation the rights\n")
		_T("to use, copy, modify, merge, publish, distribute, sublicense, and/or sell\n")
		_T("copies of the Software, and to permit persons to whom the Software is\n")
		_T("furnished to do so, subject to the following conditions:\n")
		_T("\n")
		_T("The above copyright notice and this permission notice shall be included in all\n")
		_T("copies or substantial portions of the Software.\n")
		_T("\n")
		_T("THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR\n")
		_T("IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,\n")
		_T("FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE\n")
		_T("AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER\n")
		_T("LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,\n")
		_T("OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE\n")
		_T("SOFTWARE.\n\n")
		;

	AfxMessageBox( pchText, 0, MB_OK );
}

void
CMainWnd::OnMenuAbout( void )
{
	CChkMailsApp*	pApp = (CChkMailsApp*)AfxGetApp();
	CString	strText = pApp->GetVersionInfo( _T("%P\n%D\nVersion %V\n\n%L\n") );
	strText.Replace( _T("(C)"), _T("\x00a9") );

	AfxMessageBox( strText.GetBuffer(), MB_ICONMASK, MB_OK );
}

#include "AccountDlg.h"

void
CMainWnd::OnMenuAccounts( void )
{
	CAccountDlg	dlg;

	CArray	<CAccount, CAccount&>
		aAccount;
	aAccount.Append( m_aAccount );

	dlg.SetOwner( this );
	dlg.SetAccounts( aAccount );

	if	( dlg.DoModal() != IDOK )
		return;

	dlg.GetAccounts( aAccount );

	bool	bUpdate = false;

	INT_PTR	i, n = aAccount.GetCount();
	if	( n != m_aAccount.GetCount() )
		bUpdate = true;
	else{
		for	( i = 0; i < n; i++ )
			if	( aAccount[i] != m_aAccount[i] ){
				bUpdate = true;
				break;
			}
	}

	if	( bUpdate ){
		m_aAccount.RemoveAll();
		m_aAccount.Append( aAccount );
		SaveAccounts();

		if	( m_aAccount.GetCount() ){
			PollNow();
		}
		else{
			CString	strTip;
			(void)strTip.LoadString( IDS_NI_NOACCOUNTS );
			ModNI( IDR_MAINFRAME, strTip );
		}
	}
}

#include "FilterSheet.h"

void
CMainWnd::OnMenuFilter( void )
{
	CFilterSheet	sheet( IDS_CAPT_FILTER, this, 0 );

	sheet.m_pageAuth  .m_nAuth       = m_nAuth;
	sheet.m_pageAuth  .m_dwFlags     = m_dwAuthCache;
	sheet.m_pageCode  .m_dwFlags     = m_dwCode;
	sheet.m_pageDomain.m_strDomains  = m_strDomains;
	sheet.m_pageDomain.m_strTLDCache = m_strTLDCache;
	sheet.m_pageName  .m_strNames    = m_strNames;
	sheet.m_pageSender.m_dwFlags     = m_dwSender;
	sheet.m_pageZone  .m_strTimes    = m_strTimes;
	sheet.m_pageWhite .m_strWhites   = m_strWhites;

	sheet.SetOwner( this );

	if	( sheet.DoModal() != IDOK )
		return;

	bool	bUpdate = false;

	if	( sheet.m_pageAuth  .m_nAuth       != m_nAuth ){
		m_nAuth    = sheet.m_pageAuth        .m_nAuth;
		bUpdate = true;
	}
	if	( sheet.m_pageCode  .m_dwFlags     != m_dwCode ){
		m_dwCode   = sheet.m_pageCode        .m_dwFlags;
		bUpdate = true;
	}
	if	( sheet.m_pageDomain.m_strDomains  != m_strDomains ){
		m_strDomains  = sheet.m_pageDomain   .m_strDomains;
		bUpdate = true;
	}
	if	( sheet.m_pageDomain.m_strTLDCache != m_strTLDCache ){
		m_strTLDCache  = sheet.m_pageDomain  .m_strTLDCache;
		SaveTLDs();
	}
	if	( sheet.m_pageName  .m_strNames    != m_strNames ){
		m_strNames  = sheet.m_pageName       .m_strNames;
		bUpdate = true;
	}
	if	( sheet.m_pageSender.m_dwFlags     != m_dwSender ){
		m_dwSender = sheet.m_pageSender      .m_dwFlags;
		bUpdate = true;
	}
	if	( sheet.m_pageZone  .m_strTimes    != m_strTimes ){
		m_strTimes = sheet.m_pageZone        .m_strTimes;
		bUpdate = true;
	}
	if	( sheet.m_pageWhite .m_strWhites   != m_strWhites ){
		m_strWhites = sheet.m_pageWhite      .m_strWhites;
		bUpdate = true;
	}

	if	( bUpdate ){
		SaveFilters();
		if	( AfxMessageBox( IDS_MB_REEVAL, MB_YESNO | MB_ICONQUESTION ) == IDYES )
			ReadFromEML( m_strLogPath );
	}
}

void
CMainWnd::OnMenuLogs( void )
{
	ShellExecute( NULL, _T("explore"), m_strLogPath, NULL, NULL, SW_SHOWNORMAL );
}

#include "SetupDlg.h"

void
CMainWnd::OnMenuSetup( void )
{
	CSetupDlg	dlg;

	dlg.m_nPoll       = m_nPoll;
	dlg.m_bSummary    = m_bSummary;
	dlg.m_bTone       = m_bTone;
	dlg.m_strToneFile = m_strToneFile;
	dlg.m_strLogPath  = m_strLogPath;
	dlg.m_nLog        = m_nLog;
	dlg.m_bLogAll     = m_bLogAll;

	dlg.SetOwner( this );

	if	( dlg.DoModal() != IDOK )
		return;

	bool	bUpdate = false;

	if	( dlg.m_nPoll != m_nPoll ){
		m_nPoll       = dlg.m_nPoll;
		PollNow();
		bUpdate = true;
	}
	if	( dlg.m_bSummary != m_bSummary ){
		m_bSummary    = dlg.m_bSummary;
		bUpdate = true;
	}
	if	( dlg.m_bTone != m_bTone ){
		m_bTone       = dlg.m_bTone;
		bUpdate = true;
	}
	if	( dlg.m_strToneFile != m_strToneFile ){
		m_strToneFile = dlg.m_strToneFile;
		bUpdate = true;
	}
	if	( dlg.m_strLogPath != m_strLogPath ){
		if	( dlg.m_strLogPath.Right( 1 ) != _T("\\") )
			dlg.m_strLogPath += _T("\\");
		MoveFiles( m_strLogPath, dlg.m_strLogPath );
		m_strLogPath  = dlg.m_strLogPath;
		bUpdate = true;
	}
	if	( dlg.m_nLog != m_nLog ){
		m_nLog        = dlg.m_nLog;
		TrimFiles( m_strLogPath, _T("*.txt"), m_nLog );
		TrimFiles( m_strLogPath, _T("*.eml"), m_nLog );
		bUpdate = true;
	}
	if	( dlg.m_bLogAll != m_bLogAll ){
		m_bLogAll     = dlg.m_bLogAll;
		bUpdate = true;
	}

	if	( bUpdate )
		SaveSettings();
}

void
CMainWnd::OnMenuExit( void )
{
	if	( AfxMessageBox( IDS_MB_EXIT, MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2 ) == IDYES )
		PostMessage( WM_CLOSE, 0, 0 );
}

///////////////////////////////////////////////////////////////////////////////////////
// Specific Functions

bool
CMainWnd::AddNI( void )
{
	CString	strTip;
	(void)strTip.LoadString( IDS_NI_NOACCOUNTS );

	memset( &m_nid, 0, sizeof( m_nid ) );
	m_nid.cbSize = sizeof( NOTIFYICONDATA );
	m_nid.hWnd = m_hWnd;
	m_nid.uID = 1;
	m_nid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
	m_nid.uCallbackMessage = WM_USER_TRAY_NOTIFY;
	m_nid.hIcon = AfxGetApp()->LoadIcon( IDR_MAINFRAME );
	strcpy_s( m_nid.szTip, strTip );

	if	( Shell_NotifyIcon( NIM_ADD, &m_nid ) ){
		ShareSummary( 0 );
		return	true;
	}
	else{
		m_nid.cbSize = 0;
		return	false;
	}
}

void
CMainWnd::DelNI( void )
{
	if	( m_nid.cbSize ){
		Shell_NotifyIcon( NIM_DELETE, &m_nid );
		ShareSummary( NULL );
	}
}

#include <mmsystem.h>			// for PlaySound
#pragma comment( lib, "winmm.lib" )

void
CMainWnd::ModNI( UINT uIcon, CString strTip )
{
	if	( !uIcon ){
		if	( m_strSummary != m_strSummaryLast ){
			m_strSummaryLast = m_strSummary;
			if	( m_bSummary )
				ShowSummary( m_strSummary );
			if	( m_bTone )
				PlaySound( m_strToneFile, NULL, SND_FILENAME | SND_ASYNC );
		}
	}

	if	( strTip.IsEmpty() ){
		if	( m_nMail == 0 )
			(void)strTip.LoadString( IDS_NI_0MAIL );
		else if	( m_nMail == 1 )
			(void)strTip.LoadString( IDS_NI_1MAIL );
		else{
			CString	strFormat;
			(void)strFormat.LoadString( IDS_NI_NMAIL );
			strTip.Format( strFormat, m_nMail );
		}
		uIcon = m_nMail? IDI_BUSY: IDI_IDLE;
	}

	m_nid.uFlags = NIF_TIP | NIF_ICON;
	strcpy_s( m_nid.szTip, strTip );
	m_nid.hIcon = AfxGetApp()->LoadIcon( uIcon );
	Shell_NotifyIcon( NIM_MODIFY, &m_nid );

	ShareSummary( m_strSummary );
}

void
CMainWnd::SetMenu( void )
{
	CMenu	menu;
	menu.LoadMenu( IDR_MENU_POPUP );
	CMenu*	pPopup = menu.GetSubMenu( 0 );

	SetForegroundWindow();

	CPoint	ptIcon;
	GetCursorPos( &ptIcon );
	pPopup->TrackPopupMenu( TPM_RIGHTALIGN | TPM_LEFTBUTTON, ptIcon.x, ptIcon.y, this );

	PostMessage( WM_NULL, 0, 0 );
}

bool
CMainWnd::Introduce( void )
{
	LoadAccounts();

	if	( m_aAccount.GetCount() > 0 )
		return	true;

	if	( AfxMessageBox( IDS_MB_INTRODUCE, MB_ICONMASK | MB_YESNO ) == IDNO )
		return	false;

	SendMessage( WM_COMMAND, ID_MENU_ACCOUNTS, 0 );

	return	true;
}

void
CMainWnd::LoadAccounts( void )
{
	m_aAccount.RemoveAll();

	CChkMailsApp*	pApp = (CChkMailsApp*)AfxGetApp();

	for	( int i = 0; ; i++ ){
		BYTE*	pbRecord = NULL;
		DWORD	cbRecord = 0;
		CString	strEntry;
		strEntry.Format( _T("Entry%d"), i );
		pApp->GetProfileBinary( _T("Records"), strEntry, &pbRecord, (UINT*)&cbRecord );
		if	( !pbRecord )
			break;

		char*	pszRecord = NULL;
		DWORD	dwError = EnDecrypt( pszRecord, pbRecord, cbRecord, false );
		CStringA strRecord = pszRecord;
		delete[] pszRecord;
		delete[] pbRecord;

		CAccount	account;
		CStringA str;
		int	x = 0;
		account.m_strHost = strRecord.Tokenize( "\n", x );
		account.m_strUser = strRecord.Tokenize( "\n", x );
		account.m_strPass = strRecord.Tokenize( "\n", x );
		account.m_nPort   = atoi( strRecord.Tokenize( "\n", x ) );
		m_aAccount.Add( account );
	}
}

void
CMainWnd::SaveAccounts( void )
{
	CChkMailsApp*	pApp = (CChkMailsApp*)AfxGetApp();

	INT_PTR	n = m_aAccount.GetCount();
	int	i;
	for	( i = 0; i < n; i++ ){
		CStringA	strRecord;
		strRecord += m_aAccount[i].m_strHost;
		strRecord += "\n";
		strRecord += m_aAccount[i].m_strUser;
		strRecord += "\n";
		strRecord += m_aAccount[i].m_strPass;
		strRecord += "\n";
		CStringA str;
		str.Format( "%d\n", m_aAccount[i].m_nPort );
		strRecord += str;

		BYTE*	pbRecord = NULL;
		DWORD	cbRecord = 0;
		char*	pszRecord = strRecord.GetBuffer();
		DWORD	dwError = EnDecrypt( pszRecord, pbRecord, cbRecord, true );
		CString	strEntry;
		strEntry.Format( _T("Entry%d"), i );
		pApp->WriteProfileBinary( _T("Records"), strEntry, pbRecord, (UINT)cbRecord );
		delete[] pbRecord;
	}
	for	( ;; i++ ){
		CString	strEntry;
		strEntry.Format( _T("Entry%d"), i );
		if	( !pApp->DeleteProfileValue( _T("Records"), strEntry ) )
			break;
	}
}

void
CMainWnd::LoadSettings( void )
{
	CChkMailsApp*	pApp = (CChkMailsApp*)AfxGetApp();

	m_nPoll       = pApp->GetProfileInt(    _T("Settings"), _T("Poll"),     10 );

	m_bSummary    = pApp->GetProfileInt(    _T("Settings"), _T("Summary"),   0 );
	m_bTone       = pApp->GetProfileInt(    _T("Settings"), _T("Tone"),      0 );
	m_strToneFile = pApp->GetProfileString( _T("Settings"), _T("TonePath"), _T("C:\\Windows\\Media\\Windows Notify Email.wav") );

	TCHAR	achPath[_MAX_PATH] = { 0 };
	GetTempPath( _countof( achPath ), achPath );
	m_strLogPath  = achPath;
	m_strLogPath += pApp->m_pszExeName;
	m_strLogPath += _T("\\");
	m_strLogPath  = pApp->GetProfileString( _T("Settings"), _T("LogPath"), m_strLogPath );
	m_nLog        = pApp->GetProfileInt(    _T("Settings"), _T("LogKeep"), 100 );
	m_bLogAll     = pApp->GetProfileInt(    _T("Settings"), _T("LogAll"),    1 );
}

void
CMainWnd::SaveSettings( void )
{
	CChkMailsApp*	pApp = (CChkMailsApp*)AfxGetApp();

	pApp->WriteProfileInt(    _T("Settings"), _T("Summary"),  m_bSummary? 1: 0 );
	pApp->WriteProfileInt(    _T("Settings"), _T("Tone"),     m_bTone?    1: 0 );
	pApp->WriteProfileString( _T("Settings"), _T("TonePath"), m_strToneFile );
	pApp->WriteProfileInt(    _T("Settings"), _T("Poll"),     m_nPoll );
	pApp->WriteProfileString( _T("Settings"), _T("LogPath"),  m_strLogPath );
	pApp->WriteProfileInt(    _T("Settings"), _T("LogKeep"),  m_nLog );
	pApp->WriteProfileInt(    _T("Settings"), _T("LogAll"),   m_bLogAll? 1: 0 );
}

void
CMainWnd::LoadFilters( void )
{
	CChkMailsApp*	pApp = (CChkMailsApp*)AfxGetApp();

	m_nAuth       = pApp->GetProfileInt(    _T("Filters"), _T("AuthFail"), 2 );
	m_dwAuthCache = pApp->GetProfileInt(    _T("Filters"), _T("AuthBits"), 0x0000 );
	m_dwCode      = pApp->GetProfileInt(    _T("Filters"), _T("Code"),     0xffff );
	m_strDomains  = pApp->GetProfileString( _T("Filters"), _T("Drop"),     _T("") );
	m_strNames    = pApp->GetProfileString( _T("Filters"), _T("Name"),     _T("") );
	m_dwSender    = pApp->GetProfileInt(    _T("Filters"), _T("Sender"),   0xffff );
	m_strTimes    = pApp->GetProfileString( _T("Filters"), _T("TimeZone"), _T("") );
	m_strWhites   = pApp->GetProfileString( _T("Filters"), _T("White"),    _T("") );
}

void
CMainWnd::SaveFilters( void )
{
	CChkMailsApp*	pApp = (CChkMailsApp*)AfxGetApp();

	pApp->WriteProfileInt(    _T("Filters"), _T("AuthFail"), m_nAuth );
	pApp->WriteProfileInt(    _T("Filters"), _T("AuthBits"), m_dwAuthCache );
	pApp->WriteProfileInt(    _T("Filters"), _T("Code"),     m_dwCode );
	pApp->WriteProfileString( _T("Filters"), _T("Drop"),     m_strDomains );
	pApp->WriteProfileString( _T("Filters"), _T("Name"),     m_strNames );
	pApp->WriteProfileInt(    _T("Filters"), _T("Sender"),   m_dwSender );
	pApp->WriteProfileString( _T("Filters"), _T("TimeZone"), m_strTimes );
	pApp->WriteProfileString( _T("Filters"), _T("White"),    m_strWhites );
}

void
CMainWnd::LoadTLDs( void )
{
	CChkMailsApp*	pApp = (CChkMailsApp*)AfxGetApp();

	m_strTLDCache   = pApp->GetProfileString( _T("Filters"), _T("TLDs"),  _T(".com\n") );
}

void
CMainWnd::SaveTLDs( void )
{
	CChkMailsApp*	pApp = (CChkMailsApp*)AfxGetApp();

	pApp->WriteProfileString( _T("Filters"), _T("TLDs"), m_strTLDCache );
}

#include <bcrypt.h>
#pragma comment( lib, "Bcrypt.lib" )

void
CMainWnd::MakeBlob( void )
{
	m_pbBlob = NULL;
	m_cbBlob = 0;
	AfxGetApp()->GetProfileBinary( _T("Data"), _T("Blob"), &m_pbBlob, &m_cbBlob );
	if	( !m_pbBlob ){
		m_cbBlob = 192;
		m_pbBlob = new BYTE[m_cbBlob];
		(void)BCryptGenRandom( NULL, m_pbBlob, m_cbBlob, BCRYPT_USE_SYSTEM_PREFERRED_RNG );
		AfxGetApp()->WriteProfileBinary( _T("Data"), _T("Blob"), m_pbBlob, m_cbBlob );
	}
}

#define	OFS_IV	 64
#define	OFS_SK	128

DWORD
CMainWnd::EnDecrypt( char*& lpszPlain, BYTE*& pbCoded, DWORD& cbCoded, bool bEncode )
{
	BCRYPT_ALG_HANDLE	hAlgorithm = NULL;
	BCRYPT_KEY_HANDLE	hKey = NULL;
	BYTE*	pbObject = NULL;
	BYTE*	pbBlock = NULL;
	BYTE*	pbPlain = NULL;

	do{
		// Initialize CNG for AES algorithm. ( CNG: Cryptography API Next Generation )

		NTSTATUS	status;
		status = BCryptOpenAlgorithmProvider( &hAlgorithm, BCRYPT_AES_ALGORITHM, NULL, 0 );
		if	( status < 0 )
			break;

		// Get the size of the subobject.

		DWORD	cbData = 0;
		DWORD	cbObject = 0;
		status = BCryptGetProperty( hAlgorithm, BCRYPT_OBJECT_LENGTH, (BYTE*)&cbObject, sizeof( cbObject ), &cbData, 0 );
		if	( status < 0 )
			break;

		// Get the size of cipher block for the algorithm.

		pbObject = new BYTE[cbObject];
		DWORD	cbBlock = 0;
		status = BCryptGetProperty( hAlgorithm, BCRYPT_BLOCK_LENGTH, (BYTE*)&cbBlock, sizeof( cbBlock ), &cbData, 0 );
		if	( status < 0 )
			break;

		// Allocate a cipher block and fill it with initial vector.

		pbBlock = new BYTE[cbBlock];
		memcpy( pbBlock, m_pbBlob+OFS_IV, cbBlock );

		// Set the chaining mode as 'CBC'.

		status = BCryptSetProperty( hAlgorithm, BCRYPT_CHAINING_MODE, (BYTE*)BCRYPT_CHAIN_MODE_CBC, sizeof( BCRYPT_CHAIN_MODE_CBC ), 0 );
		if	( status < 0 )
			break;

		// Create a key.

		status = BCryptGenerateSymmetricKey( hAlgorithm, &hKey, pbObject, cbObject, m_pbBlob+OFS_SK, cbBlock, 0 );
		if	( status < 0 )
			break;

		DWORD	dwFlags = BCRYPT_BLOCK_PADDING;
		if	( bEncode ){
			// Encode the given plain text.

			DWORD	cbPlain = (DWORD)strlen( lpszPlain );
			pbPlain = new BYTE[cbPlain];
			memcpy( pbPlain, lpszPlain, cbPlain );

			status = BCryptEncrypt( hKey, pbPlain, cbPlain, NULL, pbBlock, cbBlock, NULL, 0, &cbCoded, dwFlags );
			if	( status < 0 )
				break;

			SetLastError( 0 );
			pbCoded = new BYTE[cbCoded];
			status = BCryptEncrypt( hKey, pbPlain, cbPlain, NULL, pbBlock, cbBlock, pbCoded, cbCoded, &cbData, dwFlags );
			if	( status < 0 ){
				delete[] pbCoded;
				pbCoded = NULL;
				cbCoded = 0;
				break;
			}
		}
		else{
			// Decode the given code.

			DWORD	cbPlain = 0;
			status = BCryptDecrypt( hKey, pbCoded, cbCoded, NULL, pbBlock, cbBlock, NULL, 0, &cbPlain, dwFlags );
			if	( status < 0 )
				break;

			SetLastError( 0 );
			pbPlain = new BYTE[cbPlain+1];
			status = BCryptDecrypt( hKey, pbCoded, cbCoded, NULL, pbBlock, cbBlock, pbPlain, cbPlain, &cbPlain, dwFlags );
			if	( status < 0 ){
				lpszPlain = NULL;
				break;
			}

			pbPlain[cbPlain] = '\0';
			lpszPlain = new char[cbPlain+1];
			memcpy( lpszPlain, pbPlain, cbPlain+1 );
		}
	}while	( 0 );

	if	( hAlgorithm )
		BCryptCloseAlgorithmProvider( hAlgorithm, 0 );
	if	( hKey )
		BCryptDestroyKey( hKey );
	if	( pbObject )
		delete[] pbObject;
	if	( pbBlock )
		delete[] pbBlock;
	if	( pbPlain )
		delete[] pbPlain;

	return	GetLastError();
}

ULONG
CMainWnd::OnPower( void* pContext, ULONG uType, void* pSetting )
{
	if	( uType == PBT_APMRESUMESUSPEND ){
		CMainWnd*	pWnd = (CMainWnd*)pContext;
		pWnd->SetTimer( TID_START, pWnd->m_nWakeDealy*1000, NULL );
	}

	return	0;
}

void
CMainWnd::PollNow( void )
{
	SetTimer( TID_POLL, m_nPoll*60*1000, NULL );
	PostMessage( WM_TIMER, TID_POLL, NULL );
}

void
CMainWnd::PollMails( void )
{
	ClosePOP( 0 );
	m_iUser = 0;
	m_nMail = 0;
	m_strSummary.Empty();

	ConnectPOP();
}

bool
CMainWnd::ParseMail( CStringA strMail, LPCTSTR pchFile )
{
	bool	bKeep = true;
	CAttr	attr;
	
	// Get `attributes` of the mail.

	attr = GetAttr( strMail );

	// Set the file name if given.

	if	( pchFile )
		attr.m_strFile = pchFile;

	// Make a log of the mail.

	CString	strLog = MakeLog( strMail, attr );

	// Keep the mail if it's in the whitelist.

	CheckWhiteList( strLog, attr );

	// Add summary if the mail has kept.

	if	( attr.m_dwReason )
		bKeep = false;
	else
		m_strSummary.Insert( 0, MakeSummary( strLog, attr ) );

	// Leave a log of the mail to be discarded.

	if	( m_bLogAll || !bKeep )
		SaveLog( strMail, strLog, attr );

	return	bKeep;
}

CAttr
CMainWnd::GetAttr( CStringA strMail )
{
	CAttr	attr;

	GetAuth(   strMail, attr );
	GetFrom(   strMail, attr );
	GetSender( strMail, attr );
	GetType(   strMail, attr );
	GetEncode( strMail, attr );
	GetTime(   strMail, attr );
	CheckMID(      strMail, attr );
	CheckReceived( strMail, attr );

	return	attr;
}

void
CMainWnd::GetAuth( CStringA strMail, CAttr& attr )
{
	int	x, xIn, xOut;

	do{
		x = strMail.Find( "Authentication-Results: " );
		if	( x >= 0 )
			break;
		x = strMail.Find( "Authentication-results: " );
		if	( x >= 0 )
			break;
		x = strMail.Find( "authentication-results: " );
	}while	( 0 );

	DWORD	dwAuth = 0x0;

	if	( x >= 0 ){
		xIn  = strMail.Find( ": ", x );
		xOut = strMail.Find( "\r\n\r\n", xIn+2 );
		CStringA str = strMail.Mid( xIn+2, xOut-(xIn+2) );
		str.TrimRight();
		str.MakeLower();

		char*	pchTag[] = { "sender-id=", "spf=", "dkim=", "dmarc=", NULL };
		int	xTag = 0;
		for	( int i = 0; pchTag[i]; i++ ){
			xTag = str.Find( pchTag[i], xTag );
			if	( xTag >= 0 ){

				// Avoid "dmarc_spf=", "dmarc_dkim=", "aspf=".

				char	ch = str[xTag-1];
				if	( !isspace( ch ) ){
					i--;
					xTag++;
					continue;
				}

				CStringA strResult = str.Mid( xTag+(int)strlen( pchTag[i] ) );
				x = strResult.FindOneOf( " \t\r\n" );
				if	( x >= 0 )
					strResult = strResult.Left( x );
				if	( strResult == "fail" )
					attr.m_nAuth = m_nAuth;
				else if	( strResult == "hardfail" )
					attr.m_nAuth = m_nAuth;
				else if	( strResult == "pass" )
					;
				else
					attr.m_nAuth++;
				dwAuth |= (1<<i);
			}
			xTag = 0;
		}
	}

	if	( dwAuth != m_dwAuthCache ){
		m_dwAuthCache |= dwAuth;
		SaveSettings();
	}

	FilterError( IDS_RF_AUTH, attr );
}

void
CMainWnd::GetFrom( CStringA strMail, CAttr& attr )
{
	int	x, xIn, xOut;

	// Get nominal sender.

	x = strMail.Find( "From: " );
	if	( x < 0 )
		x = strMail.Find( "from: " );
	if	( x >= 0 ){

		// Extract sender address.

		CStringA strFrom;
		xIn  = x + 6;
		x = xIn;
		while	( true ){
			xOut = strMail.Find( ':', x );
			strFrom = strMail.Mid( xIn, xOut-xIn );
			if	( strFrom.Find( '\r' ) >= 0 )
				break;
			x = xOut+1;
		}

		xIn  = strFrom.Find( '<', 0 );
		xOut = strFrom.Find( '>', xIn+1 );
		if	( xIn >= 0 && xOut >= xIn ){
			strFrom = strFrom.Mid( xIn+1, xOut-(xIn+1) );
		}
		else{
			xOut = strFrom.Find( '\r' );
			strFrom = strFrom.Left( xOut );
		}
		strFrom.Trim();
		attr.m_strFrom = strFrom;

		// Get the top level domain.

		x = strFrom.ReverseFind( '.' );
		CString	strTLD = (CString)strFrom.Mid( x );
		strTLD += "\n";

		// Cache the top level domain.

		if	( m_strTLDCache.Find( strTLD ) < 0 ){
			m_strTLDCache += strTLD;
			SaveTLDs();
		}

		// Check the 'black list' of the domains.

		CheckBlackList( attr.m_strFrom, attr );

		// Check if the sender faking a domain.

		strFrom = attr.m_strFrom;
		x = strFrom.Find( '@' );
		if	( x >= 0 ){
			strFrom = strFrom.Left( x );
			x = strFrom.ReverseFind( '.' );
			if	( x >= 0 ){
				CStringA strFakeDomain = strFrom.Mid( x );
				strFakeDomain.Trim();
				x = 0;
				bool	bDrop2 = false;
				for	( ;; ){
					CString	strTLD = m_strTLDCache.Tokenize( _T("\n"), x );
					if	( strTLD.IsEmpty() )
						break;
					if	( strFakeDomain.Right( strTLD.GetLength() ) == (CStringA)strTLD ){
						bDrop2 = true;
						break;
					}
				}
				if	( bDrop2 )
					FilterError( IDS_RF_FAKE_DOMAIN, attr );
			}
		}
	}
}

void
CMainWnd::GetSender( CStringA strMail, CAttr& attr )
{
	int	x, xIn, xOut;

	// Get actual sender from smtp.mailfrom in Authentication-Results: ( RFC 5451 ) 

	x = strMail.Find( "smtp.mailfrom=" );
	if	( x >= 0 ){
		xIn  = strMail.Find( '=', x );
		xOut = strMail.Find( ';', xIn+1 );
		CStringA strSender = strMail.Mid( xIn+1, xOut-(xIn+1) );
		x = strSender.FindOneOf( " \t\r\n" );
		if	( x >= 0 )
			strSender = strSender.Left( x );
		
		x = attr.m_strFrom.Find( '@' );
		int	nch = attr.m_strFrom.GetLength() - x;
		CStringA strFromTail   = attr.m_strFrom.Mid( x+1 );
		CStringA strSenderTail = strSender.Mid( strSender.GetLength() - nch );
		if	( strSenderTail[0] == '.' )
			strSenderTail.Delete( 0, 1 );
		if	( strSenderTail[0] == '@' )
			strSenderTail.Delete( 0, 1 );
		if	( strSenderTail != strFromTail )
			attr.m_strSender = strSender;
	}

	// Get the first sender from Received:

	if	( x < 0 ){
		CStringA strSender, strReceived;
		x = 0;
		do{
			int	x2 = strMail.Find( "Received: ", x );
			if	( x2 < 0 ){
				x2 = strMail.Find( "received: ", x );
				if	( x2 < 0 )
					break;
			}
			if	( x2 < 0 )
				break;
			x = x2;
			x += 10;
			x2 = strMail.Find( "\r\n", x );
			strReceived = strMail.Mid( x, x2-x );

			if	( !strReceived.IsEmpty() ){
				int	i = strReceived.Find( "from " );
				if	( i >= 0 ){
					strReceived.Delete( 0, i+5 );
					i = strReceived.FindOneOf( " \t\r\n(" );
					strReceived = strReceived.Left( i );
					if	( strReceived.Find( '.' ) > 0 )
						strSender = strReceived;
				}
			}

		}while	( x >= 0 );

		if	( !strSender.IsEmpty() )
			attr.m_strSender = strSender;
	}

	if	( !attr.m_strSender.IsEmpty() ){
	//	TRACE( "sent from '%s' on behalf of '%s'.\n", attr.m_strSender, attr.m_strFrom );

		// Check the 'black list' of the domains.

		CheckBlackList( attr.m_strSender, attr );
	}
}

void
CMainWnd::GetType( CStringA strMail, CAttr& attr )
{
	int	x, xIn, xOut;

	do{
		x = strMail.Find( "Content-Type: " );
		if	( x >= 0 )
			break;
		x = strMail.Find( "Content-type: " );
		if	( x >= 0 )
			break;
		x = strMail.Find( "content-type: " );
	}while	( 0 );

	if	( x >= 0 ){
		xIn  = strMail.Find( ": ", x );
		xOut = strMail.Find( ": ", xIn+2 );
		if	( xOut < 0 )
			xOut = strMail.GetLength();
		CStringA str = strMail.Mid( xIn+2, xOut-(xIn+2) );
		x = str.ReverseFind( '\r' );
		if	( x >= 0 )
			str = str.Left( x );
		str.Trim();

		if	( str.Left( 4 ) == "text" ){
			attr.m_iType = CAttr::Text;
			CStringA strSubType = str.Mid( 5 );
			if	( strSubType.Left( 5 ) == "plain" )
				attr.m_iSubType = CAttr::Plain;
			else if	( strSubType.Left( 4 ) == "html" )
				attr.m_iSubType = CAttr::HTML;
		}
		else if	( str.Left( 9 ) == "multipart" )
			attr.m_iType = CAttr::Multipart;

		x = str.Find( "charset=" );
		if	( x >= 0 ){
			CStringA strCharset = str;
			x += 8;
			strCharset.Delete( 0, x );
			if	( strCharset[0] == '"' ){
				strCharset.Delete( 0, 1 );
				x = strCharset.Find( '"' );
				strCharset = strCharset.Left( x );
			}
			else{
				x = strCharset.Find( "\r\n" );
				if	( x > 0 )
					strCharset = strCharset.Left( x );
			}

			attr.m_iCharset = GetCodePage( strCharset );
			if	( !attr.m_iCharset )
				FilterError( IDS_RF_CHARSET, attr );
		}

		if	( attr.m_iType == CAttr::Multipart ){
			x = str.Find( "boundary=" );
			if	( x > 0 ){
				CStringA strBoundary = str;
				strBoundary.Delete( 0, x+9 );
				if	( strBoundary[0] == '"' ){
					strBoundary.Delete( 0, 1 );
					x = strBoundary.Find( '"', 0 );
				}
				else
					x = strBoundary.Find( "\r\n" );
				if	( x >= 0 )
					strBoundary = strBoundary.Left( x );
				attr.m_strBoundary = strBoundary;
			}
		}
	}
}

void
CMainWnd::GetEncode( CStringA strMail, CAttr& attr )
{
	int	x, xIn, xOut;

	do{
		x = strMail.Find( "Content-Transfer-Encoding: " );
		if	( x >= 0 )
			break;
		x = strMail.Find( "Content-transfer-encoding: " );
		if	( x >= 0 )
			break;
		x = strMail.Find( "content-transfer-encoding: " );
	}while	( 0 );
	if	( x >= 0 ){
		xIn  = strMail.Find( ": ", x );
		xOut = strMail.Find( "\r\n", xIn+2 );
		CStringA str = strMail.Mid( xIn+2, xOut-(xIn+2) );
		if	( !str.CompareNoCase( "7bit" ) )
			attr.m_iEncode = CAttr::Bit7;
		else if	( !str.CompareNoCase( "8bit" ) )
			attr.m_iEncode = CAttr::Bit8;
		else if	( !str.CompareNoCase( "base64" ) )
			attr.m_iEncode = CAttr::Base64;
		else if	( !str.CompareNoCase( "quoted-printable" ) )
			attr.m_iEncode = CAttr::Quoted;
		else if	( !str.CompareNoCase( "binary" ) )
			attr.m_iEncode = CAttr::Binary;
		else{
			attr.m_iEncode = CAttr::unencoded;
			TRACE( "encoding '%s' unknown\n", str );
		}
	}
}

void
CMainWnd::GetTime( CStringA strMail, CAttr& attr )
{
	// Get time according to RFC 5322 and save it in UTC.

	int	x, xIn, xOut;

	x = strMail.Find( "Date: " );
	if	( x < 0 )
		x = strMail.Find( "date: " );
	if	( x >= 0 ){
		xIn  = strMail.Find( ": ", x );
		xOut = strMail.Find( "\r\n", xIn+2 );
		CStringA strDate = strMail.Mid( xIn+2, xOut-(xIn+2) );

		x = strDate.ReverseFind( ' ' );
		CStringA strZone = strDate.Mid( x+1 );
		strDate = strDate.Left( x );
		if	( strZone[0] == '(' ){
			strZone.Delete( 0, 1 );
			strZone.Delete( strZone.GetLength()-1, 1 );
		}
		if	( x >= 5 &&
			  ( strDate[x-5] == '+' ||
			    strDate[x-5] == '-' ) )
			strZone = strDate.Mid( x-5 );

		int	nYear = 0, nMonth = 0, nDay = 0, nHour = 0, nMinute = 0, nSecond = 0;
		bool	bError = false;
		do{
			x = strDate.Find( ", " );
			if	( x < 0 ){				// missing week
				if	( !isdigit( strDate[0] ) ){	// omitted week
					bError = true;	
					break;
				}
			}
			else
				strDate.Delete( 0, x+2 );
			attr.m_strZone = strZone;

			char*	pchDate = strDate.GetBuffer();
			char*	pch = NULL;
			nDay = strtol( pchDate, &pch, 10 );
			pch++;
			char*	apchMonth[] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec", NULL };
			for	( int i = 0; apchMonth[i]; i++ )
				if	( !strncmp( pch, apchMonth[i], 3 ) )
					nMonth = i+1;
			if	( !nMonth ){
				bError = true;	// missing month
				break;
			}
			pch += 4;
			nYear = strtol( pch, &pch, 10 );
			if	( nYear < 2000 ){
				bError = true;	// missing year
				break;
			}
			nHour = strtol( ++pch, &pch, 10 );
			if	( nHour < 0 || nHour > 23 ){
				bError = true;	// missing hour
				break;
			}
			nMinute = strtol( ++pch, &pch, 10 );
			if	( nMinute < 0 || nMinute > 59 ){
				bError = true;	// missing minute
				break;
			}
			nSecond = strtol( ++pch, &pch, 10 );
			if	( nSecond < 0 || nSecond > 59 ){
				bError = true;	// missing second
				break;
			}
		}while	( 0 );

		if	( bError ){
			FilterError( IDS_RE_DATE, attr );
			return;
		}

		CTime	time( nYear, nMonth, nDay, nHour, nMinute, nSecond );
		
		long	lBias = 0;
		bool	bMinus = false;
		if	( strZone[0] == '+' ||
			  strZone[0] == '-' ){
			lBias = atoi( strZone );
		}
		else{
			// Replace "GMT" with "UTC" since they say "GMT" in the mail header.

			if	( strZone == "GMT" )
				  strZone = "UTC";
			CTimeZones	zones;
			lBias = zones.GetBias( (CString)strZone );
			if	( lBias == INT_MAX ){
				FilterError( IDS_RE_ABBR, attr );
				lBias = 0;
			}
			lBias = (lBias/60)*100 + (lBias%60);
			strZone.Format( "%04d", abs( lBias ) );	
			strZone.Insert( 0, ( lBias >= 0 )? "+": "-" );
		}
		if	( lBias <= 0 ){
			lBias = -lBias;
			bMinus = true;
		}
		int	nBias = (lBias/100)*60 + (lBias%100);

		CTimeSpan	tsBias( 0, 0, nBias, 0 );
		if	( bMinus )
			time += tsBias;
		else
			time -= tsBias;
		
		attr.m_time = time;

		x = 0;
		for	( ;; ){
			CString	strTime = m_strTimes.Tokenize( _T("\n"), x );
			if	( strTime.IsEmpty() )
				break;
			if	( strTime == (CString)strZone ){
				FilterError( IDS_RF_TIMEZONE, attr );
				break;
			}
		}
	}
}

// See https://learn.microsoft.com/en-us/windows/win32/intl/code-page-identifiers

#define	CP_SHIFT_JIS	  932
#define	CP_ISO_2022_JP	50220	// JIS X 0201-1989
#define	CP_GB18030	54936	// GB18030
//efine	CP_UTF8		65001	// defined in <WinNls.h>

int
CMainWnd::GetCodePage( CStringA strMail )
{
	int	iCodePage = 0;

	CString	strKey = _T("MIME\\Database\\Charset\\");
	strKey += strMail;
	HKEY	hKey = NULL;
	DWORD	dwResult = RegOpenKeyEx( HKEY_CLASSES_ROOT, strKey, 0, KEY_READ, &hKey );

	if	( dwResult == ERROR_SUCCESS ){
		int	nValue = 0;
		ULONG	cbValue = sizeof( nValue );

		dwResult = RegGetValue( hKey, NULL, _T("InternetEncoding"), RRF_RT_REG_DWORD, NULL, (PVOID)&nValue, &cbValue );
		if	( dwResult == ERROR_SUCCESS )
			iCodePage = nValue;
		else{
			char	achValue[512] = { 0 };
			ULONG	cchValue = sizeof( achValue );
			dwResult = RegGetValueA( hKey, NULL, "AliasForCharset", RRF_RT_REG_SZ, NULL, (PVOID)achValue, &cchValue );
			if	( dwResult == ERROR_SUCCESS )
				iCodePage = GetCodePage( achValue );
		}

		RegCloseKey( hKey );
	}
	if	( !iCodePage ){
		// MS says that they start supporting GB18030 in KB5028171 which fails in installation for Japanese locale.
		if	( !strMail.CompareNoCase( "GB18030" ) )
			iCodePage = CP_GB18030;
		else
			TRACE( "code page '%s' is unknown.\n", strMail );
	}

	return	iCodePage;
}

void
CMainWnd::CheckMID( CStringA strMail, CAttr& attr )
{
	int	x, xIn, xOut;

	x = strMail.Find( "Message-ID: " );
	if	( x >= 0 ){
		xIn  = strMail.Find( "<", x );
		xOut = strMail.Find( ">", xIn+1 );
		CStringA strMessageId = strMail.Mid( xIn+1, xOut-(xIn+1) );
		x = strMessageId.ReverseFind( '@' );
		do{
			if	( x < 0 )
				break;
			CStringA strRight = strMessageId.Mid( x+1 );
			CStringA strLeft  = strMessageId.Left( x );
			x = strRight.Find( '.' );
			if	( x < 0 )
				break;
			for	( int i = 0; i < strRight.GetLength(); i++ )
				if	( strRight[i] >= 'A' && strRight[i] <= 'Z' ){
					x = -1;
					break;
				}
			// We abandoned check of the left side ( in strLeft above )
			// as we observed some senders do not conform to RFC 5322...
		}while	( 0 );
		if	( x < 0 )
			FilterError( IDS_RF_MESSAGEID, attr );
	}
}

void
CMainWnd::CheckReceived( CStringA strMail, CAttr& attr )
{
	// Get the last ( the first ) 'Received: '.

	int	x, xIn, xOut;

	xIn = -1;
	x = 0;
	do{
		x = strMail.Find( "\r\nReceived: ", x );
		if	( x < 0 )
			break;
		x += 2+10;
		xIn = x;
	}while	( true );
	if	( xIn < 0 )
		return;

	x = xIn;
	do{
		x = strMail.Find( "\r\n", x );
		x += 2;
		if	( strMail[x] > ' ' ){
			xOut = x;
			break;
		}
	}while	( true );
	
	CStringA strReceived = strMail.Mid( xIn, xOut-xIn );

	// Get the time zone in the 'Received: '.

	CStringA strZone;
	x = 0;
	while	( strZone.IsEmpty() ){
		x = strReceived.Find( '+', x );
		if	( x < 0 )
			break;
		else if	( isdigit( strReceived[x+1] ) &&
			  isdigit( strReceived[x+2] ) &&
			  isdigit( strReceived[x+3] ) &&
			  isdigit( strReceived[x+4] ) &&
			 !isdigit( strReceived[x+5] )    ){
			strZone = strReceived.Mid( x, 5 );
			int	nHour = atoi( strZone.GetBuffer()+1 );
			if	( nHour < 2400 )
				break;
		}
		x++;
	}
	x = 0;
	while	( strZone.IsEmpty() ){
		x = strReceived.Find( '-', x );
		if	( x < 0 )
			break;
		else if	( isdigit( strReceived[x+1] ) &&
			  isdigit( strReceived[x+2] ) &&
			  isdigit( strReceived[x+3] ) &&
			  isdigit( strReceived[x+4] ) &&
			 !isdigit( strReceived[x+5] )    ){
			strZone = strReceived.Mid( x, 5 );
			int	nHour = atoi( strZone.GetBuffer()+1 );
			if	( nHour < 2400 )
				break;
		}
		x++;
	}

	// Check if the time zone is unreliale.

	if	( !strZone.IsEmpty() ){
		x = 0;
		for	( ;; ){
			CString	strTime = m_strTimes.Tokenize( _T("\n"), x );
			if	( strTime.IsEmpty() )
				break;
			if	( strTime == (CString)strZone ){
				FilterError( IDS_RF_TIMEZONE, attr );
				break;
			}
		}
	}
}

void
CMainWnd::CheckBlackList( CStringA strSender, CAttr& attr )
{
	bool	bDrop = false;
	int	x;

	x = 0;
	for	( ;; ){
		CString	strDrop = m_strDomains.Tokenize( _T("\n"), x );
		if	( strDrop.IsEmpty() )
			break;
		if	( strSender.Right( strDrop.GetLength() ) == (CStringA)strDrop ){
			bDrop = true;
			break;
		}
	}

	if	( bDrop )
		FilterError( IDS_RF_DOMAIN, attr );
}

CString
CMainWnd::MakeLog( CStringA strMail, CAttr& attr )
{
	CString	strLog;
	CStringA strDecoded;

	// Separate the mail header and the mail body.

	int	x = strMail.Find( "\r\n\r\n" );
	if	( x < 0 ){
		FilterError( IDS_RE_MAIL_HEADER, attr );
		return	_T("");
	}

	CStringA strHeader = strMail.Left( x+4 );
	CStringA strBody   = strMail.Mid( x+4 );

	// Make a log from the mail header.

	strLog = StringFromHeader( strHeader, attr );

	// Check 'alias' of 'From:'.

	CheckAlias( strLog, attr );

	// Make a log from the mail body.

	if	( attr.m_iType == CAttr::Text )
		strLog += StringFromBody( strBody, attr );

	// Make logs from the multipart bodies.

	else if	( attr.m_iType == CAttr::Multipart ){
		do{
			// Seek the top of boundary.

			x = strBody.Find( attr.m_strBoundary );
			if	( attr.m_strBoundary.IsEmpty() || x < 0 ){
				FilterError( IDS_RE_MULTIPART, attr );
				break;
			}

			// Add the boundary and the following spaces to the log.

			for	( x += attr.m_strBoundary.GetLength();; x++ )
				if	( strBody[x] == '\n' )
					break;
				else if	( strBody[x] == '\0' )
					break;
			strLog += (CString)strBody.Left( x+1 );
			strBody.Delete( 0, x+1 );

			// Seek the bottom of boundary and get the part body.

			x = strBody.Find( attr.m_strBoundary );
			if	( x < 0 ){
				strLog += (CString)strBody;
				break;
			}
			for	( --x; x > 0; x-- )
				if	( strBody[x] == '\n' ){
					x++;
					break;
				}
			CStringA strPart = strBody.Left( x );
			strBody.Delete( 0, x );
			strBody.TrimLeft();

			// Get 'attribute' of the part body ( w/o Authentication check ).

			CAttr	attrPart = GetAttr( strPart );
			attrPart.m_strFrom = attr.m_strFrom;

			// Add the boundary header of the part to the log.

			x = strPart.Find( "\r\n\r\n" );
			if	( x < 0 ){
				FilterError( IDS_RE_MULTIPART_BODY, attr );
				break;
			}
			strLog += (CString)strPart.Left( x+4 );
			strPart.Delete( 0, x+4 );

			// Count EOL following the part body.

			int	nEOL;
			for	( nEOL = 0; ; nEOL++ )
				if	( strPart.Right( 2 ) == "\r\n" )
					strPart.Delete( strPart.GetLength()-2, 2 );
				else
					break;

			// Add the part body to the log.

			strLog += StringFromBody( strPart, attrPart );

			// Add EOLs to the log.

			for	( int i = 0; i < nEOL; i++ )
				strLog += _T("\r\n");

			// Merge the links and the reasons.

			if	( !attrPart.m_strLinks.IsEmpty() ){
				x = 0;
				do{
					CString	strLink = attrPart.m_strLinks.Tokenize( _T("\n"), x );
					if	( strLink.IsEmpty() )
						break;
					else if	( attr.m_strLinks.Find( strLink ) < 0 )
						attr.m_strLinks += strLink + _T("\n");
				}while	( 1 );
			}
			if	( attrPart.m_dwReason )
				attr.m_dwReason |= attrPart.m_dwReason;
		}while	( 1 );
	}

	// Insert the links and the reasons in the log.

	if	( attr.m_dwReason ){
		if	( !attr.m_strLinks.IsEmpty() ){
			x = 0;
			CString	strLinks;
			for	( ;; ){
				CString	strLink = attr.m_strLinks.Tokenize( _T("\n"), x );
				if	( strLink.IsEmpty() )
					break;
				strLinks += _T("Embedded-Link: ");
				strLinks += strLink + _T("\r\n");
			}
			strLog.Insert( 0, strLinks );
		}
		CString	strReason;
		DWORD	dwReason = attr.m_dwReason;
		for	( UINT uID = IDS_RF_AUTH; uID <= IDS_RF_TIMEZONE; uID++ ){
			UINT	uBit = 1<<(uID-IDS_RF_AUTH);
			if	( dwReason & uBit ){
				CString	strError;
				(void)strError.LoadString( uID );
				strReason += strError;
				dwReason &= ~uBit;
				if	( dwReason )
					strReason += _T(", ");
			}
		}
		strLog.Insert( 0, strReason + _T("\r\n") );
		strLog.Insert( 0, _T("Discard-Reason: ") );
	}

	return	strLog;
}

void
CMainWnd::SaveLog( CStringA strMail, CString strLog, CAttr& attr )
{
	// Check the folder to save logs.

	CFileStatus	fs;
	if	( !CFile::GetStatus( m_strLogPath, fs ) )
		CreateDirectory( m_strLogPath, NULL );

	// Get a local time from time in attribute.

	CTime	time = attr.m_time;
	{
		TIME_ZONE_INFORMATION	tzi;
		GetTimeZoneInformation( &tzi );
		CTimeSpan	spanBias( 0, 0, tzi.Bias, 0 );
		time -= spanBias;
	}

	// Delete old files before re-evaluation.

	if	( !attr.m_strFile.IsEmpty() ){
		DeleteFile( m_strLogPath + attr.m_strFile + _T(".eml") );
		DeleteFile( m_strLogPath + attr.m_strFile + _T(".txt") );
		DeleteFile( m_strLogPath + _T("!") + attr.m_strFile + _T(".eml") );
		DeleteFile( m_strLogPath + _T("!") + attr.m_strFile + _T(".txt") );
	}

	// Save the original image as a '.eml' file.

	CString	strFile;
	bool	bSave;

	bSave = true;
	strFile = attr.m_strFile.IsEmpty()? (CString)attr.m_strFrom: attr.m_strFile;
	if	( attr.m_dwReason )
		strFile.Insert( 0, _T("!") );
	strFile.Insert( 0, m_strLogPath );
	strFile += _T(".eml");

	if	( attr.m_strFile.IsEmpty() )
		if	( IsDuplicated( strFile, time ) )
			bSave = false;

	if	( bSave ){
		if	( attr.m_strFile.IsEmpty() )
			strFile = AddSuffix( strFile );
		{
			CFile	f;
			if	( f.Open( strFile, CFile::modeWrite | CFile::modeCreate | CFile::shareExclusive ) )
				f.Write( strMail.GetBuffer(), strMail.GetLength() );
		}
		if	( CFile::GetStatus( strFile, fs ) ){
			fs.m_ctime = time;
			fs.m_mtime = time;
			CFile::SetStatus( strFile, fs );
			// If you are a Windows 11 user and observed "Date modified" changes, check
			// https://www.google.com/search?q=stop+windows+11+to+change+date+modified
			// until MS fixes this bug introduced by KB5039212.
		}
	}

	// Save decoded text files.

	bSave = true;
	strFile = attr.m_strFile.IsEmpty()? (CString)attr.m_strFrom: attr.m_strFile;
	if	( attr.m_dwReason )
		strFile.Insert( 0, _T("!") );
	strFile.Insert( 0, m_strLogPath );
	strFile += _T(".txt");

	if	( attr.m_strFile.IsEmpty() )
		if	( IsDuplicated( strFile, time ) )
			bSave = false;

	if	( bSave ){
		if	( attr.m_strFile.IsEmpty() )
			strFile = AddSuffix( strFile );
		{
			CFile	f;
			DWORD	dwBOM = 0xbfbbef;
			if	( f.Open( strFile, CFile::modeWrite | CFile::modeCreate | CFile::shareExclusive ) ){
				f.Write( &dwBOM, 3 );
				CStringA strLogFile = StringToUTF8( (CStringW)strLog, attr );
				f.Write( strLogFile.GetBuffer(), strLogFile.GetLength() );
			}
		}
		if	( CFile::GetStatus( strFile, fs ) ){
			fs.m_ctime = time;
			fs.m_mtime = time;
			CFile::SetStatus( strFile, fs );
		}
	}

	// Trim the files to keep within the specified number of files.

	TrimFiles( m_strLogPath, _T("*.eml"), m_nLog );
	TrimFiles( m_strLogPath, _T("*.txt"), m_nLog );
}

bool
CMainWnd::IsDuplicated( CString strFile, CTime time )
{
	int	x;
	x = strFile.ReverseFind( '\\' );
	CString strPath = strFile.Left( x+1 );
	x = strFile.ReverseFind( '.' );
	CString strExt = strFile.Mid( x );
	CString strPrefix = strFile.Left( x );

	CFileFind finder;
	BOOL	  bWorking = finder.FindFile( strPrefix + _T("*") + strExt );
	while	( bWorking ){
		bWorking = finder.FindNextFile();
		CString	strFound = finder.GetFileName();
		CFileStatus	fs;
		CFile::GetStatus( strPath + strFound, fs );
		if	( fs.m_ctime == time )
			return	true;
	}

	return	false;
}

#ifdef	UNICODE
#define	atoi	_wtoi
#endif//UNICODE

CString
CMainWnd::AddSuffix( CString strFile )
{
	int	x;
	x = strFile.ReverseFind( '.' );
	CString strExt = strFile.Mid( x );
	CString strPrefix = strFile.Left( x );

	int	nFound = 0;
	CFileFind finder;
	BOOL	  bWorking = finder.FindFile( strPrefix + _T("*") + strExt );
	while	( bWorking ){
		bWorking = finder.FindNextFile();
		CString	strFound = finder.GetFileName();
		x = strFound.GetLength() - strExt.GetLength();
		if	( x > 0 )
			x--;
		TCHAR	ch = strFound[x];
		if	( ch != ')' )
			nFound = max( nFound, 1 );
		else{
			x = strFound.ReverseFind( '(' );
			if	( x < 0 )
				continue;
			int	iFound = 0;
			iFound = atoi( strFound.Mid( x+1 ) );
			nFound = max( nFound, iFound );
		}
	}

	CString	strSuffix;
	if	( nFound > 0 )
		strSuffix.Format( _T("(%d)"), ++nFound );

	return	strPrefix + strSuffix + strExt;
}

#ifdef	UNICODE
#undef	atoi
#endif

void
CMainWnd::TrimFiles( CString strPath, CString strFile, int nFile )
{
	class	CFileTime
	{
	public:
		CString	m_strFile;
		CTime	m_time;
	};
	CArray	<CFileTime, CFileTime&>
		aFileTime;

	if	( strPath.Right( 1 ) != _T("\\") )
		strPath += _T("\\");

	CFileStatus	fs;
	if	( !CFile::GetStatus( strPath, fs ) )
		return;

	CFileFind finder;
	BOOL	  bWorking = finder.FindFile( strPath + strFile );
	while	( bWorking ){
		bWorking = finder.FindNextFile();
		CFileTime	ft;
		ft.m_strFile = finder.GetFilePath();
		CFile::GetStatus( ft.m_strFile, fs );
		ft.m_time = fs.m_ctime;

		INT_PTR	i, n = aFileTime.GetCount();
		for	( i = 0; i < n; i++ )
			if	( aFileTime[i].m_time >= ft.m_time )
				break;
		aFileTime.InsertAt( i, ft );
	}
	{
		INT_PTR	i, n = aFileTime.GetCount() - m_nLog;
		for	( i = 0; i < n; i++ )
			DeleteFile( aFileTime[i].m_strFile );
	}
}

void
CMainWnd::MoveFiles( CString strPathFrom, CString strPathTo )
{
	if	( strPathFrom.Right( 1 ) != _T("\\") )
		strPathFrom += _T("\\");
	if	( strPathTo.Right( 1 ) != _T("\\") )
		strPathTo += _T("\\");

	CFileStatus	fs;
	if	( !CFile::GetStatus( strPathFrom, fs ) )
		return;
	if	( !CFile::GetStatus( strPathTo, fs ) )
		return;

	CFileFind finder;
	BOOL	  bWorking = finder.FindFile( strPathFrom + _T("*") );
	while	( bWorking ){
		bWorking = finder.FindNextFile();
		if	( finder.IsDots() )
			continue;

		CString	strFrom = finder.GetFilePath();
		CFile::GetStatus( strFrom, fs );

		CString	strTo = strPathTo + finder.GetFileName();
		MoveFile( strFrom, strTo );
		CFile::SetStatus( strTo, fs );
	}

	RemoveDirectory( strPathFrom );
}

CString
CMainWnd::MakeSummary( CString strLog, CAttr attr )
{
	CString	strSummary;

	int	x, xIn, xOut;

	do{
		x = strLog.Find( _T("Subject: ") );
		if	( x >= 0 )
			break;
		x = strLog.Find( _T("subject: ") );
	}while	( 0 );

	if	( x >= 0 ){
		xIn  = strLog.Find( _T(": "), x );
		xOut = strLog.Find( _T("\r\n"), xIn+2 );
		CString str = strLog.Mid( xIn+2, xOut-(xIn+2) );
		str.TrimRight();
		strSummary += str + _T("\n");
	}

	do{
		x = strLog.Find( _T("From: ") );
		if	( x >= 0 )
			break;
		x = strLog.Find( _T("from: ") );
	}while	( 0 );

	if	( x >= 0 ){
		xIn  = strLog.Find( _T(": "), x );
		xOut = strLog.Find( _T("\r\n"), xIn+2 );
		CString strFrom = strLog.Mid( xIn+2, xOut-(xIn+2) );
		strFrom.TrimRight();
		if	( strFrom.Find( '<' ) < 0 )
			strFrom = attr.m_strFrom;
		strSummary += strFrom + _T("\n");
	}

	SYSTEMTIME	stime = {};
	CTime	time = attr.m_time;
	{
		TIME_ZONE_INFORMATION	tzi;
		GetTimeZoneInformation( &tzi );
		CTimeSpan	spanBias( 0, 0, tzi.Bias, 0 );
		time -= spanBias;
	}
	time.GetAsSystemTime( stime );
	TCHAR	achDate[256] = {};
	GetDateFormatEx( LOCALE_NAME_USER_DEFAULT, DATE_SHORTDATE, &stime, NULL, achDate, _countof( achDate ), NULL );
	CString	str = achDate;
	str += time.Format( _T(" %H:%M") );
	str += _T("\n\n");
	strSummary += str;

	return	strSummary;
}

void
CMainWnd::ShowSummary( CString strSummary )
{
	CString	strNotify;

	for	( ;; ){
		int	x = strSummary.Find( _T("\n\n") );
		if	( x < 0 )
			break;

		CString	strItem = strSummary.Left( x );
		strSummary.Delete( 0, x+2 );
		x = strItem.Find( '\n' );
		CString	strSubject = strItem.Left( x );
		strItem.Delete( 0, x+1 );
		x = strItem.Find( '\n' );
		CString	strFrom = strItem.Left( x );
		strItem.Delete( 0, x+1 );
		CString	strTime = strItem;
		strNotify += strTime + _T("\t");
		strNotify += strFrom + _T("\n");
		strNotify += strSubject + _T("\n\n");
	}

	m_pWndNotify->SetText( strNotify );
}

void
CMainWnd::ShareSummary( CString strSummary )
{
	CloseHandle( m_hSummary );
	BOOL	bShare = AfxGetApp()->GetProfileInt( _T("Settings"), _T("ShareSummary"), 0 );
	if	( bShare ){
		int	nch = strSummary.GetLength()+1;
		int	cb = nch * sizeof( TCHAR );
		m_hSummary = CreateFileMapping( INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, cb, _T("ChkMails Summary") );

		if	( m_hSummary ){
			void*	pView = MapViewOfFileEx( m_hSummary, FILE_MAP_WRITE, 0, 0, 0, NULL );
			if	( pView ){
				memcpy( (BYTE*)pView, strSummary.GetBuffer(), cb );
				UnmapViewOfFile( pView );
			}
		}
	}
}

CString
CMainWnd::StringFromHeader( CStringA strIn, CAttr& attr )
{
	CString	strOut;
	int	iCharset = attr.m_iCharset;

	for	( ;; ){
		bool	bQuoted = false;

		if	( strIn.IsEmpty() )
			break;

		int	x = strIn.Find( "=?" );
		if	( x < 0 ){
			strOut += strIn;
			break;
		}

		int	i = strIn.Find( "?", x+2 );
		CStringA strCharset = strIn.Mid( x+2, i-(x+2) );
		attr.m_iCharset = GetCodePage( strCharset );
		if	( !attr.m_iCharset ){
			strOut += strIn;
			FilterError( IDS_RF_CHARSET, attr );
			break;
		}

		i++;
		if	( strIn[i] == 'Q' ||
			  strIn[i] == 'q'    )
			bQuoted = true;
		i = strIn.Find( "?", i+1 );

		CStringA strLeft = strIn.Left( x );
		if	( strLeft.Left( 3 ) == "\r\n " )
			strLeft.Delete( 0, 3 );
		else if	( strLeft.Left( 3 ) == "\r\n\t" )
			strLeft.Delete( 0, 3 );
		if	( strLeft.Right( 3 ) == "\r\n " )
			strLeft.Delete( strLeft.GetLength()-3, 3 );
		else if	( strLeft.Right( 3 ) == "\r\n\t" )
			strLeft.Delete( strLeft.GetLength()-3, 3 );
		strOut += strLeft;

		strIn.Delete( 0, i+1 );
	//	x = strIn.Find( "?=" );
	//	CStringA::Find may fail just after encoded binary.
		{
			x = 0;
			BYTE*	pb = (BYTE*)strIn.GetBuffer();
			for	( x = 0; pb[x]; x++ )
				if	( !memcmp( pb+x, "?=", 2 ) )
					break;
			if	( !pb[x] )
				x = -1;
		}

		CStringA strEncoded;
		CStringA strDecoded;
		if	( x >= 0 ){
			strEncoded = strIn.Left( x );
			strIn = strIn.Mid( x+2 );
		}
		else
			strIn.Empty();

		if	( attr.m_iEncode == CAttr::Binary )
			strDecoded = (CString)strEncoded;
		else if	( bQuoted )
			strDecoded = (CString)DecodeQuoted( strEncoded );
		else
			strDecoded = (CString)DecodeBase64( strEncoded );

		strOut += StringFromCodePage( strDecoded, attr );
	}

	attr.m_iCharset = iCharset;

	return	strOut;
}

CString
CMainWnd::StringFromBody( CStringA strIn, CAttr& attr )
{
	CString	strOut;
	CStringA strDecoded;

	if	( attr.m_iType != CAttr::Text )
		return	strOut;

	if	( attr.m_iEncode == CAttr::unencoded )
		strDecoded = (CString)strIn;
	else if	( attr.m_iEncode == CAttr::Binary )
		strDecoded = (CString)strIn;
	else if	( attr.m_iEncode == CAttr::Bit7 )
		strDecoded = (CString)strIn;
	else if	( attr.m_iEncode == CAttr::Bit8 )
		strDecoded = (CString)strIn;
	else if	( attr.m_iEncode == CAttr::Base64 )
		strDecoded = DecodeBase64( strIn );
	else if	( attr.m_iEncode == CAttr::Quoted )
		strDecoded = DecodeQuoted( strIn );
	else
		;

	strOut = StringFromCodePage( strDecoded, attr );

	LFtoCRLF( strOut );
	HexToUnicode( strOut );
	CheckUnicode( strOut, attr );
	CheckLink( strOut, attr );

	return	strOut;
}

CStringA
CMainWnd::DecodeBase64( CStringA strEncoded )
{
	CStringA strBase64 =	"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
				"abcdefghijklmnopqrstuvwxyz"
				"0123456789+/";

	// Fill each 8 bit with 6 bit data as follow
	// |00000011|11112222|22333333|

	int	nch = strEncoded.GetLength();
	BYTE*	ba = new BYTE[nch+1];
	int	nb = 0;
	int	ib = 0;
	for	( int ich = 0; ich < nch; ich++ ){
		char	ch = strEncoded.GetAt( ich );
		int	x = strBase64.Find( ch );
		if	( x < 0 )
			continue;

		BYTE	b = (BYTE)x;
		switch	( ib%4 ){
			case	0:
				ba[nb]    = b << 2;
				break;
			case	1:
				ba[nb++] |= ( b & 0x30 ) >> 4;
				ba[nb]    = ( b & 0x0f ) << 4;
				break;
			case	2:
				ba[nb++] |= ( b & 0x3c ) >> 2;
				ba[nb]    = ( b & 0x03 ) << 6;
				break;
			case	3:
				ba[nb++] |= ( b & 0x3f );
				break;
		}
		ib++;
	}
	ba[nb] = '\0';

	CStringA strDecoded = (char*)ba;
	delete	[]	ba;

	return	strDecoded;
}

CStringA
CMainWnd::DecodeQuoted( CStringA strEncoded )
{
	CStringA strDecoded;

	CByteArray	ba;
	char*	pch = strEncoded.GetBuffer();
	while	( *pch ){
		BYTE	b;
		if	( *pch == '=' ){
			pch++;
			if	( pch[0] == '\r' &&
				  pch[1] == '\n' ){
				pch += 2;
				continue;
			}
			b = 0;
			for	( int i = 0; i < 2; i++ ){
				if	( *pch >= '0' && *pch <= '9' )
					b |= ( *pch - '0' );
				else if	( *pch >= 'A' && *pch <= 'F' )
					b |= ( *pch - 'A' ) + 0x0a;
				else{
					b = *pch++;
					break;
				}
				if	( i == 0 )
					b <<= 4;
				pch++;
			}
		}
		else
			b = *pch++;
		ba.Add( b );
	}
	ba.Add( '\0' );

	strDecoded = (char*)ba.GetData();

	return	strDecoded;
}

CString
CMainWnd::StringFromCodePage( CStringA strIn, CAttr& attr )
{
	// ISO 2022-JP with SI/SO: Take escape sequences.

	if	( attr.m_iCharset == CP_ISO_2022_JP )
		strIn = EscapeFromJIS( strIn, attr );

	// Convert the multibyte string to a wide-character string according to the specified code page.

	char*	pchIn = strIn.GetBuffer();
	int	cchOut =
	::MultiByteToWideChar( attr.m_iCharset, MB_ERR_INVALID_CHARS, pchIn, -1, NULL, 0 );
	SetLastError( 0 );
	WCHAR*	pchOut = new WCHAR[cchOut];
	::MultiByteToWideChar( attr.m_iCharset, MB_ERR_INVALID_CHARS, pchIn, -1, pchOut, cchOut );

	DWORD	dwError = GetLastError();
	if	( dwError == ERROR_NO_UNICODE_TRANSLATION )
		FilterError( IDS_RF_CHARSET, attr );

	CString	strOut = pchOut;
	delete []pchOut;

	return	strOut;
}

CStringA
CMainWnd::EscapeFromJIS( CStringA strIn, CAttr& attr )
{
	CStringA strOut;

	struct{
		char*	pchSeq;
		bool	bMultiByte;
		bool	bKana;
	}		stTable[] = {
		{ "\x1b$@",		true,	false	},	// JIS C 6226-1978
		{ "\x1b$B",		true,	false	},	// JIS X 0208-1983 / JIS X 0208:1990
		{ "\x1b(B",		false,	false	},	// ASCII
		{ "\x1b(J",		false,	false	},	// JIS X 0201-1976 Latin
		{ "\x1b$(D",		true,	false	},	// JIS X 0212-1990
		{ "\x1b(I",		false,	true	},	// JIS X 0201-1976 Kana
		{ NULL,			false,	false	}
	};

	bool	bMutiByte   = false;
	bool	bKana       = false;
	int	n = strIn.GetLength()-1;

	// 1st: Conver the JIS sequence into a SJIS string.

	for	( int i = 0; i < n; ){

		// Escape sequence: Enter into the specified mode.

		if	( strIn[i] == '\x1b' ){
			int	x;
			for	( x = 0; stTable[x].pchSeq; x++ ){
				int	nSeq = (int)strlen( stTable[x].pchSeq );
				if	( strIn.Mid( i, nSeq ) == stTable[x].pchSeq ){
					bMutiByte = stTable[x].bMultiByte;
					bKana     = stTable[x].bKana;
					i += nSeq;
					break;
				}
			}
			if	( !stTable[x].pchSeq ){
				FilterError( IDS_RE_ESCSEQUENCE, attr );
				break;
			}
		}

		// Kanji: Convert the multi-byte into a SJIS code.

		else if	( bMutiByte ){
			WORD	w = MAKEWORD( strIn[i+1], strIn[i] );
			w =_mbcjistojms( w );
			strOut += HIBYTE( w );
			strOut += LOBYTE( w );
			i += 2;
		}

		// Kana: Replace it with a SJIS code.

		else if	( bKana ){
			CStringA	strConvert =
				"000102030405060708090A0B0C0D0E0F"
				"101112131415161718191A1B1C1D1E1F"
				"20。「」、・ヲァィゥェォャュョッ"
				"ーアイウエオカキクケコサシスセソ"
				"タチツテトナニヌネノハヒフヘホマ"
				"ミムメモヤユヨラリルレロワン゛゜"
				"606162636465666768696A6B6C6D6E6F"
				"707172737475767778797A7B7C7D7E7F";
			BYTE	b = strIn[i++];
			b &= 0x7f;
			strOut += strConvert.Mid( b*2, 2 );
		}

		// ASCII code: Copy it.

		else
			strOut += strIn[i++];
	}

	// 2nd: Set to convert from SJIS to UNICODE.

	strIn = strOut;
	attr.m_iCharset = CP_SHIFT_JIS;

	return	strOut;
}

CStringA
CMainWnd::StringToUTF8( CStringW strIn, CAttr& attr )
{
	CStringA	strOut;

#ifndef	UNICODE
	strIn = StringFromSJIS( (CStringA)strIn, attr );
#endif
	// Convert UTF-16 to UTF-8.

	WCHAR*	pchIn = strIn.GetBuffer();
	int	cchOut =
	::WideCharToMultiByte( CP_UTF8, 0, pchIn, -1, NULL,        0, NULL, NULL );
	CHAR*	pchOut = new CHAR[cchOut];
	::WideCharToMultiByte( CP_UTF8, 0, pchIn, -1, pchOut, cchOut, NULL, NULL );
	strOut = pchOut;
	delete []pchOut;

	return	strOut;
}

void
CMainWnd::LFtoCRLF( CString& strLines )
{
	int	n = strLines.GetLength();
	for	( int i = 0; i < n; i++ ){
		TCHAR	ch = strLines[i];
		if	( ch == '\0' )
			break;
		else if	( ch == '\n' ){
			if	( i == 0 )
				;
			else if	( strLines[i-1] != '\r' )
				;
			else
				continue;
			strLines.Insert( i, _T("\r") );
			i++;
			n++;
		}
	}
}

void
CMainWnd::HexToUnicode( CString& strLines )
{
#ifdef	UNICODE
	int	n;
	for	( int x = 0;; ){
		n = strLines.GetLength();
		if	( x > n-6 )
			break;
		x = strLines.Find( _T("&#"), x );
		if	( x < 0 )
			break;

		TCHAR*	pch = strLines.GetBuffer();
		TCHAR*	pchDelim = NULL;
		int	xSkip = 0;
		int	nBase = 0;
		pch += x+2;

		if	( tolower( strLines[x+2] ) == 'x' ){
			xSkip = 1;
			nBase = 16;
		}
		else{
			xSkip = 0;
			nBase = 10;
		}

		WCHAR	wch = (WORD)wcstol( pch+xSkip, &pchDelim, nBase );
		if	( *pchDelim != ';' ){
			x += 2;
			continue;
		}

		xSkip = 2 + ( int )( pchDelim-pch );
		strLines.Delete( x, xSkip );
		strLines.SetAt( x, wch );
	}
#endif//UNICODE
}

void
CMainWnd::CheckUnicode( CString& strLog, CAttr &attr )
{
	// Check bidirectional characters.

	WORD	awBidirectional[] = {
			0x200e,	// U+200E: Left-to-Right Mark
			0x200f,	// U+200F: Right-to-Left Mark
			0x202a,	// U+202A: Left-to-Right Embedding
			0x202b,	// U+202B: Right-to-Left Embedding
			0x202c,	// U+202C: Pop Directional Formatting
			0x202d,	// U+202D: Left-to-Right Override
			0x202e,	// U+202E: Right-to-Left Override
			0x2066,	// U+2066: Left-to-Right Isolate
			0x2067,	// U+2067: Right-to-Left Isolate
			0x2068,	// U+2068: First Strong Isolate
			0x2069	// U+2069: Pop Directional Isolate
	};

	int	nch = strLog.GetLength();
	DWORD	dwUnicode = 0;
	for	( int i = 0; i < nch; i++ ){

		// Do not accept bidirectional codes.

		bool	bBidirectional = false;
		for	( int x = 0; x < _countof( awBidirectional ); x++ ){
			if	( strLog[i] == awBidirectional[x] ){
				strLog.Delete( i, 1 );
				nch -= 1;
				CString	strHex;
				strHex.Format( _T("&#x%04x;"), awBidirectional[x] );
				strLog.Insert( i, strHex );
				nch += 8;
				bBidirectional = true;
			}
		}
		if	( bBidirectional )
			FilterError( IDS_RF_CONTROLCODE, attr );

		// Do not accept illegal sequence.

		else if	( strLog[i] == 0xfffd )
			FilterError( IDS_RF_CHARSET, attr );
	}
}

void
CMainWnd::CheckAlias( CString strLog, CAttr& attr )
{
	int	x, xIn, xOut;

	// Get nominal sender.

	x = strLog.Find( _T("From: ") );
	if	( x < 0 )
		x = strLog.Find( _T("from: ") );
	do{
		if	( x < 0 )
			break;

		// Extract sender's alias.

		xIn  = x + 6;
		xOut = strLog.Find( ':', xIn );
		CString strAlias = strLog.Mid( xIn, xOut-xIn );

		xOut = strAlias.ReverseFind( '<' );
		if	( xOut < 0 )
			break;
		strAlias = strAlias.Left( xOut );
		NormalizeAlias( strAlias );

		// Extract sender's domain.

		CString	strDomain = (CString)attr.m_strFrom;
		x = strLog.Find( _T("Sender: ") );
		if	( x >= 0 ){
			strDomain = strLog.Mid( x );
			x = strDomain.Find( '\r' );
			strDomain = strDomain.Left( x );
		}
		x = strDomain.Find( '@' );
		if	( x < 0 )
			break;

		strDomain = strDomain.Mid( x+1 );

		// Check the combination of alias and domain.

		bool	bHit = false;
		bool	bMet = false;

		int	iName = 0;
		for	( ;; ){

			// Check if the the registered word in alias.

			CString	strName = m_strNames.Tokenize( _T("\n"), iName );
			if	( iName < 0 )
				break;
			x = strName.Find( '\t' );
			CString	strWord = strName.Left( x );
			NormalizeAlias( strWord );
			int	iWord = strAlias.Find( strWord );
			if	( iWord < 0 ){
				if	( strWord.GetLength() > 3 ){
					CString	strAliasL = strAlias;
					strAliasL.MakeLower();
					strWord.MakeLower();
					iWord = strAliasL.Find( strWord );
					if	( iWord < 0 )
						continue;
				}
				else
					continue;
			}
			bHit = true;

			// Check if the the registered word in domain.

			strWord = strName.Mid( x+1 );
			if	( strWord.FindOneOf( _T("*?") ) >= 0 ){
				if	( !CompareWild( strWord, strDomain ) ){
					bMet = true;
					break;
				}
			}
			else if	( strWord.Find( '.' ) >= 0 ){
				int	cch = strWord.GetLength();
				if	( strDomain.Right( cch ) == strWord ){
					bMet = true;
					break;
				}
			}
			else{
				if	( strDomain.Find( strWord ) >= 0 ){
					bMet = true;
					break;
				}
			}
		}

		if	( bHit && !bMet )
			FilterError( IDS_RF_FAKE_ALIAS, attr );			
	}while	( 0 );

	// Get the sendee.

	x = strLog.Find( _T("To: ") );
	if	( x < 0 )
		x = strLog.Find( _T("to: ") );
	do{
		if	( x < 0 )
			break;

		// Extract sendee's alias.

		xIn  = x + 4;
		xOut = strLog.Find( ':', xIn );
		CString strAlias = strLog.Mid( xIn, xOut-xIn );

		xOut = strAlias.Find( '<', 0 );
		if	( xOut < 0 )
			break;

		CString	strTo = strAlias.Mid( xOut+1 );
		strAlias = strAlias.Left( xOut );
		strAlias.Trim();

		if	( strAlias[0] == '"' ){
			strAlias.Delete( 0, 1 );
			strAlias.Delete( strAlias.GetLength()-1, 1 );
		}

		if	( strAlias.IsEmpty() )
			break;

		xOut = strTo.Find( '>' );
		strTo = strTo.Left( xOut );

		x = strTo.Find( '@' );
		CString	strUser = strTo.Left( x );

		// Check if the alias is included in the address.

		if	( strAlias == strUser )
			;
		else if	( strAlias == strTo )
			;
		else
			break;

		FilterError( IDS_RF_CALL_BY_ADDR, attr );

	}while	( 0 );
}

int
CMainWnd::CompareWild( CString strWild, CString strName )
{
	for	( ;; ){
		int	x;

		x = strWild.FindOneOf( _T("*?") );

		// No wildcards left, do normal comparison.

		if	( x < 0 )
			return	strWild.Compare( strName );

		// Take a part to compare.

		CString	strWildPart = strWild.Left( x );
		CString	strNamePart = strName.Left( x );
		int	iPart = strWildPart.Compare( strNamePart );

		// Parts differ, return result.

		if	( iPart )
			return	-1;

		// Parts match, delete the parts.

		TCHAR	ch = strWild[x];
		if	( ch == '?' ){
			strWild.Delete( 0, x+1 );
			strName.Delete( 0, x+1 );
		}
		else if	( ch == '*' ){
			strWild.Delete( 0, x+1 );
			strName.Delete( 0, x+0 );
		}

		// No more string, return 'matched'.

		if	( strWild.IsEmpty() )
			return	0;

		// Seek the next word to compare.

		if	( ch == '*' ){
			CString	strWildNext = strWild;
			int	xNext = strWildNext.FindOneOf( _T("*?") );
			if	( xNext >= 0 )
				strWildNext = strWildNext.Left( xNext );
			xNext = strName.Find( strWildNext );
			if	( xNext >= 0 )
				strName.Delete( 0, xNext );
			else
				strName.Delete( 0, x );
		}
	}
}

void
CMainWnd::NormalizeAlias( CString& strAlias )
{
	// Remove symbols.

	strAlias.Replace( _T("\""), _T(" ") );
	strAlias.Replace( _T("'"),  _T(" ") );
	strAlias.Replace( _T("・"),  _T("") );
	strAlias.Replace( _T("_"),  _T(" ") );
	strAlias.Trim();

	// Remove padding spaces.
#if	0
	bool	bSpaced = true;
	int	nch = strAlias.GetLength();
	for	( int x = 1; x < nch; x += 2 ){
		if	( strAlias[x] != ' ' ){
			bSpaced = false;
			break;
		}
	}
	if	( bSpaced ){
		strAlias.Replace( _T(" "), _T("") );
		n = strAlias.GetLength();
	}
#else
	strAlias.Replace( _T(" "),  _T("") );
	strAlias.Trim();
#endif

	// Large codes to ASCII code.

	int	n = strAlias.GetLength();
	for	( int x = 0; x < n; x++ ){
		TCHAR	ch = strAlias[x];

		if	( ch >= 0x0300 && ch <= 0x036f ){	// Combining Diacritical Marks
			strAlias.Delete( x, 1 );
			n--;
		}
		else if	( ch >= 0x2460 && ch <= 0x2468 )	// Circled numbers 1 to 9
			strAlias.SetAt( x, '1'+(ch-0x2460) );
		else if	( ch >= 0x24b6 && ch <= 0x24e9 )	// Circled letters A to Z
			strAlias.SetAt( x, 'A'+(ch-0x24b6) );
		else if	( ch == 0x24ea )			// Circled numbers 0
			strAlias.SetAt( x, '0' );
		else if	( ch >= 0x24f5 && ch <= 0x24fd )	// DoubleCircled numbers 1 to 9
			strAlias.SetAt( x, '1'+(ch-0x24f5) );
		else if	( ch >= 0x2776 && ch <= 0x277e )	// Black circled numbers 1 to 9
			strAlias.SetAt( x, '1'+(ch-0x277e) );
		else if	( ch >= 0x2780 && ch <= 0x2788 )	// White circled numbers 1 to 9
			strAlias.SetAt( x, '1'+(ch-0x2780) );
		else if	( ch >= 0x278a && ch <= 0x2792 )	// Black circled numbers 1 to 9
			strAlias.SetAt( x, '1'+(ch-0x278a) );
		else if	( ch >= 0xff01 &&
			  ch <= 0xff5e )
			strAlias.SetAt( x, '!'+(ch-0xff01) );
		else if	( ch >= 0xd800 && ch <= 0xdbff ){	// Letters in a surrogate pair
			UINT	uch = ( ch & 0x3ff ) << 10;
			ch = strAlias[x+1];
			if	( ch >= 0xdc00 && ch <= 0xdfff ){
				uch |= ( ch & 0x3ff );
				uch += 0x10000;
				bool	bHit = true;
				if	( uch >= 0x1d400 && uch <= 0x1d6a3 )	// Decorated letters A to Z
					strAlias.SetAt( x, 'A'+((uch-0x1d400)%26) );
				else if	( uch >= 0x1d7ce && uch <= 0x1d7ff )	// Decorated numbers 0 to 9
					strAlias.SetAt( x, '0'+((uch-0x1d7ce)%10) );
				else
					bHit = false;
				if	( bHit ){
					strAlias.Delete( x+1, 1 );
					n--;
				}
			}

		}
	}
}

void
CMainWnd::CheckWhiteList( CString strLog, CAttr& attr )
{
	if	( !attr.m_dwReason )
		return;

	for	( int xToken = 0; xToken >= 0; ){
		CString	strItem = m_strWhites.Tokenize( _T("\n"), xToken );
		if	( strItem.IsEmpty() )
			break;

		int	x = strItem.Find( '\t' );
		CString	strFrom   = strItem.Left( x );
		CString	strDomain = strItem.Mid( x+1 );

		if	( strFrom == (CString)attr.m_strFrom ){
			CString	strSender = (CString)attr.m_strSender;
			if	( strSender.IsEmpty() )
				strSender = (CString)attr.m_strFrom;
			strSender = strSender.Right( strDomain.GetLength() );
			if	( strSender == strDomain ){
				attr.m_dwReason = 0;
				break;
			}
		}
	}
}

void
CMainWnd::CheckLink( CString& strLog, CAttr& attr )
{
#ifdef	UNICODE
	int	xLines;
	TCHAR*	apchScheme[2] = { _T("http://"), _T("https://") };

	for	( int iScheme = 0; iScheme < 2; iScheme++ ){
		TCHAR*	pchScheme = apchScheme[iScheme];
		xLines = 0;
		for	( ;; ){
			// Seek the given scheme tag.

			xLines = strLog.Find( pchScheme, xLines );
			if	( xLines < 0 )
				break;

			CString	strLink;
			CString	strDisplay;
			int	xLink = -1;

			if	( attr.m_iSubType == CAttr::HTML ){
				if	( !GetLinkInHTML( strLog, xLines, strLink, strDisplay ) )
					continue;
			}
			else if	( attr.m_iSubType == CAttr::Plain ){
				GetLinkInText( strLog, xLines, strLink );
			}
			SetLinkVisible( strLink, attr );

			// Seek the first '/' just after the domain.

			xLink = strLink.Find( '/' );

			// Take a path following the URL.

			CString	strURL, strPath;
			if	( xLink >= 0 ){
				strPath = strLink.Mid(  xLink+1 );
				strURL  = strLink.Left( xLink );
			}

			// Check if the path is faking a domain.

			do{
				if	( strPath.IsEmpty() )
					break;
				int	x = strPath.FindOneOf( _T("?#<") );
				if	( x < 0 )
					strPath = strPath.Left( x );
				x = strPath.ReverseFind( '.' );
				if	( x < 0 )
					break;
				CString	strTLD = strPath.Mid( x );
				strTLD.MakeLower();
				if	( m_strTLDCache.Find( strTLD ) < 0 )
					break;
				strTLD += _T("\n");
				if	( m_strTLDCache.Find( strTLD ) >= 0 )
					FilterError( IDS_RF_LINK_FAKED, attr );
			}while	( 0 );

			// Cut off arguments.

			xLink = strLink.ReverseFind( '?' );
			if	( xLink >= 0 )
				strLink = strLink.Left( xLink );
			xLink = strLink.ReverseFind( '#' );
			if	( xLink >= 0 )
				strLink = strLink.Left( xLink );
			xLink = strLink.ReverseFind( '<' );
			if	( xLink >= 0 )
				strLink = strLink.Left( xLink );

			// Add the link to show later.

			if	( attr.m_strLinks.Find( strLink ) < 0 )
				attr.m_strLinks += strLink + _T("\n");

			// Check if the link is described in evasive codes.

			bool	bEvasive = false;
			int	nch = strLink.GetLength();
			for	( int i = 0; i < nch; i++ )
				if	( strLink[i] >= 0x80 )
					if	( IsEvasiveCode( strLink, i ) ){
						FilterError( IDS_RF_LINK_EVASIVE, attr );
						bEvasive = true;
						break;
					}

			// Take the top level domain.

			xLink = strURL.ReverseFind( '.' );
			if	( xLink < 0 )
				continue;

			CString	strTLD = strURL.Mid( xLink );
			int	x = strTLD.FindOneOf( _T("?#=") );
			if	( x >= 0 )
				strTLD = strTLD.Left( x );

			// Evasive domain? It's not worth remembering.
		
			nch = strTLD.GetLength();
			for	( x = 0; x < nch; x++ )
				if	( (WORD)strTLD[x] > 0x7f )
					break;
			if	( x < nch )
				continue;

			// Check if the domain is on the blacklist.

			for	( x = 0; x >= 0; ){
				CString	strDrop = m_strDomains.Tokenize( _T("\n"), x );
				if	( strDrop.IsEmpty() )
					break;
				if	( strDrop == strTLD ){
					FilterError( IDS_RF_LINK_DOMAIN, attr );
					if	( attr.m_strLinks.Find( strLink ) < 0 )
						attr.m_strLinks += strLink + _T("\n");
					break;
				}
			}
		}
	}
#endif//UNICODE
}

bool
CMainWnd::GetLinkInHTML( CString strLog, int& xLines, CString& strLink, CString& strDisplay )
{
	int	x = strLog.Find( ':', xLines );
	CString	strScheme = strLog.Mid( xLines, x+3-xLines );
	int	nchScheme = strScheme.GetLength();

	if	( strLog[xLines+nchScheme+1] == '"' ||
		  strLog[xLines+nchScheme+1] == ' ' ){
		xLines += nchScheme+1;
		return	false;
	}

	// Seek '<a' before the scheme.

	for	( x = xLines; x >= 0; x-- )
		if	( strLog[x] == '<' )
			break;
	CString	strTag = strLog.Mid( x, xLines-x );
	strTag.MakeLower();
	if	( strTag.Find( _T("href") ) < 0 ){
		xLines += nchScheme;
		return	false;
	}

	x = strTag.Find( _T(" ") );
	if	( x >= 0 )
		strTag = strTag.Left( x );
	xLines += nchScheme;
	if	( strTag != _T("<a") )
		return	false;

	// Take a URL.

	strLink = strLog.Mid( xLines );
	int	xLink = strLink.Find( _T(">") );
	if	( xLink < 0 )
		return	false;

	xLines += xLink +1;

	// Get the string between <a> and </a>.

	CString	strLeft = strLog.Mid( xLines );
	strLeft.MakeLower();
	x = strLeft.Find( _T("</a") );
	if	( x >= 0 ){
		strDisplay = strLog.Mid( xLines, x );
		xLines += x;
	}
	strLink = strLink.Left( xLink );

	// Get the URL quoted.

	xLink = strLink.Find( '"' );
	if	( xLink >= 0 )
		strLink = strLink.Left( xLink );

	return	true;
}

void
CMainWnd::GetLinkInText( CString strLog, int& xLines, CString& strLink )
{
	int	x = strLog.Find( ':', xLines );
	CString	strScheme = strLog.Mid( xLines, x+3-xLines );
	xLines += strScheme.GetLength();

	// Seek a delimiter character.

	for	( x = xLines; ; x++ ){
		TCHAR	ch = strLog[x];
		if	( ch == '%' )
			continue;
		else if	( ch == '#' )
			continue;
		else if	( ch == '<' )
			break;
		else if	( ch <  '+' )
			break;
		else if	( ch >= '{' && ch <= 0x7f )
			break;

		// Leave large alphanumeric characters.

		else if	( ch >= 0x80 ){
			if	( ch == 0x2215 )			// Division operator
				continue;
			else if	( IsEvasiveCode( strLog, x ) )
				continue;
			else
				break;
		}
	}

	strLink = strLog.Mid( xLines, x-xLines );
	xLines = x;
}

bool
CMainWnd::IsEvasiveCode( CString strLog, int& xLines )
{
	TCHAR	ch = strLog[xLines];

	if	( ch >= 0x2460 && ch <= 0x2468 )	// Circled numbers 1 to 9
		return	true;
	else if	( ch >= 0x24b6 && ch <= 0x24ea )	// Circled letters A to z + 0
		return	true;
	else if	( ch >= 0x2776 && ch <= 0x277e )	// Black circled numbers 1 to 9
		return	true;
	else if	( ch >= 0x2780 && ch <= 0x2788 )	// White circled numbers 1 to 9
		return	true;
	else if	( ch >= 0x278a && ch <= 0x2792 )	// Black circled numbers 1 to 9
		return	true;
#if	0
	else if	( ch >= 0xff10 && ch <= 0xff19 )	// Large numbers 0 to 9
		return	true;
	else if	( ch >= 0xff21 && ch <= 0xff3a )	// Large letters A to Z
		return	true;
	else if	( ch >= 0xff41 && ch <= 0xff5a )	// Large letters a to z
		return	true;
#endif
	else if	( ch >= 0xd800 && ch <= 0xdbff ){	// Letters in a surrogate pair
		UINT	uch = ( ch & 0x3ff ) << 10;
		ch = strLog[++xLines];
		if	( ch >= 0xdc00 && ch <= 0xdfff ){
			uch |= ( ch & 0x3ff );
			uch += 0x10000;
			if	( uch >= 0x1d400 && uch <= 0x1d6a3 )	// Decorated letters A to z
				return	true;
			else if	( uch >= 0x1f10b && uch <= 0x1f189 )	// Decorated letters 0 to Z
				return	true;
			else if	( uch >= 0x1f1e6 && uch <= 0x1f1ff )	// Small letters A to Z
				return	true;
		}
	}

	return	false;
}

void
CMainWnd::SetLinkVisible( CString& strLink, CAttr& attr )
{
	bool	bEncoded = false;
	for	( int x = 0;; ){
		int	n = strLink.GetLength();
		if	( x > n-3 )
			break;
		x = strLink.Find( _T("%"), x );
		if	( x < 0 )
			break;
		if	( x > n-2 )
			break;

		TCHAR*	pch = strLink.GetBuffer();
		pch += x+1;
		if	( isspace( pch[0] ) || isspace( pch[1] ) ){
			x += 2;
			continue;
		}
		TCHAR	achHex[3] = { 0 };
		achHex[0] = pch[0];
		achHex[1] = pch[1];
		TCHAR*	pchNew = NULL;
		WCHAR	wch = (WORD)wcstol( achHex, &pchNew, 16 );
		if	( pchNew != achHex+2 ){
			x++;
			continue;
		}

		if	( wch > 0x80 )
			bEncoded = true;

		strLink.Delete( x, 2 );
		strLink.SetAt( x, wch );
	}

	if	( bEncoded ){
		CStringA strEncoded;
		int	n = strLink.GetLength();
		for	( int x = 0; x < n; x++ )
			strEncoded += (char)strLink[x];
		strLink = StringFromCodePage( strEncoded, attr );
	}
}

void
CMainWnd::FilterError( UINT uIdError, CAttr& attr )
{
	if	( uIdError == IDS_RF_AUTH ){
		if	( attr.m_nAuth < m_nAuth )
			uIdError = 0;
	}
	else if	( uIdError >= IDS_RF_CHARSET && uIdError <= IDS_RF_EVASIVECODE ){
		int	i = uIdError-IDS_RF_CHARSET;
		if	( !( m_dwCode & (1<<i) ) )
			uIdError = 0;
	}
	else if	( uIdError == IDS_RF_DOMAIN ){
	}
	else if	( uIdError == IDS_RF_FAKE_ALIAS ){
	}
	else if	( uIdError >= IDS_RF_MESSAGEID && uIdError <= IDS_RF_LINK_FAKED ){
		int	i = uIdError-IDS_RF_MESSAGEID;
		if	( !( m_dwSender & (1<<i) ) )
			uIdError = 0;
	}
	else if	( uIdError == IDS_RF_LINK_DOMAIN ){
	}
	else if	( uIdError == IDS_RF_LINK_EVASIVE ){
		if	( !( m_dwCode & (1<<(IDS_RF_EVASIVECODE-IDS_RF_CHARSET)) ) )
			uIdError = 0;
	}
	else if	( uIdError == IDS_RF_TIMEZONE ){
	}

	if	( uIdError ){
		uIdError -= IDS_RF_AUTH;
		attr.m_dwReason |= 1<<uIdError;
	}
}

void
CMainWnd::ConnectPOP( void )
{
	if	( ReadFromEML( NULL ) ){
		KillTimer( TID_POLL );
		return;
	}

	if	( !m_aAccount.GetCount() )
		return;

	CString	strTip;
	(void)strTip.LoadString( IDS_NI_CONNECTING );
	ModNI( IDI_YELLOW, strTip );

	m_pSocket = new CParaSocket;

	m_iPhase = 0;
	m_iMessage = 0;
	m_nMessage = 0;

	bool	bError = true;

	if	( !m_pSocket->Create() )
		;
	else if	( !m_pSocket->Connect( m_aAccount[m_iUser].m_strHost, m_aAccount[m_iUser].m_nPort ) ){
		DWORD	dwError = m_pSocket->GetLastError();
		if	( dwError == 0 )
			bError = false;
		else if	( dwError == WSAEWOULDBLOCK )
			bError = false;
	}

	if	( bError ){
		DWORD	dwError = m_pSocket->GetLastError();
		ClosePOP( dwError );
		delete	m_pSocket;
	}
	else
		m_iPhase = 1;
}

#ifdef	UNICODE
#undef	atoi
#endif//UNICODE

//#define	DBGOUTPUT

void
CMainWnd::RespondPOP( CStringA strMessage )
{
	if	( !m_iPhase )
		return;

	CStringA strCommand;
	CStringA str;

#ifdef	DBGOUTPUT
	str.Format( "\nPhase %d:\n", m_iPhase );
	OutputDebugStringA( str );
	OutputDebugStringA( "> " );//DBG
	OutputDebugStringA( strMessage );//DBG
#endif//DBGOUTPUT

	switch	( m_iPhase ){
		case	1:	// Connected: to send USER
			if	( strMessage.IsEmpty() ){
				ClosePOP( -1 );
			}
			else{
				str = m_aAccount[m_iUser].m_strUser;
				strCommand.Format( "USER %s\r\n", str.GetBuffer() );
				m_strMail.Empty();
				m_iPhase++;
			}
			break;
		case	2:	// Sent USER: to send PASS
			if	( strMessage.Left( 3 ) == "+OK" ){
				str = m_aAccount[m_iUser].m_strPass;
				strCommand.Format( "PASS %s\r\n", str.GetBuffer() );
				m_iPhase++;
			}
			else{
				strCommand = "QUIT\r\n";
				m_iPhase = 10;
			}
			break;
		case	3:	// Sent PASS: to send STAT
			if	( strMessage.Left( 3 ) == "+OK" ){
				strCommand = "STAT\r\n";
				m_iPhase++;
			}
			else{
				strCommand = "QUIT\r\n";
				m_iPhase = 10;
			}
			break;
		case	4:	// Sent STAT: to receice STAT
			if	( strMessage.Left( 3 ) == "+OK" ){
				CStringA strArgument = strMessage.Mid( 4 );
				m_nMessage = atoi( strArgument );
				m_iMessage = 0;
				if	( !m_nMessage ){
					strCommand = "QUIT\r\n";
					m_iPhase = 8;
					break;
				}
				else
					m_iPhase = 5;
					// fall through
			}
			else{
				strCommand = "QUIT\r\n";
				m_iPhase = 10;
				break;
			}
		case	5:	// Received STAT: to send RETR
			strCommand.Format( "RETR %d\r\n", ++m_iMessage );
			m_iPhase++;
			break;
		case	6:	// Sent RETR: to receive RETR
			if	( m_strMail.IsEmpty() &&
				  strMessage.Left( 3 ) == "+OK" ){
				int	x = strMessage.Find( "\n" );
				strMessage.Delete( 0, x+1 );
			}

			m_strMail += strMessage;
			if	( m_strMail.Right( 5 ) == "\r\n.\r\n" ){
				m_strMail.Delete( m_strMail.GetLength()-3, 3 );
				bool	bKeep = ParseMail( m_strMail );
				m_strMail.Empty();
				if	( !bKeep ){
					strCommand.Format( "DELE %d\r\n", m_iMessage );
					m_iPhase = 7;
					break;
				}
				else
					m_nMail++;
					// fall through
			}
			else
				break;
		case	7:	// Received RETR: to send QUIT / RETR
			if	( m_iMessage >= m_nMessage ){
				strCommand = "QUIT\r\n";
				m_iPhase = 8;
			}
			else{
				strCommand.Format( "RETR %d\r\n", ++m_iMessage );
				m_iPhase = 6;
			}
			break;
		case	8:	// Sent QUIT: to close and to connect the next user
			ClosePOP( 0 );
			SetTimer( TID_CLOSE, 0, NULL );
			m_iPhase++;
			break;
		case	9:
			// All users done: Show the result.

			if	( ++m_iUser >= m_aAccount.GetCount() ){
				m_iUser = 0;
				ModNI( 0, NULL );
			}

			// There's the next: Connect in the name of the user.

			else{
				m_iPhase = 0;
				ConnectPOP();
			}
			break;
		case	10:
			ClosePOP( -1 );
			break;
	}

	if	( !m_pSocket )
		;
	else if	( !strCommand.IsEmpty() )
		m_pSocket->Send( (void*)strCommand.GetBuffer(), strCommand.GetLength() );

#ifdef	DBGOUTPUT
	if	( !strCommand.IsEmpty() ){//DBG
		OutputDebugStringA( "< " );//DBG
		OutputDebugStringA( strCommand );//DBG
	}
#endif//DBGOUTPUT
}

void
CMainWnd::ClosePOP( int nError )
{
	CString	strError;
	if	( m_iPhase >= 1 && m_iPhase <= 3 )
		nError = -1;

	if	( nError == -1 )
		(void)strError.LoadString( IDS_NI_FAIL_POP );
	else
		(void)strError.LoadString( IDS_NI_FAIL_CONNECT );

	if	( nError ){
		m_aAccount[m_iUser].m_nError = nError;
		ModNI( IDI_RED, strError );
		KillTimer( TID_POLL );
	}

	if	( m_pSocket ){
		m_pSocket->Close();
		delete	m_pSocket;
		m_pSocket = NULL;
	}
}

///////////////////////////////////////////////////////////////////////////////////////
// EML log re-evaluation

bool
CMainWnd::ReadFromEML( LPCTSTR pchFolder )
{
	{
		CString	strTip;
		(void)strTip.LoadString( IDS_NI_REEVAL );
		ModNI( IDI_YELLOW, strTip );
	}

	CString	strPath;
	if	( pchFolder ){
		strPath = pchFolder;
	}
	else{
		TCHAR	ach[_MAX_PATH];
		GetModuleFileName( NULL, ach, _countof( ach ) );
		strPath = ach;
	
		int	i = strPath.ReverseFind( '\\' );
		strPath = strPath.Left( i+1 );
		strPath += _T("Mails\\");
	}

	class	CFileTime
	{
	public:
		CString	m_strFile;
		CTime	m_time;
	};
	CArray	<CFileTime, CFileTime&>
		aFileTime;

	CFileStatus	fs;
	if	( CFile::GetStatus( strPath, fs ) ){
		CFileFind finder;
		CString	strFile;
		BOOL	  bWorking = finder.FindFile( strPath + _T("*.eml") );
		if	( bWorking ){
			m_nMail = 0;
			m_strSummary.Empty();
		}
		while	( bWorking ){
			bWorking = finder.FindNextFile();
			strFile.Format( _T("%s%s"), strPath.GetBuffer(), finder.GetFileName().GetBuffer() );
			CFileTime ft;
			ft.m_strFile = strFile;
			ft.m_time = 0;

			CFile	fIn;
			if	( fIn.Open( strFile, CFile::modeRead | CFile::shareDenyNone ) ){
				UINT	cch = (UINT)fIn.GetLength();
				char*	pch = new char[cch+1];
				fIn.Read( pch, cch );
				pch[cch] = '\0';
				CAttr	attr;
				GetTime( pch, attr );
				ft.m_time = attr.m_time;
				delete	[] pch;
			}

			INT_PTR	i, n = aFileTime.GetCount();
			for	( i = 0; i < n; i++ ){
				if	( aFileTime[i].m_time > ft.m_time ){
					aFileTime.InsertAt( i, ft );
					break;
				}
			}
			if	( i >= n )
				aFileTime.Add( ft );
		}
	}

	INT_PTR	i, n = aFileTime.GetCount();

	for	( i = 0; i < n; i++ )
		ReadEML( aFileTime[i].m_strFile );

	if	( n )
		ModNI( 0, NULL );

	return	n? true: false;
}

void
CMainWnd::ReadEML( CString strFile )
{
	CFile	fIn;
	if	( fIn.Open( strFile, CFile::modeRead | CFile::shareDenyNone ) ){

		UINT	cch = (UINT)fIn.GetLength();
		char*	pch = new char[cch+1];
		fIn.Read( pch, cch );
		pch[cch] = '\0';
		int	x = strFile.ReverseFind( '\\' );
		strFile = strFile.Mid( x+1 );
		x = strFile.ReverseFind( '.' );
		strFile = strFile.Left( x );
		if	( strFile[0] == '!' )
			strFile.Delete( 0, 1 );

		if	( ParseMail( pch, strFile ) )
			m_nMail++;

		delete	[] pch;
	}
}
