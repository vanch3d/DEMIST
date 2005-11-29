// DefineERPropSheet.cpp : implementation file
//

#include "stdafx.h"
#include "simul.h"

#include <Prefs\Pref.h>
#include <MSimulation\Model.h>
#include <MInstruction\LearningUnit.h>

#include "DefineERPropSheet.h"

#include <MInteraction\Format.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CERPropertyPage property page
CERPropertyPage::CERPropertyPage() : CPropertyPage()
{
}

CERPropertyPage::CERPropertyPage(UINT nIDTemplate) : CPropertyPage(nIDTemplate)
{
}

CERPropertyPage::~CERPropertyPage()
{
}



/////////////////////////////////////////////////////////////////////////////
// CERDefinePage dialog
/////////////////////////////////////////////////////////////////////////////
#define ID_FIRST	IDR_SIMULPIE
#define ID_LAST		IDR_SIMULTIMER


CERDefinePage::CERDefinePage(
				CExternRepUnit *pER,
				CMdEquation *pEqu,
				CLearningUnit *pLU)
	: CERPropertyPage(CERDefinePage::IDD)
{
	m_pER = pER;
	m_pEqu = pEqu;
	m_pLU = pLU;
	m_pSheet = NULL;

	int tt = m_pER->GetIcon();

	//m_nIconIDList.Add(IDR_VIEWCONTROLLER);
	m_nIconIDList.Add(IDR_SIMULPIE);
	m_nIconIDList.Add(IDR_SIMULTEXT);
	m_nIconIDList.Add(IDR_SIMULGRAPH);
	m_nIconIDList.Add(IDR_SIMULANIM);
	m_nIconIDList.Add(IDR_SIMULTABLE);
	m_nIconIDList.Add(IDR_SIMULHISTOGRAM);
	m_nIconIDList.Add(IDR_SIMULXYGRAPH);
	m_nIconIDList.Add(IDR_SIMULTIMER);
	m_nIconIDList.Add(IDR_SIMULEQUATION);
	m_nIconIDList.Add(IDR_SIMULRULER);
	m_nIconIDList.Add(IDR_SIMULGLASS);
	m_nIconIDList.Add(IDR_SIMULBOOK);
	m_nIconIDList.Add(IDR_SIMULGRAPHLOG);
	m_nIconIDList.Add(IDR_SIMULNODE);
	m_nIconIDList.Add(IDR_SIMULTHERMO);

	int res= 0;
	for (int i=0;i<m_nIconIDList.GetSize();i++)
	{
		if (m_nIconIDList.GetAt(i) == tt)
		{
			res = i;
			break;
		}
	}
	m_nCurrIcon = res;

	int nExpID = m_pER->GetID();
	if (nExpID !=-1)
		nExpID-=ID_FIRST;

	//{{AFX_DATA_INIT(CERDefinePage)
	m_bStartOpen = pER->m_bOpenStart;
	m_strName = m_pER->GetName();
	m_nCategory=-1;
	m_nExtRep = nExpID;
	m_bKeepOpen = pER->m_bKeepOpen;
	//}}AFX_DATA_INIT

	int nb = CPref::g_cCatList.GetSize();
	for (i=0;i<nb;i++)
	{
		CString mstr= CPref::g_cCatList.GetAt(i);
		if (mstr == m_pER->m_sCategory)
		{
			m_nCategory = i;
			break;
		}
	}
}

CERDefinePage::~CERDefinePage()
{
}

void CERDefinePage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CERDefinePage)
	DDX_Control(pDX, IDC_ER_ICONSPIN, m_wndERIconSpin);
	DDX_Control(pDX, IDC_ER_CAT, m_wndERCat);
	DDX_Control(pDX, IDC_ER_NAME, m_wndERName);
	DDX_Control(pDX, IDC_ER_EXTREP, m_wndERType);
	DDX_Control(pDX, IDC_ER_ICON, m_wndERIcon);
	DDX_Control(pDX, IDC_ER_START, m_wndEROpen);
	DDX_Control(pDX, IDC_ER_KEEPOPEN, m_wndERKeep);
	
	DDX_Check(pDX, IDC_ER_START, m_bStartOpen);
	DDX_Check(pDX, IDC_ER_KEEPOPEN, m_bKeepOpen);
	DDX_Text(pDX, IDC_ER_NAME, m_strName);
	DDV_MaxChars(pDX, m_strName, 255);
	DDX_CBIndex(pDX, IDC_ER_CAT, m_nCategory);
	DDX_CBIndex(pDX, IDC_ER_EXTREP, m_nExtRep);
	//}}AFX_DATA_MAP

	//if (!pDX->m_bSaveAndValidate)
	{
		SetMERIcon(m_nIconIDList.GetAt(m_nCurrIcon));

		m_wndERName.EnableWindow((m_nExtRep != -1));
		m_wndERCat.EnableWindow((m_nExtRep != -1));
		m_wndERIconSpin.EnableWindow((m_nExtRep != -1));
		m_wndEROpen.EnableWindow((m_nExtRep != -1));
	}
	if (pDX->m_bSaveAndValidate && m_nExtRep != -1)
		m_nExtRep+=ID_FIRST;
}


BEGIN_MESSAGE_MAP(CERDefinePage, CPropertyPage)
	//{{AFX_MSG_MAP(CERDefinePage)
	ON_CBN_SELCHANGE(IDC_ER_EXTREP, OnSelchangeER)
	ON_NOTIFY(UDN_DELTAPOS, IDC_ER_ICONSPIN, OnChangeIcon)
	ON_BN_CLICKED(IDC_ER_START, OnSelectSart)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CERDefinePage message handlers

void CERDefinePage::SetMERIcon(UINT id)
{
	HICON myicon = NULL;
	myicon = AfxGetApp()->LoadIcon(id);
	if (myicon)
		//SendDlgItemMessage(IDC_MERICONdd,BM_SETIMAGE,IMAGE_ICON,(LPARAM)myicon);*/
		m_wndERIcon.SetIcon(myicon);
}

