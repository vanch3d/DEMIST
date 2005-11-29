// SimulXYGraph.cpp : implementation file
//

#include "stdafx.h"
#include "simul.h"

#include "LearnerTrace.h"

#include "SimulDoc.h"
#include "SimulXYGraph.h"

#include <MInstruction\LearningUnit.h>
#include <MSimulation\Model.h>


#include "SelectParamDlg.h"
#include <MInteraction\Format.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	DEMO_MAX_GRAPHS		16

COLORREF DefaultGraphColors[DEMO_MAX_GRAPHS] = {RGB(128, 0, 255), RGB(64, 128, 128), RGB(0, 0, 255), 
	RGB(255, 255, 0), RGB(0, 255, 255), 32768, 128, 
	8388736, 8421440, 12615808, 8421504, 33023, 
	16711935, 12632256, 32896, RGB(0, 0, 0)};

/////////////////////////////////////////////////////////////////////////////
// CViewPhasePlot

IMPLEMENT_DYNCREATE(CViewPhasePlot, CScrollView)

CViewPhasePlot::CViewPhasePlot()
{
	m_nTransData = -1;
//	m_nCurrTTask=0;
	m_pER = NULL;
//	m_GraphCount = 0;

	m_pSelData = NULL;
	m_pHypData = NULL;
	m_pHypData2 = m_pHypData3 = NULL;

	m_bCanEdit = FALSE;

	m_nMaxTime = 0;
	m_nCurrTime = 0;

/*	x[0]  = 5;
	y[0]  = 70;
	x[1]  = 30;
	y[1]  = 30;
	x[2]  = 40;
	y[2]  = 4;
	x[3]  = 70;
	y[3]  = 50;
	x[4]  = 80;
	y[4]  = 80;*/

	// init
//	m_fXScale   = 1.0f; // zooming scale
//	m_fYScale   = 1.0f;
//	m_fDelta	= 1.2f;
	m_nMAPMode	= MM_TEXT;

	m_nScale	= 0;
	m_bCanDraw	= TRUE;
	m_bMemDraw	= TRUE;
	m_bTitle	= TRUE;
	m_bLegend	= TRUE;
	m_bBoundary	= FALSE;
	m_bGrid		= TRUE;

	m_bStabPt = TRUE;
	m_bCurrT  = TRUE;

	m_nXData = m_nYData = -1;

	
	SetScaleType();
	//m_pGraph	= &m_XLinearYLog;
	m_nBkColor  = m_pGraph->GetBackColor();
	m_pGraph->SetRange(0, 0, 100, 100);
	m_pGraph->EnableMemoryDraw(m_bMemDraw);
	m_pGraph->EnableLegend(m_bLegend);
	m_pGraph->SetBackColor(m_nBkColor);

	m_strTitle	= _T("Phase Plot");
}

CViewPhasePlot::~CViewPhasePlot()
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

	for (i=0;i<m_cIsocline.GetSize();i++)
	{
		CPhasePlotData* pData = m_cIsocline.GetAt(i);
		if (!pData) continue;
		delete pData;
	}
	m_cIsocline.RemoveAll();
}

CFormatPhasePlot* CViewPhasePlot::GetFormats()
{
	CFormatPhasePlot* pDlg = NULL;

	if (!m_pER) return pDlg;
	int nbF = m_pER->m_cFormatSet.GetSize();
	if (nbF)
	{
		CFormat *pPage = m_pER->m_cFormatSet.GetAt(0);
		pDlg = DYNAMIC_DOWNCAST(CFormatPhasePlot,pPage);
	}

	return pDlg;
}

int	CViewPhasePlot::GetTranslation()
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


/////////////////////////////////////////////////////////////////////////////
/// The framework calls this function after receiving a DOC_UPDATE_VIEWCONTENT notification.
///
/// Called just before the view is displayed for the first time, this function just
/// assignes to the data member m_pER a pointer to the Learning Unit (see CLearningUnit)
/// associated with that view.
/////////////////////////////////////////////////////////////////////////////
LRESULT CViewPhasePlot::OnUpdateInitContent(WPARAM wp, LPARAM lp)
{
	m_pER = (CExternRepUnit*)lp;
	return 0;
}

LRESULT CViewPhasePlot::OnActivateER(WPARAM wp, LPARAM lp)
{
	static BOOL bFirst = TRUE;

	if (!bFirst) CTrace::T_SWITCH(this, (CWnd*)wp);
	bFirst = FALSE;
	return 0;
}



BEGIN_MESSAGE_MAP(CViewPhasePlot, CScrollView)
	//{{AFX_MSG_MAP(CViewPhasePlot)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
	ON_COMMAND(ID_SELECTDATA, OnSelectData)
	ON_WM_DESTROY()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	//}}AFX_MSG_MAP
	ON_MESSAGE(DOC_UPDATE_VIEWCONTENT, OnUpdateInitContent)
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	//ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
	//ON_COMMAND_RANGE(ID_TRANS_INDEPENDENT,ID_TRANS_DYNALINKED,OnTranslationTasks)
	ON_UPDATE_COMMAND_UI_RANGE(ID_TRANS_INDEPENDENT,ID_TRANS_DYNALINKED,OnUpdateTransTasks)
	ON_COMMAND_RANGE(ID_XYGRAPH_SCALETYPE_LINEAR,ID_XYGRAPH_SCALETYPE_XYLOG,OnSetScaleType)
	ON_UPDATE_COMMAND_UI_RANGE(ID_XYGRAPH_SCALETYPE_LINEAR,ID_XYGRAPH_SCALETYPE_XYLOG,OnUpdateScaleType)
	ON_MESSAGE(TRACE_VIEW_ACTIVATE, OnActivateER)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewPhasePlot diagnostics

#ifdef _DEBUG
void CViewPhasePlot::AssertValid() const
{
	CScrollView::AssertValid();
}

void CViewPhasePlot::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CSimulDoc* CViewPhasePlot::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSimulDoc)));
	return (CSimulDoc*)m_pDocument;
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CViewGraph printing

BOOL CViewPhasePlot::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CViewPhasePlot::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CViewPhasePlot::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CViewPhasePlot::OnPrint(CDC* pDC, CPrintInfo* pInfo) 
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
// CViewPhasePlot drawing

BOOL CViewPhasePlot::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	
	return FALSE;
}

