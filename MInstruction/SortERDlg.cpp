// SortERDlg.cpp : implementation file
//

#include "stdafx.h"
#include "simul.h"
#include "SortERDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSortERDlg dialog


CSortERDlg::CSortERDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSortERDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSortERDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pMER = NULL;

	BOOL bRes= m_ImageList.Create(IDB_MERSORT,16,0,RGB(192,192,192));
	//bRes = m_IconList.Create(IDB_ER_ICONS,10,0,RGB(192,192,192));
	m_IconList.Create(16,16,ILC_COLOR4|ILC_MASK,0,20);

	HICON hIcon = AfxGetApp()->LoadIcon(IDR_SIMULPIE);
	m_IconList.Add(hIcon);
	hIcon = AfxGetApp()->LoadIcon(IDR_SIMULTEXT);
	m_IconList.Add(hIcon);
	hIcon = AfxGetApp()->LoadIcon(IDR_SIMULGRAPH);
	m_IconList.Add(hIcon);
	hIcon = AfxGetApp()->LoadIcon(IDR_SIMULANIM);
	m_IconList.Add(hIcon);
	hIcon = AfxGetApp()->LoadIcon(IDR_SIMULTABLE);
	m_IconList.Add(hIcon);
	hIcon = AfxGetApp()->LoadIcon(IDR_SIMULHISTOGRAM);
	m_IconList.Add(hIcon);
	hIcon = AfxGetApp()->LoadIcon(IDR_SIMULXYGRAPH);
	m_IconList.Add(hIcon);
	hIcon = AfxGetApp()->LoadIcon(IDR_SIMULTIMER);
	m_IconList.Add(hIcon);
	hIcon = AfxGetApp()->LoadIcon(IDR_SIMULEQUATION);
	m_IconList.Add(hIcon);
	hIcon = AfxGetApp()->LoadIcon(IDR_SIMULRULER);
	m_IconList.Add(hIcon);
	hIcon = AfxGetApp()->LoadIcon(IDR_SIMULGLASS);
	m_IconList.Add(hIcon);
	hIcon = AfxGetApp()->LoadIcon(IDR_SIMULBOOK);
	m_IconList.Add(hIcon);
	hIcon = AfxGetApp()->LoadIcon(IDR_SIMULGRAPHLOG);
	m_IconList.Add(hIcon);
	hIcon = AfxGetApp()->LoadIcon(IDR_SIMULNODE);
	m_IconList.Add(hIcon);
	hIcon = AfxGetApp()->LoadIcon(IDR_SIMULTHERMO);
	m_IconList.Add(hIcon);
}


void CSortERDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSortERDlg)
	DDX_Control(pDX, IDC_MER_LIST, m_cERList);
	DDX_Control(pDX, IDC_MER_UP, m_cERUp);
	DDX_Control(pDX, IDC_MER_DOWN, m_cERDown);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSortERDlg, CDialog)
	//{{AFX_MSG_MAP(CSortERDlg)
	ON_BN_CLICKED(IDC_MER_UP, OnSelectMERUp)
	ON_BN_CLICKED(IDC_MER_DOWN, OnSelectMERDown)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_MER_LIST, OnItemchangedMerList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSortERDlg message handlers