void CERDefinePage::OnPreparePages()
{
	switch(ID_FIRST+m_nExtRep){
	case IDR_SIMULPIE:			// PIE CHARTS
		{
			CFormatChart *pFTable = new CFormatChart();
			pFTable->m_nMaxTime = m_pLU->GetMaxTimer();
			pFTable->m_nChartKind = CFormatChart::CHART_PIE;
			m_pER->m_cFormatSet.Add(pFTable);
			m_pER->m_cFormatSet.Add(new CFormatTranslation());
		}
		break;
	case IDR_SIMULTEXT:			// EQUATIONS
		{
			CFormatText* pFTable = new CFormatText();
			pFTable->m_nMaxTime = m_pLU->GetMaxTimer();
			m_pER->m_cFormatSet.Add(pFTable);
		}
		break;
	case IDR_SIMULGRAPH:		// X-Time GRAPH
		{
			CFormatGraph *pFTable = new CFormatGraph();
			pFTable->m_nMaxTime = m_pLU->GetMaxTimer();
			m_pER->m_cFormatSet.Add(pFTable);
			m_pER->m_cFormatSet.Add(new CFormatTranslation());
		}
		break;
	case IDR_SIMULANIM:			// ANIMATION
		{
			CFormatAnimation *pFTable = new CFormatAnimation();
			pFTable->m_nMaxTime = m_pLU->GetMaxTimer();
			m_pER->m_cFormatSet.Add(pFTable);
			m_pER->m_cFormatSet.Add(new CFormatTranslation());
		}
		break;
	case IDR_SIMULTABLE:		// TABLE
		{
			CFormatTable *pFTable = new CFormatTable();
			pFTable->m_nMaxTime = m_pLU->GetMaxTimer();
			m_pER->m_cFormatSet.Add(pFTable);
			m_pER->m_cFormatSet.Add(new CFormatTranslation());
		}
		break;
	case IDR_SIMULHISTOGRAM:	// BAR CHARTS
		{
			CFormatChart *pFTable = new CFormatChart();
			pFTable->m_nMaxTime = m_pLU->GetMaxTimer();
			pFTable->m_nChartKind = CFormatChart::CHART_BAR;
			m_pER->m_cFormatSet.Add(pFTable);
			m_pER->m_cFormatSet.Add(new CFormatTranslation());
		}
		break;
	case IDR_SIMULXYGRAPH:		// XY-GRAPH
		{
			CFormatPhasePlot *pFTable = new CFormatPhasePlot();
			pFTable->m_nMaxTime = m_pLU->GetMaxTimer();
			m_pER->m_cFormatSet.Add(pFTable);
			m_pER->m_cFormatSet.Add(new CFormatTranslation());

		}
		break;
	case IDR_SIMULTIMER:	// TIMER
		{
			CFormatTimer *pFTable = new CFormatTimer();
			pFTable->m_nMaxTime = m_pLU->GetMaxTimer();
			m_pER->m_cFormatSet.Add(pFTable);
			m_pER->m_cFormatSet.Add(new CFormatTranslation());
		}
		break;
	case ID_FIRST-1:
	default:
		{
			for (int i=0;i<m_pER->m_cFormatSet.GetSize();i++)
				delete m_pER->m_cFormatSet.GetAt(i);
			m_pER->m_cFormatSet.RemoveAll();
		}
	}

	CString mstr;
	m_wndERType.GetWindowText(mstr);
	m_wndERName.SetWindowText(mstr);
}


void CERDefinePage::OnSelectSart() 
{
	// TODO: Add your control notification handler code here
	int nCheck = m_wndEROpen.GetCheck();

	if (nCheck==0)
	{
		m_wndERKeep.EnableWindow(FALSE);
		m_wndERKeep.SetCheck(0);
	}
	else
	{
		m_wndERKeep.EnableWindow(TRUE);
	}
}



void CERDefinePage::OnSelchangeER()
{
	int res = m_wndERType.GetCurSel();
	BOOL test = (res == CB_ERR || res == m_nExtRep);
	if (test) return;

	m_nExtRep = res;
	SetMERIcon(ID_FIRST+m_nExtRep);

	res= 0;
	for (int i=0;i<m_nIconIDList.GetSize();i++)
	{
		if (m_nIconIDList.GetAt(i) == ID_FIRST+m_nExtRep)
		{
			res = i;
			break;
		}
	}
	m_nCurrIcon = res;


	if (m_pSheet) 
	{
		m_pSheet->SetPages(FALSE);
		m_pSheet->m_uID = ID_FIRST+m_nExtRep;
	}

	int nbF = m_pER->m_cFormatSet.GetSize();
	for (i =0;i<nbF;i++)
		delete m_pER->m_cFormatSet.GetAt(i);
	m_pER->m_cFormatSet.RemoveAll();

	OnPreparePages();

	if (m_pSheet) m_pSheet->SetPages(TRUE);
	UpdateData(TRUE);
}

void CERDefinePage::OnChangeIcon(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	
	pNMUpDown->iPos;
	pNMUpDown->iDelta;

	m_nCurrIcon = pNMUpDown->iPos;

	SetMERIcon(m_nIconIDList.GetAt(m_nCurrIcon));
	*pResult = 0;
}


BOOL CERDefinePage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	int nb = CPref::g_cCatList.GetSize();
	for (int i=0;i<nb;i++)
	{
		CString mstr = CPref::g_cCatList.GetAt(i);
		m_wndERCat.AddString(mstr);
	}
	if (m_nCategory != LB_ERR)
		m_wndERCat.SetCurSel(m_nCategory);

	m_wndERType.ResetContent();
	for (i=ID_FIRST;i<=ID_LAST;i++)
	{
		CString mstring;
		BOOL res = mstring.LoadString(i);
		if (res)
		{
			// Find first CR
			int r = mstring.Find("\n");
			if (r != -1)
				mstring=mstring.Left(r);
			m_wndERType.AddString(mstring);
		}
	}
	m_wndERIconSpin.SetRange(0,m_nIconIDList.GetSize()-1);
	m_wndERIconSpin.SetPos(m_nCurrIcon);
	m_wndERType.EnableWindow(m_nExtRep == -1);

	UpdateData(FALSE);
	OnSelectSart();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


/////////////////////////////////////////////////////////////////////////////
// CEROutcomePage dialog
/////////////////////////////////////////////////////////////////////////////

