// ChkMails.h : main header file for the ChkMails application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"

#define	WM_USER_TRAY_NOTIFY	(WM_APP+1)
#define	WM_POPUP_NOTIFY		(WM_APP+2)
#define	WM_GET_COMMONFONT	(WM_APP+3)
#define	WM_GET_SENDER		(WM_APP+4)
#define	WM_SHOW_HELP		(WM_APP+5)

#define	NotifyPopup( hWnd )		AfxGetMainWnd()->PostMessage( WM_POPUP_NOTIFY, 0, (LPARAM)(hWnd) )
#define	GetCommonFont()			(CFont*)AfxGetMainWnd()->SendMessage( WM_GET_COMMONFONT, 0, 0 )
#define	ShowHelp( hWnd )		AfxGetMainWnd()->PostMessage( WM_SHOW_HELP, 0, (LPARAM)(hWnd) )

class CChkMailsApp : public CWinApp
{
public:
		BOOL	DeleteProfileValue( CString strSection, CString strName );
		CString	GetVersionInfo( TCHAR* pchQuery, LPCTSTR pszExe = NULL );
protected:
	virtual	BOOL	InitInstance( void );
	virtual	BOOL	PreTranslateMessage( MSG* pMsg );
	virtual	int	DoMessageBox( LPCTSTR lpszPrompt, UINT nType, UINT nIDPrompt );
};

extern	CChkMailsApp	theApp;
