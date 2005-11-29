// AnimSectorPage.cpp : implementation file
//

#include "stdafx.h"
#include "simul.h"
#include "AnimSectorPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAnimSectorPage CPropertyPage

BEGIN_MESSAGE_MAP(CAnimSectorPage, CPropertyPage)
	//{{AFX_MSG_MAP(CAnimSectorPage)
	ON_BN_CLICKED(IDC_SETFONT, OnSetFont)
	ON_BN_CLICKED(IDC_TITLE_TEXT, OnSetTitleType)
	ON_BN_CLICKED(IDC_TITLE_VALUE, OnSetTitleType)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CAnimSectorPage::CAnimSectorPage()
	: CAnimPage(CAnimSectorPage::IDD)
{
	//{{AFX_DATA_INIT(CAnimSectorPage)
	m_bLocked = FALSE;
	m_bTitleText = 0;
	m_nValueID = 0;
	m_bDisplayUnit = FALSE;
	m_bPrevTime = FALSE;
	m_sTitle = _T("");
	m_nPrec = 1;
	//}}AFX_DATA_INIT
}


void CAnimSectorPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAnimSectorPage)
	DDX_Control(pDX, IDC_SPINPREC, m_cSpinPrec);
	DDX_Control(pDX, IDC_DATALIST, m_cValues);
	DDX_LBIndex(pDX, IDC_DATALIST, m_nValueID);
	DDX_Check(pDX, IDC_LOCKED, m_bLocked);
	DDX_Radio(pDX, IDC_TITLE_TEXT, m_bTitleText);
	DDX_Check(pDX, IDC_DISPLAY_UNIT, m_bDisplayUnit);
	DDX_Check(pDX, IDC_PREVTIME, m_bPrevTime);
	DDX_Text(pDX, IDC_EDITTEXT, m_sTitle);
	DDX_Text(pDX, IDC_PRECISION, m_nPrec);
	DDV_MinMaxInt(pDX, m_nPrec, 1, 10);
	//}}AFX_DATA_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CAnimSectorPage message handlers

BOOL CAnimSectorPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_cSpinPrec.SetRange(1,10);
	m_cValues.ResetContent();
	//m_cValues.AddString("< none >");

	int nb = m_sVarList.GetSize();
	for (int i=0;i<nb;i++)
		m_cValues.AddString(m_sVarList[i]);
	m_cValues.SetCurSel(m_nValueID);

	OnSetTitleType();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAnimSectorPage::OnSetFont() 
{
	// TODO: Add your control notification handler code here
	LOGFONT lf = m_lfDefFont;
	//int x=m_font.GetLogFont(&lf);
	//::GetObject(GetStockObject(SYSTEM_FONT), sizeof(LOGFONT), &lf);

	CFontDialog dlg(&lf, CF_EFFECTS|CF_SCREENFONTS|CF_INITTOLOGFONTSTRUCT);
	dlg.m_cf.rgbColors = m_color;
	if (dlg.DoModal() == IDOK)
	{
		// switch to new font.
		m_font.DeleteObject();
		if (m_font.CreateFontIndirect(&lf))
		{
			CWaitCursor wait;
			//SetFont(&m_font);
			m_lfDefFont = lf;
			m_color=dlg.GetColor();
		}
	}
	
}

void CAnimSectorPage::OnSetTitleType() 
{
	// TODO: Add your control notification handler code here
	CButton *pBtn = (CButton *)GetDlgItem(IDC_TITLE_TEXT);
	if (!pBtn) return;
	BOOL bText = pBtn->GetCheck();

	CWnd *pWnd = GetDlgItem(IDC_EDITTEXT);
	if (pWnd) pWnd->EnableWindow(bText);
	pWnd = GetDlgItem(IDC_DATALIST);
	if (pWnd) pWnd->EnableWindow(!bText);
	pWnd = GetDlgItem(IDC_DISPLAY_UNIT);
	if (pWnd) pWnd->EnableWindow(!bText);
	pWnd = GetDlgItem(IDC_PRECISION);
	if (pWnd) pWnd->EnableWindow(!bText);
	pWnd = GetDlgItem(IDC_SPINPREC);
	if (pWnd) pWnd->EnableWindow(!bText);
	pWnd = GetDlgItem(IDC_PREVTIME);
	if (pWnd) pWnd->EnableWindow(!bText);
	pWnd = GetDlgItem(IDC_MYSTATIC);
	if (pWnd) pWnd->EnableWindow(!bText);
}
