// TraceConfigPage.cpp : implementation file
//

#include "stdafx.h"
#include "simul.h"
#include "TraceConfigPage.h"
#include <Prefs\Pref.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPrefTracePage dialog


CPrefTracePage::CPrefTracePage()
	: CPreferencePage(CPrefTracePage::IDD)
{
	//{{AFX_DATA_INIT(CPrefTracePage)
	m_bClose = CPref::CTrace.g_bCloseER;
	m_bOpen = CPref::CTrace.g_bOpenER;
	m_bSwitch = CPref::CTrace.g_bSwitchER;
	m_bPause = CPref::CTrace.g_bPauseSimul;
	m_bReview = CPref::CTrace.g_bReviewSimul;
	m_bRun = CPref::CTrace.g_bRunSimul;
	//}}AFX_DATA_INIT
}


void CPrefTracePage::DoDataExchange(CDataExchange* pDX)
{
	CPreferencePage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPrefTracePage)
	DDX_Check(pDX, IDC_TCLOSE, m_bClose);
	DDX_Check(pDX, IDC_TOPEN, m_bOpen);
	DDX_Check(pDX, IDC_TSWITCH, m_bSwitch);
	DDX_Check(pDX, IDC_TPAUSE, m_bPause);
	DDX_Check(pDX, IDC_TREVIEW, m_bReview);
	DDX_Check(pDX, IDC_TRUN, m_bRun);
	//}}AFX_DATA_MAP
	if (pDX->m_bSaveAndValidate)
	{
		CPref::CTrace.g_bCloseER = m_bClose;
		CPref::CTrace.g_bOpenER = m_bOpen;
		CPref::CTrace.g_bSwitchER = m_bSwitch;
		CPref::CTrace.g_bPauseSimul = m_bPause;
		CPref::CTrace.g_bReviewSimul = m_bReview;
		CPref::CTrace.g_bRunSimul = m_bRun;
	}
}


BEGIN_MESSAGE_MAP(CPrefTracePage, CPreferencePage)
	//{{AFX_MSG_MAP(CPrefTracePage)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPrefTracePage message handlers
