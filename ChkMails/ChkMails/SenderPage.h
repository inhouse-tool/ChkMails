// SenderPage.h : header file
//

#pragma once

class CSenderPage : public CPropertyPage
{
	DECLARE_DYNAMIC( CSenderPage )

public:
	CSenderPage( void );

		DWORD	m_dwFlags;

protected:
	virtual	BOOL	OnInitDialog( void );
	virtual	void	OnOK( void );
};
