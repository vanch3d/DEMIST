// SelectParamDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Simul.h"

#include "SimulDoc.h"
#include <MSimulation\Model.h>
#include "SelectParamDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSelectOutcomeDlg dialog

CSimulDoc*	CSelectOutcomeDlg::GetDocument()
{
	return (CSimulDoc*)m_pDoc;
}

CSelectOutcomeDlg::~CSelectOutcomeDlg()
{
	int nb = m_cLocOutputSet.GetSize();
	for (int i=0;i<nb;i++)
		delete m_cLocOutputSet.GetAt(i);
	m_cLocOutputSet.RemoveAll();
}


CSelectOutcomeDlg::CSelectOutcomeDlg(CWnd* pParent /*=NULL*/,CDocument* pDoc)
	: CHelpDlg(CSelectOutcomeDlg::IDD, pParent)
{
	m_pDoc = pDoc;
	m_nSelected = -1;
	m_bForceSync = FALSE;
	m_bFirstOpen = FALSE;
	//{{AFX_DATA_INIT(CSelectOutcomeDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pImageList.Create(IDB_VAR_ICONS,14,0,RGB(128,128,128));

}

void CSelectOutcomeDlg::DoDataExchange(CDataExchange* pDX)
{
	CHelpDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSelectOutcomeDlg)
	DDX_Control(pDX, IDC_DESCRIPTION, m_strDescr);
	DDX_Control(pDX, IDC_TAB_EXPSET, m_wndTabExpSet);
	DDX_Control(pDX, IDC_LISTPARAM, m_wndSelectParam);
	//}}AFX_DATA_MAP
	if (pDX->m_bSaveAndValidate)
	{
		m_nSelected = m_wndSelectParam.GetNextItem(-1,LVNI_SELECTED);
	}
}


BEGIN_MESSAGE_MAP(CSelectOutcomeDlg, CHelpDlg)
	//{{AFX_MSG_MAP(CSelectOutcomeDlg)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LISTPARAM, OnItemchanged)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_EXPSET, OnSelchangeTabExpset)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSelectOutcomeDlg message handlers

BOOL CSelectOutcomeDlg::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	return CHelpDlg::PreCreateWindow(cs);
}

void CSelectOutcomeDlg::OnOK()
{
//	int nb = m_wndSelectParam.GetItemCount();
//
//	for (int i=0;nb;i++)
//		m_checkArray[i] = m_wndSelectParam.GetCheck(i);
	CHelpDlg::OnOK();
}

/////////////////////////////////////////////////////////////////////////////
/// Initalise the Tab Control by creating a tab per Experimental Set.
///
/// \param nTab The zero-based index of the Tab to update (default: 0)
/////////////////////////////////////////////////////////////////////////////
BOOL CSelectOutcomeDlg::OnInitTab(int nTab)
{
	m_wndSelectParam.DeleteAllItems( );
	LV_ITEM lvi;
	int nbItem = 0;

	for (int i=0;i<m_cLocOutputSet.GetSize();i++)
	{
		COutcomes *pCo = m_cLocOutputSet.GetAt(i);
		if (!pCo->m_nAllow) continue;
		if (pCo->m_nExpSet != nTab) continue;

		CLearningUnit *pLU = GetDocument()->GetCurrentLU();
		CMdData *pData = pLU->GetDataAt(pCo->m_nData);

		CString strItem = pData->GetDataName(pLU->m_bShowAbbrev);

		lvi.mask = LVIF_TEXT | LVIF_PARAM | LVIF_IMAGE; //| LVIF_IMAGE | LVIF_STATE;
		lvi.iItem = nbItem;
		lvi.iSubItem = 0;
		lvi.iImage = pData->GetDataInfo()->GetIcon();
		lvi.lParam = i;
		lvi.pszText = strItem.GetBuffer(strItem.GetLength());
		int aa = m_wndSelectParam.InsertItem(&lvi);
		if (pCo->m_nSelected)
			m_wndSelectParam.SetCheck(aa);
		nbItem++;
	}

	return TRUE;

}



