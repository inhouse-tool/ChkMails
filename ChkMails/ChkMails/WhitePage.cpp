// WhitePage.cpp : implementation file
//

#include "pch.h"
#include "ChkMails.h"
#include "WhitePage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC( CWhitePage, CPropertyPage )

///////////////////////////////////////////////////////////////////////////////////////
// Constructor

CWhitePage::CWhitePage( void )
	: CPropertyPage( IDD_WHITE )
{
}

///////////////////////////////////////////////////////////////////////////////////////
// Overridden Functions

BOOL
CWhitePage::OnInitDialog( void )
{
	CPropertyPage::OnInitDialog();

	// Get a fixed-pitch font for the editboxes.

	CFont*	pFont = GetCommonFont();

	// Initialize the domain list.

	CListCtrl*	pList;
	DWORD		dwExStyle;
	
	pList = (CListCtrl*)GetDlgItem( IDC_LIST_WHITE );
	pList->SetFont( pFont, FALSE );

	dwExStyle = pList->GetExtendedStyle();
	dwExStyle |= LVS_EX_FULLROWSELECT;
	dwExStyle |= LVS_EX_GRIDLINES;
	pList->SetExtendedStyle( dwExStyle );

	// Insert columns into the domain list.
	
	int	iColumn = 0;
	pList->InsertColumn( iColumn++, _T("dummy"),  LVCFMT_LEFT,   10 );
	pList->InsertColumn( iColumn++, _T("From:"),  LVCFMT_RIGHT, 200 );
	pList->InsertColumn( iColumn++, _T("Domain"), LVCFMT_RIGHT, 200 );
	pList->InsertColumn( iColumn++, _T(""),       LVCFMT_LEFT,   10 );
	pList->DeleteColumn( 0 );
	pList->SetColumnWidth( 2, LVSCW_AUTOSIZE_USEHEADER );
	//NOTE: Column 0 can not be set right-aligned.
	// see Remarks in https://learn.microsoft.com/en-us/windows/win32/api/commctrl/ns-commctrl-lvcolumnw

	int	iRow = 0;
	for	( int xToken = 0; xToken >= 0; ){
		CString	strItem = m_strWhites.Tokenize( _T("\n"), xToken );
		if	( strItem.IsEmpty() )
			break;
		int	x = strItem.Find( '\t' );
		CString	strFrom   = strItem.Left( x );
		CString	strDomain = strItem.Mid( x+1 );
		pList->InsertItem(  iRow, strFrom );
		pList->SetItemText( iRow, 1, strDomain );
		iRow++;
	}

	if	( pList->GetItemCount() ){
		pList->SetColumnWidth( 0, LVSCW_AUTOSIZE );
		pList->SetColumnWidth( 1, LVSCW_AUTOSIZE );
		pList->SetColumnWidth( 2, LVSCW_AUTOSIZE_USEHEADER );
	}

	CHeaderCtrl*	pHeader = pList->GetHeaderCtrl();
	HDITEM	header = {};
	header.mask = HDI_FORMAT;
	pHeader->GetItem( 0, &header );
	header.fmt |=  HDF_SORTUP;
	pHeader->SetItem( 0, &header );

	CString	str;
	(void )str.LoadString( IDS_TEXT_WHITE_ADD );
	GetDlgItem( IDC_STATIC_WHITE )->SetWindowText( str );

	return	TRUE;
}

void
CWhitePage::OnOK( void )
{
	CListCtrl*	pList;
	INT_PTR	n;

	m_strWhites.Empty();
	pList = (CListCtrl*)GetDlgItem( IDC_LIST_WHITE );

	SortArg	arg = {};

	arg.pList   = pList;
	arg.iColumn = 0;
	arg.iOrder  = 1;
	pList->SortItems( CompareSender, (DWORD_PTR)&arg );

	n = pList->GetItemCount();
	for	( int i = 0; i < n; i++ )
		m_strWhites += pList->GetItemText( i, 0 ) + _T("\t") + pList->GetItemText( i, 1 ) + _T("\n");
}

///////////////////////////////////////////////////////////////////////////////////////
// Message Handlers

BEGIN_MESSAGE_MAP( CWhitePage, CPropertyPage )
	ON_WM_DROPFILES()
	ON_BN_CLICKED( IDC_BUTTON_WHITE_DEL, OnButtonDelete )
	ON_NOTIFY( LVN_ITEMCHANGED, IDC_LIST_WHITE, OnChangeList )
	ON_NOTIFY( HDN_ITEMCLICK,   0,              OnClickHeader )
	ON_COMMAND( ID_EDIT_PASTE, OnEditPaste )
END_MESSAGE_MAP()

