// WhitePage.h : header file
//

#pragma once

class CWhitePage : public CPropertyPage
{
	DECLARE_DYNAMIC( CWhitePage )

public:
	CWhitePage( void );

		CString	m_strWhites;

protected:
	virtual	BOOL	OnInitDialog( void );
	virtual	void	OnOK( void );

	afx_msg	void	OnDropFiles( HDROP hDropInfo );
	afx_msg	void	OnButtonDelete( void );
	afx_msg	void	OnChangeList(    NMHDR* pNMHDR, LRESULT* pResult );
	afx_msg	void	OnClickHeader(   NMHDR* pNMHDR, LRESULT* pResult );
	afx_msg	void	OnEditPaste( void );
	DECLARE_MESSAGE_MAP()

		bool	TakeSender( CStringA strHeader );
		void	AddSender( CString strFrom, CString strDomain );
		void	SortSender( void );
	static	int	CALLBACK
			CompareSender( LPARAM lItem1, LPARAM lItem2, LPARAM lParam );

	typedef	struct{
		CListCtrl*	pList;
		int		iColumn;
		int		iOrder;
	}SortArg;
};
