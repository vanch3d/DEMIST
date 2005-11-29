// AnimObjSizePage.cpp : implementation file
//

#include "stdafx.h"
#include "simul.h"
#include "AnimObjSizePage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAnimObjSizePage dialog


CAnimObjSizePage::CAnimObjSizePage()
	: CAnimPage(CAnimObjSizePage::IDD)
{
	//{{AFX_DATA_INIT(CAnimObjSizePage)
	m_nHeigth = 0;
	m_nWidth = 0;
	m_nDefHeight = 0;
	m_nDefWidth = 0;
	m_nDefHScale = 0.0;
	m_nDefWScale = 0.0;
	m_nDefArea = 0;
	m_nDefAScale = 0.0;
	m_nArea = 0;
	m_nDimension = 0;
	//}}AFX_DATA_INIT
	m_bResize = TRUE;
	m_nSizeType = SEL_SIZE_DIM;
	m_ImageAggList.Create(IDB_VAR_ICONS,14,0,RGB(128,128,128));
}


void CAnimObjSizePage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAnimObjSizePage)
	DDX_Control(pDX, IDC_CBAREA, m_cbArea);
	DDX_Control(pDX, IDC_CBWIDTH, m_cbWidth);
	DDX_Control(pDX, IDC_CBHEIGHT, m_cbHeigth);
	DDX_CBIndex(pDX, IDC_CBHEIGHT, m_nHeigth);
	DDX_CBIndex(pDX, IDC_CBWIDTH, m_nWidth);
	DDX_Text(pDX, IDC_HDEF, m_nDefHeight);
	DDX_Text(pDX, IDC_WDEF, m_nDefWidth);
	DDX_Text(pDX, IDC_HSCALE, m_nDefHScale);
	DDV_MinMaxDouble(pDX, m_nDefHScale, -1000., 1000.);
	DDX_Text(pDX, IDC_WSCALE, m_nDefWScale);
	DDV_MinMaxDouble(pDX, m_nDefWScale, -1000., 1000.);
	DDX_Text(pDX, IDC_ADEF, m_nDefArea);
	DDX_Text(pDX, IDC_ASCALE, m_nDefAScale);
	DDX_CBIndex(pDX, IDC_CBAREA, m_nArea);
	DDX_Radio(pDX, IDC_DIM, m_nDimension);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAnimObjSizePage, CPropertyPage)
	//{{AFX_MSG_MAP(CAnimObjSizePage)
	ON_CBN_SELCHANGE(IDC_CBWIDTH, OnSelchangeWidth)
	ON_CBN_SELCHANGE(IDC_CBHEIGHT, OnSelchangeHeight)
	ON_CBN_EDITCHANGE(IDC_CBAREA, OnSelchangeArea)
	ON_BN_CLICKED(IDC_DIM, OnSelectDim)
	ON_BN_CLICKED(IDC_DIM2, OnSelectArea)
	ON_BN_CLICKED(IDC_DIM3, OnSelectFit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAnimObjSizePage message handlers

BOOL CAnimObjSizePage::OnInitDialog() 
{
	BOOL res = CPropertyPage::OnInitDialog();


	m_cbArea.SetImageList(&m_ImageAggList);
	m_cbWidth.SetImageList(&m_ImageAggList);
	m_cbHeigth.SetImageList(&m_ImageAggList);

	m_cbArea.ResetContent();
	m_cbWidth.ResetContent();
	m_cbHeigth.ResetContent();
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
		
		int nItem = m_cbArea.InsertItem(&cbi);
		ASSERT(nItem == 0);
		nItem = m_cbWidth.InsertItem(&cbi);
		ASSERT(nItem == 0);
		nItem = m_cbHeigth.InsertItem(&cbi);
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

		int nItem = m_cbArea.InsertItem(&cbi);
		ASSERT(nItem == i+1);
		nItem = m_cbWidth.InsertItem(&cbi);
		ASSERT(nItem == i+1);
		nItem = m_cbHeigth.InsertItem(&cbi);
		ASSERT(nItem == i+1);
	}


/*
	CComboBox *pCB[3]= {&m_cbWidth,&m_cbHeigth,&m_cbArea};

	int nb = m_sVarList.GetSize();
	int size=0;
	for (int i=0;i<3;i++)
	{
		pCB[i]->ResetContent();
		pCB[i]->AddString("< none >");
		for (int j=0;j<nb;j++)
			pCB[i]->AddString(m_sVarList[j]);
		if (!i)
			size = pCB[i]->GetDroppedWidth();
		pCB[i]->SetDroppedWidth((int)1.5*size);
	}*/
	m_cbWidth.SetCurSel(m_nWidth);
	m_cbHeigth.SetCurSel(m_nHeigth);
	m_cbArea.SetCurSel(m_nArea);

/*	if (m_bResize)
	{
		CWnd *pWnd = GetDlgItem(IDC_HDEF);
		if (pWnd) pWnd->EnableWindow();
		pWnd = GetDlgItem(IDC_WDEF);
		if (pWnd) pWnd->EnableWindow();
	}*/

	BOOL bSeld = m_nSizeType & SEL_SIZE_DIM;
	CWnd *pWnd = GetDlgItem(IDC_DIM);
	if (pWnd) pWnd->EnableWindow(bSeld);
	BOOL bSela = m_nSizeType & SEL_SIZE_AREA;
	pWnd = GetDlgItem(IDC_DIM2);
	if (pWnd) pWnd->EnableWindow(bSela);
	BOOL bSelf = m_nSizeType & SEL_SIZE_FIT;
	pWnd = GetDlgItem(IDC_DIM3);
	if (pWnd) pWnd->EnableWindow(bSelf);

	if (bSeld) m_nDimension = 0;
	else if (bSela) m_nDimension = 1;
	else if (bSelf) m_nDimension = 2;
	else m_nDimension = -1;

	UpdateData(FALSE);
	EnableComponent(m_nDimension);
	//OnSelchangeWidth();
	//OnSelchangeHeight();
	//OnSelchangeArea();
	return res;
}

