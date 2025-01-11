// TimeZones.h : header file
//

#pragma once

class CTimeZone
{
public:
		CString	m_strKey;
		CString	m_strOffset;
		CString	m_strName;
		CString	m_strCity;
		CString	m_strAbbr;
		LONG	m_lBias;
		LONG	m_lBiasSTD;
		LONG	m_lBiasDST;
	    SYSTEMTIME	m_tSTD;
	    SYSTEMTIME	m_tDST;

		CTimeZone( void )
		{
			m_lBias    = 0;
			m_lBiasSTD = 0;
			m_lBiasDST = 0;
			m_tSTD = { 0 };
			m_tDST = { 0 };
		}
		bool	IsDST( CTime tNow );	
};

class CTimeZones
{
public:
		CTimeZones( void );
		CTimeZone&	operator[]( INT_PTR iIndex ){	return	m_aTimeZone[iIndex];	}
		INT_PTR	GetCount( void ){	return	m_aTimeZone.GetCount();	}
		long	GetBias( CString strAbbreviation );

protected:
		CArray	<CTimeZone, CTimeZone&>
			m_aTimeZone;

		void	ReadTimeZones( void );
		void	MakeAbbr( CTimeZone& tz );
};