void CViewPhasePlot::OnDraw(CDC* pDC)
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
	DrawFrame(rect, RGB(255, 0, 0),	m_strTitle);

	if (m_bStabPt)
	{
		CLearningUnit*	pLU = GetDocument()->GetCurrentLU();
		int n1=-1;
		int n2=-1;
		int n3=-1;
		int n4=-1;

		CFormatPhasePlot *pDlg = GetFormats();
		if (pDlg)
		{
			n1 = pDlg->m_nIsoX0;
			n2 =pDlg->m_nIsoY0;
			n3 = pDlg->m_nIsoXN;
			n4 = pDlg->m_nIsoYN;
		}
		if (n1!=-1 || n2 != -1)
		{
			for (int k=0;k<m_cPPData.GetSize();k++)
			{
				CPhasePlotData *pData = m_cPPData.GetAt(k);
				if (!pData) continue;


				CMdData *pp1 = NULL;
				CMdData *pp2 = NULL;
				CTPoint<double> pt1[2];
				if (n1!=-1 && n2 != -1)
				{
					pp1 = pLU->GetDataAt(n1);
					pp2 = pLU->GetDataAt(n2);
					pt1[0] = CTPoint<double>(0, pp1->GetAt(0,pData->m_nExpSet));
					pt1[1] = CTPoint<double>(pp2->GetAt(0,pData->m_nExpSet),0);
				}
				else if (n1!=-1)
				{
					pp1 = pLU->GetDataAt(n1);
					//pp2 = pLU->GetDataAt(n4);
					pt1[0] = CTPoint<double>(pp1->GetAt(0,pData->m_nExpSet),yRange.x);
					pt1[1] = CTPoint<double>(pp1->GetAt(0,pData->m_nExpSet),yRange.y);
				}
				else
				{
					pp2 = pLU->GetDataAt(n2);
					pt1[0] = CTPoint<double>(xRange.x,pp2->GetAt(0,pData->m_nExpSet));
					pt1[1] = CTPoint<double>(xRange.y,pp2->GetAt(0,pData->m_nExpSet));
				}

/*				CMdData *pp1 = pLU->GetDataAt(n1);
				CMdData *pp2 = pLU->GetDataAt(n2);
				CTPoint<double> pt1[2];
				pt1[0] = CTPoint<double>(0, pp1->GetAt(0,pData->m_nExpSet));
				pt1[1] = CTPoint<double>(pp2->GetAt(0,pData->m_nExpSet),0);

				COLORREF cr1 = pp1->GetDataInfo()->GetColor();*/
				COLORREF cr1 = (pp1) ? pp1->GetDataInfo()->GetColor() : 
										pp2->GetDataInfo()->GetColor();
				CString mstr1;
				mstr1.Format(_T("%s isocline"),m_strXPlot);
				m_pGraph->Lines(pt1, 2, cr1,m_cPPData.GetSize()+2+2,mstr1);
			}
		}
		if (n3!=-1 || n4 != -1)
		{
			for (int k=0;k<m_cPPData.GetSize();k++)
			{
				CPhasePlotData *pData = m_cPPData.GetAt(k);
				if (!pData) continue;


				CMdData *pp1 = NULL;
				CMdData *pp2 = NULL;
				CTPoint<double> pt1[2];
				if (n3!=-1 && n4 != -1)
				{
					pp1 = pLU->GetDataAt(n3);
					pp2 = pLU->GetDataAt(n4);
					pt1[0] = CTPoint<double>(0, pp1->GetAt(0,pData->m_nExpSet));
					pt1[1] = CTPoint<double>(pp2->GetAt(0,pData->m_nExpSet),0);
				}
				else if (n3!=-1)
				{
					pp1 = pLU->GetDataAt(n3);
					//pp2 = pLU->GetDataAt(n4);
					pt1[0] = CTPoint<double>(pp1->GetAt(0,pData->m_nExpSet),yRange.x);
					pt1[1] = CTPoint<double>(pp1->GetAt(0,pData->m_nExpSet),yRange.y);
				}
				else
				{
					pp2 = pLU->GetDataAt(n4);
					pt1[0] = CTPoint<double>(0,pp2->GetAt(0,pData->m_nExpSet));
					pt1[1] = CTPoint<double>(50,pp2->GetAt(0,pData->m_nExpSet));
				}

				COLORREF cr1 = (pp1) ? pp1->GetDataInfo()->GetColor() : 
										pp2->GetDataInfo()->GetColor();
				CString mstr1;
				mstr1.Format(_T("%s isocline"),m_strYPlot);
				m_pGraph->Lines(pt1, 2, cr1,m_cPPData.GetSize()+2+3,mstr1);
			}
		}
	}
	
//	int nCT = GetDocument()->m_currTimer;
	int nCT = m_nCurrTime;
	int nRT = GetDocument()->GetRunTime();
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

			m_pGraph->Lines(pData->m_pXaxis, pData->m_pYaxis, nItem, DefaultGraphColors[k], k+1, pData->m_strName);
		}

	}
	if (m_bCurrT)
	{
		CLearningUnit*	pLU = GetDocument()->GetCurrentLU();
		for (int k=0;k<m_cPPData.GetSize();k++)
		{
			CPhasePlotData *pData = m_cPPData.GetAt(k);
			if (!pData) continue;

			CMdData *pPrey = pLU->GetDataAt(pData->m_nXData);
			CMdData *pPred = pLU->GetDataAt(pData->m_nYData);

			CTPoint<double> ptTime[1];
			ptTime[0].x = pData->m_pXaxis[nCT];
			ptTime[0].y = pData->m_pYaxis[nCT];
			CString mstr;
			mstr.Format(_T("Time : %d"),nCT);

			COLORREF mclr = RGB(192, 192,192);
			if (m_nTransData == -1)
				mclr = RGB(192, 192,192);
			else if (pData->m_bSelected==1)
				mclr = pPrey->GetDataInfo()->GetColor();
			else if (pData->m_bSelected==2)
				mclr = pPred->GetDataInfo()->GetColor();
			else if (pData->m_bSelected==3)
				mclr = DefaultGraphColors[k];
			else continue;

			int nMode = m_pGraph->FCIRCLE;
			int nSize = 4;

			if (m_bCanEdit && pData->m_bEditable)
			//if (m_pSelData == pData && ptTime[0].x == GetDocument()->GetRunTime())
			{
				nMode = m_pGraph->FDIAMOND;
				nSize = 5;
			}

			m_pGraph->Markers(ptTime, 1, mclr,nMode , m_cPPData.GetSize()+2, mstr,nSize);
			if (pData->m_bSelected)
				m_pGraph->Markers(ptTime, 1, mclr,nMode , k+1, pData->m_strName,nSize);

/*			if (m_nTransData == -1)
			{
				CTPoint<double> ptTime[1];
				ptTime[0].x = pData->m_pXaxis[nCT];
				ptTime[0].y = pData->m_pYaxis[nCT];
				CString mstr;
				mstr.Format(_T("Time : %d"),nCT);
				COLORREF mclr = RGB(192, 192,192);
				//if (m_nTransData ==k)
				//	mclr = DefaultGraphColors[k];
				m_pGraph->Markers(ptTime, 1, mclr, m_pGraph->FCIRCLE, m_cPPData.GetSize()+2, mstr,4);
				//m_pGraph->Markers(ptTime, 1, DefaultGraphColors[k], m_pGraph->FCIRCLE, k+1, pData->m_strName,4);
			}
			else if (pData->m_bSelected)
			{
				CTPoint<double> ptTime[1];
				ptTime[0].x = pData->m_pXaxis[nCT];
				ptTime[0].y = pData->m_pYaxis[nCT];
				CString mstr;
				mstr.Format(_T("Time : %d"),nCT);

				CMdData *pPrey = pLU->GetDataAt(pData->m_nXData);
				CMdData *pPred = pLU->GetDataAt(pData->m_nYData);

				COLORREF mclr = pData->m_clrData;
				if (pData->m_bSelected==1)
					mclr = pPrey->GetDataInfo()->GetColor();
				else if (pData->m_bSelected==2)
					mclr = pPred->GetDataInfo()->GetColor();
				else mclr = DefaultGraphColors[k];
				//if (m_nTransData ==k)
				//	mclr = DefaultGraphColors[k];
				m_pGraph->Markers(ptTime, 1, mclr, m_pGraph->FCIRCLE, m_cPPData.GetSize()+2, mstr,4);
				m_pGraph->Markers(ptTime, 1, mclr, m_pGraph->FCIRCLE, k+1, pData->m_strName,4);
			}*/
		}
	}

	for (int  i=0;i<m_cUserData.GetSize();i++)
	{
		CPhasePlotData* pUserData = m_cUserData.GetAt(i);
		if (!pUserData) continue;

		CTPoint<double> ptTime[2];
		if (pUserData->m_nXData !=-1)
		{
			ptTime[0].x = pUserData->m_pXaxis[0];
			ptTime[0].y = yRange.x;

			ptTime[1].x = pUserData->m_pXaxis[0];
			ptTime[1].y = yRange.y;
		}
		if (pUserData->m_nYData !=-1)
		{
			ptTime[0].y = pUserData->m_pYaxis[0];
			ptTime[0].x = xRange.x;

			ptTime[1].y = pUserData->m_pYaxis[0];
			ptTime[1].x = xRange.y;
		}
		COLORREF mclr = pUserData->m_clrData;
		int nMode = m_pGraph->FSQUARE;
		int nSize = 4;

		int nwidth= 1;
		if (m_pHypData && m_pHypData==pUserData)
		{
			nwidth= 2;
		}

		CString strHyp;
		strHyp.Format(_T("Hypothesis (T = %d)"),(int)pUserData->m_bSelected);
		m_pGraph->Markers(ptTime, 1 ,RGB(192,192,192), nMode,m_cPPData.GetSize()+4, strHyp, 0);
		//m_pGraph->Markers(ptTime, 1, mclr, nMode,m_cPPData.GetSize()+5+i,pUserData->m_strName,nSize);
		m_pGraph->Lines(ptTime,2, mclr, m_cPPData.GetSize()+5+i, pUserData->m_strName,nwidth,PS_SOLID);
	}

	//	m_pGraph->MoveTo(CTPoint<double>(2, 2));
	//	m_pGraph->LineTo(CTPoint<double>(95, 95));
	m_pGraph->EndDraw(pDC->m_hDC);
}

