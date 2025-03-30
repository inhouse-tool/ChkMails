// NamePage.h : header file
//

#pragma once

class CNamePage : public CPropertyPage
{
	DECLARE_DYNAMIC( CNamePage )

public:
	CNamePage( void );

		CString	m_strNames;

protected:
	virtual	BOOL	OnInitDialog( void );
	virtual	void	OnOK( void );
	virtual	BOOL	PreTranslateMessage( MSG* pMsg );

	afx_msg	void	OnDropFiles( HDROP hDropInfo );
	afx_msg	void	OnTimer( UINT_PTR nIDEvent );
	afx_msg	void	OnButtonAdd( void );
	afx_msg	void	OnButtonDelete( void );
	afx_msg	void	OnChangeEdit( UINT uID );
	afx_msg	void	OnChangeList( NMHDR* pNMHDR, LRESULT* pResult );
	afx_msg	void	OnEditPaste( void );
	DECLARE_MESSAGE_MAP()

		void	OnSelectItem( void );
		void	SortName( CStringArray& saList, CString strList );
		void	AddNames( CString strNames );
		CString	LoadText( CString strFile );
};
