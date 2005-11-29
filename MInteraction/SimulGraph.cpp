// SimulGraph.cpp : implementation file
//

#include "stdafx.h"
#include "Simul.h"

#include "LearnerTrace.h"

#include "simulDoc.h"

#include "SimulXYGraph.h"
#include "SimulGraph.h"

#include <MInstruction\LearningUnit.h>

#include "SelectParamDlg.h"
#include <MSimulation\Model.h>

#include "Format.h"
#include <Prefs\Pref.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define TLISTSIZE	80
#define	IDC_CHART	200

/////////////////////////////////////////////////////////////////////////////
// CViewGraph

IMPLEMENT_DYNCREATE(CViewGraph, CScrollView)

CViewGraph::CViewGraph()
{
	m_pER = NULL;
	m_pSelData = NULL;
	m_pHypData = NULL;
	m_bCanEdit = FALSE;

	m_nTransData = -1;
	m_nHypData = -1;
//	m_nEditData = -1;

	m_nMaxTime = 0;
	m_nCurrTime = 0;

	SetScaleType();
	COLORREF m_nBkColor  = m_pGraph->GetBackColor();
	m_pGraph->SetRange(0, 0, 100, 100);
	m_pGraph->EnableMemoryDraw(TRUE);
	m_pGraph->EnableLegend(TRUE);
	m_pGraph->SetBackColor(m_nBkColor);
}

CViewGraph::~CViewGraph()
{
	CTrace::T_CLOSE(GetParentFrame());
	for (int i=0;i<m_cPPData.GetSize();i++)
	{
		CPhasePlotData* pData = m_cPPData.GetAt(i);
		if (!pData) continue;
		delete pData;
	}
	m_cPPData.RemoveAll();

	for (i=0;i<m_cUserData.GetSize();i++)
	{
		CPhasePlotData* pData = m_cUserData.GetAt(i);
		if (!pData) continue;
		delete pData;
	}
	m_cUserData.RemoveAll();
	
}

int CViewGraph::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CScrollView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// ****** Add your code below this line ********** //

	if (!m_wndToolBar.CreateEx(GetParent(), TBSTYLE_FLAT | TBSTYLE_TOOLTIPS, 
			WS_CHILD | WS_VISIBLE | CBRS_TOP | 
			/*CBRS_GRIPPER | */CBRS_TOOLTIPS | CBRS_FLYBY | CCS_ADJUSTABLE,
			CRect(0, 0, 0, 0), 120) ||
		!m_wndToolBar.LoadToolBar(IDR_SIMULGRAPH))
	{
		TRACE0("Failed to create toolbar1\n");
		return -1;      // fail to create  
	}
	//m_wndToolBar.SetBarStyle(CBRS_ALIGN_TOP| CBRS_TOOLTIPS | CBRS_FLYBY);
	//GetParentFrame()->RecalcLayout();
	// ****** Add your code above this line ********** //
	
	return 0;
}

BEGIN_MESSAGE_MAP(CViewGraph, CScrollView)
	//{{AFX_MSG_MAP(CViewGraph)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(ID_SELECTDATA, OnSelectdata)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_WM_MOUSEMOVE()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
		// Notify commands
	ON_NOTIFY_EX(TTN_NEEDTEXT, 0, OnToolTipNeedText)
	ON_MESSAGE(DOC_UPDATE_VIEWCONTENT, OnUpdateInitContent)
	ON_COMMAND(ID_FILE_PRINT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview)
	ON_MESSAGE(TRACE_VIEW_ACTIVATE, OnActivateER)
	//ON_COMMAND_RANGE(ID_TRANS_INDEPENDENT,ID_TRANS_DYNALINKED,OnTranslationTasks)
	ON_UPDATE_COMMAND_UI_RANGE(ID_TRANS_INDEPENDENT,ID_TRANS_DYNALINKED,OnUpdateTransTasks)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewGraph printing

BOOL CViewGraph::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CViewGraph::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CViewGraph::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CViewGraph::OnPrint(CDC* pDC, CPrintInfo* pInfo) 
{
	// TODO: Add your specialized code here and/or call the base class
	// Get a DC for the current window (will be a screen DC for print previewing)
/*	CDC *pCurrentDC = GetDC();        // will have dimensions of the client area
	if (!pCurrentDC) return;
	
	CSize PaperPixelsPerInch(pDC->GetDeviceCaps(LOGPIXELSX), pDC->GetDeviceCaps(LOGPIXELSY));
	CSize ScreenPixelsPerInch(pCurrentDC->GetDeviceCaps(LOGPIXELSX), pCurrentDC->GetDeviceCaps(LOGPIXELSY));
	CSize m_CharSize = pDC->GetTextExtent(_T("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSATUVWXYZ"),52);
	m_CharSize.cx /= 52;
	
	// Get the page sizes (physical and logical)
	CSize m_PaperSize = CSize(pDC->GetDeviceCaps(HORZRES), pDC->GetDeviceCaps(VERTRES));
	CSize m_LogicalPageSize;
	m_LogicalPageSize.cx = ScreenPixelsPerInch.cx * m_PaperSize.cx / PaperPixelsPerInch.cx * 3 / 2;
	m_LogicalPageSize.cy = ScreenPixelsPerInch.cy * m_PaperSize.cy / PaperPixelsPerInch.cy * 3 / 2;

	pDC->SetMapMode(MM_ISOTROPIC);
	//pDC->SetWindowExt(CSize(1200,600));//m_DocSize);
	pDC->SetWindowExt(m_LogicalPageSize);
	pDC->SetViewportExt(pInfo->m_rectDraw.Width(),pInfo->m_rectDraw.Height());
*/	
	CScrollView::OnPrint(pDC, pInfo);
}


/////////////////////////////////////////////////////////////////////////////
// CViewGraph drawing

BOOL CViewGraph::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	
	//return CScrollView::OnEraseBkgnd(pDC);
	return FALSE;
}

int DefSize[]={PS_DASH,PS_DOT,PS_DASHDOT,PS_DASHDOTDOT};

void CViewGraph::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here

	CRect rect;
	if (pDC->IsPrinting())
	{
		rect.left = rect.top = 0;
		rect.right  = pDC->GetDeviceCaps(HORZRES);
		rect.bottom = pDC->GetDeviceCaps(VERTRES);
		m_pGraph->SetPrintScale(pDC->m_hDC, rect);
	}
	else
	{
		CSize size = GetTotalSize();
		rect.left  = rect.top = 0;
        rect.right = size.cx;
        rect.bottom= size.cy;
	}

	m_pGraph->BeginDraw(pDC->m_hDC);

	int nRT = GetDocument()->GetRunTime();
	//int nCT = GetDocument()->GetCurrTime();
	int nCT = m_nCurrTime;
	int nMT = GetDocument()->GetMaxTime();

	CTPoint<double>	x(xRange.x,(nRT) ? (nRT-0.5)*m_nDeltaTime : 0),
					y(yRange.x,yRange.y);

	CTPoint<double>	x2((nRT==nMT) ? nRT*m_nDeltaTime:(nRT+0.5)*m_nDeltaTime,xRange.y),
					y2(yRange.x,yRange.y);

//	CBrush pb( RGB(0xFF, 0xFF, 0xE0));

	CDC ttt;
	CDC *pDDD = ttt.FromHandle(m_pGraph->GetHDC());
	CBrush pb( CPref::g_crPast);
	CBrush pb2( CPref::g_crFuture);
	CPen	pen(PS_SOLID,0,CPref::g_crPast);
	CPen	pen2(PS_SOLID,0,CPref::g_crFuture);
	CBrush *pOldB = pDDD->SelectObject(&pb);
	CPen *pOldP = pDDD->SelectObject(&pen);

	m_pGraph->FilledRectangle(x.x,y.x,x.y,y.y,CPref::g_crPast);
	//m_pGraph->Rectangle(x.x,y.x,x.y,y.y);

	pDDD->SelectObject(&pb2);
	pDDD->SelectObject(&pen2);

	m_pGraph->FilledRectangle(x2.x,y2.x,x2.y,y2.y,CPref::g_crFuture);

	pDDD->SelectObject(pOldP);
	pDDD->SelectObject(pOldB);

	int oldM = pDDD->SetBkMode(TRANSPARENT);

	OnDrawFrame(rect, RGB(255, 0, 0),	NULL/*m_strTitle*/);
	
