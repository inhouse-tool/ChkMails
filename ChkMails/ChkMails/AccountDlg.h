// AccountDlg.h : header file
//

#pragma once

class CAccountDlg : public CDialog
{
	DECLARE_DYNAMIC( CAccountDlg )

public:
	CAccountDlg( CWnd* pParent = nullptr );

		void	SetAccounts( CArray <CAccount, CAccount&>& aAccount );
		void	GetAccounts( CArray <CAccount, CAccount&>& aAccount );

protected:
		CRect	m_rectOriginal;
		CArray	<CAccount, CAccount&>
			m_aAccount;

	virtual	BOOL	OnInitDialog( void );
	virtual	BOOL	DestroyWindow( void );
	virtual	void	OnOK( void );

	afx_msg	void	OnGetMinMaxInfo( MINMAXINFO* lpMMI );
	afx_msg	void	OnTimer( UINT_PTR nIDEvent );
	afx_msg	void	OnHelp( void );
	afx_msg	void	OnCheckHide( void );
	afx_msg	void	OnButtonAdd( void );
	afx_msg	void	OnButtonDelete( void );
	afx_msg	void	OnChangeEdit( UINT uID );
	afx_msg	void	OnClickRadio( UINT uID );
	afx_msg	void	OnChangeList( NMHDR* pNMHDR, LRESULT* pResult );
	DECLARE_MESSAGE_MAP()

		void	OnSelectItem( void );
		void	EnableAdd( void );
};