CEROutcomePage::CEROutcomePage(
				CExternRepUnit *pER,
				CMdEquation *pEqu,
				CLearningUnit *pLU)
	: CERPropertyPage(CEROutcomePage::IDD)
{
	m_pER = pER;
	m_pEqu = pEqu;
	m_pLU = pLU;
	m_pSheet = NULL;

	m_bForceSync = FALSE;

	//{{AFX_DATA_INIT(CEROutcomePage)
	m_bSyncDef = FALSE;
	m_bSetDefault = FALSE;
	//}}AFX_DATA_INIT

	m_pImageList.Create(IDB_TREE_CTRL_OPTIONS,16,0,RGB(255, 0, 255));
}

CEROutcomePage::~CEROutcomePage()
{
}

void CEROutcomePage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEROutcomePage)
	DDX_Check(pDX, IDC_ER_SYNCDEF, m_bSyncDef);
	DDX_Check(pDX, IDC_ER_DEFAULT, m_bSetDefault);
	DDX_GridControl(pDX, IDC_ER_GRID, m_wndOutGrid);
	//}}AFX_DATA_MAP

}


BEGIN_MESSAGE_MAP(CEROutcomePage, CPropertyPage)
	//{{AFX_MSG_MAP(CEROutcomePage)
	ON_BN_CLICKED(IDC_ER_SYNCDEF, OnSynchronise)
	ON_BN_CLICKED(IDC_ER_RESTORE, OnRestoreDefault)
    ON_NOTIFY(NM_CLICK , IDC_ER_GRID, OnGridClick)
    ON_NOTIFY(NM_RCLICK , IDC_ER_GRID, OnGridRClick)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEROutcomePage message handlers

void CEROutcomePage::OnSynchronise() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
}

BOOL CEROutcomePage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	// TODO: Add extra initialization here
	int nbpar = m_pEqu->m_cParSet.GetSize();
	int nbvar = m_pEqu->m_cVarSet.GetSize();
	int nbexp = m_pLU->m_cExpSet.GetSize();
	int nbR = nbpar+nbvar; 

	m_wndOutGrid.SetImageList(&m_pImageList);
	m_wndOutGrid.SetFixedRowCount(1);
	m_wndOutGrid.SetFixedColumnCount(1);
	m_wndOutGrid.SetColumnCount(nbexp+1);
	m_wndOutGrid.SetRowCount(nbR+1);
	m_wndOutGrid.SetRowResize(FALSE);
	m_wndOutGrid.SetColumnResize(FALSE);	
	m_wndOutGrid.SetListMode(FALSE);
	m_wndOutGrid.SetSingleRowSelection(TRUE);
	m_wndOutGrid.SetSingleColSelection(TRUE);
	m_wndOutGrid.SetHeaderSort(FALSE);
	m_wndOutGrid.SetEditable(FALSE);
	m_wndOutGrid.EnableSelection(FALSE);

	for (int i=0;i<nbexp;i++)
	{
		CMdExpertSet *pExpSet = m_pLU->m_cExpSet.GetAt(i);
		CString name = pExpSet->GetName();

		GV_ITEM Item;
		Item.mask = GVIF_TEXT|GVIF_FORMAT;
		
		Item.row = 0;
		Item.col = i+1;
		Item.nFormat = DT_CENTER |DT_WORDBREAK; //DT_CENTER|DT_VCENTER
			//Item.szText.Format(_T("%s"),CS->m_dataName);
			//Item.strText.Format(_T("%s"),CS->m_dataName);
		Item.strText = name;
		m_wndOutGrid.SetItem(&Item);
	}
	for (i=0;i<nbR;i++)
	{	
		CModelObj *pObj=NULL;
		CString name;
		int nIcon=0;
		if (i<nbpar)
		{
			pObj = m_pEqu->m_cParSet.GetAt(i);
			name = pObj->GetName();
			nIcon = pObj->GetIcon();
		}
		else
		{
			pObj = m_pEqu->m_cVarSet.GetAt(i-nbpar);
			name = pObj->GetName();
			nIcon = pObj->GetIcon();
		}

		GV_ITEM Item;
		Item.mask = GVIF_TEXT|GVIF_FORMAT|GVIF_IMAGE|GVIF_PARAM;
		
		Item.row = i+1;
		Item.col = 0;
		Item.nFormat = DT_LEFT |DT_WORDBREAK; //DT_CENTER|DT_VCENTER
			//Item.szText.Format(_T("%s"),CS->m_dataName);
			//Item.strText.Format(_T("%s"),CS->m_dataName);
		Item.strText = name;
		Item.iImage = nIcon+12;
		Item.lParam = i;
		m_wndOutGrid.SetItem(&Item);
	}

	for (int j=0;j<nbR;j++)
	{
		for (i=0;i<nbexp;i++)
		{
			BOOL bSel = FALSE;
			BOOL bAll = FALSE;
			for (int k=0;k<m_pER->m_cOutputSet.GetSize();k++)
			{
				COutcomes *po = m_pER->m_cOutputSet.GetAt(k);
				if (!po) continue;
				if ((po->m_nExpSet == i) &&(po->m_nData == j))
				{
					bSel=po->m_nSelected;
					bAll=po->m_nAllow;
					break;
				}
			}
			GV_ITEM Item;
			Item.row = j+1;
			Item.col = i+1;
			Item.mask = GVIF_IMAGE;

			// Set image :	0 Unsel	Allowed
			//				1 Sel	Allowed
			//				4 Unsel	UnAllowed
			//				5 Sel	UnAllowed
			Item.iImage = 0;
			if (bSel) Item.iImage++;
			if (!bAll) Item.iImage+=4;

			if (m_bSyncDef)
			{
				for (int r=0;r<nbexp;r++)
				{
					GV_ITEM Item2 = Item;
					Item2.col = r+1;
					m_wndOutGrid.SetItem(&Item2);

				}
				break;
			}
			else
				m_wndOutGrid.SetItem(&Item);

		}
	}

	m_wndOutGrid.AutoSizeColumns();

	if (m_bForceSync)
	{
		CWnd *pWnd = GetDlgItem(IDC_ER_SYNCDEF);
		pWnd->EnableWindow(FALSE);
		m_bSyncDef = TRUE;
	}

	UpdateData(FALSE);

	//m_outGrid.SortItems(0,FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CEROutcomePage::OnGridClick(NMHDR *pNotifyStruct, LRESULT* pResult)
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;

    int nRow = pItem->iRow;
    int nCol = pItem->iColumn;

	int nMaxR = m_wndOutGrid.GetRowCount();
	int nMaxC = m_wndOutGrid.GetColumnCount();
	if (!nRow || !nCol) return;
	if (nRow==-1 || nCol==-1) return;

	int nImage = m_wndOutGrid.GetItemImage(nRow,nCol);
	if (nImage % 2 == 0)  nImage+=1;
	else  nImage-=1;

	int nDeb = 0;
	int nFin = 0;
	int nRidx = 0;
	int nCidx = 0;

	if (m_bSyncDef)
	{
		nDeb=1-nCol;
		nFin=m_wndOutGrid.GetColumnCount()-nCol;
		nRidx = 0;
		nCidx = 1;
	}
	else
	{		
		nDeb=1;
		nFin=2;
		nRidx = 0;
		nCidx = 0;
	}

	for (int i=nDeb;i<nFin;i++)
	{
		GV_ITEM Item;
		Item.row = nRow+nRidx*i;
		Item.col = nCol+nCidx*i;
		Item.mask = GVIF_IMAGE;

		Item.iImage = nImage;
		m_wndOutGrid.SetItem(&Item);
		m_wndOutGrid.RedrawCell(nRow+nRidx*i,nCol+nCidx*i);
	}

}

