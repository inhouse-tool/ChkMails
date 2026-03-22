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
	m_nNameInBody = 0;
}

///////////////////////////////////////////////////////////////////////////////////////
// Overridden Functions

BOOL
CSenderPage::OnInitDialog( void )
{
	CPropertyPage::OnInitDialog();

	for	( int i = 0; i < 6; i++ )
		if	( m_dwFlags & ( 1 << i ) )
			((CButton*)GetDlgItem( IDC_CHECK_SENDER_MESSAGEID+i ))->
				SetCheck( BST_CHECKED );

	CSpinButtonCtrl*	pSpin = (CSpinButtonCtrl*)GetDlgItem( IDC_SPIN_SENDER_FAKEBODY );
	pSpin->SetRange( 1, 99 );
	pSpin->SetPos( m_nNameInBody );

	return	TRUE;
}

void
CSenderPage::OnOK( void )
{
	m_dwFlags = 0xffff;
	for	( int i = 0; i < 6; i++ )
		if	( ((CButton*)GetDlgItem( IDC_CHECK_SENDER_MESSAGEID+i ))->
				GetCheck() != BST_CHECKED )
			m_dwFlags &= ~( 1 << i );

	CSpinButtonCtrl*	pSpin = (CSpinButtonCtrl*)GetDlgItem( IDC_SPIN_SENDER_FAKEBODY );
	m_nNameInBody = pSpin->GetPos();
}
