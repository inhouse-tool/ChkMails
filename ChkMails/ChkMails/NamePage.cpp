// NamePage.cpp : implementation file
//

#include "pch.h"
#include "ChkMails.h"
#include "NamePage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#ifdef	UNICODE
#define	strcpy_s	wcscpy_s
#endif//UNICODE

#define	TID_SELCHANGE	1	// to get the last result of the selection

IMPLEMENT_DYNAMIC( CNamePage, CPropertyPage )

///////////////////////////////////////////////////////////////////////////////////////
// Constructor

CNamePage::CNamePage( void )
	: CPropertyPage( IDD_NAME )
{
}

///////////////////////////////////////////////////////////////////////////////////////
// Overridden Functions

BOOL
CNamePage::OnInitDialog( void )
{
	CPropertyPage::OnInitDialog();

	// Get a fixed-pitch font for the editboxes.

	CFont*	pFont = GetCommonFont();
	GetDlgItem( IDC_EDIT_NAME_DOMAIN )->SetFont( pFont, FALSE );

	// Initialize the domain list.

	CListCtrl*	pList;
	CStringArray	saList;
	DWORD		dwExStyle;
	
	pList = (CListCtrl*)GetDlgItem( IDC_LIST_NAME );
	pList->SetFont( pFont, FALSE );

	dwExStyle = pList->GetExtendedStyle();
	dwExStyle |= LVS_EX_FULLROWSELECT;
	dwExStyle |= LVS_EX_GRIDLINES;
	pList->SetExtendedStyle( dwExStyle );

	// Insert columns into the name list.
	{
		int	iColumn = 0;
		pList->InsertColumn( iColumn++, _T("Word in alias"),  LVCFMT_LEFT,   200 );
		pList->InsertColumn( iColumn++, _T("Word in domain"), LVCFMT_LEFT,   200 );
	}

	// Add names.
	{
		SortName( saList, m_strNames );
		INT_PTR	nRow = saList.GetCount();

		for	( int iRow = 0; iRow < nRow; iRow++ ){
			CString	str = saList[iRow];
			int	x = str.Find( '\t' );
			pList->InsertItem(  iRow,    str.Left( x ) );
			pList->SetItemText( iRow, 1, str.Mid( x+1 ) );
		}
	}

	// Adjust the size of columns.

	pList->SetColumnWidth( 0, LVSCW_AUTOSIZE_USEHEADER );
	pList->SetColumnWidth( 1, LVSCW_AUTOSIZE_USEHEADER );
	pList->Invalidate( TRUE );

	return	TRUE;
}

void
CNamePage::OnOK( void )
{
	m_strNames.Empty();

	CListCtrl*	pList = (CListCtrl*)GetDlgItem( IDC_LIST_NAME );
	int	nRow = pList->GetItemCount();
	for	( int iRow = 0; iRow < nRow; iRow++ ){
		CString	str, strRow;
		str = pList->GetItemText( iRow, 0 );
		strRow += str + _T("\t");
		str = pList->GetItemText( iRow, 1 );
		strRow += str + _T("\n");
		m_strNames += strRow;
	}
}

BOOL
CNamePage::PreTranslateMessage( MSG* pMsg )
{
	if	( pMsg->message == WM_KEYDOWN &&
		  pMsg->wParam  == VK_RETURN ){
		CWnd*	pwnd = GetFocus();
		if	( pwnd == GetDlgItem( IDC_EDIT_NAME_DOMAIN ) ){
			PostMessage( WM_COMMAND, MAKEWPARAM( IDC_BUTTON_NAME_ADD,  BN_CLICKED ), NULL );
			return	TRUE;
		}
	}

	return	CPropertyPage::PreTranslateMessage( pMsg );
}

///////////////////////////////////////////////////////////////////////////////////////
// Message Handlers

BEGIN_MESSAGE_MAP( CNamePage, CPropertyPage )
	ON_WM_DROPFILES()
	ON_WM_TIMER()
	ON_BN_CLICKED( IDC_BUTTON_NAME_ADD,  OnButtonAdd )
	ON_BN_CLICKED( IDC_BUTTON_NAME_DEL,  OnButtonDelete )
	ON_CONTROL_RANGE( EN_CHANGE, IDC_EDIT_NAME_ALIAS, IDC_EDIT_NAME_DOMAIN, OnChangeEdit )
	ON_NOTIFY( LVN_ITEMCHANGED, IDC_LIST_NAME,  OnChangeList )
	ON_COMMAND( ID_EDIT_PASTE, OnEditPaste )
END_MESSAGE_MAP()

void
CNamePage::OnDropFiles( HDROP hDropInfo )
{
	int	nDone = 0;

	UINT	nFile =
	DragQueryFile( hDropInfo, -1, NULL, 0 );

	for	( UINT iFile = 0; iFile < nFile; iFile++ ){
		TCHAR	szFile[MAX_PATH];
		DragQueryFile( hDropInfo, iFile, szFile, _countof( szFile ) );
		CString	strNames = LoadText( szFile );
		AddNames( strNames );
	}
}

