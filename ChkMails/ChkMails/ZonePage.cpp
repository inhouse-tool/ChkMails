// ZonePage.cpp : implementation file
//

#include "pch.h"
#include "ChkMails.h"
#include "ZonePage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#ifdef	UNICODE
#define	atoi		_wtoi
#endif//UNICODE

#define	TID_SECOND	1

IMPLEMENT_DYNAMIC( CZonePage, CPropertyPage )

///////////////////////////////////////////////////////////////////////////////////////
// Constructor

CZonePage::CZonePage( void )
	: CPropertyPage( IDD_ZONE )
{
	m_iZone = -1;
}

///////////////////////////////////////////////////////////////////////////////////////
// Overridden Functions

BOOL
CZonePage::OnInitDialog( void )
{
	CPropertyPage::OnInitDialog();

	CListCtrl*	pList = (CListCtrl*)GetDlgItem( IDC_LIST_TIMEZONE );

	DWORD	dwExStyle = pList->GetExtendedStyle();
	dwExStyle |= LVS_EX_FULLROWSELECT;
	dwExStyle |= LVS_EX_GRIDLINES;
	dwExStyle |= LVS_EX_CHECKBOXES;
	pList->SetExtendedStyle( dwExStyle );

	{
		int	iColumn = 0;
		pList->InsertColumn( iColumn++, _T(""),       LVCFMT_LEFT,   20 );
		pList->InsertColumn( iColumn++, _T("Time"),   LVCFMT_LEFT,   50 );
		pList->InsertColumn( iColumn++, _T("DST"),    LVCFMT_LEFT,   40 );
		pList->InsertColumn( iColumn++, _T("Abbr."),  LVCFMT_LEFT,   50 );
		pList->InsertColumn( iColumn++, _T("Cities"), LVCFMT_LEFT,  350 );
	}

	CTime	tNow = CTime::GetCurrentTime();
	int	n = (int)m_timezones.GetCount();
	int	iRow = 0;
	for	( int i = n-1; i >= 0; i-- ){
		CTimeZone&	tz = m_timezones[i];

		bool	bDST = tz.IsDST( tNow );
		int	nTime = tz.m_lBias;
		if	( bDST )
			nTime += tz.m_lBiasDST;
		TCHAR	chSign;
		if	( nTime <= 0 ){
			chSign = '+';
			nTime = -nTime;
		}
		else{
			chSign = '-';
		}
		nTime = (nTime/60)*100 + (nTime%60);
		CString	strTime;
		strTime.Format( _T("%c%02d%02d"), chSign, nTime/100, nTime%100 );
		pList->InsertItem(  iRow, _T("") );
		pList->SetItemText( iRow, 1, strTime );
		pList->SetItemText( iRow, 2, (!tz.m_tDST.wMonth)? _T("   "): bDST? _T("DST"): _T("STD") );
		pList->SetItemText( iRow, 3, tz.m_strAbbr );
		pList->SetItemText( iRow, 4, tz.m_strCity );
		pList->SetCheck(    iRow, BST_UNCHECKED );
		pList->SetItemData( iRow, iRow );
		iRow++;
	}

	pList->SetColumnWidth( 0, LVSCW_AUTOSIZE );
	pList->SetColumnWidth( 1, LVSCW_AUTOSIZE );
	pList->SetColumnWidth( 2, LVSCW_AUTOSIZE );
	pList->SetColumnWidth( 3, LVSCW_AUTOSIZE );
	pList->SetColumnWidth( 4, LVSCW_AUTOSIZE_USEHEADER );

	int	iChecked = -1;
	int	nRow = pList->GetItemCount();
	for	( iRow = 0; iRow < nRow; iRow++ ){
		CString	str = pList->GetItemText( iRow, 1 );
		if	( m_strTimes.Find( str ) >= 0 ){
			pList->SetCheck( iRow, TRUE );
			iChecked = iRow;
		}
	}
	if	( iChecked >= 0 )
		pList->EnsureVisible( iChecked, TRUE );

	SetTimer( TID_SECOND, 1000, NULL );
	OnTimer( TID_SECOND );

	return	TRUE;
}

void
CZonePage::OnOK( void )
{
	m_strTimes.Empty();
	CString	strLast;
	CListCtrl*	pList = (CListCtrl*)GetDlgItem( IDC_LIST_TIMEZONE );
	INT_PTR	nRow = pList->GetItemCount();
	for	( int iRow = 0; iRow < nRow; iRow++ )
		if	( pList->GetCheck( iRow ) ){
			CString	str = pList->GetItemText( iRow, 1 );
			if	( str != strLast ){
				m_strTimes += str + _T("\n");
				strLast = str;
			}
		}
}

