// CharMap.cpp : implementation file
//

#include "pch.h"
#include "CharMap.h"

///////////////////////////////////////////////////////////////////////////////////////
// Constructor

CCharMap::CCharMap( void )
{
	m_nChar = 0;
	m_strFontName = SelectDefaultFont();
	SetFontName( (LPCTSTR)m_strFontName );
}

///////////////////////////////////////////////////////////////////////////////////////
// Interface Functions for Application

void
CCharMap::SetFontName( LPCTSTR lpszFontName )
{
	m_strFontName = 
		( !lpszFontName || !*lpszFontName )?	SelectDefaultFont():
							lpszFontName;

	m_uaCodes.RemoveAll();
	m_nChar = 0;
	m_strSLNG.Empty();

	CFont	font;
	{
		LOGFONT	lf = {};
		lf.lfCharSet    = DEFAULT_CHARSET;
		wcscpy_s( lf.lfFaceName, (LPCTSTR)m_strFontName );
		font.CreateFontIndirect( &lf );
	}

	HDC	hDC = GetDC( NULL );
	if	( hDC ){
		SelectObject( hDC, font );

		ReadCMAP( hDC );
		ReadSLNG( hDC );

		ReleaseDC( NULL, hDC );
	}
}

CString
CCharMap::GetFontName( void )
{
	return	m_strFontName;
}

bool
CCharMap::IsMapped( UINT uCode )
{
	INT_PTR	n = m_uaCodes.GetCount();

	if	( n ){
		INT_PTR	i;
		for	( i = 0; i < n; i++ )
			if	( m_uaCodes[i] >= uCode )
				break;
		if	( i >= n )
			return	false;
		else if	( i%2 == 0 )
			if	( m_uaCodes[i] != uCode )
				return	false;
	}

	return	true;
}

int
CCharMap::GetCount( void )
{
	return	m_nChar;
}

UINT
CCharMap::GetLowerBound( void )
{
	return	m_uaCodes[0];
}

UINT
CCharMap::GetUpperBound( void )
{
	return	m_uaCodes[m_uaCodes.GetUpperBound()];
}

CString
CCharMap::GetSLNG( void )
{
	return	m_strSLNG;
}

///////////////////////////////////////////////////////////////////////////////////////
// Internal Functions of this class

CString
CCharMap::SelectDefaultFont( void )
{
	// Get the default language.
	// See https://learn.microsoft.com/en-us/windows-hardware/manufacture/desktop/available-language-packs-for-windows
	// cf. https://en.wikipedia.org/wiki/ISO_15924

	WCHAR	achLocale[LOCALE_NAME_MAX_LENGTH] = {};
	GetUserDefaultLocaleName( achLocale, _countof( achLocale ) );
	CString	strLocale = achLocale;
	
	// List of Font used in Windows
	// See https://learn.microsoft.com/en-us/windows/apps/design/globalizing/loc-international-fonts

	static	struct{
		WCHAR*	pszLocale;
		WCHAR*	pszFontName;
	}		astTable[] = {
		{ L"am-ET",		L"Ebrima"		},
		{ L"as-IN",		L"Nirmala UI"		},
		{ L"bn-BD",		L"Nirmala UI"		},
		{ L"bn-IN",		L"Nirmala UI"		},
		{ L"chr-CHER-US",	L"Gadugi"		},
		{ L"zh-CN",		L"Microsoft YaHei"	},
		{ L"zh-HK",		L"Microsoft JhengHei"	},
		{ L"zh-TW",		L"Microsoft JhengHei"	},
		{ L"gu-IN",		L"Nirmala UI"		},
		{ L"hi-IN",		L"Nirmala UI"		},
		{ L"ja-JP",		L"Yu Gothic UI"		},
		{ L"kn-IN",		L"Nirmala UI"		},
		{ L"km-KH",		L"Leelawadee UI"	},
		{ L"kok-IN",		L"Nirmala UI"		},
		{ L"ko-KR",		L"Malgun Gothic"	},
		{ L"ml-IN",		L"Nirmala UI"		},
		{ L"mr-IN",		L"Nirmala UI"		},
		{ L"ne-NP",		L"Nirmala UI"		},
		{ L"or-IN",		L"Nirmala UI"		},
		{ L"pa-IN",		L"Nirmala UI"		},
		{ L"si-LK",		L"Nirmala UI"		},
		{ L"ta-IN",		L"Nirmala UI"		},
		{ L"te-IN",		L"Nirmala UI"		},
		{ L"th-TH",		L"Leelawadee UI"	},
		{ L"ti-ET",		L"Ebrima"		},
		{ NULL,			L"Segoe UI"		}
	};

	int	i;
	for	( i = 0; astTable[i].pszLocale; i++ )
		if	( strLocale == astTable[i].pszLocale )
			break;

	return	astTable[i].pszFontName;
}

// See https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6cmap.html

typedef	struct	_cmap{
	WORD	version;
	WORD	numberSubtables;
}	cmap;

typedef	struct	_subtable{
	WORD	platformID;
	WORD	platformSpecificID;
	DWORD	offset;
}	subtable;

typedef	struct	_format4{
	WORD	format;
	WORD	length;
	WORD	language;
	WORD	segCountX2;
	WORD	searchRange;
	WORD	entrySelector;
	WORD	rangeShift;
//	WORD	endCode[segCount];
//	WORD	reservedPad;
//	WORD	startCode[segCount];
//	WORD	idDelta[segCount];
//	WORD	idRangeOffset[segCount];
//	WORD	glyphIndexArray[variable];
}	format4;

typedef	struct	_format12{
	WORD	format;
	WORD	reserved;
	DWORD	length;
	DWORD	language;
	DWORD	nGroups;
}	format12;

typedef	struct	_format12g{
	DWORD	startCharCode;
	DWORD	endCharCode;
	DWORD	startGlyphCode;
}	format12g;

bool
CCharMap::ReadCMAP( HDC hDC )
{
	// Read 'cmap' of the font.

	DWORD	dwTable = 'pamc';// reversed 'cmap'
	DWORD	cbData = GetFontData( hDC, dwTable, 0, NULL, 0 );
	DWORD	dwError = GetLastError();
	if	( !cbData )
		return	false;

	CUIntArray	uaFormat;
	CUIntArray	uaOffset;

	BYTE*	pbData = new BYTE[cbData];
	GetFontData( hDC, dwTable, 0, pbData, cbData );

	// Get number of subtables.

	cmap*	pcmap = (cmap*)pbData;
	WORD	numberSubtables = Swap16( pcmap->numberSubtables );
	subtable*	psub = (subtable*)( pcmap+1 );

	// Follow each subtable.

	for	( int i = 0 ; i < numberSubtables; i++, psub++ ){
		DWORD	offset = Swap32( psub->offset );
		WORD*	pwFormat = (WORD*)( pbData+offset );
		WORD	wFormat = Swap16( *pwFormat );
	//	TRACE( "%08x: format %d\n", offset, wFormat );

		// 'format 4': Append it.

		if	( wFormat == 4 ){
			uaFormat.Add( wFormat );
			uaOffset.Add( offset );
		}

		// 'format 12': Discard all 'format 4' and take this one.

		if	( wFormat == 12 ){
			uaFormat.RemoveAll();
			uaOffset.RemoveAll();
				
			uaFormat.Add( wFormat );
			uaOffset.Add( offset );
		}
	}

	// Take each 'format 4' or one 'format 12'.

	DWORD	dwEndCharCode = 0;
	for	( INT_PTR i = 0 ; i < 1; i++ ){
		WORD	wFormat  = uaFormat[i];
		DWORD	dwOffset = uaOffset[i];

		// 'format 4': Two-byte Encoding

		if	( wFormat == 4 ){
			format4* f4 = (format4*)( pbData+dwOffset );
			WORD	length   = Swap16( f4->length );
			WORD	segCount = Swap16( f4->segCountX2 ) / 2;
			WORD*	pEndCode = (WORD*)( f4+1 );
			WORD*	pStartCode = pEndCode+segCount+1;
			for	( int i = 0; i < segCount; i++ ){
				WORD	startCode = Swap16( pStartCode[i] );
				WORD	endCode   = Swap16(   pEndCode[i] );
				if	( startCode == 0xffff )
					break;
				if	( startCode == dwEndCharCode+1 )
					m_uaCodes[m_uaCodes.GetUpperBound()] = endCode;
				else{
					m_uaCodes.Add( startCode );
					m_uaCodes.Add( endCode );
				}
				dwEndCharCode = endCode;
			}
		}

		// 'format 12': Segmented coverage

		else if	( wFormat == 12 ){
			format12* f12 = (format12*)( pbData+dwOffset );
			DWORD	length   = Swap32( f12->length );
			DWORD	nGroups  = Swap32( f12->nGroups );
			format12g* f12g = (format12g*)( f12+1 );
			for	( DWORD i = 0; i < nGroups; i++ ){
				DWORD	startCharCode  = Swap32( f12g->startCharCode );
				DWORD	endCharCode    = Swap32( f12g->endCharCode );
				if	( startCharCode == dwEndCharCode+1 )
					m_uaCodes[m_uaCodes.GetUpperBound()] = endCharCode;
				else{
					m_uaCodes.Add( startCharCode );
					m_uaCodes.Add( endCharCode );
				}
				dwEndCharCode = endCharCode;
				f12g++;
			}
		}
	}

	delete[]	pbData;

	// Calculate total number of characters mapped.

	INT_PTR	n = m_uaCodes.GetCount();
	for	( int i = 0; i < n; i += 2 )
		m_nChar += m_uaCodes[i+1] - m_uaCodes[i] +1;

	return	true;
}

