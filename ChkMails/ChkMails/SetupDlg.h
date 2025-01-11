// SetupDlg.h : header file
//

#pragma once

class CSetupDlg : public CDialog
{
	DECLARE_DYNAMIC( CSetupDlg )

public:
	CSetupDlg( CWnd* pParent = nullptr );

		int	m_nPoll;
		bool	m_bSummary;
		bool	m_bTone;
		CString	m_strToneFile;
		CString	m_strLogPath;
		int	m_nLog;
		bool	m_bLogAll;

protected:
		CRect	m_rectOriginal;
		CString	m_strModal;

	virtual	BOOL	OnInitDialog( void );
	virtual	BOOL	DestroyWindow( void );
	virtual	void	OnOK( void );

	afx_msg	BOOL	OnNcActivate( BOOL bActive );
	afx_msg	void	OnGetMinMaxInfo( MINMAXINFO* lpMMI );
	afx_msg	void	OnHelp( void );
	afx_msg	void	OnButtonRefTone( void );
	afx_msg	void	OnButtonRefLog( void );
	afx_msg	void	OnChangeEdit( UINT uID );
	DECLARE_MESSAGE_MAP()

		bool	IsInStartup( void );
		void	PutInStartup( bool bPut );
		CString	GetLinkPath( GUID guidFolder, LPCTSTR pszSubFolder = NULL );
		void	CreateLink( CString strLink, CString strExe );
		bool	IsExisting( CString strPath );
};
