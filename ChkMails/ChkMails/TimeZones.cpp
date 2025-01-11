// TimeZones.cpp : implementation file
//

#include "pch.h"
#include "TimeZones.h"

CTimeZones::CTimeZones( void )
{
	ReadTimeZones();
}

long
CTimeZones::GetBias( CString strAbbreviation )
{
	INT_PTR	i, n = m_aTimeZone.GetCount();
	for	( i = 0; i < n; i++ ){
		CString	strAbbr = m_aTimeZone[i].m_strAbbr;
		if	( strAbbr.IsEmpty() )
			continue;
		if	( strAbbr == strAbbreviation )
			break;
		int	x = strAbbr.Find( '/' );
		if	( x < 0 )
			continue;
		if	( strAbbr.Left( x ) == strAbbreviation )
			break;
		if	( strAbbr.Mid( x+1 ) == strAbbreviation )
			break;
	}
	if	( i >= n ){
		TRACE( L"CTimeZones: Abbreviation '%s' is not found.\n", strAbbreviation );
		return	INT_MAX;
	}
	
	CTime	tNow = CTime::GetCurrentTime();
	CTimeZone&	tz = m_aTimeZone[i];
	long	lResult = tz.IsDST( tNow )? tz.m_lBiasDST: tz.m_lBiasSTD;
	lResult -= tz.m_lBias;

	return	lResult;
}

#pragma	warning( push )
#pragma	warning( disable : 6386 )

void
CTimeZones::ReadTimeZones( void )
{
	HKEY	hKey, hSubKey;
	CString	strBase = _T("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Time Zones");

	DWORD	dwResult = RegOpenKeyEx( HKEY_LOCAL_MACHINE, strBase, 0,
					KEY_QUERY_VALUE | KEY_ENUMERATE_SUB_KEYS, &hKey );

	if	( dwResult == ERROR_SUCCESS ){
		int	i = 0;
		TCHAR	achBuf[512];
		ULONG	ulSize = _countof( achBuf );
		while	( RegEnumKeyEx( hKey, i++, achBuf, &ulSize, NULL, NULL, NULL, NULL ) == ERROR_SUCCESS ){
			CString	strKey = achBuf;
			CString	str = strBase + "\\" + strKey;
			dwResult = RegOpenKeyEx( HKEY_LOCAL_MACHINE, str, 0,
							KEY_QUERY_VALUE, &hSubKey );
			if	( dwResult == ERROR_SUCCESS ){
				DWORD	dwValue;
				ulSize = sizeof( dwValue );
				dwResult = RegQueryValueEx( hSubKey, _T("IsObsolete"), NULL, NULL,
							(unsigned char*)&dwValue, (unsigned long*)&ulSize );
				if	( dwResult == ERROR_SUCCESS && dwValue )
					;
				else{
					CTimeZone	tz;
					tz.m_strKey = strKey;

					ulSize = sizeof( achBuf );
					dwResult = RegQueryValueEx( hSubKey, _T("Display"), NULL, NULL,
								(unsigned char*)achBuf, (unsigned long*)&ulSize );
					str = achBuf;
					if	( str.GetAt( 0 ) == '(' ){
						int	x = str.Find( ')', 1 );
						tz.m_strOffset = str.Mid( 1, x-1 );
						str.Delete( 0, x+1 );
						str.TrimLeft();
					}
					tz.m_strCity = str;

					ulSize = sizeof( achBuf );
					dwResult = RegQueryValueEx( hSubKey, _T("Std"), NULL, NULL,
								(unsigned char*)achBuf, (unsigned long*)&ulSize );
					tz.m_strName = achBuf;

					ulSize = (unsigned long)( sizeof( tz ) - ( (BYTE*)&tz.m_lBias - (BYTE*)&tz ) );
					dwResult = RegQueryValueEx( hSubKey, _T("TZI"), NULL, NULL,
							(unsigned char*)&tz.m_lBias, (unsigned long*)&ulSize );

					MakeAbbr( tz );

					INT_PTR	i, n = m_aTimeZone.GetCount();
					for	( i = 0; i < n; i++ )
						if	( m_aTimeZone[i].m_lBias  == tz.m_lBias )
							break;
						else if	( m_aTimeZone[i].m_lBias  <  tz.m_lBias )
							break;

					if	( tz.m_strName.Left( 3 ) == _T("UTC") )
						;
					else if	( i >= n )
						m_aTimeZone.Add( tz );
					else
						m_aTimeZone.InsertAt( i, tz );
				}
				RegCloseKey( hSubKey );
			}
			ulSize = _countof( achBuf );
		}
		RegCloseKey( hKey );
	}
}

