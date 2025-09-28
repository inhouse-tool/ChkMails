// RecipientPage.cpp : implementation file
//

#include "pch.h"
#include "ChkMails.h"
#include "RecipientPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC( CRecipientPage, CPropertyPage )

///////////////////////////////////////////////////////////////////////////////////////
// Constructor

CRecipientPage::CRecipientPage( void )
	: CPropertyPage( IDD_RECIPIENT )
{
}

///////////////////////////////////////////////////////////////////////////////////////
// Overridden Functions

BOOL
CRecipientPage::OnInitDialog( void )
{
	CPropertyPage::OnInitDialog();

	// Get a fixed-pitch font for the editboxes.

	CFont*	pFont = GetCommonFont();
	GetDlgItem( IDC_EDIT_RECIPIENT )->SetFont( pFont, FALSE );
	GetDlgItem( IDC_EDIT_SENDER    )->SetFont( pFont, FALSE );

	// Initialize the domain list.

	CListCtrl*	pList;
	DWORD		dwExStyle;
	
	pList = (CListCtrl*)GetDlgItem( IDC_LIST_RECIPIENT );
	pList->SetFont( pFont, FALSE );

	dwExStyle = pList->GetExtendedStyle();
	dwExStyle |= LVS_EX_FULLROWSELECT;
	dwExStyle |= LVS_EX_GRIDLINES;
	pList->SetExtendedStyle( dwExStyle );

	pList->InsertColumn( 0, _T("dummy"),  LVCFMT_LEFT,   10 );
	pList->InsertColumn( 1, _T(""), LVCFMT_RIGHT, 0 );
	pList->DeleteColumn( 0 );
	pList->SetColumnWidth( 0, LVSCW_AUTOSIZE_USEHEADER );
	//NOTE: Column 0 can not be set right-aligned.
	// see Remarks in https://learn.microsoft.com/en-us/windows/win32/api/commctrl/ns-commctrl-lvcolumnw

	pList->SetColumnWidth( 0, LVSCW_AUTOSIZE_USEHEADER );
	pList->Invalidate( TRUE );

	pList = (CListCtrl*)GetDlgItem( IDC_LIST_SENDER );
	pList->SetFont( pFont, FALSE );

	dwExStyle = pList->GetExtendedStyle();
	dwExStyle |= LVS_EX_FULLROWSELECT;
	dwExStyle |= LVS_EX_GRIDLINES;
	pList->SetExtendedStyle( dwExStyle );

	pList->InsertColumn( 0, _T("dummy"),  LVCFMT_LEFT,   10 );
	pList->InsertColumn( 1, _T(""), LVCFMT_RIGHT, 0 );
	pList->DeleteColumn( 0 );
	pList->SetColumnWidth( 0, LVSCW_AUTOSIZE_USEHEADER );
	//NOTE: Column 0 can not be set right-aligned.
	// see Remarks in https://learn.microsoft.com/en-us/windows/win32/api/commctrl/ns-commctrl-lvcolumnw

	pList->SetColumnWidth( 0, LVSCW_AUTOSIZE_USEHEADER );
	pList->Invalidate( TRUE );

	// Add domains.
	{
		CStringArray	saList;

		SortDomain( saList, m_strRecipients );
		INT_PTR	nRow = saList.GetCount();

		// Get maximum length of domain.

		int	nch = 0;
		for	( INT_PTR i = 0; i < nRow; i++ )
			if	( saList[i].GetLength() > nch )
				nch = saList[i].GetLength();

		pList = (CListCtrl*)GetDlgItem( IDC_LIST_RECIPIENT );
		for	( int iRow = 0; iRow < nRow; iRow++ ){
			CString	str = saList[iRow];
			pList->InsertItem(  iRow, _T("") );

			// Set right-align to the address ( not a top level domain ).

			if	( str[0] != '.' )
				str.Insert( 0, CString( ' ', nch-saList[iRow].GetLength() ) );
			pList->SetItemText( iRow, 0, str );
		}
		pList->SetItemState( (int)nRow-1, 0, LVNI_SELECTED );
	}
	{
		CStringArray	saList;

		SortDomain( saList, m_strSenders );
		INT_PTR	nRow = saList.GetCount();

		// Get maximum length of domain.

		int	nch = 0;
		for	( INT_PTR i = 0; i < nRow; i++ )
			if	( saList[i].GetLength() > nch )
				nch = saList[i].GetLength();

		pList = (CListCtrl*)GetDlgItem( IDC_LIST_SENDER );
		for	( int iRow = 0; iRow < nRow; iRow++ ){
			CString	str = saList[iRow];
			pList->InsertItem(  iRow, _T("") );

			// Set right-align to the address ( not a top level domain ).

			if	( str[0] != '.' )
				str.Insert( 0, CString( ' ', nch-saList[iRow].GetLength() ) );
			pList->SetItemText( iRow, 0, str );
		}
		pList->SetItemState( (int)nRow-1, 0, LVNI_SELECTED );
	}

	return	TRUE;
}