///////////////////////////////////////////////////////////////////////////////////////
// Message Handlers

BEGIN_MESSAGE_MAP( CZonePage, CPropertyPage )
	ON_WM_TIMER()
	ON_NOTIFY( LVN_ITEMCHANGED, IDC_LIST_TIMEZONE, OnChangeList )
	ON_NOTIFY( HDN_ITEMCLICK, 0, OnClickHeader )
END_MESSAGE_MAP()

void
CZonePage::OnTimer( UINT_PTR nIDEvent )
{
	if	( nIDEvent == TID_SECOND ){
		int	nMinute = m_tNow.GetMinute();
		m_tNow = CTime::GetCurrentTime();
		if	( m_tNow.GetMinute() != nMinute )
			DrawLocalTime( m_iZone );
	}
	else
		CPropertyPage::OnTimer( nIDEvent );
}

#define	LVNI_UNCHECKED	0x1000
#define	LVNI_CHECKED	0x2000

void
CZonePage::OnChangeList( NMHDR* pNMHDR, LRESULT* pResult )
{
	if	( m_tNow == 0 )
		return;

	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>( pNMHDR );
	int	iRow    = pNMLV->iItem;
	int	iColumn = pNMLV->iSubItem;

	int	iChecked = -1;
	if	( iColumn == 0 )
		if	( ( pNMLV->uOldState & LVNI_UNCHECKED ) &&
			  ( pNMLV->uNewState & LVNI_CHECKED ) )
			iChecked = 1;
		else if	( ( pNMLV->uOldState & LVNI_CHECKED ) &&
			  ( pNMLV->uNewState & LVNI_UNCHECKED ) )
			iChecked = 0;
		else if	( ( pNMLV->uNewState & LVNI_SELECTED ) )
			;
		else
			return;

	CListCtrl*	pList = (CListCtrl*)GetDlgItem( IDC_LIST_TIMEZONE );

	if	( iChecked >= 0 ){
		CString	strTime = pList->GetItemText( iRow, 1 );
		if	( strTime.IsEmpty() )
			return;

		int	nRow = pList->GetItemCount();
		for	( int i = 0; i < nRow; i++ ){
			if	( i == iRow )
				continue;

			CString	str = pList->GetItemText( i, 1 );
			if	( str == strTime )
				pList->SetCheck( i, iChecked? TRUE: FALSE );
		}
		pList->SetItemState( iRow, LVNI_SELECTED, LVNI_SELECTED );
	}

	OnSelectItem( iRow );
}

void
CZonePage::OnClickHeader( NMHDR* pNMHDR, LRESULT* pResult )
{
	NMHEADER*	phdr = reinterpret_cast<NMHEADER*>( pNMHDR );

	CListCtrl*	pList = (CListCtrl*)GetDlgItem( IDC_LIST_TIMEZONE );
	CHeaderCtrl*	pHeader = pList->GetHeaderCtrl();

	int	iColumn = phdr->iItem;
	int	iOrder = 0;

	HDITEM	header = {};
	header.mask = HDI_FORMAT;
	pHeader->GetItem( iColumn, &header );

	if	( header.fmt & HDF_SORTUP ){
		header.fmt &= ~HDF_SORTUP;
		header.fmt |=  HDF_SORTDOWN;
		iOrder = 2;
	}
	else if	( header.fmt & HDF_SORTDOWN ){
		header.fmt &= ~HDF_SORTDOWN;
		iOrder = 0;
	}
	else{
		header.fmt |= HDF_SORTUP;
		iOrder = 1;
	}

	pHeader->SetItem( iColumn, &header );

	int	nColumn = pHeader->GetItemCount();
	for	( int i = 0; i < nColumn; i++ ){
		if	( i != iColumn ){
			pHeader->GetItem( i, &header );
			header.fmt &= ~(HDF_SORTUP | HDF_SORTDOWN);
			pHeader->SetItem( i, &header );
		}
	}

	SortArg	arg = {};

	arg.pList   = pList;
	arg.iColumn = 0;
	arg.iOrder  = 0;
	pList->SortItems( CompareZone, (DWORD_PTR)&arg );

	arg.pList   = pList;
	arg.iColumn = iColumn;
	arg.iOrder  = iOrder;
	pList->SortItems( CompareZone, (DWORD_PTR)&arg );

	*pResult = 0;
}

///////////////////////////////////////////////////////////////////////////////////////
// Specific Functions

