// VariableDefDlg.cpp : implementation file
//

#include "stdafx.h"
#include "simul.h"
#include "VariableDefDlg.h"

#include <MSimulation\Model.h>
//#include "GridCtrl\GridBtnCell\GridBtnCell.h"
//#include "GridCtrl\GridBtnCell\GridBtnCellCombo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CVariableDefDlg dialog


CVariableDefDlg::CVariableDefDlg(CWnd* pParent /*=NULL*/,const CMdEquation *pEqu /*= NULL*/)
	: CDialog(CVariableDefDlg::IDD, pParent)
{
	m_pEquation = pEqu;
	//{{AFX_DATA_INIT(CVariableDefDlg)
	m_sDesc = _T("");
	m_nInitVal = 0.0;
	m_nMaxVal = 0.0;
	m_nMinVal = 0.0;
	m_sDefUnit = _T("fd");
	m_nDefIcon = -1;
	//}}AFX_DATA_INIT
	m_pImageList.Create(IDB_VAR_ICONS,14,0,RGB(128,128,128));
}


void CVariableDefDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CVariableDefDlg)
	DDX_Control(pDX, IDC_DEFICON, m_DefIcon);
	DDX_Control(pDX, IDC_DEFUNIT, m_DefUnit);
	DDX_Control(pDX, IDC_DESC, m_Desc);
	DDX_Control(pDX, IDC_VAL_MIN, m_MinVal);
	DDX_Control(pDX, IDC_VAL_MAX, m_MaxVal);
	DDX_Control(pDX, IDC_VAL_INIT, m_InitVal);
	DDX_Control(pDX, IDC_PARAM_TREE, m_parTree);
	DDX_Control(pDX, IDC_DEFCOLOR, m_clrDefault);
	DDX_Text(pDX, IDC_DESC, m_sDesc);
	DDV_MaxChars(pDX, m_sDesc, 150);
	DDX_Text(pDX, IDC_VAL_INIT, m_nInitVal);
	DDX_Text(pDX, IDC_VAL_MAX, m_nMaxVal);
	DDX_Text(pDX, IDC_VAL_MIN, m_nMinVal);
	DDX_CBString(pDX, IDC_DEFUNIT, m_sDefUnit);
	DDV_MaxChars(pDX, m_sDefUnit, 15);
	DDX_CBIcon(pDX, IDC_DEFICON, m_nDefIcon);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CVariableDefDlg, CDialog)
	//{{AFX_MSG_MAP(CVariableDefDlg)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_GETMINMAXINFO()
	ON_NOTIFY(TVN_SELCHANGING, IDC_PARAM_TREE, OnSelchangingTree)
	ON_NOTIFY(TVN_SELCHANGED, IDC_PARAM_TREE, OnSelchangedTree)
	ON_NOTIFY(TVN_ENDLABELEDIT, IDC_PARAM_TREE, OnEndlabeleditTree)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVariableDefDlg message handlers

BOOL CVariableDefDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_DefIcon.AddIcon(IDR_SIMULPIE);
	m_DefIcon.AddIcon(IDR_SIMULANIM);
	m_DefIcon.AddIcon(IDR_SIMULGRAPH);
	m_DefIcon.AddIcon(IDR_SIMULTYPE);
	m_DefIcon.AddIcon(IDR_SIMULPARAM_TMPL);
	m_DefIcon.AddIcon(IDR_SIMUL_CONTEXT);

	// set data for the CCtreeCtrl
	int nb = m_pEquation->m_cParSet.GetSize();
	int nb2 = m_pEquation->m_cVarSet.GetSize();

	m_parTree.SetImageList(&m_pImageList, TVSIL_NORMAL);
	for (int i=0;i<nb+nb2;i++)
	{
		TV_INSERTSTRUCT	curTreeItem;
		CModelObj		*obj;

		if (i < nb)
			obj = m_pEquation->m_cParSet.GetAt(i);
		else
			obj = m_pEquation->m_cVarSet.GetAt(i-nb);
		CString tt = obj->GetName();

		int nbImage = obj->GetIcon();
/*		if (i >= nb)
		{
			if ( ((CModelVar *)obj)->GetInitValue() == -1)
				nbImage = 3;
			else
				nbImage = 0;
		}
		else 
		{	if ( ((CModelPar *)obj)->m_InitValue == -1)
				nbImage = 1;
			else nbImage = 2;
		}*/


		curTreeItem.item.mask = TVIF_IMAGE | TVIF_TEXT | TVIF_PARAM | TVIF_SELECTEDIMAGE ;
		curTreeItem.hParent = TVI_ROOT;
		curTreeItem.hInsertAfter = TVI_LAST;
		curTreeItem.item.iImage = nbImage;
		curTreeItem.item.iSelectedImage = nbImage;
		curTreeItem.item.pszText = tt.GetBuffer(tt.GetLength());//[iItem].GetBuffer(strItems[iItem].GetLength());
		curTreeItem.item.lParam = (LPARAM)obj;
//		m_rghItem[iItem] = m_mytreectrl.InsertItem(&curTreeItem);
		m_parTree.InsertItem(&curTreeItem);

	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


BOOL CVariableDefDlg::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::PreCreateWindow(cs);
}

int CVariableDefDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	return 0;
}

void CVariableDefDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
}

void CVariableDefDlg::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	// TODO: Add your message handler code here and/or call default
	CDialog::OnGetMinMaxInfo(lpMMI);
}

void CVariableDefDlg::OnSelchangingTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	
	int res = UpdateData();

	if (res)
	{
		CModelObj *obj = (CModelObj*) pNMTreeView->itemOld.lParam;
		CModelObj *obj2 = (CModelObj*)pNMTreeView->itemNew.lParam;
		if (obj) GetProperties(obj);
		if (obj2) SetProperties(obj2);
	}
	*pResult = (res) ? FALSE : TRUE;
}


void CVariableDefDlg::OnSelchangedTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
	//CModelObj *obj = (CModelObj*) pNMTreeView->itemOld.lParam;
	//CModelObj *obj2 = (CModelObj*)pNMTreeView->itemNew.lParam;
}

void CVariableDefDlg::OnOK( )
{
	CDialog::OnOK();
	HTREEITEM pItem = m_parTree.GetSelectedItem( );
	if (pItem)
	{
		CModelObj *obj = (CModelObj*)m_parTree.GetItemData(pItem);
		if (obj) GetProperties(obj);

	}
}

void CVariableDefDlg::SetProperties(CModelObj* pObj)
{
	if (!pObj) return;
	m_Desc.SetWindowText(pObj->GetDesc());
	m_clrDefault.SetBkColour(pObj->GetColor());
	m_sDefUnit = pObj->GetUnit();
	int res = m_DefUnit.SelectString(-1,m_sDefUnit);
	if (res == CB_ERR /*&& m_sDefUnit != ""*/)
	{
		if (m_sDefUnit != "") m_DefUnit.AddString(m_sDefUnit);
//		res = m_DefUnit.SelectString(-1,m_sDefUnit);
		m_DefUnit.SetWindowText(m_sDefUnit);
	}
	//m_DefUnit.SetCurSel()


	BOOL bIsParam = (BOOL)DYNAMIC_DOWNCAST( CModelPar, pObj );
	CString dd;
	if (bIsParam)
	{
		dd.Format(_T("%.4g"),((CModelPar*)pObj)->GetValue());
		m_InitVal.SetWindowText(dd);
		dd.Format(_T("%.4g"),((CModelPar*)pObj)->GetMin());
		m_MinVal.SetWindowText(dd);
		dd.Format(_T("%.4g"),((CModelPar*)pObj)->GetMax());
		m_MaxVal.SetWindowText(dd);
	}
	else
	{
		dd = "0";
		m_InitVal.SetWindowText(dd);
		m_MinVal.SetWindowText(dd);
		m_MaxVal.SetWindowText(dd);
	}
	m_InitVal.EnableWindow(bIsParam);
	m_MaxVal.EnableWindow(bIsParam);
	m_MinVal.EnableWindow(bIsParam);
}

void CVariableDefDlg::GetProperties(CModelObj* pObj)
{
	if (!pObj) return;

	CString tt;
	m_Desc.GetWindowText(tt);
	pObj->SetDesc(tt);

	CModelPar *pParam = DYNAMIC_DOWNCAST( CModelPar, pObj );
	if (pParam)
	{
		m_MaxVal.GetWindowText(tt);
		double mx = atof(tt.GetBuffer(tt.GetLength()));
		m_MinVal.GetWindowText(tt);
		double mn = atof(tt.GetBuffer(tt.GetLength()));
		m_InitVal.GetWindowText(tt);
		double vl = atof(tt.GetBuffer(tt.GetLength()));
		pParam->SetInfos(vl,mn,mx);
	}
	COLORREF clr = m_clrDefault.GetBkColour();
	pObj->SetColor(clr);
	pObj->SetUnit(m_sDefUnit);

}


void CVariableDefDlg::OnEndlabeleditTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	*pResult = FALSE;

	TV_DISPINFO* pTVDispInfo = (TV_DISPINFO*)pNMHDR;
	// TODO: Add your control notification handler code here
	CModelObj *obj = (CModelObj*) pTVDispInfo->item.lParam;
	if (!obj) return;
	
	int r = pTVDispInfo->item.mask;
	CString newStr = pTVDispInfo->item.pszText;
	if (newStr == "" || newStr != obj->GetName()) return;

	obj->SetName(newStr);

	*pResult = TRUE;
}