void CEROutcomePage::OnGridRClick(NMHDR *pNotifyStruct, LRESULT* pResult)
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;

    int nRow = pItem->iRow;
    int nCol = pItem->iColumn;
	if (!nRow || !nCol) return;
	if (nRow==-1 || nCol==-1) return;

	int nImage = m_wndOutGrid.GetItemImage(nRow,nCol);
	if (nImage < 4) nImage+=4;
	else nImage-=4;

	int nDeb = 0;
	int nFin = 0;
	int nRidx = 0;
	int nCidx = 0;

	if (m_bSyncDef)
	{
		nDeb=1-nCol;
		nFin=m_wndOutGrid.GetColumnCount()-nCol;
		nRidx = 0;
		nCidx = 1;
	}
	/*if (!nRow)
	{
		nDeb=1;
		nFin=m_wndOutGrid.GetRowCount();
		nRidx = 1;
		nCidx = 0;
	}
	else if (!nCol)
	{
		nDeb=1;
		nFin=m_wndOutGrid.GetColumnCount();
		nRidx = 0;
		nCidx = 1;
	}*/
	else
	{		
		nDeb=1;
		nFin=2;
		nRidx = 0;
		nCidx = 0;
	}

	for (int i=nDeb;i<nFin;i++)
	{
		GV_ITEM Item;
		Item.row = nRow+nRidx*i;
		Item.col = nCol+nCidx*i;
		Item.mask = GVIF_IMAGE;

		Item.iImage = nImage;
		m_wndOutGrid.SetItem(&Item);
		m_wndOutGrid.RedrawCell(nRow+nRidx*i,nCol+nCidx*i);
	}
}

void CEROutcomePage::OnOK() 
{
	// TODO: Add extra validation here
	CPropertyPage::OnOK();

	for (int i=0;i<m_pER->m_cOutputSet.GetSize();i++)
		delete m_pER->m_cOutputSet.GetAt(i);
	m_pER->m_cOutputSet.RemoveAll();

	int maxR = m_wndOutGrid.GetRowCount();
	int maxC = m_wndOutGrid.GetColumnCount();
//	for (int nRow=1;nRow<maxR;nRow++)
//		for (int nCol=1;nCol<maxC;nCol++)
	for (int nCol=1;nCol<maxC;nCol++)
		for (int nRow=1;nRow<maxR;nRow++)
		{
			int nData = m_wndOutGrid.GetItemData(nRow,0);
			int nImage = m_wndOutGrid.GetItemImage(nRow,nCol);
			// Set image :	0 Unsel	Allowed
			//				1 Sel	Allowed
			//				4 Unsel	UnAllowed
			//				5 Sel	UnAllowed
			BOOL bCheck = (nImage %2 == 1);
			BOOL bEnable = (nImage < 2);
			if (bCheck || bEnable)
			{
				COutcomes *pOutp = new COutcomes();
				pOutp->m_nAllow = bEnable;
				pOutp->m_nSelected =bCheck;
				pOutp->m_nExpSet = nCol-1;
				pOutp->m_nData = nData;//nRow-1;
				m_pER->m_cOutputSet.Add(pOutp);
			}
	
		}

	if (m_bSetDefault)
	{
		for (i=0;i<m_pLU->m_cDefOutputSet.GetSize();i++)
			delete m_pLU->m_cDefOutputSet.GetAt(i);
		m_pLU->m_cDefOutputSet.RemoveAll();
		for (i=0;i<m_pER->m_cOutputSet.GetSize();i++)
		{
			COutcomes *po = m_pER->m_cOutputSet.GetAt(i)->Clone();
			m_pLU->m_cDefOutputSet.Add(po);
		}

	}
}


void CEROutcomePage::OnRestoreDefault() 
{
	for (int i=0;i<m_pER->m_cOutputSet.GetSize();i++)
		delete m_pER->m_cOutputSet.GetAt(i);
	m_pER->m_cOutputSet.RemoveAll();
	for (i=0;i<m_pLU->m_cDefOutputSet.GetSize();i++)
	{
		COutcomes *po = m_pLU->m_cDefOutputSet.GetAt(i)->Clone();
		m_pER->m_cOutputSet.Add(po);
	}

	int maxR = m_wndOutGrid.GetRowCount();
	int maxC = m_wndOutGrid.GetColumnCount();
	for (int nRow=1;nRow<maxR;nRow++)
		for (int nCol=1;nCol<maxC;nCol++)
		{
			GV_ITEM Item;
			Item.row = nRow;
			Item.col = nCol;
			Item.mask = GVIF_IMAGE;

			Item.iImage = 4;
			m_wndOutGrid.SetItem(&Item);
		}

	for (int k=0;k<m_pER->m_cOutputSet.GetSize();k++)
	{
		COutcomes *po = m_pER->m_cOutputSet.GetAt(k);
		if (!po) continue;
		GV_ITEM Item;
		Item.row = po->m_nData+1;
		Item.col = po->m_nExpSet+1;
		
		if (Item.row > maxR) continue;
		if (Item.col > maxC) continue;
		Item.mask = GVIF_IMAGE;

		// Set image :	0 Unsel	Allowed
		//				1 Sel	Allowed
		//				4 Unsel	UnAllowed
		//				5 Sel	UnAllowed
		Item.iImage = 0;
		if (po->m_nSelected) Item.iImage++;
		if (!po->m_nAllow) Item.iImage+=4;
		m_wndOutGrid.SetItem(&Item);
	}
	m_wndOutGrid.RedrawWindow();
}


