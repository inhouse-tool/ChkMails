// DomainPage.cpp : implementation file
//

#include "pch.h"
#include "ChkMails.h"
#include "DomainPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#ifdef	UNICODE
#define	strcpy_s	wcscpy_s
#endif//UNICODE

IMPLEMENT_DYNAMIC( CDomainPage, CPropertyPage )

///////////////////////////////////////////////////////////////////////////////////////
// Constructor

CDomainPage::CDomainPage( void )
	: CPropertyPage( IDD_DOMAIN )
{
}

///////////////////////////////////////////////////////////////////////////////////////
// Overridden Functions

BOOL
CDomainPage::OnInitDialog( void )
{
	CPropertyPage::OnInitDialog();

	// Get a fixed-pitch font for the editboxes.

	CFont*	pFont = GetCommonFont();
	GetDlgItem( IDC_EDIT_DOMAIN )->SetFont( pFont, FALSE );

	// Initialize the domain list.

	CListCtrl*	pList;
	CStringArray	saList;
	DWORD		dwExStyle;
	
	pList = (CListCtrl*)GetDlgItem( IDC_LIST_DOMAIN );
	pList->SetFont( pFont, FALSE );

	dwExStyle = pList->GetExtendedStyle();
	dwExStyle |= LVS_EX_FULLROWSELECT;
	dwExStyle |= LVS_EX_GRIDLINES;
	dwExStyle |= LVS_EX_CHECKBOXES;
	pList->SetExtendedStyle( dwExStyle );

	// Insert columns into the domain list.
	{
		int	iColumn = 0;
		pList->InsertColumn( iColumn++, _T(""), LVCFMT_LEFT,   20 );
		pList->InsertColumn( iColumn++, _T(""), LVCFMT_LEFT,   60 );
		pList->InsertColumn( iColumn++, _T(""), LVCFMT_LEFT,    0 );
	}

	// Add domains.
	{
		SortDomain( saList, m_strTLDCache );
		INT_PTR	nRow = saList.GetCount();

		// Get maximum length of domain.

		int	nch = 0;
		for	( INT_PTR i = 0; i < nRow; i++ )
			if	( saList[i].GetLength() > nch )
				nch = saList[i].GetLength();

		for	( int iRow = 0; iRow < nRow; iRow++ ){
			CString	str = saList[iRow];
			pList->InsertItem(  iRow, _T("") );

			// Set right-align to the address ( not a top level domain ).

			if	( str[0] != '.' )
				str.Insert( 0, CString( ' ', nch-saList[iRow].GetLength() ) );
			pList->SetItemText( iRow, 1, str );

			// Set a check for the domain in the blacklist.

			str += _T("\n");
			int	xDrop = m_strDomains.Find( str );
			if	( xDrop < 0 )
				continue;
			else if	( xDrop == 0 )
				;
			else if	( m_strDomains[xDrop-1] == '\n' )
				;
			else
				continue;
			pList->SetCheck( iRow, TRUE );
		}
		pList->SetItemState( (int)nRow-1, 0, LVNI_SELECTED );
	}

	// Adjust the size of columns.

	pList->SetColumnWidth( 0, LVSCW_AUTOSIZE );
	pList->SetColumnWidth( 1, LVSCW_AUTOSIZE );
	pList->SetColumnWidth( 2, LVSCW_AUTOSIZE_USEHEADER );
	pList->Invalidate( TRUE );

	return	TRUE;
}

void
CDomainPage::OnOK( void )
{
	m_strTLDCache.Empty();
	m_strDomains.Empty();

	CListCtrl*	pList = (CListCtrl*)GetDlgItem( IDC_LIST_DOMAIN );
	int	nRow = pList->GetItemCount();
	for	( int iRow = 0; iRow < nRow; iRow++ ){
		CString	str = pList->GetItemText( iRow, 1 );
		str.TrimLeft();
		m_strTLDCache += str + _T("\n");
		if	( pList->GetCheck( iRow ) )
			m_strDomains += str + _T("\n");
	}
}

BOOL
CDomainPage::PreTranslateMessage( MSG* pMsg )
{
	if	( pMsg->message == WM_KEYDOWN &&
		  pMsg->wParam  == VK_RETURN ){
		CWnd*	pwnd = GetFocus();
		if	( pwnd == GetDlgItem( IDC_EDIT_DOMAIN ) ){
			PostMessage( WM_COMMAND, MAKEWPARAM( IDC_BUTTON_DOMAIN_ADD,  BN_CLICKED ), NULL );
			return	TRUE;
		}
	}

	return	CPropertyPage::PreTranslateMessage( pMsg );
}

///////////////////////////////////////////////////////////////////////////////////////
// Message Handlers