//	int nRT = GetDocument()->GetRunTime();
	if (m_nMaxTime)
	{
		int nItem = m_nMaxTime; //nCT+1;
		if (m_pER)
		{
			//nItem = (m_pER->m_nMode) ? m_nMaxTime : nCT+1 ;
			nItem = nRT+1;
		}
		for (int k=0;k<m_cPPData.GetSize();k++)
		{
			CPhasePlotData *pData = m_cPPData.GetAt(k);
			if (!pData) continue;

			if (pData->m_nExpSet==0)
				m_pGraph->Lines(pData->m_pXaxis, pData->m_pYaxis, nItem, pData->m_clrData, k+1, pData->m_strName);
			else
				m_pGraph->Lines(pData->m_pXaxis, pData->m_pYaxis, nItem, pData->m_clrData, k+1, 
									pData->m_strName,0,DefSize[pData->m_nExpSet]);
		}

	}

	{
		CTPoint<double> pt[2];
		COLORREF mclr = RGB(192, 192,192);
		pt[0].x = GetDocument()->GetRunTime()*m_nDeltaTime;
		pt[0].y = yRange.x;
		pt[1].x = GetDocument()->GetRunTime()*m_nDeltaTime;
		pt[1].y = yRange.y;
		m_pGraph->Lines(pt,2,mclr);
	}


	for (int k=0;k<m_cPPData.GetSize();k++)
	{
		CPhasePlotData *pData = m_cPPData.GetAt(k);
		if (!pData) continue;

		CTPoint<double> ptTime[1];
		ptTime[0].x = pData->m_pXaxis[nCT];
		ptTime[0].y = pData->m_pYaxis[nCT];
		CString mstr;
		//mstr.Format(_T("Time : %d"),nCT);
		mstr.Format(_T("Time : %.3g"),nCT*m_nDeltaTime);

		COLORREF mclr = RGB(192, 192,192);
		if (m_nTransData == -1)
			mclr = RGB(192, 192,192);
		else if (pData->m_bSelected)
			mclr = pData->m_clrData;
		else continue;
		int nMode = m_pGraph->FCIRCLE;
		int nSize = 4;
//		if (m_bCanEdit && ptTime[0].x == GetDocument()->GetRunTime()
//			&& pData->m_bEditable)
		if (m_bCanEdit && pData->m_bEditable)
		//if (m_pSelData == pData && ptTime[0].x == GetDocument()->GetRunTime())
		{
			nMode = m_pGraph->FDIAMOND;
			nSize = 5;
		}
		m_pGraph->Markers(ptTime, 1, mclr,nMode , m_cPPData.GetSize()+2, mstr,nSize);
		if (pData->m_bSelected)
			m_pGraph->Markers(ptTime, 1, mclr,nMode , k+1, pData->m_strName,nSize);
	}

	for (int  i=0;i<m_cUserData.GetSize();i++)
	{
		CPhasePlotData* pUserData = m_cUserData.GetAt(i);
		if (!pUserData) continue;

		CTPoint<double> ptTime[1];
		ptTime[0].x = pUserData->m_pXaxis[0]*m_nDeltaTime;
		ptTime[0].y = pUserData->m_pYaxis[0];
		COLORREF mclr = pUserData->m_clrData;
		int nMode = m_pGraph->FSQUARE;
		int nSize = 4;

		if (m_pHypData && m_pHypData==pUserData)
		{
			nMode = m_pGraph->FDIAMOND;
			nSize = 5;
			{
				CTPoint<double> pt[2];
				//COLORREF mclr2 = RGB(192, 192,192);
				pt[0].x = ptTime[0].x;
				pt[0].y = yRange.x;
				pt[1].x = ptTime[0].x;
				pt[1].y = yRange.y;
				m_pGraph->Lines(pt,2,mclr,0,NULL,1,PS_DOT);
			}
		}

		if (m_nHypData != -1)
		{
			if (m_nHypData == i)
			{
				//mclr = RGB(192, 192,192);
			}
			else
			{
				mclr = RGB(192, 192,192);
			}

		}


		int nnTime = (int)(ptTime[0].x);
		int nnnId = m_cPPData.GetSize()+4;

		CString strHyp;
		strHyp.Format(_T("Hypothesis (T = %d)"),nnTime);
		m_pGraph->Markers(ptTime, 1 ,RGB(192,192,192), nMode,nnnId, strHyp, 0);
		m_pGraph->Markers(ptTime, 1, mclr, nMode,nnnId+1+i,pUserData->m_strName,nSize);
/*		if (pUserData->m_nData==2)
		{
			CTPoint<double> pt[2];

			pt[0] = CTPoint<double>(pUserData->m_pXaxis[1],pUserData->m_pYaxis[1]);
			pt[1] = CTPoint<double>(pUserData->m_pXaxis[0],pUserData->m_pYaxis[0]);
			m_pGraph->Lines(pt,2,pUserData->m_clrData,0,NULL,1,PS_DOT);
		}*/
	}

/*	for (int  i=0;i<m_cUserData.GetSize();i++)
	{
		CPhasePlotData* pUserData = m_cUserData.GetAt(i);
		if (!pUserData) continue;

		CTPoint<double> ptTime[1];
		ptTime[0].x = pUserData->m_pXaxis[0];
		ptTime[0].y = pUserData->m_pYaxis[0];
		COLORREF mclr = pUserData->m_clrData;
		int nMode = m_pGraph->FSQUARE;
		int nSize = 4;

		if (m_pHypData && m_pHypData==pUserData)
		{
			nMode = m_pGraph->FDIAMOND;
			nSize = 5;
			{
				CTPoint<double> pt[2];
				COLORREF mclr = RGB(192, 192,192);
				pt[0].x = ptTime[0].x;
				pt[0].y = yRange.x;
				pt[1].x = ptTime[0].x;
				pt[1].y = yRange.y;
				m_pGraph->Lines(pt,2,mclr);
			}
		}

		if (nnTime==-1) nnTime = (int)ptTime[0].x;

		int nnnId = m_cPPData.GetSize()+4+nnHyp;

		CString strHyp;
		strHyp.Format(_T("Hypothesis (T = %d)"),nTime);
		m_pGraph->Markers(ptTime, 1 ,RGB(192,192,192), nnnId, strHyp, 0);
		m_pGraph->Markers(ptTime, 1, mclr, nMode,nnnId+1+i,pUserData->m_strName,nSize);
	}*/

/*	}
	else
	{
			CPhasePlotData *pData = m_cPPData.GetAt(m_nTransData);
			if (pData) 
			{

			CTPoint<double> ptTime[1];
			ptTime[0].x = pData->m_pXaxis[nCT];
			ptTime[0].y = pData->m_pYaxis[nCT];
			CString mstr;
			mstr.Format(_T("Time : %d"),nCT);
			COLORREF mclr = pData->m_clrData;
			int nMode = m_pGraph->FCIRCLE;
			if (m_bCanEdit && ptTime[0].x == GetDocument()->GetRunTime())
				nMode = m_pGraph->FDIAMOND;
			m_pGraph->Markers(ptTime, 1, mclr, nMode, m_cPPData.GetSize()+2, mstr,4);
			}
	}*/

	pDDD->SetBkMode(oldM);

	m_pGraph->EndDraw(pDC->m_hDC);
}

void CViewGraph::OnDrawFrame(CRect& rect, COLORREF cr, const char* Title)
{
	m_pGraph->RecalcRects(rect);

	//m_pGraph->DrawBoundary(cr, 2);

/*	CLearningUnit*	pLU = GetDocument()->GetCurrentLU();
	int nTT = GetDocument()->m_currTimer;
	if (pLU && m_nMaxTime)
	{
		CMdData *pPrey = pLU->GetDataAt(m_nXData);
		CMdData *pPred = pLU->GetDataAt(m_nYData);
		//CTPoint<double> ptTime[1];
		//ptTime[0].x = x[nTT];
		//ptTime[0].y = y[nTT];
		CString mstr;
		COLORREF oldClr = m_pGraph->GetTitleColor();
			
			//mstr.Format(_T("%s : %.4g"),mstr1,ptTime[0].x);
		m_pGraph->SetTitleColor(pPrey->GetDataInfo()->GetColor());
		m_pGraph->XAxisTitle(m_strXPlot);

			//mstr.Format(_T("%s : %.4g"),mstr2,ptTime[0].y);
			m_pGraph->SetTitleColor(pPred->GetDataInfo()->GetColor());
			m_pGraph->YAxisTitle(m_strYPlot);

			//m_pGraph->Title(Title);
			m_pGraph->SetTitleColor(oldClr);
		}
	}*/
	m_pGraph->Axes();
	m_pGraph->Grid();
}

/////////////////////////////////////////////////////////////////////////////
// CViewGraph diagnostics

#ifdef _DEBUG
void CViewGraph::AssertValid() const
{
	CScrollView::AssertValid();
}

