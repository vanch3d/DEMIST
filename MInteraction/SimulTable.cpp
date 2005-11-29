// SimulTable.cpp : implementation file
//

#include "stdafx.h"
#include "Simul.h"

#include "LearnerTrace.h"

#include "simulDoc.h"
#include "SimulTable.h"

#include <MSimulation\Model.h>
//#include <MInstruction\LearningUnit.h>

#include "Format.h"
#include "SelectParamDlg.h"

#include <Prefs\Pref.h>


#include "UserOutput.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//IMPLEMENT_DYNAMIC( TableInfo, CObject )
/////////////////////////////////////////////////////////////////////////////
// CViewTable

IMPLEMENT_DYNCREATE(CViewTable, CFormView)

BEGIN_MESSAGE_MAP(CViewTable, CFormView)
	//{{AFX_MSG_MAP(CViewTable)
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_COMMAND(ID_SELECTDATA, OnSelectdata)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	//////// NVL
	ON_NOTIFY(GVN_SELCHANGED,IDC_GRID,OnGridSelChanged)
	ON_NOTIFY(GVN_SELCHANGING,IDC_GRID,OnGridSelChanging)
	ON_NOTIFY(GVN_BEGINLABELEDIT,IDC_GRID,OnGridBeginEdit)
	ON_NOTIFY(GVN_ENDLABELEDIT,IDC_GRID,OnGridEndEdit)
	ON_NOTIFY(NM_DBLCLK,IDC_GRID,OnGridDblClick)
	ON_MESSAGE(DOC_UPDATE_VIEWCONTENT, OnUpdateInitContent)
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
	//ON_COMMAND_RANGE(ID_TRANS_INDEPENDENT,ID_TRANS_DYNALINKED,OnTranslationTasks)
	ON_UPDATE_COMMAND_UI_RANGE(ID_TRANS_INDEPENDENT,ID_TRANS_DYNALINKED,OnUpdateTransTasks)
	ON_MESSAGE(TRACE_VIEW_ACTIVATE, OnActivateER)
END_MESSAGE_MAP()

CViewTable::CViewTable()
	: CFormView(CViewTable::IDD)
{
	m_pER = NULL;
//	m_nCurrTTask=0;
	m_bIsInEdit = FALSE;
	m_nDeltaTime = 1.0;
}


CViewTable::~CViewTable()
{
	CTrace::T_CLOSE(GetParentFrame());
}

void CViewTable::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CViewTable)
	//}}AFX_DATA_MAP
	DDX_GridControl(pDX, IDC_GRID, m_pGridCtrl);
}



BOOL CViewTable::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	return CFormView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CViewTable drawing

void CViewTable::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

BOOL CViewTable::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	//CFormView::OnEraseBkgnd(pDC);
	return false;
}

/////////////////////////////////////////////////////////////////////////////
// CViewTable diagnostics

#ifdef _DEBUG
void CViewTable::AssertValid() const
{
	CFormView::AssertValid();
}

void CViewTable::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
CSimulDoc* CViewTable::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSimulDoc)));
	return (CSimulDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CViewTable Printing handlers

BOOL CViewTable::OnPreparePrinting(CPrintInfo* pInfo) 
{
	// TODO: call DoPreparePrinting to invoke the Print dialog box
	
	return DoPreparePrinting(pInfo);
	//CFormView::OnPreparePrinting(pInfo);
}

void CViewTable::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (m_pGridCtrl.GetSafeHwnd())
		m_pGridCtrl.OnEndPrinting(pDC, pInfo);
	//CFormView::OnEndPrinting(pDC, pInfo);
}

void CViewTable::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	if (m_pGridCtrl.GetSafeHwnd())
		m_pGridCtrl.OnBeginPrinting(pDC, pInfo);
	//CFormView::OnBeginPrinting(pDC, pInfo);
}

void CViewTable::OnPrint(CDC* pDC, CPrintInfo* pInfo) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (m_pGridCtrl.GetSafeHwnd())
		m_pGridCtrl.OnPrint(pDC, pInfo);
	//CFormView::OnPrint(pDC, pInfo);
}

/////////////////////////////////////////////////////////////////////////////
// CViewTable Updates handlers

