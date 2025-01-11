// ChkMails.cpp : Defines the class behaviors for the application.
//

#include "pch.h"
#include "ChkMails.h"
#include "MainWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CChkMailsApp	theApp;

///////////////////////////////////////////////////////////////////////////////////////
// Public Functions

BOOL
CChkMailsApp::DeleteProfileValue( CString strSection, CString strName )
{
	CString	strKey;
	strKey.Format( _T("Software\\%s\\%s\\%s"), m_pszRegistryKey, m_pszAppName, strSection.GetBuffer() );

	HKEY	hKey = NULL;
	DWORD	dwResult = RegOpenKeyEx( HKEY_CURRENT_USER, strKey, 0, KEY_WRITE, &hKey );

	if	( dwResult == ERROR_SUCCESS ){
		dwResult = RegDeleteValue( hKey, strName );
		RegCloseKey( hKey );
	}

	return	( dwResult == ERROR_SUCCESS )? TRUE: FALSE;
}

#pragma comment( lib, "version.lib" )

CString
CChkMailsApp::GetVersionInfo( TCHAR* pchQuery, LPCTSTR pszExe )
{
	CString	strAnswer;

	// Get fullpath of the executable.
	
	CString	strExe;
	if	( !pszExe || !*pszExe ){
		strExe = AfxGetApp()->m_pszHelpFilePath;
		int	x = strExe.ReverseFind( '.' );
		strExe = strExe.Left( x ) + _T(".exe");
	}
	else
		strExe = pszExe;

	// Get version information of the executable.

	DWORD	dwLen = GetFileVersionInfoSize( strExe, NULL );

	if	( dwLen ){
		char*	pchVerInfo = new char[dwLen];

		GetFileVersionInfo( strExe, 0, dwLen, (LPVOID)pchVerInfo );

		struct LANGANDCODEPAGE{
			WORD	wLanguage;
			WORD	wCodePage;
		}	*lpTranslate;
		UINT	uLen;

		VerQueryValue( pchVerInfo, _T("\\VarFileInfo\\Translation"), (LPVOID*)&lpTranslate, &uLen );

		if	( uLen ){
			for	( ; *pchQuery; pchQuery++ ){
				if	( *pchQuery != '%' ){
					strAnswer += *pchQuery;
					continue;
				}
				if	( !*++pchQuery )
					break;

				TCHAR*	pch;
				CString	str;

				if	( *pchQuery == 'C' ){
					str.Format( _T("\\StringFileInfo\\%04x%04x\\CompanyName"),
							lpTranslate->wLanguage, lpTranslate->wCodePage );
					VerQueryValue( pchVerInfo, str, (LPVOID*)&pch, &uLen );
					if	( uLen )
						strAnswer += pch;
				}
				else if	( *pchQuery == 'D' ){
					str.Format( _T("\\StringFileInfo\\%04x%04x\\FileDescription"),
							lpTranslate->wLanguage, lpTranslate->wCodePage );
					VerQueryValue( pchVerInfo, str, (LPVOID*)&pch, &uLen );
					if	( uLen )
						strAnswer += pch;
				}

				else if	( *pchQuery == 'V' ){
					str.Format( _T("\\StringFileInfo\\%04x%04x\\FileVersion"),
							lpTranslate->wLanguage, lpTranslate->wCodePage );
					VerQueryValue( pchVerInfo, str, (LPVOID*)&pch, &uLen );
					if	( uLen )
						strAnswer += pch;
				}

				else if	( *pchQuery == 'L' ){
					str.Format( _T("\\StringFileInfo\\%04x%04x\\LegalCopyright"),
							lpTranslate->wLanguage, lpTranslate->wCodePage );
					VerQueryValue( pchVerInfo, str, (LPVOID*)&pch, &uLen );
					if	( uLen )
						strAnswer += pch;
				}

				else if	( *pchQuery == 'P' ){
					str.Format( _T("\\StringFileInfo\\%04x%04x\\ProductName"),
							lpTranslate->wLanguage, lpTranslate->wCodePage );
					VerQueryValue( pchVerInfo, str, (LPVOID*)&pch, &uLen );
					if	( uLen )
						strAnswer += pch;
				}

				else{
					TRACE( _T("GetVersionInfo: Unknown query '%c'\n"), *pchQuery );
				}
			}
		}

		delete[]	pchVerInfo;
	}

	return	strAnswer;
}

///////////////////////////////////////////////////////////////////////////////////////
// Overridden Functions

BOOL
CChkMailsApp::InitInstance( void )
{
	// Do the standard initializations.

	INITCOMMONCONTROLSEX InitCtrls = {};
	InitCtrls.dwSize = sizeof( InitCtrls );
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx( &InitCtrls );

	CWinApp::InitInstance();

	// Do the custom initializations.

	SetRegistryKey( _T("In-house Tool") );

	AfxSocketInit();

	// If another process is running, try to close it.

	CString	strClass = m_pszAppName;
	CString	strTitle = strClass + _T(" Window" );

	for	( int i = 0; i < 10; i++ ){
		HWND	hWnd = FindWindow( strClass, strTitle );
		if	( hWnd ){
			PostMessage( hWnd, WM_CLOSE, 0, 0 );
			Sleep( 100 );
		}
		else
			break;
	}

	// Register a unique class for this program.

	WNDCLASS wc = { 0 };
	wc.lpfnWndProc   = AfxWndProc;
	wc.hInstance     = AfxGetInstanceHandle();
	wc.lpszClassName = strClass;
	AfxRegisterClass( &wc );

	// Create the main window.

	CMainWnd* pWnd = new CMainWnd;
	m_pMainWnd = (CWnd*)pWnd;

	pWnd->Create( strClass, strTitle, 0x0, CRect( 0, 0, 0, 0 ), CWnd::GetDesktopWindow(), 0 );

	return	TRUE;
}

BOOL
CChkMailsApp::PreTranslateMessage( MSG* pMsg )
{
	if	( pMsg->message == WM_CLOSE ){
		CWnd* pWnd = m_pMainWnd;
		if	( pWnd ){
			pWnd->DestroyWindow();
			delete	pWnd;
		}
		return	TRUE;
	}
	else if	( ( pMsg->message >= WM_KEYFIRST ) &&
		  ( pMsg->message <= WM_KEYLAST  )    ){
		CWnd* pWnd = CWnd::GetFocus();
		if	( pWnd ){
			TCHAR	achClass[256];
			GetClassName( pWnd->m_hWnd, achClass, _countof( achClass ) );
			CString	strClass = achClass;
			if	( strClass == _T("Edit") )
				if	( pMsg->wParam != VK_RETURN )
					return	pWnd->PreTranslateMessage( pMsg );
		}
	}

	return	CWinApp::PreTranslateMessage( pMsg );
}

#include "MessageBox.h"

int
CChkMailsApp::DoMessageBox( LPCTSTR lpszPrompt, UINT nType, UINT nIDPrompt )
{
	CMessageBox	dlg;
	dlg.CreateModal( lpszPrompt, nType );
	dlg.SetOwner( AfxGetMainWnd() );

	return	(int)dlg.DoModal();
}
