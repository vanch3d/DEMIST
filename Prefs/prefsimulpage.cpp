// PrefSimulPage.cpp : implementation file
//

#include "stdafx.h"
#include "simul.h"
#include "PrefSimulPage.h"
#include <Prefs\Pref.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPrefSimulPage dialog


CPrefSimulPage::CPrefSimulPage()
	: CPreferencePage(CPrefSimulPage::IDD)
{
	//{{AFX_DATA_INIT(CPrefSimulPage)
	m_nSpeedAuth = CPref::g_nSpeedAcc;
	m_nSpeedLearn = CPref::g_nSpeedNorm;
	//}}AFX_DATA_INIT
	m_clrFuture = CPref::g_crFuture;
	m_clrPast = CPref::g_crPast;
}


void CPrefSimulPage::DoDataExchange(CDataExchange* pDX)
{
	CPreferencePage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPrefSimulPage)
	DDX_Control(pDX, IDC_SIM_PASTCLR, m_cPast);
	DDX_Control(pDX, IDC_SIM_FUTURECLR, m_cFuture);
	DDX_Text(pDX, IDC_SIM_SPEED_AUTH, m_nSpeedAuth);
	DDV_MinMaxInt(pDX, m_nSpeedAuth, 1, 1000);
	DDX_Text(pDX, IDC_SIM_SPEED_LEARN, m_nSpeedLearn);
	DDV_MinMaxInt(pDX, m_nSpeedLearn, 1, 1000);
	//}}AFX_DATA_MAP
	if (!pDX->m_bSaveAndValidate)
	{
		m_cPast.SetBkColour(m_clrPast);
		m_cPast.SetDefaultColor(RGB(0xFF, 0xFF, 0xE0));
		m_cFuture.SetBkColour(m_clrFuture);
		m_cFuture.SetDefaultColor(RGB(235,235,235));
	}
	else
	{
		m_clrPast = m_cPast.GetBkColour();
		m_clrFuture = m_cFuture.GetBkColour();
	}
}


BEGIN_MESSAGE_MAP(CPrefSimulPage, CPreferencePage)
	//{{AFX_MSG_MAP(CPrefSimulPage)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPrefSimulPage message handlers
