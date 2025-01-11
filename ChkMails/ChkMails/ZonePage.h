// ZonePage.h : header file
//

#pragma once

#include "TimeZones.h"

class CZonePage : public CPropertyPage
{
	DECLARE_DYNAMIC( CZonePage )

public:
	CZonePage( void );

		CString	m_strTimes;

protected:
	    CTimeZones	m_timezones;

		int	m_iZone;
		CTime	m_tNow;

	virtual	BOOL	OnInitDialog( void );
	virtual	void	OnOK( void );

	afx_msg	void	OnTimer( UINT_PTR nIDEvent );
	afx_msg	void	OnChangeList( NMHDR* pNMHDR, LRESULT* pResult );
	afx_msg	void	OnClickHeader( NMHDR* pNMHDR, LRESULT* pResult );
	DECLARE_MESSAGE_MAP()

		void	OnSelectItem( int iRow );
		CString	DecodeDST( SYSTEMTIME time );
		void	DrawLocalTime( int iZone );
	static	int	CALLBACK
			CompareZone( LPARAM lItem1, LPARAM lItem2, LPARAM lParam );

	typedef	struct{
		CListCtrl*	pList;
		int		iColumn;
		int		iOrder;
	}SortArg;
};
