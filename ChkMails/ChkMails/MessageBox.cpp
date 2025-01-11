// MessageBox.cpp : implementation file
//

#include "pch.h"
#include "ChkMails.h"
#include "MessageBox.h"

///////////////////////////////////////////////////////////////////////////////////////
// Constants

#define	NMARGIN		 4
#define	CXBUTTON	50
#define	CYBUTTON	14
#define	CXICON		24

#define	DB_FONTNAME	L"Arial"
#define	DB_FONTSIZE	9
#define	MIN_WIDTH	160
#define	MIN_HEIGHT	64

///////////////////////////////////////////////////////////////////////////////////////
// Constructor & Destructor

CMessageBox::CMessageBox( void )
{
	m_pbTemp = NULL;
	m_uDefButton = 0;
}

CMessageBox::~CMessageBox( void )
{
	delete[]	m_pbTemp;
}

///////////////////////////////////////////////////////////////////////////////////////
// Public Functions

void
CMessageBox::CreateModal( LPCTSTR lpszText, UINT nType )
{
	// Make a Dialog Template memory area.

	DWORD	cb = 0;
	cb += FillDialog(  NULL, NULL, nType );
	cb += FillIcon(    NULL, NULL, nType );
	cb += FillText(    NULL, NULL, nType, lpszText );
	cb += FillButtons( NULL, NULL, nType );
	m_pbTemp = new BYTE[cb];
	memset( m_pbTemp, 0, cb );

	// Fill the Dialog template.

	BYTE*	pb = m_pbTemp;
	DLGTEMPLATE*	pTemp = (DLGTEMPLATE*)pb;
	pb += FillDialog(  pb, pTemp, nType );
	pb += FillIcon(    pb, pTemp, nType );
	pb += FillText(    pb, pTemp, nType, lpszText );
	pb += FillButtons( pb, pTemp, nType );

	// Set the Dialog template.

	InitModalIndirect( (DLGTEMPLATE*)m_pbTemp, NULL, NULL );
}

///////////////////////////////////////////////////////////////////////////////////////
// Overridden Functions

BOOL
CMessageBox::OnInitDialog( void )
{
	CDialog::OnInitDialog();

	// Ask main window to set position of this window.

	NotifyPopup( m_hWnd );

	return	TRUE;
}

BOOL
CMessageBox::DestroyWindow( void )
{
	NotifyPopup( NULL );

	return	CDialog::DestroyWindow();
}

BOOL
CMessageBox::OnCommand( WPARAM wParam, LPARAM lParam )
{
	EndDialog( LOWORD( wParam ) );

	return	TRUE;
}

///////////////////////////////////////////////////////////////////////////////////////
// Message Handlers

BEGIN_MESSAGE_MAP( CMessageBox, CDialog )
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()

void
CMessageBox::OnShowWindow( BOOL bShow, UINT nStatus )
{
	CDialog::OnShowWindow( bShow, nStatus );

	if	( bShow )
		if	( m_uDefButton ){
			SetDefID( m_uDefButton );
			GetDlgItem( m_uDefButton )->SetFocus();
		}
}

///////////////////////////////////////////////////////////////////////////////////////
// Specific Functions