void CViewPhasePlot::DrawFrame(CRect& rect, COLORREF cr, const char* Title)
{
	m_pGraph->RecalcRects(rect);
	if (m_bBoundary)
		m_pGraph->DrawBoundary(cr, 2);

	if (m_bTitle)
	{
		CLearningUnit*	pLU = GetDocument()->GetCurrentLU();
		//int nTT = GetDocument()->m_currTimer;
		int nTT = m_nCurrTime;

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
	}
	m_pGraph->Axes();
	if (m_bGrid)
		m_pGraph->Grid();
}

/////////////////////////////////////////////////////////////////////////////
// CViewPhasePlot message handlers

int CViewPhasePlot::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CScrollView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// ****** Add your code below this line ********** //
	//if (!m_cToolBar.Create( GetParent(),WS_CHILD | WS_VISIBLE | CBRS_TOP,120) ||
	if (!m_cToolBar.CreateEx(GetParent(), TBSTYLE_FLAT | TBSTYLE_TOOLTIPS, 
			WS_CHILD | WS_VISIBLE | CBRS_TOP | 
			/*CBRS_GRIPPER | */CBRS_TOOLTIPS | CBRS_FLYBY | CCS_ADJUSTABLE,
			CRect(0, 0, 0, 0), 120) ||
		!m_cToolBar.LoadToolBar(IDR_SIMULGRAPH))
	{
		TRACE0("Failed to create toolbar1\n");
		return -1;      // fail to create  
	}

	CRect rect;
	GetClientRect(rect);
//	m_GraphWnd.Create(_T("Graph Window"), rect, this, 11000);
	
	return 0;
}

void CViewPhasePlot::OnSize(UINT nType, int cx, int cy) 
{
	CScrollView::OnSize(nType, cx, cy);
	
//	m_GraphWnd.MoveWindow(0, 0, cx, cy);
	if(cx <= 0 || cy <= 0)
		return;
	RecalcSize();
	if (m_cToolBar && ::IsWindow(m_cToolBar.m_hWnd))
	{
		UINT tt = ID_TASK_CANCELTASK;
		UINT nID,nStyle;
		int nImage;
		m_cToolBar.GetButtonInfo(2,nID,nStyle,nImage);
		m_cToolBar.SetButtonInfo(2,nID,TBBS_SEPARATOR,cx-100);

	}
}

void CViewPhasePlot::RecalcSize()
{
	CRect rect;
	CSize sizeTotal;

	GetClientRect(rect);
	sizeTotal = rect.Size();
	SetScaleToFitSize(sizeTotal); // make it similar to CView

	if(m_bCanDraw)
		m_pGraph->RecalcRects(rect);	
}


LRESULT CViewPhasePlot::OnUpdateObjTooltip(WPARAM wp, LPARAM lp)
{
	if (!lp || !wp)
	{
		myToolTip.Activate(FALSE);
		return 0L;
	}

	CPhasePlotData *pData = (CPhasePlotData*)wp;
	CLearningUnit*	pLU = GetDocument()->GetCurrentLU();

	//CMdData *pObj = pLU->GetDataAt(pData->m_nYData);

	CString mstr = pData->m_strName;//pObj->GetDataName(FALSE);

	myToolTip.UpdateTipText(mstr,this);
	if (!myToolTip.IsWindowVisible())
	{
		CRect mmmm;
		mmmm.SetRectEmpty();
		myToolTip.GetMargin(mmmm);
		myToolTip.Activate(TRUE);
	}
	return 0L;
}