/////////////////////////////////////////////////////////////////////////////
// CEROutcomeXYPage dialog
/////////////////////////////////////////////////////////////////////////////

CEROutcomeXYPage::CEROutcomeXYPage(
				CExternRepUnit *pER,
				CMdEquation *pEqu,
				CLearningUnit *pLU)
	: CERPropertyPage(CEROutcomeXYPage::IDD)
{
	m_pER = pER;
	m_pEqu = pEqu;
	m_pLU = pLU;
	m_pSheet = NULL;

	//{{AFX_DATA_INIT(CEROutcomeXYPage)
	m_nXOutcome = -1;
	m_nYOutcome = -1;
	m_nIso1X= -1;
	m_nIso1Y= -1;
	m_nIso2X= -1;
	m_nIso2Y= -1;
	m_nStabX= -1;
	m_nStabY= -1;
	//}}AFX_DATA_INIT

	m_ImageList.Create(IDB_TREE_CTRL_OPTIONS,16,0,RGB(255, 0, 255));
	m_ImageAggList.Create(IDB_VAR_ICONS,14,0,RGB(128,128,128));
}

CEROutcomeXYPage::~CEROutcomeXYPage()
{
}

void CEROutcomeXYPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEROutcomeXYPage)
	DDX_Control(pDX, IDC_ER_XOUTCOME, m_wndOutX);
	DDX_Control(pDX, IDC_ER_YOUTCOME, m_wndOutY);
	DDX_Control(pDX, IDC_ER_XSTAB, m_wndStabX);
	DDX_Control(pDX, IDC_ER_YSTAB, m_wndStabY);
	DDX_Control(pDX, IDC_ER_XISO1, m_wndIso1X);
	DDX_Control(pDX, IDC_ER_YISO1, m_wndIso1Y);
	DDX_Control(pDX, IDC_ER_XISO2, m_wndIso2X);
	DDX_Control(pDX, IDC_ER_YISO2, m_wndIso2Y);
	DDX_CBIndex(pDX, IDC_ER_XOUTCOME, m_nXOutcome);
	DDX_CBIndex(pDX, IDC_ER_YOUTCOME, m_nYOutcome);
	DDX_CBIndex(pDX, IDC_ER_XSTAB, m_nStabX);
	DDX_CBIndex(pDX, IDC_ER_YSTAB, m_nStabY);
	DDX_CBIndex(pDX, IDC_ER_XISO1, m_nIso1X);
	DDX_CBIndex(pDX, IDC_ER_YISO1, m_nIso1Y);
	DDX_CBIndex(pDX, IDC_ER_XISO2, m_nIso2X);
	DDX_CBIndex(pDX, IDC_ER_YISO2, m_nIso2Y);
	DDX_GridControl(pDX, IDC_ER_EXPSETGRID, m_wndOutGrid);
	//}}AFX_DATA_MAP

}


BEGIN_MESSAGE_MAP(CEROutcomeXYPage, CPropertyPage)
	//{{AFX_MSG_MAP(CEROutcomeXYPage)
    ON_NOTIFY(NM_CLICK , IDC_ER_EXPSETGRID, OnGridClick)
    ON_NOTIFY(NM_RCLICK , IDC_ER_EXPSETGRID, OnGridRClick)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEROutcomeXYPage message handlers

