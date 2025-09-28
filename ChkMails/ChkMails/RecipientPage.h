// RecipientPage.h : header file
//

#pragma once

class CRecipientPage : public CPropertyPage
{
	DECLARE_DYNAMIC( CRecipientPage )

public:
	CRecipientPage( void );

		CString	m_strRecipients;
		CString	m_strSenders;

protected:
	virtual	BOOL	OnInitDialog( void );
	virtual	void	OnOK( void );
	virtual	BOOL	PreTranslateMessage( MSG* pMsg );

	afx_msg	void	OnButtonAddRecipient( void );
	afx_msg	void	OnButtonDeleteRecipient( void );
	afx_msg	void	OnEditRecipient( void );
	afx_msg	void	OnChangeListRecipient( NMHDR* pNMHDR, LRESULT* pResult );
	afx_msg	void	OnButtonAddSender( void );
	afx_msg	void	OnButtonDeleteSender( void );
	afx_msg	void	OnEditSender( void );
	afx_msg	void	OnChangeListSender( NMHDR* pNMHDR, LRESULT* pResult );
	DECLARE_MESSAGE_MAP()

		void	SortDomain( CStringArray& saList, CString strList );
		int	CompareDomain( CString strDomain1, CString strDomain2 );
};
