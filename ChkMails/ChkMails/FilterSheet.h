// FilterSheet.h : header file
//

#pragma once

#include "ProperSheet.h"
#include "AuthPage.h"
#include "CodePage.h"
#include "DomainPage.h"
#include "NamePage.h"
#include "SenderPage.h"
#include "ZonePage.h"
#include "WhitePage.h"

class CFilterSheet : public CProperSheet
{
	DECLARE_DYNAMIC( CFilterSheet )

public:
	CFilterSheet( UINT uIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0 );

	CAuthPage	m_pageAuth;
	CCodePage	m_pageCode;
	CDomainPage	m_pageDomain;
	CNamePage	m_pageName;
	CSenderPage	m_pageSender;
	CZonePage	m_pageZone;
	CWhitePage	m_pageWhite;

protected:
	virtual	BOOL	OnInitDialog( void );
	virtual	BOOL	DestroyWindow( void );
	virtual	BOOL	PreTranslateMessage( MSG* pMsg );
	virtual	BOOL	OnCommand( WPARAM wParam, LPARAM lParam );

		void	OnHelp( void );
		void	AddPages( void );
};