BOOL CSortERDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_cERUp.SetIcon(m_ImageList.ExtractIcon(0));
	m_cERDown.SetIcon(m_ImageList.ExtractIcon(1));

	m_cERUp.SetAlign(CButtonST::ST_ALIGN_VERT);
	//m_cERUp.SetTooltipText(ID_CONTR_TASK);
	//m_cERUp.SetShowText(FALSE);
	//m_cERUp.DrawBorder(TRUE);
	m_cERDown.SetAlign(CButtonST::ST_ALIGN_VERT);
	//m_cERDown.SetTooltipText(ID_CONTR_TASK);
	//m_cERDown.SetShowText(FALSE);

	
	m_cERList.SetImageList(&m_IconList,LVSIL_SMALL);


	CRect rect;

	int nb = 0;
	if (m_pMER)
		nb = m_pMER->GetSize();
	
	m_cERList.GetWindowRect(&rect);
	m_cERList.InsertColumn(0, _T("Component"), LVCFMT_LEFT,
		rect.Width()-20, 0);
	m_cERList.DeleteAllItems();
	for (int i=1;i<nb;i++)
	{
		CExternRepUnit* pER = m_pMER->GetAt(i);
		if (!pER) continue;

		//UINT nID = pER->GetID();
		UINT nID = pER->GetIcon();
		CString mstr = pER->GetName();
		//mstr.Format(_T("ds dsdfsdsfafds %d"),i+1);
		LV_ITEM lvi;
		lvi.mask = LVIF_TEXT | LVIF_PARAM | LVIF_IMAGE; //| LVIF_IMAGE | LVIF_STATE;
		lvi.iImage = nID - 132;
		lvi.iItem = i;
		lvi.iSubItem = 0;
		lvi.lParam = (LPARAM)i;
		lvi.pszText = mstr.GetBuffer(mstr.GetLength());
		int nActItem = m_cERList.InsertItem(&lvi);

	}

	m_cERUp.EnableWindow(FALSE);
	m_cERDown.EnableWindow(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSortERDlg::OnSelectMERUp() 
{
	if (!m_pMER) return;
	// TODO: Add your control notification handler code here
	int nb = m_cERList.GetItemCount();
	int nSelItem = -1;
	for (int i=0;i<nb;i++)
	{
		if (m_cERList.GetItemState(i,LVIS_SELECTED)) 
		{
			nSelItem = i;
			break;
		}
	}
	if (nSelItem==-1) return;

	int nItem = m_cERList.GetItemData(nSelItem);
	CExternRepUnit* pER = m_pMER->GetAt(nItem);
	m_pMER->RemoveAt(nItem);
	m_pMER->InsertAt(nItem-1,pER);

/*	char toto[255];
	LVITEM mItem;
	mItem.iItem=nSelItem; 
	mItem.iSubItem =0;
	mItem.pszText= toto;
	mItem.cchTextMax =255;
	mItem.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM ;
	BOOL bRes = m_cERList.GetItem(&mItem);

	m_cERList.DeleteItem(nSelItem);
	mItem.iItem--; 
	mItem.mask |= LVIF_STATE ;
	mItem.state = LVIS_SELECTED;
	mItem.stateMask =LVIS_SELECTED ;
	mItem.lParam = (LPARAM)nItem-1;
	m_cERList.InsertItem(&mItem);*/
	//m_cERList.SetItemState(mItem.iItem,TRUE,LVIS_SELECTED);

	m_cERList.DeleteAllItems();
	for (i=1;i<m_pMER->GetSize();i++)
	{
		CExternRepUnit* pER = m_pMER->GetAt(i);
		if (!pER) continue;

		//UINT nID = pER->GetID();
		UINT nID = pER->GetIcon();
		CString mstr = pER->GetName();
		//mstr.Format(_T("ds dsdfsdsfafds %d"),i+1);
		LV_ITEM lvi;
		lvi.mask = LVIF_TEXT | LVIF_PARAM | LVIF_IMAGE; //| LVIF_IMAGE | LVIF_STATE;
		lvi.iImage = nID - 132;
		lvi.iItem = i;
		lvi.iSubItem = 0;
		lvi.lParam = (LPARAM)i;
		if (i==(nItem-1))
		{
			lvi.mask |= LVIF_STATE ;
			lvi.state = LVIS_SELECTED;
			lvi.stateMask =LVIS_SELECTED ;
		}
		lvi.pszText = mstr.GetBuffer(mstr.GetLength());
		int nActItem = m_cERList.InsertItem(&lvi);

	}
	BOOL bEnabU = FALSE;
	BOOL bEnabD = FALSE;

	if (nItem-1 == 1)
		bEnabD = TRUE;
	else
	{
		bEnabU = TRUE;
		bEnabD = TRUE;
	}
	m_cERUp.EnableWindow(bEnabU);
	m_cERDown.EnableWindow(bEnabD);
	
}

void CSortERDlg::OnSelectMERDown() 
{
	// TODO: Add your control notification handler code here
	if (!m_pMER) return;
	// TODO: Add your control notification handler code here
	int nb = m_cERList.GetItemCount();
	int nSelItem = -1;
	for (int i=0;i<nb;i++)
	{
		if (m_cERList.GetItemState(i,LVIS_SELECTED)) 
		{
			nSelItem = i;
			break;
		}
	}
	if (nSelItem==-1) return;

	int nItem = m_cERList.GetItemData(nSelItem);
	CExternRepUnit* pER = m_pMER->GetAt(nItem);
	m_pMER->RemoveAt(nItem);
	m_pMER->InsertAt(nItem+1,pER);

/*	char toto[255];
	LVITEM mItem;
	mItem.iItem=nSelItem; 
	mItem.iSubItem =0;
	mItem.pszText= toto;
	mItem.cchTextMax =255;
	mItem.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM ;
	BOOL bRes = m_cERList.GetItem(&mItem);

	m_cERList.DeleteItem(nSelItem);
	mItem.iItem++; 
	mItem.mask |= LVIF_STATE ;
	mItem.state = LVIS_SELECTED;
	mItem.stateMask =LVIS_SELECTED ;
	mItem.lParam = (LPARAM)nItem+1;
	m_cERList.InsertItem(&mItem);*/
	//m_cERList.SetItemState(mItem.iItem,TRUE,LVIS_SELECTED);
		m_cERList.DeleteAllItems();
	for (i=1;i<m_pMER->GetSize();i++)
	{
		CExternRepUnit* pER = m_pMER->GetAt(i);
		if (!pER) continue;

		//UINT nID = pER->GetID();
		UINT nID = pER->GetIcon();
		CString mstr = pER->GetName();
		//mstr.Format(_T("ds dsdfsdsfafds %d"),i+1);
		LV_ITEM lvi;
		lvi.mask = LVIF_TEXT | LVIF_PARAM | LVIF_IMAGE; //| LVIF_IMAGE | LVIF_STATE;
		lvi.iImage = nID - 132;
		lvi.iItem = i;
		lvi.iSubItem = 0;
		lvi.lParam = (LPARAM)i;
		if (i==(nItem+1))
		{
			lvi.mask |= LVIF_STATE ;
			lvi.state = LVIS_SELECTED;
			lvi.stateMask =LVIS_SELECTED ;
		}
		lvi.pszText = mstr.GetBuffer(mstr.GetLength());
		int nActItem = m_cERList.InsertItem(&lvi);

	}
	BOOL bEnabU = FALSE;
	BOOL bEnabD = FALSE;

	if (nItem+1 == m_pMER->GetSize()-1)
		bEnabU = TRUE;
	else
	{
		bEnabU = TRUE;
		bEnabD = TRUE;
	}
	m_cERUp.EnableWindow(bEnabU);
	m_cERDown.EnableWindow(bEnabD);
	

}

void CSortERDlg::OnItemchangedMerList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	if (!pNMListView) return;
//	if (pNMListView->uNewState != 3) return; 
	UINT nState = m_cERList.GetItemState(pNMListView->iItem,LVIS_SELECTED);
	BOOL bEnabU = FALSE;
	BOOL bEnabD = FALSE;

	if (nState)
	{
		int nb = m_cERList.GetItemCount();
		if (nb<=1)
		{
			bEnabU = FALSE;
			bEnabD = FALSE;
		}
		else if (pNMListView->iItem == 0)
			bEnabD = TRUE;
		else if (pNMListView->iItem == nb-1)
			bEnabU = TRUE;
		else
		{
			bEnabU = TRUE;
			bEnabD = TRUE;
		}
	}

	m_cERUp.EnableWindow(bEnabU);
	m_cERDown.EnableWindow(bEnabD);

	*pResult = 0;
}