void
CRecipientPage::OnOK( void )
{
	m_strRecipients.Empty();
	m_strSenders.Empty();

	CListCtrl*	pList;
	int		nRow;

	pList = (CListCtrl*)GetDlgItem( IDC_LIST_RECIPIENT );
	nRow = pList->GetItemCount();
	for	( int iRow = 0; iRow < nRow; iRow++ ){
		CString	str = pList->GetItemText( iRow, 0 );
		str.TrimLeft();
		m_strRecipients += str + _T("\n");
	}

	pList = (CListCtrl*)GetDlgItem( IDC_LIST_SENDER );
	nRow = pList->GetItemCount();
	for	( int iRow = 0; iRow < nRow; iRow++ ){
		CString	str = pList->GetItemText( iRow, 0 );
		str.TrimLeft();
		m_strSenders += str + _T("\n");
	}
}

BOOL
CRecipientPage::PreTranslateMessage( MSG* pMsg )
{
	if	( pMsg->message == WM_KEYDOWN &&
		  pMsg->wParam  == VK_RETURN ){
		CWnd*	pwnd = GetFocus();
		if	( pwnd == GetDlgItem( IDC_EDIT_RECIPIENT ) ){
			PostMessage( WM_COMMAND, MAKEWPARAM( IDC_BUTTON_RECIPIENT_ADD, BN_CLICKED ), NULL );
			return	TRUE;
		}
		else if	( pwnd == GetDlgItem( IDC_EDIT_SENDER ) ){
			PostMessage( WM_COMMAND, MAKEWPARAM( IDC_BUTTON_SENDER_ADD,    BN_CLICKED ), NULL );
			return	TRUE;
		}
	}

	return	CPropertyPage::PreTranslateMessage( pMsg );
}

///////////////////////////////////////////////////////////////////////////////////////
// Message Handlers

BEGIN_MESSAGE_MAP( CRecipientPage, CPropertyPage )
	ON_BN_CLICKED( IDC_BUTTON_RECIPIENT_ADD,         OnButtonAddRecipient )
	ON_BN_CLICKED( IDC_BUTTON_RECIPIENT_DEL,         OnButtonDeleteRecipient )
	ON_EN_CHANGE( IDC_EDIT_RECIPIENT,                OnEditRecipient )
	ON_NOTIFY( LVN_ITEMCHANGED, IDC_LIST_RECIPIENT,  OnChangeListRecipient )
	ON_BN_CLICKED( IDC_BUTTON_SENDER_ADD,         OnButtonAddSender )
	ON_BN_CLICKED( IDC_BUTTON_SENDER_DEL,         OnButtonDeleteSender )
	ON_EN_CHANGE( IDC_EDIT_SENDER,                OnEditSender )
	ON_NOTIFY( LVN_ITEMCHANGED, IDC_LIST_SENDER,  OnChangeListSender )