// See https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6meta.html

typedef	struct	_meta{
	DWORD	version;
	DWORD	flags;
	DWORD	dataOffset;
	DWORD	numDataMaps;
}	meta;

typedef	struct	_metadata{
	DWORD	tag;
	DWORD	dataOffset;
	DWORD	dataLength;
}	metadata;

bool
CCharMap::ReadSLNG( HDC hDC )
{
	// Read 'meta' of the font.

	DWORD	dwTable = 'atem';// reversed 'meta'
	DWORD	cbData = GetFontData( hDC, dwTable, 0, NULL, 0 );
	DWORD	dwError = GetLastError();
	if	( (int)cbData <= 0 )
		return	false;

	BYTE*	pbData = new BYTE[cbData];
	GetFontData( hDC, dwTable, 0, pbData, cbData );

	// Get number of data maps.

	meta*	pmeta = (meta*)pbData;
	DWORD	dataOffset  = Swap32( pmeta->dataOffset );
	DWORD	numDataMaps = Swap32( pmeta->numDataMaps );
	metadata*	pdata = (metadata*)( pmeta+1 );

	// Follow each data map.

	for	( DWORD i = 0; i < numDataMaps; i++ ){
		DWORD	tag = Swap32( pdata->tag );
		DWORD	dataOffset = Swap32( pdata->dataOffset );
		DWORD	dataLength = Swap32( pdata->dataLength );

		// 'slng' supported languages: Take this representation.

		if	( tag == 'slng' ){
			char*	pch = new char[dataLength+1];
			memcpy( pch, pbData+dataOffset, dataLength );
			pch[dataLength] = '\0';
			m_strSLNG = pch;
			delete[]	pch;
			m_strSLNG.Replace( L", ", L"," );
			m_strSLNG.Replace( L",", L" " );
			// see https://www.unicode.org/iso15924/iso15924-codes.html
			// cf. https://en.wikipedia.org/wiki/ISO_15924
		}

		// 'dlng' design languages: Do not use this since 'slng' is more useful.

		else if	( tag == 'dlng' ){
			char*	pch = new char[dataLength+1];
			memcpy( pch, pbData+dataOffset, dataLength );
			pch[dataLength] = '\0';
		//	TRACE( "dlng: %s\n", pch );
			delete[]	pch;
		}
		pdata++;
	}

	delete[]	pbData;

	// Just confirm absence of 'ltag'. ( That's why we must read 'slng' out of 'meta'. )

	dwTable = 'gatl';// reversed 'ltag'
	cbData = GetFontData( hDC, dwTable, 0, NULL, 0 );
	dwError = GetLastError();
	if	( (int)cbData > 0 ){
		BYTE*	pbData = new BYTE[cbData];
		GetFontData( hDC, dwTable, 0, pbData, cbData );
		TRACE( "found 'ltag'\n" );
		delete[]	pbData;
	}

	return	true;
}

WORD
CCharMap::Swap16( WORD w )
{
	WORD	b0 = ( w & 0xff00 ) >> 8;
	WORD	b1 = ( w & 0x00ff ) << 8;

	return	b0 | b1;
}

DWORD
CCharMap::Swap32( DWORD dw )
{
	DWORD	b0 = ( dw & 0xff000000 ) >> 24;
	DWORD	b1 = ( dw & 0x00ff0000 ) >>  8;
	DWORD	b2 = ( dw & 0x0000ff00 ) <<  8;
	DWORD	b3 = ( dw & 0x000000ff ) << 24;

	return	b0 | b1 | b2 | b3;
}






///////////////////////////////////////////////////////////////////////////////////////
// Supplemental Dialog for test and apply 'CCharMap' class.

#define	WM_PAINT_MAP	(WM_APP+1)

///////////////////////////////////////////////////////////////////////////////////////
// Message Handlers ( of the Map window )

BEGIN_MESSAGE_MAP( CCharMapWnd, CWnd )
	ON_WM_PAINT()
END_MESSAGE_MAP()

void
CCharMapWnd::OnPaint( void )
{
	PAINTSTRUCT ps;
	CDC*	pDC = BeginPaint( &ps );

	GetParent()->SendMessage( WM_PAINT_MAP, (WPARAM)&ps.rcPaint, (LPARAM)pDC );

	EndPaint( &ps );
}

///////////////////////////////////////////////////////////////////////////////////////
// Constants

// Fonts

#define	FONTNAME_FIX	L"Consolas"
#define	FONTNAME_VAR	L"Arial"
#define	FONTSIZE_DLG	9

// DLU: Dialog Units

#define	NMARGIN		  5
#define	CXLABEL		 20
#define	CXFONT		100
#define	CXCODES		 65
#define	CXCHARS		 60
#define	CXBUTTON	 50
#define	CYBUTTON	 12
#define	CXCHARACTER	190
#define	CYCHARACTER	 30
#define	CXSCROLLBAR	  5

#define	MIN_WIDTH	320
#define	MIN_HEIGHT	342

// Dialog Item IDs ( avoid IDOK, IDCANCEL, IDR_MAINFRAME, etc. )

#define	IDC_COMBO_FONT		20
#define	IDC_STATIC_CODES	22
#define	IDC_STATIC_CHARS	21
#define	IDC_EDIT_LANG		23
#define	IDC_STATIC_MAP		24
#define	IDC_EDIT_CHAR		25
#define	IDC_EDIT_CODE		26
#define	IDC_SCROLLBAR		27

// Other constants

#define	UNICODE_MIN	0x00000
#define	UNICODE_MAX	0x33500	// Unicode 17.0: CJK Unified Ideographs Extension J

///////////////////////////////////////////////////////////////////////////////////////
// Constructor & Destructor

CCharMapDlg::CCharMapDlg( void )
{
	m_pbTemp = NULL;
	m_iOffset = 0;
	m_nUnit  = 0;
	m_uCode  = (UINT)-1;
	m_xCode  = 0;
	m_xMargin  = 0;
	m_yEdit  = 0;
	m_cyEdit = 0;
	m_nRow   = 16;
	m_crBack     = RGB( 240, 240, 240 );
	m_crMapped   = RGB( 255, 255, 255 );
	m_crUnmapped = RGB( 200, 200, 200 );
	m_crSelected = RGB( 255,   0,   0 );

	CreateModal();
}

CCharMapDlg::~CCharMapDlg( void )
{
	if	( m_pbTemp )
		delete[]	m_pbTemp;
}

///////////////////////////////////////////////////////////////////////////////////////
// Overridden Functions

#if	0
//#include "resource.h"
#else
#define	IDR_MAINFRAME	128
#endif