void CViewGraph::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CSimulDoc* CViewGraph::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSimulDoc)));
	return (CSimulDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CViewGraph Helpers

BOOL CViewGraph::IsViewActive()
{
	//Get MDI frame window
	CMDIFrameWnd *pFrame = (CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;
	// Get the active MDI child window.
	CMDIChildWnd *pChild = (CMDIChildWnd *) pFrame->GetActiveFrame();
	// or CMDIChildWnd *pChild = pFrame->MDIGetActive();
	
	// Get the active view attached to the active MDI child window.
	CView *pView = pChild->GetActiveView();

	return (pView == this);
}

CFormatGraph* CViewGraph::GetFormats()
{
	CFormatGraph* pDlg = NULL;

	if (!m_pER) return pDlg;
	int nbF = m_pER->m_cFormatSet.GetSize();
	if (nbF)
	{
		CFormat *pPage = m_pER->m_cFormatSet.GetAt(0);
		pDlg = DYNAMIC_DOWNCAST(CFormatGraph,pPage);
	}

	return pDlg;
}

int	CViewGraph::GetTranslation()
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


/*CDlgActionGraph* CViewGraph::GetActions()
{
	CDlgActionGraph* pDlg = NULL;

	if (!m_pER) return pDlg;
	int nbF = m_pER->m_cFormatSet.GetSize();
	if (nbF>=2)
	{
		CPropertyPage *pPage = m_pER->m_cFormatSet.GetAt(1);
		pDlg = DYNAMIC_DOWNCAST(CDlgActionGraph,pPage);
	}

	return pDlg;
}*/


/////////////////////////////////////////////////////////////////////////////
/// The framework calls this function after receiving a DOC_UPDATE_VIEWCONTENT notification.
///
/// Called just before the view is displayed for the first time, this function just
/// assignes to the data member m_pER a pointer to the Learning Unit (see CLearningUnit)
/// associated with that view.
/////////////////////////////////////////////////////////////////////////////
LRESULT CViewGraph::OnUpdateInitContent(WPARAM wp, LPARAM lp)
{
	m_pER = (CExternRepUnit*)lp;
	return 0;
}

LRESULT CViewGraph::OnActivateER(WPARAM wp, LPARAM lp)
{
	static BOOL bFirst = TRUE;

	if (!bFirst) CTrace::T_SWITCH(this, (CWnd*)wp);
	bFirst = FALSE;
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CViewGraph message handlers
BOOL CViewGraph::SetScaleType(int nScale)
{
	BOOL bRet = TRUE;
	switch (nScale)
	{
	case 0:
		m_pGraph = &m_Linear;
		break;
	case 1:
		m_pGraph = &m_XLinearYLog;
		break;
	default:
		bRet=FALSE;
	}
	return bRet;
}

void CViewGraph::RecalcSize()
{
	CRect rect;
	CSize sizeTotal;

	GetClientRect(rect);
	sizeTotal = rect.Size();
	SetScaleToFitSize(sizeTotal); // make it similar to CView
	m_pGraph->RecalcRects(rect);	
}

LRESULT CViewGraph::OnUpdateObjTooltip(WPARAM wp, LPARAM lp)
{
	if (!lp || !wp)
	{
		m_wndToolTip.Activate(FALSE);
		return 0L;
	}

	CPhasePlotData *pData = (CPhasePlotData*)wp;
	CLearningUnit*	pLU = GetDocument()->GetCurrentLU();

	CMdData *pObj = pLU->GetDataAt(pData->m_nYData);

	CString mstr = pData->m_strName;//pObj->GetDataName(FALSE);

	m_wndToolTip.UpdateTipText(mstr,this);
	if (!m_wndToolTip.IsWindowVisible())
	{
		CRect mmmm;
		mmmm.SetRectEmpty();
		m_wndToolTip.GetMargin(mmmm);
		m_wndToolTip.Activate(TRUE);
	}
	return 0L;
}



void CViewGraph::OnInitialUpdate() 
{
	if (m_pER)
		GetParentFrame()->SetWindowText(m_pER->GetName());
	CTrace::T_OPEN(GetParentFrame());

	m_wndToolTip.Create(this);
	m_wndToolTip.AddTool(this);
	m_wndToolTip.SetDelayTime(TTDT_INITIAL,10);
	m_wndToolTip.SetDelayTime(TTDT_RESHOW,100);

	CFormatGraph *pDlg = GetFormats();
	if (pDlg)
	{
		SetScaleType(pDlg->m_bAutoScale);
	}

	m_nDeltaTime = 1.0;

	OnUpdateTimer();
	CScrollView::OnInitialUpdate();

	CSimulDoc *pDoc = GetDocument();

	BOOL bDrawPoints= TRUE;
	double x_range = pDoc->GetMaxTime()-1;

	CRect  rect;
    GetClientRect(&rect);
    CSize sizeTotal = CSize(rect.right-rect.left, rect.bottom-rect.top);
	SetScaleToFitSize(sizeTotal);



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
				OnUpdateHypothesis(&pUserData);
			}
		}

	}

	m_pGraph->EnableLegend(TRUE);
	RecalcSize();
	//Invalidate();

	CWnd *pWnd = GetParent();
	CRect mrect;
	if (pWnd)
	{
		pWnd->GetWindowRect(mrect);
		pWnd->SetWindowPos(NULL,0,0,mrect.Width()-1,mrect.Height(),SWP_NOZORDER|SWP_NOMOVE);
	}

}

BOOL CViewGraph::CanEdit()
{
	CSimulDoc *pDoc = GetDocument();
	if (!pDoc) return FALSE;

	int	nTrans = GetTranslation();
	//if (nTrans != TRANS_DYNAL) return FALSE;

	BOOL bD = pDoc->m_bTaskDone;

	int nCBP = pDoc->m_nCurrBP;
//	int nCT = pDoc->GetCurrTime();
	int nCT = m_nCurrTime;
	int nMT = pDoc->GetMaxTime();
	int nRT = pDoc->GetRunTime();

	CTimerBreakPoint pBP;
	BOOL bEditable = pDoc->GetCurrentLU()->GetTimerBPAt(nCBP,pBP);
	if (!bEditable) return FALSE;

	bEditable = (nCT==nRT && nCT==nCBP) &&
				(pBP.m_nBPType == BREAKP_ACTION || pBP.m_nBPType == BREAKP_HYPOT) &&
				!bD;

	return bEditable;

}