void
CNamePage::OnTimer( UINT_PTR nIDEvent )
{
	if	( nIDEvent == TID_SELCHANGE ){
		KillTimer( nIDEvent );
		OnSelectItem();
	}
	else
		CDialog::OnTimer( nIDEvent );
}

void
CNamePage::OnButtonAdd( void )
{
	CString	strAlias;
	GetDlgItem( IDC_EDIT_NAME_ALIAS  )->GetWindowText( strAlias );
	CString	strDomain;
	GetDlgItem( IDC_EDIT_NAME_DOMAIN )->GetWindowText( strDomain );

	CListCtrl*	pList = (CListCtrl*)GetDlgItem( IDC_LIST_NAME );
	
	int	iRow, nRow = pList->GetItemCount();
	for	( iRow = 0; iRow < nRow; iRow++ ){
		CString	str0 = pList->GetItemText( iRow, 0 );
		CString	str1 = pList->GetItemText( iRow, 1 );
		if	( str0 == strAlias && str1 == strDomain )
			return;
	}
	pList->InsertItem(    iRow, strAlias );
	pList->SetItemText(   iRow, 1, strDomain );
	pList->SetColumnWidth( 0, LVSCW_AUTOSIZE_USEHEADER );
	pList->SetColumnWidth( 1, LVSCW_AUTOSIZE_USEHEADER );
	pList->EnsureVisible( iRow, FALSE );
	pList->SetItemState(  iRow, LVNI_SELECTED, LVNI_SELECTED );

	GetDlgItem( IDC_EDIT_NAME_ALIAS  )->SetWindowText( _T("") );
	GetDlgItem( IDC_EDIT_NAME_DOMAIN )->SetWindowText( _T("") );
}

void
CNamePage::OnButtonDelete( void )
{
	CListCtrl*	pList = (CListCtrl*)GetDlgItem( IDC_LIST_NAME );

	POSITION	pos = pList->GetFirstSelectedItemPosition();
	while	( pos ){
		int	iRow = pList->GetNextSelectedItem( pos );
		pList->DeleteItem( iRow );
	}
}

void
CNamePage::OnChangeEdit( UINT uID )
{
	CString	strAlias;
	GetDlgItem( IDC_EDIT_NAME_ALIAS )->GetWindowText( strAlias );
	CString	strDomain;
	GetDlgItem( IDC_EDIT_NAME_DOMAIN )->GetWindowText( strDomain );
	BOOL	bEnable = TRUE;
	if	( strAlias.IsEmpty() || strDomain.IsEmpty() )
		bEnable = FALSE;
	GetDlgItem( IDC_BUTTON_NAME_ADD )->EnableWindow( bEnable );
}

void
CNamePage::OnChangeList( NMHDR* pNMHDR, LRESULT* pResult )
{
	SetTimer( TID_SELCHANGE, 0, NULL );
	*pResult = 0;
}

void
CNamePage::OnEditPaste( void )
{
	if	( OpenClipboard() ){
		HGLOBAL	hMem = GetClipboardData( CF_UNICODETEXT );
		if	( hMem ){
			void*	pMem = GlobalLock( hMem );
			CString strNames( (TCHAR*)pMem );
			GlobalUnlock( hMem );

			AddNames( strNames );
		}
		CloseClipboard();
	}
}

///////////////////////////////////////////////////////////////////////////////////////
// Specific Functions

void
CNamePage::OnSelectItem( void )
{
	CListCtrl*	pList = (CListCtrl*)GetDlgItem( IDC_LIST_NAME );
	int	iRow = pList->GetNextItem( -1, LVNI_ALL | LVNI_SELECTED );
	GetDlgItem( IDC_BUTTON_NAME_DEL  )->EnableWindow( ( iRow >= 0 )? TRUE: FALSE );
}

void
CNamePage::SortName( CStringArray& saList, CString strList )
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

void
CNamePage::AddNames( CString strNames )
{
	strNames += _T("\n");
	int	iLine = 0;
	for	( ;; ){
		CString	strName = strNames.Tokenize( _T("\n"), iLine );
		if	( iLine < 0 )
			break;
		int	x = 0;
		CString	strAlias = strName.Tokenize( _T("\t"), x );
		CString	strDomain = strName.Tokenize( _T("\t\r"), x );
		GetDlgItem( IDC_EDIT_NAME_ALIAS  )->SetWindowText( strAlias );
		GetDlgItem( IDC_EDIT_NAME_DOMAIN )->SetWindowText( strDomain );
		OnButtonAdd();
	}
	GetDlgItem( IDC_EDIT_NAME_ALIAS  )->SetWindowText( _T("") );
	GetDlgItem( IDC_EDIT_NAME_DOMAIN )->SetWindowText( _T("") );
}