BOOL
CCharMapDlg::OnInitDialog( void )
{
	CDialog::OnInitDialog();

#ifdef	IDR_MAINFRAME
	HICON	hIcon = AfxGetApp()->LoadIcon( IDR_MAINFRAME );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );
#endif//IDR_MAINFRAME

	m_wndMap.SubclassDlgItem( IDC_STATIC_MAP, this );

	CRect	rectBar;
	m_wndMap.GetClientRect( &rectBar );
	m_wndMap.MapWindowPoints( this, &rectBar );
	rectBar.left = rectBar.right;
	rectBar.right  += 20;
	m_wndScrollbar.Create( WS_CHILD | WS_VISIBLE | SBS_VERT, rectBar, this, IDC_SCROLLBAR );
	SetScrollBar();

	LOGFONT	lf = {};
	EnumFontFamiliesEx( GetDC()->m_hDC, &lf, EnumFontProc, (LPARAM)this, 0 );

	CComboBox*	pCombo = (CComboBox*)GetDlgItem( IDC_COMBO_FONT );
	int	n = pCombo->GetCount();
	CString	strLast;
	for	( int i = 0; i < n; i++ ){
		CString	str;
		pCombo->GetLBText( i, str );
		if	( strLast.IsEmpty() )
			;
		else if	( str.Left( strLast.GetLength() ) == strLast ){
			if	( str.GetLength() == strLast.GetLength()+3 ){
				CString	strTail = str.Right( 3 );
				if	( strTail == L" UI" )
					continue;
			}
			pCombo->DeleteString( i );
			i--;
			n--;
		//	TRACE( L"Omit %s after %s\n", str, strLast );
			continue;
		}
		strLast = str;
	}
	pCombo->SetMinVisibleItems( 40 );
	{
		pCombo->InsertString( 0, L"(Default)" );
		int	iFont = pCombo->FindStringExact( -1, m_charmap.GetFontName() );
		pCombo->SetCurSel( iFont );
	}

	GetLayout();
	GetWindowRect( &m_rectOriginal );

	LoadCharacterNames();

	FollowFont();

	m_wndMap.SetFocus();

	return	FALSE;
}

BOOL
CCharMapDlg::PreTranslateMessage( MSG* pMsg )
{
	if	( pMsg->message == WM_KEYDOWN ){
		CWnd*	pwnd = GetFocus();

		if	( GetKeyState( VK_CONTROL ) & 0x0080 ){
			if	( pMsg->wParam == 'C' ){
				if	( pwnd == &m_wndMap ){
					if	( m_uCode != (UINT)-1 )
						CopyText( CodeToStr( m_uCode ) );
					return	TRUE;
				}
			}
			else if	( pMsg->wParam == 'F' ){
				if	( pwnd != GetDlgItem( IDC_COMBO_FONT ) ){
					OnEditStart();
					return	TRUE;
				}
			}
		}
		else if	( pMsg->wParam == VK_F3 ){
			OnEditFind();
			return	TRUE;
		}

		if	( pwnd == &m_wndMap ){
			if	( pMsg->wParam == VK_TAB )
				;
			else if	( OnKeyOnMap( pMsg->wParam ) )
				return	TRUE;
		}
		else if	( pwnd == GetDlgItem( IDC_EDIT_CHAR ) ){
			if	( pMsg->wParam == VK_RETURN ||
				  pMsg->wParam == VK_ESCAPE ){
				OnEditEnd( pMsg->wParam );
				return	TRUE;
			}
		}
	}

	return	CDialog::PreTranslateMessage( pMsg );
}

///////////////////////////////////////////////////////////////////////////////////////
// Message Handlers

BEGIN_MESSAGE_MAP( CCharMapDlg, CDialog )
	ON_WM_GETMINMAXINFO()
	ON_WM_MOUSEWHEEL()
	ON_WM_SIZE()
	ON_WM_VSCROLL()
	ON_CONTROL( CBN_SELCHANGE, IDC_COMBO_FONT, OnSelChange )
	ON_STN_CLICKED( IDC_STATIC_MAP, OnClickMap )
	ON_MESSAGE( WM_PAINT_MAP, OnPaintMap )
END_MESSAGE_MAP()

void
CCharMapDlg::OnGetMinMaxInfo( MINMAXINFO* lpMMI )
{
	lpMMI->ptMinTrackSize.x = m_rectOriginal.Width();
	lpMMI->ptMinTrackSize.y = m_rectOriginal.Height();
}

BOOL
CCharMapDlg::OnMouseWheel( UINT nFlags, short zDelta, CPoint pt )
{
	// Get the location of the mouse event.

	CRect	rectBar;
	m_wndScrollbar.GetClientRect( &rectBar );
	int	cxBar = rectBar.Width();

	CRect	rectMap;
	m_wndMap.GetClientRect( &rectMap );
	m_wndMap.MapWindowPoints( this, &rectMap );
	rectMap.right += cxBar;
	::MapWindowPoints( NULL, m_hWnd, &pt, 1 );

	// Make a margin on the top of the map.

	rectMap.top += m_nUnit;

	// Ctrl+Wheel: Ignore it. ( Resize nothing. )

	if	( nFlags & MK_CONTROL )
		;

	// Not in the map: Ignore it.

	else if	( !rectMap.PtInRect( pt ) )
		;

	// Wheel down: Scroll down.

	else if	( zDelta < 0 )
		OnVScroll( ( nFlags & MK_SHIFT )? SB_LINEDOWN: SB_PAGEDOWN, 0, NULL );

	// Wheel up: Scroll up.

	else if	( m_iOffset > 0 )
		OnVScroll( ( nFlags & MK_SHIFT )? SB_LINEUP:   SB_PAGEUP,   0, NULL );

	return	0;
}

void
CCharMapDlg::OnSize( UINT nType, int cx, int cy )
{
	CDialog::OnSize( nType, cx, cy );

	if	( nType == SIZE_MINIMIZED  )
		return;

	if	( IsWindow( m_wndMap.GetSafeHwnd() ) ){
		SetLayout();
		SetFont();
	}
}

void
CCharMapDlg::OnVScroll( UINT nSBCode, UINT nPos, CScrollBar* pScrollBar )
{
	SCROLLINFO info = {};
	info.cbSize = sizeof( info );
	info.fMask = SIF_ALL;
	m_wndScrollbar.GetScrollInfo( &info );

	if	( nSBCode == SB_LINEUP ){
		if	( m_iOffset > info.nMin )
			m_iOffset -= 0x10;
		else
			return;
	}
	else if	( nSBCode == SB_LINEDOWN ){
		if	( m_iOffset + (int)info.nPage < info.nMax  )
			m_iOffset += 0x10;
		else
			return;
	}
	else if	( nSBCode == SB_PAGEUP ){
		if	( m_iOffset > 0 )
			m_iOffset -= info.nPage;
	}
	else if	( nSBCode == SB_PAGEDOWN ){
		if	( m_iOffset + (int)info.nPage < info.nMax  )
			m_iOffset += info.nPage;
		else
			return;
	}
	else if	( nSBCode == SB_THUMBPOSITION ){
		m_iOffset = ( info.nTrackPos / info.nPage ) * info.nPage;
		if	( info.nTrackPos % info.nPage )
			m_iOffset += info.nPage;
	}
	else if	( nSBCode == SB_THUMBTRACK ){
		m_iOffset = ( info.nTrackPos / info.nPage ) * info.nPage;
		if	( info.nTrackPos % info.nPage )
			m_iOffset += info.nPage;
	}
	else
		return;

	CRect	rect;
	m_wndMap.GetClientRect( &rect );
	rect.top += m_nUnit;
	m_wndMap.InvalidateRect( &rect, FALSE );

	SetScrollBar();

	if	( pScrollBar )
		m_wndMap.SetFocus();
}