/////////////////////////////////////////////////////////////////////////////
/// The framework calls this function after receiving a DOC_UPDATE_BREAKPOINT notification.
///
/// Response to data update
/////////////////////////////////////////////////////////////////////////////
BOOL CViewGraph::OnUpdateBreakPoint(int nBPTime,int nBPType)
{
	CSimulDoc *pDoc = GetDocument();
	if (!pDoc) return FALSE;

	m_bCanEdit = FALSE;
	m_pSelData = NULL;
	m_pHypData = NULL;

	BOOL bD = pDoc->m_bTaskDone;

	int nCBP = pDoc->m_nCurrBP;
	//int nCT = pDoc->GetCurrTime();
	int nCT = m_nCurrTime;
	int nMT = pDoc->GetMaxTime();
	int nRT = pDoc->GetRunTime();

	m_bCanEdit = CanEdit();

//	CTimerBreakPoint pBP;
//	BOOL bEditable = pDoc->GetCurrentLU()->GetTimerBPAt(nBPTime,pBP);
//	if (bEditable)
//	{
//		m_bCanEdit = (pBP.m_nBPType == BREAKP_ACTION || pBP.m_nBPType == BREAKP_HYPOT);
//	}
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
/// The framework calls this function after receiving a TRANSLATION_ACTION notification.
///
/// Response to data update
/////////////////////////////////////////////////////////////////////////////
BOOL CViewGraph::OnUpdateAction(CUserData *pData)
{
	int nbTrans = GetTranslation();
	BOOL bTrans = (nbTrans<TRANS_DYNAL);

	//OnUpdateData();
	//if (pData)
	BOOL bRet = FALSE;
	//	FormatCells(pData->m_nTime,TRUE);
	if (pData && !pData->m_bUpdateNow)
	{
		if (bTrans) return FALSE;
		for (int  i=0;i<m_cPPData.GetSize();i++)
		{
			CPhasePlotData* pUserData = m_cPPData.GetAt(i);
			if (!pUserData) continue;
			if ((pUserData->m_nExpSet == pData->m_nExpSet) && 
				(pUserData->m_nYData== pData->m_nOutcome))
			{
				//(nRT-0.5)*m_nDeltaTime
				pUserData->m_pXaxis[pData->m_nTime] = pData->m_nTime*m_nDeltaTime;
				pUserData->m_pYaxis[pData->m_nTime] = pData->m_dValue;
				bRet = TRUE;
				break;
			}
		}
	}
	else bRet = OnUpdateData();

	if (pData && pData->m_bUpdateNow)
	{
		m_nTransData = -1;
		m_nHypData = -1;

		m_pSelData = NULL;
		m_pHypData = NULL;
	}

	return bRet;
}

/////////////////////////////////////////////////////////////////////////////
/// The framework calls this function after receiving a TRANSLATION_HYPOTHESIS notification.
///
/// Response to data update
/////////////////////////////////////////////////////////////////////////////
BOOL CViewGraph::OnUpdateHypothesis(CUserData *pData)
{
	if (!pData) return FALSE;

	GetDocument()->UpdateERUserData(m_pER,pData);


	int nbTrans = GetTranslation();
//	if (nbTrans<TRANS_DYNAL) return FALSE;

	int nRT = GetDocument()->GetRunTime();

	BOOL bOld = FALSE;
	double oldPt[2];

	for (int  i=0;i<m_cPPData.GetSize();i++)
	{
		CPhasePlotData* pUserData = m_cPPData.GetAt(i);
		if (!pUserData) continue;
		if ((pUserData->m_nExpSet == pData->m_nExpSet) && (pUserData->m_nYData== pData->m_nOutcome))
		{
			oldPt[0] = nRT;
			oldPt[1] = pUserData->m_pYaxis[nRT];
			bOld = TRUE;
			break;
		}
	}

	BOOL bFound = FALSE;
	for (i=0;i<m_cUserData.GetSize();i++)
	{
		CPhasePlotData* pUserData = m_cUserData.GetAt(i);
		if (!pUserData) continue;
		if ((pUserData->m_nExpSet == pData->m_nExpSet) && (pUserData->m_nYData== pData->m_nOutcome))
		{
			pUserData->m_pXaxis[0] = pData->m_nTime;
			pUserData->m_pYaxis[0] = pData->m_dValue;
			bFound = TRUE;
			break;
		}
	}
	if (!bFound)
	{
		CSimulDoc *pDoc = GetDocument();
		if (!pDoc) return FALSE;
		CMdEquation*	m_pEqu = pDoc->GetCurrentModel();
		if (!m_pEqu) return FALSE;
		CLearningUnit*	pLU = pDoc->GetCurrentLU();
		if (!pLU) return FALSE;

		int nbOutC = m_pER->m_cOutputSet.GetSize();
		BOOL bDisplay = FALSE;
		for (int i=0;i<nbOutC;i++)
		{
			COutcomes *pCo= m_pER->m_cOutputSet.GetAt(i);
			if (!pCo) continue;
			if (!pCo->m_nSelected) continue;

			if (pCo->m_nData == pData->m_nOutcome &&
				pCo->m_nExpSet == pData->m_nExpSet)
			{
				bDisplay = TRUE;
				break;
			}
		}

		if (!bDisplay) return FALSE;

		int nbPar = m_pEqu->m_cParSet.GetSize();
		CPhasePlotData* pUserData = new CPhasePlotData((bOld) ? 2 : 1);
		pUserData->m_bPred = TRUE;
		pUserData->m_bEditable = TRUE;
		pUserData->m_nExpSet = pData->m_nExpSet;
		pUserData->m_bSelected = pData->m_nTime;
		pUserData->m_nXData = nbPar;
		pUserData->m_nYData = pData->m_nOutcome;

		CMdData *pTime = pLU->GetDataAt(pData->m_nOutcome);
		CMdData *pPred = pLU->GetDataAt(pUserData->m_nYData);
		pUserData->m_clrData = pPred->GetDataInfo()->GetColor();

		CString strExpSet = pLU->m_cExpSet.GetAt(pData->m_nExpSet)->GetName();
		CString m_strYPlot = pPred->GetDataName(pLU->m_bShowAbbrev);
		pUserData->m_strName = pPred->GetDataName(pLU->m_bShowAbbrev) + _T(" (") + strExpSet + _T(")");

		pUserData->m_pXaxis[0] = pData->m_nTime;
		pUserData->m_pYaxis[0] = pData->m_dValue;

		if (bOld)
		{
			pUserData->m_pXaxis[1] = oldPt[0];
			pUserData->m_pYaxis[1] = oldPt[1];
		}

		m_cUserData.Add(pUserData);
	}
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
/// The framework calls this function after receiving a TRANSLATION_MAPRELATION notification.
///
/// Response to data update
/////////////////////////////////////////////////////////////////////////////
BOOL CViewGraph::OnUpdateMapRelation(CTranslationInfo *mTInfo )
{
	if (mTInfo)
	{
		m_nTransData = -1;
		m_nHypData = -1;

		int nTime = -1;//mTInfo->m_nTime;
		int nExpSet = -1;//mTInfo->m_nExpSet;
		int nData = -1;//mTInfo->m_nData;

		int nbD = mTInfo->m_nDatas.GetSize();
		for (int kk=0;kk<m_cPPData.GetSize();kk++)
		{
			CPhasePlotData* pData = m_cPPData.GetAt(kk);
			if (!pData) continue;
				pData->m_bSelected = FALSE;
			m_nCurrTime = mTInfo->m_nTime;
			for (int jj=0;jj<nbD;jj++)
			{
				CTranslationInfo::CTInfo tt1 = mTInfo->m_nDatas.GetAt(jj);
				nTime = tt1.nTime;
				nExpSet = tt1.nExpSet;
				nData = tt1.nData;

				if (((pData->m_nXData ==nData) || (pData->m_nYData ==nData)) &&
					pData->m_nExpSet == nExpSet)
				{
					m_nTransData = kk;
					m_nCurrTime = nTime;
					//break;
					pData->m_bSelected = TRUE;
				}
				//else
				//	pData->m_bSelected = FALSE;
			}
		}
	}
	else
	{
		m_nTransData = -1;
		m_nHypData = -1;

		for (int kk=0;kk<m_cPPData.GetSize();kk++)
		{
			CPhasePlotData* pData = m_cPPData.GetAt(kk);
			if (!pData) continue;
			pData->m_bSelected = FALSE;
		}
	}
	m_bCanEdit = CanEdit();
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
/// The framework calls this function after receiving a DOC_UPDATE_TIMERDATA notification.
///
/// Response to data update
/////////////////////////////////////////////////////////////////////////////
BOOL CViewGraph::OnUpdateTimer()
{
	CSimulDoc *pDoc = GetDocument();
	if (!pDoc) return FALSE;
	CLearningUnit*	pLU = pDoc->GetCurrentLU();
	if (!pLU) return FALSE;

	m_bCanEdit = FALSE;
	m_pSelData = NULL;
	m_pHypData = NULL;
	m_nCurrTime = pDoc->GetCurrTime();
	//int deb = pDoc->m_currTimer;
	//int nbExpSet = pLU->m_cExpSet.GetSize();
	//nbExpSet = 0;
//	m_nTransData = -1;
//	GetDocument()->UpdateMapRelation(NULL,NULL);
	m_bCanEdit = CanEdit();
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
/// The framework calls this function after receiving a DOC_UPDATE_ALLDATA notification.
///
/// Response to data update
/////////////////////////////////////////////////////////////////////////////
BOOL CViewGraph::OnUpdateData()
{
	CSimulDoc *pDoc = GetDocument();
	if (!pDoc) return FALSE;
	CMdEquation*	m_pEqu = pDoc->GetCurrentModel();
	if (!m_pEqu) return FALSE;
	CLearningUnit*	pLU = pDoc->GetCurrentLU();
	if (!pLU) return FALSE;
	if (!m_pER) return FALSE;

//	m_nTransData = -1;
//	m_pSelData = NULL;
//	m_pHypData = NULL;

	//m_bCanEdit = FALSE;
	CFormatGraph *pDlg = GetFormats();
	m_nMaxTime = 0;

	for (int kk=0;kk<m_cPPData.GetSize();kk++)
	{
		CPhasePlotData* pData = m_cPPData.GetAt(kk);
		if (!pData) continue;
		delete pData;
	}
	m_cPPData.RemoveAll();

	int nbOutC = m_pER->m_cOutputSet.GetSize();
	int nbPar = m_pEqu->m_cParSet.GetSize();

	double xMin = 45000000;
	double yMin = 45000000;
	double xMax = -45000000;
	double yMax = -45000000;

	BOOL bTime = FALSE;

	for (int i=0;i<nbOutC;i++)
	{
		COutcomes *pCo= m_pER->m_cOutputSet.GetAt(i);
		if (!pCo) continue;
		if (!pCo->m_nSelected) continue;
		int nbExpSet = pLU->m_cExpSet.GetSize();
		if (pCo->m_nExpSet >= nbExpSet) continue;

		int m_nXData = pCo->m_nData;
		int maxT = pLU->GetMaxTimer();
		
		m_nMaxTime = maxT;

		CPhasePlotData *pData=new CPhasePlotData(maxT+2);
		pData->m_nExpSet = pCo->m_nExpSet;

		pData->m_nXData = nbPar;
		pData->m_nYData = pCo->m_nData;

		m_cPPData.Add(pData);

		int nb = pLU->GetDataSize();
		CString strExpSet = pLU->m_cExpSet.GetAt(pData->m_nExpSet)->GetName();

		CMdData *pTime = pLU->GetDataAt(pData->m_nXData);
		CString m_strXPlot = pTime->GetDataName(pLU->m_bShowAbbrev);
		//m_pGraph->XAxisTitle(mstr);

		CMdData *pPred = pLU->GetDataAt(pData->m_nYData);
		CString m_strYPlot = pPred->GetDataName(pLU->m_bShowAbbrev);
		//m_pGraph->YAxisTitle(mstr);

		CModelObj* pObj = pPred->GetDataInfo();
		if (pObj)
			pData->m_bEditable = (pObj->GetIcon() <=2);
			
		pData->m_clrData = pPred->GetDataInfo()->GetColor();
		pData->m_strName = pPred->GetDataName(pLU->m_bShowAbbrev) + _T(" (") + strExpSet + _T(")");

		if (!bTime)
		{
			double d1 = pTime->GetAt(0,pData->m_nExpSet);
			double d2 = pTime->GetAt(1,pData->m_nExpSet);
			m_nDeltaTime = d2-d1;
			bTime = TRUE;
		}

		for (int j=0;j<=maxT;j++)
		{
			pData->m_pXaxis[j] = pTime->GetAt(j,pData->m_nExpSet);
			pData->m_pYaxis[j] = pPred->GetAt(j,pData->m_nExpSet);
			//if (nbExpSet >=2)
		}

		double nMin,nMax;
		pTime->GetMinMax(&nMin,&nMax,pData->m_nExpSet);
		double nMin1,nMax1;
		pPred->GetMinMax(&nMin1,&nMax1,pData->m_nExpSet);

		xMin = min(xMin,nMin);
		yMin = min(yMin,nMin1);
		xMax = max(xMax,nMax);
		yMax = max(yMax,nMax1);
	}

	int nTicks = 8;

	if (pDlg && pDlg->m_bAutoScale==1)
	{
	    double ddd  = log10(yMax);
		ddd = ceil(ddd);
		yMax = pow(10.0, ddd);
	    double bbb  = log10(yMin);
		bbb = floor(bbb);
		yMin = pow(10.0, bbb);
	}
	else
	{
		double range = yMax-yMin;
		double cc = log10(range);
		int dec = (int)floor(cc);
		double mul = pow(10.0, dec);
		int rrr = (int)(ceil(range / mul) * mul);
		nTicks = (int)(rrr / mul);
		if (nTicks<=2) nTicks*=5;
		if (nTicks<=5) nTicks*=2;
		int nn = (int)(yMin /mul); 
		yMin = floor(yMin / mul) * mul;
		//yMin = yMin * mul;
		yMax = ceil(yMax / mul) * mul;
		nTicks = (int)((yMax-yMin)/mul);
		if (nTicks<=2) nTicks*=5;
		if (nTicks<=5) nTicks*=2;
		if (!nTicks) nTicks=10;
	}
	xRange.x = xMin;
	xRange.y = xMax;
	yRange.x = yMin;
	yRange.y = yMax;
	m_pGraph->SetRange(xMin,yMin, xMax, yMax);
	m_pGraph->SetRatio(0,0 , 1, 1);
	m_pGraph->SetXDecimal(2);
	m_pGraph->SetYDecimal(2);

	CString m_strTitle = m_pEqu->GetSimulationName();

	m_pGraph->SetXNumOfTicks(20);
	m_pGraph->SetXNumOfGridTicks(10);
	m_pGraph->SetYNumOfTicks(nTicks*5);
	m_pGraph->SetYNumOfSep(nTicks);
	m_pGraph->SetYNumOfGridTicks(nTicks);

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
void CViewGraph::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	int nbTrans = GetTranslation();
	BOOL bRet = TRUE;

	if (lHint == DOC_UPDATE_TIMERDATA)
	{
		OnUpdateTimer();
	}
	if (lHint == TRANSLATION_HYPOTHESIS )
	{
		if (nbTrans<TRANS_MAPRELAT) 
		{
			bRet = FALSE;
		}
		else
		{
			CUserData *pData = (CUserData *)pHint;

			if (pData->m_nTLevel<=nbTrans)
				bRet = OnUpdateHypothesis(pData);
		}
	}
	else if (lHint==TRANSLATION_ACTION)
	{
		CUserData *pData = (CUserData *)pHint;
		OnUpdateAction(pData);
		//Invalidate();
	}

	if (lHint == DOC_UPDATE_BREAKPOINT )
	{
		CTimerBreakPoint *mTInfo = (CTimerBreakPoint *)pHint;
		int nBPTime = mTInfo->m_tBreakPt;
		int nBPType = mTInfo->m_nBPType;
		bRet = OnUpdateBreakPoint(nBPTime,0);
	}
	if (lHint == TRANSLATION_MAPRELATION)
	{
		if (nbTrans==TRANS_INDEP) 
		{
			bRet = FALSE;
		}
		else
		{
			CTranslationInfo *mTInfo = (CTranslationInfo *)pHint;
			bRet = OnUpdateMapRelation(mTInfo);
		}
	}
	if (lHint == DOC_UPDATE_RESTARTSIMUL)
	{
		for (int i=0;i<m_cUserData.GetSize();i++)
		{
			CPhasePlotData* pData = m_cUserData.GetAt(i);
			if (!pData) continue;
			delete pData;
		}
		m_cUserData.RemoveAll();
		OnUpdateTimer();
		bRet = OnUpdateData();
		m_nTransData = -1;
		m_nHypData = -1;

		m_pSelData = NULL;
		m_pHypData = NULL;

	}		
	if (!lHint || lHint == DOC_UPDATE_ALLDATA)
	{
		bRet = OnUpdateData();
		m_nTransData = -1;
		m_nHypData = -1;

		m_pSelData = NULL;
		m_pHypData = NULL;
	}

	if (bRet)
	{
		RecalcSize();
		Invalidate();
		UpdateWindow();
	}
}

BOOL CViewGraph::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	// TODO: Add your specialized code here and/or call the base class
	NMHDR *pp = (NMHDR*)lParam;
	if (pp->code == TTN_NEEDTEXT)
	{
		int i=0;
		//return m_xyGraph.OnToolTipNeedText((UINT) wParam, pp,pResult);
	}
	return CScrollView::OnNotify(wParam, lParam, pResult);
}


void CViewGraph::OnSize(UINT nType, int cx, int cy) 
{
	CScrollView::OnSize(nType, cx, cy);
	// TODO: Add your message handler code here
	RecalcSize();
	if (m_wndToolBar && ::IsWindow(m_wndToolBar.m_hWnd))
	{
		UINT tt = ID_TASK_CANCELTASK;
		UINT nID,nStyle;
		int nImage;
		m_wndToolBar.GetButtonInfo(2,nID,nStyle,nImage);
		m_wndToolBar.SetButtonInfo(2,nID,TBBS_SEPARATOR,cx-100);

	}

}

void CViewGraph::OnSelectdata() 
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

		OnUpdate(0,0,0) ;
	}
}

void CViewGraph::OnEditCopy() 
{
	CBitmap         bitmap;
	CClientDC       dc(this);
	CDC             memDC;

	BeginWaitCursor();
	memDC.CreateCompatibleDC(&dc);
	CRect m_rectArea;
	GetClientRect(&m_rectArea);
	bitmap.CreateCompatibleBitmap(&dc, m_rectArea.Width(),m_rectArea.Height() );
	CBitmap* pOldBitmap = memDC.SelectObject(&bitmap);

	OnDraw(&memDC);

	OpenClipboard() ;
	EmptyClipboard() ;
	SetClipboardData (CF_BITMAP, bitmap.GetSafeHandle() ) ;
	CloseClipboard () ;

	bitmap.Detach();

	EndWaitCursor();
	
}


BOOL CViewGraph::OnToolTipNeedText(UINT id, NMHDR * pNMHDR, LRESULT * pResult)
{
	BOOL bHandledNotify = FALSE;

/*	CPoint CursorPos;
	VERIFY(::GetCursorPos(&CursorPos));
	ScreenToClient(&CursorPos);

	CRect ClientRect;
	GetClientRect(ClientRect);

	// Make certain that the cursor is in the client rect, because the
	// mainframe also wants these messages to provide tooltips for the
	// toolbar.
	if (ClientRect.PtInRect(CursorPos))
	{
		TOOLTIPTEXT *pTTT = (TOOLTIPTEXT *)pNMHDR;
		//Data2D* MarkerHit = HitTest(CursorPos);
		::strcpy(pTTT->szText, (LPCTSTR)"fdfd£");
		bHandledNotify = TRUE;
	}*/
	return bHandledNotify;

}

void CViewGraph::OnDestroy() 
{
	CTrace::T_CLOSE(GetParentFrame());
	CScrollView::OnDestroy();
	
	// TODO: Add your message handler code here
	
}

int  CViewGraph::HitTest(CPoint mPt,CPhasePlotData *pData,int nLim/*=-1*/)
{
	int nRet = -1;
	BOOL bFound = FALSE;
	int nCount = (nLim == -1) ? pData->m_nData : nLim;
	if (pData->m_bPred) nCount=0;
	for (int i=nCount;i>=0 && !bFound;i--)
	{
		CTPoint<double> pt1;
		if (pData->m_bPred)
			pt1.x = pData->m_pXaxis[i]*m_nDeltaTime;
		else
			pt1.x = pData->m_pXaxis[i];
		pt1.y = pData->m_pYaxis[i];
		CPoint npt = m_pGraph->GetTPoint(pt1);
		CRect mrect(npt,npt);
		mrect.InflateRect(10,10);
		bFound = mrect.PtInRect(mPt);
		if (bFound) 
			nRet = i;
	}
	return nRet;
}


void CViewGraph::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	BOOL bFound = FALSE;
	int	nTrans = GetTranslation();

	{
		CRect m_PlotRect = m_pGraph->m_PlotRect;
		CRect m_Rect;
		GetClientRect(m_Rect);
		//m_pGraph->GetPixelRect(mrect2);

		int GL = m_PlotRect.left;
		int GR = m_PlotRect.right;
		int GT = m_PlotRect.top;
		int GB = m_PlotRect.bottom;
		int PX = GR - GL;
		int PY = GB - GT;
		
		//m_Scale.dx   = (m_Scale.xmax- m_Scale.xmin) / PX;
		//m_Scale.dy   = (m_Scale.ymax- m_Scale.ymin) / PY;
		CSize m_Size;
		m_Size.cx = (PY < PX) ? PY : PX;

		LOGFONT m_LogFont; 
		m_LogFont.lfHeight = (int)(m_Size.cx / -25.0);
		m_LogFont.lfHeight = -10;
		m_LogFont.lfWeight	   = 500;
		m_LogFont.lfOrientation= 0;
		m_LogFont.lfEscapement = 0;
		CFont m_Font;
		m_Font.CreateFontIndirect(&m_LogFont);


		int n  = (m_Rect.right - GR) / 20 + 1;
		int xb = GR + 2 * n;
		int xe = xb + 4 * n;

		for (int k=0;k<m_cPPData.GetSize();k++)
		{
			int Index=k+1;
			int y  = GT - 3 * Index * m_LogFont.lfHeight / 2;

			CRect rTest(xb,y-6,m_Rect.right,y+6);
			BOOL bFound = rTest.PtInRect(point);

			if (bFound)
			{
				CString mstr;
				mstr.Format(_T("Data : %d"),k);
				//AfxMessageBox(mstr);

				CTranslationInfo mTInfo;
				CPhasePlotData *pData = m_cPPData.GetAt(k);

				mTInfo.m_nTransLvl = TRANS_MAPRELAT;
				mTInfo.m_nTime=m_nCurrTime;
				mTInfo.m_nDatas.RemoveAll();
				CTranslationInfo::CTInfo tt={m_nCurrTime,pData->m_nYData,pData->m_nExpSet};
				mTInfo.m_nDatas.Add(tt);

				OnUpdateMapRelation(&mTInfo);
				//OnUpdate(this,TRANSLATION_MAPRELATION,(CObject*)&mTInfo);

				Invalidate();
				UpdateWindow();
				OnUpdateObjTooltip((WPARAM)pData,TRUE);
				return;
			}
		}
/*		{
			int Index=m_cPPData.GetSize()+2;
			int y  = GT - 3 * Index * m_LogFont.lfHeight / 2;
			CRect rTest(xb,y-6,m_Rect.right,y+6);
			BOOL bFound = rTest.PtInRect(point);
			if (bFound)
			{
				//AfxMessageBox(_T("Time"));
				OnUpdateMapRelation(NULL);
				//OnUpdate(this,TRANSLATION_MAPRELATION,NULL);
				Invalidate();
				UpdateWindow();
				//GetDocument()->UpdateMapRelation(NULL,NULL);
				return;
			}
		}*/

		m_nHypData = -1;
		for (int  i=0;i<m_cUserData.GetSize();i++)
		{
			int Index=m_cPPData.GetSize()+5+i;
			int y  = GT - 3 * Index * m_LogFont.lfHeight / 2;
			CRect rTest(xb,y-6,m_Rect.right,y+6);
			BOOL bFound = rTest.PtInRect(point);
			if (bFound)
			{
				CPhasePlotData *pData = m_cUserData.GetAt(i);
				CString mstr;
				mstr.Format(_T("Hypothesis : %d"),i);
				//AfxMessageBox(mstr);
				//m_pHypData = pData;
				m_nHypData = i;

				Invalidate();
				UpdateWindow();
				OnUpdateObjTooltip((WPARAM)pData,TRUE);
				return;
			}

			CPhasePlotData *pData = m_cUserData.GetAt(i);
			if (pData && GetDocument()->m_nCurrBP==GetDocument()->m_runTimer)
			{
				int nTest = HitTest(point,pData,GetDocument()->m_runTimer);
				if (nTest!=-1)
				{
					int nbTrans = GetTranslation();
					if (nbTrans<TRANS_MAPRELAT) 
					{
					}
					else
					{
						CUserData pUData;
						
						pUData.m_nTLevel = TRANS_MAPRELAT;
						pUData.m_nExpSet = pData->m_nExpSet; 
						pUData.m_nOutcome = pData->m_nYData;
						pUData.m_nTime = (int)pData->m_pXaxis[0];//pgvItem->row-1;
						pUData.m_dOldValue = pData->m_pXaxis[0];
						pUData.m_dValue = pData->m_pYaxis[0];

						GetDocument()->UpdateUserData(this,&pUData);
						//GetDocument()->UpdateERUserData(m_pER,&pUData);
					}
					bFound = TRUE;
					return;
				}
			}

		}

		
		//DrawLine(xb, y, xe, y);
		//PrintString(xe + n, y, 0, Name);




	}

	for (int k=0;k<m_cPPData.GetSize();k++)
	{
		CPhasePlotData *pData = m_cPPData.GetAt(k);
		if (!pData) continue;

		//int nTest = pData->HitTest(m_pGraph,point,pt1,nDelta,GetDocument()->m_runTimer);
		int nTest = HitTest(point,pData,GetDocument()->m_runTimer);
		if (nTest != -1)
		{
			CTranslationInfo mTInfo;

			mTInfo.m_nTransLvl = TRANS_MAPRELAT;
			mTInfo.m_nTime=nTest;
			mTInfo.m_nDatas.RemoveAll();
			CTranslationInfo::CTInfo tt={nTest,pData->m_nYData,pData->m_nExpSet};
			mTInfo.m_nDatas.Add(tt);

			//GetDocument()->m_currTimer = nTest;
			if (nTrans==TRANS_INDEP)
			{
				OnUpdateMapRelation(&mTInfo);
				//OnUpdate(this,TRANSLATION_MAPRELATION,(CObject*)&mTInfo);
				Invalidate();
				UpdateWindow();
			}
			else
				GetDocument()->UpdateMapRelation(this,&mTInfo);
			bFound = TRUE;
			break;
		}
	}
	if (!bFound)
	{
		if (nTrans==TRANS_INDEP)
		{
			//OnUpdate(this,TRANSLATION_MAPRELATION,NULL);
			OnUpdateMapRelation(NULL);
			Invalidate();
			UpdateWindow();
		}
		else
			GetDocument()->UpdateMapRelation(NULL,NULL);
	}
	CScrollView::OnLButtonDblClk(nFlags, point);
}