CString
CNamePage::LoadText( CString strFile )
{
	CString	strLines;
	enum	Encode{
		unknown,
		ASCII,
		ShiftJIS,
		UTF8,
		UTF16BE,
		UTF16LE,
		UTF32BE,
		UTF32LE
	}	eEncode;
	DWORD	cbBOM = 0;
	DWORD	dwBOM = 0;

	CFile	f;
	if	( f.Open( strFile, CFile::modeRead | CFile::shareExclusive ) ){
		DWORD	cbData = (DWORD)f.GetLength();
		BYTE*	pbData = new BYTE[cbData+1];
		f.Read( pbData, cbData );
		pbData[cbData] = '\0';

		BYTE*	pb = pbData;

		// If the data is empty, encoding is unknown.

		if	( cbData == 0 )
			eEncode = unknown;

		// If the BOM exists, encoding is written in BOM.

		else if	( pb[0] == 0xef && pb[1] == 0xbb && pb[2] == 0xbf ){
			cbBOM = 3;
			memcpy( &dwBOM, pb, cbBOM );
			eEncode = UTF8;
		}
		else if	( pb[0] == 0xff && pb[1] == 0xfe ){
			cbBOM = 2;
			memcpy( &dwBOM, pb, cbBOM );
			eEncode = UTF16LE;
		}
		else if	( pb[0] == 0xfe && pb[1] == 0xff ){
			cbBOM = 2;
			memcpy( &dwBOM, pb, cbBOM );
			eEncode = UTF16BE;
		}
		else if	( pb[0] == 0x00 && pb[1] == 0x00 && pb[2] == 0xfe && pb[3] == 0xff ){
			cbBOM = 4;
			memcpy( &dwBOM, pb, cbBOM );
			eEncode = UTF32BE;
		}
		else if	( pb[0] == 0xff && pb[1] == 0xfe && pb[2] == 0x00 && pb[3] == 0x00 ){
			cbBOM = 4;
			memcpy( &dwBOM, pb, cbBOM );
			eEncode = UTF32LE;
		}
		// If there's no BOM, judge encoding from some data.

		else{
			Encode	aeEncode[16] = {};
			int	nEncode = 0;

			for	( QWORD cb = cbData; cb > 0; ){
				if	( *pb < 0x7f ){
					cb--;
					pb++;
				}
				else if	( pb[0] >= 0xc2 && pb[0] <= 0xdf &&
					  pb[1] >= 0x80 && pb[1] <= 0xbf ){
					aeEncode[nEncode++] = UTF8;	// 11bit code
					cb -= 2;
					pb += 2;
				}
				else if	( pb[0] >= 0xe0 && pb[0] <= 0xef &&
					  pb[1] >= 0x80 && pb[1] <= 0xbf &&
					  pb[2] >= 0x80 && pb[2] <= 0xbf ){
					aeEncode[nEncode++] = UTF8;	// 16bit code
					cb -= 3;
					pb += 3;
				}
				else if	( pb[0] >= 0xf0 && pb[0] <= 0xf4 &&
					  pb[1] >= 0x80 && pb[1] <= 0xbf &&
					  pb[2] >= 0x80 && pb[2] <= 0xbf &&
					  pb[3] >= 0x80 && pb[3] <= 0xbf ){
					aeEncode[nEncode++] = UTF8;	// 21bit code
					cb -= 4;
					pb += 4;
				}
				else if	( ( ( pb[0] >= 0x81 && pb[0] <= 0x9f ) ||
					    ( pb[0] >= 0xe0 && pb[0] <= 0xef )    ) &&
					  ( ( pb[1] >= 0x40 && pb[1] <= 0x7e ) ||
					    ( pb[0] >= 0x80 && pb[0] <= 0xfc )    )    ){
					aeEncode[nEncode++] = ShiftJIS;
					cb -= 2;
					pb += 2;
				}
				else{
					aeEncode[nEncode++] = unknown;
					break;
				}
				if	( nEncode >= _countof( aeEncode ) )
					break;
			}
			{
				int	i;
				for	( i = 1; i < nEncode; i++ )
					if	( aeEncode[i] != aeEncode[0] )
						break;	
				eEncode =	( nEncode == 0 )?	ASCII:
						( i >= nEncode )?	aeEncode[0]:
									unknown;
			}
		}

		CHAR*	pbText = (CHAR*)( pbData + cbBOM );

		if	( eEncode == ASCII || eEncode == ShiftJIS ){
			int	cwch =
			MultiByteToWideChar( CP_ACP, 0, (char*)pbData, -1, NULL, 0 );
			WCHAR*	pwch = new WCHAR[cwch+1];
			MultiByteToWideChar( CP_ACP, 0, (char*)pbData, -1, pwch, cwch );
			pwch[cwch] = '\0';
			strLines = pwch;
			delete	[]pwch;
		}
		else if	( eEncode == UTF8 ){
			int	cwch =
			MultiByteToWideChar( CP_UTF8, 0, pbText, -1, NULL, 0 );
			WCHAR*	pwch = new WCHAR[cwch+1];
			MultiByteToWideChar( CP_UTF8, 0, pbText, -1, pwch, cwch );
			pwch[cwch] = '\0';
			strLines = pwch;
			delete	[]pwch;
		}
		else if	( eEncode == UTF16LE )
			strLines = (WCHAR*)pbText;

		delete []pbData;
	}

	return	strLines;
}
