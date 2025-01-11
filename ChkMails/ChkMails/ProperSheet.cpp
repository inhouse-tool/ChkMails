// ProperSheet.cpp : implementation file
//

#include "pch.h"
#include "ProperSheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC( CProperSheet, CPropertySheet )

///////////////////////////////////////////////////////////////////////////////////////
// Constructor

CProperSheet::CProperSheet( UINT uIDCaption, CWnd* pParentWnd, UINT iSelectPage )
	: CPropertySheet( uIDCaption, pParentWnd, iSelectPage )
{
	m_xTabMargin  = 0;
	m_yTabMargin  = 0;
	m_xPageMargin = 0;
	m_yPageMargin = 0;
	m_bInit = false;
}

///////////////////////////////////////////////////////////////////////////////////////
// Overridden Functions

BOOL
CProperSheet::OnInitDialog( void )
{
	BOOL	bResult = CPropertySheet::OnInitDialog();

	// Set the first size as the minimum size.

	CRect	rectSheet;

	GetWindowRect( &rectSheet );
	m_rectOriginal = rectSheet;
	GetClientRect( &rectSheet );

	// Get tab control's offset to the sheet.

	CRect	rectTab;

	CTabCtrl*	pTab = GetTabControl();
	pTab->GetClientRect( &rectTab );
	pTab->MapWindowPoints( this, &rectTab );
	m_xTabMargin = rectSheet.right  - rectTab.right;
	m_yTabMargin = rectSheet.bottom - rectTab.bottom;

	// Get page's offset to the tab control.

	CRect	rectPage;

	CPropertyPage*	pPage = GetActivePage();
	pPage->GetClientRect( &rectPage );
	pPage->MapWindowPoints( this, &rectPage );
	m_xPageMargin = rectTab.right  - rectPage.right;
	m_yPageMargin = rectTab.bottom - rectPage.bottom;

	// Get the buttons on the sheet.

	CWnd*	pWnd = GetWindow( GW_CHILD );
	while	( pWnd ){
		pWnd = FindWindowEx( m_hWnd, pWnd->m_hWnd, _T("Button"), NULL );
		if	( !pWnd )
			break;
		m_haButtons.Add( pWnd->m_hWnd );
	}
		
	m_bInit = true;

	return	bResult;
}

INT_PTR
CProperSheet::DoModal( void )
{
	// Enable the callback before enter the modal loop.

	m_psh.dwFlags |= PSH_USECALLBACK;
	m_psh.pfnCallback = OnCallback;

	return	CPropertySheet::DoModal();
}

void
CProperSheet::BuildPropPageArray( void )
{
	// Do 'BuildPropPageArray()' as usual.

	CPropertySheet::BuildPropPageArray();

	// Get the font applied to the main window.

	CWnd*	pwnd = AfxGetMainWnd();
	CFont*	pFont = pwnd->GetFont();
	LOGFONT	lf = {};
	pFont->GetLogFont( &lf );

	// Replace the font of each page.

	_PROPSHEETPAGEW* ppsp = (_PROPSHEETPAGEW*)m_psh.ppsp;
	INT_PTR	n = m_pages.GetSize();
	for	( INT_PTR i = 0; i < n; i++ ){
		DLGTEMPLATE* pTemp = (DLGTEMPLATE*)ppsp[i].pResource;

		CDialogTemple temp;
		temp.Attach( pTemp );
		temp.SetFont( lf.lfFaceName, (WORD)lf.lfHeight );
		temp.Detach();
	}
}

///////////////////////////////////////////////////////////////////////////////////////
// Message Handlers

BEGIN_MESSAGE_MAP( CProperSheet, CPropertySheet )
	ON_WM_GETMINMAXINFO()
	ON_WM_SIZE()
END_MESSAGE_MAP()

void
CProperSheet::OnGetMinMaxInfo( MINMAXINFO* lpMMI )
{
	// Answer minimum size.

	lpMMI->ptMinTrackSize.x = m_rectOriginal.Width();
	lpMMI->ptMinTrackSize.y = m_rectOriginal.Height();
}

void
CProperSheet::OnSize( UINT nType, int cx, int cy )
{
	if	( m_bInit ){
		// Get rectangles of the sheet and tab control.

		CRect	rectSheet;
		GetClientRect( &rectSheet );

		CRect	rectTab;
		CTabCtrl*	pTab = GetTabControl();
		pTab->GetClientRect( &rectTab );
		pTab->MapWindowPoints( this, &rectTab );

		// Adjust the size of tab control.

		rectTab.right  = rectSheet.right  - m_xTabMargin;
		rectTab.bottom = rectSheet.bottom - m_yTabMargin;
		pTab->SetWindowPos( NULL, 0, 0, rectTab.Width(), rectTab.Height(), SWP_NOZORDER | SWP_NOMOVE );

		// Adjust the position of the buttons on the sheet.

		INT_PTR	i, n = m_haButtons.GetCount();
		for	( i = 0; i < n; i++ ){
			HWND	hWnd = (HWND)m_haButtons[i];

			CRect	rectButton;
			::GetClientRect( hWnd, &rectButton );
			::InvalidateRect( hWnd, &rectButton, FALSE );
			::MapWindowPoints( hWnd, m_hWnd, (POINT*)&rectButton, 2 );

			int	x;
			if	( rectButton.left > m_rectLast.Width() / 2 )
				x = rectSheet.right - ( m_rectLast.right - rectButton.left );
			else
				x = rectSheet.left + m_xTabMargin;
			int	y = rectSheet.bottom - rectButton.Height() - ( ( m_yTabMargin - rectButton.Height() ) / 2 ) - 1;

			::SetWindowPos( hWnd, NULL, x, y, 0, 0, SWP_NOZORDER | SWP_NOSIZE );
		}

		// Adjust the size of the active page.

		CPropertyPage*	pPage = GetActivePage();
		if	( pPage ){
			CRect	rectPage;
			pPage->GetClientRect( &rectPage );
			pPage->MapWindowPoints( this, &rectPage );
			rectPage.right  = rectTab.right  - m_xPageMargin;
			rectPage.bottom = rectTab.bottom - m_yPageMargin;
			pPage->SetWindowPos( NULL, 0, 0, rectPage.Width(), rectPage.Height(), SWP_NOZORDER | SWP_NOMOVE );
		}
	}

	// Set current size as the last size.

	GetClientRect( &m_rectLast );
}

///////////////////////////////////////////////////////////////////////////////////////
// Specific Functions

int	CALLBACK
CProperSheet::OnCallback( HWND hwnd, UINT uMsg, LPARAM lParam )
{
	// When the sheet is about to be created, replace the font.

	if	( uMsg == PSCB_PRECREATE ){
		CWnd*	pwnd = AfxGetMainWnd();
		CFont*	pFont = pwnd->GetFont();
		LOGFONT	lf = {};
		pFont->GetLogFont( &lf );

		CDialogTemple temp;
		temp.Attach( (DLGTEMPLATE*)lParam );
		temp.SetFont( lf.lfFaceName, (WORD)lf.lfHeight );
		temp.Detach();

		((DLGTEMPLATE*)lParam)->style |= WS_THICKFRAME;
	}

	return	0;
}
