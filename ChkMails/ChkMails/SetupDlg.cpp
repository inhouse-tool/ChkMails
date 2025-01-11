// AccountDlg.cpp : implementation file
//

#include "pch.h"
#include "ChkMails.h"
#include "SetupDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#ifdef	UNICODE
#define	strcpy_s	wcscpy_s
#define	atoi		_wtoi
#endif//UNICODE

IMPLEMENT_DYNAMIC( CSetupDlg, CDialog )

///////////////////////////////////////////////////////////////////////////////////////
// Constructor

CSetupDlg::CSetupDlg( CWnd* pParent )
	: CDialog( IDD_SETUP, pParent )
{
	m_nPoll    = 0;
	m_bSummary = false;
	m_bTone    = false;
	m_nLog     = 0;
	m_bLogAll  = false;
}

///////////////////////////////////////////////////////////////////////////////////////
// Overridden Functions

BOOL
CSetupDlg::OnInitDialog( void )
{
	// Do the usual initializations.

	CDialog::OnInitDialog();

	HICON	hIcon = AfxGetApp()->LoadIcon( IDR_MAINFRAME );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	GetWindowRect( &m_rectOriginal );

	// Get a fixed-pitch font for the editboxes.

	CFont*	pFont = GetCommonFont();
	
	// Set the fixed-pitch font to the editboxes.

	GetDlgItem( IDC_EDIT_LOGPATH  )->SetFont( pFont, FALSE );
	GetDlgItem( IDC_EDIT_TONEPATH )->SetFont( pFont, FALSE );

	// Initialize controls.

	CString	str;
	str.Format( _T("%d"), m_nPoll );
	GetDlgItem( IDC_EDIT_POLL     )->SetWindowText( str );
	((CButton*)GetDlgItem( IDC_CHECK_SUMMARY ))->SetCheck( m_bSummary? BST_CHECKED: BST_UNCHECKED );
	((CButton*)GetDlgItem( IDC_CHECK_TONE )   )->SetCheck( m_bTone?    BST_CHECKED: BST_UNCHECKED );
	GetDlgItem( IDC_EDIT_TONEPATH )->SetWindowText( m_strToneFile );
	GetDlgItem( IDC_EDIT_LOGPATH  )->SetWindowText( m_strLogPath );
	str.Format( _T("%d"), m_nLog );
	GetDlgItem( IDC_EDIT_LOGS     )->SetWindowText( str );
	((CComboBox*)GetDlgItem( IDC_COMBO_LOG ))->SetCurSel( m_bLogAll? 1: 0 );

	((CButton*)GetDlgItem( IDC_CHECK_STARTUP ))->SetCheck( IsInStartup()? BST_CHECKED: BST_UNCHECKED );

	// Ask main window to set position of this window.

	NotifyPopup( m_hWnd );

	GotoDlgCtrl( GetDlgItem( IDOK ) );

	return	FALSE;
}

BOOL
CSetupDlg::DestroyWindow( void )
{
	NotifyPopup( NULL );

	return	CDialog::DestroyWindow();
}

void
CSetupDlg::OnOK( void )
{
	CString	str;

	GetDlgItem( IDC_EDIT_POLL    )->GetWindowText( str );
	m_nPoll = atoi( str.GetBuffer() );
	if	( m_nPoll <= 0 )
		m_nPoll = 1;

	m_bSummary = ((CButton*)GetDlgItem( IDC_CHECK_SUMMARY ))->GetCheck() == BST_CHECKED;
	m_bTone    = ((CButton*)GetDlgItem( IDC_CHECK_TONE )   )->GetCheck() == BST_CHECKED;
	GetDlgItem( IDC_EDIT_TONEPATH )->GetWindowText( m_strToneFile );

	GetDlgItem( IDC_EDIT_LOGPATH )->GetWindowText( m_strLogPath );
	GetDlgItem( IDC_EDIT_LOGS    )->GetWindowText( str );
	m_nLog = atoi( str.GetBuffer() );
	m_bLogAll = ((CComboBox*)GetDlgItem( IDC_COMBO_LOG ))->GetCurSel() == 1;

	bool	bPut = ((CButton*)GetDlgItem( IDC_CHECK_STARTUP ))->GetCheck() == BST_CHECKED;
	PutInStartup( bPut );

	CDialog::OnOK();
}

