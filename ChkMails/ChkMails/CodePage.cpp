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

#include "CharMap.h"

BOOL
CCodePage::OnInitDialog( void )
{
	CPropertyPage::OnInitDialog();

	for	( int i = 0; i < 4; i++ )
		if	( m_dwFlags & ( 1 << i ) )
			((CButton*)GetDlgItem( IDC_CHECK_CODE_CHARSET+i ))->
				SetCheck( BST_CHECKED );

	if	( m_strFont.IsEmpty() ){
		CCharMap	charmap;
		m_strFont = charmap.GetFontName();
	}
	GetDlgItem( IDC_BUTTON_CODE_FONT )->SetWindowText( m_strFont );

	return	TRUE;
}

void
CCodePage::OnOK( void )
{
	m_dwFlags = 0xffff;
	for	( int i = 0; i < 4; i++ )
		if	( ((CButton*)GetDlgItem( IDC_CHECK_CODE_CHARSET+i ))->
				GetCheck() != BST_CHECKED )
			m_dwFlags &= ~( 1 << i );
}

BEGIN_MESSAGE_MAP( CCodePage, CPropertyPage )
	ON_BN_CLICKED( IDC_BUTTON_CODE_FONT, OnButtonFont )
END_MESSAGE_MAP()

void
CCodePage::OnButtonFont( void )
{
	CCharMapDlg	dlg;
	dlg.m_charmap.SetFontName( m_strFont );
	if	( dlg.DoModal() == IDOK ){
		m_strFont = dlg.m_charmap.GetFontName();
		GetDlgItem( IDC_BUTTON_CODE_FONT )->SetWindowText( m_strFont );
	}
}
