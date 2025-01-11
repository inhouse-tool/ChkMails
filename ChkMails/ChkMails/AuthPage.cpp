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
		if	( m_dwFlags & ( 1 << i ) )
			;
		else
			GetDlgItem( IDC_STATIC_AUTH_SENDERID+i )->EnableWindow( FALSE );

	return	TRUE;
}

void
CAuthPage::OnOK( void )
{
	CComboBox*	pCombo = (CComboBox*)GetDlgItem( IDC_COMBO_AUTH );
	m_nAuth = pCombo->GetCurSel();
}