BEGIN_MESSAGE_MAP( CDomainPage, CPropertyPage )
	ON_BN_CLICKED( IDC_BUTTON_DOMAIN_ADD,  OnButtonAdd )
	ON_BN_CLICKED( IDC_BUTTON_DOMAIN_DEL,  OnButtonDelete )
	ON_EN_CHANGE( IDC_EDIT_DOMAIN,  OnEditDomain )
	ON_NOTIFY( LVN_ITEMCHANGED, IDC_LIST_DOMAIN,  OnChangeList )
END_MESSAGE_MAP()

void
CDomainPage::OnButtonAdd( void )
{
	CString	strNew;
	GetDlgItem( IDC_EDIT_DOMAIN )->GetWindowText( strNew );

	CListCtrl*	pList = (CListCtrl*)GetDlgItem( IDC_LIST_DOMAIN );
	
	int	iRow, nRow = pList->GetItemCount();
	for	( iRow = 0; iRow < nRow; iRow++ ){
		CString	str = pList->GetItemText( iRow, 1 );
		if	( strNew == str )
			return;
	}
	int	iColumn = 1;
	pList->InsertItem(    iRow, _T("") );
	pList->SetItemText(   iRow, iColumn, strNew );
	pList->SetColumnWidth(   iColumn, LVSCW_AUTOSIZE );
	pList->SetColumnWidth( ++iColumn, LVSCW_AUTOSIZE_USEHEADER );
	pList->EnsureVisible( iRow, FALSE );
	pList->SetItemState(  iRow, LVNI_SELECTED, LVNI_SELECTED );

	GetDlgItem( IDC_EDIT_DOMAIN )->SetWindowText( _T("") );
}

void
CDomainPage::OnButtonDelete( void )
{
	CListCtrl*	pList = (CListCtrl*)GetDlgItem( IDC_LIST_DOMAIN );

	POSITION	pos = pList->GetFirstSelectedItemPosition();
	while	( pos ){
		int	iRow = pList->GetNextSelectedItem( pos );
		pList->DeleteItem( iRow );
	}
}

void
CDomainPage::OnEditDomain( void )
{
	CString	str;
	GetDlgItem( IDC_EDIT_DOMAIN )->GetWindowText( str );
	GetDlgItem( IDC_BUTTON_DOMAIN_ADD )->EnableWindow( str.IsEmpty()? FALSE: TRUE );
}

#define	LVNI_UNCHECKED	0x1000
#define	LVNI_CHECKED	0x2000

void
CDomainPage::OnChangeList( NMHDR* pNMHDR, LRESULT* pResult )
{
	*pResult = 0;
	NMLISTVIEW* pNMLV = reinterpret_cast<NMLISTVIEW*>( pNMHDR );
	int	iRow    = pNMLV->iItem;
	int	iColumn = pNMLV->iSubItem;

	int	iChecked = -1;
	if	( iColumn == 0 ){
		if	( ( pNMLV->uOldState & LVNI_UNCHECKED ) &&
			  ( pNMLV->uNewState & LVNI_CHECKED ) )
			iChecked = 1;
		else if	( ( pNMLV->uOldState & LVNI_CHECKED ) &&
			  ( pNMLV->uNewState & LVNI_UNCHECKED ) )
			iChecked = 0;
	}

	int	iSelected = -1;
	{
		if	(!( pNMLV->uOldState & LVNI_SELECTED ) &&
			  ( pNMLV->uNewState & LVNI_SELECTED ) )
			iSelected = 1;
		else if	( ( pNMLV->uOldState & LVNI_SELECTED ) &&
			 !( pNMLV->uNewState & LVNI_SELECTED ) )
			iSelected = 0;
	}

	if	( iChecked >= 0 ){
		CListCtrl*	pList = (CListCtrl*)GetDlgItem( (UINT)pNMLV->hdr.idFrom );
		CString	strDomain = pList->GetItemText( iRow, 1 );
		if	( strDomain.IsEmpty() )
			return;

		int	nRow = pList->GetItemCount();
		for	( int i = 0; i < nRow; i++ ){
			if	( i == iRow )
				continue;

			CString	str = pList->GetItemText( i, 1 );
			if	( str == strDomain )
				pList->SetCheck( i, ( iChecked > 0 )? TRUE: FALSE );
		}
		pList->SetItemState( iRow, LVNI_SELECTED, LVNI_SELECTED );
		iSelected = 1;
	}

	GetDlgItem( IDC_BUTTON_DOMAIN_DEL  )->EnableWindow( ( iSelected > 0 )? TRUE: FALSE );
}

///////////////////////////////////////////////////////////////////////////////////////
// Specific Functions

void
CDomainPage::SortDomain( CStringArray& saList, CString strList )
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
CDomainPage::CompareDomain( CString strDomain1, CString strDomain2 )
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