void CViewPhasePlot::OnInitialUpdate() 
{
	if (m_pER)
		GetParentFrame()->SetWindowText(m_pER->GetName());
	CTrace::T_OPEN(GetParentFrame());

	myToolTip.Create(this);
	myToolTip.AddTool(this);
	myToolTip.SetDelayTime(TTDT_INITIAL,10);
	myToolTip.SetDelayTime(TTDT_RESHOW,100);

	CFormatPhasePlot *pDlg = GetFormats();
	if (pDlg)
	{
		SetScaleType(pDlg->m_nScale);
		m_bStabPt = pDlg->m_bShowStab;
		m_bCurrT = pDlg->m_bShowCurrTime;
		m_bTitle = pDlg->m_bShowLegend;
		m_bGrid = pDlg->m_bShowAxe;
	}
	OnUpdateTimer();
	CScrollView::OnInitialUpdate();

	CSimulDoc *pDoc = GetDocument();

	BOOL bDrawPoints= TRUE;
	double x_range = pDoc->GetMaxTime()-1;

	m_nDeltaTime = 1.0;


	CRect  rect;
    GetClientRect(&rect);
    CSize sizeTotal = CSize(rect.right-rect.left, rect.bottom-rect.top);
	SetScaleToFitSize(sizeTotal);
	// this demo only support MM_TEXT mode
	ASSERT(m_nMAPMode == MM_TEXT);

//	CFormatPhasePlot *pDlg = GetFormats();
/*	if (pDlg)
	{
		m_bStabPt = pDlg->m_bShowStab;
		m_bCurrT = pDlg->m_bShowCurrTime;
		m_bTitle = pDlg->m_bShowLegend;
		m_bGrid = pDlg->m_bShowAxe;
	}*/

	POSITION pos = pDoc->m_cUserInput.GetStartPosition();
	while (pos)
	{
		CUserOutput pUser;
		int nb;
		pDoc->m_cUserInput.GetNextAssoc(pos,nb,pUser);
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

	m_pGraph->EnableLegend(m_bTitle==TRUE);
	RecalcSize();
//	Invalidate();
	CWnd *pWnd = GetParent();
	CRect mrect;
	if (pWnd)
	{
		pWnd->GetWindowRect(mrect);
		pWnd->SetWindowPos(NULL,0,0,mrect.Width()-1,mrect.Height(),SWP_NOZORDER|SWP_NOMOVE);
	}

}

BOOL CViewPhasePlot::CanEdit()
{
	CSimulDoc *pDoc = GetDocument();
	if (!pDoc) return FALSE;

	int	nTrans = GetTranslation();
	if (nTrans != TRANS_DYNAL) return FALSE;

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
				(pBP.m_nBPType == BREAKP_ACTION/* || pBP.m_nBPType == BREAKP_HYPOT*/) &&
				!bD;

	return bEditable;

}



/////////////////////////////////////////////////////////////////////////////
/// The framework calls this function after receiving a DOC_UPDATE_ALLDATA notification.
///
/// Response to data update
/////////////////////////////////////////////////////////////////////////////
BOOL CViewPhasePlot::OnUpdateData()
{
	CSimulDoc *pDoc = GetDocument();
	if (!pDoc) return FALSE;
	CMdEquation*	m_pEqu = pDoc->GetCurrentModel();
	if (!m_pEqu) return FALSE;
	CLearningUnit*	pLU = pDoc->GetCurrentLU();
	if (!pLU) return FALSE;

	m_nTransData = -1;
	m_pSelData = NULL;
	m_pHypData = NULL;
	m_pHypData2 = m_pHypData3 = NULL;

	int nGType;
	CFormatPhasePlot *pDlg = GetFormats();
	if (pDlg)
	{
		nGType = pDlg->m_nScale;
	}
	m_nMaxTime = 0;
	//if (pLU->m_simulID < 2) return;

	for (int kk=0;kk<m_cPPData.GetSize();kk++)
	{
		CPhasePlotData* pData = m_cPPData.GetAt(kk);
		if (!pData) continue;
		delete pData;
	}
	m_cPPData.RemoveAll();

	int nbOutC = m_pER->m_cOutputSet.GetSize();

	double xMin = 45000000;
	double yMin = 45000000;
	double xMax = -45000000;
	double yMax = -45000000;

	int nExpSet = pLU->m_cExpSet.GetSize();
	int nbPar = m_pEqu->m_cParSet.GetSize();

	CMdData *pTime = pLU->GetDataAt(nbPar);
	if (pTime)
	{
		CString m_strXPlot = pTime->GetDataName(pLU->m_bShowAbbrev);
		double d1 = pTime->GetAt(0,0);
		double d2 = pTime->GetAt(1,0);
		m_nDeltaTime = d2-d1;
	}


	for (int i=0;i<nbOutC;i+=2)
	{
		COutcomes *pCo= m_pER->m_cOutputSet.GetAt(i);
		if (!pCo) continue;
		if (m_nXData == -1)	m_nXData = pCo->m_nData;
		COutcomes *pCo2= m_pER->m_cOutputSet.GetAt(i+1);
		if (!pCo2) continue;
		if (m_nYData == -1)	m_nYData = pCo2->m_nData;

		if (!pCo->m_nSelected || !pCo2->m_nSelected) continue;
		if (pCo->m_nExpSet>= nExpSet) continue;

		if (pCo->m_nExpSet == pCo2->m_nExpSet)
		{
			int maxT = pLU->GetMaxTimer();
			m_nMaxTime = maxT;

			CPhasePlotData *pData=new CPhasePlotData(maxT+2);
			pData->m_nExpSet = pCo->m_nExpSet;

			pData->m_strName =  pLU->m_cExpSet.GetAt(pData->m_nExpSet)->GetName();
			pData->m_nXData = m_nXData;
			pData->m_nYData = m_nYData;

			m_cPPData.Add(pData);

			int nb = pLU->GetDataSize();
			//int nExpSet = 0;
			//int nbExpSet = pLU->m_cExpSet.GetSize();

			CMdData *pPrey = pLU->GetDataAt(m_nXData);
			m_strXPlot = pPrey->GetDataName(pLU->m_bShowAbbrev);
			//m_pGraph->XAxisTitle(mstr);

			CMdData *pPred = pLU->GetDataAt(m_nYData);
			m_strYPlot = pPred->GetDataName(pLU->m_bShowAbbrev);
			//m_pGraph->YAxisTitle(mstr);

			BOOL bXEdit=FALSE;
			BOOL bYEdit=FALSE;
			CModelObj* pObj = pPrey->GetDataInfo();
			if (pObj) bXEdit = (pObj->GetIcon() <=2);
			pObj = pPred->GetDataInfo();
			if (pObj) bYEdit = (pObj->GetIcon() <=2);
			bYEdit*=2;

			pData->m_bEditable = bXEdit + bYEdit;

			for (int i=0;i<=maxT;i++)
			{
				pData->m_pXaxis[i] = pPrey->GetAt(i,pData->m_nExpSet);
				pData->m_pYaxis[i] = pPred->GetAt(i,pData->m_nExpSet);
				//if (nbExpSet >=2)
			}
			double nMin,nMax;
			pPrey->GetMinMax(&nMin,&nMax,pData->m_nExpSet);
			double nMin1,nMax1;
			pPred->GetMinMax(&nMin1,&nMax1,pData->m_nExpSet);

			//CFormatPhasePlot *pDlg = GetFormats();

			/*	double dx = nMax - nMin;
				double dy = nMax1 - nMin1;

				dx = (5.+dx)/5.;
				int bg = 5*(int)(dx);
				dy = (5.+dy)/5.;
				int bg2 = 5*(int)(dy);
				//		dx = dx / 50;
				//		dy = dy / 50;

		
				double xlMin = floor(nMin);
				double ylMin = floor(nMin1);
				double xlMax = ceil(nMax);
				double ylMax = ceil(nMax1);

				bg = (int)((bg-(xlMax-xlMin)) / 2);
				bg2 = (int)((bg2-(ylMax-ylMin)) / 2);

				xMin = min(xMin,(xlMin-bg));
				yMin = min(yMin,ylMin);
				xMax = max(xMax,1+xlMax+bg);
				yMax = max(yMax,ylMax);*/
			xMin = min(xMin,nMin);
			yMin = min(yMin,nMin1);
			xMax = max(xMax,nMax);
			yMax = max(yMax,nMax1);
			if (pDlg)
			{
				int n1 = pDlg->m_nIsoX0;
				int n2 =pDlg->m_nIsoY0;
				int n3 = pDlg->m_nIsoXN;
				int n4 = pDlg->m_nIsoYN;

				if (n1!=-1)
				{
					CMdData *pp1 = pLU->GetDataAt(n1);
					if (pp1)
					{
						double dd = pp1->GetAt(0,pData->m_nExpSet);
						yMax = max(yMax,dd);
					}
				}
				if (n2!=-1)
				{
					CMdData *pp1 = pLU->GetDataAt(n2);
					if (pp1)
					{
						double dd = pp1->GetAt(0,pData->m_nExpSet);
						xMax = max(xMax,dd);
					}
				}
				if (n3!=-1)
				{
					CMdData *pp1 = pLU->GetDataAt(n3);
					if (pp1)
					{
						double dd = pp1->GetAt(0,pData->m_nExpSet);
						yMax = max(yMax,dd);
					}
				}
				if (n4!=-1)
				{
					CMdData *pp1 = pLU->GetDataAt(n4);
					if (pp1)
					{
						double dd = pp1->GetAt(0,pData->m_nExpSet);
						xMax = max(xMax,dd);
					}
				}

			}
		}
	}
	int nXTicks = 8;
	int nYTicks = 8;

	if (nGType==1 || nGType==3)
	{
		if (xMin<0.1) xMin=0.1;
		if (xMax<0.1) xMax=0.1;
	    double ddd  = log10(xMax);
		ddd = ceil(ddd);
		xMax = pow(10.0, ddd);
		double bbb  = log10(xMin);
		bbb = floor(bbb);
		xMin = pow(10.0, bbb);
	}
	if (nGType==2 || nGType==3)
	{
		if (yMin<0.1) yMin=0.1;
		if (yMax<0.1) yMax=0.1;
	    double ddd  = log10(yMax);
		ddd = ceil(ddd);
		yMax = pow(10.0, ddd);
		double bbb  = log10(yMin);
		bbb = floor(bbb);
		yMin = pow(10.0, bbb);
	}
	if (nGType==0 || nGType==2) // X lineare
	{
		double range = xMax-xMin;
		double cc = log10(range);
		int dec = (int)floor(cc);
		double mul = pow(10.0, dec);
		int rrr = (int)(ceil(range / mul) * mul);
		nXTicks = (int)(rrr / mul);
		if (nXTicks<=2) nXTicks*=5;
		if (nXTicks<=5) nXTicks*=2;
		int nn = (int)(yMin /mul); 
		//xMin = nn * mul;
		xMin = floor(xMin / mul) * mul;
		xMax = ceil(xMax / mul) * mul;
		nXTicks = (int)((xMax-xMin)/mul);
		if (nXTicks<=2) nXTicks*=5;
		if (nXTicks<=5) nXTicks*=2;
		if (!nXTicks) nXTicks = 10;
	}
	if (nGType==0 || nGType==1) // Y lineare
	{
		double range = yMax-yMin;
		double cc = log10(range);
		int dec = (int)floor(cc);
		double mul = pow(10.0, dec);
		int rrr = (int)(ceil(range / mul) * mul);
		nYTicks = (int)(rrr / mul);
		if (nYTicks<=2) nYTicks*=5;
		if (nYTicks<=5) nYTicks*=2;
		int nn = (int)(yMin /mul); 
		//yMin = nn * mul;
		yMin = floor(yMin / mul) * mul;
		yMax = ceil(yMax / mul) * mul;
		nYTicks = (int)((yMax-yMin)/mul);
		if (nYTicks<=2) nYTicks*=5;
		if (nYTicks<=5) nYTicks*=2;
		if (!nYTicks) nYTicks = 10;
	
	}

	if (yMin==yMax)
	{
		yMin -= yMin*.1;
		yMax += yMax*.1;
	}
	if (xMin==xMax)
	{
		xMin -= xMin*.1;
		xMax += xMax*.1;
	}

	xRange.x = xMin;
	xRange.y = xMax;
	yRange.x = yMin;
	yRange.y = yMax;


	m_pGraph->SetRange(xMin,yMin, xMax, yMax);
	m_pGraph->SetRatio(0,0 , 1, 1);
	m_pGraph->SetXDecimal(2);
	m_pGraph->SetYDecimal(2);

	m_strTitle = m_pEqu->GetSimulationName();

	m_pGraph->SetYNumOfGridTicks(nYTicks);
	m_pGraph->SetYNumOfTicks(nYTicks*5);
	m_pGraph->SetYNumOfSep(nYTicks);
	m_pGraph->SetXNumOfGridTicks(nXTicks);
	m_pGraph->SetXNumOfTicks(nXTicks*5);
	m_pGraph->SetXNumOfSep(nXTicks);
	//m_pGraph->SetYNumOfSep(nTicks);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
/// The framework calls this function after receiving a DOC_UPDATE_TIMERDATA notification.
///
/// Response to data update
/////////////////////////////////////////////////////////////////////////////
BOOL CViewPhasePlot::OnUpdateTimer()
{
//	int deb = GetDocument()->m_currTimer;
//	int nbExpSet = pLU->m_cExpSet.GetSize();
//	nbExpSet = 0;
//	m_nTransData = -1;
/****		CMdData *pPreyL = pLU->GetDataAt(nbX);
		CString mstr = pPreyL->GetDataName();

		CMdData *pPredL = pLU->GetDataAt(nbY);
		mstr = pPredL->GetDataName();
	
		pt[0].x = pPreyL->GetAt(deb,nbExpSet);
		pt[0].y = pPredL->GetAt(deb,nbExpSet);*///////
//	}
	/*else if (lHint==TRANSLATION_ACTION)
	{
		CUserData *pData = (CUserData *)pHint;
		OnUpdateAction(pData);
		//Invalidate();
	}*/
	m_bCanEdit = FALSE;
	m_pSelData = NULL;
	m_pHypData = NULL;
	m_pHypData2 = m_pHypData3 = NULL;

//	m_nTransData = -1;

	m_nCurrTime = GetDocument()->GetCurrTime();

	m_bCanEdit = CanEdit();
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
/// The framework calls this function after receiving a DOC_UPDATE_BREAKPOINT notification.
///
/// Response to data update
/////////////////////////////////////////////////////////////////////////////
BOOL CViewPhasePlot::OnUpdateBreakPoint(int nBPTime,int nBPType)
{
	m_bCanEdit = CanEdit();
	m_bCanEdit = FALSE;
	m_pSelData = NULL;
	m_pHypData = NULL;
	m_pHypData2 = m_pHypData3 = NULL;
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
/// The framework calls this function after receiving a TRANSLATION_ACTION notification.
///
/// Response to data update
/////////////////////////////////////////////////////////////////////////////
BOOL CViewPhasePlot::OnUpdateAction(CUserData *pData)
{
	int nbTrans = GetTranslation();
	BOOL bTrans = (nbTrans<TRANS_DYNAL);

	BOOL bRet = FALSE;

	if (pData && !pData->m_bUpdateNow)
	{
		if (bTrans) return FALSE;
		for (int  i=0;i<m_cPPData.GetSize();i++)
		{
			CPhasePlotData* pUserData = m_cPPData.GetAt(i);
			if (!pUserData) continue;
			if (pUserData->m_nExpSet != pData->m_nExpSet) continue;
			
			if (pUserData->m_nYData== pData->m_nOutcome)
			{
				//pUserData->m_pXaxis[pData->m_nTime] = pData->m_nTime;
				pUserData->m_pYaxis[pData->m_nTime] = pData->m_dValue;
				bRet = TRUE;
				//break;
			}
			if (pUserData->m_nXData== pData->m_nOutcome)
			{
				//pUserData->m_pXaxis[pData->m_nTime] = pData->m_nTime;
				pUserData->m_pXaxis[pData->m_nTime] = pData->m_dValue;
				bRet = TRUE;
			}
		}
	}
	else bRet = OnUpdateData();

	return bRet;
}

/////////////////////////////////////////////////////////////////////////////
/// The framework calls this function after receiving a TRANSLATION_HYPOTHESIS notification.
///
/// Response to data update
/////////////////////////////////////////////////////////////////////////////
BOOL CViewPhasePlot::OnUpdateHypothesis(CUserData *pData)
{
	if (!pData) return FALSE;

	int nbTrans = GetTranslation();
	if (nbTrans<TRANS_DYNAL) return FALSE;

	BOOL bFound = FALSE;
	for (int  i=0;i<m_cUserData.GetSize();i++)
	{
		CPhasePlotData* pUserData = m_cUserData.GetAt(i);
		if (!pUserData) continue;
		if (pUserData->m_nExpSet == pData->m_nExpSet)
		{
			if (pUserData->m_nYData== pData->m_nOutcome)
			{
				//pUserData->m_pXaxis[0] = pData->m_nTime;
				pUserData->m_pYaxis[0] = pData->m_dValue;
				bFound = TRUE;
				break;
			}
			else if (pUserData->m_nXData== pData->m_nOutcome)
			{
				//pUserData->m_pXaxis[0] = pData->m_nTime;
				pUserData->m_pXaxis[0] = pData->m_dValue;
				bFound = TRUE;
				break;
			}
		}
	}
	if (bFound) return TRUE;

	CSimulDoc *pDoc = GetDocument();
	if (!pDoc) return FALSE;
	CMdEquation*	m_pEqu = pDoc->GetCurrentModel();
	if (!m_pEqu) return FALSE;
	CLearningUnit*	pLU = pDoc->GetCurrentLU();
	if (!pLU) return FALSE;

	int nbOutC = m_pER->m_cOutputSet.GetSize();
	BOOL bDisplay = FALSE;
	int nXY = 0;
	
	for (i=0;i<nbOutC;i+=2)
	{
		COutcomes *pCoX= m_pER->m_cOutputSet.GetAt(i);
		if (!pCoX) continue;
		COutcomes *pCoY= m_pER->m_cOutputSet.GetAt(i+1);
		if (!pCoY) continue;
		//if (!pCo->m_nSelected) continue;

		if (pCoX->m_nData == pData->m_nOutcome &&
			pCoX->m_nExpSet == pData->m_nExpSet)
		{
			bDisplay = TRUE;
			nXY |= 1;
		}
		if (pCoY->m_nData == pData->m_nOutcome &&
			pCoX->m_nExpSet == pData->m_nExpSet)
		{
			bDisplay = TRUE;
			nXY |= 2;
		}
	}

	if (!nXY) return FALSE;

	CMdData *pX = pLU->GetDataAt(pData->m_nOutcome);

	//int nbPar = m_pEqu->m_cParSet.GetSize();
	CPhasePlotData* pUserData = new CPhasePlotData(1);
	pUserData->m_bPred = TRUE;
	pUserData->m_bEditable = TRUE;
	pUserData->m_nExpSet = pData->m_nExpSet;
	pUserData->m_bSelected = pData->m_nTime;
	
	pUserData->m_nXData = -1;
	pUserData->m_nYData = -1;
	if (nXY & 1)
	{
		pUserData->m_nXData = pData->m_nOutcome;
		pUserData->m_pXaxis[0] = pData->m_dValue;
	}
	if (nXY & 2)
	{
		pUserData->m_nYData = pData->m_nOutcome;
		pUserData->m_pYaxis[0] = pData->m_dValue;
	}

	pUserData->m_clrData = pX->GetDataInfo()->GetColor();

	CString strExpSet = pLU->m_cExpSet.GetAt(pData->m_nExpSet)->GetName();
	CString m_strYPlot = pX->GetDataName(pLU->m_bShowAbbrev);
	pUserData->m_strName = pX->GetDataName(pLU->m_bShowAbbrev) + _T(" (") + strExpSet + _T(")");

	m_cUserData.Add(pUserData);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
/// The framework calls this function after receiving a TRANSLATION_MAPRELATION notification.
///
/// Response to data update
/////////////////////////////////////////////////////////////////////////////
BOOL CViewPhasePlot::OnUpdateMapRelation(CTranslationInfo *mTInfo )
{
	if (mTInfo) 
	{
		//CTranslationInfo *mTInfo = (CTranslationInfo *)pHint;
		m_nTransData = -1;
		int nTime = -1;//mTInfo->m_nTime;
		int nExpSet = -1;//mTInfo->m_nExpSet;
		int nData = -1;//mTInfo->m_nData;
		int nb = mTInfo->m_nDatas.GetSize();

		m_nCurrTime = mTInfo->m_nTime;
		if (nb == 2)
		{
			CTranslationInfo::CTInfo tt1 = mTInfo->m_nDatas.GetAt(0);
			CTranslationInfo::CTInfo tt2 = mTInfo->m_nDatas.GetAt(1);
			tt1.nData;
			tt1.nExpSet;
			for (int kk=0;kk<m_cPPData.GetSize();kk++)
			{
				CPhasePlotData* pData = m_cPPData.GetAt(kk);
				if (!pData) continue;
				
				pData->m_bSelected = FALSE;
				if ( 
						(((pData->m_nXData ==tt1.nData) && (pData->m_nExpSet ==tt1.nExpSet)) ||
						((pData->m_nXData ==tt2.nData) && (pData->m_nExpSet ==tt2.nExpSet))) &&
						(((pData->m_nYData ==tt1.nData) && (pData->m_nExpSet ==tt1.nExpSet)) ||
						((pData->m_nYData ==tt2.nData) && (pData->m_nExpSet ==tt2.nExpSet)))
					   )
				{
					pData->m_bSelected = 3;
					m_nTransData = kk;
					//break;
				}
				//else
				//	pData->m_bSelected = FALSE;
			}
		}
		else if (nb)
		{
			CTranslationInfo::CTInfo tt1 = mTInfo->m_nDatas.GetAt(0);
			nData = tt1.nData;
			nExpSet = tt1.nExpSet;
			for (int kk=0;kk<m_cPPData.GetSize();kk++)
			{
				CPhasePlotData* pData = m_cPPData.GetAt(kk);
				if (!pData) continue;

				if (((pData->m_nXData ==nData) || (pData->m_nYData ==nData)) &&
					pData->m_nExpSet == nExpSet)
				{
					if (pData->m_nXData ==nData)
						pData->m_bSelected = 1;
					else
						pData->m_bSelected = 2;
					m_nTransData = kk;
					//break;
				}
				else
					pData->m_bSelected = FALSE;
			}
		}
	}
	else
	{
		m_nTransData = -1;
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
/// The framework calls this function after receiving a notification message.
///
/// \param pSender Points to the view that modified the document, or NULL if all views are to be updated.
/// \param lHint Contains information about the modifications.
/// \param pHint Points to an object storing information about the modifications.
///
/// This member function proceeds and/or dispatches all the messages sent by the framework
/// to the relevant function.
/////////////////////////////////////////////////////////////////////////////
void CViewPhasePlot::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	// TODO: Add your specialized code here and/or call the base class
	CMdEquation*	m_pEqu = GetDocument()->GetCurrentModel();
	if (!m_pEqu) return;
	CLearningUnit*	pLU = GetDocument()->GetCurrentLU();
	if (!pLU) return;

	int nbTrans = GetTranslation();
	BOOL bRet = TRUE;

	if (lHint == DOC_UPDATE_TIMERDATA)
	{
		OnUpdateTimer();
	}
	if (lHint == TRANSLATION_HYPOTHESIS )
	{
		CUserData *pData = (CUserData *)pHint;
		bRet = OnUpdateHypothesis(pData);
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


	if (lHint == DOC_UPDATE_ER || !lHint)
	{
		CFormatPhasePlot *pDlg = GetFormats();
		if (pDlg)
		{
			m_bStabPt = pDlg->m_bShowStab;
			m_bCurrT = pDlg->m_bShowCurrTime;
			m_bTitle = pDlg->m_bShowLegend;
			m_bGrid = pDlg->m_bShowAxe;
			if (lHint == DOC_UPDATE_ER)
				SetScaleType(pDlg->m_nScale);
		}
		m_pGraph->EnableLegend(m_bTitle==TRUE);
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
	}
	if (!lHint || lHint == DOC_UPDATE_ALLDATA)
	{
		bRet = OnUpdateData();
	}

	if (bRet)
	{
		RecalcSize();
		Invalidate();
		UpdateWindow();
	}
}

BOOL CViewPhasePlot::SetScaleType(int nScale)
{
	BOOL bRet = TRUE;
	switch (nScale)
	{
	case 0:
		m_pGraph = &m_Linear;
		break;
	case 1:
		m_pGraph = &m_XLogYLinear;
		break;
	case 2:
		m_pGraph = &m_XLinearYLog;
		break;
	case 3:
		m_pGraph = &m_XLogYLog;
		break;
	default:
		bRet=FALSE;
	}
	return bRet;
}


void CViewPhasePlot::OnSetScaleType(UINT nID) 
{
	// TODO: Add your command handler code here
	BOOL nRedraw = TRUE;
	switch (nID){
	case ID_XYGRAPH_SCALETYPE_LINEAR:
		//m_pGraph = &m_Linear;
		//break;
	case ID_XYGRAPH_SCALETYPE_XLOG:
		//m_pGraph = &m_XLogYLinear;
		//break;
	case ID_XYGRAPH_SCALETYPE_YLOG:
		//m_pGraph = &m_XLinearYLog;
		//break;
	case ID_XYGRAPH_SCALETYPE_XYLOG:
		//m_pGraph = &m_XLogYLog;
		nRedraw = SetScaleType(nID-ID_XYGRAPH_SCALETYPE_LINEAR);
		break;
	default:
		nRedraw = FALSE;
		break;
	}
	if (nRedraw)
	{
		OnUpdate(NULL,0,NULL);
		RecalcSize();
		Invalidate();
		UpdateWindow();
	}
}

void CViewPhasePlot::OnUpdateScaleType(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	BOOL bSetCheck = FALSE;
	switch (pCmdUI->m_nID){
	case ID_XYGRAPH_SCALETYPE_LINEAR:
		bSetCheck = (m_pGraph == &m_Linear);
		break;
	case ID_XYGRAPH_SCALETYPE_XLOG:
		bSetCheck = (m_pGraph == &m_XLogYLinear);
		break;
	case ID_XYGRAPH_SCALETYPE_YLOG:
		bSetCheck = (m_pGraph == &m_XLinearYLog);
		break;
	case ID_XYGRAPH_SCALETYPE_XYLOG:
		bSetCheck = (m_pGraph == &m_XLogYLog);
		break;
	default:
		break;
	}
	pCmdUI->SetCheck(bSetCheck);
}

void CViewPhasePlot::OnFilePrintPreview() 
{
	// TODO: Add your command handler code here
	CScrollView::OnFilePrintPreview();
}

void CViewPhasePlot::OnEndPrintPreview(CDC* pDC, CPrintInfo* pInfo, POINT point, CPreviewView* pView) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CScrollView::OnEndPrintPreview(pDC, pInfo, point, pView);
	RecalcSize();

}

void CViewPhasePlot::OnSelectData() 
{
	// TODO: Add your command handler code here
	if (!m_pER) return;

	CSelectOutcomeDlg dlg(this,GetDocument());
	dlg.m_bForceSync = TRUE;

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
		//InitView();
		OnUpdate(0,0,0) ;
	}	
}

void CViewPhasePlot::OnTranslationTasks(UINT nID)
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

void CViewPhasePlot::OnUpdateTransTasks(CCmdUI* pCmdUI) 
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

void CViewPhasePlot::OnDestroy() 
{
	CTrace::T_CLOSE(GetParentFrame());
	CScrollView::OnDestroy();
	
	// TODO: Add your message handler code here
	
}

int  CViewPhasePlot::HitTest(CPoint mPt,CPhasePlotData *pData,int nLim/*=-1*/)
{
	int nRet = -1;
	BOOL bFound = FALSE;
	int nCount = (nLim == -1) ? pData->m_nData : nLim;
	if (pData->m_bPred) 
	{
		CTPoint<double> ptTime[2];
		CRect mrect;
		if (pData->m_nXData !=-1)
		{
			ptTime[0].x = pData->m_pXaxis[0];
			ptTime[0].y = yRange.x;

			ptTime[1].x = pData->m_pXaxis[0];
			ptTime[1].y = yRange.y;
			CPoint npt1 = m_pGraph->GetTPoint(ptTime[0]);
			CPoint npt2 = m_pGraph->GetTPoint(ptTime[1]);
			mrect = CRect(npt1,npt2);
			mrect.InflateRect(5,0,5,0);
			mrect.NormalizeRect();
		}
		if (pData->m_nYData !=-1)
		{
			ptTime[0].y = pData->m_pYaxis[0];
			ptTime[0].x = xRange.x;

			ptTime[1].y = pData->m_pYaxis[0];
			ptTime[1].x = xRange.y;
			CPoint npt1 = m_pGraph->GetTPoint(ptTime[0]);
			CPoint npt2 = m_pGraph->GetTPoint(ptTime[1]);
			mrect = CRect(npt1,npt2);
			mrect.InflateRect(0,5,0,5);
		}
		bFound = mrect.PtInRect(mPt);
		if (bFound) nRet = 0;
		//nCount=0;
	}
	else for (int i=nCount;i>=0 && !bFound;i--)
	{
		CTPoint<double> pt1;
		pt1.x = pData->m_pXaxis[i];
		pt1.y = pData->m_pYaxis[i];
		CPoint npt = m_pGraph->GetTPoint(pt1);
		CRect mrect(npt,npt);
		mrect.InflateRect(10,10);
		bFound = mrect.PtInRect(mPt);
		if (bFound) nRet = i;
	}
	return nRet;
}

void CViewPhasePlot::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	m_pSelData = NULL;
	m_pHypData = NULL;
	m_pHypData2 = m_pHypData3 = NULL;
	
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
		for (int i=0;i<nb;i++)
		{
			CPhasePlotData *pData = m_cUserData.GetAt(i);
			if (!pData) continue;

			int nTest = HitTest(point,pData,GetDocument()->m_runTimer);
			if (nTest!=-1)
			{

				m_pHypData = pData;
				bFound = TRUE;
				break;
			}
		}
		if (!bFound && m_nTransData != -1)
		{
			CPhasePlotData *pData = m_cPPData.GetAt(m_nTransData);

			int nTest = HitTest(point,pData,GetDocument()->m_runTimer);
			if (nTest != GetDocument()->m_runTimer)  return;

			int nSel = pData->m_bSelected;
			int nb = m_cUserData.GetSize();

			for (int i=0;i<nb;i++)
			{
				CPhasePlotData *pUData = m_cUserData.GetAt(i);
				if (!pUData) continue;

				if (pUData->m_nExpSet == pData->m_nExpSet &&
					pUData->m_nYData == pData->m_nYData)
				{
					if (nSel==2)
					{
						m_pHypData = pUData;
						return;
					}
					else if (nSel==3)
					{
						m_pHypData3 = pUData;
						m_pHypData= pUData;
					}
				}
				if (pUData->m_nExpSet == pData->m_nExpSet &&
					pUData->m_nXData == pData->m_nXData)
				{
					if (nSel==1)
					{
						m_pHypData = pUData;
						return;
					}
					else if (nSel==3)
					{
						m_pHypData2 = pUData;
						m_pHypData= pUData;
					}
				}
			}
			if (m_pHypData2 || m_pHypData3) return;


			CUserOutput pT2;
			BOOL bOutput = GetDocument()->m_cUserInput.Lookup(nCBP,pT2);
			if (!bOutput) return;

			CLearningUnit*	pLU = GetDocument()->GetCurrentLU();
			BOOL bEditable = pLU->GetTimerBPAt(pT2.m_nTime,pBP);
			if (!bEditable) return;

			if (nSel==1)
			{
				CUserData pUData;
				pUData.m_nExpSet = pData->m_nExpSet; 
				pUData.m_nOutcome = pData->m_nXData;
				pUData.m_nTime = pT2.m_nTime;
				pUData.m_dOldValue = pData->m_pXaxis[nCBP];
				pUData.m_dValue = pData->m_pXaxis[nCBP];

				if (OnUpdateHypothesis(&pUData))
				{
					nb = m_cUserData.GetSize();
					m_pHypData = m_cUserData.GetAt(nb-1);
				}
			}
			else if (nSel==2)
			{
				CUserData pUData;
				pUData.m_nExpSet = pData->m_nExpSet; 
				pUData.m_nOutcome = pData->m_nYData;
				pUData.m_nTime = pT2.m_nTime;
				pUData.m_dOldValue = pData->m_pYaxis[nCBP];
				pUData.m_dValue = pData->m_pYaxis[nCBP];

				if (OnUpdateHypothesis(&pUData))
				{
					nb = m_cUserData.GetSize();
					m_pHypData = m_cUserData.GetAt(nb-1);
				}
			}
			else if (nSel==3)
			{
				CUserData pUData;
				pUData.m_nExpSet = pData->m_nExpSet; 
				pUData.m_nOutcome = pData->m_nXData;
				pUData.m_nTime = pT2.m_nTime;
				pUData.m_dOldValue = pData->m_pXaxis[nCBP];
				pUData.m_dValue = pData->m_pXaxis[nCBP];

				if (OnUpdateHypothesis(&pUData))
				{
					nb = m_cUserData.GetSize();
					m_pHypData2 = m_cUserData.GetAt(nb-1);
				}
				CUserData pUData2;
				pUData2.m_nExpSet = pData->m_nExpSet; 
				pUData2.m_nOutcome = pData->m_nYData;
				pUData2.m_nTime = pT2.m_nTime;
				pUData2.m_dOldValue = pData->m_pYaxis[nCBP];
				pUData2.m_dValue = pData->m_pYaxis[nCBP];

				if (OnUpdateHypothesis(&pUData2))
				{
					nb = m_cUserData.GetSize();
					m_pHypData3 = m_cUserData.GetAt(nb-1);
				}
			}
			if (m_pHypData3) m_pHypData=m_pHypData3;
			if (m_pHypData2) m_pHypData=m_pHypData2;

		}
	}

	CScrollView::OnLButtonDown(nFlags, point);
}

BOOL CViewPhasePlot::OnMoveData(CPhasePlotData* m_pHypData,CPoint point)
{
	if (!m_pHypData) return FALSE;

	double x = m_pHypData->m_pXaxis[0];
	double y = m_pHypData->m_pYaxis[0];

	CTPoint<double> pt1 = m_pGraph->GetTValue(point);
	y = pt1.y;
	x = pt1.x;

	if (y>yRange.y)	y=yRange.y;
	if (y<yRange.x)	y=yRange.x;
	if (x>xRange.y)	x=xRange.y;
	if (x<xRange.x)	x=xRange.x;

	CUserData pUData;
	pUData.m_nExpSet = m_pHypData->m_nExpSet; 
	pUData.m_nTime = m_pHypData->m_bSelected;//pgvItem->row-1;

	if (m_pHypData->m_nYData !=-1)
	{
		pUData.m_dOldValue = m_pHypData->m_pYaxis[0];
		pUData.m_nOutcome = m_pHypData->m_nYData;
		m_pHypData->m_pYaxis[0] = y;
		pUData.m_dValue = m_pHypData->m_pYaxis[0];
	}
	if (m_pHypData->m_nXData !=-1)
	{
		pUData.m_dOldValue = m_pHypData->m_pXaxis[0];
		pUData.m_nOutcome = m_pHypData->m_nXData;
		m_pHypData->m_pXaxis[0] = x;
		pUData.m_dValue = m_pHypData->m_pXaxis[0];
	}
	GetDocument()->UpdateUserData(NULL,&pUData);
	return TRUE;

}

void CViewPhasePlot::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CScrollView::OnMouseMove(nFlags, point);
	BOOL bUpdate = FALSE;

	int nCBP = GetDocument()->m_nCurrBP;
	int nRT = GetDocument()->m_runTimer;
	CTimerBreakPoint pBP;
	BOOL bEditable = GetDocument()->GetCurrentLU()->GetTimerBPAt(nCBP,pBP);
//	if (!bEditable) 
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
		}
		{
			int Index=m_cPPData.GetSize()+2;
			int y  = GT - 3 * Index * m_LogFont.lfHeight / 2;
			CRect rTest(xb,y-6,m_Rect.right,y+6);
			BOOL bFound = rTest.PtInRect(point);
			if (bFound)
			{
				//return;
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
				OnUpdateObjTooltip((WPARAM)pData,TRUE);
				return;
			}
		}
		OnUpdateObjTooltip(NULL,NULL);
	}
	if (!bEditable) 
		return;



	if (m_pSelData && m_nTransData !=-1)
	{
		double x = m_pSelData->m_pXaxis[GetDocument()->m_runTimer];
		double y = m_pSelData->m_pYaxis[GetDocument()->m_runTimer];

		CTPoint<double> pt1 = m_pGraph->GetTValue(point);

		if (m_pSelData->m_bSelected == 3) // X & Y
		{
			x = pt1.x;
			y = pt1.y;
		}
		else if (m_pSelData->m_bSelected == 2) // Y
		{
			y = pt1.y;
		}
		else if (m_pSelData->m_bSelected == 1) // X
		{
			x = pt1.x;
		}

		CPhasePlotData *pRRData = m_cPPData.GetAt(m_nTransData);
		CLearningUnit* pLU = GetDocument()->GetCurrentLU();
		CMdData *pObjData = pLU->GetDataAt(pRRData->m_nYData);
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
		pObjData = pLU->GetDataAt(pRRData->m_nXData);
		if (!pObjData) return;

		pObj = pObjData->GetDataInfo();
		pVarObj = DYNAMIC_DOWNCAST( CModelPar, pObj);

		dNewVal = x;

		if (pVarObj)
		{
			double dmin = pVarObj->GetMin();
			double dmax = pVarObj->GetMax();

			x = min(max(dNewVal,dmin),dmax);
		}


		m_pSelData->m_pXaxis[GetDocument()->m_runTimer] = x;
		m_pSelData->m_pYaxis[GetDocument()->m_runTimer] = y;

		double xMin = min(xRange.x,x);
		double xMax = max(xRange.y,x);
		double yMin = min(yRange.x,y);
		double yMax = max(yRange.y,y);

		m_pGraph->SetRange(xMin,yMin, xMax, yMax);
		//m_pGraph->SetYNumOfTicks(nTicks*5);
		//m_pGraph->SetYNumOfSep(nTicks);
		//m_pGraph->SetYNumOfGridTicks(nTicks);
		int m_nExpSet = m_pSelData->m_nExpSet;
		int m_nYData = m_pSelData->m_nYData;
		int m_nXData = m_pSelData->m_nXData;

		//CLearningUnit *pLU = GetDocument()->GetCurrentLU();
		CMdEquation*	pEqu = GetDocument()->GetCurrentModel();

		CMdData *pPrey= pLU->m_cDataPoints.GetAt(m_nXData);
		CString mstrX=pPrey->GetDataName(pLU->m_bShowAbbrev);
		CMdData *pPred= pLU->m_cDataPoints.GetAt(m_nYData);
		CString mstrY=pPred->GetDataName(pLU->m_bShowAbbrev);

		double nXo = pPrey->GetAt(GetDocument()->m_runTimer,m_nExpSet);
		double nXn = x;//m_pSelData->m_pXaxis[GetDocument()->m_runTimer];
		double nYo = pPred->GetAt(GetDocument()->m_runTimer,m_nExpSet);
		double nYn = y;//m_pSelData->m_pYaxis[GetDocument()->m_runTimer];

		CUserData pUData;
		pUData.m_nExpSet = m_nExpSet; 
		pUData.m_nOutcome = m_nXData;
		pUData.m_nTime = GetDocument()->m_runTimer;//pgvItem->row-1;
		pUData.m_dOldValue = nXo;
		pUData.m_dValue = nXn;
		pUData.m_bUpdateNow = FALSE;

		GetDocument()->UpdateUserData(NULL,&pUData);

		CUserData pUData2;
		pUData2.m_nExpSet = m_nExpSet; 
		pUData2.m_nOutcome = m_nYData;
		pUData2.m_nTime = GetDocument()->m_runTimer;//pgvItem->row-1;
		pUData2.m_dOldValue = nYo;
		pUData2.m_dValue = nYn;
		pUData2.m_bUpdateNow = FALSE;

		GetDocument()->UpdateUserData(NULL,&pUData2);

		bUpdate = TRUE;
	}
	else if (m_pHypData && nCBP==nRT && pBP.m_nBPType==BREAKP_HYPOT && !GetDocument()->m_bTaskDone)
	{
		if (m_pHypData2 || m_pHypData3)
		{
			if (m_pHypData2)
				 bUpdate = OnMoveData(m_pHypData2,point);
			if (m_pHypData3)
				 bUpdate = OnMoveData(m_pHypData3,point);
		}
		else
			bUpdate = OnMoveData(m_pHypData,point);
	}

	if (bUpdate)
	{
		//CPhasePlotData *pData = m_pSelData;

		RecalcSize();
		Invalidate();
		UpdateWindow();
	}
}

