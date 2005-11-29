// AnimPolygonPage.cpp : implementation file
//

#include "stdafx.h"
#include "simul.h"
#include "AnimPolygonPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAnimPolygonPage dialog


CAnimPolygonPage::CAnimPolygonPage()
	: CAnimPage(CAnimPolygonPage::IDD)
{
	//{{AFX_DATA_INIT(CAnimPolygonPage)
	m_nPolyLink = -1;
	m_nPolyPt = -1;
	//}}AFX_DATA_INIT

	m_sCpntList.SetSize(10,5);
	m_ImageList.Create(IDB_ANIM_COMPONENT,16,0,RGB(192,192,192));

}


void CAnimPolygonPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAnimPolygonPage)
	DDX_Control(pDX, IDC_START_PT, m_wndPolyPt);
	DDX_Control(pDX, IDC_START, m_wndPolyLink);
	DDX_Control(pDX, IDC_POLYGON, m_wndPolyList);
	DDX_CBIndex(pDX, IDC_START, m_nPolyLink);
	DDX_CBIndex(pDX, IDC_START_PT, m_nPolyPt);
	//}}AFX_DATA_MAP

	if (pDX->m_bSaveAndValidate)
	{
	}
}


BEGIN_MESSAGE_MAP(CAnimPolygonPage, CPropertyPage)
	//{{AFX_MSG_MAP(CAnimPolygonPage)
	ON_BN_CLICKED(IDC_POLYG_ADD, OnPolygonAdd)
	ON_BN_CLICKED(IDC_POLYG_DEL, OnPolygonDel)
	ON_NOTIFY(TVN_SELCHANGED, IDC_POLYGON, OnSelchangedPolygon)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAnimPolygonPage message handlers

BOOL CAnimPolygonPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_wndPolyList.SetImageList(&m_ImageList,TVSIL_NORMAL);

	int nb = m_sCpntList.GetSize();
	m_wndPolyLink.ResetContent();
	m_wndPolyLink.AddString(_T("< None >"));
	for (int i=0;i<nb;i++)
	{
		m_wndPolyLink.AddString(m_sCpntList.GetAt(i));
	}

	nb = m_cItems.GetSize();
	for (i=0;i<nb;i++)
	{
		CPoint pt = m_cItems.GetAt(i);

		HTREEITEM fItem = NULL;
		CBitmap mBmp;

		TV_INSERTSTRUCT	curTreeItem;

		CString mstr;
		mstr.Format(_T("Point %d"),i);
		curTreeItem.item.mask = TVIF_IMAGE | TVIF_TEXT | TVIF_PARAM | TVIF_SELECTEDIMAGE ;
		curTreeItem.hParent = TVI_ROOT;
		curTreeItem.hInsertAfter = TVI_LAST;
		curTreeItem.item.iImage = 0;
		curTreeItem.item.iSelectedImage = 0;
		curTreeItem.item.pszText = mstr.GetBuffer(mstr.GetLength());//[iItem].GetBuffer(strItems[iItem].GetLength());
		curTreeItem.item.lParam = (LPARAM) new CPoint(pt);

		HTREEITEM gItem = m_wndPolyList.InsertItem(&curTreeItem);

	}

	nb = m_wndPolyList.GetCount();
	
	m_wndPolyLink.EnableWindow(nb);
	m_wndPolyPt.EnableWindow(nb);

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAnimPolygonPage::OnPolygonAdd() 
{
	// TODO: Add your control notification handler code here
	int nb = m_wndPolyList.GetCount();
	

	HTREEITEM fItem = NULL;
	CBitmap mBmp;

	TV_INSERTSTRUCT	curTreeItem;

	CString mstr;
	mstr.Format(_T("Point %d"),nb);
	curTreeItem.item.mask = TVIF_IMAGE | TVIF_TEXT | TVIF_PARAM | TVIF_SELECTEDIMAGE ;
	curTreeItem.hParent = TVI_ROOT;
	curTreeItem.hInsertAfter = TVI_LAST;
	curTreeItem.item.iImage = 0;
	curTreeItem.item.iSelectedImage = 0;
	curTreeItem.item.pszText = mstr.GetBuffer(mstr.GetLength());//[iItem].GetBuffer(strItems[iItem].GetLength());
	curTreeItem.item.lParam = (LPARAM) new CPoint(0,0);

	HTREEITEM gItem = m_wndPolyList.InsertItem(&curTreeItem);

	m_wndPolyList.SelectItem(gItem);

}

