// EditInterval.cpp : implementation file
//

#include "stdafx.h"
//#include "multisliderdemo.h"
#include "..\resource.h"

#include "EditInterval.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditInterval dialog


CEditInterval::CEditInterval(CWnd* pParent /*=NULL*/)
	: CHelpDlg(CEditInterval::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEditInterval)
	m_Value = 0.0f;
	m_strDescr = _T("");
	m_nStyle = -1;
	m_strHTML = _T("");
	//}}AFX_DATA_INIT
}


void CEditInterval::DoDataExchange(CDataExchange* pDX)
{
	CHelpDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEditInterval)
	DDX_Control(pDX, IDC_ICON4, m_cIconTest);
	DDX_Control(pDX, IDC_ICON3, m_cIconPred);
	DDX_Control(pDX, IDC_ICON2, m_cIconAct);
	DDX_Control(pDX, IDC_ICON1, m_cIconInf);
	DDX_Text(pDX, IDC_BP_VALUE, m_Value);
	DDX_Text(pDX, IDC_BP_DESCR, m_strDescr);
	DDX_Radio(pDX, IDC_BP_STYLE, m_nStyle);
	DDX_Text(pDX, IDC_BP_HTMLDOC, m_strHTML);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEditInterval, CHelpDlg)
	//{{AFX_MSG_MAP(CEditInterval)
	ON_BN_CLICKED(IDC_BP_BROWSE, OnBrowse)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditInterval message handlers

void CEditInterval::OnOK() 
{
	float prev;
	CWnd* wnd;
	CEdit* edit;

	prev = m_Value;
	UpdateData(TRUE);

	if(m_Value > m_Max || m_Value < m_Min)
	{
		m_Value = prev;
		UpdateData(FALSE);
		wnd = GetDlgItem(IDC_BP_VALUE);
		wnd->SetFocus();
		edit = (CEdit*)wnd;
		edit->SetSel(0, -1);
		return;
	}
	
	CHelpDlg::OnOK();
}

BOOL CEditInterval::OnInitDialog() 
{
	CHelpDlg::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_pImageList.Create(IDB_TIME_BREAKPOINT,16,0,RGB(255, 0, 255));
	DWORD nStyle = m_cIconInf.GetStyle();
	nStyle |= SS_ICON;
	::SetWindowLong(m_cIconInf,GWL_STYLE,nStyle);
	::SetWindowLong(m_cIconAct,GWL_STYLE,nStyle);
	::SetWindowLong(m_cIconPred,GWL_STYLE,nStyle);
	::SetWindowLong(m_cIconTest,GWL_STYLE,nStyle);
	m_cIconInf.SetIcon(m_pImageList.ExtractIcon(0));
	m_cIconAct.SetIcon(m_pImageList.ExtractIcon(1));
	m_cIconPred.SetIcon(m_pImageList.ExtractIcon(2));
	m_cIconTest.SetIcon(m_pImageList.ExtractIcon(3));
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CEditInterval::OnBrowse() 
{
	// TODO: Add your control notification handler code here
	
}
