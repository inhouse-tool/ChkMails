// DomainPage.h : header file
//

#pragma once

class CDomainPage : public CPropertyPage
{
	DECLARE_DYNAMIC( CDomainPage )

public:
	CDomainPage( void );

		CString	m_strDomains;
		CString	m_strTLDCache;

protected:
	virtual	BOOL	OnInitDialog( void );
	virtual	void	OnOK( void );
	virtual	BOOL	PreTranslateMessage( MSG* pMsg );

	afx_msg	void	OnButtonAdd( void );
	afx_msg	void	OnButtonDelete( void );
	afx_msg	void	OnEditDomain( void );
	afx_msg	void	OnChangeList( NMHDR* pNMHDR, LRESULT* pResult );
	DECLARE_MESSAGE_MAP()

		void	SortDomain( CStringArray& saList, CString strList );
		int	CompareDomain( CString strDomain1, CString strDomain2 );
};