BOOL CEROutcomeXYPage::OnInitDialog() 
{
	CFormatPhasePlot *pDlg=NULL;
	int nbF = m_pER->m_cFormatSet.GetSize();
	if (nbF)
	{
		CFormat *pPage = m_pER->m_cFormatSet.GetAt(0);
		pDlg = DYNAMIC_DOWNCAST(CFormatPhasePlot,pPage);
	}
	if (pDlg)
	{
		m_nIso1X = pDlg->m_nIsoX0;
		m_nIso1Y = pDlg->m_nIsoY0;
		m_nIso2X = pDlg->m_nIsoXN;
		m_nIso2Y = pDlg->m_nIsoYN;
	}

	CPropertyPage::OnInitDialog();

	int nbexp = m_pLU->m_cExpSet.GetSize();
	int nbvar = m_pEqu->m_cVarSet.GetSize();
	int nbpar = m_pEqu->m_cParSet.GetSize();
	int nbR = nbpar+nbvar; 

	m_wndOutGrid.SetImageList(&m_ImageList);

	m_wndOutGrid.SetGridBkColor(::GetSysColor(COLOR_3DFACE));
	m_wndOutGrid.SetTextBkColor(::GetSysColor(COLOR_3DFACE));

	m_wndOutGrid.SetRowCount(nbexp);
	m_wndOutGrid.SetRowResize(FALSE);
	m_wndOutGrid.SetColumnResize(FALSE);	
	m_wndOutGrid.SetColumnCount(2);
	m_wndOutGrid.SetFixedRowCount(0);
	m_wndOutGrid.SetFixedColumnCount(0);

	m_wndOutGrid.EnableSelection(FALSE);
	m_wndOutGrid.SetListMode(FALSE);
	m_wndOutGrid.SetGridLines(GVL_NONE);
	m_wndOutGrid.SetSingleRowSelection(FALSE);
	m_wndOutGrid.SetSingleColSelection(FALSE);
	m_wndOutGrid.SetHeaderSort(FALSE);
	m_wndOutGrid.SetEditable(FALSE);
    m_wndOutGrid.SetTrackFocusCell(FALSE);
    m_wndOutGrid.SetFrameFocusCell(FALSE);

	for (int i=0;i<nbexp;i++)
	{
		CMdExpertSet *pExpSet = m_pLU->m_cExpSet.GetAt(i);
		CString name = pExpSet->GetName();

		GV_ITEM Item;
		Item.mask = GVIF_TEXT|GVIF_FORMAT;
		
		Item.row = i;
		Item.col = 0;
		Item.nFormat = DT_SINGLELINE | DT_END_ELLIPSIS| DT_LEFT; //DT_CENTER|DT_VCENTER
			//Item.szText.Format(_T("%s"),CS->m_dataName);
			//Item.strText.Format(_T("%s"),CS->m_dataName);
		Item.strText = name;
		m_wndOutGrid.SetItem(&Item);

		GV_ITEM Item2;
		Item2.row = i;
		Item2.col = 1;
		Item2.mask = GVIF_IMAGE;
		// Set image :	0 Unsel	Allowed
		//				1 Sel	Allowed
		//				4 Unsel	UnAllowed
		//				5 Sel	UnAllowed
		Item2.iImage = 4;
		m_wndOutGrid.SetItem(&Item2);
	}

	m_wndOutX.SetImageList(&m_ImageAggList);
	m_wndOutX.ResetContent();
	m_wndOutY.SetImageList(&m_ImageAggList);
	m_wndOutY.ResetContent();

	m_wndIso1X.SetImageList(&m_ImageAggList);
	m_wndIso1X.ResetContent();
	m_wndIso1Y.SetImageList(&m_ImageAggList);
	m_wndIso1Y.ResetContent();
	m_wndIso2X.SetImageList(&m_ImageAggList);
	m_wndIso2X.ResetContent();
	m_wndIso2Y.SetImageList(&m_ImageAggList);
	m_wndIso2Y.ResetContent();

	for (i=0;i<nbR;i++)
	{
		CString name;
		int nbIcon;
		if (i<nbpar)
		{
			CModelPar *pPar = m_pEqu->m_cParSet.GetAt(i);
			name = pPar->GetName();
			nbIcon = pPar->GetIcon();
		}
		else
		{
			CModelVar *pPar = m_pEqu->m_cVarSet.GetAt(i-nbpar);
			name = pPar->GetName();
			nbIcon = pPar->GetIcon();
		}

		COMBOBOXEXITEM cbi;
		cbi.mask = CBEIF_IMAGE | CBEIF_INDENT | CBEIF_SELECTEDIMAGE | 
					CBEIF_TEXT /*| CBEIF_OVERLAY |
					CBEIF_SELECTEDIMAGE | CBEIF_TEXT*/;

		cbi.iItem = i;
		cbi.pszText = name.GetBuffer(name.GetLength());
		cbi.cchTextMax = name.GetLength();
		cbi.iIndent = 0;//(i & 0x03);   //Set indentation according
				                    //to item position
		cbi.iImage = nbIcon;
		cbi.iSelectedImage = nbIcon;

		int nItem = m_wndOutX.InsertItem(&cbi);
		ASSERT(nItem == i);
		nItem = m_wndOutY.InsertItem(&cbi);
		ASSERT(nItem == i);
		nItem = m_wndIso1X.InsertItem(&cbi);
		ASSERT(nItem == i);
		nItem = m_wndIso1Y.InsertItem(&cbi);
		ASSERT(nItem == i);
		nItem = m_wndIso2X.InsertItem(&cbi);
		ASSERT(nItem == i);
		nItem = m_wndIso2Y.InsertItem(&cbi);
		ASSERT(nItem == i);
	}

	{
		CString name(_T("< None >"));
		COMBOBOXEXITEM cbi;
		cbi.mask = CBEIF_INDENT | CBEIF_TEXT;
		cbi.iItem = 0;
		cbi.pszText = name.GetBuffer(name.GetLength());
		cbi.cchTextMax = name.GetLength();
		cbi.iIndent = 0;

		int nItem = m_wndIso1X.InsertItem(&cbi);
		ASSERT(nItem == 0);
		nItem = m_wndIso1Y.InsertItem(&cbi);
		ASSERT(nItem == 0);
		nItem = m_wndIso2X.InsertItem(&cbi);
		ASSERT(nItem == 0);
		nItem = m_wndIso2Y.InsertItem(&cbi);
		ASSERT(nItem == 0);
	}


	for (int k=0;k<m_pER->m_cOutputSet.GetSize();k++)
	{
		COutcomes *po = m_pER->m_cOutputSet.GetAt(k);
		if (!po) continue;
		if (po->m_nData != -1)
		{
			if (m_nXOutcome == -1) m_nXOutcome = po->m_nData;
			else if (m_nYOutcome == -1) m_nYOutcome = po->m_nData;
		}
		if (po->m_nExpSet>=0 && po->m_nExpSet<nbexp)
		{
			BOOL bSel = po->m_nSelected;;
			BOOL bAll = po->m_nAllow;
			GV_ITEM Item;
			Item.row = po->m_nExpSet;
			Item.col = 1;
			Item.mask = GVIF_IMAGE;
			// Set image :	0 Unsel	Allowed
			//				1 Sel	Allowed
			//				4 Unsel	UnAllowed
			//				5 Sel	UnAllowed
			Item.iImage = 4;
			if (bSel) Item.iImage++;
			if (bAll) Item.iImage-=4;
			m_wndOutGrid.SetItem(&Item);
		}
	}

	m_wndOutX.SetCurSel(m_nXOutcome);
	m_wndOutY.SetCurSel(m_nYOutcome);

	m_wndIso1X.SetCurSel(m_nIso1X+1);
	m_wndIso1Y.SetCurSel(m_nIso1Y+1);
	m_wndIso2X.SetCurSel(m_nIso2X+1);
	m_wndIso2Y.SetCurSel(m_nIso2Y+1);

	m_wndOutGrid.AutoSizeColumns();
    CRect rect;
	int nCol = m_wndOutGrid.GetColumnWidth(0);
	int nCol2 = m_wndOutGrid.GetColumnWidth(1);
    m_wndOutGrid.GetClientRect(rect);
	if (nCol>(rect.Width()-nCol2))
	{
		m_wndOutGrid.SetColumnWidth(0,rect.Width()-nCol2);
	}
	else
		m_wndOutGrid.SetColumnWidth(1,rect.Width()-nCol);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CEROutcomeXYPage::OnOK() 
{
	// TODO: Add extra validation here
	CPropertyPage::OnOK();

	for (int i=0;i<m_pER->m_cOutputSet.GetSize();i++)
		delete m_pER->m_cOutputSet.GetAt(i);
	m_pER->m_cOutputSet.RemoveAll();

	if (m_nXOutcome == -1 || m_nYOutcome == -1) return;

	int maxR = m_wndOutGrid.GetRowCount();
	for (int nRow=0;nRow<maxR;nRow++)
	{
		int nImage = m_wndOutGrid.GetItemImage(nRow,1);
			// Set image :	0 Unsel	Allowed
			//				1 Sel	Allowed
			//				4 Unsel	UnAllowed
			//				5 Sel	UnAllowed
		BOOL bCheck = (nImage %2 == 1);
		BOOL bEnable = (nImage < 2);
		if (bCheck || bEnable)
		{
			COutcomes *pOutp = new COutcomes();
			pOutp->m_nAllow = bEnable;
			pOutp->m_nSelected =bCheck;
			pOutp->m_nExpSet = nRow;
			pOutp->m_nData = m_nXOutcome;
			m_pER->m_cOutputSet.Add(pOutp);
			
			pOutp = new COutcomes();
			pOutp->m_nAllow = bEnable;
			pOutp->m_nSelected =bCheck;
			pOutp->m_nExpSet = nRow;
			pOutp->m_nData = m_nYOutcome;
			m_pER->m_cOutputSet.Add(pOutp);
		}
	}

	CFormatPhasePlot *pDlg=NULL;
	int nbF = m_pER->m_cFormatSet.GetSize();
	if (nbF)
	{
		CFormat *pPage = m_pER->m_cFormatSet.GetAt(0);
		pDlg = DYNAMIC_DOWNCAST(CFormatPhasePlot,pPage);
	}

	if (pDlg)
	{
		pDlg->m_nIsoX0 = m_nIso1X-1;
		pDlg->m_nIsoY0 = m_nIso1Y-1;
		pDlg->m_nIsoXN = m_nIso2X-1;
		pDlg->m_nIsoYN = m_nIso2Y-1;
	}
}


void CEROutcomeXYPage::OnGridClick(NMHDR *pNotifyStruct, LRESULT* pResult)
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;

    int nRow = pItem->iRow;
    int nCol = pItem->iColumn;

	int nMaxR = m_wndOutGrid.GetRowCount();
	int nMaxC = m_wndOutGrid.GetColumnCount();
//	if (!nCol) return;
	if (nRow==-1 || nCol==-1) return;

	int nImage = m_wndOutGrid.GetItemImage(nRow,1);
	if (nImage % 2 == 0)  nImage+=1;
	else  nImage-=1;

	GV_ITEM Item;
	Item.row = nRow;
	Item.col = 1;
	Item.mask = GVIF_IMAGE;

	Item.iImage = nImage;
	m_wndOutGrid.SetItem(&Item);
	m_wndOutGrid.RedrawCell(nRow,1);
}

