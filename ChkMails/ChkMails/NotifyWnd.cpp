// NotifyWnd.cpp : implementation file
//

#include "pch.h"
#include "NotifyWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#ifdef	UNICODE
#define	strcpy_s	wcscpy_s
#endif//UNICODE

#define	TOAST_STEP	20

#define	TID_TOAST	1
#define	TID_MONITOR	2

///////////////////////////////////////////////////////////////////////////////////////
// Constructor

CNotifyWnd::CNotifyWnd( void )
	: CWnd()
{
	CString	strClass = _T("NotifyWnd\t\t");
	LPCTSTR	pchSection = _T("NotifyWnd");

	// Load settings.

	m_strFontName = _T("Arial");
	m_nFontSize   = 14;
	m_bToast      = true;
	m_nPopup      = 400;
	m_crText      = RGB( 255, 255, 255 );
	m_crBack      = RGB(   0,   0,   0 );
	m_crLine      = RGB( 127, 127, 127 );
	m_bAlpha      = 168;

	CWinApp*	pApp = AfxGetApp();

	CString	strKey;
	strKey.Format( _T("Software\\%s\\%s\\%s"), pApp->m_pszRegistryKey, pApp->m_pszAppName, pchSection );

	HKEY	hKey = NULL;
	DWORD	dwResult = RegOpenKeyEx( HKEY_CURRENT_USER, strKey, 0, KEY_WRITE, &hKey );

	if	( dwResult == ERROR_SUCCESS ){
		RegCloseKey( hKey );

		m_strFontName = pApp->GetProfileString( pchSection, _T("FontName"), m_strFontName );
		m_nFontSize   = pApp->GetProfileInt(    pchSection, _T("FontSize"), m_nFontSize );
		m_bToast      = pApp->GetProfileInt(    pchSection, _T("Toast"),    m_bToast );
		m_nPopup      = pApp->GetProfileInt(    pchSection, _T("Popup"),    m_nPopup );
		m_crText      = pApp->GetProfileInt(    pchSection, _T("crText"),   m_crText );
		m_crBack      = pApp->GetProfileInt(    pchSection, _T("crBack"),   m_crBack );
		m_crLine      = pApp->GetProfileInt(    pchSection, _T("crLine"),   m_crLine );
		m_bAlpha      = pApp->GetProfileInt(    pchSection, _T("Alpha"),    m_bAlpha );
	}

	m_xyMargin = m_nFontSize/2;
	m_iToast   = 0;
	m_bPullDown  = false;

	// Register a unique class for this program.

	WNDCLASS wc = { 0 };
	wc.lpfnWndProc   = AfxWndProc;
	wc.hInstance     = AfxGetInstanceHandle();
	wc.lpszClassName = strClass;
	AfxRegisterClass( &wc );

	// Create a window of tge registered class.

	Create( strClass, _T("Notify Window"), 0x0, CRect( 0, 0, 0, 0 ), GetDesktopWindow(), 0 );
}

///////////////////////////////////////////////////////////////////////////////////////
// Public Functions

void
CNotifyWnd::SetText( CString strText )
{
	m_strText = strText;
	m_strText.Trim();

	if	( m_strText.IsEmpty() ){
		ShowWindow( SW_HIDE );
		return;
	}

	CDC*	pDC = GetDC();
	pDC->SelectObject( &m_font );
	CSize	size = pDC->GetTextExtent( _T(" ") );

	int	cx = 0, cy = 0;
	int	i = 0, x = 0;
	do{
		CString	str;
		i = m_strText.Find( '\n', x );
		if	( i < 0 )
			str = m_strText.Mid( x );
		else
			str = m_strText.Mid( x, i-x );
		size = pDC->GetTextExtent( str );
		if	( str.Find( '\t' ) >= 0 ){
			size.cx += m_xyMargin;
		}
		if	( size.cx > cx )
			cx = size.cx;
		cy += m_nFontSize;
		x = i+1;

	}while	( i >= 0 );

	ReleaseDC( pDC );

	cx += m_xyMargin*2;
	cy += m_xyMargin*2;

	PlaceWindow( cx, cy );

	if	( m_bToast ){
		m_iToast = 0;
		PostMessage( WM_TIMER, TID_TOAST, 0 );
		SetTimer( TID_TOAST, m_nPopup/TOAST_STEP, NULL );
	}
	else
		ShowWindow( SW_SHOWNA );
}

