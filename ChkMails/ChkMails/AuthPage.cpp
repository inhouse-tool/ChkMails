// AuthPage.cpp : implementation file
//

#include "pch.h"
#include "ChkMails.h"
#include "AuthPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC( CAuthPage, CPropertyPage )

///////////////////////////////////////////////////////////////////////////////////////
// Constructor

CAuthPage::CAuthPage( void )
	: CPropertyPage( IDD_AUTH )
{
	m_nAuth = 0;
	m_dwFlags = 0;
	m_dwUsed = 0;
}

///////////////////////////////////////////////////////////////////////////////////////
// Overridden Functions

BOOL
CAuthPage::OnInitDialog( void )
{
	CPropertyPage::OnInitDialog();

	CComboBox*	pCombo = (CComboBox*)GetDlgItem( IDC_COMBO_AUTH );
	pCombo->SetCurSel( m_nAuth );

	for	( int i = 0; i < 4; i++ )
		if	( m_dwUsed & ( 1 << i ) )
			;
		else
			GetDlgItem( IDC_STATIC_AUTH_SENDERID+i )->EnableWindow( FALSE );

	for	( int i = 0; i < 2; i++ )
		if	( m_dwFlags & ( 1 << i ) )
			((CButton*)GetDlgItem( IDC_CHECK_AUTH_RECEIVED+i ))->SetCheck( BST_CHECKED );

	return	TRUE;
}

void
CAuthPage::OnOK( void )
{
	CComboBox*	pCombo = (CComboBox*)GetDlgItem( IDC_COMBO_AUTH );
	m_nAuth = pCombo->GetCurSel();

	m_dwFlags = 0xffff;
	for	( int i = 0; i < 2; i++ )
		if	( ((CButton*)GetDlgItem( IDC_CHECK_AUTH_RECEIVED+i ))->
				GetCheck() != BST_CHECKED )
			m_dwFlags &= ~( 1 << i );
}
