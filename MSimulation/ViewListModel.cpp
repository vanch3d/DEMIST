// ViewNewListModel.cpp : implementation file
//

#include "stdafx.h"
#include "simul.h"

#include "SimulDoc.h"
#include <MSimulation\PreyPredModel.h>
#include <MInstruction\LearningUnit.h>

#include "AddModelDlg.h"
#include "ViewListModel.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CViewSDIlist

IMPLEMENT_DYNCREATE(CViewSDIlist, CFormView)

CViewSDIlist::CViewSDIlist()
	: CFormView(CViewSDIlist::IDD)
{
	//{{AFX_DATA_INIT(CViewSDIlist)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_ImageList.Create(IDB_ER_ICONS,32,0,RGB(192,192,192));
	m_ImageIcon.Create(IDB_MODEL_DESIGN,16,0,RGB(192,192,192));

	m_bCanProceed = FALSE;
}

CViewSDIlist::~CViewSDIlist()
{
}

void CViewSDIlist::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CViewSDIlist)
	DDX_Control(pDX, IDC_MODEL_DEL, m_ctrDel);
	DDX_Control(pDX, IDC_MODEL_ADD, m_ctrAdd);
	DDX_Control(pDX, IDC_MODEL_LIST, m_ctrModelList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CViewSDIlist, CFormView)
	//{{AFX_MSG_MAP(CViewSDIlist)
	ON_BN_CLICKED(IDC_MODEL_ADD, OnModelAdd)
	ON_BN_CLICKED(IDC_MODEL_DEL, OnModelDel)
	ON_WM_SIZE()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_MODEL_LIST, OnItemchangedModelList)
	ON_NOTIFY(NM_CLICK, IDC_MODEL_LIST, OnClickModelList)
	ON_NOTIFY(NM_RCLICK, IDC_MODEL_LIST, OnRclickModelList)
	ON_NOTIFY(LVN_KEYDOWN, IDC_MODEL_LIST, OnKeydownModelList)
	ON_NOTIFY(LVN_ENDLABELEDIT, IDC_MODEL_LIST, OnEndlabeleditModelList)
	ON_NOTIFY(NM_DBLCLK, IDC_MODEL_LIST, OnDblclkModelList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewSDIlist diagnostics

#ifdef _DEBUG
void CViewSDIlist::AssertValid() const
{
	CFormView::AssertValid();
}

void CViewSDIlist::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CSimulDoc* CViewSDIlist::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSimulDoc)));
	return (CSimulDoc*)m_pDocument;
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CViewSDIlist message handlers

void CViewSDIlist::OnModelAdd() 
{
	// TODO: Add your control notification handler code here
	CAddModelDlg dlg(this);

	dlg.m_pSimul = GetDocument()->m_pSimul;
	if (dlg.DoModal()==IDOK)
	{
		if (dlg.m_pEq)
		{
			GetDocument()->m_pSimul->m_cEquSet.Add(dlg.m_pEq);
			OnUpdateModels();

			int nb = GetDocument()->m_pSimul->m_cEquSet.GetSize();
			GetDocument()->m_nModSelItem = nb-1;
			GetDocument()->UpdateAllViews(this,DOC_UPDATE_MODEL);

		}
		int r=0;
		//delete dlg.m_pEq;
		GetDocument()->SetModifiedFlag();
	}
}

