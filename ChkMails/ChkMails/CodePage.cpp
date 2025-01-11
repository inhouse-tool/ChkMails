// CodePage.cpp : implementation file
//

#include "pch.h"
#include "ChkMails.h"
#include "CodePage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC( CCodePage, CPropertyPage )

///////////////////////////////////////////////////////////////////////////////////////
// Constructor

CCodePage::CCodePage( void )
	: CPropertyPage( IDD_CODE )
{
	m_dwFlags = 0;
}

///////////////////////////////////////////////////////////////////////////////////////
// Overridden Functions

BOOL
CCodePage::OnInitDialog( void )
{
	CPropertyPage::OnInitDialog();

	for	( int i = 0; i < 3; i++ )
		if	( m_dwFlags & ( 1 << i ) )
			((CButton*)GetDlgItem( IDC_CHECK_CODE_CHARSET+i ))->
				SetCheck( BST_CHECKED );

	return	TRUE;
}

void
CCodePage::OnOK( void )
{
	m_dwFlags = 0;
	for	( int i = 0; i < 3; i++ )
		if	( ((CButton*)GetDlgItem( IDC_CHECK_CODE_CHARSET+i ))->
				GetCheck() == BST_CHECKED )
			m_dwFlags |= ( 1 << i );
}
