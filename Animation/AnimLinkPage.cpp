// AnimLinkPage.cpp : implementation file
//

#include "stdafx.h"
#include "simul.h"
#include "AnimLinkPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// CAnimLinkPage dialog


CAnimLinkPage::CAnimLinkPage()
	: CAnimPage(CAnimLinkPage::IDD)
{
	//{{AFX_DATA_INIT(CAnimLinkPage)
	m_nStyleConnector = 0;
	m_bArrowEnd = FALSE;
	m_bArrowStart = FALSE;
	m_nIDStart = 0;
	m_nIDEnd = 0;
	m_nAttachStart = 0;
	m_nAttachEnd = 0;
	//}}AFX_DATA_INIT
}


void CAnimLinkPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAnimLinkPage)
	DDX_Control(pDX, IDC_START, m_cStart);
	DDX_Control(pDX, IDC_END, m_cEnd);
	DDX_Radio(pDX, IDC_CONNECTOR, m_nStyleConnector);
	DDX_Check(pDX, IDC_ARROW_END, m_bArrowEnd);
	DDX_Check(pDX, IDC_ARROW_START, m_bArrowStart);
	DDX_CBIndex(pDX, IDC_START, m_nIDStart);
	DDX_CBIndex(pDX, IDC_END, m_nIDEnd);
	DDX_CBIndex(pDX, IDC_START_PT, m_nAttachStart);
	DDX_CBIndex(pDX, IDC_END_PT, m_nAttachEnd);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAnimLinkPage, CPropertyPage)
	//{{AFX_MSG_MAP(CAnimLinkPage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAnimLinkPage message handlers

BOOL CAnimLinkPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	int nb = m_sCpntList.GetSize();
	m_cStart.ResetContent();
	m_cEnd.ResetContent();
	m_cStart.AddString(_T("< None >"));
	m_cEnd.AddString(_T("< None >"));
	for (int i=0;i<nb;i++)
	{
		m_cStart.AddString(m_sCpntList.GetAt(i));
		m_cEnd.AddString(m_sCpntList.GetAt(i));
	}
	m_cStart.SetCurSel(m_nIDStart);
	m_cEnd.SetCurSel(m_nIDEnd);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