void CViewSDIlist::OnModelDel() 
{
	// TODO: Add your control notification handler code here
	int nb = GetDocument()->m_pSimul->m_cEquSet.GetSize();

	int nItem = -1;
	for (int i=0;i<nb;i++)
	{
		UINT nState = m_ctrModelList.GetItemState(i,LVIS_SELECTED);
		if (nState)
		{
			nItem = i;
			break;
		}
	}
	if (nItem==-1) return;

	int nbLU = GetDocument()->m_pInstruct->m_cLUSet.GetSize();

	CString mstr;
	for (int j=0;j<nbLU;j++)
	{
		CLearningUnit *pLU = GetDocument()->m_pInstruct->m_cLUSet.GetAt(j);
		if (!pLU) continue;

		if (pLU->m_nSimulID==nItem)
		{
			CString mstr2;
			mstr2.Format(_T("\t(%d) - %s\n"),j,pLU->m_sLUName);
			mstr+=mstr2;
		}
	}
	CString mstr3;
	if (mstr.IsEmpty())
		mstr3.Format(_T("Do you really want to delete this model?"));
	else
	{
		mstr3.Format(_T("This model is used in the following unit(s) :\n\n"
						"%s\n"
						"Do you really want to delete it ?"),mstr);
	}
	int nRes = MessageBox(mstr3,NULL,MB_OKCANCEL);
	if (nRes!=IDOK) return;

	CMdEquation* pEqu = GetDocument()->m_pSimul->m_cEquSet.GetAt(i);
	if (pEqu)
	{
		GetDocument()->m_pSimul->m_cEquSet.RemoveAt(nItem);
		delete pEqu;
	}

	GetDocument()->m_nModSelItem = 0;
	OnUpdateModels();
	GetDocument()->UpdateAllViews(this,DOC_UPDATE_MODEL);
	Invalidate();
	UpdateWindow();

}

/////////////////////////////////////////////////////////////////////////////
/// The framework calls this function after receiving a notification message.
///
/// \param pSender Points to the view that modified the document, or NULL if all views are to be updated.
/// \param lHint Contains information about the modifications.
/// \param pHint Points to an object storing information about the modifications.
///
/// This member function proceeds and/or dispatches all the messages sent by the framework
/// to the relevant function.
/////////////////////////////////////////////////////////////////////////////
void CViewSDIlist::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	// TODO: Add your specialized code here and/or call the base class

	BOOL bRedraw = FALSE;

	//CListCtrl &mList = GetListCtrl();
	int nb = GetDocument()->m_pSimul->m_cEquSet.GetSize();

	if (lHint == DOC_UPDATE_MODEL)
	{
		if (pHint)
		{
			OnUpdateModels();
		}
		for (int i=0;i<nb;i++)
		{
			int rr = (i==GetDocument()->m_nModSelItem) ? 2 : 0;

			UINT nState = m_ctrModelList.GetItemState(i,LVIS_SELECTED);
			if (i==GetDocument()->m_nModSelItem)
				m_ctrModelList.SetItemState(i,rr,LVIS_SELECTED);
		}
		bRedraw = TRUE;
	}
	if (bRedraw)
	{
		Invalidate();
		UpdateWindow();
	}
	
	
}

void CViewSDIlist::OnUpdateModels()
{
	m_ctrModelList.DeleteAllItems();

	CSimulDoc* pDoc = GetDocument();
	int nb = pDoc->m_pSimul->m_cEquSet.GetSize();
	////// MODELS
	for (int i=0;i<nb;i++)
	{
		CMdEquation *pEq = pDoc->m_pSimul->m_cEquSet.GetAt(i);

		CString mstr = pEq->GetSimulationName();
		LV_ITEM lvi;
		lvi.mask = LVIF_TEXT | LVIF_PARAM | LVIF_IMAGE; //| LVIF_IMAGE | LVIF_STATE;
		lvi.iItem = i;
		lvi.iSubItem = 0;
		lvi.lParam = i;
		lvi.iImage = 1;
		if (i==pDoc->m_nModSelItem)
		{	
			lvi.mask |= LVIF_STATE ;
			lvi.state = 2 ;
			lvi.stateMask = LVIS_SELECTED;
		}
		lvi.pszText = mstr.GetBuffer(mstr.GetLength());
		m_ctrModelList.InsertItem(&lvi);
	}	
}

void CViewSDIlist::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	m_bCanProceed= TRUE;
	// TODO: Add your specialized code here and/or call the base class
