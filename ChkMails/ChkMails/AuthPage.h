// AuthPage.h : header file
//

#pragma once

class CAuthPage : public CPropertyPage
{
	DECLARE_DYNAMIC( CAuthPage )

public:
	CAuthPage( void );

		int	m_nAuth;
		DWORD	m_dwFlags;
		DWORD	m_dwUsed;

protected:
	virtual	BOOL	OnInitDialog( void );
	virtual	void	OnOK( void );
};
