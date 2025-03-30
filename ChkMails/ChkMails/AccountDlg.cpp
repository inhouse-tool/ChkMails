// AccountDlg.cpp : implementation file
//

#include "pch.h"
#include "ChkMails.h"
#include "MainWnd.h"
#include "AccountDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#ifdef	UNICODE
#define	strcpy_s	wcscpy_s
#define	HIDE_CHAR	L'\x25cf'
#else
#define	HIDE_CHAR	'*'
#endif//UNICODE

#define	TID_SELCHANGE	1	// to get the last result of the selection

#define	PORT_POP	110
#define	PORT_POPS	995

IMPLEMENT_DYNAMIC( CAccountDlg, CDialog )

///////////////////////////////////////////////////////////////////////////////////////
// Constructor

CAccountDlg::CAccountDlg( CWnd* pParent )
	: CDialog( IDD_ACCOUNTS, pParent )
{
}

///////////////////////////////////////////////////////////////////////////////////////
// Public Methods

void
CAccountDlg::SetAccounts( CArray <CAccount, CAccount&>& aAccount )
{
	m_aAccount.RemoveAll();
	m_aAccount.Append( aAccount );
}

void
CAccountDlg::GetAccounts( CArray <CAccount, CAccount&>& aAccount )
{
	aAccount.RemoveAll();
	aAccount.Append( m_aAccount );
}

///////////////////////////////////////////////////////////////////////////////////////
// Overridden Functions

BOOL
CAccountDlg::OnInitDialog( void )
{
	CDialog::OnInitDialog();

	HICON	hIcon = AfxGetApp()->LoadIcon( IDR_MAINFRAME );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	GetWindowRect( &m_rectOriginal );

	// Get a fixed-pitch font for the editboxes.

	CFont*	pFont = GetCommonFont();

	// Set the fixed-pich font to the editboxes and the list.

	GetDlgItem( IDC_EDIT_SERVER )->SetFont( pFont, FALSE );
	GetDlgItem( IDC_EDIT_USER   )->SetFont( pFont, FALSE );
	GetDlgItem( IDC_EDIT_PASS   )->SetFont( pFont, FALSE );

	CListCtrl*	pList = (CListCtrl*)GetDlgItem( IDC_LIST_ACCOUNTS );

	pList->SetFont( pFont, FALSE );

	// Change style of the list.

	DWORD	dwExStyle = pList->GetExtendedStyle();
	dwExStyle |= LVS_EX_FULLROWSELECT;
	dwExStyle |= LVS_EX_GRIDLINES;
	pList->SetExtendedStyle( dwExStyle );

	// Insert columns into the list.

	int	iColumn = 0;
	pList->InsertColumn( iColumn++, _T("dummy"), LVCFMT_LEFT,   10 );
	pList->InsertColumn( iColumn++, _T("User"),  LVCFMT_RIGHT, 100 );
	pList->InsertColumn( iColumn++, _T("Host"),  LVCFMT_LEFT,  240 );
	pList->DeleteColumn( 0 );
	//NOTE: Column 0 can not be set right-aligned.
	// see Remarks in https://learn.microsoft.com/en-us/windows/win32/api/commctrl/ns-commctrl-lvcolumnw

	// Add items to the list.

	pList->DeleteAllItems();
	int	iRow = 0;
	for	( iRow = 0; iRow < m_aAccount.GetCount(); iRow++ ){
		pList->InsertItem(  iRow, _T("") );
		pList->SetItemText( iRow, 0, m_aAccount[iRow].m_strUser );
		pList->SetItemText( iRow, 1, m_aAccount[iRow].m_strHost );
	}

	if	( iRow ){
		pList->SetColumnWidth( 0, LVSCW_AUTOSIZE );
		pList->SetColumnWidth( 1, LVSCW_AUTOSIZE );
	}

	// Set default status of the contols.

	((CButton*)GetDlgItem( IDC_CHECK_HIDE ))->SetCheck( BST_CHECKED );
	GetDlgItem( IDC_BUTTON_ADD )->EnableWindow( FALSE );
	CEdit*	pEdit = (CEdit*)GetDlgItem( IDC_EDIT_PASS );
	pEdit->SetPasswordChar( HIDE_CHAR );

	for	( iRow = 0; iRow < m_aAccount.GetCount(); iRow++ )
		if	( m_aAccount[iRow].m_nError ){
			pList->SetItemState( iRow, LVNI_SELECTED, LVNI_SELECTED );
			break;
		}

	// Ask main window to set position of this window.

	NotifyPopup( m_hWnd );

	GotoDlgCtrl( GetDlgItem( IDOK ) );

	return	FALSE;
}