void CAnimPolygonPage::OnPolygonDel() 
{
	// TODO: Add your control notification handler code here
	HTREEITEM hItem = m_wndPolyList.GetSelectedItem();
	if (!hItem) return;

	CPoint *pt = (CPoint*) m_wndPolyList.GetItemData(hItem);
	delete pt;
	m_wndPolyList.DeleteItem(hItem);
	m_wndPolyList.SelectItem(NULL);

	HTREEITEM hItemRoot = m_wndPolyList.GetRootItem();

	int nb = 0;

	CString mstr= m_wndPolyList.GetItemText(hItemRoot);
	mstr.Format(_T("Point %d"),nb++);
	m_wndPolyList.SetItemText(hItemRoot,mstr);

	HTREEITEM hItemChlf = m_wndPolyList.GetNextItem(hItemRoot,TVGN_NEXT   );
	while (hItemChlf)
	{
		CString mstr= m_wndPolyList.GetItemText(hItemChlf);
		mstr.Format(_T("Point %d"),nb++);
		m_wndPolyList.SetItemText(hItemChlf,mstr);

		HTREEITEM hItemChlf2 = m_wndPolyList.GetNextItem(hItemChlf,TVGN_NEXT   );
		hItemChlf = hItemChlf2;

	}
}

void CAnimPolygonPage::OnSelchangedPolygon(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	
	int nb1 = pNMTreeView->itemOld.lParam;
	int nb2 = pNMTreeView->itemNew.lParam;

	if (pNMTreeView->itemOld.hItem)
	{
		CPoint *pt = NULL;
		pt = (CPoint*) m_wndPolyList.GetItemData(pNMTreeView->itemOld.hItem);
		if (pt)
		{
			int a = m_wndPolyPt.GetCurSel();
			int b = m_wndPolyLink.GetCurSel();

			pt->x = b;
			pt->y = a;

			m_wndPolyList.SetItemData(pNMTreeView->itemOld.hItem,(DWORD)pt);
		}
	}

	if (pNMTreeView->itemNew.hItem)
	{
		CPoint *pt = NULL;
		pt = (CPoint*) m_wndPolyList.GetItemData(pNMTreeView->itemNew.hItem);
		if (pt)
		{
			m_wndPolyLink.SetCurSel(pt->x);
			m_wndPolyPt.SetCurSel(pt->y);
		}
	}

	BOOL bSel = (BOOL)(pNMTreeView->itemNew.hItem);
	m_wndPolyLink.EnableWindow(bSel);
	m_wndPolyPt.EnableWindow(bSel);

	*pResult = 0;
}


void CAnimPolygonPage::OnOK()
{
	CPropertyPage::OnOK();

	HTREEITEM hItemRoot = m_wndPolyList.GetRootItem();

	m_cItems.RemoveAll();

	if (hItemRoot)
	{
		CString mstr= m_wndPolyList.GetItemText(hItemRoot);
		CPoint *pt = NULL;
		
		pt = (CPoint*) m_wndPolyList.GetItemData(hItemRoot);
		if (pt)
		{
			m_cItems.Add(*pt);
			delete pt;
			pt = NULL;
		}

		HTREEITEM hItemChlf = m_wndPolyList.GetNextItem(hItemRoot,TVGN_NEXT   );
		while (hItemChlf)
		{
			mstr= m_wndPolyList.GetItemText(hItemChlf);
			pt = (CPoint*) m_wndPolyList.GetItemData(hItemChlf);
			if (pt)
			{
				m_cItems.Add(*pt);
				delete pt;
				pt = NULL;
			}

			HTREEITEM hItemChlf2 = m_wndPolyList.GetNextItem(hItemChlf,TVGN_NEXT   );
			hItemChlf = hItemChlf2;

		}
	}
}
