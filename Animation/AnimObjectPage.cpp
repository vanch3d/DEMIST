// AnimObjectPage.cpp : implementation file
//

#include "stdafx.h"
#include "simul.h"
#include "AnimObjectPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(CAnimObjPosPage, CPropertyPage)
	//{{AFX_MSG_MAP(CAnimObjPosPage)
	ON_CBN_SELCHANGE(IDC_HORIZ, OnSelchangeHoriz)
	ON_CBN_SELCHANGE(IDC_VERTIC, OnSelchangeVertic)
	ON_BN_CLICKED(IDC_ATTACHED, OnSelectAttached)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAnimObjPosPage

CAnimObjPosPage::CAnimObjPosPage()
	: CAnimPage(CAnimObjPosPage::IDD)
{
	//{{AFX_DATA_INIT(CAnimObjPosPage)
	m_xScale = 1.0;
	m_yScale = 1.0;
	m_vHoriz = 0;
	m_vVert = 0;
	m_nDefYPos = 0;
	m_nDefXPos = 0;
	m_bShowTrace = FALSE;
	m_bAttached = FALSE;
	m_nComponent = -1;
	m_bShowAxes = FALSE;
	//}}AFX_DATA_INIT


	m_ImageAggList.Create(IDB_VAR_ICONS,14,0,RGB(128,128,128));
}

void CAnimObjPosPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAnimObjPosPage)
	DDX_Control(pDX, IDC_COMPONENT, m_cComponent);
	DDX_Control(pDX, IDC_VERTIC, m_cVert);
	DDX_Control(pDX, IDC_HORIZ, m_cHoriz);
	DDX_Text(pDX, IDC_XSCALE, m_xScale);
	DDV_MinMaxDouble(pDX, m_xScale, -1000., 1000.);
	DDX_Text(pDX, IDC_YSCALE, m_yScale);
	DDV_MinMaxDouble(pDX, m_yScale, -1000., 1000.);
	DDX_CBIndex(pDX, IDC_HORIZ, m_vHoriz);
	DDX_CBIndex(pDX, IDC_VERTIC, m_vVert);
	DDX_Text(pDX, IDC_YDEF, m_nDefYPos);
	DDX_Text(pDX, IDC_XDEF, m_nDefXPos);
	DDX_Check(pDX, IDC_SHOWTRACE, m_bShowTrace);
	DDX_Check(pDX, IDC_ATTACHED, m_bAttached);
	DDX_CBIndex(pDX, IDC_COMPONENT, m_nComponent);
	DDX_Check(pDX, IDC_SHOWAXIS, m_bShowAxes);
	//}}AFX_DATA_MAP
}



/////////////////////////////////////////////////////////////////////////////
// CAnimObjPosPage message handlers

BOOL CAnimObjPosPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
/*	CComboBox *pCB[2]= {&m_cHoriz,&m_cVert};

	int nb = m_sVarList.GetSize();
	int size=0;
	for (int i=1;i<2;i++)
	{
		pCB[i]->ResetContent();
		pCB[i]->AddString("< none >");
		for (int j=0;j<nb;j++)
			pCB[i]->AddString(m_sVarList[j]);
		if (!i)
			size = pCB[i]->GetDroppedWidth();
		pCB[i]->SetDroppedWidth((int)1.5*size);
	}*/

	m_cHoriz.SetImageList(&m_ImageAggList);
	m_cVert.SetImageList(&m_ImageAggList);

	m_cHoriz.ResetContent();
	m_cVert.ResetContent();
	{
		COMBOBOXEXITEM     cbi;
		CString            str = _T("< none >");

		cbi.mask = CBEIF_IMAGE | CBEIF_INDENT | 
					CBEIF_TEXT /*| CBEIF_OVERLAY |
					CBEIF_SELECTEDIMAGE | CBEIF_TEXT*/;

		cbi.iItem = 0;
		cbi.pszText = str.GetBuffer(str.GetLength());
		cbi.cchTextMax = str.GetLength();
		cbi.iIndent = 0;//(i & 0x03);   //Set indentation according
				                    //to item position
		
		int nItem = m_cHoriz.InsertItem(&cbi);
		ASSERT(nItem == 0);
		nItem = m_cVert.InsertItem(&cbi);
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
					CBEIF_TEXT /*| CBEIF_OVERLAY |
					CBEIF_SELECTEDIMAGE | CBEIF_TEXT*/;

		cbi.iItem = i+1;
		cbi.pszText = mstr.GetBuffer(mstr.GetLength());
		cbi.cchTextMax = mstr.GetLength();
		cbi.iIndent = 0;//(i & 0x03);   //Set indentation according
				                    //to item position
		cbi.iImage = nb;
		cbi.iSelectedImage = nb;

		int nItem = m_cHoriz.InsertItem(&cbi);
		ASSERT(nItem == i+1);
		nItem = m_cVert.InsertItem(&cbi);
		ASSERT(nItem == i+1);
	}





/*	m_cHoriz.ResetContent();
	m_cVert.ResetContent();
	m_cHoriz.AddString("< none >");
	m_cVert.AddString("< none >");
	int nb = m_sVarList.GetSize();
	nb = m_cHoriz.GetDroppedWidth();
	m_cHoriz.SetDroppedWidth((int)1.5*nb);*/
	m_cHoriz.SetCurSel(m_vHoriz);
	m_cVert.SetCurSel(m_vVert);

	int nb = m_sCpntList.GetSize();
	m_cComponent.ResetContent();
	for (i=0;i<nb;i++)
	{
		m_cComponent.AddString(m_sCpntList.GetAt(i));
	}
	m_cComponent.SetCurSel(m_nComponent);

	OnSelchangeHoriz();
	OnSelchangeVertic();
	OnSelectAttached();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAnimObjPosPage::OnSelchangeHoriz() 
{
	// TODO: Add your control notification handler code here
	int nb = m_cHoriz.GetCurSel();
	CWnd *pWnd = GetDlgItem(IDC_XDEF);
	//if (pWnd) pWnd->EnableWindow(nb);
	pWnd = GetDlgItem(IDC_XSCALE);
	if (pWnd) pWnd->EnableWindow(nb);
}

void CAnimObjPosPage::OnSelchangeVertic() 
{
	// TODO: Add your control notification handler code here
	int nb = m_cVert.GetCurSel();
	CWnd *pWnd = GetDlgItem(IDC_YDEF);
	//if (pWnd) pWnd->EnableWindow(nb);
	pWnd = GetDlgItem(IDC_YSCALE);
	if (pWnd) pWnd->EnableWindow(nb);
}

void CAnimObjPosPage::OnSelectAttached() 
{
	// TODO: Add your control notification handler code here
	CButton *pWnd = (CButton *)GetDlgItem(IDC_ATTACHED);
	if (pWnd) 
		m_cComponent.EnableWindow(pWnd->GetCheck());
	
}