///////////////////////////////////////////////////////////////////////////////////////
// Message Handlers

BEGIN_MESSAGE_MAP( CNotifyWnd, CWnd )
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_MESSAGE( WM_DISPLAYCHANGE, OnDisplayChange )
END_MESSAGE_MAP()

int
CNotifyWnd::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
	if	( CWnd::OnCreate( lpCreateStruct ) == -1 )
		return	-1;

	DWORD	dwExStyle = GetWindowLong( m_hWnd, GWL_EXSTYLE );
	dwExStyle |= WS_EX_LAYERED;
	dwExStyle |= WS_EX_TOPMOST;
	dwExStyle |= WS_EX_TOOLWINDOW;
	SetWindowLong( m_hWnd, GWL_EXSTYLE, dwExStyle );

	SetCursor( LoadCursor( NULL, IDC_ARROW ) );

	SetLayeredWindowAttributes( m_crBack, m_bAlpha, LWA_ALPHA );

	// Make a font.

	LOGFONT	lf = {};
	lf.lfHeight         = m_nFontSize;
	lf.lfWeight         = FW_REGULAR;
	lf.lfCharSet        = ANSI_CHARSET;
	lf.lfPitchAndFamily = VARIABLE_PITCH | FF_SWISS;
	lf.lfQuality        = CLEARTYPE_QUALITY;
	strcpy_s( lf.lfFaceName, m_strFontName );
	m_font.CreateFontIndirect( &lf );

	return	0;
}

BOOL
CNotifyWnd::OnEraseBkgnd( CDC* pDC )
{
	return	FALSE;
}

void
CNotifyWnd::OnLButtonDown( UINT nFlags, CPoint point )
{
	ShowWindow( SW_HIDE );
}

void
CNotifyWnd::OnPaint( void )
{
	PAINTSTRUCT ps;
	CDC*	pDC = BeginPaint( &ps );
	int	nDC = pDC->SaveDC();
	CRect	rectUpdate = ps.rcPaint;

	pDC->FillSolidRect( &rectUpdate, m_crBack );

	Draw( pDC );

	pDC->RestoreDC( nDC );
	EndPaint( &ps );
}

void
CNotifyWnd::OnTimer( UINT_PTR nIDEvent )
{
	if	( nIDEvent == TID_TOAST ){
		ToastWindow();
		if	( ++m_iToast > TOAST_STEP ){
			m_iToast = 0;
			KillTimer( nIDEvent );
		}
	}
	else if	( nIDEvent == TID_MONITOR ){
		KillTimer( nIDEvent );
		CRect	rectWindow;
		GetWindowRect( &rectWindow );
		PlaceWindow( rectWindow.Width(), rectWindow.Height() );
		ShowWindow( SW_SHOWNA );
	}
	else
		CWnd::OnTimer( nIDEvent );
}

LRESULT
CNotifyWnd::OnDisplayChange( WPARAM wParam, LPARAM lParam )
{
	if	( IsWindowVisible() ){
		ShowWindow( SW_HIDE );
		SetTimer( TID_MONITOR, 5000, NULL );
	}

	return	0;
}

///////////////////////////////////////////////////////////////////////////////////////
// Specific Functions

