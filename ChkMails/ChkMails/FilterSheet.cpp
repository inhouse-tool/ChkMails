// FilterSheet.cpp : implementation file
//

#include "pch.h"
#include "ChkMails.h"
#include "FilterSheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC( CFilterSheet, CProperSheet )

///////////////////////////////////////////////////////////////////////////////////////
// Constructor

CFilterSheet::CFilterSheet( UINT uIDCaption, CWnd* pParentWnd, UINT iSelectPage )
	: CProperSheet( uIDCaption, pParentWnd, iSelectPage )
{
	AddPages();
}

///////////////////////////////////////////////////////////////////////////////////////
// Overridden Functions

BOOL
CFilterSheet::OnInitDialog( void )
{
	BOOL	bResult = CProperSheet::OnInitDialog();

	// Select the last page.

	SetActivePage( AfxGetApp()->GetProfileInt( _T("Sheet"), _T("Filter"), 0 ) );

	// Move 'OK' button to the position of 'Help' button.

	CRect	rectButton;

	CWnd*	pwndHelp = GetDlgItem( IDHELP );
	pwndHelp->GetClientRect( &rectButton );
	pwndHelp->MapWindowPoints( this, &rectButton );
	CWnd*	pwndOK = GetDlgItem( IDOK );
	pwndOK->MoveWindow( &rectButton );
	pwndOK->SetFocus();

	// Move 'Help' button to the left side.

	CRect	rectClient;

	GetClientRect( &rectClient );
	int	cx = rectButton.Width();
	int	dx = rectClient.right - rectButton.right;
	rectButton.left = rectClient.left + dx;
	rectButton.right = rectButton.left + cx;
	pwndHelp->MoveWindow( &rectButton );

	// And change ID from 'IDHELP' to 'ID_HELP' since we won't wrestle with 'Help in CPropertySheet' problem.

	pwndHelp->SetDlgCtrlID( ID_HELP );
	pwndHelp->EnableWindow( TRUE );

	// Hide 'Cancel' button.

	CWnd*	pwndCancel = GetDlgItem( IDCANCEL );
	pwndCancel->ShowWindow( SW_HIDE );

	// Ask main window to set position of this window.

	NotifyPopup( m_hWnd );

	return	bResult;
}

BOOL
CFilterSheet::DestroyWindow( void )
{
	NotifyPopup( NULL );

	return	CProperSheet::DestroyWindow();
}

BOOL
CFilterSheet::PreTranslateMessage( MSG* pMsg )
{
	if	( pMsg->message == WM_KEYDOWN )
		if	( pMsg->wParam == 'V' && GetKeyState( VK_CONTROL ) ){
			int	iIndex = GetActiveIndex();
			CPropertyPage* pPage = GetPage( iIndex );
			pPage->PostMessage( WM_COMMAND, ID_EDIT_PASTE, 0 );
			return	TRUE;
		}

	return	CProperSheet::PreTranslateMessage( pMsg );
}

BOOL
CFilterSheet::OnCommand( WPARAM wParam, LPARAM lParam )
{
	// Remember the last page when this sheet was closed by 'OK'.

	if	( LOWORD( wParam ) == IDOK ){
		int	iIndexOld = AfxGetApp()->GetProfileInt( _T("Sheet"), _T("Filter"), 0 );
		int	iIndexNew = GetActiveIndex();
		if	( iIndexNew != iIndexOld )
			AfxGetApp()->WriteProfileInt( _T("Sheet"), _T("Filter"), iIndexNew );
	}
	else if	( LOWORD( wParam ) == ID_HELP ){
		OnHelp();
		return	TRUE;
	}

	return	CProperSheet::OnCommand( wParam, lParam );
}

///////////////////////////////////////////////////////////////////////////////////////
// Specific Functions

void
CFilterSheet::OnHelp( void )
{
	int	iIndex = GetActiveIndex();
	CPropertyPage* pPage = GetPage( iIndex );
	ShowHelp( pPage->m_hWnd );
}

void
CFilterSheet::AddPages( void )
{
	// Choose behavior of this sheet.

	m_psh.dwFlags |= PSH_HASHELP;
	m_psh.dwFlags |= PSH_NOAPPLYNOW;
	m_psh.dwFlags |= PSP_USEHICON;
	m_psh.hIcon = AfxGetApp()->LoadIcon( IDR_MAINFRAME );

	// Add pages.

	AddPage( &m_pageAuth );
	AddPage( &m_pageCode );
	AddPage( &m_pageDomain );
	AddPage( &m_pageName );
	AddPage( &m_pageRecipient );
	AddPage( &m_pageSender );
	AddPage( &m_pageZone );
	AddPage( &m_pageWhite );
}
