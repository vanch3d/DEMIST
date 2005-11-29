// DefineLUPropSheet.cpp : implementation file
//

#include "stdafx.h"
#include "simul.h"

#include "DefineLUPropSheet.h"

#include <MSimulation\Model.h>
#include <MSimulation\AuthorModel.h>
#include <MInstruction\LearningUnit.h>

#include <MInstruction\DefineExpSetDlg.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLUPropertyPage property page
CLUPropertyPage::CLUPropertyPage() : CPropertyPage()
{
}

CLUPropertyPage::CLUPropertyPage(UINT nIDTemplate) : CPropertyPage(nIDTemplate)
{
}

CLUPropertyPage::~CLUPropertyPage()
{
}

/////////////////////////////////////////////////////////////////////////////
// CLUModelPage dialog
/////////////////////////////////////////////////////////////////////////////

CLUModelPage::CLUModelPage(CSimModel *pSim /*= NULL*/,CLearningUnit *pLU /*= NULL*/)
	: CLUPropertyPage(CLUModelPage::IDD)
{
	m_pSim = pSim;
	m_pLU = pLU;

	//{{AFX_DATA_INIT(CLUModelPage)
	m_dRangeX = 0;
	m_dRangeY = 0;
	//}}AFX_DATA_INIT
}

CLUModelPage::~CLUModelPage()
{
}

void CLUModelPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);

	double nDefH = 1.;

	//{{AFX_DATA_MAP(CLUModelPage)
	DDX_Control(pDX, IDC_SIM_MODEL, m_wndModel);
	DDX_Control(pDX, IDD_SIM_METHOD, m_wndNumMethod);
	DDX_Control(pDX, IDD_SIM_H, m_wndHEuler);
	DDX_Control(pDX, IDC_SIM_TIMERSPIN, m_wndSpinTimer);
	DDX_Text(pDX, IDC_SIM_LUNAME, m_pLU->m_sLUName);
	//}}AFX_DATA_MAP

	if (!pDX->m_bSaveAndValidate)
	{
		CMdEquation *pEqu = NULL;
		// Add Model names
		m_wndModel.ResetContent();
		for (int i=0;i<m_pSim->m_cEquSet.GetSize();i++)
		{
			pEqu = m_pSim->GetEquation(i);
			CString str = pEqu->GetSimulationName();
			m_wndModel.AddString(str);
		}
	}
	
	DDX_CBIndex(pDX, IDC_SIM_MODEL, m_pLU->m_nSimulID);
	//DDX_Text(pDX, IDC_SIM_TIMER, m_nTimer);
	DDX_Text(pDX, IDC_SIM_TIMER, m_pLU->m_nMaxTimer);
	DDV_MinMaxInt(pDX, m_pLU->GetMaxTimer(), 10, 10000);

	m_wndSpinTimer.SetRange(10,1000);

	CPP_SimModel *pHSim = DYNAMIC_DOWNCAST(CPP_SimModel,m_pSim->GetEquation(m_pLU->m_nSimulID));
	m_wndNumMethod.EnableWindow((BOOL)pHSim);
	m_wndHEuler.EnableWindow((BOOL)pHSim);
	m_wndNumMethod.SetCurSel(0);

	if (pHSim)
	{
		DDX_Text(pDX, IDD_SIM_H, pHSim->m_nEulerH);
		DDV_MinMaxDouble(pDX, pHSim->m_nEulerH, 1.e-003, 1.);
		m_dRangeY = pHSim->m_nEulerH * m_pLU->GetMaxTimer();
	}
	else
		m_dRangeY = m_pLU->GetMaxTimer();



	BOOL bOld = pDX->m_bSaveAndValidate;
	pDX->m_bSaveAndValidate = FALSE;
	DDX_Text(pDX, IDD_SIM_RANGE_X, m_dRangeX);
	DDX_Text(pDX, IDD_SIM_RANGE_Y, m_dRangeY);
	pDX->m_bSaveAndValidate = bOld;
}


