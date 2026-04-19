// WordPage.h : header file
//

#pragma once

class CWordPage : public CPropertyPage
{
	DECLARE_DYNAMIC( CWordPage )

public:
	CWordPage( void );

		CString	m_strWords;

protected:
	virtual	BOOL	OnInitDialog( void );
	virtual	void	OnOK( void );
	virtual	BOOL	PreTranslateMessage( MSG* pMsg );

	afx_msg	void	OnButtonAdd( void );
	afx_msg	void	OnButtonDelete( void );
	afx_msg	void	OnChangeEdit( void );
	afx_msg	void	OnChangeList( NMHDR* pNMHDR, LRESULT* pResult );
	DECLARE_MESSAGE_MAP()

		void	SortWord( CStringArray& saList, CString strList );
};
