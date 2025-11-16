// MainWnd.cpp : implementation file
//

#include "pch.h"
#include "ChkMails.h"
#include "MainWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#ifdef	_UNICODE
#define	strcpy_s	wcscpy_s
#endif//_UNICODE

#define	TID_INIT	1
#define	TID_READY	2
#define	TID_START	3
#define	TID_POLL	4
#define	TID_CLOSE	5

#define	UC_SPACE	0x0001	// to cut off to compare name
#define	UC_ASCII	0x0002	// to apply to compare name
#define	UC_EVASIVE	0x0004	// to replace to compare name
#define	UC_CONTROLCODE	0x0008	// to detect as a doubtful control code
#define	UC_SYMBOL	0x0010	// to ignore
#define	UC_OTHER	0x0020	// to leave
#define	UC_REPLACEMENT	L"\xfffd"

///////////////////////////////////////////////////////////////////////////////////////
// Constructor & Destructor

CMainWnd::CMainWnd( void )
	: CWnd()
{
	LoadSettings();
	LoadFilters();
	LoadTLDs();

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
	m_nWakeDelay = 0;
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
	if	( m_hSummary ){
		CloseHandle( m_hSummary );
		m_hSummary = NULL;
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

	m_nWakeDelay = AfxGetApp()->GetProfileInt( _T("Settings"), _T("WakeDelay"),   5 );
	DEVICE_NOTIFY_SUBSCRIBE_PARAMETERS	dnsp = { OnPower, this };
	PowerRegisterSuspendResumeNotification( DEVICE_NOTIFY_CALLBACK, &dnsp, &m_hPower );

	// Get the default language.
	// https://learn.microsoft.com/en-us/windows-hardware/manufacture/desktop/available-language-packs-for-windows

	WCHAR	achLocale[LOCALE_NAME_MAX_LENGTH] = {};
	GetUserDefaultLocaleName( achLocale, _countof( achLocale ) );
	m_strLanguage = achLocale;

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

	CString	strPath = _T("https://github.com/inhouse-tool/chkmails");
	strURL.Insert( 0, _T("#") );
	strURL.Insert( 0, strPath );
	ShellExecute( NULL, _T("open"), strURL, NULL, NULL, SW_SHOWNORMAL );

	return	0;
}

void
CMainWnd::OnMenuWeb( void )
{
	CString	strPath = _T("https://github.com/inhouse-tool/chkmails");
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
#ifdef	_UNICODE
	strText.Replace( L"(C)", L"\x00a9" );
#endif//_UNICODE

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

	sheet.m_pageAuth     .m_nAuth         = m_nAuth;
	sheet.m_pageAuth     .m_dwFlags       = m_dwAuthCache;
	sheet.m_pageCode     .m_dwFlags       = m_dwCode;
	sheet.m_pageCode     .m_strFont       = m_strCodeFont;
	sheet.m_pageDomain   .m_strDomains    = m_strDomains;
	sheet.m_pageDomain   .m_strTLDCache   = m_strTLDCache;
	sheet.m_pageName     .m_strNames      = m_strNames;
	sheet.m_pageRecipient.m_strRecipients = m_strRecipients;
	sheet.m_pageRecipient.m_strSenders    = m_strSenders;
	sheet.m_pageSender   .m_dwFlags       = m_dwSender;
	sheet.m_pageZone     .m_strTimes      = m_strTimes;
	sheet.m_pageWhite    .m_strWhites     = m_strWhites;

	sheet.SetOwner( this );

	if	( sheet.DoModal() != IDOK )
		return;

	bool	bUpdate = false;

	if	( sheet.m_pageAuth  .m_nAuth            != m_nAuth ){
		m_nAuth         = sheet.m_pageAuth        .m_nAuth;
		bUpdate = true;
	}
	if	( sheet.m_pageCode  .m_dwFlags          != m_dwCode ){
		m_dwCode        = sheet.m_pageCode        .m_dwFlags;
		bUpdate = true;
	}
	if	( sheet.m_pageCode  .m_strFont          != m_strCodeFont ){
		m_strCodeFont   = sheet.m_pageCode        .m_strFont;
		m_charmap.SetFontName( m_strCodeFont );
		bUpdate = true;
	}
	if	( sheet.m_pageDomain.m_strDomains       != m_strDomains ){
		m_strDomains    = sheet.m_pageDomain      .m_strDomains;
		bUpdate = true;
	}
	if	( sheet.m_pageDomain.m_strTLDCache      != m_strTLDCache ){
		m_strTLDCache   = sheet.m_pageDomain      .m_strTLDCache;
		SaveTLDs();
	}
	if	( sheet.m_pageName  .m_strNames         != m_strNames ){
		m_strNames      = sheet.m_pageName        .m_strNames;
		bUpdate = true;
	}
	if	( sheet.m_pageRecipient.m_strRecipients != m_strRecipients ){
		m_strRecipients = sheet.m_pageRecipient   .m_strRecipients;
		bUpdate = true;
	}
	if	( sheet.m_pageRecipient.m_strSenders    != m_strSenders ){
		m_strSenders    = sheet.m_pageRecipient   .m_strSenders;
		bUpdate = true;
	}
	if	( sheet.m_pageSender.m_dwFlags          != m_dwSender ){
		m_dwSender      = sheet.m_pageSender      .m_dwFlags;
		bUpdate = true;
	}
	if	( sheet.m_pageZone  .m_strTimes         != m_strTimes ){
		m_strTimes      = sheet.m_pageZone        .m_strTimes;
		bUpdate = true;
	}
	if	( sheet.m_pageWhite .m_strWhites        != m_strWhites ){
		m_strWhites     = sheet.m_pageWhite       .m_strWhites;
		bUpdate = true;
	}

	if	( bUpdate )
		SaveFilters();

	if	( bUpdate )
		if	( AfxMessageBox( IDS_MB_REEVAL, MB_YESNO | MB_ICONQUESTION ) == IDYES ){
			KillTimer( TID_POLL );
			ReadFromEML( m_strLogPath );
			PollNow();
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
			if	( !m_strSummary.IsEmpty() )
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
		CString	strEntry;
		strEntry.Format( _T("Entry%d"), i );
		CString strRecord = pApp->GetProfileCode( _T("Records"), strEntry, _T("") );
		if	( strRecord.IsEmpty() )
			break;

		CAccount	account;
		int	x = 0;
		account.m_strHost = strRecord.Tokenize( _T("\n"), x );
		account.m_strUser = strRecord.Tokenize( _T("\n"), x );
		account.m_strPass = strRecord.Tokenize( _T("\n"), x );
		account.m_nPort   = _wtoi( strRecord.Tokenize( _T("\n"), x ) );
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
		CString	strRecord;
		strRecord += m_aAccount[i].m_strHost;
		strRecord += "\n";
		strRecord += m_aAccount[i].m_strUser;
		strRecord += "\n";
		strRecord += m_aAccount[i].m_strPass;
		strRecord += "\n";
		CStringA str;
		str.Format( "%d\n", m_aAccount[i].m_nPort );
		strRecord += str;

		CString	strEntry;
		strEntry.Format( _T("Entry%d"), i );
		pApp->WriteProfileCode( _T("Records"), strEntry, strRecord );
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

	m_nAuth         = pApp->GetProfileInt(    _T("Filters"), _T("AuthFail"),   2 );
	m_dwAuthCache   = pApp->GetProfileInt(    _T("Filters"), _T("AuthBits"),   0x0000 );
	m_dwCode        = pApp->GetProfileInt(    _T("Filters"), _T("Code"),       0xffff );
	m_strCodeFont   = pApp->GetProfileString( _T("Filters"), _T("CodeFont"),   _T("") );
	if	( m_strCodeFont.IsEmpty() )
		m_dwCode &= ~( 1<<3 );
	m_charmap.SetFontName( m_strCodeFont );
	m_strDomains    = pApp->GetProfileString( _T("Filters"), _T("Drop"),       _T("") );
	m_strNames      = pApp->GetProfileString( _T("Filters"), _T("Name"),       _T("") );
	m_strRecipients = pApp->GetProfileCode(   _T("Filters"), _T("Recipients"), _T("") );
	m_strSenders    = pApp->GetProfileCode(   _T("Filters"), _T("Senders"),    _T("") );
	m_dwSender      = pApp->GetProfileInt(    _T("Filters"), _T("Sender"),     0xffff );
	m_strTimes      = pApp->GetProfileString( _T("Filters"), _T("TimeZone"),   _T("") );
	m_strWhites     = pApp->GetProfileString( _T("Filters"), _T("White"),      _T("") );
}

void
CMainWnd::SaveFilters( void )
{
	CChkMailsApp*	pApp = (CChkMailsApp*)AfxGetApp();

	pApp->WriteProfileInt(    _T("Filters"), _T("AuthFail"),   m_nAuth );
	pApp->WriteProfileInt(    _T("Filters"), _T("AuthBits"),   m_dwAuthCache );
	pApp->WriteProfileInt(    _T("Filters"), _T("Code"),       m_dwCode );
	pApp->WriteProfileString( _T("Filters"), _T("CodeFont"),   m_strCodeFont );
	pApp->WriteProfileString( _T("Filters"), _T("Drop"),       m_strDomains );
	pApp->WriteProfileString( _T("Filters"), _T("Name"),       m_strNames );
	pApp->WriteProfileCode(   _T("Filters"), _T("Recipients"), m_strRecipients );
	pApp->WriteProfileCode(   _T("Filters"), _T("Senders"),    m_strSenders );
	pApp->WriteProfileInt(    _T("Filters"), _T("Sender"),     m_dwSender );
	pApp->WriteProfileString( _T("Filters"), _T("TimeZone"),   m_strTimes );
	pApp->WriteProfileString( _T("Filters"), _T("White"),      m_strWhites );
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

ULONG
CMainWnd::OnPower( void* pContext, ULONG uType, void* pSetting )
{
	if	( uType == PBT_APMRESUMESUSPEND ){
		CMainWnd*	pWnd = (CMainWnd*)pContext;
		pWnd->SetTimer( TID_START, pWnd->m_nWakeDelay*1000, NULL );
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
	TRY
	{
		bool	bKeep = true;
		CAttr	attr;
	
		// Get `attributes` of the mail.

		int	iHeader = strMail.Find( "\r\n\r\n" );
		if	( iHeader < 0 )
			return	false;

		CStringA strHeader = strMail.Left( iHeader+4 );

		attr = GetAttr( strHeader );

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
	CATCH_ALL(e)
	{
#if	1
		e->ReportError();	// This only makes useless report as "The Parameter Is Incorrect".
#endif
		e->Delete();
		return	true;
	}
	END_CATCH_ALL
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
	GetDate(   strMail, attr );
	CheckToCc(     strMail, attr );
	CheckMID(      strMail, attr );
	CheckReceived( strMail, attr );

	return	attr;
}

void
CMainWnd::GetAuth( CStringA strMail, CAttr& attr )
{
	int	x;

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
		int	xIn  = strMail.Find( ": ", x ) +2;
		int	xOut = strMail.Find( "\r\n\r\n", xIn );
		CStringA str = strMail.Mid( xIn, xOut-xIn );
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
				x = strResult.FindOneOf( " ;\t\r\n" );
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
	// Get nominal sender.

	int	x = FindHeaderField( strMail, "\nFrom: " );
	if	( x >= 0 ){

		// Extract sender address.

		CStringA strFrom = GetHeaderField( strMail, x );

		int	xIn  = strFrom.Find( '<', 0 )+1;
		int	xOut = strFrom.Find( '>', xIn );
		if	( xIn >= 0 && xOut >= xIn )
			strFrom = strFrom.Mid( xIn, xOut-xIn );
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
	// Get actual sender from smtp.mailfrom in Authentication-Results: ( RFC 5451 ) 

	int	x = strMail.Find( "smtp.mailfrom=" );
	if	( x >= 0 ){
		int	xIn  = strMail.Find( '=', x )+1;
		int	xOut = strMail.Find( ';', xIn );
		CStringA strSender = strMail.Mid( xIn, xOut-xIn );
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
	int	x = FindHeaderField( strMail, "\nContent-Type: " );

	if	( x >= 0 ){
		CStringA strField = GetHeaderField( strMail, x );
		x = strField.Find( ';' );
		CStringA strType = strField.Left( x );
		strField.Delete( 0, x+1 );
		strField.TrimLeft();
		CStringA strBoundary = strField;
		x = strType.Find( '/' );
		CStringA strSubType = strType.Mid( x+1 );
		strType = strType.Left( x );
		strField.MakeLower();
		strType.MakeLower();
		strSubType.MakeLower();

		if	( strType == "text" ){
			attr.m_iType = CAttr::Text;
			if	( strSubType == "plain" )
				attr.m_iSubType = CAttr::Plain;
			else if	( strSubType == "html" )
				attr.m_iSubType = CAttr::HTML;
		}
		else if	( strType == "multipart" )
			attr.m_iType = CAttr::Multipart;

		x = strField.Find( "charset=" );
		if	( x >= 0 ){
			CStringA strCharset = strField;
			x += 8;
			strCharset.Delete( 0, x );
			x = strCharset.Find( ';' );
			if	( x >= 0 )
				strCharset = strCharset.Left( x );
			if	( strCharset[0] == '"' ){
				strCharset.Delete( 0, 1 );
				x = strCharset.Find( '"' );
			}
			else
				x = strCharset.Find( "\r\n" );
			if	( x >= 0 )
				strCharset = strCharset.Left( x );

			attr.m_iCharset = GetCodePage( strCharset );
			if	( !attr.m_iCharset )
				FilterError( IDS_RF_CHARSET, attr );
		}

		x = strField.Find( "boundary=" );
		if	( x >= 0 ){
			x += 9;
			strBoundary.Delete( 0, x );
			x = strBoundary.Find( ';' );
			if	( x >= 0 )
				strBoundary = strBoundary.Left( x );
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

void
CMainWnd::GetEncode( CStringA strMail, CAttr& attr )
{
	int	x = FindHeaderField( strMail, "\nContent-Transfer-Encoding: " );

	if	( x >= 0 ){
		CStringA str = GetHeaderField( strMail, x );
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
CMainWnd::GetDate( CStringA strMail, CAttr& attr )
{
	int	x = FindHeaderField( strMail, "\nDate: " );

	if	( x >= 0 ){

		// Take 'Date:'.

		CStringA strDate = GetHeaderField( strMail, x );

		// Get CTime from the 'Date:'.

		attr.m_time = GetTime( strDate, attr );

		// Check if the timezone is filtered.

		x = 0;
		for	( ;; ){
			CString	strTime = m_strTimes.Tokenize( _T("\n"), x );
			if	( strTime.IsEmpty() )
				break;
			if	( strTime == (CString)attr.m_strZone ){
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
		if	( !strMail.CompareNoCase( "GB18030" ) )
			iCodePage = CP_GB18030;
		else
			TRACE( "code page '%s' is unknown.\n", strMail );
	}

	return	iCodePage;
}

void
CMainWnd::CheckToCc( CStringA strMail, CAttr& attr )
{
	// No recipients to check, skip this check.

	if	( m_strRecipients.IsEmpty() )
		return;

	CStringA strRecipients;

	// Get nominal recipients.

	int	x = FindHeaderField( strMail, "\nTo: " );
	if	( x >= 0 )
		strRecipients += GetHeaderField( strMail, x );

	// Get carbon copies.

	x = strMail.Find( "Cc: " );
	if	( x < 0 )
		x = strMail.Find( "cc: " );
	if	( x < 0 )
		x = strMail.Find( "CC: " );
	if	( x < 0 )
		x = strMail.Find( "cC: " );
	if	( x >= 0 )
		strRecipients += GetHeaderField( strMail, x );

	// Do nothing in the turn of a partial check.

	if	( strRecipients.IsEmpty() )
		return;

	// Check each recipient.

	bool	bKeep = false;
	while	( !bKeep ){
		if	( strRecipients.IsEmpty() )
			break;

		CStringA strAddr;
		x = strRecipients.Find( '<' );
		if	( x >= 0 ){
			{
				strAddr = strRecipients.Mid( x+1 );
				int	i = strAddr.Find( '>' );
				strAddr = strAddr.Left( i );
				x += i+1;
			}
		}
		else{
			x = strRecipients.Find( ',' );
			if	( x < 0 ){
				strAddr = strRecipients;
				strRecipients.Empty();
			}
			else
				strAddr = strRecipients.Left( x );
		}
		if	( x > 0 ){
			strRecipients.Delete( 0, x+1 );
			strRecipients.TrimLeft();
			if	( strRecipients[0] == ',' ){
				strRecipients.Delete( 0, 1 );
				strRecipients.TrimLeft();
			}
		}

		CString	strTo = CString( strAddr );
		x = 0;
		while	( !bKeep ){
			CString	strRecipient = m_strRecipients.Tokenize( _T("\n"), x );
			if	( strRecipient.IsEmpty() )
				break;
			if	( strTo.Find( strRecipient ) >= 0 )
				bKeep = true;
		}
	}

	// If the sender belongs to the trusted domain, keep this mail.

	if	( !bKeep ){
		CString	strFrom = CString( attr.m_strFrom );
		x = 0;
		for	( ;; ){
			CString	strTrusted = m_strSenders.Tokenize( _T("\n"), x );
			if	( strTrusted.IsEmpty() )
				break;
			if	( strFrom.Find( strTrusted ) >= 0 ){
				bKeep = true;
				break;
			}
		}
	}

	// From untrusted sender to unknown recipient, discard it.

	if	( !bKeep )
		FilterError( IDS_RF_RECIPIENT, attr );
}

void
CMainWnd::CheckMID( CStringA strMail, CAttr& attr )
{
	int	x = FindHeaderField( strMail, "\nMessage-ID: " );

	if	( x >= 0 ){
		int	xIn  = strMail.Find( "<", x )+1;
		int	xOut = strMail.Find( ">", xIn );
		CStringA strMessageId = strMail.Mid( xIn, xOut-xIn );
		x = strMessageId.ReverseFind( '@' );
		bool	bIP = true;
		do{
			if	( x < 0 )
				break;
			CStringA strRight = strMessageId.Mid( x+1 );
			CStringA strLeft  = strMessageId.Left( x );
			if	( strLeft.IsEmpty() ){
				x = -1;
				break;
			}
			x = strRight.Find( '.' );
			if	( x < 0 )
				break;
			for	( int i = 0; i < strRight.GetLength(); i++ ){
				char	ch = strRight[i];
				if	( ch >= 'A' && ch <= 'Z' ){
					x = -1;
					break;
				}
				else if	( ch >= '0' && ch <= '9' )
					;
				else if	( ch == '.' )
					;
				else
					bIP = false;
			}
			// We abandoned check of the left side ( in strLeft above )
			// as we observed some senders do not conform to RFC 5322...
		}while	( 0 );
		if	( x < 0 || bIP )
			FilterError( IDS_RF_MESSAGEID, attr );
	}
}

void
CMainWnd::CheckReceived( CStringA strMail, CAttr& attr )
{
	// Get the last ( the first ) 'Received: '.

	int	xIn = -1;
	int	x = 0;
	do{
		x = strMail.Find( "\r\nReceived: ", x );
		if	( x < 0 )
			break;
		x += 2+10;
		xIn = x;
	}while	( true );
	if	( xIn < 0 )
		return;

	int	xOut = -1;
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

	// Get the time in the last 'Received:'.

	x = strReceived.ReverseFind( ';' );
	CStringA strTime = strReceived.Mid( x+1 );
	strTime.Trim();
	CAttr	attrTime;
	CTime	time = GetTime( strTime, attrTime );

	// Check if the time zone is unreliale.

	if	( !attrTime.m_strZone.IsEmpty() ){
		x = 0;
		for	( ;; ){
			CString	strTime = m_strTimes.Tokenize( _T("\n"), x );
			if	( strTime.IsEmpty() )
				break;
			if	( strTime == (CString)attrTime.m_strZone ){
				FilterError( IDS_RF_TIMEZONE, attr );
				break;
			}
		}

		// Check if the time in the last 'Received:' is in the past of 'Date:'.

		if	( time < attr.m_time ){
			attr.m_time = time;
			FilterError( IDS_RF_TIMEZONE, attr );
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

CTime
CMainWnd::GetTime( CStringA strDate, CAttr& attr )
{
	// Get time according to RFC 5322 and save it in UTC.

	int	x;
	long	lBias = 0;
	{
		CStringA strZone;
		x = strDate.ReverseFind( '+' );
		if	( x < 0 )
			x = strDate.ReverseFind( '-' );
		if	( x >= 0 ){
			strZone = strDate.Mid( x, 5 );
			strDate = strDate.Left( x );
		}
		else{
			x = strDate.ReverseFind( ' ' );
			strZone = strDate.Mid( x+1 );
			strZone.Trim();
			strZone.MakeUpper();
			if	( strZone[0] == '(' ){
				strZone.Delete( 0, 1 );
				strZone.Delete( strZone.GetLength()-1, 1 );
			}

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
		attr.m_strZone = strZone;
		lBias = atoi( strZone );
	}

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

		char*	pchDate = strDate.GetBuffer();
		char*	pch = NULL;
		nDay = strtol( pchDate, &pch, 10 );
		pch++;
		char*	apchMonth[] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec", NULL };
		for	( int i = 0; apchMonth[i]; i++ )
			if	( !strncmp( pch, apchMonth[i], 3 ) )
				nMonth = i+1;
		if	( nMonth )
			pch += 4;
		else{
			nMonth = strtol( pch, &pch, 10 );
			if	( nMonth <= 0 || nMonth > 12 ){
				bError = true;	// missing month
				break;
			}
		}
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
		return	CTime( 0 );
	}

	CTime	time( nYear, nMonth, nDay, nHour, nMinute, nSecond );
		
	bool	bMinus = false;

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

	return	time;
}

int
CMainWnd::FindHeaderField( CStringA strMail, CStringA strField )
{
	int	x = strMail.Find( strField );
	if	( x < 0 ){
		CStringA strLower = strMail.MakeLower();
		x = strLower.Find( strField.MakeLower() );
	}

	return	x;
}

CStringA
CMainWnd::GetHeaderField( CStringA strMail, int iField )
{
	int	xIn = strMail.Find( ": ", iField );
	if	( xIn < 0 )
		return	NULL;

	for	( ++xIn; strMail[xIn] <= ' '; xIn++ )
		;

	CStringA strField = strMail.Mid( xIn );
	int	xBody = strMail.Find( "\r\n\r\n", xIn );
	int	xOut  = strMail.Find( ": ",       xIn );
	if	( xOut < 0 )
		xOut = xBody;
	else if	( xBody < xOut )
		xOut = xBody;

	strField = strMail.Mid( xIn, xOut-xIn );

	if	( xOut < xBody ){
		xOut = strField.ReverseFind( '\r' );
		if	( xOut >= 0 )
			strField = strField.Left( xOut );
		strField.Replace( "\r\n\t", " " );
	}

	return	strField;
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

	// Check 'alias' in 'Subject:'.

	CheckSubject( strLog, attr );

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

			// Get 'Content-Type:' and 'Content-Transfer-Encoding:' of the part body.

			int	iHeader = strPart.Find( "\r\n\r\n" );
			CStringA strPartHeader = strPart.Left( iHeader+4 );
			strPartHeader.Insert( 0, "\n" );
			CAttr	attrPart = GetAttr( strPartHeader );
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
		for	( UINT uID = IDS_RF_AUTH; uID <= IDS_RF_LINK_EVASIVE; uID++ ){
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
			// https://www.google.com/search?q=stop+windows+11+to+change+date+modified+KB5039212
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

#ifdef	_UNICODE
#define	atoi	_wtoi
#endif//_UNICODE

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

#ifdef	_UNICODE
#undef	atoi
#endif//_UNICODE

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

	int	x;

	do{
		x = strLog.Find( _T("Subject: ") );
		if	( x >= 0 )
			break;
		CString	strLower = strLog.MakeLower();
		x = strLower.Find( _T("subject: ") );
	}while	( 0 );

	if	( x >= 0 ){
		int	xIn  = strLog.Find( _T(": "), x );
		int	xOut = strLog.Find( _T("\r\n"), xIn+2 );
		CString str = strLog.Mid( xIn+2, xOut-(xIn+2) );
		str.TrimRight();
		strSummary += str + _T("\n");
	}

	do{
		x = strLog.Find( _T("From: ") );
		if	( x >= 0 )
			break;
		CString	strLower = strLog.MakeLower();
		x = strLower.Find( _T("from: ") );
	}while	( 0 );

	if	( x >= 0 ){
		int	xIn  = strLog.Find( _T(": "), x );
		int	xOut = strLog.Find( _T("\r\n"), xIn+2 );
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

	while	( !strIn.IsEmpty() ){
		bool	bQuoted = false;

		CStringA strEncoded;
		for	( ;; ){
			// Find an encoded part.

			int	x = strIn.Find( "=?" );
			if	( x < 0 ){
				strOut += strIn;
				strIn.Empty();
				break;
			}

			// Get charset.

			int	i = strIn.Find( "?", x+2 );
			CStringA strCharset = strIn.Mid( x+2, i-(x+2) );
			i++;
			attr.m_iCharset = GetCodePage( strCharset );
			if	( !attr.m_iCharset ){
				strOut += strIn;
				FilterError( IDS_RF_CHARSET, attr );
				break;
			}

			// Get 'quoted' or not ( 'base64' ).

			if	( strIn[i] == 'Q' ||
				  strIn[i] == 'q'    )
				bQuoted = true;
			i = strIn.Find( "?", i+1 );

			// Append non-encoded part.
			{
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
			}
			strIn.Delete( 0, i+1 );

			// Find the tail of the encoded part.
			//	x = strIn.Find( "?=" );
			//	( CStringA::Find as above may fail just after an encoded binary code. We use memcmp as bellow. )
			{
				x = 0;
				BYTE*	pb = (BYTE*)strIn.GetBuffer();
				for	( x = 0; pb[x]; x++ )
					if	( !memcmp( pb+x, "?=", 2 ) )
						break;
				if	( !pb[x] )
					x = -1;
			}

			// Append the encoded part.

			if	( x >= 0 ){
				CStringA strNew = strIn.Left( x );
				int	i = 0;
				for	( ;; ){
					i = strNew.Find( "\r\n" );
					if	( i < 0 )
						break;
					strNew.Delete( i, 2 );
					while	( strNew[i] == '\t' ||
						  strNew[i] == ' ' )
						strNew.Delete( i, 1 );
				}
				strEncoded += strNew;
				strIn = strIn.Mid( x+2 );
			}
			else{
				strIn.Empty();
				break;
			}

			// Finish if the field ends.

			if	( strIn.Left( 5 ) == "\r\n\t=?" )
				;
			else if	( strIn.Left( 5 ) == "\r\n =?" )
				;
			else
				break;
		}

		CStringA strDecoded;
		if	( strEncoded.IsEmpty() )
			;
		else if	( attr.m_iEncode == CAttr::Binary )
			strDecoded = strEncoded;
		else if	( bQuoted )
			strDecoded = DecodeQuoted( strEncoded );
		else
			strDecoded = DecodeBase64( strEncoded );

		strOut += StringFromCodePage( strDecoded, attr );
	}

	attr.m_iCharset = iCharset;

	CheckUnicode( strOut, attr );

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
		strDecoded = strIn;
	else if	( attr.m_iEncode == CAttr::Binary )
		strDecoded = strIn;
	else if	( attr.m_iEncode == CAttr::Bit7 )
		strDecoded = strIn;
	else if	( attr.m_iEncode == CAttr::Bit8 )
		strDecoded = strIn;
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
		if	( x < 0 ){
			ib = 0;
			continue;
		}

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
	ba[nb++] = '\0';

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

	SetLastError( 0 );
	CString	strOut;
	char*	pchIn = strIn.GetBuffer();
	int	cchOut = ::MultiByteToWideChar( attr.m_iCharset, MB_ERR_INVALID_CHARS, pchIn, -1, NULL, 0 );

	DWORD	dwError = GetLastError();
	if	( dwError == ERROR_NO_UNICODE_TRANSLATION ){
		FilterError( IDS_RF_CHARSET, attr );
		strOut = _T("<<UNREADABLE>>");
	}
	else{
		WCHAR*	pchOut = new WCHAR[cchOut];
		::MultiByteToWideChar( attr.m_iCharset, MB_ERR_INVALID_CHARS, pchIn, -1, pchOut, cchOut );
		strOut = pchOut;
		delete []pchOut;
	}

	return	strOut;
}

#include <locale.h>

CStringA
CMainWnd::EscapeFromJIS( CStringA strIn, CAttr& attr )
{
	_locale_t	lt = _wcreate_locale( LC_ALL, _T("ja-JP") );

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
			w =_mbcjistojms_l( w, lt );
			strOut += HIBYTE( w );
			strOut += LOBYTE( w );
			i += 2;
		}

		// Kana: Replace it with a SJIS code.

		else if	( bKana ){
			CStringA	strConvert =
				"000102030405060708090A0B0C0D0E0F"
				"101112131415161718191A1B1C1D1E1F"
				"\x81\x40\x81\x42\x81\x75\x81\x76\x81\x41\x81\x45\x83\x92\x83\x40"
				"\x83\x42\x83\x44\x83\x46\x83\x48\x83\x83\x83\x85\x83\x87\x83\x62"
				"\x81\x5b\x83\x41\x83\x43\x83\x45\x83\x47\x83\x49\x83\x4a\x83\x4c"
				"\x83\x4e\x83\x50\x83\x52\x83\x54\x83\x56\x83\x58\x83\x5a\x83\x5c"
				"\x83\x5e\x83\x60\x83\x63\x83\x65\x83\x67\x83\x69\x83\x6a\x83\x6b"
				"\x83\x6c\x83\x6d\x83\x6e\x83\x71\x83\x74\x83\x77\x83\x7a\x83\x7d"
				"\x83\x7e\x83\x80\x83\x81\x83\x82\x83\x84\x83\x86\x83\x88\x83\x89"
				"\x83\x8a\x83\x8b\x83\x8c\x83\x8d\x83\x8f\x83\x93\x81\x4a\x81\x4b"
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

	_free_locale( lt );

	// 2nd: Set to convert from SJIS to UNICODE.

	strIn = strOut;
	attr.m_iCharset = CP_SHIFT_JIS;

	return	strOut;
}

CStringA
CMainWnd::StringToUTF8( CStringW strIn, CAttr& attr )
{
	CStringA	strOut;

#ifndef	_UNICODE
	strIn = StringFromSJIS( (CStringA)strIn, attr );
#endif//_UNICODE
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
#ifdef	_UNICODE
	int	n;
	for	( int x = 0;; ){
		n = strLines.GetLength();
		if	( x > n-6 )
			break;
		x = strLines.Find( L"&#", x );
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
#endif//_UNICODE
}

void
CMainWnd::CheckUnicode( CString& strLog, CAttr &attr )
{
#ifdef	_UNICODE
	int	nch = strLog.GetLength();
	for	( int i = 0; i < nch; i++ ){

		bool	bTrace = false;//DBG
		int	iTrace = i;
		UINT	uAlt;
		DWORD	dwType = GetCharType( strLog, i, uAlt );

		// Do not accept evasive codes.

		if	( dwType == UC_CONTROLCODE ){
			strLog.Delete( i, 1 );
			strLog.Insert( i, UC_REPLACEMENT );
			FilterError( IDS_RF_CONTROLCODE, attr );
			bTrace = true;
		}

		// Do not accept the code cannot be displayed with the selected font.

		else if	( dwType == UC_OTHER ){
			if	( !( m_dwCode & (1<<(IDS_RF_UNMAPPEDCODE-IDS_RF_CHARSET)) ) )
				;
			else if	( !m_charmap.IsMapped( uAlt ) ){
				FilterError( IDS_RF_UNMAPPEDCODE, attr );
				bTrace = true;
			}
		}

		else if	( dwType == UC_EVASIVE ){
			bTrace = true;
		}
#ifdef	_DEBUG
		if	( bTrace ){
			int	xS = i;
			for	( ; xS >= 0; xS-- )
				if	( strLog[xS] == '\n' ){
					xS++;
					break;
				}
			int	xE = i;
			for	( ; strLog[xE]; xE++ )
				if	( strLog[xE] == '\r' )
					break;
			TCHAR*	pch =	( dwType == UC_CONTROLCODE )?	L"Control":
					( dwType == UC_EVASIVE )?	L"Evasive":
									L"Unmapped";
			UINT	uCode = strLog[iTrace];
			if	( uCode >= 0xd800 && uCode <= 0xdbff &&
				  strLog[iTrace+1] >= 0xdc00 && strLog[iTrace+1] <= 0xdfff ){
				uCode =  ( strLog[iTrace+0] & 0x3ff ) << 10;
				uCode |= ( strLog[iTrace+1] & 0x3ff );
				uCode += 0x10000;
			}
			TRACE( L"%s U+%04x as '%s'\n", pch, uCode, strLog.Mid( xS, xE-xS ) );
		}
#endif//_DEBUG
	}
#endif//_UNICODE
}

void
CMainWnd::CheckAlias( CString strLog, CAttr& attr )
{
	// Get nominal sender.

	int	x = strLog.Find( _T("From: ") );
	if	( x < 0 ){
		CString	strLower = strLog.MakeLower();
		x = strLower.Find( _T("from: ") );
	}

	do{
		if	( x < 0 )
			break;

		// Extract sender's alias.

		int	xIn  = x + 6;
		int	xOut = strLog.Find( ':', xIn );
		CString strAlias = strLog.Mid( xIn, xOut-xIn );

		xOut = strAlias.ReverseFind( '<' );
		if	( xOut < 0 )
			break;
		strAlias = strAlias.Left( xOut );
		NormalizeAlias( strAlias );

		// Extract sender's domain.

		CString	strDomain = (CString)attr.m_strFrom;
		x = strLog.Find( _T("Sender: ") );
		if	( x < 0 ){
			CString	strLower = strLog.MakeLower();
			x = strLower.Find( _T("sender: ") );
		}
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
		while	( !bHit && !bMet ){

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

	// Get the recipient.

	x = strLog.Find( _T("\nTo: ") );
	if	( x < 0 ){
		CString	strLower = strLog.MakeLower();
		x = strLower.Find( _T("\nto: ") );
	}

	do{
		if	( x < 0 )
			break;

		// Extract recipient's alias.

		int	xIn  = x + 5;
		int	xOut = strLog.Find( ':', xIn );
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

void
CMainWnd::CheckSubject( CString strLog, CAttr& attr )
{
	// Get the subject.

	int	x = strLog.Find( _T("Subject: ") );
	if	( x < 0 ){
		CString	strLower = strLog.MakeLower();
		x = strLower.Find( _T("subject: ") );
	}

	do{
		if	( x < 0 )
			break;

		// Extract the subject.

		int	xIn  = x + 9;
		int	xOut = strLog.Find( ':', xIn );
		CString strSubject = strLog.Mid( xIn, xOut-xIn );
		strSubject.MakeLower();

		// Extract sender's domain.

		CString	strDomain = (CString)attr.m_strFrom;
		x = strLog.Find( _T("Sender: ") );
		if	( x < 0 ){
			CString	strLower = strLog.MakeLower();
			x = strLower.Find( _T("sender: ") );
		}
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
			strWord.MakeLower();
			int	iWord = strSubject.Find( strWord );
			if	( iWord < 0 )
				continue;

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
			FilterError( IDS_RF_SUBJECT_FAKED, attr );			
	}while	( 0 );
}

void
CMainWnd::NormalizeAlias( CString& strAlias )
{
#ifdef	_UNICODE

	// Normarize all characters in Alias.

	int	n = strAlias.GetLength();
	for	( int x = 0; x < n; x++ ){
		UINT	uAlt;
		TCHAR	ch = strAlias[x];
		GetCharType( strAlias, x, uAlt );

		// Skip characters not for pattern matching.

		if	( !uAlt ){
			strAlias.Delete( x, 1 );
			n--;
			x--;
		}

		// Large codes to ASCII code.

		else if	( uAlt != ch ){
			strAlias.SetAt( x, (TCHAR)uAlt);
		}
	}

	// Remove padding spaces.

	strAlias.Replace( L" ",           L"" );
	strAlias.Replace( UC_REPLACEMENT, L"" );
	strAlias.Trim();
#endif//_UNICODE
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
CMainWnd::CheckLink( CString strLog, CAttr& attr )
{
#ifdef	_UNICODE
	strLog.Replace( L"\r\n", L"" );

	int	xLines;
	TCHAR*	apchScheme[2] = { L"http://", L"https://" };

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

			// Check if the path is faking a domain like
			// "amazon.com@phishing.com".

			do{
				if	( strPath.IsEmpty() )
					break;
				int	x = strPath.FindOneOf( L"?#<" );
				if	( x >= 0 )
					strPath = strPath.Left( x );
				x = strPath.ReverseFind( '.' );
				if	( x < 0 )
					break;
				CString	strTLD = strPath.Mid( x );
				strTLD += L"\n";
				strTLD.MakeLower();
				if	( m_strTLDCache.Find( strTLD ) < 0 )
					break;
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
				attr.m_strLinks += strLink + L"\n";

			// Check if the link is described in evasive codes.

			bool	bEvasive = false;
			int	nch = strLink.GetLength();
			for	( int i = 0; i < nch; i++ ){
				UINT	uAlt = 0;
				DWORD	dwType = GetCharType( strLink, i, uAlt );
				if	( dwType == UC_EVASIVE ||
					  dwType == UC_CONTROLCODE ){
					bEvasive = true;
					break;
				}
			}
			if	( bEvasive )
				FilterError( IDS_RF_LINK_EVASIVE, attr );

			// Take the top level domain.

			xLink = strURL.ReverseFind( '.' );
			if	( xLink < 0 )
				continue;

			CString	strTLD = strURL.Mid( xLink );
			int	x = strTLD.FindOneOf( L"?#=" );
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
				CString	strDrop = m_strDomains.Tokenize( L"\n", x );
				if	( strDrop.IsEmpty() )
					break;
				bool	bDrop = false;
				if	( strDrop[0] == '.' ){
					if	( strDrop == strTLD )
						bDrop = true;
				}
				else{
					if	( !CompareWild( strDrop, strURL ) )
						bDrop = true;
				}
				if	( bDrop ){
					FilterError( IDS_RF_LINK_DOMAIN, attr );
					if	( attr.m_strLinks.Find( strLink ) < 0 )
						attr.m_strLinks += strLink + L"\n";
					break;
				}
			}

			// Check if the URL faking a top level domain like
			// "amazon.com.phising.com".

			CString	strMid;
			CString	strMatch;
			bool	bTop = true;
			int	nHit = 0;
			for	( int i = 0; i >= 0; ){

				// Take one dotted word.

				CString	strDot = strURL.Tokenize( L".", i );
				if	( strDot.IsEmpty() )
					break;

				if	( bTop )
					bTop = false;
				else
					strDot.Insert( 0, L"." );

				// Check if the word is a TLD.

				for	( x = 0; x >= 0; ){
					CString	strDom = m_strTLDCache.Tokenize( L"\n", x );
					if	( strDom.IsEmpty() )
						break;
					else if	( strDom == strDot ){
						if	( strMid.IsEmpty() )
							strMatch = strDot;
						else{
						//	TRACE( L"%s followed by %s is not TLD.\n", strMatch, strMid );
							FilterError( IDS_RF_LINK_FAKED, attr );
						}
						break;
					}
				}

				// Hold the non-TLD token that follows TLD.

				if	( !strMatch.IsEmpty() && strDot != strMatch )
					strMid = strDot;
			}
		}
	}
#endif//_UNICODE
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
	strLink.Replace( _T("\r\n"), _T("") );

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

	UINT	uAlt;
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
			else if	( ch == 0x2571 )			// Box Drawings Light Diagonal Upper Right to Lower Left
				continue;
			else if	( GetCharType( strLog, x, uAlt ) == UC_EVASIVE )
				continue;
			else{
				if	( ch >= 0xd800 && ch <= 0xdbff )
					x--;
				break;
			}
		}
	}

	strLink = strLog.Mid( xLines, x-xLines );
	xLines = x;
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

int
CMainWnd::CompareWild( CString strWild, CString strName )
{
	for	( ;; ){
		int	x = strWild.FindOneOf( _T("*?") );

		// No wildcards left, do normal comparison.

		if	( x < 0 )
			return	strWild.Compare( strName );

		// Take a part to compare.

		CString	strWildPart = strWild.Left( x );
		CString	strNamePart = strName.Left( x );
		int	iPart = strWildPart.Compare( strNamePart );

		// Parts differ, return result.

		if	( iPart )
			return	iPart;

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

DWORD
CMainWnd::GetCharType( CString strUC, int& iRef, UINT& uAlt )
{
	DWORD	dwType = 0;
	TCHAR	ch = strUC[iRef];
	UINT	uch = uAlt = ch;

#ifdef	_UNICODE
	if	( ch == '\t' ){				// Tab code:
		dwType = UC_ASCII;			//   Keep it.
	}
	else if	( ch == '\n' ){				// Line-Feed code:
		dwType = UC_ASCII;			//   Keep it.
	}
	else if	( ch == '\r' ){				// Carridge-Return code:
		dwType = UC_ASCII;			//   Keep it.
	}
	else if	( ch < ' ' ){				// Other Control codes:
		uAlt = '\0';				//   are not for patern matching,
		dwType = UC_CONTROLCODE;		//   Detect them.
	}
	else if	( ch == ' ' ){				// Space code:
		dwType = UC_SPACE;			//   Keep it.
	}
	else if	( ch >= '0' && ch <= '9' ){		// Digit codes:
		dwType = UC_ASCII;			//   Keep them.
	}
	else if	( ch >= 'A' && ch <= 'Z' ){		// Upper cases:
		dwType = UC_ASCII;			//   Keep them.
	}
	else if	( ch >= 'a' && ch <= 'z' ){		// Lower cases:
		dwType = UC_ASCII;			//   Keep them.
	}
	else if	( ch >= '!' && ch <= '~' ){		// Other Visible ASCII codes:
		uAlt = '\0';				//   are not for patern matching,
		dwType = UC_ASCII;			//   Keep them.
	}

	else if	( ch >= 0x7f && ch <= 0x9f ){		// Control codes:
		uAlt = '\0';				//   are not for patern matching,
		dwType = UC_CONTROLCODE;		//   Detect them.
	}
	else if	( ch == 0xa0 ){				// No-break Space code:
		uAlt = '\0';				//   are not for patern matching,
		dwType = UC_SPACE;			//   Keep it.
	}						//   ( used by bma.mpse.jp to make an empty row of a table )
	else if	( ch == 0xad ){				// Soft Hyphen code:
		uAlt = '\0';				//   are not for patern matching,
		dwType = UC_SPACE;			//   Keep it.
	}

	else if	( ch == 0xb7 ){				// Middle Dot:
		uAlt = '\0';				//   is not for patern matching,
		dwType = UC_SPACE;			//   Keep it.
	}
	else if	( ch == 0x2d9 ){			// Dot Above:
		uAlt = '\0';				//   is not for patern matching,
		dwType = UC_SPACE;			//   Keep it.
	}
	else if	( ch >= 0x300 && ch <= 0x36f ){		// Combining Diacritical Marks:
		uAlt = '\0';				//   is not for patern matching,
		dwType = UC_SPACE;			//   Keep it.
	}

	else if	( ch == 0x119e ){			// Hangul Jungseong Araea:
		uAlt = '\0';				//   is not for patern matching,
		dwType = UC_SPACE;			//   Keep it.
	}

	else if	( ch >= 0x2000 && ch <= 0x200a ){	// Space codes:
		uAlt = ' ';				//   are ASCII space code for patern matching,
		dwType = UC_SPACE;			//   Keep them.
	}
	else if	( ch >= 0x200b && ch <= 0x200f ){	// Format codes:
		uAlt = '\0';				//   are not for patern matching,
		dwType = UC_CONTROLCODE;		//   Detect them.
	}						//   ( used by many phishers to evade pattern matching )
	else if	( ch >= 0x2010 && ch <= 0x2027 ){	// Punctuation codes:
		uAlt = ' ';				//   are ASCII space code for patern matching,
	}
	else if	( ch >= 0x2028 && ch <= 0x202e ){	// Format codes:
		uAlt = '\0';				//   are not for patern matching,
		dwType = UC_CONTROLCODE;		//   Detect them.
	}
	else if	( ch == 0x202f ){			// Narrow No-break Space code:
		uAlt = ' ';				//   is ASCII space code for patern matching,
		dwType = UC_SPACE;			//   Keep it.
	}
	else if	( ch == 0x2044 ){			// Fraction Slash ( Math ):
		uAlt = '/';				//   is ASCII slash code for patern matching,
		dwType = UC_EVASIVE;			//   Watch it.
	}
	else if	( ch == 0x205f ){			// Space code ( Math ):
		uAlt = ' ';				//   is ASCII space code for patern matching,
		dwType = UC_SPACE;			//   Keep it.
	}
	else if	( ch == 0x2060 ){			// Word Joiner code:
		uAlt = '\0';				//   is not for patern matching,
		dwType = UC_SPACE;			//   Keep it.
	}
	else if	( ch >= 0x2061 && ch <= 0x206f ){	// Format codes:
		uAlt = '\0';				//   are not for patern matching,
		dwType = UC_CONTROLCODE;		//   Detect them.
	}
	else if	( ch == 0x2215 ){			// Division Slash ( Math ):
		uAlt = '/';				//   is ASCII slash code for patern matching,
		dwType = UC_EVASIVE;			//   Watch it.
	}
	else if	( ch >= 0x2460 && ch <= 0x2468 ){	// Circled numbers 1 to 9:
		uAlt = ch - 0x2460 + '1';		//   are ASCII digit code for patern matching,
		dwType = UC_SYMBOL;			//   Ignore them.
	}
	else if	( ch >= 0x24b6 && ch <= 0x24cf ){	// Circled letters A to Z:
		uAlt = ch - 0x24b6 + 'A';		//   are ASCII letter code for patern matching,
		dwType = UC_EVASIVE;			//   Watch them.
	}
	else if	( ch >= 0x24d0 && ch <= 0x24e9 ){	// Circled letters a to z:
		uAlt = ch - 0x24d0 + 'a';		//   are ASCII letter code for patern matching,
		dwType = UC_EVASIVE;			//   Watch them.
	}
	else if	( ch == 0x24ea ){			// Circled number 0:
		uAlt = ch - 0x24ea + '0';		//   is ASCII digit code for patern matching,
		dwType = UC_SYMBOL;			//   Ignore it.
	}
	else if	( ch >= 0x2776 && ch <= 0x277e ){	// Black circled numbers 1 to 9:
		uAlt = ch - 0x2776 + '1';		//   are ASCII digit code for patern matching,
		dwType = UC_SYMBOL;			//   Ignore them.
	}
	else if	( ch >= 0x2780 && ch <= 0x2788 ){	// White circled numbers 1 to 9:
		uAlt = ch - 0x2780 + '1';		//   are ASCII digit code for patern matching,
		dwType = UC_SYMBOL;			//   Ignore them.
	}
	else if	( ch >= 0x278a && ch <= 0x2792 ){	// Black circled numbers 1 to 9:
		uAlt = ch - 0x278a + '1';		//   are ASCII digit code for patern matching,
		dwType = UC_SYMBOL;			//   Ignore them.
	}
	else if	( ch >= 0x2000 && ch <= 0x2e7f ){	// Symbol codes:
		uAlt = '\0';				//   are not for patern matching,
		dwType = UC_SYMBOL;			//   Ignore them.
	}

	else if	( ch == 0x30fb ){			// Katakana Middle Dot:
		uAlt = '\0';				//   is not for patern matching,
		dwType = UC_SPACE;			//   Keep it.
	}

	else if	( ch == 0x318d ){			// Hangul Letter Araea:
		uAlt = '\0';				//   is not for patern matching,
		dwType = UC_SPACE;			//   Keep it.
	}

	else if	( ch >= 0xd800 && ch <= 0xdbff ){	// Letters in a surrogate pair:
		uch = ( ch & 0x3ff ) << 10;
		ch = strUC[++iRef];
		if	( ch >= 0xdc00 && ch <= 0xdfff ){
			uch |= ( ch & 0x3ff );
			uch += 0x10000;

			if	( uch >= 0x1d400 && uch <= 0x1d6a3 ){	// Mathematical Alphanumeric Symbols A to z:
				UINT	nAlpha = 'Z' - 'A' +1;
				uch -= 0x1d400;
				uch %= (nAlpha*2);
				uAlt = ( uch <= nAlpha )?
					uch + 'A': uch - nAlpha + 'a';	//   are ASCII letter code for patern matching,
				dwType = UC_EVASIVE;			//   Watch them.
			}
			else if	( uch >= 0x1d7ce && uch <= 0x1d7ff ){	// Mathematical digits 0 to 9:
				uch -= 0x1d7ce;
				uch %= 10;
				uAlt = uch + '0';			//   are ASCII digit code for patern matching,
				dwType = UC_SYMBOL;			//   Ignore them.
			}

			else if	( uch >= 0x1f000 && uch <= 0x1f0ff ){	// Game Symbols:
				uAlt = '\0';				//   are not for patern matching,
				dwType = UC_SYMBOL;			//   Ignore them.
			}
			else if	( uch >= 0x1f10b && uch <= 0x1f10d ){	// Decorated letters 0:
				uAlt = '0';				//   are ASCII digit code for patern matching,
				dwType = UC_SYMBOL;			//   Ignore them.
			}
			else if	( uch >= 0x1f130 && uch <= 0x1f149 ){	// Decorated letters A to Z:
				uAlt = uch - 0x1f130 + 'A';		//   are ASCII letter code for patern matching,
				dwType = UC_EVASIVE;			//   Watch them.
			}
			else if	( uch >= 0x1f150 && uch <= 0x1f169 ){	// Decorated letters A to Z:
				uAlt = uch - 0x1f150 + 'A';		//   are ASCII letter code for patern matching,
				dwType = UC_EVASIVE;			//   Watch them.
			}
			else if	( uch >= 0x1f170 && uch <= 0x1f189 ){	// Decorated letters A to Z:
				uAlt = uch - 0x1f170 + 'A';		//   are ASCII letter code for patern matching,
				dwType = UC_EVASIVE;			//   Watch them.
			}
			else if	( uch >= 0x1f18a && uch <= 0x1f1ac ){	// Marks:
				uAlt = '\0';				//   are not for patern matching,
				dwType = UC_SYMBOL;			//   Ignore them.
			}
			else if	( uch >= 0x1f150 && uch <= 0x1f169 ){	// Decorated letters A to Z:
				uAlt = uch - 0x1f150 + 'A';		//   are ASCII letter code for patern matching,
				dwType = UC_EVASIVE;			//   Watch them.
			}
			else if	( uch >= 0x1f1e6 && uch <= 0x1f1ff ){	// Regional Indicators A to Z:
				uAlt = uch - 0x1f1e6 + 'A';		//   are ASCII letter code for patern matching,
				dwType = UC_EVASIVE;			//   Watch them.
			}
			else if	( uch >= 0x1f200 && uch <= 0x1ffff ){	// Symbols:
				uAlt = '\0';				//   are not for patern matching,
				dwType = UC_SYMBOL;			//   Ignore them.
			}
			else{						// Others:
				uAlt = uch;				//   are long codes.
			}
		}
	}

	else if	( ch >= 0xe000 && ch <= 0xf8ff ){	// Private Use Area:
		uAlt = '\0';				//   is not for patern matching,
		dwType = UC_CONTROLCODE;		//   Detect it.
	}

	else if	( ch >= 0xfe00 && ch <= 0xfe0f ){	// Variation Selector codes:
		uAlt = '\0';				//   is not for patern matching,
		dwType = UC_SYMBOL;			//   Ignore it.
	}
	else if	( ch == 0xfeff ){			// Zero-width No-break Space code:
		uAlt = '\0';				//   is not for patern matching,
		dwType = UC_CONTROLCODE;		//   Detect it.
	}
	else if	( ch >= 0xff01 && ch <= 0xff5e ){	// Large ASCII letters:
		uAlt = ch - 0xff01 + '!';		//   are ASCII letter code for patern matching,
	}

	else if	( ch == 0xff65 ){			// Halfwidth Katakana Middle Dot:
		uAlt = '\0';				//   is not for patern matching,
		dwType = UC_SPACE;			//   Keep it.
	}

	else if	( ch >= 0xfff9 && ch <= 0xfffd ){	// Special codes:
		uAlt = '\0';				//   is not for patern matching,
		dwType = UC_SYMBOL;			//   Ignore it.
	}
	else if	( ch >= 0xfffe && ch <= 0xffff ){	// Non-character codes:
		uAlt = '\0';				//   are not for patern matching,
		dwType = UC_CONTROLCODE;		//   Detect them.
	}

	else{						// Other codes:
		TCHAR	uch = ReplaceByASCII( ch );	//   Check similar letter.
		if	( uch != ch ){			//   Similar letter is replaced
			uAlt = uch;			//   with ASCII code.
			dwType = UC_EVASIVE;		//   Watch them.
		}
	}

	if	( !dwType ){				// Non of above type is assigned:
		dwType = UC_OTHER;			//   Keep them.
	}
#endif//_UNICODE

	return	dwType;
}

TCHAR
CMainWnd::ReplaceByASCII( TCHAR ch )
{
	// Codes of similar letters: Hover mouse pointer on the string and you'll see what I mean.

	static	TCHAR	*apch[] = {
		L"A\x0c0\x0c1\x0c2\x0c3\x0c4\x0c5\x100\x102\x104\x1cd\x1de\x1e0\x1fa\x200\x202\x226\x23a"
			L"\x1d00\x1d2c"
			L"\x1e00\x1ea0\x1ea2\x1ea4\x1ea6\x1ea8\x1eaa\x1eac\x1eae\x1eb0\x1eb2\x1eb4\x1eb6"
			L"\x1f08\x1f09\x1f0a\x1f0b\x1f0c\x1f0d\x1f0e\x1f0f\x1f88\x1f89\x1f8a\x1f8b\x1f8c\x1f8d\x1f8e\x1f8f\x1fb8\x1fb9\x1fbc"
			L"\x212b",
		L"B\x243"
			L"\x1d03\x1d2e\x1d2f"
			L"\x1e02\x1e04\x1e06",
		L"C\x0c7\x106\x108\x10a\x10c\x187\x23b"
			L"\x1d04"
			L"\x1e08",
		L"D\xd0\x10e\x110\x189\x18a"
			L"\x1d05\x1d06\x1d30"
			L"\x1e0a\x1e0c\x1e0e\x1e10\x1e12",
		L"E\x0c8\x0c9\x0ca\x0cb\x112\x114\x116\x118\x11a\x204\x206\x228\x246"
			L"\x1d07\x1d31"
			L"\x1e14\x1e16\x1e18\x1e1c\x1eb8\x1eba\x1ebc\x1ebe\x1ec0\x1ec2\x1ec4\x1ec6",
		L"F\x191"
			L"\x1e1e",
		L"G\x11c\x11e\x120\x122\x193\x1e4\x1e6\x1f4\x262"
			L"\x1d33",
		L"H\x124\x126\x21e\x16ba"
			L"\x1d34"
			L"\x1e22\x1e24\x1e26\x1e28\x1e2a",
		L"I\xcc\xcd\xce\xcf\x128\x12a\x12c\x12e\x130\x196\x1cf\x1d0\x208\x20a\x26a"
			L"\x1d35\x1da6\x1da7"
			L"\x1e2c\x1e2e\x1ec8\x1eca",
		L"J\x134\x248\x37f"
			L"\x1d0a\x1d36",
		L"K\x136\x138\x198\x1e8\x16d5"
			L"\x1d0b\x1d37"
			L"\x1e30\x1e32\x1e34",
		L"L\x139\x13b\x13d\x13f\x141\x23d"
			L"\x1d0c\x1d38\x1dab"
			L"\x1e36\x1e38\x1e3a\x1e3c",
		L"M\x16d6"
			L"\x1d0d\x1d39"
			L"\x1e3e\x1e40\x1e42",
		L"N\xd1\x143\x145\x147\x14a\x1f8\x274"
			L"\x1d3a\x1db0"
			L"\x1e44\x1e46\x1e48\x1e4a",
		L"O\x0d2\x0d3\x0d4\x0d5\x0d6\xd8\x14c\x14e\x150\x1a0\x1d1\x1ea\x1ec\x1fe\x20c\x20e\x22a\x22c\x22e\x230"
			L"\x1d0f\x1d3c"
			L"\x1e4c\x1e4e\x1e50\x1e52\x1ecc\x1ece\x1ed0\x1ed2\x1ed4\x1ed6\x1ed8\x1eda\x1edc\x1ede\x1ee0\x1ee2",
		L"P"
			L"\x1d18\x1d3e"
			L"\x1e54\x1e56",
		L"R\x154\x156\x158\x24c\x280"
			L"\x1d3f"
			L"\x1e58\x1e5a\x1e5c\x1e5e",
		L"S\x15a\x15c\x15e\x160\x218"
			L"\x1e60\x1e62\x1e64\x1e66\x1e68",
		L"T\x162\x164\x166\x1ac\x1ae\x21a"
			L"\x1d1b\x1d40"
			L"\x1e6a\x1e6c\x1e6e\x1e70",
		L"U\x0d9\x0da\x0db\x0dc\x168\x16a\x16c\x16e\x170\x172\x1d3\x1d5\x1d7\x1d9\x1db\x23e"
			L"\x1d1c\x1d41\x1db8"
			L"\x1e72\x1e74\x1e76\x1e78\x1e7a\x1ee4\x1ee6\x1ee8\x1eea\x1eec\x1eee\x1ef0",
		L"V"
			L"\x1d20\x1db9"
			L"\x1e7c\x1e7e",
		L"W\x174"
			L"\x1d21\x1d42"
			L"\x1e80\x1e82\x1e84\x1e86\x1e88",
		L"X"
			L"\x1e8a\x1e8c",
		L"Y\x0dd\x176\x178\x232\x24e\x28f"
			L"\x1e8e\x1ef2\x1ef4\x1ef6\x1ef8",
		L"Z\x179\x17b\x17d\x224"
			L"\x1d22"
			L"\x1e90\x1e92\x1e94",
		L"a\x0e0\x0e1\x0e2\x0e3\x0e4\x0e5\x101\x103\x105\x1ce\x1df\x1e1\x1fb\x201\x203\x251"
			L"\x1d43"
			L"\x1e01"
			L"\x1e9a\x1ea1\x1ea3\x1ea5\x1ea7\x1ea9\x1eab\x1ead\x1eaf\x1eb1\x1eb3\x1eb5\x1eb7"
			L"\x2090",
		L"b\x180\x184\x185\x253"
			L"\x1d47"
			L"\x1e03\x1e05\x1e07",
		L"c\x00e7\x0107\x109\x10b\x10d\x23c\x255"
			L"\x1e09",
		L"d\x10f\x111\x256\x257"
			L"\x1d48"
			L"\x1e0b\x1e0d\x1e0f\x1e11\x1e13",
		L"e\x0e8\x0e9\x0ea\x0eb\x113\x115\x117\x119\x11b\x205\x207\x229\x247"
			L"\x1d49"
			L"\x1e15\x1e17\x1e19\x1e1b\x1e1d\x1eb9\x1ebb\x1ebd\x1ebf\x1ec1\x1ec3\x1ec5\x1ec7"
			L"\x2091",
		L"f\x192"
			L"\x1da0"
			L"\x1e1f",
		L"g\x11d\x11f\x121\x123\x1e5\x1e7\x1f5\x260\x261"
			L"\x1d4d\x1da2"
			L"\x1e21",
		L"h\x125\x127\x21f\x266\x2b0\x2b1"
			L"\x1e96"
			L"\x1e23\x1e25\x1e27\x1e29\x1e2b"
			L"\x2095",
		L"i\x0ec\x0ed\x0ee\x0ef\x129\x12b\x12d\x12f\x131\x1d0\x209\x20b\x268"
			L"\x1d62\x1da4"
			L"\x1e2d\x1e2f\x1ec9\x1ecb",
		L"j\x135\x237\x249\x2b2",
		L"k\x137\x199\x1e9"
			L"\x1d4f"
			L"\x1e31\x1e33\x1e35"
			L"\x2096",
		L"l\x13a\x13c\x13e\x140\x142\x26b\x26c\x26d\x2e1"
			L"\x1da9\x1daa"
			L"\x2097",
		L"m\x271"
			L"\x1d50\x1dac"
			L"\x1e3f\x1e41\x1e43"
			L"\x2098",
		L"n\xf1\x144\x146\x148\x149\x14b\x1f9\x272\x273"
			L"\x1dae\x1daf"
			L"\x1e45\x1e47\x1e49\x1e4b"
			L"\x2099",
		L"o\x0f2\x0f3\x0f4\x0f5\x0f6\xf8\x14d\x14f\x151\x1a1\x1d2\x1eb\x1ed\x1ff\x20d\x20f\x22b\x22d\x22f\x231"
			L"\x1d52"
			L"\x1e4d\x1e4f\x1e51\x1e53\x1ecd\x1ecf\x1ed1\x1ed3\x1ed5\x1ed7\x1ed9\x1edb\x1edd\x1edf\x1ee1\x1ee3"
			L"\x2092",
		L"p"
			L"\x1d56"
			L"\x1e55\x1e57"
			L"\x209a",
		L"q\x24a\x24b\x2a0",
		L"r\x155\x157\x159\x211\x213\x27c\x27d\x27e\x2b3"
			L"\x1d63"
			L"\x1e59\x1e5b\x1e5d\x1e5f",
		L"s\x15b\x15d\x15f\x161\x219\x23f\x282\x2e2"
			L"\x1db3"
			L"\x1e61\x1e63\x1e65\x1e67\x1e69"
			L"\x209b",
		L"t\x163\x165\x167\x1ab\x1ad\x21b\x288"
			L"\x1d57"
			L"\x1e6b\x1e6d\x1e6f\x1e71\x1e97"
			L"\x209c",
		L"u\x0f9\x0fa\x0fb\x0fc\x169\x16b\x16d\x16f\x171\x1d4\x1d6\x1d8\x1da\x1dc\x215\x217\x289"
			L"\x1d58\x1d64"
			L"\x1e73\x1e75\x1e77\x1e79\x1e7b\x1ee5\x1ee7\x1ee9\x1eeb\x1eed\x1eef\x1ef1",
		L"v\x28b"
			L"\x1d5b\x1d65"
			L"\x1e7d\x1e7f",
		L"w\x175\x2b7"
			L"\x1e81\x1e83\x1e85\x1e87\x1e89\x1e98",
		L"x\x2e3"
			L"\x1e8b\x1e8d"
			L"\x2093",
		L"y\x0fd\x0ff\x177\x233\x24f\x2b8"
			L"\x1e8f\x1e99\x1ef3\x1ef5\x1ef7\x1ef9",
		L"z\x17a\x17c\x17e\x1b6"
			L"\x1dbb\x1dbc\x1dbd",
		NULL
	};
	static	TCHAR	*apchGreek[] = {	// el-GR
		L"A\x386\x391"
			L"\x1f08\x1f09\x1f0a\x1f0b\x1f0c\x1f0a\x1f0b\x1f0c\x1f0d\x1f0e\x1f0f"
			L"\x1f88\x1f89\x1f8a\x1f8b\x1f8c\x1f8d\x1f8e\x1f8f",
		L"B\x392",
		L"C\x3f9\x3fe",
		L"E\x395"
			L"\x1f18\x1f19\x1f1a\x1f1b\x1f1c\x1f1d"
			L"\x1fc8\x1fc9",
		L"F\x3dc",
		L"H\x389\x397"
			L"\x1f28\x1f29\x1f2a\x1f2b\x1f2c\x1f2d\x1f2e\x1f2f"
			L"\x1fca\x1fcb\x1fcc",
		L"I\x38a\x399\x3aa"
			L"\x1f38\x1f39\x1f3a\x1f3b\x1f3c\x1f3d\x1f3e\x1f3f"
			L"\x1fd8\x1fd9\x1fda\x1fdb",
		L"K\x39a\x3cf",
		L"M\x39c\x3fa\x3fb",
		L"N\x39d",
		L"O\x38c\x39f"		L"\x1f48\x1f49\x1f4a\x1f4b\x1f4c\x1f4d\x1ff8\x1ff9",
		L"P\x3a1"
			L"\x1d29\x1fec",
		L"Q\x3d8",
		L"T\x3a4",
		L"X\x3a7",
		L"Y\x38e\x3a5"	
			L"\x1f59\x1f5b\x1f5d\x1f5f\x1fe8\x1fe9\x1fea\x1feb",
		L"Z\x396",
		L"a\x3ac\x3b1"
			L"\x1f00\x1f01\x1f02\x1f03\x1f04\x1f05\x1f07"
			L"\x1f70\x1f71\x1f80\x1f81\x1f82\x1f83\x1f84\x1f85\x1f87",
		L"c\x3f2",
		L"i\x390\x3af\x3ca"
			L"\x1f30\x1f31\x1f32\x1f33\x1f34\x1f35\x1f36\x1f37\x1f76\x1f77"
			L"\x1fd0\x1fd1\x1fd2\x1fd3\x1fd6\x1fd7",
		L"j\x3f3",
		L"k\x3ba",
		L"n"
			L"\x1f20\x1f21\x1f22\x1f23\x1f24\x1f25\x1f26\x1f27\x1f74\x1f75"
			L"\x1f90\x1f91\x1f92\x1f93\x1f94\x1f95\x1f96\x1f97",
		L"o\x3bf\x3cc"		L"\x1f40\x1f41\x1f42\x1f43\x1f44\x1f45\x1f78\x1f79",
		L"p\x3c1"
			L"\x1fe4\x1fe5",
		L"s\x3c2",
		L"t\x3c4",
		L"u\x3c5\x3cb\x3cd"
			L"\x1f50\x1f51\x1f52\x1f53\x1f54\x1f55\x1f56\x1f57\x1f7a\x1f7b"
			L"\x1fe0\x1fe1\x1fe2\x1fe3\x1fe6\x1fe7",
		L"v\x3bd",
		L"x\x3c7",
		L"w\x3c9",
		NULL
	};
	static	TCHAR	*apchCyrillic[] = {	// ru-RU, uk-UA, be-BY, bg-BG, mk-MK, sr-Cyrl-BA, (sr-Cyrl-CS), sr-Cyrl-RS, mn-MN, kk-KZ, tg-Cyrl-TJ
		L"A\x410\x4d0\x4d2",
		L"B\x412\x432",
		L"C\x421\x4aa\x4ab",
		L"E\x400\x401\x415",
		L"F\x4fa\x4fb",
		L"G\x50c\x50d",
		L"H\x43d\x4a2\x4a3\x4a4\x4a5\x4c7\x4c9\x4ca\x528\x529",
		L"I\x406\x407\x4c0",
		L"J\x408",
		L"K\x40c\x43a\x45c\x49a\x49b\x49c\x49d\x49e\x4a0\x4a1\x51e\x51f",
		L"M\x41c\x43c",
		L"O\x41e",
		L"P\x420",
		L"Q\x51a",
		L"S\x405",
		L"T\x422\x4ac\x4ad",
		L"V\x474\x476",
		L"W\x51c\x51d",
		L"X\x425\x4fc\x4fd\x4fe\x4ff",
		L"Y\x4ae\x4af",
		L"a\x430\x4d1\x4d3",
		L"b\x44c",
		L"c\x441\x4ab",
		L"e\x435\x450\x451\x4bd\x4bf\x4d7",
		L"h\x526\x527",
		L"i\x456\x457",
		L"j\x458\x458",
		L"k\x49f\x4a1",
		L"l\x4cf",
		L"o\x43e\x4e7",
		L"p\x440",
		L"q\x51b",
		L"r\x433\x453",
		L"s\x455",
		L"v\x475\x477",
		L"w\x461",
		L"x\x445",
		L"y\x40e\x423\x443\x45e\x4ee\x4ef\x4f0\x4f1\x4f2\x4f3",
		NULL
	};
	static	TCHAR	*apchArmenian[] = {	// hy-AM
		L"L\x53c",
		L"O\x555",
		L"S\x54f",
		L"U\x531\x54d",
		L"g\x581",
		L"h\x56b\x570",
		L"j\x575",
		L"l\x56c",
		L"m\x560",
		L"n\x572\x578",
		L"o\x585",
		L"q\x566",
		L"t\x535\x565\x567",
		L"u\x57d",
		NULL
	};
	static	TCHAR	*apchGeorgian[] = {	// ka-GE
		L"G\x10ba",
		L"Q\x10ad\x10b3",
		L"S\x10bd\x10fd",
		L"U\x10ae",
		L"Y\x10c4",
		L"y\x10b8",
		NULL
	};
	static	TCHAR	*apchCherokee[] = {	// chr-CHER-US
		L"A\x13aa\xab7a",
		L"B\x13f4\x13fc",
		L"C\x13df\x13e3\xabaf\xabb3",
		L"D\x13a0\xab70",
		L"E\x13ac\xab7c",
		L"G\x13c0\x13fd\xab86\xab90",
		L"H\x13bb\xab8b",
		L"I\x13c6\xab96",
		L"J\x13ab\xab7b",
		L"K\x13e6\xabb6",
		L"L\x13de\xabae",
		L"M\x13b7\xab87",
		L"P\x13e2\xabb2",
		L"R\x13a1\x13d2\xab71\xaba2",
		L"S\x13da\xabaa",
		L"T\x13a2\xab72",
		L"V\x13d9\x13e4\xaba9\xabb4",
		L"W\x13b3\x13d4\xab83\xaba4",
		L"Z\x13c3\xab93",
		L"b\x13cf\xab9f",
		L"d\x13e7\xabb7",
		L"h\x13c2\xab92",
		L"i\x13a5\xab75",
		L"y\x13a9\xab79",
		NULL
	};
	static	TCHAR	*apchInuktitut[] = {	// iu-Latn-CA
		L"B\x15f7\x15f8\x15f9\x15fd\x15fe\x15ff",
		L"D\x15de\x15df\x15e0\x15ea\x15eb\x15ec",
		L"J\x148d\x148e\x148f\x1498\x1499\x149a\x149b",
		L"U\x1457\x1458\x1467",
		L"b\x1472\x1473\x147e\x147f\x1480\x1481\x1482\x1488",
		L"d\x146f\x1470\x1471\x147a\x147b\x147c\x147d\x1487",
		L"p\x1476\x1477\x1478\x1479\x1486",
		L"q\x1474\x1475\x1485",
		NULL
	};
	static	TCHAR	*apchLisu[] = {		// (Lisu)
		L"A\xa4ee",
		L"B\xa4d0",
		L"C\xa4da",
		L"D\xa4d3",
		L"E\xa4f0",
		L"F\xa4dd",
		L"G\xa4d6",
		L"H\xa4e7",
		L"I\xa4f2",
		L"J\xa4d9",
		L"K\xa4d7",
		L"L\xa4e1",
		L"M\xa4df",
		L"N\xa4e0",
		L"O\xa4f3",
		L"P\xa4d1",
		L"R\xa4e3",
		L"S\xa4e2",
		L"T\xa4d4",
		L"U\xa4f4",
		L"V\xa4e6",
		L"W\xa4ea",
		L"X\xa4eb",
		L"Y\xa4ec",
		L"Z\xa4dc",
		L"d\xa4d2",
		NULL
	};

	TCHAR	tch;
	tch = SeekASCII( apch, ch );
	if	( tch != ch )
		return	tch;

	if	( m_strLanguage != L"el-GR" ){
		tch = SeekASCII( apchGreek, ch );
		if	( tch != ch )
			return	tch;
	}
	if	( m_strLanguage != L"be-BY" &&
		  m_strLanguage != L"bg-BG" &&
		  m_strLanguage != L"kk-KZ" &&
		  m_strLanguage != L"mk-MK" &&
		  m_strLanguage != L"mn-MN" &&
		  m_strLanguage != L"ru-RU" &&
		  m_strLanguage != L"sr-Cyrl-BA" &&
		  m_strLanguage != L"sr-Cyrl-CS" &&
		  m_strLanguage != L"sr-Cyrl-RS" &&
		  m_strLanguage != L"tg-Cyrl-TJ" &&
		  m_strLanguage != L"uk-UA" ){
		tch = SeekASCII( apchCyrillic, ch );
		if	( tch != ch )
			return	tch;
	}
	if	( m_strLanguage != L"hy-AM" ){
		tch = SeekASCII( apchArmenian, ch );
		if	( tch != ch )
			return	tch;
	}
	if	( m_strLanguage != L"cka-GE" ){
		tch = SeekASCII( apchGeorgian, ch );
		if	( tch != ch )
			return	tch;
	}
	if	( m_strLanguage != L"chr-CHER-US" ){
		tch = SeekASCII( apchCherokee, ch );
		if	( tch != ch )
			return	tch;
	}
	if	( m_strLanguage != L"iu-Latn-CA" ){
		tch = SeekASCII( apchInuktitut, ch );
		if	( tch != ch )
			return	tch;
	}
	{
		tch = SeekASCII( apchLisu, ch );
		if	( tch != ch )
			return	tch;
	}

	return	ch;
}

TCHAR
CMainWnd::SeekASCII( TCHAR** ppchTable, TCHAR ch )
{
	for	( int y = 0; ppchTable[y]; y++ )
		for	( int x = 1; ppchTable[y][x]; x++ )
			if	( ch == ppchTable[y][x] )
				return	ppchTable[y][0];
	return	ch;
}

void
CMainWnd::FilterError( UINT uIdError, CAttr& attr )
{
	if	( uIdError == IDS_RF_AUTH ){
		if	( attr.m_nAuth < m_nAuth )
			uIdError = 0;
	}
	else if	( uIdError >= IDS_RF_CHARSET && uIdError <= IDS_RF_UNMAPPEDCODE ){
		int	i = uIdError-IDS_RF_CHARSET;
		if	( !( m_dwCode & (1<<i) ) )
			uIdError = 0;
	}
	else if	( uIdError == IDS_RF_DOMAIN ){
	}
	else if	( uIdError == IDS_RF_FAKE_ALIAS ){
	}
	else if	( uIdError >= IDS_RF_MESSAGEID && uIdError <= IDS_RF_SUBJECT_FAKED ){
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

#ifdef	_UNICODE
#undef	atoi
#endif//_UNICODE

#define	DBGOUTPUT

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
				GetDate( pch, attr );
				CheckReceived( pch, attr );
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