BEGIN_MESSAGE_MAP(CLUModelPage, CPropertyPage)
	//{{AFX_MSG_MAP(CLUModelPage)
	ON_CBN_SELCHANGE(IDC_SIM_MODEL, OnSelchangeModel)
	ON_CBN_EDITUPDATE(IDC_SIM_MODEL, OnEditupdateModel)
	ON_EN_KILLFOCUS(IDC_SIM_TIMER, OnKillFocusTimer)
	ON_EN_KILLFOCUS(IDD_SIM_H, OnKillFocusInc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAnimDensityPage message handlers

BOOL CLUModelPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CLUModelPage::OnSelchangeModel()
{
	int res = m_wndModel.GetCurSel();
	if (res == CB_ERR || res == m_pLU->m_nSimulID) return;

	if (m_pLU->m_nSimulID==-1)
		m_pSheet->SetPages();

	m_pLU->ClearUnit();
	m_pLU->AddDefaultExpSet(res);
	m_pLU->m_nSimulID = res;
	CMdEquation *pEqu = m_pSim->GetEquation(m_pLU->m_nSimulID);
	pEqu->OnUpdate(/*m_pSim,*/m_pLU,TRUE);

	UpdateData(TRUE);

}

void CLUModelPage::OnEditupdateModel()
{
	UpdateData(TRUE);
}

void CLUModelPage::OnKillFocusTimer() 
{
	UpdateData(TRUE);
}

void CLUModelPage::OnKillFocusInc() 
{
	UpdateData(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// CLUExpSetPage dialog
/////////////////////////////////////////////////////////////////////////////

CLUExpSetPage::CLUExpSetPage(CSimModel *pSim /*= NULL*/,CLearningUnit *pLU /*= NULL*/)
	: CLUPropertyPage(CLUExpSetPage::IDD)
{
	m_pSim = pSim;
	m_pLU = pLU;

	//{{AFX_DATA_INIT(CLUExpSetPage)
	//}}AFX_DATA_INIT
}

CLUExpSetPage::~CLUExpSetPage()
{
}

void CLUExpSetPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLUExpSetPage)
	DDX_Control(pDX, IDC_EXPSET, m_wndExpSet);
	DDX_Check(pDX, IDC_EXPSET_LOCK, m_pLU->m_bExpSetLocked);
	DDX_Control(pDX, IDC_EXPSET_ADD, m_wndESAdd);
	DDX_Control(pDX, IDC_EXPSET_DEL, m_wndESDel);
	DDX_Control(pDX, IDC_EXPSET_DUP, m_wndESDup);
	DDX_Control(pDX, IDC_EXPSET_DEF, m_wndESDef);
	//}}AFX_DATA_MAP

	if (!pDX->m_bSaveAndValidate)
	{
		// Add Experimental Sets
		UpdateExpSet();
	}

}


BEGIN_MESSAGE_MAP(CLUExpSetPage, CPropertyPage)
	//{{AFX_MSG_MAP(CLUExpSetPage)
	ON_LBN_DBLCLK(IDC_EXPSET, OnDblclkExpSet)
	ON_BN_CLICKED(IDC_EXPSET_DEF, OnExpSetDef)
	ON_BN_CLICKED(IDC_EXPSET_ADD, OnExpSetAdd)
	ON_BN_CLICKED(IDC_EXPSET_DUP, OnExpSetDup)
	ON_BN_CLICKED(IDC_EXPSET_DEL, OnExpSetDel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLUExpSetPage message handlers

BOOL CLUExpSetPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	m_ImageIcon.Create(IDB_MODEL_LU,16,0,RGB(192,192,192));

	m_wndESAdd.SetIcon(m_ImageIcon.ExtractIcon(4));
	//m_wndESAdd.SetTooltipText(ID_CONTR_TASK);
	m_wndESAdd.SetShowText(TRUE);

	m_wndESDel.SetIcon(m_ImageIcon.ExtractIcon(5));
	//m_wndESDel.SetTooltipText(ID_CONTR_TASK);
	m_wndESDel.SetShowText(TRUE);

	m_wndESDef.SetIcon(m_ImageIcon.ExtractIcon(6));
	//m_wndESDef.SetTooltipText(ID_CONTR_TASK);
	m_wndESDef.SetShowText(TRUE);

	m_wndESDup.SetIcon(m_ImageIcon.ExtractIcon(7));
	//m_wndESDup.SetTooltipText(ID_CONTR_TASK);
	m_wndESDup.SetShowText(TRUE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CLUExpSetPage::UpdateExpSet()
{
	m_wndExpSet.ResetContent();
	//CMdEquation *pEqu = m_pSim->GetEquation(m_nModel);
		
	for (int i=0;i<m_pLU->m_cExpSet.GetSize();i++)
	{
		CString str = m_pLU->m_cExpSet.GetAt(i)->GetName();
		m_wndExpSet.AddString(str);
	}
	m_wndExpSet.SetCurSel(0);
}


void CLUExpSetPage::OnExpSetDef()
{
	int nb = m_wndExpSet.GetCurSel();
	if (nb == CB_ERR) return;

	CMdEquation *pEqu = m_pSim->GetEquation(m_pLU->m_nSimulID);
	CMdExpertSet *pExpSet = m_pLU->m_cExpSet.GetAt(nb);
	if (!pEqu || !pExpSet) return;

	CDefineExpSetDlg dlg(this,pEqu,pExpSet);
	if (dlg.DoModal() == IDOK)
	{
		pExpSet->SetName(dlg.m_ExpName);
		m_wndExpSet.DeleteString(nb);
		m_wndExpSet.InsertString(nb,dlg.m_ExpName);
	}


	for(int i = 0; i <  m_pLU->m_cDefExpSet.GetSize();i++ )
		delete  m_pLU->m_cDefExpSet.GetAt(i);
	 m_pLU->m_cDefExpSet.RemoveAll();

	 for (i=0;i<m_pLU->m_cExpSet.GetSize();i++)
	 {
		 CMdExpertSet *pExpSet = m_pLU->m_cExpSet.GetAt(i);
		 if (!pExpSet) continue;
		 CMdExpertSet *pNewExpSet = pExpSet->Clone();
		 if (!pNewExpSet) continue;
		 m_pLU->m_cDefExpSet.Add(pNewExpSet);
	 }

	//pEqu->OnUpdate(/*m_pSim,*/m_pLU);
	pEqu->OnUpdate(m_pLU,nb);
}

void CLUExpSetPage::OnExpSetAdd()
{
	// TODO: Add your control notification handler code here

	int res = m_pLU->m_nSimulID;
	int nb = m_pLU->AddDefaultExpSet(res);

	if (!nb) return;

	CMdExpertSet *pExpSet = m_pLU->m_cExpSet.GetAt(nb-1);
	//CMdExpertSet *pExpSet = pEqu->m_cExpSet.GetAt(nb-1);
	CString mstr = pExpSet->GetName();
	m_wndExpSet.AddString(mstr);
	m_wndExpSet.SetCurSel(nb-1);
	OnExpSetDef();
//	m_bModified = true;

	//CMdEquation *pEqu = m_pSim->GetEquation(m_nModel);
	//pEqu->OnUpdate(m_pSim,m_pLU);
}

void CLUExpSetPage::OnExpSetDup()
{
}

void CLUExpSetPage::OnExpSetDel()
{
	int nItem = m_wndExpSet.GetCurSel();
	if (nItem == CB_ERR) return;

	int nb = m_wndExpSet.GetCount();
	if (nb <= 1)
	{
		CString m_sMsg;
		m_sMsg.LoadString(ERR_EXPSET_LOWERLIMIT);
		MessageBox(m_sMsg,"Warning ...",MB_ICONWARNING|MB_OK);
		return;
	}
	CMdExpertSet *pExpSet = m_pLU->m_cExpSet.GetAt(nItem);
	m_pLU->m_cExpSet.RemoveAt(nItem);
	delete pExpSet;
	m_wndExpSet.DeleteString(nItem);
	m_wndExpSet.SetCurSel((nItem > 1) ? nItem-1 : 0);
	m_wndExpSet.SetFocus();
	//m_bModified = true;

	for (int i=0;i<m_pLU->m_cMERSet.GetSize();i++)
	{
		CExternRepUnit *pER = m_pLU->m_cMERSet.GetAt(i);
		if (!pER) continue;

		CxOutcomes newOutSet;

		int nbOut = pER->m_cOutputSet.GetSize();
		for (int k=0;k<nbOut;k++)
		{
			COutcomes *po = pER->m_cOutputSet.GetAt(k);
			if (!po) continue;
			if (po->m_nExpSet == nItem)
			{
				delete po;
			}
			else
			{
				newOutSet.Add(po);
			}
		}
		pER->m_cOutputSet.RemoveAll();
		pER->m_cOutputSet.Copy(newOutSet);
		int gggg= pER->m_cOutputSet.GetSize();
	}


	CMdEquation *pEqu = m_pSim->GetEquation(m_pLU->m_nSimulID);
	if (pEqu) pEqu->OnUpdate(/*m_pSim,*/m_pLU,TRUE);
}

void CLUExpSetPage::OnDblclkExpSet()
{
	OnExpSetDef();
}

/////////////////////////////////////////////////////////////////////////////
// CLUMERsPage dialog
/////////////////////////////////////////////////////////////////////////////

CLUMERsPage::CLUMERsPage(CSimModel *pSim /*= NULL*/,CLearningUnit *pLU /*= NULL*/)
	: CLUPropertyPage(CLUMERsPage::IDD)
{
	m_pSim = pSim;
	m_pLU = pLU;
	//{{AFX_DATA_INIT(CLUMERsPage)
	//}}AFX_DATA_INIT
}

CLUMERsPage::~CLUMERsPage()
{
}

void CLUMERsPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLUMERsPage)
	DDX_Text(pDX, IDC_MERS_ORDER, m_pLU->m_nbERMax);
	DDV_MinMaxInt(pDX, m_pLU->m_nbERMax, 0, 16);
	DDX_Check(pDX, IDC_MERS_SHOWLABEL, m_pLU->m_bShowAbbrev);
	DDX_Control(pDX, IDC_MERS_ORDERSPIN, m_wndERSpin);
	//}}AFX_DATA_MAP

}


BEGIN_MESSAGE_MAP(CLUMERsPage, CPropertyPage)
	//{{AFX_MSG_MAP(CLUMERsPage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLUExpSetPage message handlers

BOOL CLUMERsPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	m_wndERSpin.SetRange(0,16);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/////////////////////////////////////////////////////////////////////////////
// CLUInstPage dialog
/////////////////////////////////////////////////////////////////////////////

CLUInstPage::CLUInstPage(CSimModel *pSim /*= NULL*/,CLearningUnit *pLU /*= NULL*/)
	: CLUPropertyPage(CLUInstPage::IDD)
{
	m_pSim = pSim;
	m_pLU = pLU;
	m_bEdited = FALSE;
	BOOL bFF = pLU->m_bFirstRun;
	BOOL bFA = pLU->m_bActRequest;
	//{{AFX_DATA_INIT(CLUInstPage)
	m_bFirstRun = bFF;
	m_bActions = bFA;
	//}}AFX_DATA_INIT
}

CLUInstPage::~CLUInstPage()
{
}

void CLUInstPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	m_nTimer = m_pLU->GetMaxTimer();

	//{{AFX_DATA_MAP(CLUInstPage)
	DDX_Text(pDX, IDC_INSTR_HTMLDOC, m_pLU->m_sLUHTML);
	DDX_Control(pDX, IDC_INSTR_SLIDER, m_wndBreakSlider);
	DDX_Control(pDX, IDC_INSTR_FIRST, m_wndFirstRun);
	DDX_Check(pDX, IDC_INSTR_FIRST, m_bFirstRun);
	DDX_Control(pDX, IDC_INSTR_ACTIONS, m_wndActions);
	DDX_Check(pDX, IDC_INSTR_ACTIONS, m_bActions);
	//}}AFX_DATA_MAP

	if (!pDX->m_bSaveAndValidate)
	{
		SetSliders();
	}
}


