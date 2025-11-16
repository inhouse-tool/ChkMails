// CharMap.h : interface of the CCharMap class
//

#pragma once

class CCharMap
{
public:
	    CCharMap( void );
		void	SetFontName( LPCTSTR lpszFontName );
		CString	GetFontName( void );
		bool	IsMapped( UINT uCode );
		int	GetCount( void );
		UINT	GetLowerBound( void );
		UINT	GetUpperBound( void );
		CString	GetSLNG( void );

protected:
		CString	m_strFontName;
	     CUIntArray	m_uaCodes;
		int	m_nChar;
		CString	m_strSLNG;

		CString	SelectDefaultFont( void );

		bool	ReadCMAP( HDC hDC );
		bool	ReadSLNG( HDC hDC );

		WORD	Swap16( WORD w );
		DWORD	Swap32( DWORD dw );
};

class CCharMapWnd : public CWnd
{
protected:
	afx_msg	void	OnPaint( void );
	DECLARE_MESSAGE_MAP()
};

class CCharMapDlg : public CDialog
{
public:
	CCharMapDlg( void );
	~CCharMapDlg( void );

	    CCharMap	m_charmap;

protected:
	    CCharMapWnd	m_wndMap;
	    CScrollBar	m_wndScrollbar;

		CFont	m_fontSel,
			m_fontFix;

		CRect	m_rectOriginal;
		int	m_nUnit;
		int	m_nRow;
		int	m_iOffset;
		UINT	m_uCode;

		int	m_xCode,
			m_xMargin,
			m_yEdit,
			m_cyEdit;
	    COLORREF	m_crBack,
			m_crMapped,
			m_crUnmapped,
			m_crSelected;

	virtual	BOOL	OnInitDialog( void );
	virtual	BOOL	PreTranslateMessage( MSG* pMsg );

	afx_msg	void	OnGetMinMaxInfo( MINMAXINFO* lpMMI );
	afx_msg	BOOL	OnMouseWheel( UINT nFlags, short zDelta, CPoint pt );
	afx_msg	void	OnSize( UINT nType, int cx, int cy );
	afx_msg	void	OnVScroll( UINT nSBCode, UINT nPos, CScrollBar* pScrollBar );
	afx_msg	void	OnSelChange( void );
	afx_msg	void	OnClickMap( void );
	afx_msg LRESULT	OnPaintMap( WPARAM wParam, LPARAM lParam );
	DECLARE_MESSAGE_MAP()

		BYTE*	m_pbTemp;
		void	CreateModal( void );
		DWORD	FillDialog(  BYTE* pb, DLGTEMPLATE* pTemp );
		DWORD	FillStatic(  BYTE* pb, DLGTEMPLATE* pTemp );
		DWORD	FillCombo(   BYTE* pb, DLGTEMPLATE* pTemp );
		DWORD	FillButton(  BYTE* pb, DLGTEMPLATE* pTemp );
		DWORD	FillEdit(    BYTE* pb, DLGTEMPLATE* pTemp );
		BYTE*	Align( BYTE* pb );

		void	GetLayout( void );
		void	SetLayout( void );
		void	SetScrollBar( void );

		void	FollowFont( void );
		void	SetFont( void );

		BOOL	OnKeyOnMap( WPARAM wParam );

	    CString	m_strToFind;
		void	OnEditStart( void );
		void	OnEditEnd( WPARAM wParam );
		void	OnEditFind( void );

		void	SelectCode( UINT uCode );
		void	InvalidateCode( UINT uCode );
		CString	CodeToStr( UINT uCode );
		UINT	StrToCode( CString strCode );
		bool	IsSurrogate( CString strCode );

		void	CopyText( CString strText );

	    CStringA	m_strCharacterNames;
		void	LoadCharacterNames( void );
		CString	GetCharName( UINT uCode );

	static	int	CALLBACK
			EnumFontProc( const LOGFONT* plf, const TEXTMETRIC* ptm, DWORD dwType, LPARAM lparam );
};