void
CCharMapDlg::OnSelChange( void )
{
	CComboBox*	pCombo = (CComboBox*)GetDlgItem( IDC_COMBO_FONT );
	int	iFont = pCombo->GetCurSel();
	if	( iFont >= 0 ){
		CString	str;
		if	( iFont == 0 ){
			m_charmap.SetFontName( NULL );
			iFont = pCombo->FindStringExact( -1, m_charmap.GetFontName() );
			pCombo->SetCurSel( iFont );
		}
		pCombo->GetLBText( iFont, str );
		m_charmap.SetFontName( str );
		FollowFont();
		m_wndMap.Invalidate( TRUE );
	}
}

void
CCharMapDlg::OnClickMap( void )
{
	CPoint	pt;
	GetCursorPos( &pt );
	GetDesktopWindow()->MapWindowPoints( &m_wndMap, &pt, 1 );
	CRect	rectMap;
	m_wndMap.GetClientRect( rectMap );
	int	x = rectMap.right - ( m_nUnit*0x10 );
	int	y = rectMap.top   +   m_nUnit;

	UINT	uCode = m_uCode;
	m_uCode = (UINT)-1;

	if	( pt.x >= x && pt.y >= y ){
		x = pt.x - x;
		y = pt.y - y;
		x /= m_nUnit;
		y /= m_nUnit;
		UINT	uCode = (y*0x10) + x;
		if	( uCode < (UINT)m_nRow*0x10 ){
			uCode += m_iOffset;
			m_uCode = uCode;
		}
	}

	if	( m_uCode == uCode )
		m_uCode = (UINT)-1;
	m_wndMap.Invalidate( FALSE );
	m_wndMap.SetFocus();

	SelectCode( m_uCode );
}

LRESULT
CCharMapDlg::OnPaintMap( WPARAM wParam, LPARAM lParam )
{
	CDC*	pDC = (CDC*)lParam;
	CRect	rectUpdate = *(RECT*)wParam;
	CRect	rectMap;
	m_wndMap.GetClientRect( rectMap );

	pDC->SetBkMode( OPAQUE );
	pDC->SetBkColor( m_crBack );
	pDC->SelectObject( &m_fontFix );

	CRect	rectRow;
	rectRow = rectMap;
	rectRow.left  = rectRow.right - m_nUnit;
	rectRow.bottom = rectRow.top + m_nUnit;
	for	( int i = 0x0f; i >= 0x00; i-- ){
		CString	str;
		str.Format( L"%X", i );
		pDC->DrawText( str, &rectRow, DT_SINGLELINE | DT_CENTER | DT_VCENTER | DT_NOCLIP );
		rectRow.left  -= m_nUnit;
		rectRow.right -= m_nUnit;
	}

	rectRow = rectMap;
	rectRow.top   += m_nUnit;
	rectRow.bottom = rectRow.top  + m_nUnit;
	rectRow.right -= m_nUnit * 0x10;
	{
		int	cyBaseline = 2;
		rectRow.top    += cyBaseline;
		rectRow.bottom += cyBaseline;
	}
	for	( int i = 0; i < m_nRow; i++ ){
		CString	str;
		int	nOffset = m_iOffset + (i*16);
		if	( nOffset < UNICODE_MAX )
			str.Format( L"%05X", nOffset );
		else
			str = L"      ";
		pDC->DrawText( str, &rectRow, DT_SINGLELINE | DT_CENTER | DT_VCENTER | DT_NOCLIP );
		rectRow.top    += m_nUnit;
		rectRow.bottom += m_nUnit;
	}

	pDC->SetBkMode( TRANSPARENT );
	pDC->SelectObject( &m_fontSel );

	rectRow = rectMap;
	rectRow.top += m_nUnit;
	rectRow.bottom = rectRow.top   +  m_nUnit;
	rectRow.left   = rectRow.right - (m_nUnit*16);
	for	( int i = 0; i < m_nRow; i++ ){
		CRect	rect = rectRow;
		rect.right = rect.left +  m_nUnit;
		UINT	uCode = m_iOffset + (i*16);
		for	( int x = 0; x < 16; x++ ){
			rect.left += 1;
			rect.top  += 1;
			if	( uCode < UNICODE_MAX ){
				pDC->FillSolidRect( &rect, m_charmap.IsMapped( uCode )? m_crMapped: m_crUnmapped );
				CString	str = CodeToStr( uCode );
				pDC->DrawText( str, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER );
				if	( uCode == m_uCode )
					pDC->FrameRect( &rect, &CBrush( m_crSelected ) );
			}
			else
				pDC->FillSolidRect( &rect, m_crUnmapped );
			rect.left -= 1;
			rect.top  -= 1;
			rect.left  += m_nUnit;
			rect.right += m_nUnit;
			uCode++;
		}
		rectRow.top    += m_nUnit;
		rectRow.bottom += m_nUnit;
	}

	return	0;
}

///////////////////////////////////////////////////////////////////////////////////////
// Specific Functions

#define	TEMP_ALLOC_SIZE	4096

void
CCharMapDlg::CreateModal( void )
{
	// Make a Dialog Template memory area.

	DWORD	cb = 0;
	cb += FillDialog(  NULL, NULL );
	cb += FillStatic(  NULL, NULL );
	cb += FillCombo(   NULL, NULL );
	cb += FillButton(  NULL, NULL );
	cb += FillEdit(    NULL, NULL );
	m_pbTemp = new BYTE[cb];
	memset( m_pbTemp, 0, cb );

	// Fill the Dialog template.

	BYTE*	pb = m_pbTemp;
	DLGTEMPLATE*	pTemp = (DLGTEMPLATE*)pb;
	pb += FillDialog(  pb, pTemp );
	pb += FillStatic(  pb, pTemp );
	pb += FillCombo(   pb, pTemp );
	pb += FillButton(  pb, pTemp );
	pb += FillEdit(    pb, pTemp );

	// Set the Dialog template.

	InitModalIndirect( (DLGTEMPLATE*)m_pbTemp, NULL, NULL );
}

DWORD
CCharMapDlg::FillDialog( BYTE* pb, DLGTEMPLATE* pTemp )
{
	BYTE*	pbStart;
	bool	bFill;

	// Is it turn to fill the template or turn to calculate the size of the template?

	if	( pb ){
		pbStart = pb;
		bFill = true;
	}
	else{
		pbStart = pb = new BYTE[TEMP_ALLOC_SIZE];
		bFill = false;
		pTemp = (DLGTEMPLATE*)pb;
		memset( pTemp, 0, sizeof( *pTemp ) );
	}

	// Setup the dialog template with minimum size.

	int	nStatic = 4;
	int	nCombo  = 1;
	int	nButton = 1;
	int	nEdit   = 3;
	pTemp->style = WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | DS_SETFONT;
	pTemp->dwExtendedStyle = 0;
	pTemp->cdit = nStatic + nCombo + nButton + nEdit;
	pTemp->x  =   0;
	pTemp->y  =   0;
	pTemp->cx = MIN_WIDTH;
	pTemp->cy = MIN_HEIGHT;

	// Get the caption and message text.

	CString	strCaption = L"Character Map";

	WORD*	pw = (WORD*)( pTemp+1 );

	// Menu array
	*pw++ = 0;			// No Menu

	// Class array
	*pw++ = 0;			// No Class

	// Title array
	DWORD	cb = ( strCaption.GetLength()+1 ) * (int)sizeof( WCHAR );
	memcpy( pw, (LPCTSTR)strCaption, cb );
	pw += strCaption.GetLength()+1;

	// Font size
	*pw++ = FONTSIZE_DLG;

	// Font name
	int	cchFontName = _countof( FONTNAME_VAR );
	memcpy( pw, FONTNAME_VAR, cchFontName * sizeof( WCHAR ) );
	pw += cchFontName;

	pb = Align( (BYTE*)pw );

	DWORD	cbFilled = (DWORD)( pb - pbStart );
	if	( !bFill )
		delete[]	pbStart;

	return	cbFilled;
}