void CEROutcomeXYPage::OnGridRClick(NMHDR *pNotifyStruct, LRESULT* pResult)
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;

    int nRow = pItem->iRow;
    int nCol = pItem->iColumn;
//	if (!nCol) return;
	if (nRow==-1 || nCol==-1) return;

	int nImage = m_wndOutGrid.GetItemImage(nRow,1);
	if (nImage < 4) nImage+=4;
	else nImage-=4;

	GV_ITEM Item;
	Item.row = nRow;
	Item.col = 1;
	Item.mask = GVIF_IMAGE;

	Item.iImage = nImage;
	m_wndOutGrid.SetItem(&Item);
	m_wndOutGrid.RedrawCell(nRow,1);
}


/////////////////////////////////////////////////////////////////////////////
// CDefineERPropSheet
/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CDefineERPropSheet, CHelpPrSheet)

CDefineERPropSheet::CDefineERPropSheet(
				CWnd* pParentWnd /*= NULL*/,
				CExternRepUnit *pER /*= NULL*/,
				CMdEquation *pEqu /*= NULL*/,
				CLearningUnit *pLU /*= NULL*/,
				UINT iSelectPage /*= 0*/)

	:CHelpPrSheet(_T("External Representation"), pParentWnd, iSelectPage)
{
	m_psh.dwFlags |= PSH_NOAPPLYNOW;

	m_pER = pER;
	m_pEqu = pEqu;
	m_pLU = pLU;

	m_uID = m_pER->GetID();

	pLUProp = NULL;
	pLUOut = NULL;
	pLUOutXY = NULL;
	InitPropertySheet();
}


CDefineERPropSheet::~CDefineERPropSheet()
{
	if (pLUProp) delete pLUProp;
	if (pLUOut) delete pLUOut;
	if (pLUOutXY) delete pLUOutXY;
}


BEGIN_MESSAGE_MAP(CDefineERPropSheet, CHelpPrSheet)
	//{{AFX_MSG_MAP(CDefineERPropSheet)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDefineERPropSheet message handlers