DWORD
CMessageBox::FillDialog( BYTE* pb, DLGTEMPLATE* pTemp, UINT nType )
{
	BYTE*	pbStart;
	bool	bFill;

	// Is it turn to fill the template or turn to calculate the size of the template?

	if	( pb ){
		pbStart = pb;
		bFill = true;
	}
	else{
		pbStart = pb = new BYTE[4096];
		bFill = false;
		pTemp = (DLGTEMPLATE*)pb;
		memset( pTemp, 0, sizeof( *pTemp ) );
	}

	// Select the icon.

	UINT	uIcon = nType & MB_ICONMASK;
	LPWSTR	pIcon = SelectIcon( uIcon );

	// Select the buttons.

	CString	strButton[4];
	UINT	uIdButton[4] = {};

	UINT	uButton = nType & MB_TYPEMASK;
	int	nButton = SelectButtons( uButton, uIdButton, strButton );

	// Setup the dialog template with minimum size.

	pTemp->style = WS_POPUP | WS_CAPTION | DS_SETFONT;
	pTemp->dwExtendedStyle = 0;
	pTemp->cdit = 1 + ( pIcon? 1: 0 ) + nButton;
	pTemp->x  =   0;
	pTemp->y  =   0;
	pTemp->cx = MIN_WIDTH;
	pTemp->cy = MIN_HEIGHT;

	// Get the caption and message text.

	CString	strCaption = AfxGetApp()->m_pszAppName;

	WORD*	pw = (WORD*)( pTemp+1 );

	// Menu array
	*pw++ = 0;			// No Menu

	// Class array
	*pw++ = 0;			// No Class

	// Title array
	DWORD	cb = ( strCaption.GetLength()+1 ) * (int)sizeof( WCHAR );
	memcpy( pw, strCaption.GetBuffer(), cb );
	pw += strCaption.GetLength()+1;

	// Font size
	*pw++ = DB_FONTSIZE;

	// Font name
	int	cchFontName = _countof( DB_FONTNAME );
	memcpy( pw, DB_FONTNAME, cchFontName * sizeof( WCHAR ) );
	pw += cchFontName;

	pb = Align( (BYTE*)pw );

	DWORD	cbFilled = (DWORD)( pb - pbStart );
	if	( !bFill )
		delete[]	pbStart;

	return	cbFilled;
}

DWORD
CMessageBox::FillIcon( BYTE* pb, DLGTEMPLATE* pTemp, UINT nType )
{
	BYTE*	pbStart;
	bool	bFill;

	// Is it turn to fill the template or turn to calculate the size of the template?

	if	( pb ){
		pbStart = pb;
		bFill = true;
	}
	else{
		pbStart = pb = new BYTE[4096];
		bFill = false;
		pTemp = (DLGTEMPLATE*)pb;
		memset( pTemp, 0, sizeof( *pTemp ) );
	}

	// Select the icon.

	UINT	uIcon = nType & MB_ICONMASK;
	LPWSTR	pIcon = SelectIcon( uIcon );

	if	( pIcon ){
		DLGITEMTEMPLATE*	pItem = (DLGITEMTEMPLATE*)pb;

		pItem->style = WS_CHILD | WS_VISIBLE | SS_ICON;
		pItem->dwExtendedStyle = 0;
		pItem->x = NMARGIN;
		pItem->y = NMARGIN;
		pItem->cx = CXICON;
		pItem->cy = CXICON;
		pItem->id = 0;
		WORD*	pw = (WORD*)( pItem+1 );

		// Class array:
		*pw++ = 0xffff;			// ORDINAL of
		*pw++ = 0x0082;			// 'Static Control`

		// Resource array:
		*pw++ = 0xffff;			// ORDINAL of
		*pw++ = (WORD)(QWORD)pIcon;	// Resource ID

		// Title array:
		*pw++ = '\0';			// TEXT of empty

		pb = Align( (BYTE*)pw );
	}

	DWORD	cbFilled = (DWORD)( pb - pbStart );
	if	( !bFill )
		delete[]	pbStart;

	return	cbFilled;
}

