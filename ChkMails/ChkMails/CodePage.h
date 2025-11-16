// CodePage.h : header file
//

#pragma once

class CCodePage : public CPropertyPage
{
	DECLARE_DYNAMIC( CCodePage )

public:
	CCodePage( void );

		DWORD	m_dwFlags;
		CString	m_strFont;

protected:
	virtual	BOOL	OnInitDialog( void );
	virtual	void	OnOK( void );

	afx_msg	void	OnButtonFont( void );
	DECLARE_MESSAGE_MAP()
};