BOOL CDefineERPropSheet::OnInitDialog() 
{
	CHelpPrSheet::OnInitDialog();

	CString mstr;
	GetWindowText(mstr);
	m_strTitle = mstr;

	if (m_uID!=-1)
		mstr = m_strTitle + _T(" : ") + m_pER->GetName();
	SetWindowText(mstr);

	// TODO: Add extra initialization here
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDefineERPropSheet::InitPropertySheet()
{
	pLUProp = new CERDefinePage(m_pER,m_pEqu,m_pLU);
	pLUOut = new CEROutcomePage(m_pER,m_pEqu,m_pLU);
	pLUOutXY = new CEROutcomeXYPage(m_pER,m_pEqu,m_pLU);

	CImageList pImageList;
	pImageList.Create(IDB_MODEL_ERS,16,0,RGB(192,192,192));

	pLUProp->m_psp.dwFlags |= PSP_USEHICON ;
	pLUProp->m_psp.hIcon = pImageList.ExtractIcon(0);
	pLUOut->m_psp.dwFlags |= PSP_USEHICON ;
	pLUOut->m_psp.hIcon = pImageList.ExtractIcon(1);
	pLUOutXY->m_psp.dwFlags |= PSP_USEHICON ;
	pLUOutXY->m_psp.hIcon = pImageList.ExtractIcon(1);
	//pLUTrans->m_psp.dwFlags |= PSP_USEHICON ;
	//pLUTrans->m_psp.hIcon = pImageList.ExtractIcon(2);
	//pLUFormat->m_psp.dwFlags |= PSP_USEHICON ;
	//pLUFormat->m_psp.hIcon = pImageList.ExtractIcon(3);


	pLUProp->m_pSheet = this;
	pLUOut->m_pSheet = this;
	pLUOutXY->m_pSheet = this;
	//pInstPage->m_pSheet = this;
	//pMERPage->m_pSheet = this;

	if (m_uID == IDR_SIMULHISTOGRAM ||	m_uID == IDR_SIMULPIE)
		pLUOut->m_bForceSync = TRUE;
	else
		pLUOut->m_bForceSync = FALSE;

	AddPage(pLUProp);

	if (m_pER->GetID()!=-1)
	{
		if (m_uID == IDR_SIMULXYGRAPH)
			AddPage(pLUOutXY);
		else if (m_uID != IDR_SIMULANIM)
			AddPage(pLUOut);

		//AddPage(pLUFormat);
		//AddPage(pLUTrans);

		int nbF = m_pER->m_cFormatSet.GetSize();
		for (int i=0;i<nbF;i++)
		{
			CPropertyPage *pPage = m_pER->m_cFormatSet.GetAt(i)->GetPage();
			if (pPage)
			{
				pPage->m_psp.dwFlags |= PSP_USEHICON;
				pPage->m_psp.hIcon = pImageList.ExtractIcon(2+i);
				AddPage(pPage);
			}
		}

	}
}

void CDefineERPropSheet::SetPages(BOOL bAdd)
{
	if (bAdd)
	{
		if (GetPageCount()!=1) return;
		int nbF = m_pER->m_cFormatSet.GetSize();
		CImageList pImageList;
		pImageList.Create(IDB_MODEL_ERS,16,0,RGB(192,192,192));
		pLUOut->m_psp.dwFlags |= PSP_USEHICON ;
		pLUOut->m_psp.hIcon = pImageList.ExtractIcon(1);
		pLUOutXY->m_psp.dwFlags |= PSP_USEHICON ;
		pLUOutXY->m_psp.hIcon = pImageList.ExtractIcon(1);

		if (m_uID == IDR_SIMULHISTOGRAM ||	m_uID == IDR_SIMULPIE)
			pLUOut->m_bForceSync = TRUE;
		else
			pLUOut->m_bForceSync = FALSE;

		if (m_uID!=-1)
		{
			CString mstring;
			BOOL res = mstring.LoadString(m_uID);
			if (res)
			{
				int r = mstring.Find("\n");
				if (r != -1)
					mstring=mstring.Left(r);
				CString mstr;

				mstr = m_strTitle + _T(" : ") + mstring;
				SetWindowText(mstr);
			}
		}

		if (m_uID == IDR_SIMULXYGRAPH)
			AddPage(pLUOutXY);
		else if (m_uID != IDR_SIMULANIM)
			AddPage(pLUOut);

		/*if (m_uID != IDR_SIMULXYGRAPH)
			AddPage(pLUOut);
		else
			AddPage(pLUOutXY);*/

		for (int i=0;i<nbF;i++)
		{
			CPropertyPage *pPage = m_pER->m_cFormatSet.GetAt(i)->GetPage();
			if (pPage)
			{
				pPage->m_psp.dwFlags |= PSP_USEHICON;
				pPage->m_psp.hIcon = pImageList.ExtractIcon(2+i);
				AddPage(pPage);
			}
		}
	}
	else
	{
		if (GetPageCount()==1) return;

		if (m_uID == IDR_SIMULXYGRAPH)
			RemovePage(pLUOutXY);
		else if (m_uID != IDR_SIMULANIM)
			RemovePage(pLUOut);


		int nbF = m_pER->m_cFormatSet.GetSize();
		for (int i=0;i<nbF;i++)
		{
			CFormat *pFormat = m_pER->m_cFormatSet.GetAt(i);
			if (pFormat)
			{
				CPropertyPage *pPage = pFormat->GetPage();
				if (pPage) RemovePage(pPage);
				pFormat->UpdateFormat(FALSE);
			}
		}
	}

}

int CDefineERPropSheet::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	EnableStackedTabs(TRUE);
	
	if (CHelpPrSheet::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO: Add your specialized creation code here
	
	return 0;
}

int CDefineERPropSheet::DoModal() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	int nRes = CHelpPrSheet::DoModal();

	int nbF = m_pER->m_cFormatSet.GetSize();

	for (int i=0;i<nbF;i++)
	{
		CFormat *pFormat = m_pER->m_cFormatSet.GetAt(i);
		if (pFormat)
			pFormat->UpdateFormat(nRes==IDOK);
	}

	if (nRes==IDOK)
	{
		m_pER->SetID(pLUProp->m_nExtRep);
		m_pER->SetIcon(pLUProp->m_nIconIDList.GetAt(pLUProp->m_nCurrIcon));
		CString mstr;
		mstr = pLUProp->m_strName;
		m_pER->SetName(mstr);
		m_pER->m_bIsReady = TRUE;
		//m_pER->m_nMode = pLUProp->m_nRunMode;
		//m_pER->SetTranslation(pLUProp->m_bTransMode);
		m_pER->m_bOpenStart = pLUProp->m_bStartOpen;
		m_pER->m_bKeepOpen = pLUProp->m_bKeepOpen;

		if (pLUProp->m_nCategory != LB_ERR && 
			pLUProp->m_nCategory < CPref::g_cCatList.GetSize()) 
				m_pER->m_sCategory = CPref::g_cCatList.GetAt(pLUProp->m_nCategory);
		else
			m_pER->m_sCategory.Empty();

		m_pLU->UpdateERMenu(NULL);
	}



	return nRes;
}

