// NotifyWnd.h : header file
//

#pragma once
class CNotifyWnd : public CWnd
{
public:
	CNotifyWnd( void );

		void	SetText( CString strText );

protected:
		CString	m_strFontName;
		int	m_nFontSize;
		bool	m_bToast;
		int	m_nPopup;
		DWORD	m_crText,
			m_crBack,
			m_crLine;
		BYTE	m_bAlpha;

		int	m_xyMargin;
		int	m_iToast;
		CRect	m_rectToasted;
		bool	m_bPullDown;

		CFont	m_font;
		CString	m_strText;

	afx_msg	int	OnCreate( LPCREATESTRUCT lpCreateStruct );
	afx_msg	BOOL	OnEraseBkgnd( CDC* pDC );
	afx_msg	void	OnLButtonDown( UINT nFlags, CPoint point );
	afx_msg	void	OnPaint( void );
	afx_msg	void	OnTimer( UINT_PTR nIDEvent );
	afx_msg	LRESULT	OnDisplayChange( WPARAM wParam, LPARAM lParam );
	DECLARE_MESSAGE_MAP()

		void	PlaceWindow( int cx, int cy );
		void	ToastWindow( void );
		void	Draw( CDC* pDC );
};