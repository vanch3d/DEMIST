// AnimShapePage.cpp : implementation file
//

#include "stdafx.h"
#include "simul.h"
#include "AnimShapePage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAnimShapePage dialog


CAnimShapePage::CAnimShapePage()
	: CAnimPage(CAnimShapePage::IDD)
{
	//{{AFX_DATA_INIT(CAnimShapePage)
	m_nLSize = 1;
	m_nShape = 0;
	//}}AFX_DATA_INIT
	m_BackColor.SetDefaultText("< None >");
	m_LineColor.SetDefaultText("< None >");
	m_clrBk = CLR_DEFAULT;
	m_clrLine = CLR_DEFAULT;
	m_bAllowBk = TRUE;
	m_ImageAggList.Create(IDB_VAR_ICONS,14,0,RGB(128,128,128));
}


void CAnimShapePage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAnimShapePage)
	DDX_Control(pDX, IDC_BCOLOR_OBJ, m_BackColorObj);
	DDX_Control(pDX, IDC_LCOLOR_OBJ, m_LineColorObj);
	DDX_Control(pDX, IDC_LSTYLE, m_cLineShape);
	DDX_Control(pDX, IDC_SHAPE, m_cShape);
	DDX_Control(pDX, IDC_SPIN_SIZE, m_cSSpin);
	DDX_Control(pDX, IDC_BCOLOR, m_BackColor);
	DDX_Control(pDX, IDC_LCOLOR, m_LineColor);
	DDX_Text(pDX, IDC_LSIZE, m_nLSize);
	DDV_MinMaxInt(pDX, m_nLSize, 1, 10);
	DDX_CBIndex(pDX, IDC_SHAPE, m_nShape);
	//}}AFX_DATA_MAP
	DDX_LinePicker(pDX, IDC_LSTYLE, m_nLineStyle);
	if (!pDX->m_bSaveAndValidate)
	{
		m_BackColor.SetBkColour(m_clrBk);
		m_LineColor.SetBkColour(m_clrLine);
		OnSelChange(1,IDC_LCOLOR);
		OnSelChange(1,IDC_BCOLOR);
		m_cSSpin.SetRange(1,10);
	}
	else
	{
		m_clrBk = m_BackColor.GetBkColour();
		m_clrLine = m_LineColor.GetBkColour();

	}
}


BEGIN_MESSAGE_MAP(CAnimShapePage, CPropertyPage)
	//{{AFX_MSG_MAP(CAnimShapePage)
	ON_CBN_SELCHANGE(IDC_LCOLOR_OBJ, OnSelchangeLcolorObj)
	ON_CBN_SELCHANGE(IDC_BCOLOR_OBJ, OnSelchangeBcolorObj)
	//}}AFX_MSG_MAP
    ON_MESSAGE(CPN_SELENDOK,OnSelChange)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAnimShapePage message handlers

void CAnimShapePage::OnSelchangeLcolorObj() 
{
	// TODO: Add your control notification handler code here
	int nb = m_LineColorObj.GetCurSel();
	if (nb==-1) return;
	COLORREF clrObj = m_LineColorObj.GetSelectedColorValue();
	//m_BackColor.SetBkColour(clrObj);
	m_LineColor.SetBkColour(clrObj);
	OnSelChange(0,IDC_LCOLOR);
}

void CAnimShapePage::OnSelchangeBcolorObj() 
{
	// TODO: Add your control notification handler code here
	int nb = m_BackColorObj.GetCurSel();
	if (nb==-1) return;

	COLORREF clrObj = m_BackColorObj.GetSelectedColorValue();
	m_BackColor.SetBkColour(clrObj);

	OnSelChange(0,IDC_BCOLOR);
	
}

LONG CAnimShapePage::OnSelChange(UINT lParam, LONG Param)
{
	if (Param == IDC_LCOLOR)
	{
		COLORREF ref = m_LineColor.GetBkColour();
		BOOL res = (ref == CLR_DEFAULT);

		CString mstr;
		mstr = res ? "< None >" : "";

		m_LineColor.SetWindowText(mstr);
		if (lParam) m_LineColorObj.SetCurSel(-1);
	}
	if (Param == IDC_BCOLOR)
	{
		COLORREF ref = m_BackColor.GetBkColour();
		BOOL res = (ref == CLR_DEFAULT);

		CString mstr;
		mstr = res ? "< None >" : "";

		m_BackColor.SetWindowText(mstr);
		if (lParam) m_BackColorObj.SetCurSel(-1);
	}
	return TRUE;
}

BOOL CAnimShapePage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_cShape.EnableWindow(m_bAllowBk);
	if (!m_bAllowBk)
	{
		m_cShape.SetCurSel(-1);
	}
	m_BackColor.EnableWindow(m_bAllowBk);
	m_BackColorObj.EnableWindow(m_bAllowBk);
	

	int nb = m_sVarList.GetSize();
	m_BackColorObj.ResetContent();
	m_LineColorObj.ResetContent();
	//m_BackColorObj.AddColor("< None >",-1);
	//m_LineColorObj.AddColor("< None >",-1);
	for (int i=0;i<nb;i++)
	{
		m_BackColorObj.AddColor(m_sVarList.GetAt(i),m_sVarColorList.GetAt(i));
		m_LineColorObj.AddColor(m_sVarList.GetAt(i),m_sVarColorList.GetAt(i));
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