#pragma	warning( pop )

void
CTimeZones::MakeAbbr( CTimeZone& tz )
{
	static	struct{
		TCHAR*	pchAbbr;
		TCHAR*	pchKey;
	}
		astAbbr[] =
	{
			{ _T("AFT"),		_T("Afghanistan Standard Time") },
			{ _T("AKST/AKDT"),	_T("Alaskan Standard Time") },
			{ _T("HAST/HADT"),	_T("Aleutian Standard Time") },
			{ _T("KRAT"),		_T("Altai Standard Time") },
			{ _T(""),		_T("Arab Standard Time") },
			{ _T(""),		_T("Arabian Standard Time") },
			{ _T(""),		_T("Arabic Standard Time") },
			{ _T("ART"),		_T("Argentina Standard Time") },
			{ _T("SAMT"),		_T("Astrakhan Standard Time") },
			{ _T("AST/ADT"),	_T("Atlantic Standard Time") },
			{ _T("ACT"),		_T("AUS Central Standard Time") },
			{ _T("AWCST"),		_T("Aus Central W. Standard Time") },
			{ _T("AEST/AEDT"),	_T("AUS Eastern Standard Time") },
			{ _T("AZT"),		_T("Azerbaijan Standard Time") },
			{ _T("AZOT/AZOST"),	_T("Azores Standard Time") },
			{ _T("BRT"),		_T("Bahia Standard Time") },
			{ _T("BST"),		_T("Bangladesh Standard Time") },
			{ _T("FET"),		_T("Belarus Standard Time") },
			{ _T(""),		_T("Bougainville Standard Time") },		// BST is for Bangladesh Standard Time
			{ _T(""),		_T("Canada Central Standard Time") },		// CST w/o DST
			{ _T("CVT"),		_T("Cape Verde Standard Time") },
			{ _T("AMT"),		_T("Caucasus Standard Time") },
			{ _T("ACST/ACDT"),	_T("Cen. Australia Standard Time") },
			{ _T(""),		_T("Central America Standard Time") },		// CST w/o DST
			{ _T("KGT"),		_T("Central Asia Standard Time") },
			{ _T("AMT"),		_T("Central Brazilian Standard Time") },
			{ _T("CET/CEST"),	_T("Central Europe Standard Time") },
			{ _T("CET/CEST"),	_T("Central European Standard Time") },
			{ _T("NCT"),		_T("Central Pacific Standard Time") },		// PST is for PST/PDT
			{ _T("CST/CDT"),	_T("Central Standard Time") },
			{ _T(""),		_T("Central Standard Time (Mexico)") },
			{ _T("CHAST/CHADT"),	_T("Chatham Islands Standard Time") },
			{ _T(""),		_T("China Standard Time") },			// CST is for Central Standard Time os USA
			{ _T("CST/CDT"),	_T("Cuba Standard Time") },			// CST is for Central Standard Time os USA
			{ _T(""),		_T("Dateline Standard Time") },			// who lives there?
			{ _T("EAT"),		_T("E. Africa Standard Time") },
			{ _T("AEST"),		_T("E. Australia Standard Time") },
			{ _T("EET/EEST"),	_T("E. Europe Standard Time") },
			{ _T("BRT"),		_T("E. South America Standard Time") },
			{ _T("EAST/EASST"),	_T("Easter Island Standard Time") },
			{ _T("EST/EDT"),	_T("Eastern Standard Time") },
			{ _T("EST"),		_T("Eastern Standard Time (Mexico)") },
			{ _T("EET/EEST"),	_T("Egypt Standard Time") },
			{ _T("YEKT"),		_T("Ekaterinburg Standard Time") },
			{ _T("FJT"),		_T("Fiji Standard Time") },
			{ _T("EET/EEST"),	_T("FLE Standard Time") },
			{ _T("GET"),		_T("Georgian Standard Time") },
			{ _T("GMT/BST"),	_T("GMT Standard Time") },			// They send "GMT" instead of "UTC" w/o DST
			{ _T("WGT/WGST"),	_T("Greenland Standard Time") },
			{ _T("GMT"),		_T("Greenwich Standard Time") },		// UTC+0000 w/o DST
			{ _T("EET/EEST"),	_T("GTB Standard Time") },
			{ _T("EST/EDT"),	_T("Haiti Standard Time") },
			{ _T("HST"),		_T("Hawaiian Standard Time") },
			{ _T("IST"),		_T("India Standard Time") },
			{ _T("IRST"),		_T("Iran Standard Time") },
			{ _T(""),		_T("Israel Standard Time") },			// IST is for India Standard Time
			{ _T("EEST"),		_T("Jordan Standard Time") },			// -> EEST w/o DST
			{ _T("KALT"),		_T("Kaliningrad Standard Time") },
			{ _T("PETT"),		_T("Kamchatka Standard Time") },
			{ _T("KST"),		_T("Korea Standard Time") },
			{ _T("EET"),		_T("Libya Standard Time") },
			{ _T("LINT"),		_T("Line Islands Standard Time") },
			{ _T("LHST/LHDT"),	_T("Lord Howe Standard Time") },
			{ _T("MAGT"),		_T("Magadan Standard Time") },
			{ _T("CLT/CLST"),	_T("Magallanes Standard Time") },
			{ _T("MART"),		_T("Marquesas Standard Time") },
			{ _T("MUT"),		_T("Mauritius Standard Time") },
			{ _T("EET/EEST"),	_T("Middle East Standard Time") },
			{ _T("UYT"),		_T("Montevideo Standard Time") },
			{ _T("WET/WEST"),	_T("Morocco Standard Time") },
			{ _T("MST/MDT"),	_T("Mountain Standard Time") },
			{ _T("MST"),		_T("Mountain Standard Time (Mexico)") },
			{ _T("MMT"),		_T("Myanmar Standard Time") },
			{ _T("KRAT"),		_T("N. Central Asia Standard Time") },
			{ _T("CAT"),		_T("Namibia Standard Time") },
			{ _T("NPT"),		_T("Nepal Standard Time") },
			{ _T("NZST/NZDT"),	_T("New Zealand Standard Time") },
			{ _T("NST/NDT"),	_T("Newfoundland Standard Time") },
			{ _T("NFT/NFDT"),	_T("Norfolk Standard Time") },
			{ _T("IRKT"),		_T("North Asia East Standard Time") },
			{ _T("KRAT"),		_T("North Asia Standard Time") },
			{ _T("PYT"),		_T("North Korea Standard Time") },
			{ _T("OMST"),		_T("Omsk Standard Time") },
			{ _T("CLT/CLST"),	_T("Pacific SA Standard Time") },
			{ _T("PST/PDT"),	_T("Pacific Standard Time (Mexico)") },
			{ _T("PST/PDT"),	_T("Pacific Standard Time") },
			{ _T("PKT"),		_T("Pakistan Standard Time") },
			{ _T(""),		_T("Paraguay Standard Time") },			// PYT is for N.Korea
			{ _T("QYZT"),		_T("Qyzylorda Standard Time") },
			{ _T("CET/CEST"),	_T("Romance Standard Time") },
			{ _T("MSK"),		_T("Russian Standard Time") },
			{ _T("SAMT"),		_T("Russia Time Zone 3") },
			{ _T("MAGT"),		_T("Russia Time Zone 10") },
			{ _T("PETT"),		_T("Russia Time Zone 11") },
			{ _T("BRT"),		_T("SA Eastern Standard Time") },
			{ _T(""),		_T("SA Pacific Standard Time") },
			{ _T(""),		_T("SA Western Standard Time") },
			{ _T("PMST/PMDT"),	_T("Saint Pierre Standard Time") },
			{ _T("SAKT"),		_T("Sakhalin Standard Time") },
			{ _T("SST"),		_T("Samoa Standard Time") },
			{ _T(""),		_T("Sao Tome Standard Time") },
			{ _T("SAMST"),		_T("Saratov Standard Time") },
			{ _T("WIB"),		_T("SE Asia Standard Time") },
			{ _T("SGT"),		_T("Singapore Standard Time") },
			{ _T("SAST"),		_T("South Africa Standard Time") },
			{ _T("CAT"),		_T("South Sudan Standard Time") },
			{ _T("SLST"),		_T("Sri Lanka Standard Time") },
			{ _T("CAT"),		_T("Sudan Standard Time") },
			{ _T("EEST"),		_T("Syria Standard Time") },
			{ _T(""),		_T("Taipei Standard Time") },			// CST is for Central Standard Time os USA
			{ _T("AET/AEDT"),	_T("Tasmania Standard Time") },
			{ _T("BRT"),		_T("Tocantins Standard Time") },
			{ _T("JST"),		_T("Tokyo Standard Time") },
			{ _T("KRAT"),		_T("Tomsk Standard Time") },
			{ _T("TOT"),		_T("Tonga Standard Time") },
			{ _T("YAKT"),		_T("Transbaikal Standard Time") },
			{ _T("EEST"),		_T("Turkey Standard Time") },
			{ _T("EST/EDT"),	_T("Turks And Caicos Standard Time") },
			{ _T("ULAT"),		_T("Ulaanbaatar Standard Time") },
			{ _T("EST/EDT"),	_T("US Eastern Standard Time") },
			{ _T("MST"),		_T("US Mountain Standard Time") },
			{ _T("UTC"),		_T("UTC") },
			{ _T("VET"),		_T("Venezuela Standard Time") },
			{ _T("VLAT"),		_T("Vladivostok Standard Time") },
			{ _T("MSK"),		_T("Volgograd Standard Time") },
			{ _T("AWST"),		_T("W. Australia Standard Time") },
			{ _T("WAT"),		_T("W. Central Africa Standard Time") },
			{ _T("CET/CEST"),	_T("W. Europe Standard Time") },		// Not WET/WEST
			{ _T("HOVT"),		_T("W. Mongolia Standard Time") },
			{ _T("WAST"),		_T("West Asia Standard Time") },
			{ _T("PSST/PSDT"),	_T("West Bank Standard Time") },
			{ _T("ChST"),		_T("West Pacific Standard Time") },		// PST is for PST/PDT
			{ _T("YAKT"),		_T("Yakutsk Standard Time") },
			{ _T("YST"),		_T("Yukon Standard Time") },
			{ NULL,			NULL }
	};

	CString	strAbbr;

	int	i;
	for	( i = 0; astAbbr[i].pchKey; i++ )
		if	( tz.m_strKey == astAbbr[i].pchKey )
			break;
	if	( astAbbr[i].pchKey )
		strAbbr = astAbbr[i].pchAbbr;
	else
		strAbbr = _T("(left)");

	tz.m_strAbbr = strAbbr;
}