BEGIN_MESSAGE_MAP(CLUInstPage, CPropertyPage)
	//{{AFX_MSG_MAP(CLUInstPage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLUInstPage message handlers

void CLUInstPage::SetSliders() 
{
	// TODO: Add your control notification handler code here
	int nb = m_nTimer;

	int min = 0;
	int max = nb;
	int range = (int)(max - min);

	int interval = 10;
	if(range%10 == 0 && range != 10)
		interval = 10;
	else if(range%7 == 0 && range != 7)
		interval = 7;
	else if(range%5 == 0 && range != 5)
		interval = 5;
	else if(range%4 == 0 && range != 4)
		interval = 4;
	else if(range%3 == 0 && range != 3)
		interval = 3;
	else if(range%2 == 0 && range != 2)
		interval = 2;
	m_wndBreakSlider.SetRanges((float)min, (float)max);	//Must call these two
	m_wndBreakSlider.SetTicFreq(interval);	
	m_wndBreakSlider.fInit = false;
	m_wndBreakSlider.Invalidate();
	
}

BOOL CLUInstPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	m_bEdited = TRUE;


	int min = 0;
	int max = m_nTimer;
	int range = (int)(max - min);

	int interval = 1;
	if(range%10 == 0 && range != 10)
		interval = 10;
	else if(range%7 == 0 && range != 7)
		interval = 7;
	else if(range%5 == 0 && range != 5)
		interval = 5;
	else if(range%4 == 0 && range != 4)
		interval = 4;
	else if(range%3 == 0 && range != 3)
		interval = 3;
	else if(range%2 == 0 && range != 2)
		interval = 2;
	m_wndBreakSlider.SetRanges((float)min, (float)max);	//Must call these two
	m_wndBreakSlider.SetTicFreq(interval);	

	m_wndBreakSlider.fInteger = TRUE;
	//m_BreakSlider.fDisplayColors = m_Colors;
	m_wndBreakSlider.m_NumMarkers = 6;

	//  cdfdfd
	if (!m_pLU) return TRUE;
	int nbTB = m_pLU->m_cTBreakSet.GetSize();
	for (int k=0;k<nbTB;k++)
	{
		CTimerBreakPoint *pTB = m_pLU->m_cTBreakSet.GetAt(k);
		if (!pTB) continue;
		int ff = pTB->m_tBreakPt;
		CMarker* pMarker = new CMarker;
		pMarker->m_Pos = (float)ff;//+1;
		pMarker->m_nType = pTB->m_nBPType;
		pMarker->m_strDesc = pTB->m_sDesc;
		pMarker->m_strHTML = pTB->m_sHTML;
		m_wndBreakSlider.SetRectangles(pMarker);
		m_wndBreakSlider.InsertMarker(pMarker);
		Invalidate();
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/////////////////////////////////////////////////////////////////////////////
// CDefineLUPropSheet
/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CDefineLUPropSheet, CHelpPrSheet)