END_MESSAGE_MAP()

void
CRecipientPage::OnButtonAddRecipient( void )
{
	CString	strNew;
	GetDlgItem( IDC_EDIT_RECIPIENT )->GetWindowText( strNew );

	CListCtrl*	pList = (CListCtrl*)GetDlgItem( IDC_LIST_RECIPIENT );
	
	int	iRow, nRow = pList->GetItemCount();
	for	( iRow = 0; iRow < nRow; iRow++ ){
		CString	str = pList->GetItemText( iRow, 1 );
		if	( strNew == str )
			return;
	}
	int	iColumn = 0;
	pList->InsertItem(    iRow, _T("") );
	pList->SetItemText(   iRow, iColumn, strNew );
	pList->SetColumnWidth( iColumn, LVSCW_AUTOSIZE_USEHEADER );
	pList->EnsureVisible( iRow, FALSE );
	pList->SetItemState(  iRow, LVNI_SELECTED, LVNI_SELECTED );

	GetDlgItem( IDC_EDIT_RECIPIENT )->SetWindowText( _T("") );
}

void
CRecipientPage::OnButtonDeleteRecipient( void )
{
	CListCtrl*	pList = (CListCtrl*)GetDlgItem( IDC_LIST_RECIPIENT );

	POSITION	pos = pList->GetFirstSelectedItemPosition();
	while	( pos ){
		int	iRow = pList->GetNextSelectedItem( pos );
		pList->DeleteItem( iRow );
	}
}

void
CRecipientPage::OnEditRecipient( void )
{
	CString	str;
	GetDlgItem( IDC_EDIT_RECIPIENT )->GetWindowText( str );
	GetDlgItem( IDC_BUTTON_RECIPIENT_ADD )->EnableWindow( str.IsEmpty()? FALSE: TRUE );
}

void
CRecipientPage::OnChangeListRecipient( NMHDR* pNMHDR, LRESULT* pResult )
{
	*pResult = 0;
	NMLISTVIEW* pNMLV = reinterpret_cast<NMLISTVIEW*>( pNMHDR );
	int	iRow    = pNMLV->iItem;
	int	iColumn = pNMLV->iSubItem;

	int	iSelected = -1;
	if	( iColumn == 0 ){
		if	(!( pNMLV->uOldState & LVNI_SELECTED ) &&
			  ( pNMLV->uNewState & LVNI_SELECTED ) )
			iSelected = 1;
		else if	( ( pNMLV->uOldState & LVNI_SELECTED ) &&
			 !( pNMLV->uNewState & LVNI_SELECTED ) )
			iSelected = 0;
	}

	GetDlgItem( IDC_BUTTON_RECIPIENT_DEL )->EnableWindow( ( iSelected > 0 )? TRUE: FALSE );
}

void
CRecipientPage::OnButtonAddSender( void )
{
	CString	strNew;
	GetDlgItem( IDC_EDIT_SENDER )->GetWindowText( strNew );

	CListCtrl*	pList = (CListCtrl*)GetDlgItem( IDC_LIST_SENDER );
	
	int	iRow, nRow = pList->GetItemCount();
	for	( iRow = 0; iRow < nRow; iRow++ ){
		CString	str = pList->GetItemText( iRow, 1 );
		if	( strNew == str )
			return;
	}
	int	iColumn = 0;
	pList->InsertItem(    iRow, _T("") );
	pList->SetItemText(   iRow, iColumn, strNew );
	pList->SetColumnWidth( iColumn, LVSCW_AUTOSIZE_USEHEADER );
	pList->EnsureVisible( iRow, FALSE );
	pList->SetItemState(  iRow, LVNI_SELECTED, LVNI_SELECTED );

	GetDlgItem( IDC_EDIT_SENDER )->SetWindowText( _T("") );
}