BOOL
CAccountDlg::DestroyWindow( void )
{
	NotifyPopup( NULL );

	return	CDialog::DestroyWindow();
}

void 
CAccountDlg::OnOK( void )
{
	DWORD	dwStyle = GetDlgItem( IDC_BUTTON_ADD )->GetStyle();
	if	( dwStyle & WS_DISABLED ){
		CString	strHost, strUser, strPass;
		GetDlgItem( IDC_EDIT_SERVER )->GetWindowText( strHost );
		GetDlgItem( IDC_EDIT_USER   )->GetWindowText( strUser );
		GetDlgItem( IDC_EDIT_PASS   )->GetWindowText( strPass );
		int	iRadio = GetCheckedRadioButton( IDC_RADIO_POP, IDC_RADIO_POPS );
		if	( !iRadio ){
			if	( strHost.IsEmpty() && strUser.IsEmpty() && strPass.IsEmpty() )
				;
			else{
				AfxMessageBox( IDS_MB_POP_UNCHOSEN, MB_ICONEXCLAMATION );
				return;
			}
		}
	}
	else
		OnButtonAdd();

	CDialog::OnOK();
}

///////////////////////////////////////////////////////////////////////////////////////
// Message Handlers

BEGIN_MESSAGE_MAP( CAccountDlg, CDialog )
	ON_WM_GETMINMAXINFO()
	ON_WM_TIMER()
	ON_BN_CLICKED( IDHELP, OnHelp )
	ON_BN_CLICKED( IDC_CHECK_HIDE, OnCheckHide )
	ON_BN_CLICKED( IDC_BUTTON_ADD,    OnButtonAdd )
	ON_BN_CLICKED( IDC_BUTTON_DELETE, OnButtonDelete )
	ON_CONTROL_RANGE( EN_CHANGE, IDC_EDIT_SERVER, IDC_EDIT_PASS, OnChangeEdit )
	ON_CONTROL_RANGE( BN_CLICKED, IDC_RADIO_POP, IDC_RADIO_POPS, OnClickRadio )
	ON_NOTIFY( LVN_ITEMCHANGED, IDC_LIST_ACCOUNTS, OnChangeList )
END_MESSAGE_MAP()

void
CAccountDlg::OnGetMinMaxInfo( MINMAXINFO* lpMMI )
{
	// Answer minimum size.

	lpMMI->ptMinTrackSize.x = m_rectOriginal.Width();
	lpMMI->ptMinTrackSize.y = m_rectOriginal.Height();
}

void
CAccountDlg::OnTimer( UINT_PTR nIDEvent )
{
	if	( nIDEvent == TID_SELCHANGE ){
		KillTimer( nIDEvent );
		OnSelectItem();
	}
	else
		CDialog::OnTimer( nIDEvent );
}

void
CAccountDlg::OnHelp( void )
{
	ShowHelp( m_hWnd );
}

void
CAccountDlg::OnCheckHide( void )
{
	CEdit*	pEdit = (CEdit*)GetDlgItem( IDC_EDIT_PASS );
	TCHAR	ch = ( ((CButton*)GetDlgItem( IDC_CHECK_HIDE ))->GetCheck() == BST_CHECKED )? HIDE_CHAR: 0;
	pEdit->SetPasswordChar( ch );
	pEdit->Invalidate( TRUE );
}

void
CAccountDlg::OnButtonAdd( void )
{
	// Get parameters.

	CAccount	account;
	GetDlgItem( IDC_EDIT_SERVER )->GetWindowText( account.m_strHost );
	GetDlgItem( IDC_EDIT_USER   )->GetWindowText( account.m_strUser );
	GetDlgItem( IDC_EDIT_PASS   )->GetWindowText( account.m_strPass );
	if	( ((CButton*)GetDlgItem( IDC_RADIO_POP  ))->GetCheck() == BST_CHECKED )
		account.m_nPort = PORT_POP;
	else if	( ((CButton*)GetDlgItem( IDC_RADIO_POPS ))->GetCheck() == BST_CHECKED )
		account.m_nPort = PORT_POPS;

	// Check dupllication.

	CListCtrl*	pList = (CListCtrl*)GetDlgItem( IDC_LIST_ACCOUNTS );
	int	iRow, nRow = pList->GetItemCount();
	for	( iRow = 0; iRow < nRow; iRow++ )
		if	( m_aAccount[iRow].m_strHost == account.m_strHost &&
			  m_aAccount[iRow].m_strUser == account.m_strUser )
			break;

	// Remove the old entry if duplicated.

	if	( iRow < nRow ){
		pList->DeleteItem( iRow );
		m_aAccount.RemoveAt( iRow );
		nRow--;
	}

	// Add the new entry.

	m_aAccount.Add( account );

	pList->InsertItem(   nRow, _T("") );
	pList->SetItemText(  nRow, 0, account.m_strUser );
	pList->SetItemText(  nRow, 1, account.m_strHost );
	pList->SetItemState( nRow, LVNI_SELECTED, LVNI_SELECTED );

	pList->SetColumnWidth( 0, LVSCW_AUTOSIZE );
	pList->SetColumnWidth( 1, LVSCW_AUTOSIZE );

	pList->SetFocus();
}