CDefineLUPropSheet::CDefineLUPropSheet(
						CWnd* pParentWnd, 
						CSimModel *pSim,
						CLearningUnit *pLU,
						UINT iSelectPage)
	:CHelpPrSheet(_T("Learning Unit"), pParentWnd, iSelectPage)
{
	m_pSim = pSim;
	m_pLU = pLU;

	m_psh.dwFlags |= PSH_NOAPPLYNOW;


	m_pESPage = NULL;
	m_pModPage = NULL;
	m_pInstPage = NULL;
	m_pMERPage = NULL;
	InitPropertySheet();
}

CDefineLUPropSheet::~CDefineLUPropSheet()
{
	if (m_pESPage) delete m_pESPage;
	if (m_pModPage) delete m_pModPage;
	if (m_pInstPage) delete m_pInstPage;
	if (m_pMERPage) delete m_pMERPage;
}


BEGIN_MESSAGE_MAP(CDefineLUPropSheet, CHelpPrSheet)
	//{{AFX_MSG_MAP(CDefineLUPropSheet)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDefineLUPropSheet message handlers

BOOL CDefineLUPropSheet::OnInitDialog() 
{
	CHelpPrSheet::OnInitDialog();

		CTabCtrl *pTC = GetTabControl();
	if (pTC)
		pTC->SetPadding(CSize(10,4));


	// TODO: Add extra initialization here
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDefineLUPropSheet::InitPropertySheet()
{
	m_pModPage = new CLUModelPage(m_pSim,m_pLU);
	m_pESPage = new CLUExpSetPage(m_pSim,m_pLU);
	m_pInstPage = new CLUInstPage(m_pSim,m_pLU);
	m_pMERPage = new CLUMERsPage(m_pSim,m_pLU);

	CImageList pImageList;
	pImageList.Create(IDB_MODEL_LU,16,0,RGB(192,192,192));

	m_pModPage->m_psp.dwFlags |= PSP_USEHICON ;
	m_pModPage->m_psp.hIcon = pImageList.ExtractIcon(0);
	m_pESPage->m_psp.dwFlags |= PSP_USEHICON ;
	m_pESPage->m_psp.hIcon = pImageList.ExtractIcon(1);
	m_pInstPage->m_psp.dwFlags |= PSP_USEHICON ;
	m_pInstPage->m_psp.hIcon = pImageList.ExtractIcon(2);
	m_pMERPage->m_psp.dwFlags |= PSP_USEHICON ;
	m_pMERPage->m_psp.hIcon = pImageList.ExtractIcon(3);

	m_pModPage->m_pSheet = this;
	m_pESPage->m_pSheet = this;
	m_pInstPage->m_pSheet = this;
	m_pMERPage->m_pSheet = this;

	AddPage(m_pModPage);

	if (m_pLU->m_nSimulID!=-1)
	{
		AddPage(m_pESPage);
		AddPage(m_pInstPage);
		AddPage(m_pMERPage);
	}
}

int CDefineLUPropSheet::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	EnableStackedTabs(TRUE);
	
	if (CHelpPrSheet::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO: Add your specialized creation code here
	
	return 0;
}

void CDefineLUPropSheet::SetPages()
{
	if (GetPageCount()!=1) return;
	AddPage(m_pESPage);
	AddPage(m_pInstPage);
	AddPage(m_pMERPage);
}

int CDefineLUPropSheet::DoModal() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	int nRes = CHelpPrSheet::DoModal();

	if (!m_pInstPage) return nRes;
	if (!m_pInstPage->m_bEdited) return nRes;

	if (nRes==IDOK)
	{
		m_pLU->m_bFirstRun = m_pInstPage->m_bFirstRun;
		m_pLU->m_bActRequest = m_pInstPage->m_bActions;
	}

	for(int i = 0; i < m_pLU->m_cTBreakSet.GetSize();i++ )
		delete m_pLU->m_cTBreakSet.GetAt(i);
	m_pLU->m_cTBreakSet.RemoveAll();

	int nbTB = m_pInstPage->m_wndBreakSlider.m_Markers.GetCount();
	POSITION pos = m_pInstPage->m_wndBreakSlider.m_Markers.GetHeadPosition();
	int Idx = 0;
	while (pos != NULL)
	{
		CMarker* pMarker = (CMarker*)m_pInstPage->m_wndBreakSlider.m_Markers.GetNext(pos);
			//CString mstr = dlg.m_BreakSlider.m_cStrDesc.GetAt(Idx);
	//		if (pMarker->m_Pos)
		{
			CString mstr = pMarker->m_strDesc;
			CString mstrH = pMarker->m_strHTML;
			//int nStyle = dlg.m_BreakSlider.m_cIntStyle.GetAt(Idx);
			int nStyle = pMarker->m_nType;

			CTimerBreakPoint *pTB = new CTimerBreakPoint();
			//pTB->m_sDesc = (bAct) ? tempSTR2 : tempSTR1;
			pTB->m_sDesc = mstr;
			pTB->m_sHTML= mstrH;
			pTB->m_nBPType = nStyle;
			pTB->m_tBreakPt = (int)pMarker->m_Pos;//-1;
			m_pLU->m_cTBreakSet.Add(pTB);
//				pLU->m_cTTTT.SetAt(pTB->m_tBreakPt,*pTB);
			Idx++;
		}
	}

	return nRes;

}
