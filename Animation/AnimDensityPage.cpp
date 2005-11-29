// AnimDensityPage.cpp : implementation file
//

#include "stdafx.h"
#include "simul.h"
#include "AnimDensityPage.h"

#include "AnimObject.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAnimDensityPage dialog


CAnimDensityPage::CAnimDensityPage()
	: CAnimPage(CAnimDensityPage::IDD)
{
	//{{AFX_DATA_INIT(CAnimDensityPage)
	m_nPrec = 1.0;
	m_nOutcomeCB = 0;
	m_bShowLegend = FALSE;
	m_bShowValues = 0;
	m_nNewIcon = 0;
	//}}AFX_DATA_INIT
	m_pObjects = NULL;
	//m_bmp1 = m_bmp2 = NULL;

	m_ImageList.Create(15,15,ILC_COLOR,0,10);
	m_ImageAggList.Create(IDB_VAR_ICONS,14,0,RGB(128,128,128));

	CBitmap *m_bmp1=NULL;
	m_cBitmapArray.Add(m_bmp1 = new CBitmap());
	m_bmp1->LoadBitmap(IDB_SQUARE);
	m_ImageList.Add(m_bmp1,RGB(0,0,0));
	m_cBitIdxArray.Add(IDB_SQUARE);
	m_cBitStrArray.Add(_T("Green Square"));

	m_cBitmapArray.Add(m_bmp1 = new CBitmap());
	m_bmp1->LoadBitmap(IDB_ROUND);
	m_ImageList.Add(m_bmp1,RGB(0,0,0));
	m_cBitIdxArray.Add(IDB_ROUND);
	m_cBitStrArray.Add(_T("Red Circle"));

	m_cBitmapArray.Add(m_bmp1 = new CBitmap());
	m_bmp1->LoadBitmap(IDB_ANTBLACK);
	m_ImageList.Add(m_bmp1,RGB(0,0,0));
	m_cBitIdxArray.Add(IDB_ANTBLACK);
	m_cBitStrArray.Add(_T("Black Ant"));

	m_cBitmapArray.Add(m_bmp1 = new CBitmap());
	m_bmp1->LoadBitmap(IDB_ANTRED);
	m_ImageList.Add(m_bmp1,RGB(0,0,0));
	m_cBitIdxArray.Add(IDB_ANTRED);
	m_cBitStrArray.Add(_T("Red Ant"));

	m_cBitmapArray.Add(m_bmp1 = new CBitmap());
	m_bmp1->LoadBitmap(IDB_RATKANG);
	m_ImageList.Add(m_bmp1,RGB(0,0,0));
	m_cBitIdxArray.Add(IDB_RATKANG);
	m_cBitStrArray.Add(_T("Kangaroo Rat"));

	m_cBitmapArray.Add(m_bmp1 = new CBitmap());
	m_bmp1->LoadBitmap(IDB_RATPOCK);
	m_ImageList.Add(m_bmp1,RGB(0,0,0));
	m_cBitIdxArray.Add(IDB_RATPOCK);
	m_cBitStrArray.Add(_T("Pocket Mouse"));

	m_cBitmapArray.Add(m_bmp1 = new CBitmap());
	m_bmp1->LoadBitmap(IDB_FINRED);
	m_ImageList.Add(m_bmp1,RGB(0,0,0));
	m_cBitIdxArray.Add(IDB_FINRED);
	m_cBitStrArray.Add(_T("Red Finch"));

	m_cBitmapArray.Add(m_bmp1 = new CBitmap());
	m_bmp1->LoadBitmap(IDB_FINGREEN);
	m_ImageList.Add(m_bmp1,RGB(0,0,0));
	m_cBitIdxArray.Add(IDB_FINGREEN);
	m_cBitStrArray.Add(_T("Green Finch"));

	m_cBitmapArray.Add(m_bmp1 = new CBitmap());
	m_bmp1->LoadBitmap(IDB_BARNACLE);
	m_ImageList.Add(m_bmp1,RGB(0,0,0));
	m_cBitIdxArray.Add(IDB_BARNACLE);
	m_cBitStrArray.Add(_T("Barnacle"));
}

CAnimDensityPage::~CAnimDensityPage()
{
	//if (m_bmp1) delete m_bmp1;
	//if (m_bmp2) delete m_bmp2;
	//m_bmp1 = m_bmp2 = NULL;
	for (int i=0;i<m_cBitmapArray.GetSize();i++)
	{
		delete m_cBitmapArray.GetAt(i);
	}
	m_cBitmapArray.RemoveAll();
}



void CAnimDensityPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAnimDensityPage)
	DDX_Control(pDX, IDC_OUTC_COMBOEX, m_cOutcomeEx);
	DDX_Control(pDX, IDC_DEFICON2, m_cNewIcon);
	DDX_Control(pDX, IDC_PARAM_TREE, m_popTree);
	//DDX_Control(pDX, IDC_OUTC_COMBO, m_OutcomeCB);
	DDX_Control(pDX, IDC_LCOLOR, m_clrDef);
	DDX_Control(pDX, IDC_PRECISION, m_Prec);
	DDX_Text(pDX, IDC_PRECISION, m_nPrec);
	DDV_MinMaxDouble(pDX, m_nPrec, 1e-4, 1e4);
	DDX_CBIndex(pDX, IDC_OUTC_COMBOEX, m_nOutcomeCB);
	DDX_Check(pDX, IDC_SHOWLEGEND, m_bShowLegend);
	DDX_Radio(pDX, IDC_SHOWVALUES1, m_bShowValues);
	DDX_CBIndex(pDX, IDC_DEFICON2, m_nNewIcon);
	//}}AFX_DATA_MAP

}


BEGIN_MESSAGE_MAP(CAnimDensityPage, CPropertyPage)
	//{{AFX_MSG_MAP(CAnimDensityPage)
	ON_NOTIFY(TVN_SELCHANGING, IDC_PARAM_TREE, OnSelchangingPopTree)
	ON_BN_CLICKED(IDC_SHOWLEGEND, OnShowLegend)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAnimDensityPage message handlers

BOOL CAnimDensityPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

//	m_icDef.AddImage(IDB_SQUARE);
//	m_icDef.AddImage(IDB_ROUND);

	// TODO: Add extra initialization here
	if (!m_pObjects) return TRUE;

	//m_popTree.SetImageList(&m_pImageList, TVSIL_NORMAL);
	m_popTree.SetImageList(&m_ImageList,TVSIL_NORMAL);
	m_cNewIcon.SetImageList(&m_ImageList);

	m_cOutcomeEx.SetImageList(&m_ImageAggList);

	m_cOutcomeEx.ResetContent();
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
		
		int nItem = m_cOutcomeEx.InsertItem(&cbi);
		ASSERT(nItem == 0);
	}
	int nbS1 = m_sOutcomeArray.GetSize();
	int nbS2 = m_sOutcomeIcon.GetSize();
	for (int i=0;i<nbS1;i++)
	{
		CString mstr = m_sOutcomeArray.GetAt(i);
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

		int nItem = m_cOutcomeEx.InsertItem(&cbi);
		ASSERT(nItem == i+1);
	}



	POSITION pos = m_pObjects->m_lData.GetHeadPosition ();
	int nb = 0;
	HTREEITEM fItem = NULL;
    while (pos != NULL)
	{
        CDensityData *pData = (CDensityData*)m_pObjects->m_lData.GetNext(pos);
		if (!pData) continue;

		CBitmap mBmp;
//		BOOL res = mBmp.LoadBitmap(pData->m_oPict->m_nBitmap);
//		m_icDef.AddBitmap(&mBmp);

		TV_INSERTSTRUCT	curTreeItem;

		CString mstr;
		mstr.Format(_T("Population n. %d"),nb);
//		m_lPopDensity.AddString(mstr);
		curTreeItem.item.mask = /*TVIF_IMAGE | */TVIF_TEXT | TVIF_PARAM /*| TVIF_SELECTEDIMAGE */;
		curTreeItem.hParent = TVI_ROOT;
		curTreeItem.hInsertAfter = TVI_LAST;
		//curTreeItem.item.iImage = nb;
		//curTreeItem.item.iSelectedImage = nb;
		curTreeItem.item.pszText = mstr.GetBuffer(mstr.GetLength());//[iItem].GetBuffer(strItems[iItem].GetLength());
		curTreeItem.item.lParam = (LPARAM)pData;
//		m_rghItem[iItem] = m_mytreectrl.InsertItem(&curTreeItem);
		HTREEITEM gItem = m_popTree.InsertItem(&curTreeItem);
		if (fItem == NULL)
			fItem = gItem;
		int nIdx = -1;
		for (int i=0;i<m_cBitIdxArray.GetSize();i++)
		{
			UINT ndd = m_cBitIdxArray.GetAt(i);
			if (ndd == pData->m_oPict->m_nBitmap)
			{
				nIdx=i;
				break;
			}
		}
		m_popTree.SetItemImage(gItem,nIdx,nIdx);
		nb++;
	}
	m_popTree.SelectItem(fItem);

	
	/*int nbS = m_sOutcomeArray.GetSize();
	m_OutcomeCB.ResetContent();
	m_OutcomeCB.AddString("< none >");
	for (i=0;i<nbS;i++)
	{
		CString mstr = m_sOutcomeArray.GetAt(i);
		m_OutcomeCB.AddString(mstr);
	}*/

	int nbImg = m_ImageList.GetImageCount();
	for (i=0;i<nbImg;i++)
	{
		COMBOBOXEXITEM     cbi;
		CString            str = m_cBitStrArray.GetAt(i);//(_T("       "));
		int                nItem;

		cbi.mask = CBEIF_IMAGE | CBEIF_INDENT | CBEIF_SELECTEDIMAGE | 
					CBEIF_TEXT /*| CBEIF_OVERLAY |
					CBEIF_SELECTEDIMAGE | CBEIF_TEXT*/;

		cbi.iItem = i;
		//str.Format(_T("Item %02d"), i);
		cbi.pszText = (LPTSTR)(LPCTSTR)str;
		cbi.cchTextMax = str.GetLength();
		cbi.iImage = i;
		cbi.iSelectedImage = i;
		//cbi.iOverlay = 2;
		cbi.iIndent = 1;//(i & 0x03);   //Set indentation according
				                    //to item position
		
		nItem = m_cNewIcon.InsertItem(&cbi);
		ASSERT(nItem == i);
	}

	OnInitObject((CDensityData *)m_popTree.GetItemData(fItem));
	OnShowLegend();		// Enable Show Values option
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CAnimDensityPage::OnInitObject(CDensityData *pData)
{
	if (!pData) return TRUE;

	m_nOutcomeCB = pData->m_nOutcome +1;
	m_nPrec = pData->m_nPrec;

	int nIdx = -1;
	for (int i=0;i<m_cBitIdxArray.GetSize();i++)
	{
		UINT ndd = m_cBitIdxArray.GetAt(i);
		if (ndd == pData->m_oPict->m_nBitmap)
		{
			nIdx=i;
			break;
		}
	}

	//m_cNewIcon.SetCurSel(nIdx);
	m_nNewIcon = nIdx;

	BOOL res = UpdateData(FALSE);

	return (res !=0);
}

