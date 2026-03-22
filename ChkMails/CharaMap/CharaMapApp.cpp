// CharaMapApp.cpp : Defines the class behaviors for the application.
//

#include "pch.h"
#include "CharaMapApp.h"
#include "CharMap.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CCharaMapApp theApp;

BOOL
CCharaMapApp::InitInstance( void )
{
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof( InitCtrls );
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx( &InitCtrls );

	CWinApp::InitInstance();

	CCharMapDlg dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();

	return FALSE;
}