bool
CTimeZone::IsDST( CTime tNow )
{
	if	( !m_tDST.wMonth )
		return	false;

	CTimeSpan	spanDay(  1, 0, 0, 0 );
	CTimeSpan	spanWeek( 7, 0, 0, 0 );
	int		nTh;

	nTh = m_tDST.wDay;
	CTime		tIn( tNow.GetYear(), m_tDST.wMonth, 1, m_tDST.wHour, 0, 0, 0 );
	while	( tIn.GetDayOfWeek() != m_tDST.wDayOfWeek+1 )
		tIn += spanDay;
	if	( nTh == 5 ){
		while	( tIn.GetMonth() == m_tDST.wMonth )
			tIn += spanWeek;
		tIn -= spanWeek;
	}
	else
		for	( int i = 1; i < nTh; i++ )
			tIn += spanWeek;

	nTh = m_tSTD.wDay;
	CTime		tOut( tNow.GetYear(), m_tSTD.wMonth, 1, m_tSTD.wHour, 0, 0, 0 );
	while	( tOut.GetDayOfWeek() != m_tSTD.wDayOfWeek+1 )
		tOut += spanDay;
	if	( nTh == 5 ){
		while	( tOut.GetMonth() == m_tSTD.wMonth )
			tOut += spanWeek;
		tOut -= spanWeek;
	}
	else
		for	( int i = 1; i < nTh; i++ )
			tOut += spanWeek;

	if	( tOut < tIn )
		tOut += CTimeSpan( 365, 0, 0, 0 );

	if	( tNow >= tIn && tNow < tOut )
		return	true;
	else
		return	false;
}