///////////////////////////////////////////////////////////////////////////////////////
// Message Handlers

BEGIN_MESSAGE_MAP( CSetupDlg, CDialog )
	ON_WM_NCACTIVATE()
	ON_WM_GETMINMAXINFO()
	ON_BN_CLICKED( IDHELP, OnHelp )
	ON_BN_CLICKED( IDC_BUTTON_TONEPATH, OnButtonRefTone )
	ON_BN_CLICKED( IDC_BUTTON_LOGPATH, OnButtonRefLog )
	ON_CONTROL_RANGE( EN_CHANGE, IDC_EDIT_POLL, IDC_EDIT_LOGS, OnChangeEdit )
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()

BOOL
CSetupDlg::OnNcActivate( BOOL bActive )
{
	if	( !m_strModal.IsEmpty() ){
		CWnd*	pDlgModal = FindWindow( _T("#32770"), m_strModal );
		if	( pDlgModal && bActive ){
			pDlgModal->FlashWindowEx( FLASHW_ALL, 3, 50 );
			pDlgModal->SetForegroundWindow();
			return	TRUE;
		}
	}

	return	CDialog::OnNcActivate( bActive );
}

void
CSetupDlg::OnGetMinMaxInfo( MINMAXINFO* lpMMI )
{
	// Answer minimum size.

	lpMMI->ptMinTrackSize.x = m_rectOriginal.Width();
	lpMMI->ptMinTrackSize.y = m_rectOriginal.Height();
}

void
CSetupDlg::OnHelp( void )
{
	ShowHelp( m_hWnd );
}

#include <mmsystem.h>			// for PlaySound
#pragma comment( lib, "winmm.lib" )

void
CSetupDlg::OnButtonRefTone( void )
{
	CString	strFile;
	CString	strPath = m_strToneFile;
	int	x = strPath.ReverseFind( '\\' );
	if	( x >= 0 ){
		strPath = strPath.Left( x );
		strFile = m_strToneFile.Mid( x+1 );
	}

	TCHAR	achFile[_MAX_PATH] = {};
	DWORD	dwFlags = OFN_NOCHANGEDIR;

	CFileDialog	dlg( TRUE );
	dlg.m_ofn.lpstrFilter     = _T("Wave File\0*.wav\0\0");
	dlg.m_ofn.lpstrFile       = achFile;
	dlg.m_ofn.lpstrInitialDir = strPath;
	dlg.m_ofn.lpstrTitle      = _T("Select a wave file");
	strcpy_s( achFile, (LPCTSTR)strFile );

	m_strModal = dlg.m_ofn.lpstrTitle;
	INT_PTR	iAnswer = dlg.DoModal();
	m_strModal.Empty();
	if	( iAnswer != IDOK )
		return;

	m_strToneFile = dlg.m_ofn.lpstrFile;
	GetDlgItem( IDC_EDIT_TONEPATH )->SetWindowText( m_strToneFile );
	PlaySound( m_strToneFile, NULL, SND_FILENAME | SND_ASYNC );
}

void
CSetupDlg::OnButtonRefLog( void )
{
	CFolderPickerDialog	dlg;

	dlg.m_ofn.lpstrInitialDir = m_strLogPath.GetBuffer();
	dlg.m_ofn.lpstrTitle      = _T("Select a log folder");
	m_strModal = dlg.m_ofn.lpstrTitle;

	INT_PTR	iAnswer = dlg.DoModal();
	m_strModal.Empty();
	if	( iAnswer != IDOK )
		return;

	m_strLogPath = dlg.GetPathName() + _T("\\");
	GetDlgItem( IDC_EDIT_LOGPATH )->SetWindowText( m_strLogPath );
}

