// AnimPictDensPage.cpp : implementation file
//

#include "stdafx.h"
#include "simul.h"
#include "AnimPictDensPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAnimPictDensPage dialog


CAnimPictDensPage::CAnimPictDensPage()
	: CAnimPage(CAnimPictDensPage::IDD)
{
	//{{AFX_DATA_INIT(CAnimPictDensPage)
	m_nData = 0;
	m_nDelta = 1;
	m_nAlign = 0;
	m_bLog = FALSE;
	m_bShow = FALSE;
	m_nDX = 0;
	m_nDY = 0;
	//}}AFX_DATA_INIT
}


void CAnimPictDensPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAnimPictDensPage)
	DDX_Control(pDX, IDC_DATA, m_cDataList);
	DDX_CBIndex(pDX, IDC_DATA, m_nData);
	DDX_Text(pDX, IDC_DELTA, m_nDelta);
	DDV_MinMaxInt(pDX, m_nDelta, 1, 10000);
	DDX_Radio(pDX, IDC_ALIGN, m_nAlign);
	DDX_Check(pDX, IDC_LOG, m_bLog);
	DDX_Check(pDX, IDC_SHOWVALUE, m_bShow);
	DDX_Text(pDX, IDC_XDEF, m_nDX);
	DDV_MinMaxInt(pDX, m_nDX, -1000, 1000);
	DDX_Text(pDX, IDC_YDEF, m_nDY);
	DDV_MinMaxInt(pDX, m_nDY, -1000, 1000);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAnimPictDensPage, CPropertyPage)
	//{{AFX_MSG_MAP(CAnimPictDensPage)
	ON_BN_CLICKED(IDC_LOG, OnSetLog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAnimPictDensPage message handlers

void CAnimPictDensPage::OnSetLog() 
{
	// TODO: Add your control notification handler code here
	CButton *pBtn = (CButton*)GetDlgItem(IDC_LOG);
	int nb = pBtn->GetCheck();
	CWnd *pWnd = GetDlgItem(IDC_DELTA);
	pWnd->EnableWindow(!nb);
}

BOOL CAnimPictDensPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	OnSetLog();
	m_cDataList.ResetContent();
	m_cDataList.AddString("< none >");

	int nb = m_sVarList.GetSize();
	for (int i=0;i<nb;i++)
		m_cDataList.AddString(m_sVarList[i]);
	m_cDataList.SetCurSel(m_nData);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