//	DWORD dwStyle = m_ctrModelList.GetStyle();
//	dwStyle |= /*WS_CHILD | WS_VISIBLE| */LVS_ICON | /*LVS_AUTOARRANGE | /*LVS_ALIGNTOP | /*LVS_ALIGNLEFT | */
//				LVS_SINGLESEL | LVS_SHOWSELALWAYS ;
//	::SetWindowLong(m_ctrModelList.m_hWnd,GWL_STYLE,dwStyle);
//	DWORD dwExtStyle = m_ctrModelList.GetExStyle();
//	dwExtStyle |= LVS_EX_INFOTIP   ;
//	m_ctrModelList.SetExtendedStyle(dwExtStyle);

	m_ctrDel.SetIcon(m_ImageIcon.ExtractIcon(1));
	m_ctrAdd.SetIcon(m_ImageIcon.ExtractIcon(0));

	//m_ctrDel.SetTooltipText(ID_CONTR_TASK);
	//m_ctrAdd.SetTooltipText(ID_CONTR_TASK);

	m_ctrDel.SetShowText(TRUE);
	m_ctrAdd.SetShowText(TRUE);

	
	m_ctrModelList.SetImageList(&m_ImageList,LVSIL_NORMAL);
	OnUpdateModels();

	CRect mrect,mrect2;
	GetClientRect(mrect);
	m_ctrModelList.SetWindowPos(NULL,0,0,mrect.Width(),mrect.Height()-38,SWP_NOMOVE|SWP_NOACTIVATE);
	m_ctrModelList.Arrange(LVA_DEFAULT);
	m_ctrModelList.EnsureVisible(GetDocument()->m_nModSelItem,FALSE);
}

void CViewSDIlist::OnSize(UINT nType, int cx, int cy) 
{
	CFormView::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if (m_bCanProceed)
	{
		CSimulDoc* pDoc = GetDocument();
		m_ctrModelList.SetWindowPos(NULL,0,0,cx,cy-38,SWP_NOMOVE|SWP_NOACTIVATE);
		m_ctrModelList.Arrange(LVA_DEFAULT);
		m_ctrModelList.EnsureVisible(pDoc->m_nModSelItem,FALSE);
	}
}

void CViewSDIlist::OnItemchangedModelList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here

	UINT nState = m_ctrModelList.GetItemState(pNMListView->iItem,LVIS_SELECTED);
	if (nState)
	{
		CSimulDoc* pDoc = GetDocument();
		if (pDoc->m_nModSelItem != pNMListView->iItem)
		{
			pDoc->m_nModSelItem = pNMListView->iItem;
			pDoc->UpdateAllViews(this,DOC_UPDATE_MODEL);
		}

	}
	
	*pResult = 0;
}

void CViewSDIlist::OnClickModelList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	BOOL bSel = (pNMListView->iItem !=-1);
	
	m_ctrDel.EnableWindow(bSel);

	*pResult = 0;
}

void CViewSDIlist::OnRclickModelList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}

void CViewSDIlist::OnKeydownModelList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_KEYDOWN* pLVKeyDow = (LV_KEYDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}

void CViewSDIlist::OnEndlabeleditModelList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;

	if (!pDispInfo) return;

	CString strNew = pDispInfo->item.pszText;
	// TODO: Add your control notification handler code here
	if (pDispInfo->item.iItem !=-1 && strNew != _T(""))
	{
		CSimulDoc* pDoc = GetDocument();
		int nb = pDoc->m_pSimul->m_cEquSet.GetSize();
		CMdEquation *pEq = pDoc->m_pSimul->m_cEquSet.GetAt(pDispInfo->item.iItem);
		CString mstr = pEq->GetSimulationName();

		mstr = strNew;//m_ctrModelList.GetItemText(pDispInfo->item.iItem,0);

		pEq->SetSimulationName(mstr);
		m_ctrModelList.SetItemText(pDispInfo->item.iItem,0,mstr);
	}
	
	*pResult = 0;
}

void CViewSDIlist::OnDblclkModelList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if (pNMListView->iItem != -1)
	{
		m_ctrModelList.EditLabel(pNMListView->iItem);
	}
}