void CViewTable::InitView()
{
	int col=1,row;
	int nbOutput = 0;
	BOOL bDiffExpSet = FALSE;
	int nGG = -1;

	CLearningUnit *pLU = GetDocument()->GetCurrentLU();


	m_nDeltaTime = 1.0;
	CMdEquation* m_pEqu = GetDocument()->GetCurrentModel();
	if (m_pEqu)
	{
		int nbPar = m_pEqu->m_cParSet.GetSize();
		CMdData *pTime = pLU->GetDataAt(nbPar);
		double d1 = pTime->GetAt(0,0);
		double d2 = pTime->GetAt(1,0);
		m_nDeltaTime = d2-d1;
	}



	if (m_pER)
	{
		nbOutput = m_pER->m_cOutputSet.GetSize();

		for (int hh = 0;hh < nbOutput ;hh++)
		{
			COutcomes *pCo= m_pER->m_cOutputSet.GetAt(hh);
			if (!pCo) continue;
			int nbExpSet = pLU->m_cExpSet.GetSize();
			if (pCo->m_nExpSet >= nbExpSet) continue;

			if (pCo->m_nSelected)
				col++;
			if (!bDiffExpSet && nGG == -1)
				nGG = pCo->m_nExpSet;
			if (!bDiffExpSet && nGG != pCo->m_nExpSet)
				bDiffExpSet = TRUE;
		}
	}

	int nStep = 1;
	CFormatTable *pDlg = NULL;
	if (pDlg = GetFormats())
	{
		nStep = pDlg->m_nTimeStep;
	}

	m_pGridCtrl.SetColumnCount(col);

	int m_nCols = m_pGridCtrl.GetColumnCount();
	int m_nFixCols = m_pGridCtrl.GetFixedColumnCount();


	CMdEquation *pEqu = GetDocument()->GetCurrentModel();
	//CLearningUnit *pLU = GetDocument()->GetCurrentLU();
	int maxT = pLU->GetMaxTimer();
	int nb = pLU->GetDataSize();

	col = 1;
	row = 1;

	for (int hh = 0;hh < nbOutput ;hh++)
	{
		if (!m_pER) continue;
		COutcomes *pCo= m_pER->m_cOutputSet.GetAt(hh);
		if (!pCo) continue;

		int nbExpSet = pLU->m_cExpSet.GetSize();
		if (pCo->m_nExpSet >= nbExpSet) continue;


		if (pCo->m_nSelected)
		{
			CMdData *pData = pLU->GetDataAt(pCo->m_nData);
			if (!pData) continue;

			GV_ITEM Item;
			Item.mask = GVIF_TEXT|GVIF_FORMAT|GVIF_PARAM;
			Item.row = 0;
			Item.col = col;
			Item.nFormat = DT_CENTER |DT_WORDBREAK; //DT_CENTER|DT_VCENTER
			Item.lParam = hh;
			CString pstr = pData->GetDataName(pLU->m_bShowAbbrev);
			if (bDiffExpSet)
			{
				CString temp(_T("\n("));
				temp += pLU->m_cExpSet.GetAt(pCo->m_nExpSet)->GetName();
				temp += _T(")");
				pstr += temp;
			}
			Item.strText.Format(_T("%s"),pstr);

			m_pGridCtrl.SetItem(&Item);
			m_pGridCtrl.SetItemFgColour(0,col,pData->GetDataInfo()->GetColor());
			//m_pGridCtrl.RedrawCell(0,col);
			col++;
		}

	}

	int nRow = (maxT)/nStep;
	m_pGridCtrl.SetRowCount(nRow+2);

	m_nCols = m_pGridCtrl.GetRowCount();
	m_nFixCols = m_pGridCtrl.GetFixedRowCount();

	// fill first row with text
	for (row = 0; row <= nRow; row++)
	{ 
		GV_ITEM Item;

		Item.mask = GVIF_TEXT|GVIF_FORMAT|GVIF_PARAM;
		Item.row = row+1;
		Item.col = 0;
		Item.nFormat = DT_LEFT|DT_WORDBREAK;
		//Item.strText.Format(_T("%d"),row);//*nStep+1);
		//Item.strText.Format(_T("%d"),row*nStep);//*nStep+1);m_nDeltaTime

		Item.strText.Format(_T("%.3g"),(double)row*nStep*m_nDeltaTime);//*nStep+1);

		Item.lParam = row*nStep;

		m_pGridCtrl.SetItem(&Item);

		for (int jj=m_nFixCols;jj<m_pGridCtrl.GetColumnCount();jj++)
		{
			//Item.row = row;
			//Item.col = jj;
			//Item.mask = GVIF_STATE;
			//Item.nState = GVIS_READONLY ;
			UINT nState = m_pGridCtrl.GetItemState(row,jj);
			nState |= GVIS_READONLY;
			m_pGridCtrl.SetItemState(row,jj,nState);

			//m_pGridCtrl.SetItem(&Item);
		}
	}

	if (IsWindow(m_pGridCtrl.m_hWnd))
	{
		m_pGridCtrl.AutoSizeColumn(0);
		m_pGridCtrl.AutoSizeRow(0);
		m_pGridCtrl.EnableTitleTips(FALSE);
	}
}

void CViewTable::OnInitialUpdate() 
{
	m_nOldTimer = 0;

	m_pImageList.Create(IDB_TIME_BREAKPOINT,16,0,RGB(255, 0, 255));
	m_pGridCtrl.SetImageList(&m_pImageList);

	m_pGridCtrl.SetRowCount(1);
	m_pGridCtrl.SetColumnCount(1);
	m_pGridCtrl.SetFixedRowCount(1);
	m_pGridCtrl.SetFixedColumnCount(1);
	m_pGridCtrl.SetEditable(TRUE);
	m_pGridCtrl.SetRowResize(FALSE);
	m_pGridCtrl.SetColumnResize(TRUE);
	m_pGridCtrl.EnableWysiwygPrinting(TRUE);
//	m_pGridCtrl.SetSingleColSelection(TRUE);
//	m_pGridCtrl.SetSingleRowSelection(TRUE);
	//m_pGridCtrl.SetFixedRowSelection(TRUE);
	//m_pGridCtrl.SetFixedColumnSelection(TRUE);
	//m_pGridCtrl.SetGridBkColor(RGB(192,192,192));
	CGridCellBase* pCellBase = m_pGridCtrl.GetDefaultCell(FALSE,FALSE);
	if (pCellBase)
	{
		pCellBase->SetBackClr(CPref::g_crFuture);//RGB(192,192,192));
		//ellBase->SetTextClr(RGB(255,255,0));
	}

	//pCellBase = (CGridDefaultCell*)m_pGridCtrl.GetDefaultCell(FALSE,FALSE);
	InitView();
	CFormView::OnInitialUpdate();
	if (m_pER)
		GetParentFrame()->SetWindowText(m_pER->GetName());
	CTrace::T_OPEN(GetParentFrame());
	CRect rect;
	GetClientRect(rect);
	if (IsWindow(m_pGridCtrl.m_hWnd))
	{
		m_pGridCtrl.AutoSizeColumn(0);
		m_pGridCtrl.AutoSizeRow(0);
		m_pGridCtrl.EnableTitleTips(FALSE);
	}
	m_pGridCtrl.MoveWindow(0,0,rect.Width(),rect.Height());

	int nBP = GetDocument()->m_nCurrBP;
	BOOL bDone = GetDocument()->m_bTaskDone;

	CTimerBreakPoint pBP;
	BOOL bEditable = GetDocument()->GetCurrentLU()->GetTimerBPAt(nBP,pBP);

	if (bEditable)
		OnUpdateBreakPoint(nBP,pBP.m_nBPType);

/*	if (nBP != -1)
	{
		CUserOutput pUserData;
		//int nUser = GetDocument()->m_cUserInput.GetCount();
		//BOOL bGet = GetDocument()->m_cUserInput.Lookup(nBP,pUserData);
		int nUser = m_pER->m_cUserInput.GetCount();
		BOOL bGet = m_pER->m_cUserInput.Lookup(nBP,pUserData);
		if (bGet)
		{
			//BOOL bEdit = (nbType==BREAKP_ACTION || nbType==BREAKP_HYPOT) && nTrans>=TRANS_DYNAL;
			//if (bEdit)
				FormatCells(pUserData.m_nTime,!bDone);
				m_bIsInEdit = pUserData.m_nTime;
		}
		//FormatCells(nBP,!bDone);
	}*/

	//POSITION pos = pDoc->m_cUserInput.GetStartPosition();
	POSITION pos = m_pER->m_cUserInput.GetStartPosition();
	while (pos)
	{
		CUserOutput pUser;
		int nb;
		//pDoc->m_cUserInput.GetNextAssoc(pos,nb,pUser);
		m_pER->m_cUserInput.GetNextAssoc(pos,nb,pUser);
		POSITION pos2 = pUser.m_lUserInput.GetStartPosition();
		while (pos2)
		{
			CUserData pUserData;
			unsigned long ptime;
			pUser.m_lUserInput.GetNextAssoc(pos2,ptime,pUserData);
			if (pUserData.m_nType == BREAKP_TEST)
			{
				FormatCells(pUserData.m_nTime,!bDone);
				//OnUpdateHypothesis(&pUserData);
			}
		}

	}
	
	
	
	CWnd *pWnd = GetParent();
	CRect mrect;
	if (pWnd)
	{
		pWnd->GetWindowRect(mrect);
		pWnd->SetWindowPos(NULL,0,0,mrect.Width()-1,mrect.Height(),SWP_NOZORDER|SWP_NOMOVE);
	}
	//SetWindowPos(NULL,0,0,0,0,SWP_NOZORDER|SWP_NOMOVE);
}