void CViewGraph::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	//m_nEditData = -1;
	m_pSelData = NULL;
	m_pHypData = NULL;

	CSimulDoc *pDoc = GetDocument();
	if (!pDoc) return;

	int nCBP = pDoc->m_nCurrBP;
	int nRT = pDoc->m_runTimer;
	CTimerBreakPoint pBP;
	BOOL bEditable = pDoc->GetCurrentLU()->GetTimerBPAt(nCBP,pBP);
	if (!bEditable) return;

//	CTPoint<double> pt1 = m_pGraph->GetTValue(point);
//	CTPoint<double> nDelta;
	if (m_nTransData != -1 && m_bCanEdit && pBP.m_nBPType==BREAKP_ACTION)
	{
		//nDelta.x = (xRange.y - xRange.x) / 40;
		//nDelta.y = (yRange.y - yRange.x) / 40;
	
		CPhasePlotData *pData = m_cPPData.GetAt(m_nTransData);
		if (pData && pData->m_bEditable)
		{
			//int nTest = pData->HitTest(m_pGraph,point,pt1,nDelta,GetDocument()->m_runTimer);
			int nTest = HitTest(point,pData,GetDocument()->m_runTimer);
			if (nTest == GetDocument()->m_runTimer)
			{
				//m_nEditData = m_nTransData;
				m_pSelData = pData;
			}
		}
	}
	else if (nCBP==nRT && pBP.m_nBPType==BREAKP_HYPOT && !pDoc->m_bTaskDone)
	{
		BOOL bFound = FALSE;
		int nb = m_cUserData.GetSize();

		if (m_nHypData != -1 && m_nHypData < nb)
		{
			CPhasePlotData *pData = m_cUserData.GetAt(m_nHypData);
			if (pData)
			{
				int nTest = HitTest(point,pData,GetDocument()->m_runTimer);
				if (nTest!=-1)
				{
					m_pHypData = pData;
					bFound = TRUE;
				}
			}
		}

		if (!bFound) for (int i=0;i<nb;i++)
		{
			CPhasePlotData *pData = m_cUserData.GetAt(i);
			if (!pData) continue;

			int nTest = HitTest(point,pData,GetDocument()->m_runTimer);
			if (nTest!=-1)
			{

				m_pHypData = pData;
				m_nHypData = i;
				bFound = TRUE;
				break;
			}
		}
		if (!bFound && m_nTransData != -1)
		{
			CPhasePlotData *pData = m_cPPData.GetAt(m_nTransData);

			int nTest = HitTest(point,pData,GetDocument()->m_runTimer);
			if (nTest != GetDocument()->m_runTimer)  return;

			int nb = m_cUserData.GetSize();

			for (int i=0;i<nb;i++)
			{
				CPhasePlotData *pUData = m_cUserData.GetAt(i);
				if (!pUData) continue;
		
				if (pUData->m_nExpSet == pData->m_nExpSet &&
					pUData->m_nYData == pData->m_nYData)
				{
					m_pHypData = pUData;
					m_nHypData = i;
					return;
				}
			}

			CUserOutput pT2;
			BOOL bOutput = GetDocument()->m_cUserInput.Lookup(nCBP,pT2);
			//BOOL bOutput = m_pER->m_cUserInput.Lookup(nCBP,pT2);
			if (!bOutput) return;

			CLearningUnit*	pLU = GetDocument()->GetCurrentLU();
			BOOL bEditable = pLU->GetTimerBPAt(pT2.m_nTime,pBP);
			if (!bEditable) return;

			CUserData pUData;
			pUData.m_nTLevel = GetTranslation();
			pUData.m_nExpSet = pData->m_nExpSet; 
			pUData.m_nOutcome = pData->m_nYData;
			pUData.m_nTime = pT2.m_nTime;
			pUData.m_dOldValue = pData->m_pYaxis[nCBP];
			pUData.m_dValue = pData->m_pYaxis[nCBP];

			if (OnUpdateHypothesis(&pUData))
			{
				nb = m_cUserData.GetSize();
				m_pHypData = m_cUserData.GetAt(nb-1);
				m_nHypData = nb-1;
			}
		}
	}
	
	CScrollView::OnLButtonDown(nFlags, point);
}