void
CWhitePage::OnDropFiles( HDROP hDropInfo )
{
	int	nDone = 0;

	UINT	nFile =
	DragQueryFile( hDropInfo, -1, NULL, 0 );

	for	( UINT iFile = 0; iFile < nFile; iFile++ ){
		TCHAR	szFile[MAX_PATH];
		DragQueryFile( hDropInfo, iFile, szFile, _countof( szFile ) );
		CString	strFile = szFile;
		CFile	fIn;
		if	( fIn.Open( strFile, CFile::modeRead | CFile::shareExclusive ) ){
			QWORD	cchContent = fIn.GetLength();
			char*	pchContent = new char[(int)cchContent+1];
			fIn.Read( pchContent, (UINT)cchContent );
			pchContent[cchContent] = '\0';
			CStringA	strContent( pchContent );
			delete[] pchContent;

			if	( !TakeSender( strContent ) ){
				CString	strError;
				(void)strError.LoadString( IDS_RE_UNRELIABLE );
				int	x = strFile.ReverseFind( '\\' );
				CString	str = strFile.Mid( x+1 );
				str.Insert( 0, _T("'") );
				str += _T("'.");
				strError += str;
				AfxMessageBox( strError, MB_ICONHAND );
			}
		}
	}
}

void
CWhitePage::OnButtonDelete( void )
{
	CListCtrl*	pList = (CListCtrl*)GetDlgItem( IDC_LIST_WHITE );

	POSITION	pos = pList->GetFirstSelectedItemPosition();
	while	( pos ){
		int	iSelected = pList->GetNextSelectedItem( pos );
		pList->DeleteItem( iSelected );
	}

	if	( pList->GetItemCount() ){
		pList->SetColumnWidth( 0, LVSCW_AUTOSIZE );
		pList->SetColumnWidth( 1, LVSCW_AUTOSIZE );
		pList->SetColumnWidth( 2, LVSCW_AUTOSIZE_USEHEADER );
	}
}

void
CWhitePage::OnChangeList( NMHDR* pNMHDR, LRESULT* pResult )
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>( pNMHDR );

	BOOL	bSelected = FALSE;

	if	( ( pNMLV->uNewState & LVNI_SELECTED ) )
		bSelected = TRUE;
	else
		bSelected = FALSE;

	GetDlgItem( IDC_BUTTON_WHITE_DEL )->EnableWindow( bSelected );
	CString	str;
	(void )str.LoadString( bSelected? IDS_TEXT_WHITE_DEL: IDS_TEXT_WHITE_ADD );
	GetDlgItem( IDC_STATIC_WHITE )->SetWindowText( str );
}

void
CWhitePage::OnClickHeader( NMHDR* pNMHDR, LRESULT* pResult )
{
	NMHEADER*	phdr = reinterpret_cast<NMHEADER*>( pNMHDR );

	CListCtrl*	pList = (CListCtrl*)GetDlgItem( IDC_LIST_WHITE );
	CHeaderCtrl*	pHeader = pList->GetHeaderCtrl();

	int	iColumn = phdr->iItem;
	int	iOrder = 0;

	if	( iColumn < 2 ){
		HDITEM	header = {};
		header.mask = HDI_FORMAT;
		pHeader->GetItem( iColumn, &header );
		if	( header.fmt & HDF_SORTUP ){
			header.fmt &= ~HDF_SORTUP;
			header.fmt |=  HDF_SORTDOWN;
			iOrder = 2;
		}
		else if	( header.fmt & HDF_SORTDOWN ){
			header.fmt &= ~HDF_SORTDOWN;
			header.fmt |= HDF_SORTUP;
			iOrder = 1;
		}
		else{
			header.fmt |= HDF_SORTUP;
			iOrder = 1;
		}
		pHeader->SetItem( iColumn, &header );

		int	nColumn = pHeader->GetItemCount();
		for	( int i = 0; i < nColumn; i++ ){
			if	( i != iColumn ){
				pHeader->GetItem( i, &header );
				header.fmt &= ~(HDF_SORTUP | HDF_SORTDOWN);
				pHeader->SetItem( i, &header );
			}
		}
	}

	SortSender();

	*pResult = 0;
}

void
CWhitePage::OnEditPaste( void )
{
	if	( OpenClipboard() ){
		HGLOBAL	hMem = GetClipboardData( CF_TEXT );
		if	( hMem ){
			void*	pMem = GlobalLock( hMem );
			CStringA strPasted( (char*)pMem );
			GlobalUnlock( hMem );

			if	( !TakeSender( strPasted ) ){
				CString	strError;
				(void)strError.LoadString( IDS_RE_UNRELIABLE );
				CString	str;
				(void)str.LoadString( IDS_RE_PASTED );
				str += _T(".");
				strError += str;
				AfxMessageBox( strError, MB_ICONHAND );
			}
		}
		CloseClipboard();
	}
}

///////////////////////////////////////////////////////////////////////////////////////
// Specific Functions

