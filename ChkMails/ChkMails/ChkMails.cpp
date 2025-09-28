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

void
CChkMailsApp::WriteProfileCode( LPCTSTR lpszSection, LPCTSTR lpszEntry, LPCTSTR lpszCode )
{
	BYTE*	pbRecord = NULL;
	DWORD	cbRecord = 0;

	CStringA strCode = (CStringA)lpszCode;
	BYTE*	pszRecord = (BYTE*)strCode.GetBuffer();
	DWORD	dwError = EnDecrypt( pszRecord, pbRecord, cbRecord, true );

	WriteProfileBinary( lpszSection, lpszEntry, pbRecord, (UINT)cbRecord );
	delete[] pbRecord;
}

CString
CChkMailsApp::GetProfileCode( LPCTSTR lpszSection, LPCTSTR lpszEntry, LPCTSTR lpszDefault )
{
	BYTE*	pbRecord = NULL;
	DWORD	cbRecord = 0;

	GetProfileBinary( lpszSection, lpszEntry, &pbRecord, (UINT*)&cbRecord );
	if	( !pbRecord )
		return	CString( lpszDefault );

	BYTE*	pszRecord = NULL;
	DWORD	dwError = EnDecrypt( pszRecord, pbRecord, cbRecord, false );

	CString	strRecord = CString( (char*)pszRecord );
	delete[] pszRecord;
	delete[] pbRecord;
	return	strRecord;
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
			Sleep( 100 );	// Wait the preceding process to close.
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

	// Make a blob for codes.

	MakeBlob();

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

		delete[] m_pbBlob;

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

///////////////////////////////////////////////////////////////////////////////////////
// Specific Functions

#include <bcrypt.h>
#pragma comment( lib, "Bcrypt.lib" )

void
CChkMailsApp::MakeBlob( void )
{
	m_pbBlob = NULL;
	m_cbBlob = 0;
	GetProfileBinary( _T("Data"), _T("Blob"), &m_pbBlob, &m_cbBlob );
	if	( !m_pbBlob ){
		m_cbBlob = 192;
		m_pbBlob = new BYTE[m_cbBlob];
		(void)BCryptGenRandom( NULL, m_pbBlob, m_cbBlob, BCRYPT_USE_SYSTEM_PREFERRED_RNG );
		WriteProfileBinary( _T("Data"), _T("Blob"), m_pbBlob, m_cbBlob );
	}
}

#define	OFS_IV	 64
#define	OFS_SK	128

DWORD
CChkMailsApp::EnDecrypt( BYTE*& lpszPlain, BYTE*& pbCoded, DWORD& cbCoded, bool bEncode )
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

			DWORD	cbPlain = (DWORD)strlen( (char*)lpszPlain );
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
			lpszPlain = new BYTE[cbPlain+1];
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
