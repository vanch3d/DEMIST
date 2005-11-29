// AnimValuePage.cpp : implementation file
//

#include "stdafx.h"
#include "simul.h"
#include "AnimValuePage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAnimValuePage dialog


CAnimValuePage::CAnimValuePage()
	: CAnimPage(CAnimValuePage::IDD)
{
	//{{AFX_DATA_INIT(CAnimValuePage)
	m_iPrecision = 0;
	m_Align = -1;
	m_vValue = 0;
	m_bDisplayName = FALSE;
	m_bDisplayUnit = FALSE;
	m_bPrevTime = FALSE;
	//}}AFX_DATA_INIT

	m_ImageAggList.Create(IDB_VAR_ICONS,14,0,RGB(128,128,128));
}


void CAnimValuePage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAnimValuePage)
	DDX_Control(pDX, IDC_DATALISTEX, m_cValueEx);
	DDX_Control(pDX, IDC_SPINPREC, m_cSpinPrec);
	DDX_Text(pDX, IDC_PRECISION, m_iPrecision);
	DDV_MinMaxInt(pDX, m_iPrecision, 0, 10);
	DDX_CBIndex(pDX, IDC_ALIGN, m_Align);
	DDX_CBIndex(pDX, IDC_DATALISTEX, m_vValue);
	//X_LBIndex(pDX, IDC_DATALIST, m_vValue);
	DDX_Check(pDX, IDC_DISPLAY_NAME, m_bDisplayName);
	DDX_Check(pDX, IDC_DISPLAY_UNIT, m_bDisplayUnit);
	DDX_Check(pDX, IDC_PREVTIME, m_bPrevTime);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAnimValuePage, CPropertyPage)
	//{{AFX_MSG_MAP(CAnimValuePage)
	ON_BN_CLICKED(IDC_SETFONT, OnSetfont)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAnimValuePage message handlers

void CAnimValuePage::OnSetfont() 
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

BOOL CAnimValuePage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	m_cValueEx.SetImageList(&m_ImageAggList);


	m_cValueEx.SetImageList(&m_ImageAggList);
	m_cValueEx.ResetContent();
	{
		COMBOBOXEXITEM     cbi;
		CString            str = _T("< none >");

		cbi.mask = CBEIF_IMAGE | CBEIF_INDENT | 
					CBEIF_TEXT ;//| CBEIF_OVERLAY | CBEIF_SELECTEDIMAGE | CBEIF_TEXT;

		cbi.iItem = 0;
		cbi.pszText = str.GetBuffer(str.GetLength());
		cbi.cchTextMax = str.GetLength();
		cbi.iIndent = 0;//(i & 0x03);   //Set indentation according
				                    //to item position
		
		int nItem = m_cValueEx.InsertItem(&cbi);
		ASSERT(nItem == 0);
	}
	int nbS1 = m_sVarList.GetSize();
	int nbS2 = m_sOutcomeIcon.GetSize();
	for (int i=0;i<nbS1;i++)
	{
		CString mstr = m_sVarList.GetAt(i);
		UINT nb = m_sOutcomeIcon.GetAt(i);
		COMBOBOXEXITEM     cbi;

		cbi.mask = CBEIF_IMAGE | CBEIF_INDENT | CBEIF_SELECTEDIMAGE | 
					CBEIF_TEXT ; //| CBEIF_OVERLAY | CBEIF_SELECTEDIMAGE | CBEIF_TEXT;

		cbi.iItem = i+1;
		cbi.pszText = mstr.GetBuffer(mstr.GetLength());
		cbi.cchTextMax = mstr.GetLength();
		cbi.iIndent = 0;//(i & 0x03);   //Set indentation according
				                    //to item position
		cbi.iImage = nb;
		cbi.iSelectedImage = nb;

		int nItem = m_cValueEx.InsertItem(&cbi);
		ASSERT(nItem == i+1);
	}
	m_cValueEx.SetCurSel(m_vValue);



/*_cValue.ResetContent();
	m_cValue.AddString("< none >");

	int nb = m_sVarList.GetSize();
	for (i=0;i<nb;i++)
		m_cValue.AddString(m_sVarList[i]);
	m_cValue.SetCurSel(m_vValue);*/
	m_cSpinPrec.SetRange(0,10);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