DWORD
CCharMapDlg::FillStatic( BYTE* pb, DLGTEMPLATE* pTemp )
{
	BYTE*	pbStart;
	bool	bFill;

	// Is it turn to fill the template or turn to calculate the size of the template?

	if	( pb ){
		pbStart = pb;
		bFill = true;
	}
	else{
		pbStart = pb = new BYTE[TEMP_ALLOC_SIZE];
		bFill = false;
		pTemp = (DLGTEMPLATE*)pb;
		memset( pTemp, 0, sizeof( *pTemp ) );
	}

	CString	strText[4] = { L"Font :", L"", L"", L"" };
	for	( int i = 0; i < 4; i++ ){
		DLGITEMTEMPLATE*	pItem = (DLGITEMTEMPLATE*)pb;

		int	yButton  = pTemp->cy - CYBUTTON - NMARGIN;
		int	yBaseLineAdjust = 2;

		pItem->style = WS_CHILD | WS_VISIBLE | SS_LEFT;
		pItem->dwExtendedStyle = 0;
		if	( i == 0 ){
			pItem->x  = NMARGIN;
			pItem->y  = NMARGIN + yBaseLineAdjust;
			pItem->cx = CXLABEL;
			pItem->cy = CYBUTTON;
			pItem->id = IDC_STATIC;
		}
		else if	( i == 1 ){
			pItem->x  = NMARGIN + CXLABEL + NMARGIN + CXFONT + NMARGIN;
			pItem->y  = NMARGIN + yBaseLineAdjust;
			pItem->cx = CXCODES;
			pItem->cy = CYBUTTON;
			pItem->id = IDC_STATIC_CODES;
		}
		else if	( i == 2 ){
			pItem->style |= SS_RIGHT;
			pItem->x  = pTemp->cx - NMARGIN - CXBUTTON - NMARGIN - CXCHARS;
			pItem->y  = NMARGIN + yBaseLineAdjust;
			pItem->cx = CXCHARS;
			pItem->cy = CYBUTTON;
			pItem->id = IDC_STATIC_CHARS;
		}
		else if	( i == 3 ){
			pItem->style |= WS_TABSTOP | SS_NOTIFY | SS_OWNERDRAW;
			pItem->x  = NMARGIN;
			pItem->y  = NMARGIN + CYBUTTON + NMARGIN + CYBUTTON;
			pItem->cx = pTemp->cx - NMARGIN - CXSCROLLBAR - NMARGIN - NMARGIN;
			pItem->cy = pTemp->cy - (NMARGIN*5) - (CYBUTTON*2) - CYCHARACTER - NMARGIN;
			pItem->id = IDC_STATIC_MAP;
		}

		WORD*	pw = (WORD*)( pItem+1 );

		// Class array:
		*pw++ = 0xffff;				// ORDINAL of
		*pw++ = 0x0082;				// 'Static Control`

		// Resource array:
		DWORD	cb = ( strText[i].GetLength()+1 ) * (int)sizeof( WCHAR );
		memcpy( pw, (LPCTSTR)strText[i], cb );	// TEXT of the given prompt
		pw += strText[i].GetLength()+1;

		// Title array:
		*pw++ = '\0';				// TEXT of empty
		pb = Align( (BYTE*)pw );
	}

	DWORD	cbFilled = (DWORD)( pb - pbStart );
	if	( !bFill )
		delete[]	pbStart;

	return	cbFilled;
}

DWORD
CCharMapDlg::FillCombo( BYTE* pb, DLGTEMPLATE* pTemp )
{
	BYTE*	pbStart;
	bool	bFill;

	// Is it turn to fill the template or turn to calculate the size of the template?

	if	( pb ){
		pbStart = pb;
		bFill = true;
	}
	else{
		pbStart = pb = new BYTE[TEMP_ALLOC_SIZE];
		bFill = false;
		pTemp = (DLGTEMPLATE*)pb;
		memset( pTemp, 0, sizeof( *pTemp ) );
	}

	{
		DLGITEMTEMPLATE*	pItem = (DLGITEMTEMPLATE*)pb;

		pItem->style = WS_CHILD | WS_VISIBLE | WS_TABSTOP  | WS_VSCROLL | CBS_DROPDOWNLIST | CBS_SORT;
		pItem->dwExtendedStyle = 0;
		pItem->x  = NMARGIN + CXLABEL;
		pItem->y  = NMARGIN;
		pItem->cx = CXFONT;
		pItem->cy = CYBUTTON;
		pItem->id = IDC_COMBO_FONT;
		WORD*	pw = (WORD*)( pItem+1 );

		// Class array:
		*pw++ = 0xffff;				// ORDINAL of
		*pw++ = 0x0085;				// 'Combo box'

		// Resource array:
		*pw++ = '\0';				// No Resource/Text

		// Title array:
		*pw++ = '\0';				// TEXT of empty

		pb = Align( (BYTE*)pw );
	}

	DWORD	cbFilled = (DWORD)( pb - pbStart );
	if	( !bFill )
		delete[]	pbStart;

	return	cbFilled;
}

DWORD
CCharMapDlg::FillButton( BYTE* pb, DLGTEMPLATE* pTemp )
{
	BYTE*	pbStart;
	bool	bFill;

	// Is it turn to fill the template or turn to calculate the size of the template?

	if	( pb ){
		pbStart = pb;
		bFill = true;
	}
	else{
		pbStart = pb = new BYTE[TEMP_ALLOC_SIZE];
		bFill = false;
		pTemp = (DLGTEMPLATE*)pb;
		memset( pTemp, 0, sizeof( *pTemp ) );
	}

	// Select the buttons.

	int	x = pTemp->cx - ( ( CXBUTTON + NMARGIN ) );
	{
		DLGITEMTEMPLATE*	pItem = (DLGITEMTEMPLATE*)pb;

		pItem->style = WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON;
		pItem->dwExtendedStyle = 0;
		pItem->x  = x;
		pItem->y  = NMARGIN;
		pItem->cx = CXBUTTON;
		pItem->cy = CYBUTTON;
		pItem->id = IDOK;
		WORD*	pw = (WORD*)( pItem+1 );

		// Class array:
		*pw++ = 0xffff;				// ORDINAL of
		*pw++ = 0x0080;				// 'Button Control'

		// Resource array:
		CString	strButton = L"OK";
		DWORD	cb = ( strButton.GetLength()+1 ) * (int)sizeof( WCHAR );
		memcpy( pw, (LPCTSTR)strButton, cb );	// TEXT of the button
		pw += strButton.GetLength()+1;

		// Title array:
		*pw++ = '\0';				// TEXT of empty

		pb = Align( (BYTE*)pw );
		x += pItem->cx + NMARGIN;
	}

	DWORD	cbFilled = (DWORD)( pb - pbStart );
	if	( !bFill )
		delete[]	pbStart;

	return	cbFilled;
}

DWORD
CCharMapDlg::FillEdit( BYTE* pb, DLGTEMPLATE* pTemp )
{
	BYTE*	pbStart;
	bool	bFill;

	// Is it turn to fill the template or turn to calculate the size of the template?

	if	( pb ){
		pbStart = pb;
		bFill = true;
	}
	else{
		pbStart = pb = new BYTE[TEMP_ALLOC_SIZE];
		bFill = false;
		pTemp = (DLGTEMPLATE*)pb;
		memset( pTemp, 0, sizeof( *pTemp ) );
	}

	for	( int i = 0; i < 3; i++ ){
		DLGITEMTEMPLATE*	pItem = (DLGITEMTEMPLATE*)pb;

		int	yEdit  = pTemp->cy - CYCHARACTER - NMARGIN;

		pItem->style = WS_CHILD | WS_VISIBLE | WS_TABSTOP;
		pItem->dwExtendedStyle = 0;
		if	( i == 0 ){
			pItem->style |= ES_READONLY | ES_AUTOHSCROLL;
			pItem->x  = NMARGIN + CXLABEL + 3;	// +3 to move behind the letters in the combobox
			pItem->y  = NMARGIN + CYBUTTON + NMARGIN;
			pItem->cx = pTemp->cx - NMARGIN;
			pItem->cy = CYBUTTON;
			pItem->id = IDC_EDIT_LANG;
		}
		else if	( i == 1 ){
			pItem->style |= ES_READONLY | ES_AUTOHSCROLL | ES_RIGHT | ES_WANTRETURN;
			pItem->x  = NMARGIN;
			pItem->y  = yEdit;
			pItem->cx = CXCHARACTER;
			pItem->cy = CYCHARACTER;
			pItem->id = IDC_EDIT_CHAR;
		}
		else if	( i == 2 ){
			pItem->style |= ES_READONLY | ES_AUTOVSCROLL | ES_MULTILINE | ES_NOHIDESEL;
			pItem->x  = NMARGIN + CXCHARACTER + NMARGIN;
			pItem->y  = yEdit;
			pItem->cx = pTemp->cx - pItem->x - NMARGIN;
			pItem->cy = CYCHARACTER;
			pItem->id = IDC_EDIT_CODE;
		}

		WORD*	pw = (WORD*)( pItem+1 );

		// Class array:
		*pw++ = 0xffff;				// ORDINAL of
		*pw++ = 0x0081;				// 'Edit`

		// Resource array:
		*pw++ = '\0';				// No Resource/Text

		// Title array:
		*pw++ = '\0';				// TEXT of empty
		pb = Align( (BYTE*)pw );
	}

	DWORD	cbFilled = (DWORD)( pb - pbStart );
	if	( !bFill )
		delete[]	pbStart;

	return	cbFilled;
}