void CViewPhasePlot::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	BOOL bUpdate = FALSE;
	if (m_pSelData)
	{
		int m_nExpSet = m_pSelData->m_nExpSet;
		int m_nYData = m_pSelData->m_nYData;
		int m_nXData = m_pSelData->m_nXData;

		CLearningUnit *pLU = GetDocument()->GetCurrentLU();
		CMdEquation*	pEqu = GetDocument()->GetCurrentModel();

		CMdData *pPrey= pLU->m_cDataPoints.GetAt(m_nXData);
		CString mstrX=pPrey->GetDataName(pLU->m_bShowAbbrev);
		CMdData *pPred= pLU->m_cDataPoints.GetAt(m_nYData);
		CString mstrY=pPred->GetDataName(pLU->m_bShowAbbrev);

		double nXo = pPrey->GetAt(GetDocument()->m_runTimer,m_nExpSet);
		double nXn = m_pSelData->m_pXaxis[GetDocument()->m_runTimer];
		double nYo = pPred->GetAt(GetDocument()->m_runTimer,m_nExpSet);
		double nYn = m_pSelData->m_pYaxis[GetDocument()->m_runTimer];

		CUserData pUData;
		pUData.m_nExpSet = m_nExpSet; 
		pUData.m_nOutcome = m_nXData;
		pUData.m_nTime = GetDocument()->m_runTimer;//pgvItem->row-1;
		pUData.m_dOldValue = nXo;
		pUData.m_dValue = nXn;

		GetDocument()->UpdateUserData(this,&pUData);

		CUserData pUData2;
		pUData2.m_nExpSet = m_nExpSet; 
		pUData2.m_nOutcome = m_nYData;
		pUData2.m_nTime = GetDocument()->m_runTimer;//pgvItem->row-1;
		pUData2.m_dOldValue = nYo;
		pUData2.m_dValue = nYn;

		GetDocument()->UpdateUserData(this,&pUData2);
		bUpdate = TRUE;

		m_pSelData = NULL;
		//m_pHypData = NULL;
		m_nTransData = -1;
	}
	else if (m_pHypData)
	{
		bUpdate = TRUE;
	}

	m_pSelData = NULL;
	m_pHypData = NULL;
	//m_pHypData2 = m_pHypData3 = NULL;

	if (bUpdate)
	{
//		OnUpdateMapRelation(NULL);
		RecalcSize();
		Invalidate();
		UpdateWindow();
	}
	
	CScrollView::OnLButtonUp(nFlags, point);
}


