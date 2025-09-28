// SenderPage.cpp : implementation file
//

#include "pch.h"
#include "ChkMails.h"
#include "SenderPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC( CSenderPage, CPropertyPage )

///////////////////////////////////////////////////////////////////////////////////////
// Constructor

CSenderPage::CSenderPage( void )
	: CPropertyPage( IDD_SENDER )
{
	m_dwFlags = 0;
}

///////////////////////////////////////////////////////////////////////////////////////
// Overridden Functions

BOOL
CSenderPage::OnInitDialog( void )
{
	CPropertyPage::OnInitDialog();

	for	( int i = 0; i < 5; i++ )
		if	( m_dwFlags & ( 1 << i ) )
			((CButton*)GetDlgItem( IDC_CHECK_SENDER_MESSAGEID+i ))->
				SetCheck( BST_CHECKED );

	return	TRUE;
}

void
CSenderPage::OnOK( void )
{
	m_dwFlags = 0xffff;
	for	( int i = 0; i < 5; i++ )
		if	( ((CButton*)GetDlgItem( IDC_CHECK_SENDER_MESSAGEID+i ))->
				GetCheck() != BST_CHECKED )
			m_dwFlags &= ~( 1 << i );
}