void CAnimObjSizePage::OnSelchangeWidth() 
{
	// TODO: Add your control notification handler code here
	int nb = m_cbWidth.GetCurSel();
	//CWnd *pWnd = GetDlgItem(IDC_WDEF);
	//if (pWnd) pWnd->EnableWindow(nb);
	CWnd *pWnd = GetDlgItem(IDC_WSCALE);
	if (pWnd) pWnd->EnableWindow(nb && m_nDimension==0);
}

void CAnimObjSizePage::OnSelchangeHeight() 
{
	// TODO: Add your control notification handler code here
	int nb = m_cbHeigth.GetCurSel();
	//CWnd *pWnd = GetDlgItem(IDC_HDEF);
	//if (pWnd) pWnd->EnableWindow(nb);
	CWnd *pWnd = GetDlgItem(IDC_HSCALE);
	if (pWnd) pWnd->EnableWindow(nb && m_nDimension==0);
}

void CAnimObjSizePage::OnSelchangeArea() 
{
	// TODO: Add your control notification handler code here
	int nb = m_cbArea.GetCurSel();
	//CWnd *pWnd = GetDlgItem(IDC_ADEF);
	//if (pWnd) pWnd->EnableWindow(nb);
	CWnd *pWnd = GetDlgItem(IDC_ASCALE);
	if (pWnd) pWnd->EnableWindow(nb && m_nDimension==1);
	
}

void CAnimObjSizePage::OnSelectDim() 
{
	// TODO: Add your control notification handler code here
	if (m_nDimension==0) return;
	if (UpdateData())
	{
		EnableComponent(m_nDimension);
	}
}

void CAnimObjSizePage::OnSelectArea() 
{
	// TODO: Add your control notification handler code here
	if (m_nDimension==1) return;
	if (UpdateData())
	{
		EnableComponent(m_nDimension);
	}
}

void CAnimObjSizePage::OnSelectFit() 
{
	// TODO: Add your control notification handler code here
	if (m_nDimension==2) return;
	if (UpdateData())
	{
		EnableComponent(m_nDimension);
	}
}

void CAnimObjSizePage::EnableComponent(int nb)
{
	/*	if (m_bResize)
	{
		CWnd *pWnd = GetDlgItem(IDC_HDEF);
		if (pWnd) pWnd->EnableWindow();
		pWnd = GetDlgItem(IDC_WDEF);
		if (pWnd) pWnd->EnableWindow();
	}*/

	CWnd *pWnd = GetDlgItem(IDC_WDEF);
	if (pWnd) pWnd->EnableWindow(nb==0 && m_bResize);
	pWnd = GetDlgItem(IDC_HDEF);
	if (pWnd) pWnd->EnableWindow(nb==0 && m_bResize);
	pWnd = GetDlgItem(IDC_ADEF);
	if (pWnd) pWnd->EnableWindow(nb==1);

	pWnd = GetDlgItem(IDC_CBWIDTH);
	if (pWnd) pWnd->EnableWindow(nb==0);
	pWnd = GetDlgItem(IDC_CBHEIGHT);
	if (pWnd) pWnd->EnableWindow(nb==0);
	pWnd = GetDlgItem(IDC_CBAREA);
	if (pWnd) pWnd->EnableWindow(nb==1);
	OnSelchangeWidth();
	OnSelchangeHeight();
	OnSelchangeArea();
}