DWORD
CMessageBox::FillText( BYTE* pb, DLGTEMPLATE* pTemp, UINT nType, LPCTSTR lpszPrompt )
{
	BYTE*	pbStart;
	bool	bFill;

	// Is it turn to fill the template or turn to calculate the size of the template?

	if	( pb ){
		pbStart = pb;
		bFill = true;
	}
	else{
		pbStart = pb = new BYTE[4096];
		bFill = false;
		pTemp = (DLGTEMPLATE*)pb;
		memset( pTemp, 0, sizeof( *pTemp ) );
	}

	CString	strText = lpszPrompt;
	{
		// Borrow a DC on existing window.

		CWnd*	pWnd = AfxGetMainWnd();
		CDC*	pDC = pWnd->GetDC();
		int	nDC = pDC->SaveDC();

		// Base on the dialog font.

		LOGFONT	lf = {};
		lf.lfHeight = DB_FONTSIZE;
		wcscpy_s( lf.lfFaceName, DB_FONTNAME );
		CFont	font;
		font.CreateFontIndirect( &lf );
		pDC->SelectObject( &font );

		// Get 'Dialog Unit' ( with compensation for 'All uppercase string' ).

		CSize	size = pDC->GetTextExtent( _T("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz") );
		double	dx = ( (double)size.cx / 52 ) / 4;
		double	dy =   (double)size.cy        / 8;
		dx /= 1.04;

		// Calculate a rectagle for the text.

		CRect	rect;
		pDC->DrawText( strText, &rect, DT_CALCRECT );

		int	cx = (int)( ( rect.Width() / dx ) + (NMARGIN*2) );
		int	cy = (int)( ( rect.Height()/ dy ) + (NMARGIN*3) + CYBUTTON );
		UINT	uIcon = nType & MB_ICONMASK;
		if	( uIcon )
			cx += CXICON;

		// Enlarge dialog rectangle when the text requires larger size.

		if	( cx > pTemp->cx )
			pTemp->cx = cx;
		if	( cy > pTemp->cy )
			pTemp->cy = cy;

		pDC->RestoreDC( nDC );
		pWnd->ReleaseDC( pDC );
	}

	DLGITEMTEMPLATE*	pItem = (DLGITEMTEMPLATE*)pb;

	UINT	uIcon = nType & MB_ICONMASK;
	LPWSTR	pIcon = SelectIcon( uIcon );
	int	yButton  = pTemp->cy - CYBUTTON - NMARGIN;

	pItem->style = WS_CHILD | WS_VISIBLE | SS_LEFT;
	pItem->dwExtendedStyle = 0;
	pItem->x = NMARGIN + ( pIcon? CXICON: 0 );
	pItem->y = NMARGIN;
	pItem->cx = pTemp->cx - (NMARGIN*2) - ( pIcon? CXICON: 0 );
	pItem->cy = yButton   - (NMARGIN*2);
	pItem->id = 1;
	WORD*	pw = (WORD*)( pItem+1 );

	// Class array:
	*pw++ = 0xffff;				// ORDINAL of
	*pw++ = 0x0082;				// 'Static Control`

	// Resource array:
	DWORD	cb = ( strText.GetLength()+1 ) * (int)sizeof( WCHAR );
	memcpy( pw, strText.GetBuffer(), cb );	// TEXT of the given prompt
	pw += strText.GetLength()+1;

	// Title array:
	*pw++ = '\0';				// TEXT of empty

	pb = Align( (BYTE*)pw );

	DWORD	cbFilled = (DWORD)( pb - pbStart );
	if	( !bFill )
		delete[]	pbStart;

	return	cbFilled;
}

DWORD
CMessageBox::FillButtons( BYTE* pb, DLGTEMPLATE* pTemp, UINT nType )
{
	BYTE*	pbStart;
	bool	bFill;

	// Is it turn to fill the template or turn to calculate the size of the template?

	if	( pb ){
		pbStart = pb;
		bFill = true;
	}
	else{
		pbStart = pb = new BYTE[4096];
		bFill = false;
		pTemp = (DLGTEMPLATE*)pb;
		memset( pTemp, 0, sizeof( *pTemp ) );
	}

	// Select the buttons.

	CString	strButton[4];
	UINT	uIdButton[4] = {};

	UINT	uButton = nType & MB_TYPEMASK;
	int	nButton = SelectButtons( uButton, uIdButton, strButton );
	int	yButton  = pTemp->cy - 20;
	UINT	uDefault = nType & MB_DEFMASK;

	int	x = pTemp->cx - ( ( CXBUTTON + NMARGIN ) * nButton );
	for	( int i = 0; i < nButton; i++ ){
		DLGITEMTEMPLATE*	pItem = (DLGITEMTEMPLATE*)pb;

		pItem->style = WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON;
		if	( ( ( uDefault == MB_DEFBUTTON1 ) && ( i == 0 ) ) ||
			  ( ( uDefault == MB_DEFBUTTON2 ) && ( i == 1 ) ) ||
			  ( ( uDefault == MB_DEFBUTTON3 ) && ( i == 2 ) )    )
			m_uDefButton = uIdButton[i];
		pItem->dwExtendedStyle = 0;
		pItem->x  = x;
		pItem->y  =  yButton;
		pItem->cx = CXBUTTON;
		pItem->cy = CYBUTTON;
		pItem->id = uIdButton[i];
		WORD*	pw = (WORD*)( pItem+1 );

		// Class array:
		*pw++ = 0xffff;					// ORDINAL of
		*pw++ = 0x0080;					// 'Button Control'

		// Resource array:
		DWORD	cb = ( strButton[i].GetLength()+1 ) * (int)sizeof( WCHAR );
		memcpy( pw, strButton[i].GetBuffer(), cb );	// TEXT of the button
		pw += strButton[i].GetLength()+1;

		// Title array:
		*pw++ = '\0';					// TEXT of empty

		pb = Align( (BYTE*)pw );
		x += CXBUTTON + NMARGIN;
	}

	DWORD	cbFilled = (DWORD)( pb - pbStart );
	if	( !bFill )
		delete[]	pbStart;

	return	cbFilled;
}

