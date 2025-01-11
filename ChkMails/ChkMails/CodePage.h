// CodePage.h : header file
//

#pragma once

class CCodePage : public CPropertyPage
{
	DECLARE_DYNAMIC( CCodePage )

public:
	CCodePage( void );

		DWORD	m_dwFlags;

protected:
	virtual	BOOL	OnInitDialog( void );
	virtual	void	OnOK( void );
};