void CViewGraph::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CScrollView::OnMouseMove(nFlags, point);
	int	nTrans = GetTranslation();

	int nCBP = GetDocument()->m_nCurrBP;
	int nRT = GetDocument()->m_runTimer;
	CTimerBreakPoint pBP;
	BOOL bEditable = GetDocument()->GetCurrentLU()->GetTimerBPAt(nCBP,pBP);
	//if (!bEditable) 
	{
		CRect m_PlotRect = m_pGraph->m_PlotRect;
		CRect m_Rect;
		GetClientRect(m_Rect);
		//m_pGraph->GetPixelRect(mrect2);

		int GL = m_PlotRect.left;
		int GR = m_PlotRect.right;
		int GT = m_PlotRect.top;
		int GB = m_PlotRect.bottom;
		int PX = GR - GL;
		int PY = GB - GT;
		
		//m_Scale.dx   = (m_Scale.xmax- m_Scale.xmin) / PX;
		//m_Scale.dy   = (m_Scale.ymax- m_Scale.ymin) / PY;
		CSize m_Size;
		m_Size.cx = (PY < PX) ? PY : PX;

		LOGFONT m_LogFont; 
		m_LogFont.lfHeight = (int)(m_Size.cx / -25.0);
		m_LogFont.lfHeight = -10;
		m_LogFont.lfWeight	   = 500;
		m_LogFont.lfOrientation= 0;
		m_LogFont.lfEscapement = 0;
		CFont m_Font;
		m_Font.CreateFontIndirect(&m_LogFont);


		int n  = (m_Rect.right - GR) / 20 + 1;
		int xb = GR + 2 * n;
		int xe = xb + 4 * n;

		for (int k=0;k<m_cPPData.GetSize();k++)
		{
			int Index=k+1;
			int y  = GT - 3 * Index * m_LogFont.lfHeight / 2;

			CRect rTest(xb,y-6,m_Rect.right,y+6);
			BOOL bFound = rTest.PtInRect(point);

			CPhasePlotData *pData = m_cPPData.GetAt(k);
			if (bFound)
			{
				OnUpdateObjTooltip((WPARAM)pData,TRUE);
				return;
			}
//			int nTest = HitTest(point,pData,GetDocument()->m_runTimer);
//			if (nTest!=-1)
//			//if (nTest==GetDocument()->GetCurrTime())
//			{
//				OnUpdateObjTooltip((WPARAM)pData,TRUE);
//				return;
//			}


		}
		{
			int Index=m_cPPData.GetSize()+2;
			int y  = GT - 3 * Index * m_LogFont.lfHeight / 2;
			CRect rTest(xb,y-6,m_Rect.right,y+6);
			BOOL bFound = rTest.PtInRect(point);
			if (bFound)
			{
			}
		}
		for (int  i=0;i<m_cUserData.GetSize();i++)
		{
			int Index=m_cPPData.GetSize()+5+i;
			int y  = GT - 3 * Index * m_LogFont.lfHeight / 2;
			CRect rTest(xb,y-6,m_Rect.right,y+6);
			BOOL bFound = rTest.PtInRect(point);
			if (bFound)
			{
				CPhasePlotData *pData = m_cUserData.GetAt(i);
				CString mstr;
				OnUpdateObjTooltip((WPARAM)pData,TRUE);
				//mstr.Format(_T("Hypothesis : %d"),i);
				//AfxMessageBox(mstr);
				return;
			}
		}
		OnUpdateObjTooltip(NULL,NULL);
	}		
	if (!bEditable) 
		return;

	//if (m_nEditData == -1) return;
	//CPhasePlotData *pData = m_cPPData.GetAt(m_nEditData);
	BOOL bUpdate = FALSE;
	if (m_pSelData && m_nTransData !=-1)
	{
		int nnnn = 	m_nTransData;

		CPhasePlotData *pData = m_cPPData.GetAt(m_nTransData);

		//CPhasePlotData *pData = m_pSelData;

		double x = pData->m_pXaxis[GetDocument()->m_runTimer];
		double y = pData->m_pYaxis[GetDocument()->m_runTimer];

		CTPoint<double> pt1 = m_pGraph->GetTValue(point);
		if (pt1.y < 0.00005) return;
		y = pt1.y;

		CLearningUnit* pLU = GetDocument()->GetCurrentLU();
		CMdData *pObjData = pLU->GetDataAt(pData->m_nYData);
		if (!pObjData) return;

		CModelObj *pObj = pObjData->GetDataInfo();
		CModelPar *pVarObj = DYNAMIC_DOWNCAST( CModelPar, pObj);

		double dNewVal = y;

		if (pVarObj)
		{
			double dmin = pVarObj->GetMin();
			double dmax = pVarObj->GetMax();

			y = min(max(dNewVal,dmin),dmax);
		/*	if (dNewVal<dmin|| dNewVal>dmax)
			{
				int precision = 15;
				UINT nIDPrompt = AFX_IDP_PARSE_REAL_RANGE;
				TCHAR szMin[32], szMax[32];
				CString prompt;

				_stprintf(szMin, _T("%.*g"), precision, dmin);
				_stprintf(szMax, _T("%.*g"), precision, dmax);
				AfxFormatString2(prompt, nIDPrompt, szMin, szMax);

				AfxMessageBox(prompt, MB_ICONEXCLAMATION, nIDPrompt);
				m_pSelData = NULL;
				m_pHypData = NULL;
				return;
			}*/
		}


		//pData->m_pXaxis[GetDocument()->m_runTimer];
		pData->m_pYaxis[GetDocument()->m_runTimer] = y;

		double ddx = min(yRange.x,y);
		double ddy = max(yRange.y,y);

		//xRange.x = xMin;
		//xRange.y = xMax;
		yRange.x = ddx;
		yRange.y = ddy;
		//m_pGraph->SetRange(xMin,yMin, xMax, yMax);
		m_pGraph->SetRange(xRange.x,ddx, xRange.y, ddy);

		//CLearningUnit *pLU = GetDocument()->GetCurrentLU();
		CMdEquation*	pEqu = GetDocument()->GetCurrentModel();

		CMdData *PreyInit= pLU->m_cDataPoints.GetAt(pData->m_nYData);
		CString mstr=PreyInit->GetDataName(pLU->m_bShowAbbrev);
		double ddt = PreyInit->GetAt(GetDocument()->m_runTimer,pData->m_nExpSet);
		double ddt2 = y;//pData->m_pYaxis[GetDocument()->m_runTimer];

		CUserData pUData;
		pUData.m_nTLevel = GetTranslation();
		pUData.m_nExpSet = pData->m_nExpSet; 
		pUData.m_nOutcome = pData->m_nYData;
		pUData.m_nTime = GetDocument()->m_runTimer;//pgvItem->row-1;
		pUData.m_dOldValue = ddt;
		pUData.m_dValue = ddt2;
		pUData.m_bUpdateNow = FALSE;

		GetDocument()->UpdateUserData(NULL,&pUData);
		//GetDocument()->UpdateERUserData(m_pER,&pUData);

//		m_nTransData = -1;
//		m_pSelData = pData;
//		m_nTransData = nnnn ;
		bUpdate = TRUE;
	}
	else if (m_pHypData && nCBP==nRT && pBP.m_nBPType==BREAKP_HYPOT && !GetDocument()->m_bTaskDone)
	{
		double x = m_pHypData->m_pXaxis[0];
		double y = m_pHypData->m_pYaxis[0];

		CTPoint<double> pt1 = m_pGraph->GetTValue(point);
		y = pt1.y;

		if (y>yRange.y)	y=yRange.y;
		if (y<yRange.x)	y=yRange.x;

		m_pHypData->m_pYaxis[0] = y;

		CUserData pUData;
		pUData.m_nTLevel = GetTranslation();
		pUData.m_nExpSet = m_pHypData->m_nExpSet; 
		pUData.m_nOutcome = m_pHypData->m_nYData;
		pUData.m_nTime = (int)m_pHypData->m_pXaxis[0];//pgvItem->row-1;
		pUData.m_dOldValue = m_pHypData->m_pXaxis[0];
		pUData.m_dValue = m_pHypData->m_pYaxis[0];

		if (nTrans<=TRANS_MAPRELAT)
		{
			OnUpdateHypothesis(&pUData);
		}
		else
			GetDocument()->UpdateUserData(NULL,&pUData);
			//GetDocument()->UpdateERUserData(m_pER,&pUData);


		bUpdate = TRUE;
	}

	if (bUpdate)
	{
		//CPhasePlotData *pData = m_pSelData;

		RecalcSize();
		Invalidate();
		UpdateWindow();
	}
}


