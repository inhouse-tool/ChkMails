// CharaMapApp.h : main header file for the CharaMap application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"

class CCharaMapApp : public CWinApp
{
public:
	virtual	BOOL	InitInstance( void );
};

extern	CCharaMapApp	theApp;