BOOL CSelectOutcomeDlg::OnInitDialog() 
{
	CHelpDlg::OnInitDialog();

	//m_wndSelectParam.SetExtendedStyle(WS_EX_CLIENTEDGE);
	DWORD dwStyle = m_wndSelectParam.GetExtendedStyle();
	//dwStyle &= ~LVS_EX_CHECKBOXES ;
	dwStyle |= LVS_EX_CHECKBOXES ;
	m_wndSelectParam.SetExtendedStyle(WS_EX_CLIENTEDGE | dwStyle );

	m_wndSelectParam.SetImageList(&m_pImageList,LVSIL_SMALL);

	if (!GetDocument())	return false;
	for (int i=0;i<m_cTabLabel.GetSize();i++)
	{
		CString str = m_cTabLabel.GetAt(i);
		m_wndTabExpSet.InsertItem(i,str);

	}
	OnInitTab();
	m_bFirstOpen = TRUE;
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/////////////////////////////////////////////////////////////////////////////
/// Called by the framework when the state of an item in the List Control has changed (selected or checked)
///
/// \param pNMHDR Pointer to a NM_LISTVIEW structure containing information about the selected item
/// \param pResult The zero-based index of the Tab to update (default: 0)
/////////////////////////////////////////////////////////////////////////////
void CSelectOutcomeDlg::OnItemchanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	if (!m_bFirstOpen) return;

	// TODO: Add your control notification handler code here
	int nb = pNMListView->iItem;
	int st = pNMListView->uNewState;
	int ost = pNMListView->uOldState;
//	int nTab = m_wndTabExpSet.GetCurSel( ); 8192 cocje 4096 non

	if (st == 3)
	{
		//LVITEM pItem;
		//pItem.iItem = nb;
		///bool vv = m_wndSelectParam.GetItem(&pItem);
		//int isv = m_wndSelectParam.GetCheck(nb);
		CString str = m_wndSelectParam.GetItemText(nb,0);
		int dt = m_wndSelectParam.GetItemData(nb);
		CLearningUnit *pLU = GetDocument()->GetCurrentLU();
		COutcomes *pCo = m_cLocOutputSet.GetAt(dt);
		CMdData *pData = pLU->GetDataAt(pCo->m_nData);
		CModelObj *pObj = pData->GetDataInfo();
		CString str2 = pObj->GetDesc();
		m_strDescr.SetWindowText(str2);
	}
	else if ((st == 4096 || st == 8192) && ost)
	{
		//LVITEM pItem;
		//pItem.iItem = nb;
		//m_wndSelectParam.GetItem(&pItem);
		int isv = m_wndSelectParam.GetCheck(nb);
		int dt = m_wndSelectParam.GetItemData(nb);
		COutcomes *pCo = m_cLocOutputSet.GetAt(dt);
		pCo->m_nSelected = isv;
		CLearningUnit *pLU = GetDocument()->GetCurrentLU();
		CMdData *pData = pLU->GetDataAt(pCo->m_nData);
		CModelObj *pObj = pData->GetDataInfo();
		CString str2 = pObj->GetDesc();
		m_strDescr.SetWindowText(str2);
		if (m_bForceSync)
		{
			int nb = m_wndSelectParam.GetItemCount();
			for (int i=0;i<nb;i++)
			{
				m_wndSelectParam.SetCheck(i,isv);
				int dt = m_wndSelectParam.GetItemData(i);
				COutcomes *pCo = m_cLocOutputSet.GetAt(dt);
				pCo->m_nSelected = isv;
			}
		}
	}
	else
	{
		m_strDescr.SetWindowText(_T(""));
	}

	*pResult = 0;
}

/////////////////////////////////////////////////////////////////////////////
/// Called by the framework when a different tab in the Tab Control is selected
///
/////////////////////////////////////////////////////////////////////////////
void CSelectOutcomeDlg::OnSelchangeTabExpset(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	int nb = m_wndTabExpSet.GetCurSel( );

	OnInitTab(nb);
	m_strDescr.SetWindowText(_T(""));
	*pResult = 0;
}
