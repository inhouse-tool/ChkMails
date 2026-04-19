// WordPage.cpp : implementation file
//

#include "pch.h"
#include "ChkMails.h"
#include "WordPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#ifdef	_UNICODE
#define	strcpy_s	wcscpy_s
#endif//_UNICODE

IMPLEMENT_DYNAMIC( CWordPage, CPropertyPage )

///////////////////////////////////////////////////////////////////////////////////////
// Constructor

CWordPage::CWordPage( void )
	: CPropertyPage( IDD_WORD )
{
}

///////////////////////////////////////////////////////////////////////////////////////
// Overridden Functions

BOOL
CWordPage::OnInitDialog( void )
{
	CPropertyPage::OnInitDialog();

	// Get a fixed-pitch font for the editboxes.

	CFont*	pFont = GetCommonFont();
	GetDlgItem( IDC_EDIT_WORD )->SetFont( pFont, FALSE );

	// Initialize the word list.

	CListCtrl*	pList;
	CStringArray	saList;
	DWORD		dwExStyle;
	
	pList = (CListCtrl*)GetDlgItem( IDC_LIST_WORD );
	pList->SetFont( pFont, FALSE );

	dwExStyle = pList->GetExtendedStyle();
	dwExStyle |= LVS_EX_FULLROWSELECT;
	dwExStyle |= LVS_EX_GRIDLINES;
	dwExStyle |= LVS_EX_CHECKBOXES;
	pList->SetExtendedStyle( dwExStyle );

	// Insert columns into the word list.
	{
		int	iColumn = 0;
		pList->InsertColumn( iColumn++, _T(""), LVCFMT_LEFT,   20 );
		pList->InsertColumn( iColumn++, _T(""), LVCFMT_LEFT,   60 );
		pList->InsertColumn( iColumn++, _T(""), LVCFMT_LEFT,    0 );
	}

	// Add words.
	{
		SortWord( saList, m_strWords );
		INT_PTR	nRow = saList.GetCount();

		for	( int iRow = 0; iRow < nRow; iRow++ ){
			CString	str = saList[iRow];
			bool	bDrop = false;
			if	( str.Right( 1 ) == _T("\b") ){
				str = str.Left( str.GetLength()-1 );
				bDrop = true;
			}
			pList->InsertItem( iRow, _T("") );

			pList->SetItemText( iRow, 1, str );

			// Set a check for the word in the censored words list.

			pList->SetCheck( iRow, bDrop? TRUE: FALSE );
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
CWordPage::OnOK( void )
{
	m_strWords.Empty();

	CListCtrl*	pList = (CListCtrl*)GetDlgItem( IDC_LIST_WORD );
	int	nRow = pList->GetItemCount();
	for	( int iRow = 0; iRow < nRow; iRow++ ){
		CString	str = pList->GetItemText( iRow, 1 );
		str.TrimLeft();
		if	( pList->GetCheck( iRow ) )
			str += _T("\b");
		m_strWords += str + _T("\n");
	}
}

BOOL
CWordPage::PreTranslateMessage( MSG* pMsg )
{
	if	( pMsg->message == WM_KEYDOWN &&
		  pMsg->wParam  == VK_RETURN ){
		CWnd*	pwnd = GetFocus();
		if	( pwnd == GetDlgItem( IDC_EDIT_WORD ) ){
			PostMessage( WM_COMMAND, MAKEWPARAM( IDC_BUTTON_WORD_ADD,  BN_CLICKED ), NULL );
			return	TRUE;
		}
	}

	return	CPropertyPage::PreTranslateMessage( pMsg );
}

///////////////////////////////////////////////////////////////////////////////////////
// Message Handlers

BEGIN_MESSAGE_MAP( CWordPage, CPropertyPage )
	ON_BN_CLICKED( IDC_BUTTON_WORD_ADD,  OnButtonAdd )
	ON_BN_CLICKED( IDC_BUTTON_WORD_DEL,  OnButtonDelete )
	ON_EN_CHANGE( IDC_EDIT_WORD,  OnChangeEdit )
	ON_NOTIFY( LVN_ITEMCHANGED, IDC_LIST_WORD,  OnChangeList )
END_MESSAGE_MAP()

#define	LVNI_UNCHECKED	0x1000
#define	LVNI_CHECKED	0x2000

void
CWordPage::OnButtonAdd( void )
{
	CString	strNew;
	GetDlgItem( IDC_EDIT_WORD )->GetWindowText( strNew );

	CListCtrl*	pList = (CListCtrl*)GetDlgItem( IDC_LIST_WORD );
	
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
	pList->SetCheck( iRow, TRUE );

	GetDlgItem( IDC_EDIT_WORD )->SetWindowText( _T("") );
}

void
CWordPage::OnButtonDelete( void )
{
	CListCtrl*	pList = (CListCtrl*)GetDlgItem( IDC_LIST_WORD );

	POSITION	pos = pList->GetFirstSelectedItemPosition();
	while	( pos ){
		int	iRow = pList->GetNextSelectedItem( pos );
		pList->DeleteItem( iRow );
	}
}

void
CWordPage::OnChangeEdit( void )
{
	CString	str;
	GetDlgItem( IDC_EDIT_WORD )->GetWindowText( str );
	GetDlgItem( IDC_BUTTON_WORD_ADD )->EnableWindow( str.IsEmpty()? FALSE: TRUE );
}

void
CWordPage::OnChangeList( NMHDR* pNMHDR, LRESULT* pResult )
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
		CString	strWord = pList->GetItemText( iRow, 1 );
		if	( strWord.IsEmpty() )
			return;

		int	nRow = pList->GetItemCount();
		for	( int i = 0; i < nRow; i++ ){
			if	( i == iRow )
				continue;

			CString	str = pList->GetItemText( i, 1 );
			if	( str == strWord )
				pList->SetCheck( i, ( iChecked > 0 )? TRUE: FALSE );
		}
		pList->SetItemState( iRow, LVNI_SELECTED, LVNI_SELECTED );
		iSelected = 1;
	}

	GetDlgItem( IDC_BUTTON_WORD_DEL  )->EnableWindow( ( iSelected > 0 )? TRUE: FALSE );
}

///////////////////////////////////////////////////////////////////////////////////////
// Specific Functions

void
CWordPage::SortWord( CStringArray& saList, CString strList )
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
			if	( strItem < strList )
				break;
		}
		saList.InsertAt( i, strItem );
	}
}