void
CNotifyWnd::PlaceWindow( int cx, int cy )
{
	HMONITOR hMonitor = MonitorFromPoint( CPoint( 0, 0 ), MONITOR_DEFAULTTOPRIMARY );
	MONITORINFO mi;
	mi.cbSize = sizeof( mi );
	GetMonitorInfo( hMonitor, &mi );

	enum{
		unknown, left, top, right, bottom
	}	ePos = bottom;

	if	( mi.rcMonitor.left   < mi.rcWork.left )
		ePos = left;
	else if	( mi.rcMonitor.top    < mi.rcWork.top )
		ePos = top;
	else if	( mi.rcMonitor.right  > mi.rcWork.right )
		ePos = right;
	else if	( mi.rcMonitor.bottom > mi.rcWork.bottom )
		ePos = bottom;

	int	x, y;

	m_bPullDown = false;
	if	( ePos == left ){
		x = mi.rcWork.left;
		y = mi.rcWork.bottom - cy;
	}
	else if	( ePos == top ){
		x = mi.rcWork.right  - cx;
		y = mi.rcWork.top;
		m_bPullDown = true;
	}
	else if	( ePos == right ){
		x = mi.rcWork.right  - cx;
		y = mi.rcWork.bottom - cy;
	}
	else if	( ePos == bottom ){
		x = mi.rcWork.right  - cx;
		y = mi.rcWork.bottom - cy;
	}

	SetWindowPos( &CWnd::wndTopMost, x, y, cx, cy, 0 );
}

void
CNotifyWnd::ToastWindow( void )
{
	CRect	rectWindow;
	GetWindowRect( &rectWindow );

	if	( m_iToast == 0 ){
		m_rectToasted = rectWindow;
		if	( m_bPullDown )
			rectWindow.bottom = rectWindow.top;
		else
			rectWindow.top = rectWindow.bottom;
		ShowWindow( SW_SHOWNA );
	}
	else if	( m_iToast == TOAST_STEP ){
		rectWindow = m_rectToasted;
	}
	else{
		int	yStep = m_rectToasted.Height() / TOAST_STEP;
		if	( m_bPullDown ){
			rectWindow.bottom += yStep;
			rectWindow.top     = m_rectToasted.top;	
		}
		else{
			rectWindow.top    -= yStep;
			rectWindow.bottom  = m_rectToasted.bottom;
		}
	}

	SetWindowPos( NULL, rectWindow.left, rectWindow.top, rectWindow.Width(), rectWindow.Height(), SWP_NOZORDER );

	if	( m_bPullDown )
		Invalidate( FALSE );
}

void
CNotifyWnd::Draw( CDC* pDC )
{
	CRect	rect, rectClient;
	GetClientRect( &rectClient );

	if	( m_bPullDown )
		pDC->SetWindowOrg( 0, m_rectToasted.Height()-rectClient.Height() );

	rect = rectClient;
	int	xLeft  = rectClient.left;
	int	xRight = rectClient.right;
	rect.top   += m_xyMargin;
	rect.bottom = rect.top + m_nFontSize;
	rect.left  += m_xyMargin;
	rect.right -= m_xyMargin;

	pDC->SetBkMode( TRANSPARENT );
	pDC->SetTextColor( m_crText );

	int	i = 0, x = 0;
	do{
		CString	str;
		i = m_strText.Find( '\n', x );
		if	( i < 0 )
			str = m_strText.Mid( x );
		else
			str = m_strText.Mid( x, i-x );

		if	( str.IsEmpty() ){
			CPen	pen( PS_SOLID, 1, m_crLine );
			pDC->SelectObject( &pen );
			int	yCenter = rect.top + (m_nFontSize/2);
			pDC->MoveTo( xLeft,  yCenter );
			pDC->LineTo( xRight, yCenter );
		}
		else{
			pDC->SelectObject( &m_font );
			int	xTab = str.Find( '\t' );
			if	( xTab >= 0 ){
				CString	strLeft = str.Left( xTab );
				pDC->DrawText( strLeft,  &rect, DT_SINGLELINE | DT_VCENTER | DT_LEFT );
				CString	strRight = str.Mid( xTab+1 );
				pDC->DrawText( strRight, &rect, DT_SINGLELINE | DT_VCENTER | DT_RIGHT );
			}
			else
				pDC->DrawText( str, &rect, DT_LEFT );
		}

		rect.top    += m_nFontSize;
		rect.bottom += m_nFontSize;
		x = i+1;

	}while	( i >= 0 );
}