BYTE*
CCharMapDlg::Align( BYTE* pb )
{
	if	( (QWORD)pb % sizeof( DWORD ) )
		pb += sizeof( DWORD ) - ( (QWORD)pb % sizeof( DWORD ) );
	return	pb;
}

void
CCharMapDlg::GetLayout( void )
{
	CWnd*	pwnd;
	CRect	rect;

	CRect	rectClient;
	GetClientRect( &rectClient );

	pwnd = GetDlgItem( IDC_EDIT_LANG );
	pwnd->GetClientRect( &rect );
	m_cyEdit = rect.Height();

	pwnd = GetDlgItem( IDC_EDIT_CHAR );
	pwnd->GetClientRect( &rect );
	pwnd->MapWindowPoints( this, &rect );
	m_xMargin = rect.left - rectClient.left;
	m_yEdit = rectClient.bottom - rect.top;

	pwnd = GetDlgItem( IDC_EDIT_CODE );
	pwnd->GetClientRect( &rect );
	pwnd->MapWindowPoints( this, &rect );
	m_xCode = rect.left - rectClient.left;

	SetLayout();
}

void
CCharMapDlg::SetLayout( void )
{
	CWnd*	pwnd;
	CRect	rect;

	CRect	rectClient;
	GetClientRect( &rectClient );

	// Place the controls.

	pwnd = GetDlgItem( IDC_EDIT_LANG );
	pwnd->GetClientRect( &rect );
	pwnd->MapWindowPoints( this, &rect );
	rect.right = rectClient.right - 10;
	pwnd->SetWindowPos( NULL, 0, 0, rect.Width(), m_cyEdit, SWP_NOZORDER | SWP_NOMOVE );

	pwnd = GetDlgItem( IDC_EDIT_CHAR );
	pwnd->GetClientRect( &rect );
	pwnd->MapWindowPoints( this, &rect );
	rect.bottom = rectClient.bottom - m_xMargin;
	rect.top    = rectClient.bottom - m_yEdit;
	int	y = rect.top;
	pwnd->SetWindowPos( NULL, m_xMargin, y, rect.Width(), rect.Height(), SWP_NOZORDER );

	pwnd = GetDlgItem( IDC_EDIT_CODE );
	pwnd->GetClientRect( &rect );
	pwnd->MapWindowPoints( this, &rect );
	rect.right  = rectClient.right  - m_xMargin;
	rect.bottom = rectClient.bottom - m_xMargin;
	rect.top    = y;
	pwnd->SetWindowPos( NULL, m_xCode, y, rect.Width(), rect.Height(), SWP_NOZORDER );

	// Get the original width of the scrollbar.

	CRect	rectBar;
	pwnd = &m_wndScrollbar;
	m_wndScrollbar.GetClientRect( &rectBar );
	m_wndScrollbar.MapWindowPoints( this, &rectBar );
	int	cxBar = rectBar.Width();

	// Place the map on the left side of the scrollbar.

	CRect	rectMap;
	pwnd = &m_wndMap;
	m_wndMap.GetClientRect( &rectMap );
	m_wndMap.MapWindowPoints( this, &rectMap );
	rectMap.right  = rectClient.right - cxBar - m_xMargin;
	rectMap.bottom = y - m_xMargin;
	m_wndMap.SetWindowPos( NULL, 0, 0, rectMap.Width(), rectMap.Height(), SWP_NOZORDER | SWP_NOMOVE );

	// Fill the map with the background color.

	CDC*	pDC = GetDC();
	pDC->FillSolidRect( &rectMap, m_crBack );
	m_wndMap.Invalidate( TRUE );

	// Calculate the size of cell and the number of rows.

	int	nMargin = 2;
	m_nUnit = rectMap.Width() / ( 0x10 + nMargin );
	m_nRow  = ( rectMap.Height() / m_nUnit ) -1;
	if	( m_uCode != (UINT)-1 ){
		UINT	cbPage = m_nRow * 0x10;
		while	( m_uCode >= (UINT)m_iOffset + cbPage )
			m_iOffset += cbPage;
	}

	// Place the scrollbar on the right side of the map.

	rectBar.left   = rectMap.right;
	rectBar.right  = rectBar.left + cxBar;
	rectBar.top    = rectMap.top +   m_nUnit;
	rectBar.bottom = rectMap.top + ( m_nUnit * (m_nRow+1) );
	m_wndScrollbar.SetWindowPos( NULL, rectBar.left, rectBar.top, rectBar.Width(), rectBar.Height(), SWP_NOZORDER );

	SetScrollBar();

	// Calculate size of the fixed font according to the side margin of the map.

	CRect	rectCode = rectMap;
	rectCode.bottom = rectCode.top + m_nUnit;
	rectCode.right -= m_nUnit * 0x10;

	int	cyFixedFont = m_nUnit;
	LOGFONT	lf = {};
	wcscpy_s( lf.lfFaceName, FONTNAME_FIX );
	lf.lfCharSet        = ANSI_CHARSET;
	lf.lfPitchAndFamily = FIXED_PITCH;

	for	( ;; ){
		lf.lfHeight         = cyFixedFont;
		m_fontFix.DeleteObject();
		m_fontFix.CreateFontIndirect( &lf );
		pDC->SelectObject( m_fontFix );
		rect = rectCode;
		pDC->DrawText( L"00000 ", &rect, DT_CALCRECT );
		if	( rect.Width() <= rectCode.Width() )
			break;
		cyFixedFont--;
	}
}

void
CCharMapDlg::SetScrollBar( void )
{
	SCROLLINFO	info;

	info.cbSize    = sizeof( info );
	info.fMask     = SIF_ALL;
	info.nMin      = UNICODE_MIN;
	info.nMax      = UNICODE_MAX;
	info.nPage     = m_nRow * 0x10;
	info.nPos      = m_iOffset;
	info.nTrackPos = m_iOffset;

	if	( (int)m_iOffset < 0 )
		m_iOffset = 0;
	else if	( m_iOffset >= info.nMax - (int)info.nPage )
		m_iOffset = info.nMax - info.nPage;

	m_wndScrollbar.SetScrollInfo( &info );
}

void
CCharMapDlg::FollowFont( void )
{
	CString	str;

	str = m_charmap.GetSLNG();
	GetDlgItem( IDC_EDIT_LANG )->SetWindowText( (LPCTSTR)str );
	GetDlgItem( IDC_EDIT_LANG )->ShowWindow( SW_SHOW );

	str.Format( L"U+%04X - U+%04X", m_charmap.GetLowerBound(), m_charmap.GetUpperBound() );
	GetDlgItem( IDC_STATIC_CODES )->SetWindowText( (LPCTSTR)str );

	str.Format( L"%d characters", m_charmap.GetCount() );
	GetDlgItem( IDC_STATIC_CHARS )->SetWindowText( (LPCTSTR)str );

	SetFont();
}

