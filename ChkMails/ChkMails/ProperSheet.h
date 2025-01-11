// ProperSheet.h : header file
//

#pragma once

class CProperSheet : public CPropertySheet
{
	DECLARE_DYNAMIC( CProperSheet )

public:
	CProperSheet( UINT uIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0 );

	virtual	INT_PTR	DoModal( void );
protected:
		int	m_xTabMargin,
			m_yTabMargin;
		int	m_xPageMargin,
			m_yPageMargin;
		CRect	m_rectLast;
		CRect	m_rectOriginal;
	   CPtrArray	m_haButtons;
		bool	m_bInit;

	virtual	BOOL	OnInitDialog( void );
	virtual	void	BuildPropPageArray( void );

	afx_msg	void	OnGetMinMaxInfo( MINMAXINFO* lpMMI );
	afx_msg	void	OnSize( UINT nType, int cx, int cy );
	DECLARE_MESSAGE_MAP()

	static	int	CALLBACK
			OnCallback( HWND hwnd, UINT uMsg, LPARAM lParam );

	class CDialogTemple : public CDialogTemplate
	{
	public:
		void	Attach( DLGTEMPLATE* pTemp )
		{
			m_hTemplate      = GlobalHandle( pTemp );
			m_dwTemplateSize = GetTemplateSize( pTemp );
			m_bSystemFont    = false;
		}
	};
};