void
CRecipientPage::OnButtonDeleteSender( void )
{
	CListCtrl*	pList = (CListCtrl*)GetDlgItem( IDC_LIST_SENDER );

	POSITION	pos = pList->GetFirstSelectedItemPosition();
	while	( pos ){
		int	iRow = pList->GetNextSelectedItem( pos );
		pList->DeleteItem( iRow );
	}
}

void
CRecipientPage::OnEditSender( void )
{
	CString	str;
	GetDlgItem( IDC_EDIT_SENDER )->GetWindowText( str );
	GetDlgItem( IDC_BUTTON_SENDER_ADD )->EnableWindow( str.IsEmpty()? FALSE: TRUE );
}

void
CRecipientPage::OnChangeListSender( NMHDR* pNMHDR, LRESULT* pResult )
{
	*pResult = 0;
	NMLISTVIEW* pNMLV = reinterpret_cast<NMLISTVIEW*>( pNMHDR );
	int	iRow    = pNMLV->iItem;
	int	iColumn = pNMLV->iSubItem;

	int	iSelected = -1;
	if	( iColumn == 0 ){
		if	(!( pNMLV->uOldState & LVNI_SELECTED ) &&
			  ( pNMLV->uNewState & LVNI_SELECTED ) )
			iSelected = 1;
		else if	( ( pNMLV->uOldState & LVNI_SELECTED ) &&
			 !( pNMLV->uNewState & LVNI_SELECTED ) )
			iSelected = 0;
	}

	GetDlgItem( IDC_BUTTON_SENDER_DEL )->EnableWindow( ( iSelected > 0 )? TRUE: FALSE );
}

///////////////////////////////////////////////////////////////////////////////////////
// Specific Functions

void
CRecipientPage::SortDomain( CStringArray& saList, CString strList )
{
	saList.RemoveAll();

	for	( int xToken = 0; xToken >= 0; ){
		INT_PTR	i, n = saList.GetCount();
		CString	strItem = strList.Tokenize( _T("\n"), xToken );
		if	( strItem.IsEmpty() )
			break;

		for	( i = 0; i < n; i++ ){
			CString	strList = saList[i];
			strList.TrimLeft();
			int	iOrder = CompareDomain( strItem, strList );
			if	( iOrder < 0 )
				break;
		}
		saList.InsertAt( i, strItem );
	}
}

int
CRecipientPage::CompareDomain( CString strDomain1, CString strDomain2 )
{
	int	x1, x2, x1a, x2a;

	CString	strAddr1 = strDomain1;
	CString	strAddr2 = strDomain2;

	x1  = strAddr1.ReverseFind( '.' );
	x2  = strAddr2.ReverseFind( '.' );

	CString	str1, str2;

	for	( ;; ){
		if	( x1 < 0 )
			x1 = 0;
		if	( x2 < 0 )
			x2 = 0;
		str1 = strAddr1.Mid( x1 );
		str2 = strAddr2.Mid( x2 );
		strAddr1 = strAddr1.Left( x1 );
		strAddr2 = strAddr2.Left( x2 );

		if	( str1[0] == '@' )
			str1.Delete( 0, 1 );
		if	( str2[0] == '@' )
			str2.Delete( 0, 1 );
		if	( str1[0] == '.' )
			str1.Delete( 0, 1 );
		if	( str2[0] == '.' )
			str2.Delete( 0, 1 );

		if	( str1 > str2 )
			return	+1;
		else if	( str1 < str2 )
			return	-1;
		else if	( str1.IsEmpty() && str2.IsEmpty() )
			break;

		x1  = strAddr1.ReverseFind( '.' );
		x2  = strAddr2.ReverseFind( '.' );
		x1a = strAddr1.ReverseFind( '@' );
		x2a = strAddr2.ReverseFind( '@' );

		if	( x1 < x1a )
			x1 = x1a;
		if	( x2 < x2a )
			x2 = x2a;
	}

	return	0;
}