int
CMessageBox::SelectButtons( UINT uType, UINT* puIdButton, CString* pstrButton )
{
	int	nButton = 0;

	if	( uType == MB_OK ){
		nButton = 1;
		puIdButton[0] = IDOK;
	}
	else if	( uType == MB_OKCANCEL ){
		nButton = 2;
		puIdButton[0] = IDOK;
		puIdButton[1] = IDCANCEL;
	}
	else if	( uType == MB_ABORTRETRYIGNORE ){
		nButton = 3;
		puIdButton[0] = IDABORT;
		puIdButton[1] = IDRETRY;
		puIdButton[2] = IDIGNORE;
	}
	else if	( uType == MB_YESNOCANCEL ){
		nButton = 3;
		puIdButton[0] = IDYES;
		puIdButton[1] = IDNO;
		puIdButton[2] = IDCANCEL;
	}
	else if	( uType == MB_YESNO ){
		nButton = 2;
		puIdButton[0] = IDYES;
		puIdButton[1] = IDNO;
	}
	else if	( uType == MB_RETRYCANCEL ){
		nButton = 2;
		puIdButton[0] = IDRETRY;
		puIdButton[1] = IDCANCEL;
	}
	else if	( uType == MB_CANCELTRYCONTINUE ){
		nButton = 3;
		puIdButton[0] = IDCANCEL;
		puIdButton[1] = IDTRYAGAIN;
		puIdButton[2] = IDCONTINUE;
	}
	else{
		nButton = 1;
		puIdButton[0] = IDOK;
	}

	static	WCHAR*	apchButtons[] = {
			_T(""),			// 0
			_T("OK"),		// IDOK
			_T("Cancel"),		// IDCANCEL
			_T("&Abort"),		// IDABORT
			_T("&Retry"),		// IDRETRY
			_T("&Ignore"),		// IDIGNORE
			_T("&Yes"),		// IDYES
			_T("&No"),		// IDNO
			_T("&Close"),		// IDCLOSE
			_T("&Help"),		// IDHELP
			_T("&Tray Again"),	// IDTRYAGAIN
			_T("&Continue")		// IDCONTINUE
	};

	for	( int i = 0; i < nButton; i++ )
		pstrButton[i] = apchButtons[puIdButton[i]];

	return	nButton;
}

LPWSTR
CMessageBox::SelectIcon( UINT uIcon )
{
	LPWSTR	pIcon = NULL;

	if	( uIcon == MB_ICONHAND )
		pIcon = IDI_HAND;
	else if	( uIcon == MB_ICONQUESTION )
		pIcon = IDI_QUESTION;
	else if	( uIcon == MB_ICONEXCLAMATION )
		pIcon = IDI_EXCLAMATION;
	else if	( uIcon == MB_ICONASTERISK )
		pIcon = IDI_ASTERISK;
	else if	( uIcon == MB_ICONMASK )
		pIcon = MAKEINTRESOURCE( IDR_MAINFRAME );

	return	pIcon;
}

BYTE*
CMessageBox::Align( BYTE* pb )
{
	if	( (QWORD)pb % sizeof( DWORD ) )
		pb += sizeof( DWORD ) - ( (QWORD)pb % sizeof( DWORD ) );
	return	pb;
}