CFormatTable* CViewTable::GetFormats()
{
	CFormatTable* pDlg = NULL;

	if (!m_pER) return pDlg;
	int nbF = m_pER->m_cFormatSet.GetSize();
	if (nbF)
	{
		CFormat *pPage = m_pER->m_cFormatSet.GetAt(0);
		pDlg = DYNAMIC_DOWNCAST(CFormatTable,pPage);
	}

	return pDlg;
}

int	CViewTable::GetTranslation()
{
	int nTrans = 0;
	CFormatTranslation* pDlg = NULL;

	if (!m_pER) return nTrans;
	int nbF = m_pER->m_cFormatSet.GetSize();
	if (nbF>=2)
	{
		CFormat *pPage = m_pER->m_cFormatSet.GetAt(1);
		pDlg = DYNAMIC_DOWNCAST(CFormatTranslation,pPage);
	}
	if (pDlg) 
		nTrans = pDlg->m_nTranslation;
	return nTrans;
}

BOOL CViewTable::FormatEditCells(int nTime,BOOL nEditBP/*=TRUE*/)
{
	BOOL bRedraw = FALSE;
	
	
	
	
	//m_bIsInEdit = nTime;
	return bRedraw;
}


BOOL CViewTable::FormatCells(int nTime,BOOL nEditBP) 
{
	BOOL bRedraw = FALSE;
	if (!m_pER) return bRedraw;
	int nbOutcome = m_pER->m_cOutputSet.GetSize();
	int nTrans=GetTranslation();

	BOOL	bMin = FALSE,
			bMax = FALSE;

	int		nStep = 1;
	COLORREF clrMin = RGB(0xFF, 0xFF, 0xE0),
			 clrMax = RGB(0xFF, 0xFF, 0xE0);

	CString m_strprec(_T("%.4f"));
	CFormatTable *pDlg = NULL;
	if (pDlg = GetFormats())
	{
		m_strprec = pDlg->GetFormatString();
		bMin = pDlg->m_bMin;
		bMax = pDlg->m_bMax;
		clrMin = pDlg->m_clrMin;
		clrMax = pDlg->m_clrMax;
		nStep = pDlg->m_nTimeStep;
	}

	int nCell = nTime / nStep;
	int nRCell = nTime % nStep;

	if (nRCell) return FALSE;

	CLearningUnit* pLU = GetDocument()->GetCurrentLU();
	int nbBT = pLU->m_cTBreakSet.GetSize();

	if (nEditBP)
	{
		GV_ITEM Item;
		Item.mask = GVIF_IMAGE|GVIF_TEXT;
		Item.row = nCell+1;
		Item.strText = _T("");
		Item.col = 0;
		Item.iImage = nEditBP;
		m_pGridCtrl.SetItem(&Item);
		m_pGridCtrl.RedrawCell(nCell+1,0);
	}

	int col = 0;
	for (int hh = 0;hh < nbOutcome;hh++)
	{
		COutcomes *pCo= m_pER->m_cOutputSet.GetAt(hh);
		if (!pCo) continue;
		if (!pCo->m_nSelected) continue;

		CLearningUnit *pLU = GetDocument()->GetCurrentLU();
		CMdData *pData = pLU->GetDataAt(pCo->m_nData);
		if (!pData) continue;

		int nbExpSet = pLU->m_cExpSet.GetSize();
		if (pCo->m_nExpSet >= nbExpSet) continue;

		CModelObj *pObj = pData->GetDataInfo();

		CxIntList minList,maxList;
		pData->GetLocalMinMax(minList,maxList,pCo->m_nExpSet);

		double dValue = pData->GetAt(nTime,pCo->m_nExpSet);

		BOOL	locMax = FALSE, 
				locMin = FALSE;
		for (int i=0;i<maxList.GetSize();i++)
			if (maxList.GetAt(i)==nTime+1)
			{
				locMax=TRUE;
				break;
			}
		for (i=0;i<minList.GetSize();i++)
			if (minList.GetAt(i)==nTime+1)
			{
				locMin=TRUE;
				break;
			}
	
		int nFixCol = m_pGridCtrl.GetFixedColumnCount();

		BOOL bEditable = nEditBP && pObj->GetIcon()<3 /*&& bGet*/ ;//&& (nTrans>=TRANS_DYNAL);
		if (nEditBP>=BREAKP_HYPOT) bEditable = TRUE;
				
		DWORD dwTextStyle = DT_RIGHT|DT_VCENTER|DT_SINGLELINE;
		GV_ITEM Item;
		Item.mask = GVIF_TEXT|GVIF_FORMAT;

		Item.row = nCell+1;
		Item.col = col+nFixCol;
		Item.nFormat = dwTextStyle;
		Item.nState =0;

		if (bEditable)
		{
			Item.mask |= /*GVIF_BKCLR|*/GVIF_STATE /*| GVIF_IMAGE*/;
			Item.nState &= ~GVNI_READONLY;
			//Item.iImage = 0;
			//Item.crBkClr = RGB(255,255,255);
		}
		else
		{
			Item.mask |= /*GVIF_BKCLR|*/GVIF_STATE;
			Item.nState |= GVNI_READONLY;
		}

		CUserOutput pUserData;
		//BOOL bGet = GetDocument()->m_cUserInput.Lookup(nTime,pUserData);
		BOOL bGet = m_pER->m_cUserInput.Lookup(nTime,pUserData);
/*		if (bGet)
		{
			CUserData pData;
			int nbD = pUserData.m_lUserInput.GetCount();
			BOOL bRet= pUserData.GetAt(pCo->m_nExpSet,pCo->m_nData,pData);
			if (bRet)
			{
				Item.mask |= GVIF_IMAGE;
				Item.iImage = pData.m_nType;
			}
		}*/


		CString s2(_T(""));

		int nRunT = GetDocument()->GetRunTime();
		if (nRunT >= nTime)
			s2.Format(m_strprec,dValue);

		CUserOutput pUserData2;
		//BOOL bGet2 = GetDocument()->m_cUserInput.Lookup(nTime,pUserData2);
		BOOL bGet2 = m_pER->m_cUserInput.Lookup(nTime,pUserData2);
		if (bGet2)
		{
			CUserData pData;
			int nbD = pUserData.m_lUserInput.GetCount();
			BOOL bRet= pUserData.GetAt(pCo->m_nExpSet,pCo->m_nData,pData);
			if (bRet)
			{
				CString s3;
				if (pData.m_nType==BREAKP_ACTION)
				{
					s3.Format(m_strprec,pData.m_dOldValue);
					s2 = _T("(") + s3 + _T(")    ") + s2;
				}
				else
				{
					s3.Format(m_strprec,pData.m_dValue);
					s2 = _T("(") + s3 + _T(")    ") + s2;
				}
			}
		}




		
/*		unsigned long dKey = nTime*10000+pCo->m_nData*100+pCo->m_nExpSet;
		CUserOutput *pUI = &(GetDocument()->m_cUserInput);
		CTUserOutput *pCTU = &(pUI->m_cUserOutput);
		int nbUserAct = pCTU->GetCount();
		CUserData pUserData;
		BOOL bUser = pCTU->Lookup(dKey,pUserData);
		if (bUser && bEditable && pBP.m_nBPType == BREAKP_TEST)
		{
			CString s3;
			s3.Format(m_strprec,pUserData.m_dValue);
			if (pBP.m_nBPType == BREAKP_INFO)
			{
				s2 = s3;
			}
			else
			{
				s2 = _T("(") + s3 + _T(")       ") + s2;
			}

		}*/

		Item.strText = s2;

		int nCT = GetDocument()->GetRunTime();
		int nRT = GetDocument()->GetRunTime();

		if (nTime==nRT)
		{
			Item.mask |= GVIF_BKCLR;
			Item.crBkClr = RGB(255,255,255);
		}

		else if (bEditable)
		{
			Item.mask |= GVIF_BKCLR;
			Item.crBkClr = RGB(255,255,255);
		}
		else if (locMax && bMax)
		{
			Item.mask |= GVIF_BKCLR;
			Item.crBkClr = clrMax;
		}
		else if (locMin && bMin)
		{
			Item.mask |= GVIF_BKCLR;
			Item.crBkClr = clrMin;
		}
		else
		{
			COLORREF rgb=m_pGridCtrl.GetItemBkColour(nCell+1,col+nFixCol);
			if (rgb != CPref::g_crPast)//RGB(0xFF, 0xFF, 0xE0))//(rgb==clrMin || rgb==clrMax)
			{
				Item.mask |= GVIF_BKCLR;
				Item.crBkClr = CPref::g_crPast;//RGB(0xFF, 0xFF, 0xE0);
			}
		}
		m_pGridCtrl.SetItem(&Item);
		bRedraw = bRedraw || TRUE;

		if (bRedraw)
			m_pGridCtrl.RedrawCell(nCell+1,col+nFixCol);
		col++;
	}
	return bRedraw;
}