void
CZonePage::OnSelectItem( int iRow )
{
	CListCtrl*	pList = (CListCtrl*)GetDlgItem( IDC_LIST_TIMEZONE );

	iRow = (int)pList->GetItemData( iRow );

	int	n = (int)m_timezones.GetCount()-1;
	m_iZone = n-iRow;

	CTimeZone&	tz = m_timezones[m_iZone];
	CString	str;
	str  = DecodeDST( tz.m_tDST );
	if	( str.IsEmpty() )
		(void)str.LoadString( IDS_TEXT_DST_NONE );
	else{
		CString	strHeader;
		(void)strHeader.LoadString( IDS_TEXT_DST_HEADER );
		str.Insert( 0, strHeader );
		str += _T(" - ");
		str += DecodeDST( tz.m_tSTD );
	}
	str.Insert( 0, tz.m_strKey + _T(":\n") );
	GetDlgItem( IDC_STATIC_DST )->SetWindowText( str );

	if	( m_tNow != 0 )
		DrawLocalTime( m_iZone );
}

CString
CZonePage::DecodeDST( SYSTEMTIME time )
{
	if	( !time.wDay )
		return	_T("");

	TCHAR*	apchWeek[] = { _T("Sunday"), _T("Monday"), _T("Tuesday"), _T("Wednesday"), _T("Thurseday"), _T("Friday"), _T("Saturday"), _T("???") };
	TCHAR*	apchMon[] = { _T("???"), _T("January"), _T("February"), _T("March"), _T("April"), _T("May"), _T("June"), _T("July"), _T("August"), _T("September"), _T("October"), _T("November"), _T("December"), _T("???") };
	CString	str;
	str +=
		( time.wDay == 1 )? _T("1st"):
		( time.wDay == 2 )? _T("2nd"):
		( time.wDay == 3 )? _T("3rd"):
		( time.wDay == 4 )? _T("4th"):
		( time.wDay == 5 )? _T("Last"): _T("????");
	str += " ";
	str += apchWeek[time.wDayOfWeek];
	str += " ";
	str += apchMon[time.wMonth];
	CString	strHour;
	strHour.Format( _T("%02d:%02d"), time.wHour, time.wMinute );
	str += " ";
	str += strHour;

	return	str;
}

void
CZonePage::DrawLocalTime( int iZone )
{
	CString	strTime;

	if	( m_iZone >= 0 ){
		TIME_ZONE_INFORMATION	tzi;
		GetTimeZoneInformation( &tzi );
		long	lBias = tzi.Bias;

		CTimeZone&	tz = m_timezones[iZone];
		CTimeSpan	spanZone( 0, 0, tz.m_lBias,    0 );
		CTimeSpan	spanHere( 0, 0,      lBias,    0 );
		CTimeSpan	spanDST(  0, 0, tz.m_lBiasDST, 0 );
		CTime		tLocal = m_tNow + spanHere - spanZone;

		if	( tz.IsDST( tLocal ) ){
			tLocal -= spanDST;
			if	(!tz.IsDST( tLocal ) )
				tLocal += spanDST;
		}

		strTime.Format( _T("%02d:%02d"), tLocal.GetHour(), tLocal.GetMinute() );
	}

	GetDlgItem( IDC_STATIC_LOCALTIME )->SetWindowText( strTime );
}

int	CALLBACK
CZonePage::CompareZone( LPARAM lItem1, LPARAM lItem2, LPARAM lParam )
{
	SortArg* pArg = (SortArg*)lParam;
	int	iRow1 = (int)lItem1;
	int	iRow2 = (int)lItem2;

	CListCtrl* pList = pArg->pList;
	int	iColumn  = pArg->iColumn;
	int	iOrder   = pArg->iOrder;

	if	( iOrder == 0 )
		if	( lItem1 > lItem2 )
			return	+1;
		else if	( lItem1 < lItem2 )
			return	-1;
		else
			return	0;

	int	nReturn = 0;
	if	( iColumn == 0 ){
		if	( !pList->GetCheck( iRow1 ) &&  pList->GetCheck( iRow2 ) )
			nReturn = +1;
		else if	(  pList->GetCheck( iRow1 ) && !pList->GetCheck( iRow2 ) )
			nReturn = -1;
	}

	else if	( iColumn == 1 ){
		CString	str1 = pList->GetItemText( iRow1, iColumn );
		CString	str2 = pList->GetItemText( iRow2, iColumn );
		int	nTime1 = atoi( str1 );
		int	nTime2 = atoi( str2 );
		if	( nTime1 > nTime2 )
			nReturn = +1;
		else if	( nTime1 < nTime2 )
			nReturn = -1;
	}

	else{
		CString	str1 = pList->GetItemText( iRow1, iColumn );
		CString	str2 = pList->GetItemText( iRow2, iColumn );

		if	( str1 > str2 )
			nReturn = +1;
		else if	( str1 < str2 )
			nReturn = -1;
	}

	if	( iOrder == 2 )
		nReturn	= -nReturn;

	return	nReturn;
}