void
CCharMapDlg::SetFont( void )
{
	LOGFONT	lf = {};
	lf.lfHeight         = m_nUnit-5;
	wcscpy_s( lf.lfFaceName, (LPCTSTR)m_charmap.GetFontName() );
	m_fontSel.DeleteObject();
	m_fontSel.CreateFontIndirect( &lf );

	CFont	font;
	lf.lfHeight         = 50;
	font.CreateFontIndirect( &lf );
	((CEdit*)GetDlgItem( IDC_EDIT_CHAR ))->SetFont( &font );
}

BOOL
CCharMapDlg::OnKeyOnMap( WPARAM wParam )
{
	bool	bNewCode = false;
	UINT	uCode = m_uCode;
	UINT	cbPage = m_nRow * 0x10;
	bool	bInsight = ( m_uCode >= (UINT)m_iOffset && m_uCode < (UINT)m_iOffset + cbPage );

	switch	( wParam ){
		case	VK_RETURN:
			m_uCode = (UINT)m_iOffset;
			bNewCode = true;
			break;
		case	VK_ESCAPE:
			m_uCode = (UINT)-1;
			bNewCode = ( m_uCode != uCode );
			break;
		case	VK_LEFT:
			if	( m_uCode == (UINT)-1 )
				;
			else if	( !bInsight )
				;
			else if	( (int)( m_uCode-0x1 ) >= UNICODE_MIN ){
				m_uCode -= 0x1;
				bNewCode = true;
			}
			break;
		case	VK_RIGHT:
			if	( m_uCode == (UINT)-1 )
				;
			else if	( !bInsight )
				;
			else if	( (int)( m_uCode+0x1 ) <  UNICODE_MAX ){
				m_uCode += 0x1;
				bNewCode = true;
			}
			break;
		case	VK_UP:
			if	( m_uCode == (UINT)-1 )
				OnVScroll( SB_LINEUP, 0, NULL );
			else if	( !bInsight )
				;
			else if	( m_uCode == UNICODE_MIN )
				;
			else{
				m_uCode -= 0x10;
				if	( (int)m_uCode < UNICODE_MIN )
					m_uCode = UNICODE_MIN;
				bNewCode = true;
			}
			break;
		case	VK_DOWN:
			if	( m_uCode == (UINT)-1 )
				OnVScroll( SB_LINEDOWN, 0, NULL );
			else if	( !bInsight )
				;
			else if	( (int)( m_uCode+0x10 ) <  UNICODE_MAX ){
				m_uCode += 0x10;
				bNewCode = true;
			}
			break;
		case	VK_PRIOR:
			if	( m_uCode == (UINT)-1 )
				OnVScroll( SB_PAGEUP, 0, NULL );
			else if	( !bInsight )
				OnVScroll( SB_PAGEUP, 0, NULL );
			else if	( m_uCode == UNICODE_MIN )
				;
			else{
				m_uCode -= cbPage;
				if	( (int)m_uCode < UNICODE_MIN )
					m_uCode = UNICODE_MIN;
				bNewCode = true;
			}
			break;
		case	VK_NEXT:
			if	( m_uCode == (UINT)-1 )
				OnVScroll( SB_PAGEDOWN, 0, NULL );
			else if	( !bInsight )
				OnVScroll( SB_PAGEDOWN, 0, NULL );
			else if	( (int)( m_uCode+cbPage ) <  UNICODE_MAX ){
				m_uCode += cbPage;
				bNewCode = true;
			}
			break;
		case	VK_HOME:
			m_uCode = uCode = m_charmap.GetLowerBound();
			m_iOffset = m_uCode / 0x10 * 0x10;
			m_wndMap.Invalidate( FALSE );
			bNewCode = true;
			break;
		case	VK_END:
			m_uCode = uCode = m_charmap.GetUpperBound();
			m_iOffset = m_uCode / 0x10 * 0x10;
			m_wndMap.Invalidate( FALSE );
			bNewCode = true;
			break;
	}

	if	( bNewCode ){
		SelectCode( m_uCode );
		if	( m_uCode == (UINT)-1 )
			InvalidateCode(   uCode );
		else if	( ( m_uCode - m_iOffset ) / cbPage != ( uCode - m_iOffset ) / cbPage )
			OnVScroll( ( m_uCode > uCode )? SB_PAGEDOWN: SB_PAGEUP, 0, NULL );
		else{
			InvalidateCode(   uCode );
			InvalidateCode( m_uCode );
		}
	}

	return	TRUE;
}

void
CCharMapDlg::OnEditStart( void )
{
	CEdit*	pEdit = (CEdit*)GetDlgItem( IDC_EDIT_CHAR );
	pEdit->SetWindowText( L"" );
	pEdit->SetReadOnly( FALSE );
	pEdit->SetFocus();

	GetDlgItem( IDC_EDIT_CODE )->SetWindowText( L"\x25c0 Enter a character or a code\r\n    to search" );
}

void
CCharMapDlg::OnEditEnd( WPARAM wParam )
{
	CString	strEntered;
	GetDlgItem( IDC_EDIT_CHAR )->GetWindowText( strEntered );

	if	( wParam == VK_RETURN ){
		m_strToFind = strEntered;
		OnEditFind();
	}

	m_wndMap.SetFocus();
}

void
CCharMapDlg::OnEditFind( void )
{
	UINT	uCode = (UINT)-1;
	int	n = m_strToFind.GetLength();

	// No characters:

	if	( n == 0 )
		return;

	// One character:

	if	( n == 1 ){
		uCode = m_strToFind[0];
		m_strToFind.Empty();
	}

	// Surrogate pair:

	else if	( n == 2 && IsSurrogate( m_strToFind ) ){
		uCode = StrToCode( m_strToFind );
		m_strToFind.Empty();
	}

	// Others:

	else{
		// Check if the text representing a hex code.
		// ( Do not use isxdigit() macro since its assertion fails. )
		int	i;
		for	( i = 0; i < n; i++ ){
			WCHAR	ch = m_strToFind[i];
			if	( ch >= '0' && ch <= '9' )
				continue;
			else if	( ch >= 'A' && ch <= 'F' )
				continue;
			else if	( ch >= 'a' && ch <= 'f' )
				continue;
			else
				break;
		}

		// Hex code:

		if	( i >= n ){
			UINT	u = wcstoul( (LPCTSTR)m_strToFind, NULL, 16 );
			if	( u >= UNICODE_MIN && u <= UNICODE_MAX )
				uCode = u;
			else
				TRACE( "out of range\n" );
			m_strToFind.Empty();
		}

		// Other words:

		else{
			CString	strToFind = m_strToFind;
			strToFind.Remove( '"' );
			CString	strCharNames = CString( m_strCharacterNames );
			strCharNames.MakeLower();
			strToFind.MakeLower();
			UINT	uFrom = m_uCode;
			CString	strFrom;
			strFrom.Format( L"%04x;", uFrom );
			if	( uFrom )
				strFrom.Insert( 0, L"\n" );
			int	i = strCharNames.Find( strFrom );
			i = ( i < 0 )? 0: strCharNames.Find( L"\n", i+1 );

			i = strCharNames.Find( strToFind, i );
			if	( i >= 0 ){
				int	x = i;
				if	( x != 0 ){
					for	( ; strCharNames[x] != '\n'; x-- )
						if	( x == 0 )
							break;
					if	( x != 0 )
						x++;
				}
				uCode = wcstoul( ((LPCTSTR)strCharNames)+x, NULL, 16 );
			}
			else
				m_uCode = (UINT)-1;
		}
	}

	// Hit: Select the code hit.

	m_uCode = uCode;
	if	( m_uCode != (UINT)-1 )
		m_iOffset = m_uCode & 0xffff0;
	SelectCode( m_uCode );
	m_wndMap.Invalidate( FALSE );

	// Select the word hit.

	if	( !m_strToFind.IsEmpty() && m_uCode != (UINT)-1 ){
		CString	strToFind = m_strToFind;
		CEdit*	pEdit = (CEdit*)GetDlgItem( IDC_EDIT_CODE );
		CString	strEdit;
		pEdit->GetWindowText( strEdit );
		strEdit.MakeLower();
		strToFind.MakeLower();
		strToFind.Remove( '"' );
		int	i = strEdit.Find( strToFind );
		pEdit->SetSel( i, i+strToFind.GetLength() );
	}
}