/////////////////////////////////////////////////////////////////////////////
/// The framework calls this function after receiving a DOC_UPDATE_TIMERDATA notification.
///
/// Response to data update
/////////////////////////////////////////////////////////////////////////////
BOOL CViewTable::OnUpdateTimer()
{
	CCellID currentCell;
	int nCurrT = GetDocument()->GetCurrTime();
	int nRunT = GetDocument()->GetRunTime();

	if (m_nOldTimer != nCurrT)
	{
		FormatCells(m_nOldTimer);
	}
	m_nOldTimer = nCurrT;
	BOOL bT=FALSE;
	if (nCurrT == nRunT)
		bT = FormatCells(nCurrT);

	if (m_bIsInEdit)
	{
		int nBP = GetDocument()->m_nCurrBP;
		BOOL bDone = GetDocument()->m_bTaskDone;
		//CTimerBreakPoint *mTInfo = (CTimerBreakPoint *)pHint;
		FormatCells(m_bIsInEdit,!bDone);
		if (bDone)
			m_bIsInEdit = 0;
		//FormatEditCells(0,FALSE);
	}

	CWnd* oldwnd = GetFocus( );
	CFormatTable *pDlg = NULL;
	int nStep = 1;
	if (pDlg = GetFormats())
		nStep = pDlg->m_nTimeStep;
	
	int nCell = nCurrT / nStep;
	int nRCell = nCurrT % nStep;

	if (!nRCell)
	{
		currentCell.row = 1+nCell;//GetDocument()->m_currTimer;
		currentCell.col=0;
		m_pGridCtrl.SetSelectedRange(currentCell.row, m_pGridCtrl.GetFixedColumnCount(),
                       currentCell.row, m_pGridCtrl.GetColumnCount()-1);
		m_pGridCtrl.EnsureVisible(currentCell);
	}
	else
	{
		m_pGridCtrl.SetSelectedRange(-1,-1,-1,-1);
	}
	if (oldwnd) oldwnd->SetFocus();
	if (bT) m_pGridCtrl.UpdateWindow();
	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
/// The framework calls this function after receiving a DOC_UPDATE_ALLDATA notification.
///
/// Response to data update
/////////////////////////////////////////////////////////////////////////////
BOOL CViewTable::OnUpdateData()
{
	BeginWaitCursor();
//		BOOL bSel = TRUE;
//		CDlgActionTable *pDlg2 = NULL;
//		if (pDlg2 = GetActions())
//		{
//			bSel = (pDlg2->m_bSelCell || pDlg2->m_bSelLine || pDlg2->m_bSelCol);
//		}	
	CLearningUnit *pLU = GetDocument()->GetCurrentLU();
	int maxT = pLU->GetMaxTimer();
	maxT = GetDocument()->GetRunTime();
	BOOL bRedraw = FALSE;

	for (int row=0;row<=maxT;row++)
	{
		BOOL bT = FormatCells(row);
		bRedraw = bRedraw | bT;
	}

	int nBP = GetDocument()->m_nCurrBP;
	BOOL bDone = GetDocument()->m_bTaskDone;

	if (nBP != -1)
	{
		CUserOutput pUserData;
		//int nUser = GetDocument()->m_cUserInput.GetCount();
		//BOOL bGet = GetDocument()->m_cUserInput.Lookup(nBP,pUserData);
		int nUser = m_pER->m_cUserInput.GetCount();
		BOOL bGet = m_pER->m_cUserInput.Lookup(nBP,pUserData);
		if (bGet)
		{
			//BOOL bEdit = (nbType==BREAKP_ACTION || nbType==BREAKP_HYPOT) && nTrans>=TRANS_DYNAL;
			//if (bEdit)
				FormatCells(pUserData.m_nTime,!bDone);
				m_bIsInEdit = pUserData.m_nTime;
		}
		//FormatCells(nBP,!bDone);
	}

	m_pGridCtrl.EnableSelection(TRUE);
	m_pGridCtrl.AutoSizeColumn(0);
	m_pGridCtrl.AutoSizeRow(0);
	EndWaitCursor();
	//Invalidate();
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
/// The framework calls this function after receiving a DOC_UPDATE_BREAKPOINT notification.
///
/// Response to data update
/////////////////////////////////////////////////////////////////////////////
BOOL CViewTable::OnUpdateBreakPoint(int nbBP,int nbType)
{
//	int nbBP = mTInfo->m_nTransLvl;
//	int nbType = mTInfo->m_nTime;
	//FormatEditCells(nbBP);
	int nTrans=GetTranslation();

	CUserOutput pUserData;
	int nUser = GetDocument()->m_cUserInput.GetCount();
	BOOL bGet = GetDocument()->m_cUserInput.Lookup(nbBP,pUserData);
//	int nUser = m_pER->m_cUserInput.GetCount();
//	BOOL bGet = m_pER->m_cUserInput.Lookup(nbBP,pUserData);

	if (bGet)
	{
		BOOL bEdit = (nbType==BREAKP_ACTION || nbType==BREAKP_HYPOT) /*&& nTrans>=TRANS_DYNAL*/;
		if (bEdit)
		{
			FormatCells(pUserData.m_nTime,nbType);
			m_bIsInEdit = pUserData.m_nTime;
		}
	}
	//FormatCells(nbBP,TRUE);
	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
/// The framework calls this function after receiving a TRANSLATION_ACTION notification.
///
/// Response to data update
/////////////////////////////////////////////////////////////////////////////
BOOL CViewTable::OnUpdateAction(CUserData *pData)
{
	//OnUpdateData();
	if (!pData) return FALSE;

	if (pData->m_bUpdateNow)
		OnUpdateData();
	//if (pData && !pData->m_bUpdateNow)
		FormatCells(pData->m_nTime,TRUE);

	if (pData->m_bUpdateNow)
	{
		m_pGridCtrl.AutoSizeColumns();
		m_pGridCtrl.Refresh();
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
/// The framework calls this function after receiving a TRANSLATION_HYPOTHESIS notification.
///
/// Response to data update
/////////////////////////////////////////////////////////////////////////////
BOOL CViewTable::OnUpdateHypothesis(CUserData *pData)
{
	GetDocument()->UpdateERUserData(m_pER,pData);
	if (pData)
	{
		FormatCells(pData->m_nTime,BREAKP_TEST);
		CFormatTable *pDlg = NULL;
		if (pDlg = GetFormats())
		{
			int nStep = pDlg->m_nTimeStep;
			int nCell = pData->m_nTime / nStep;
			int nRCell = pData->m_nTime % nStep;

			if (nRCell) return TRUE;

			m_pGridCtrl.EnsureVisible(nCell+1,0);

		}
	}
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
/// The framework calls this function after receiving a TRANSLATION_MAPRELATION notification.
///
/// Response to data update
/////////////////////////////////////////////////////////////////////////////
BOOL CViewTable::OnUpdateMapRelation(CTranslationInfo *mTInfo )
{

	int nbTrans = GetTranslation();
	if (nbTrans<TRANS_MAPRELAT) return FALSE;

	int nStep = 1;
//	CWnd* oldwnd = GetFocus( );
	CFormatTable *pDlg = NULL;
	if (pDlg = GetFormats())
	{
		nStep = pDlg->m_nTimeStep;
	}

	if (!mTInfo) 
	{
		m_pGridCtrl.SetFocusCell(-1,-1);
		m_pGridCtrl.SetSelectedRange(-1,-1,-1,-1,TRUE,TRUE);
			return FALSE;
	}
//	int nTrans = GetTranslation();

//	CCellRange mRange;
	m_pGridCtrl.SetSelectedRange(-1,-1,-1,-1,TRUE,TRUE);

	int nbD = mTInfo->m_nDatas.GetSize();
	if (!nbD)
	{
		int nTime = mTInfo->m_nTime;
		int nCell = nTime / nStep;
		int nRCell = nTime % nStep;
		if (nRCell) 
		{
			m_pGridCtrl.SetFocusCell(-1,-1);
			m_pGridCtrl.SetSelectedRange(-1,-1,-1,-1,TRUE,TRUE);
		}
		else
		{
			m_pGridCtrl.SetFocusCell(nCell+1,0);
			m_pGridCtrl.SetSelectedRange(nCell+1,0,nCell+1,m_pGridCtrl.GetColumnCount()-1,TRUE,TRUE);
		}
	}
	else for (int jj=0;jj<nbD;jj++)
	{
		int nTime = -1;//mTInfo->m_nTime;
		int nExpSet = -1;//mTInfo->m_nExpSet;
		int nData = -1;//mTInfo->m_nData;

		CTranslationInfo::CTInfo tt1 = mTInfo->m_nDatas.GetAt(jj);
		nTime = tt1.nTime;
		nExpSet = tt1.nExpSet;
		nData = tt1.nData;

		if (nTime==-1 /*|| nExpSet==-1 || nData==-1*/) continue;
		if (!m_pER) continue;

		int nCell = nTime / nStep;
		int nRCell = nTime % nStep;

		if (nRCell) 
		{
			m_pGridCtrl.SetFocusCell(-1,-1);
			m_pGridCtrl.SetSelectedRange(-1,-1,-1,-1,TRUE,TRUE);
			continue ;
		}

		int nbOutput = m_pER->m_cOutputSet.GetSize();
		int nIdx = -1;
		int nFixCol = m_pGridCtrl.GetFixedColumnCount();
		for (int i=nFixCol;i<m_pGridCtrl.GetColumnCount();i++)
		{
			LPARAM pOut = m_pGridCtrl.GetItemData(0,i);
			if (pOut < 0 || pOut >=nbOutput) continue;
			COutcomes *pCo= m_pER->m_cOutputSet.GetAt(pOut);
			if (!pCo) continue;

			if ((pCo->m_nData == nData) && 
				(pCo->m_nExpSet == nExpSet))
			{
				nIdx = i;
				break;
			}
		}
		if (nIdx != -1/* && (nTrans>=1)*/)
		{
			LPARAM pOut = m_pGridCtrl.GetItemData(0,nIdx);
			if (pOut < 0 || pOut >=nbOutput) continue;
			COutcomes *pCo= m_pER->m_cOutputSet.GetAt(pOut);
			int r=0;
//			CCellRange mrr;
//			mrr.Set(nCell+1,nIdx,nCell+1,nIdx);
//			if (mRange.IsValid())
//				mRange = mRange.Union(mrr);
//			else 
//				mRange = mrr;
			m_pGridCtrl.SetFocusCell(nCell+1,nIdx);
			int nState = m_pGridCtrl.GetItemState(nCell+1, nIdx);
			BOOL bSel = nState & GVIS_SELECTED;

            m_pGridCtrl.SetItemState(nCell+1, nIdx, nState | GVIS_SELECTED);
			//m_pGridCtrl.SetSelectedRange(nCell+1,nIdx,nCell+1,nIdx,TRUE,TRUE/*(jj==0)*/);
			m_pGridCtrl.EnsureVisible(nCell+1,nIdx);
		}
		else
		{
//			CCellRange mrr;
//			mrr.Set(nCell+1,0,nCell+1,m_pGridCtrl.GetColumnCount()-1);
//			if (mRange.IsValid())
//				mRange = mRange.Union(mrr);
//			else 
//				mRange = mrr;
		//	m_pGridCtrl.SetFocusCell(nCell+1,0);
		//	m_pGridCtrl.SetSelectedRange(nCell+1,0,nCell+1,m_pGridCtrl.GetColumnCount()-1,TRUE,TRUE);
		}
	}
//	if (mRange.IsValid())
//		m_pGridCtrl.SetSelectedRange(mRange,TRUE);
	m_pGridCtrl.Refresh();
	return TRUE;

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
void CViewTable::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	int nbTrans = GetTranslation();

	// TODO: Add your specialized code here and/or call the base class
	if (lHint == DOC_UPDATE_TIMERDATA)
	{
		OnUpdateTimer();
	}
	else if (!lHint || 
			 lHint == DOC_UPDATE_ALLDATA ||
			 (lHint == DOC_UPDATE_ER && (pHint==this))
			)
	{
		OnUpdateData();
	}
	else if (lHint == DOC_UPDATE_BREAKPOINT )
	{
		CTimerBreakPoint *mTInfo = (CTimerBreakPoint *)pHint;
		int nBPTime = mTInfo->m_tBreakPt;
		int nBPType = mTInfo->m_nBPType;
		OnUpdateBreakPoint(nBPTime,nBPType);
	}
	if (lHint == DOC_UPDATE_RESTARTSIMUL)
	{
		m_pGridCtrl.SetRowCount(1);
		m_pGridCtrl.SetColumnCount(2);
		m_nOldTimer = 0;
		InitView();
		OnUpdateData();
	}
	if ((lHint == TRANSLATION_MAPFACT) ||
		(lHint == TRANSLATION_MAPRELATION))
	{
		CTranslationInfo *mTInfo = (CTranslationInfo *)pHint;
		//if (nbTrans>=3)
			OnUpdateMapRelation(mTInfo );
		//else
		//	OnUpdateMapRelation(NULL);
	}
	else if (lHint==TRANSLATION_HYPOTHESIS)
	{
		if (nbTrans<TRANS_MAPRELAT) 
		{
		}
		else
		{
			CUserData *pData = (CUserData *)pHint;

			if (pData->m_nTLevel<=nbTrans)
				OnUpdateHypothesis(pData);
			//Invalidate();
		}
	}
	else if (lHint==TRANSLATION_ACTION)
	{
		CUserData *pData = (CUserData *)pHint;
		OnUpdateAction(pData);
		//Invalidate();
	}

}

/////////////////////////////////////////////////////////////////////////////
/// The framework calls this function after receiving a DOC_UPDATE_VIEWCONTENT notification.
///
/// Called just before the view is displayed for the first time, this function just
/// assignes to the data member m_pER a pointer to the Learning Unit (see CLearningUnit)
/// associated with that view.
/////////////////////////////////////////////////////////////////////////////
LRESULT CViewTable::OnUpdateInitContent(WPARAM wp, LPARAM lp)
{
	m_pER = (CExternRepUnit*)lp;
	return 0;
}

LRESULT CViewTable::OnActivateER(WPARAM wp, LPARAM lp)
{
	static BOOL bFirst = TRUE;

	if (!bFirst) CTrace::T_SWITCH(this, (CWnd*)wp);
	bFirst = FALSE;
	return 0;
}


/////////////////////////////////////////////////////////////////////////////
// CViewTable Events handlers

void CViewTable::OnSize(UINT nType, int cx, int cy) 
{
	CFormView::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	if (m_pGridCtrl && ::IsWindow(m_pGridCtrl.m_hWnd))
	{
		m_pGridCtrl.MoveWindow(0,0,cx,cy);

		UINT tt = ID_TASK_CANCELTASK;
	
		UINT nID,nStyle;
		int nImage;
		m_cToolBar.GetButtonInfo(2,nID,nStyle,nImage);
		m_cToolBar.SetButtonInfo(2,nID,TBBS_SEPARATOR,cx-100);

	}
}

int CViewTable::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CFormView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// ****** Add your code below this line ********** //
	//if (!m_cToolBar.Create( GetParent(),WS_CHILD | WS_VISIBLE | CBRS_TOP,120) ||
	if (!m_cToolBar.CreateEx(GetParent(), TBSTYLE_FLAT | TBSTYLE_TOOLTIPS, 
			WS_CHILD | WS_VISIBLE | CBRS_TOP | 
			/* CBRS_GRIPPER | */CBRS_TOOLTIPS | CBRS_FLYBY | CCS_ADJUSTABLE,
			CRect(0, 0, 0, 0), 120) ||
		!m_cToolBar.LoadToolBar(IDR_SIMULGRAPH))
	{
		TRACE0("Failed to create toolbar1\n");
		return -1;      // fail to create  
	}
	//m_cToolBar.SetBarStyle(CBRS_ALIGN_TOP| CBRS_TOOLTIPS | CBRS_FLYBY);
	//GetParentFrame()->RecalcLayout();

//	if (!m_pGridCtrl.Create(CRect(0,0,200,200), this, IDC_GRID/*,WS_VISIBLE*/))
//	{
//		TRACE0("Failed to create GridCtrl\n");
//		return -1;      // fail to create  
//	}
	// ****** Add your code above this line ********** //
	
	return 0;
}


void CViewTable::OnGridSelChanged(NMHDR* pNMHDR, LRESULT* pResult) 
{    
	int x = 0;
	x = x+4;
	GV_DISPINFO *pgvDispInfo = (GV_DISPINFO *)pNMHDR;
	GV_ITEM *pgvItem = &pgvDispInfo->item;
	CCellRange cRange = m_pGridCtrl.GetSelectedCellRange();
	int nb = cRange.Count();
	BOOL b1 = m_pGridCtrl.GetSingleRowSelection();
//	m_pGridCtrl.SetSingleRowSelection(TRUE);
	b1 = m_pGridCtrl.GetSingleRowSelection();

	BOOL b2 = m_pGridCtrl.GetSingleColSelection();
	BOOL b3 = m_pGridCtrl.IsSelectable();
	BOOL b4= m_pGridCtrl.GetListMode();
	int nb2 = m_pGridCtrl.GetSelectedCount();
//	m_pGridCtrl.SetListMode(TRUE);
	b4= m_pGridCtrl.GetListMode();

	if (cRange.GetMinCol()== pgvItem->col && cRange.GetMinRow()== pgvItem->row)
	{
//		GetDocument()->m_currTimer = pgvItem->row-1;
//		GetDocument()->UpdateTimerDoc(this);
	}

	*pResult = 1;

}	

void CViewTable::OnGridSelChanging(NMHDR* pNMHDR, LRESULT* pResult) 
{   
	int x = 0;
	x = x+4;
	GV_DISPINFO *pgvDispInfo = (GV_DISPINFO *)pNMHDR;
	GV_ITEM *pgvItem = &pgvDispInfo->item;
	CCellRange cRange = m_pGridCtrl.GetSelectedCellRange();
	int nb = cRange.Count();
	*pResult = 1;
}

void CViewTable::OnGridDblClick(NMHDR *pNotifyStruct, LRESULT* pResult)
{
	int nTransLvl =GetTranslation();
	//int nCurrTask = m_nCurrTTask;

	CFormatTable *pAnimPage = GetFormats();
	if (!pAnimPage) return;
	int nbTrans = GetTranslation();


	GV_DISPINFO *pgvDispInfo = (GV_DISPINFO *)pNotifyStruct;
	GV_ITEM *pgvItem = &pgvDispInfo->item;
	CCellRange cRange = m_pGridCtrl.GetSelectedCellRange();

	LPARAM pTOut = m_pGridCtrl.GetItemData(pgvItem->row,0);

	int nb = cRange.Count();
	int m_nFixCols = m_pGridCtrl.GetFixedColumnCount();

	if (nbTrans<TRANS_MAPRELAT)
	{
		//GetDocument()->UpdateMapRelation(this,NULL);
		return;
	}

	if (pgvItem->col && pgvItem->col<m_nFixCols)
	{
	}
	if (pgvItem->row && pgvItem->col>=m_nFixCols && nbTrans>=2)
	{
		int nCurrT = GetDocument()->GetRunTime();
		int nbOutput = m_pER->m_cOutputSet.GetSize();
		LPARAM pOut = m_pGridCtrl.GetItemData(0,pgvItem->col);
		if (pOut < 0 || pOut >=nbOutput) return;
		if (pTOut > nCurrT)
		{
			//GetDocument()->UpdateAllViews(this,TRANSLATION_MAPRELATION,NULL);
			GetDocument()->UpdateMapRelation(this,NULL);
			return;
		}
	
		COutcomes *pCo= m_pER->m_cOutputSet.GetAt(pOut);
		if (!pCo) return;

		CTranslationInfo mTInfo;
		mTInfo.m_nTransLvl = nTransLvl;
		mTInfo.m_nTime=pTOut;
		//mTInfo.m_nTime = pTOut/*pgvItem->row-1*/;
		//mTInfo.m_nExpSet = pCo->m_nExpSet;
		//mTInfo.m_nData = pCo->m_nData;

		mTInfo.m_nDatas.RemoveAll();
		CTranslationInfo::CTInfo tt={pTOut,pCo->m_nData,pCo->m_nExpSet};
		mTInfo.m_nDatas.Add(tt);

		//if (m_nCurrTTask==2)
		{
			//GetDocument()->m_currTimer = pTOut;
			//GetDocument()->UpdateAllViews(this,TRANSLATION_MAPRELATION,(CObject*)&mTInfo);
			GetDocument()->UpdateMapRelation(this,&mTInfo);

		}
		//	else
		//		GetDocument()->UpdateAllViews(this,TRANSLATION_MAPFACT,(CObject*)&mTInfo);
	}
	else if (!pgvItem->col && pgvItem->row && nbTrans>=2)
	{
		CTranslationInfo mTInfo;
		int nCurrT = GetDocument()->GetRunTime();
		LPARAM pOut = m_pGridCtrl.GetItemData(0,pgvItem->col);

		//if (pgvItem->row-1 > nCurrT) return;
		if (pTOut > nCurrT)
		{
			//GetDocument()->UpdateAllViews(this,TRANSLATION_MAPRELATION,NULL);
			GetDocument()->UpdateMapRelation(this,NULL);
			return;
		}
		mTInfo.m_nTransLvl = nTransLvl;
		mTInfo.m_nTime = pTOut;
		//mTInfo.m_nTime = pTOut/*pgvItem->row-1*/;
		//mTInfo.m_nExpSet = -1;
		//mTInfo.m_nData = -1;
		//GetDocument()->m_currTimer = pTOut;
		GetDocument()->UpdateMapRelation(this,&mTInfo);
	}

	*pResult = 1;
}

void CViewTable::OnGridBeginEdit(NMHDR* pNMHDR, LRESULT* pResult)
{
    GV_DISPINFO *pgvDispInfo = (GV_DISPINFO *)pNMHDR;
    GV_ITEM     *pgvItem = &pgvDispInfo->item;
	m_strModifiedCell = m_pGridCtrl.GetItemText(pgvItem->row,pgvItem->col);
	int ns = m_strModifiedCell.Find('(');
	int es = m_strModifiedCell.Find(')');
	if (ns != -1 && es != -1)
	{
		CString mstr = m_strModifiedCell.Left(es);
		int nb =mstr.GetLength();
		mstr = mstr.Right(nb-1);
		m_pGridCtrl.SetItemText(pgvItem->row,pgvItem->col,mstr);
		m_pGridCtrl.SetModified(FALSE,pgvItem->row,pgvItem->col);
		m_strModifiedCell = mstr;
	}
//	BOOL isMod = m_pGridCtrl.GetModified(pgvItem->row,pgvItem->col);
//	}
}

void CViewTable::OnGridEndEdit(NMHDR* pNMHDR, LRESULT* pResult) 
{    
    GV_DISPINFO *pgvDispInfo = (GV_DISPINFO *)pNMHDR;
    GV_ITEM     *pgvItem = &pgvDispInfo->item;
	CString df = m_pGridCtrl.GetItemText(pgvItem->row,pgvItem->col);
	BOOL isMod = m_pGridCtrl.GetModified(pgvItem->row,pgvItem->col);
	if (!isMod) 
	{
		FormatCells(pgvItem->row-1);
		return;
	}

	int nbOutput = m_pER->m_cOutputSet.GetSize();
	LPARAM pOut = m_pGridCtrl.GetItemData(0,pgvItem->col);
	if (pOut < 0 || pOut >=nbOutput) return;
	
	COutcomes *pCo= m_pER->m_cOutputSet.GetAt(pOut);
	if (!pCo) return;

	
	CLearningUnit* pLU = GetDocument()->GetCurrentLU();

	CMdData *pObjData = pLU->GetDataAt(pCo->m_nData);
	if (!pObjData) return;

	CModelObj *pObj = pObjData->GetDataInfo();
	CModelPar *pVarObj = DYNAMIC_DOWNCAST( CModelPar, pObj);

	double dNewVal = atof(df.GetBuffer(df.GetLength()));

	if (pVarObj)
	{
		double dmin = pVarObj->GetMin();
		double dmax = pVarObj->GetMax();
		if (dNewVal<dmin|| dNewVal>dmax)
		{
			int precision = 15;
			UINT nIDPrompt = AFX_IDP_PARSE_REAL_RANGE;
			TCHAR szMin[32], szMax[32];
			CString prompt;

			_stprintf(szMin, _T("%.*g"), precision, dmin);
			_stprintf(szMax, _T("%.*g"), precision, dmax);
			AfxFormatString2(prompt, nIDPrompt, szMin, szMax);

			AfxMessageBox(prompt, MB_ICONEXCLAMATION, nIDPrompt);
			m_pGridCtrl.SetItemText(pgvItem->row,pgvItem->col,m_strModifiedCell);
			return;
		}
	}

	
	LPARAM pTOut = m_pGridCtrl.GetItemData(pgvItem->row,0);

	CUserData pData;
	pData.m_dValue = atof(df.GetBuffer(df.GetLength()));
	pData.m_nExpSet = pCo->m_nExpSet; 
	pData.m_nOutcome = pCo->m_nData;
	pData.m_nTime = pTOut;//pgvItem->row-1;
	pData.m_dOldValue = atof(m_strModifiedCell.GetBuffer(m_strModifiedCell.GetLength()));


//	OnUpdateHypothesis(&pData);
	int nbTrans = GetTranslation();
	if (nbTrans<=TRANS_MAPRELAT) 
	{
		OnUpdateHypothesis(&pData);
	}
	else
	{
		GetDocument()->UpdateUserData(this,&pData);
	}

}

void CViewTable::OnSelectdata() 
{
	if (!m_pER) return;

	CSelectOutcomeDlg dlg(this,GetDocument());
	for (int i=0;i<m_pER->m_cOutputSet.GetSize();i++)
	{
		COutcomes *po = m_pER->m_cOutputSet.GetAt(i)->Clone();
		dlg.m_cLocOutputSet.Add(po);
	}
	CLearningUnit *pLU = GetDocument()->GetCurrentLU();
	for (i=0;i<pLU->m_cExpSet.GetSize();i++)
	{
		CString str= pLU->m_cExpSet.GetAt(i)->GetName();
		dlg.m_cTabLabel.Add(str);
	}
	if (dlg.DoModal() == IDOK)
	{
		for(int i = 0; i < m_pER->m_cOutputSet.GetSize();i++ )
			delete m_pER->m_cOutputSet.GetAt(i);
		m_pER->m_cOutputSet.RemoveAll();
		for (i=0;i<dlg.m_cLocOutputSet.GetSize();i++)
		{
			COutcomes *po =dlg.m_cLocOutputSet.GetAt(i)->Clone();
			m_pER->m_cOutputSet.Add(po);
		}
		CTrace::T_CHANGEOUTCOME(this,&(m_pER->m_cOutputSet));
		InitView();
		OnUpdateData();
		//OnUpdate(0,DOC_UPDATE_ALLDATA,0) ;
	}
}

void CViewTable::OnEditCopy() 
{
	// TODO: Add your command handler code here
	CCellRange mCells = m_pGridCtrl.GetSelectedCellRange();
	if (!mCells.IsValid()) return;

	CString strCopy;
	for (int i=mCells.GetMinRow();i<=mCells.GetMaxRow();i++)
	{
		for (int j=mCells.GetMinCol();j<=mCells.GetMaxCol();j++)
		{
			CString mstr = m_pGridCtrl.GetItemText(i,j);
			strCopy += mstr + _T("\t");
		}
		strCopy += _T("\n");
	}
	m_pGridCtrl.OnEditCopy();
}

void CViewTable::OnTranslationTasks(UINT nID)
{
	// TODO: Add your command handler code here
	switch (nID) {
	case ID_TASK_MAPFACT:
	case ID_TASK_MAPRELATION:
	case ID_TASK_SUPPORTACTION:
//		m_nCurrTTask = nID - ID_TASK_MAPFACT+1;
		break;
	case ID_TASK_CANCELTASK:
	default:
//		m_nCurrTTask = 0;
		break;
	}

}

void CViewTable::OnUpdateTransTasks(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	BOOL	bEnab = FALSE;
	BOOL	bCheck = FALSE;
	UINT	nT = GetTranslation();
	UINT	nTT = pCmdUI->m_nID - ID_TRANS_INDEPENDENT;
	//UINT	nCT = m_nCurrTTask;

	switch (pCmdUI->m_nID) {
	case ID_TRANS_INDEPENDENT:
	case ID_TRANS_MAPRELATION:
	case ID_TRANS_DYNALINKED:
		bCheck = (nT == (pCmdUI->m_nID - ID_TRANS_INDEPENDENT));
		bEnab = bCheck;
		break;
	default:
		break;
	}
	pCmdUI->Enable(bEnab);
	pCmdUI->SetCheck(bCheck);

}


void CViewTable::OnDestroy() 
{
	CTrace::T_CLOSE(GetParentFrame());
	CFormView::OnDestroy();
	
	// TODO: Add your message handler code here
	
}