void CViewGraph::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	//if (m_nEditData != -1)
	int	nTrans = GetTranslation();
	BOOL bUpdate = FALSE;
	if (m_pSelData && m_nTransData != -1)
	{
		//CPhasePlotData *pData = m_cPPData.GetAt(m_nEditData);
		//CPhasePlotData *pData = m_pSelData;
		CPhasePlotData *pData = m_cPPData.GetAt(m_nTransData);
		if (pData)
		{
			CLearningUnit *pLU = GetDocument()->GetCurrentLU();
			CMdEquation*	pEqu = GetDocument()->GetCurrentModel();

/*
			CMdData *PreyInit= pLU->m_cDataPoints.GetAt(pData->m_nYData);
			CString mstr=PreyInit->GetDataName();
			double ddt = PreyInit->GetAt(GetDocument()->m_runTimer,pData->m_nExpSet);
			PreyInit->SetAt(pData->m_pYaxis[GetDocument()->m_runTimer],GetDocument()->m_runTimer,pData->m_nExpSet);
			double ddt2 = PreyInit->GetAt(GetDocument()->m_runTimer,pData->m_nExpSet);

			//int oldD = m_nEditData;
			int oldT = m_nTransData;

			pEqu->OnUpdateFrom(pLU,pData->m_nExpSet,GetDocument()->m_runTimer);
			GetDocument()->UpdateAllViews(this,DOC_UPDATE_ALLDATA);
			OnUpdate(NULL,DOC_UPDATE_ALLDATA,NULL);

			m_bCanEdit = TRUE;
			//m_nEditData = oldD;
			//m_nTransData = oldT;
			m_nTransData = -1;
*/
			CMdData *PreyInit= pLU->m_cDataPoints.GetAt(pData->m_nYData);
			CString mstr=PreyInit->GetDataName(pLU->m_bShowAbbrev);
			double ddt = PreyInit->GetAt(GetDocument()->m_runTimer,pData->m_nExpSet);
			double ddt2 = pData->m_pYaxis[GetDocument()->m_runTimer];

	//		CUserData pData;
	//		pData.m_nExpSet = pCo->m_nExpSet; 
	//		pData.m_nOutcome = pCo->m_nData;
	//		pData.m_nTime = pTOut;//pgvItem->row-1;
	//		pData.m_dValue = atof(df.GetBuffer(df.GetLength()));
	//		pData.m_dOldValue = atof(m_strModifiedCell.GetBuffer(m_strModifiedCell.GetLength()));


			CUserData pUData;
			pUData.m_nTLevel = GetTranslation();
			pUData.m_nExpSet = pData->m_nExpSet; 
			pUData.m_nOutcome = pData->m_nYData;
			pUData.m_nTime = GetDocument()->m_runTimer;//pgvItem->row-1;
			pUData.m_dOldValue = ddt;
			pUData.m_dValue = ddt2;

			GetDocument()->UpdateUserData(this,&pUData);
			//GetDocument()->UpdateERUserData(m_pER,&pUData);


/*			CTimerBreakPoint pBP;
			BOOL bEditable = pLU->GetTimerBPAt(pUData.m_nTime,pBP);
			if (!bEditable) return;

			pUData.m_nType = pBP.m_nBPType;
	
			CUserOutput pUserData;
			BOOL bGet = GetDocument()->m_cUserInput.Lookup(GetDocument()->m_runTimer,pUserData);
			if (!bGet)
			{
				int nbD = pUserData.m_lUserInput.GetCount();
				pUserData.SetAt(pUData.m_nExpSet,pUData.m_nOutcome,pUData);
				nbD = pUserData.m_lUserInput.GetCount();
				pUserData.m_nTime = GetDocument()->m_runTimer;
				GetDocument()->m_cUserInput.SetAt(GetDocument()->m_runTimer,pUserData);
			}
			else
			{
				CUserData pData2;
				BOOL bRes = pUserData.GetAt(pUData.m_nExpSet,pUData.m_nOutcome,pData2);
				if (bRes)
				{
					pUData.m_dOldValue = pData2.m_dOldValue;
				}
				pUserData.SetAt(pUData.m_nExpSet,pUData.m_nOutcome,pUData);
				GetDocument()->m_cUserInput.SetAt(GetDocument()->m_runTimer,pUserData);
			}

			if (pBP.m_nBPType == BREAKP_ACTION)
			{
				PreyInit->SetAt(pData->m_pYaxis[GetDocument()->m_runTimer],GetDocument()->m_runTimer,pData->m_nExpSet);
				double ddt2 = PreyInit->GetAt(GetDocument()->m_runTimer,pData->m_nExpSet);
	
				pEqu->OnUpdateFrom(pLU,pData->m_nExpSet,GetDocument()->m_runTimer);
				GetDocument()->UpdateAllViews(this,DOC_UPDATE_ALLDATA);
				OnUpdate(NULL,DOC_UPDATE_ALLDATA,NULL);

				m_bCanEdit = TRUE;
				m_nTransData = -1;
			}
			else
			{
				//FormatCells(pData.m_nTime,TRUE);
				//OnUpdate(this,TRANSLATION_HYPOTHESIS,&pData);
				//GetDocument()->UpdateAllViews(this,TRANSLATION_HYPOTHESIS,&pData);
			}*/
		}
	}
	else if (m_pHypData)
	{
		int nb = 0;

		CUserData pUData;
		pUData.m_nTLevel = GetTranslation();
		pUData.m_nExpSet = m_pHypData->m_nExpSet; 
		pUData.m_nOutcome = m_pHypData->m_nYData;
		pUData.m_nTime = (int)m_pHypData->m_pXaxis[0];//pgvItem->row-1;
		pUData.m_dOldValue = m_pHypData->m_pXaxis[0];
		pUData.m_dValue = m_pHypData->m_pYaxis[0];

		if (nTrans<=TRANS_MAPRELAT)
		{
			OnUpdateHypothesis(&pUData);
		}
		else
			GetDocument()->UpdateUserData(this,&pUData);
			//GetDocument()->UpdateERUserData(m_pER,&pUData);

		bUpdate = TRUE;
	}
	//m_nEditData = -1;
	m_pSelData = NULL;
	m_pHypData = NULL;

	if (bUpdate)
	{
		//OnUpdateMapRelation(NULL);
		RecalcSize();
		Invalidate();
		UpdateWindow();
	}

	CScrollView::OnLButtonUp(nFlags, point);
}

void CViewGraph::OnUpdateTransTasks(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	BOOL	bEnab = FALSE;
	BOOL	bCheck = FALSE;
	UINT	nT = GetTranslation();
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

BOOL CViewGraph::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message== WM_LBUTTONDOWN ||
        pMsg->message== WM_LBUTTONUP ||
        pMsg->message== WM_MOUSEMOVE)
		m_wndToolTip.RelayEvent(pMsg);

	return CScrollView::PreTranslateMessage(pMsg);
}
