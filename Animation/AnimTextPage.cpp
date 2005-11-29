// AnimTextPage.cpp : implementation file
//

#include "stdafx.h"
#include "simul.h"
#include "AnimTextPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAnimTextPage dialog

CAnimTextPage::CAnimTextPage()
	: CAnimPage(CAnimTextPage::IDD)
{
	//{{AFX_DATA_INIT(CAnimTextPage)
	m_Align = 0;
	m_TextInput = _T("");
	//}}AFX_DATA_INIT
}


void CAnimTextPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAnimTextPage)
	DDX_CBIndex(pDX, IDC_ALIGN, m_Align);
	DDX_Text(pDX, IDC_EDITTEXT, m_TextInput);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAnimTextPage, CPropertyPage)
	//{{AFX_MSG_MAP(CAnimTextPage)
	ON_BN_CLICKED(IDC_SETFONT, OnSetfont)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAnimTextPage message handlers

void CAnimTextPage::OnSetfont() 
{
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

BOOL CAnimTextPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