bool
CWhitePage::TakeSender( CStringA strHeader )
{
	char*	pchPair = (char*)AfxGetMainWnd()->SendMessage( WM_GET_SENDER, 0, (LPARAM)strHeader.GetBuffer() );
	if	( !pchPair )
		return	false;

	CStringA strPair( pchPair );
	delete[]	pchPair;

	int	x = 0;
	x = strPair.Find( "\n" );
	CStringA strFrom = strPair.Left( x );
	strPair.Delete( 0, x+1 );
	x = strPair.Find( "\n" );
	CStringA strDomain = strPair.Left( x );

	if	( !strFrom.IsEmpty() && !strDomain.IsEmpty() ){
		x = strDomain.Find( '@' );
		if	( x >= 0 )
			strDomain = strDomain.Mid( x );
		int	nDot = 0;
		int	nch = strDomain.GetLength();
		for	( x = nch-1; x >= 0; x-- )
			if	( strDomain[x] == '.' ){
				if	( ++nDot >= 2 ){
					if	( nch-x > 6 )
						break;
				}
			}
			else if	( strDomain[x] == '@' ){
				x++;
				break;
			}
		strDomain = strDomain.Mid( x );
		AddSender( (CString)strFrom, (CString)strDomain );

		return	true;
	}
	else
		return	false;
}

void
CWhitePage::AddSender( CString strFrom, CString strDomain )
{
	CListCtrl*	pList = (CListCtrl*)GetDlgItem( IDC_LIST_WHITE );
	int	iRow, nRow = pList->GetItemCount();

	for	( iRow = 0; iRow < nRow; iRow++ ){
		CString	strFromList   = pList->GetItemText( iRow, 0 );
		CString	strDomainList = pList->GetItemText( iRow, 1 );

		if	( strFrom   == strFromList &&
			  strDomain == strDomainList )
			return;
	}

	pList->InsertItem(    iRow, _T("") );
	pList->SetItemText(   iRow, 0, strFrom );
	pList->SetItemText(   iRow, 1, strDomain );
	pList->SetItemState(  iRow, LVIS_SELECTED, LVIS_SELECTED );
	pList->EnsureVisible( iRow, FALSE );

	pList->SetColumnWidth( 0, LVSCW_AUTOSIZE );
	pList->SetColumnWidth( 1, LVSCW_AUTOSIZE );
	pList->SetColumnWidth( 2, LVSCW_AUTOSIZE_USEHEADER );

	SortSender();

	pList->SetFocus();

	GetDlgItem( IDC_BUTTON_WHITE_DEL )->EnableWindow( TRUE );
}

void
CWhitePage::SortSender( void )
{
	CListCtrl*	pList = (CListCtrl*)GetDlgItem( IDC_LIST_WHITE );
	CHeaderCtrl*	pHeader = pList->GetHeaderCtrl();

	int	nColumn = pHeader->GetItemCount();
	int	iColumn = 0;
	int	iOrder  = 0;

	for	( iColumn = 0; iColumn < nColumn; iColumn++ ){
		HDITEM	header = {};
		header.mask = HDI_FORMAT;
		pHeader->GetItem( iColumn, &header );
		if	( header.fmt & HDF_SORTUP ){
			iOrder  = 1;
			break;
		}
		else if	( header.fmt & HDF_SORTDOWN ){
			iOrder  = 2;
			break;
		}
	}

	int	nRow = pList->GetItemCount();
	for	( int iRow = 0; iRow < nRow; iRow++ )
		pList->SetItemData( iRow, iRow );

	SortArg	arg = {};

	arg.pList   = pList;
	arg.iColumn = iColumn;
	arg.iOrder  = iOrder;
	pList->SortItems( CompareSender, (DWORD_PTR)&arg );

	for	( int iRow = 0; iRow < nRow; iRow++ )
		pList->SetItemData( iRow, iRow );
}

int	CALLBACK
CWhitePage::CompareSender( LPARAM lItem1, LPARAM lItem2, LPARAM lParam )
{
	int	nReturn = 0;

	if	( lItem1 == lItem2 )
		return	0;

	SortArg* pArg = (SortArg*)lParam;
	int	iRow1 = (int)lItem1;
	int	iRow2 = (int)lItem2;

	CListCtrl* pList = pArg->pList;
	int	iColumn  = pArg->iColumn;
	int	iOrder   = pArg->iOrder;
	CString	strArg1 = pList->GetItemText( iRow1, iColumn );
	CString	strArg2 = pList->GetItemText( iRow2, iColumn );

	{
		int	x1, x2, x1a, x2a;

		CString	strAddr1 = strArg1;
		CString	strAddr2 = strArg2;

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

			if	( str1 > str2 ){
				nReturn = +1;
				break;
			}
			else if	( str1 < str2 ){
				nReturn = -1;
				break;
			}
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
	}

	if	( iOrder == 2 )
		nReturn	= -nReturn;

	return	nReturn;
}
