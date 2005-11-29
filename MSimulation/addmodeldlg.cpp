// AddModelDlg.cpp : implementation file
//

#include "stdafx.h"
#include "simul.h"

#include <MSimulation\PreyPredModel.h>
#include <MSimulation\AuthorModel.h>

#include "AddModelDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAddModelDlg dialog


CAddModelDlg::CAddModelDlg(CWnd* pParent /*=NULL*/)
	: CHelpDlg(CAddModelDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAddModelDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pEq = NULL;
	m_nCat = ADD_TEMPLATE;
	m_ImageList.Create(IDB_ER_ICONS,32,0,RGB(192,192,192));
	m_TabImageList.Create(IDB_TREE_CTRL_OPTIONS,16,0,RGB(255, 0, 255));

	// Add default template
	//m_cEquSet.Add(new CPP_SgUnlimited());
	//m_cEquSet.Add(new CPP_SgLimited());
	m_cEquSet.Add(new CPP_SimModel());
	m_cEquSet.Add(new CPP_SgUnlimitedII());
	m_cEquSet.Add(new CPP_SgLimitedII());
	m_cEquSet.Add(new CPP_TwCompetition());
	m_cEquSet.Add(new CPP_TwPredationUnltd());
	m_cEquSet.Add(new CPP_TwPredation());
	m_cEquSet.Add(new CPP_TwLotkaII());
#ifdef _DEBUG
	m_cEquSet.Add(new CPP_Pendulum());
#endif
}

CAddModelDlg::~CAddModelDlg()
{
	for (int i=0;i<m_cEquSet.GetSize();i++ )
	{
		CMdEquation *eq = m_cEquSet.GetAt(i);
		if (eq) delete eq;
	}
	m_cEquSet.RemoveAll();
}


void CAddModelDlg::DoDataExchange(CDataExchange* pDX)
{
	CHelpDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAddModelDlg)
	DDX_Control(pDX, IDC_ADD_DESC, m_ctrlDesc);
	DDX_Control(pDX, IDC_ADD_LIST, m_ctrlList);
	DDX_Control(pDX, IDC_ADD_TAB, m_ctrlTab);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAddModelDlg, CHelpDlg)
	//{{AFX_MSG_MAP(CAddModelDlg)
	ON_NOTIFY(TCN_SELCHANGE, IDC_ADD_TAB, OnSelChangeTab)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAddModelDlg message handlers

void CAddModelDlg::OnSelChangeTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	*pResult = 0;
	int nb = m_ctrlTab.GetCurSel();
	if (nb!=-1)
	{
		UpdateModel(nb);
		m_nCat = nb;
	}
}

void CAddModelDlg::UpdateModel(int nCat)
{
	CTypedPtrArray<CObArray,CMdEquation*>	*pEquSet = NULL;

	m_ctrlList.DeleteAllItems();
	if (nCat==ADD_RECENT)
	{
		pEquSet = &(m_pSimul->m_cEquSet);
	}
	else if (nCat==ADD_TEMPLATE)
	{
		pEquSet = &m_cEquSet;
	}
	if (!pEquSet) return;

	int nb = pEquSet->GetSize();
	for (int i=0;i<nb;i++)
	{
		CMdEquation *pEq = pEquSet->GetAt(i);
		CString mstr = pEq->GetSimulationName();
		LV_ITEM lvi;
		lvi.mask = LVIF_TEXT | LVIF_PARAM | LVIF_IMAGE; //| LVIF_IMAGE | LVIF_STATE;
		lvi.iItem = i;
		lvi.iSubItem = 0;
		lvi.lParam = i;
		lvi.iImage = 1;
		lvi.pszText = mstr.GetBuffer(mstr.GetLength());
		m_ctrlList.InsertItem(&lvi);
	}
}

BOOL CAddModelDlg::OnInitDialog() 
{
	CHelpDlg::OnInitDialog();

	m_ctrlList.SetImageList(&m_ImageList,LVSIL_NORMAL);
	m_ctrlTab.SetImageList(&m_TabImageList);
	
	// TODO: Add extra initialization here
	m_ctrlTab.InsertItem(ADD_TEMPLATE,_T("From a &Template"),8);
	m_ctrlTab.InsertItem(ADD_RECENT,_T("From a &Recent Model"),9);

	UpdateModel(m_nCat);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAddModelDlg::OnOK()
{
	for (int i=0;i<m_ctrlList.GetItemCount();i++)
	{
		UINT nState = m_ctrlList.GetItemState(i,LVIS_SELECTED);
		if (nState)
		{
			CMdEquation *pEq = NULL;
			if (m_nCat==ADD_RECENT)
				pEq = m_pSimul->m_cEquSet.GetAt(i);
			else if (m_nCat==ADD_TEMPLATE)
				pEq = m_cEquSet.GetAt(i);

			if (pEq)
			{
				m_pEq = pEq->Clone();
				m_pEq->OnNewEquation(m_pSimul);
				break;
			}
		}
	}


	CHelpDlg::OnOK();
}