void
CAccountDlg::OnButtonDelete( void )
{
	CListCtrl*	pList = (CListCtrl*)GetDlgItem( IDC_LIST_ACCOUNTS );
	int	iRow = pList->GetNextItem( -1, LVNI_ALL | LVNI_SELECTED );

	pList->DeleteItem( iRow );
	m_aAccount.RemoveAt( iRow );
}

void
CAccountDlg::OnChangeEdit( UINT uID )
{
	EnableAdd();
}

void
CAccountDlg::OnClickRadio( UINT uID )
{
	EnableAdd();
}

void
CAccountDlg::OnChangeList( NMHDR* pNMHDR, LRESULT* pResult )
{
	SetTimer( TID_SELCHANGE, 0, NULL );
	*pResult = 0;
}

///////////////////////////////////////////////////////////////////////////////////////
// Specific Functions

void
CAccountDlg::OnSelectItem( void )
{
	CListCtrl*	pList = (CListCtrl*)GetDlgItem( IDC_LIST_ACCOUNTS );
	int	iRow = pList->GetNextItem( -1, LVNI_ALL | LVNI_SELECTED );
	int	nRow = (int)m_aAccount.GetCount();

	if	( iRow >= 0 && iRow < nRow ){
		CAccount&	account = m_aAccount[iRow];

		GetDlgItem( IDC_EDIT_SERVER )->SetWindowText( account.m_strHost );
		GetDlgItem( IDC_EDIT_USER   )->SetWindowText( account.m_strUser );
		GetDlgItem( IDC_EDIT_PASS   )->SetWindowText( account.m_strPass );
		((CButton*)GetDlgItem( IDC_RADIO_POP  ))->SetCheck( BST_UNCHECKED );
		((CButton*)GetDlgItem( IDC_RADIO_POPS ))->SetCheck( BST_UNCHECKED );
		if	( account.m_nPort == PORT_POP )
			((CButton*)GetDlgItem( IDC_RADIO_POP  ))->SetCheck( BST_CHECKED );
		else if	( account.m_nPort == PORT_POPS )
			((CButton*)GetDlgItem( IDC_RADIO_POPS ))->SetCheck( BST_CHECKED );
		GetDlgItem( IDC_BUTTON_DELETE )->EnableWindow( TRUE );
	}
	else{
		GetDlgItem( IDC_EDIT_SERVER )->SetWindowText( _T("") );
		GetDlgItem( IDC_EDIT_USER   )->SetWindowText( _T("") );
		GetDlgItem( IDC_EDIT_PASS   )->SetWindowText( _T("") );
		((CButton*)GetDlgItem( IDC_RADIO_POP  ))->SetCheck( BST_UNCHECKED );
		((CButton*)GetDlgItem( IDC_RADIO_POPS ))->SetCheck( BST_UNCHECKED );
		GetDlgItem( IDC_BUTTON_DELETE )->EnableWindow( FALSE );
	}
}

void
CAccountDlg::EnableAdd( void )
{
	CString	strHost, strUser, strPass;
	GetDlgItem( IDC_EDIT_SERVER )->GetWindowText( strHost );
	GetDlgItem( IDC_EDIT_USER   )->GetWindowText( strUser );
	GetDlgItem( IDC_EDIT_PASS   )->GetWindowText( strPass );
	int	iRadio = GetCheckedRadioButton( IDC_RADIO_POP, IDC_RADIO_POPS );

	BOOL	bEnable = TRUE;
	if	( strHost.IsEmpty() || strUser.IsEmpty() || strPass.IsEmpty() )
		bEnable = FALSE;
	else if	( !iRadio )
		bEnable = FALSE;

	GetDlgItem( IDC_BUTTON_ADD )->EnableWindow( bEnable );
}
