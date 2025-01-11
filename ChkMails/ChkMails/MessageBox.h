// MessageBox.h : interface of the CDark class
//

#pragma once

class CMessageBox : public CDialog
{
public:
		CMessageBox( void );
		~CMessageBox( void );

		void	CreateModal( LPCTSTR lpszText, UINT nType = MB_OK );
protected:
		BYTE*	m_pbTemp;
		UINT	m_uDefButton;

	virtual	BOOL	OnInitDialog( void );
	virtual	BOOL	DestroyWindow( void );
	virtual	BOOL	OnCommand( WPARAM wParam, LPARAM lParam );

	afx_msg	void	OnShowWindow( BOOL bShow, UINT nStatus );
	DECLARE_MESSAGE_MAP()

		DWORD	FillDialog(  BYTE* pb, DLGTEMPLATE* pTemp, UINT nType );
		DWORD	FillIcon(    BYTE* pb, DLGTEMPLATE* pTemp, UINT nType );
		DWORD	FillText(    BYTE* pb, DLGTEMPLATE* pTemp, UINT nType, LPCTSTR lpszPrompt );
		DWORD	FillButtons( BYTE* pb, DLGTEMPLATE* pTemp, UINT nType );

		int	SelectButtons( UINT uType, UINT* puIdButton, CString* pstrButton );
		LPWSTR	SelectIcon( UINT uIcon );
		BYTE*	Align( BYTE* pb );
};