void CViewPhasePlot::OnLButtonDblClk(UINT nFlags, CPoint point) 
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
				CTranslationInfo::CTInfo tt={m_nCurrTime,pData->m_nXData,pData->m_nExpSet};
				mTInfo.m_nDatas.Add(tt);
				CTranslationInfo::CTInfo tt1={m_nCurrTime,pData->m_nYData,pData->m_nExpSet};
				mTInfo.m_nDatas.Add(tt1);

				OnUpdateMapRelation(&mTInfo);

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
				//GetDocument()->UpdateMapRelation(NULL,NULL);
				return;
			}
		}*/

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
				m_pHypData = pData;
				Invalidate();
				UpdateWindow();
				OnUpdateObjTooltip((WPARAM)pData,TRUE);
				return;
			}
		}
	}

/*	if (nTrans < TRANS_MAPRELAT)
	{
		//GetDocument()->UpdateMapRelation(NULL,NULL);
		return;
	}*/


	CTPoint<double> pt1 = m_pGraph->GetTValue(point);
	CTPoint<double> nDelta;

	nDelta.x = (xRange.y - xRange.x) / 40;
	nDelta.y = (yRange.y - yRange.x) / 40;
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
			CTranslationInfo::CTInfo tt={nTest,pData->m_nXData,pData->m_nExpSet};
			mTInfo.m_nDatas.Add(tt);
			CTranslationInfo::CTInfo tt1={nTest,pData->m_nYData,pData->m_nExpSet};
			mTInfo.m_nDatas.Add(tt1);
			//GetDocument()->m_currTimer = nTest;
			//GetDocument()->UpdateAllViews(NULL,TRANSLATION_MAPRELATION,(CObject*)&mTInfo);
			if (nTrans==TRANS_INDEP)
			{
				OnUpdateMapRelation(&mTInfo);
				//OnUpdate(this,TRANSLATION_MAPRELATION,(CObject*)&mTInfo);
				Invalidate();
				UpdateWindow();
			}
			else
				GetDocument()->UpdateMapRelation(this,&mTInfo);
			//GetDocument()->UpdateMapRelation(this,&mTInfo);
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
		//GetDocument()->UpdateMapRelation(NULL,NULL);
	}
	//GetDocument()->UpdateAllViews(NULL,TRANSLATION_MAPRELATION,NULL);
	
	CScrollView::OnLButtonDblClk(nFlags, point);
}

void CViewPhasePlot::OnEditCopy() 
{
	// TODO: Add your command handler code here
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

BOOL CViewPhasePlot::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message== WM_LBUTTONDOWN ||
        pMsg->message== WM_LBUTTONUP ||
        pMsg->message== WM_MOUSEMOVE)
		myToolTip.RelayEvent(pMsg);
	
	return CScrollView::PreTranslateMessage(pMsg);
}