BOOL CAnimDensityPage::OnValidObject(CDensityData *pData)
{
	if (!pData) return TRUE;
	BOOL res = UpdateData(TRUE);

	if (res)
	{
		pData->m_nOutcome = m_nOutcomeCB -1;
		pData->m_nPrec = m_nPrec;
		if (m_nNewIcon != -1)
			pData->m_oPict->m_nBitmap = m_cBitIdxArray.GetAt(m_nNewIcon);
	}

	return (res != 0);
}


void CAnimDensityPage::OnSelchangingPopTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	BOOL	r1=FALSE,
			r2=FALSE;

	// TODO: Add your control notification handler code here
	pNMTreeView->itemOld.lParam;
	pNMTreeView->itemNew.lParam;
	r1 = OnValidObject((CDensityData *)pNMTreeView->itemOld.lParam);
	if (r1)
		r2 = OnInitObject((CDensityData *)pNMTreeView->itemNew.lParam);
	
	*pResult = (!r1 || !r2);
}

void CAnimDensityPage::OnOK()
{
	CPropertyPage::OnOK();

	HTREEITEM pItem = m_popTree.GetSelectedItem();
	if (pItem)
	{
		DWORD dt = m_popTree.GetItemData(pItem);
		CDensityData *pData = (CDensityData *)dt;
		OnValidObject(pData);
	}
}

void CAnimDensityPage::OnShowLegend() 
{
	// TODO: Add your control notification handler code here
	CWnd *pWnd = GetDlgItem(IDC_SHOWLEGEND);
	if (pWnd)
	{
		UINT ps = ((CButton *)pWnd)->GetCheck();
		CWnd *pWnd2 = GetDlgItem(IDC_SHOWVALUES1);
		if (pWnd2)
			((CButton *)pWnd2)->EnableWindow(ps);
		pWnd2 = GetDlgItem(IDC_SHOWVALUES2);
		if (pWnd2)
			((CButton *)pWnd2)->EnableWindow(ps);
		pWnd2 = GetDlgItem(IDC_SHOWVALUES3);
		if (pWnd2)
			((CButton *)pWnd2)->EnableWindow(ps);
	}
	
}