void
CCharMapDlg::SelectCode( UINT uCode )
{
	if	( uCode == (UINT)-1 ){
		SetDlgItemText( IDC_EDIT_CHAR, L"" );
		SetDlgItemText( IDC_EDIT_CODE, L"" );
	}
	else{
		CWnd*	pwnd;
		CString	str;

		str = CodeToStr( uCode );
		pwnd = GetDlgItem( IDC_EDIT_CHAR );
		pwnd->SetWindowText( str );
		pwnd->UpdateWindow();

		str.Format( L"U+%04X\r\n%s", uCode, (LPCTSTR)GetCharName( uCode ) );
		pwnd = GetDlgItem( IDC_EDIT_CODE );
		pwnd->SetWindowText( str );
		pwnd->UpdateWindow();
	}

	((CEdit*)GetDlgItem( IDC_EDIT_CHAR ))->SetReadOnly( TRUE );;
}

void
CCharMapDlg::InvalidateCode( UINT uCode )
{
	CRect	rectMap;
	m_wndMap.GetClientRect( &rectMap );

	uCode -= m_iOffset;
	int	x = uCode % 0x10;
	int	y = ( uCode / 0x10 ) % m_nRow;
	x = 0x10 - x;
	y += 1;
	x *= m_nUnit;
	y *= m_nUnit;
	x = rectMap.right - x;
	y = rectMap.top   + y;

	CRect	rectCode( x, y, x+m_nUnit, y+m_nUnit );
	m_wndMap.InvalidateRect( &rectCode, FALSE );
}

CString
CCharMapDlg::CodeToStr( UINT uCode )
{
	TCHAR	ts[4] = {};
	if	( uCode < 0x10000 )
		ts[0] = uCode;
	else{
		ts[0] = ( ( uCode - 0x10000 ) / 0x400 ) + 0xd800;
		ts[1] = ( ( uCode - 0x10000 ) % 0x400 ) + 0xdc00;
	}

	return	ts;
}

UINT
CCharMapDlg::StrToCode( CString strCode )
{
	UINT	uCode = strCode[0];

	if	( IsSurrogate( strCode ) ){
		uCode =  ( strCode[0] & 0x3ff ) << 10;
		uCode |= ( strCode[1] & 0x3ff );
		uCode += 0x10000;
	}

	return	uCode;
}

bool
CCharMapDlg::IsSurrogate( CString strCode )
{
	if	( strCode[0] >= 0xd800 && strCode[0] <= 0xdbff &&
		  strCode[1] >= 0xdc00 && strCode[0] <= 0xdfff )
		return	true;
	else
		return	false;
}

void
CCharMapDlg::CopyText( CString strText )
{
	if	( OpenClipboard() ){
		EmptyClipboard();
		long	l = ( strText.GetLength()+1 ) * sizeof( TCHAR );
		HGLOBAL	hMem = GlobalAlloc( GMEM_MOVEABLE, l );
		if	( hMem ){
			void*	pMem = GlobalLock( hMem );
			if	( pMem ){
				memset( pMem, 0, l );
				memcpy( pMem, (LPCTSTR)strText, ( sizeof( TCHAR ) * strText.GetLength() ) );
			}
			GlobalUnlock( hMem );
			SetClipboardData( CF_UNICODETEXT, hMem );
		}
		CloseClipboard();
	}
}

void
CCharMapDlg::LoadCharacterNames( void )
{
	CString	strFile = AfxGetApp()->m_pszHelpFilePath;
	int	x = strFile.ReverseFind( '\\' );
	strFile = strFile.Left( x ) + L"\\UnicodeData.txt";

	// Get the above file from
	// https://www.unicode.org/Public/UNIDATA/

	CStringA strLines;
	CFile	f;
	if	( f.Open( strFile, CFile::modeRead | CFile::shareDenyNone ) ){
		UINT	cch = (UINT)f.GetLength();
		if	( cch > 0 ){
			char*	pch = new char[cch];
			f.Read( pch, cch );
			strLines = pch;
			delete[]	pch;
		}
	}

	if	( !strLines.IsEmpty() ){
		CStringA strNames;
		int	iToken = 0;
		while	( iToken >= 0 ){
			CStringA strLine = strLines.Tokenize( "\r\n", iToken );
			int	x0 = strLine.Find( ';' ) +1;
			int	x1 = strLine.Find( ';', x0+1 );
			CStringA strName = strLine.Mid( x0, x1-x0 );
			if	( strName == "<control>" ){
				if	( strLine.Right( 4 ) == ";;;;" ){
					strLine.Delete( strLine.GetLength()-4, 4 );
					int	x = strLine.ReverseFind( ';' );
					CStringA strSub = strLine.Mid( x+1 );
					strName += "\t" + strSub;
				}
			}
			strNames += strLine.Left( x0 );
			strNames += strName + ";\r\n";
		}
		m_strCharacterNames = strNames;
	}
}

CString
CCharMapDlg::GetCharName( UINT uCode )
{
	if	( m_strCharacterNames.IsEmpty() )
		return	NULL;

	CStringA strCode;
	strCode.Format( "%04X;", uCode );
	if	( uCode )
		strCode.Insert( 0, "\n" );
	bool	bHit = false;
	int	i;
	for	( i = 0;; i++ ){
		i = m_strCharacterNames.Find( strCode, i );
		if	( i >= 0 ){
			if	( i == 0 )
				break;
			else if	( m_strCharacterNames[i] == '\n' ){
				int	x = m_strCharacterNames.Find( ';', i );
				if	( m_strCharacterNames[x+1] == '<' ){
					x = m_strCharacterNames.Find( '>', x );
					if	( m_strCharacterNames.Mid( x-5, 6 ) == "First>" ||
						  m_strCharacterNames.Mid( x-4, 5 ) == "Last>" )
						continue;
				}
				break;
			}
		}
		else
			break;
	}
	if	( i < 0 ){
		for	( i = 0;; i++ ){
			i = m_strCharacterNames.Find( ", First>;", i );
			if	( i < 0 )
				return	NULL;
			int	x;
			for	( x = i; m_strCharacterNames[x] != '<'; x-- )
				;
			x++;
			CStringA strName = m_strCharacterNames.Mid( x, i-x );
			for	( i = x; m_strCharacterNames[i] != '\n'; i-- )
				;
			i++;
			UINT	uFirst = strtol( m_strCharacterNames.Mid( i ), NULL, 16 );
			i = m_strCharacterNames.Find( '\n', x );
			i++;
			UINT	uLast = strtol( m_strCharacterNames.Mid( i ), NULL, 16 );
			if	( uCode >= uFirst && uCode <= uLast )
				return	CString( strName );
		}
	}
	if	( i >= 0 ){
		int	x = m_strCharacterNames.Find( ';', i );
		if	( x < 0 )
			return	NULL;
		i = m_strCharacterNames.Find( ';', ++x );
		if	( i < 0 )
			return	NULL;
		CStringA strName = m_strCharacterNames.Mid( x, i-x );
		if	( strName == "<control>" ){
			i = m_strCharacterNames.Find( "\r", i );
			CStringA strSub = m_strCharacterNames.Mid( x, i-x );
			if	( strSub.Right( 4 ) == ";;;;" ){
				strSub.Delete( strSub.GetLength()-4, 4 );
				i = strSub.ReverseFind( ';' );
				strSub.Delete( 0, i+1 );
				strName += "\r\n";
				strName += strSub;
			}
		}
		return	CString( strName );
	}
	else
		return	NULL;
}

int	CALLBACK
CCharMapDlg::EnumFontProc( const LOGFONT* plf, const TEXTMETRIC* ptm, DWORD dwType, LPARAM lparam )
{
	BOOL	bContinue = TRUE;
	CCharMapDlg*	pdlg = (CCharMapDlg*)lparam;

	if	( plf->lfFaceName[0] == '@' )
		;
	else if	( dwType & TRUETYPE_FONTTYPE )
		((CComboBox*)pdlg->GetDlgItem( IDC_COMBO_FONT ))->AddString( plf->lfFaceName );

	return	bContinue;
}