void
CSetupDlg::OnChangeEdit( UINT uID )
{
	CString	strPoll, strPath, strLogs;
	GetDlgItem( IDC_EDIT_POLL    )->GetWindowText( strPoll );
	GetDlgItem( IDC_EDIT_LOGPATH )->GetWindowText( strPath );
	GetDlgItem( IDC_EDIT_LOGS    )->GetWindowText( strLogs );
	BOOL	bEnable = TRUE;
	if	( strPoll.IsEmpty() || strPath.IsEmpty() || strLogs.IsEmpty() )
		bEnable = FALSE;
	GetDlgItem( IDOK )->EnableWindow( bEnable );
}

///////////////////////////////////////////////////////////////////////////////////////
// Specific Functions

bool
CSetupDlg::IsInStartup( void )
{
	CString	strLink = GetLinkPath( FOLDERID_Startup, NULL );

	return	IsExisting( strLink );
}

void
CSetupDlg::PutInStartup( bool bPut )
{
	CString	strLink = GetLinkPath( FOLDERID_Startup, NULL );
	bool	bIn = IsExisting( strLink );
	if	(  bIn &&  bPut )
		return;
	else if	( !bIn && !bPut )
		return;

	if	( bPut ){
		CString	strExe = AfxGetApp()->m_pszHelpFilePath;
		int	x = strExe.ReverseFind( '.' );
		strExe = strExe.Left( x ) + _T(".exe");
		CreateLink( strLink, strExe );
	}
	else{
		DeleteFile( strLink );
	}
}

CString
CSetupDlg::GetLinkPath( GUID guidFolder, LPCTSTR pszSubFolder )
{
	WCHAR*	pszFolder = NULL;
	SHGetKnownFolderPath( guidFolder, 0, NULL, &pszFolder );
	CString	strPath = pszFolder;

	strPath += "\\";
	if	( guidFolder == FOLDERID_Programs ){
		if	( pszSubFolder ){
			strPath += pszSubFolder;
			strPath += "\\";
		}
	}

	CString	strName = AfxGetApp()->m_pszAppName;
	strName += _T(".lnk");

	return	strPath + strName;
}

void
CSetupDlg::CreateLink( CString strLink, CString strExe )
{
	CChkMailsApp*	pApp = (CChkMailsApp*)AfxGetApp();
	CString	strDescription = pApp->GetVersionInfo( _T("D"), strExe );

	(void)CoInitialize( NULL );

	IShellLink*	pLink;
	(void)CoCreateInstance( CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (LPVOID*)&pLink );

	SHFILEINFO	sfi = { 0 };
	SHGetFileInfo( strExe, 0, &sfi, sizeof( sfi ), SHGFI_ICONLOCATION );
	CString	strIcon = strExe;
	int	iIcon = 0;
	if	( *sfi.szDisplayName ){
		strIcon = sfi.szDisplayName;
		iIcon   = sfi.iIcon;
	}
	pLink->SetIconLocation( strIcon, iIcon );
	pLink->SetDescription( strDescription );
	pLink->SetPath( strExe );
	{
		int	x = strExe.ReverseFind( '\\' );
		CString	strDir = strExe.Left( x );
		pLink->SetWorkingDirectory( strDir );
	}

	IPersistFile* pFile;
	HRESULT	hRes = pLink->QueryInterface( IID_IPersistFile, (LPVOID*)&pFile );
	if	( SUCCEEDED( hRes ) ){		
		BSTR	wstrLnk = strLink.AllocSysString();
		hRes = pFile->Save( wstrLnk, TRUE );
		pFile->Release();
	}

	pLink->Release();

	CoUninitialize();
}

bool
CSetupDlg::IsExisting( CString strFile )
{
	CFileStatus	fs;
	if	( CFile::GetStatus( strFile, fs ) )
		return	true;
	else
		return	false;
}

