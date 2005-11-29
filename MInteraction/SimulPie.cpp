// SimulPie.cpp : implementation file
//

#include "stdafx.h"
#include "Simul.h"

#include "LearnerTrace.h"

#include "simulDoc.h"
#include "SimulPie.h"
#include <MInstruction\LearningUnit.h>
#include <MSimulation\Model.h>
#include "Format.h"

#include <math.h>

#include <Prefs\Pref.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CViewPie

IMPLEMENT_DYNCREATE(CViewPie, CView)

CViewPie::CViewPie()
{
	m_bComplete = FALSE;
	m_nCurrTime=0;
	//m_nCurrTTask=0;
	m_pER = NULL;

	m_nSelExp = m_nSelOut = -1;
}

CViewPie::~CViewPie()
{
	CTrace::T_CLOSE(GetParentFrame());
}


BEGIN_MESSAGE_MAP(CViewPie, CView)
	//{{AFX_MSG_MAP(CViewPie)
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_CHART_BAR, OnChartBar)
	ON_COMMAND(ID_CHART_LINE, OnChartLine)
	ON_COMMAND(ID_CHART_PIE, OnChartPie)
	ON_UPDATE_COMMAND_UI(ID_CHART_PIE, OnUpdateCharts)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_DESTROY()
	ON_UPDATE_COMMAND_UI(ID_CHART_LINE, OnUpdateCharts)
	ON_UPDATE_COMMAND_UI(ID_CHART_BAR, OnUpdateCharts)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(DOC_UPDATE_VIEWCONTENT, OnUpdateInitContent)
	//ON_COMMAND_RANGE(ID_TRANS_INDEPENDENT,ID_TRANS_DYNALINKED,OnTranslationTasks)
	ON_UPDATE_COMMAND_UI_RANGE(ID_TRANS_INDEPENDENT,ID_TRANS_DYNALINKED,OnUpdateTransTasks)
	ON_MESSAGE(TRACE_VIEW_ACTIVATE, OnActivateER)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewPie drawing
BOOL CViewPie::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	//return CView::OnEraseBkgnd(pDC);
	return false;
}

void CViewPie::OnDraw(CDC* pDC)
{
	CSimulDoc* pDoc = GetDocument();

	// TODO: add draw code here
	int nRT = pDoc->GetRunTime();
	if(m_bComplete)
	{
		m_PieChart.SetInsertColor(CPref::g_crFuture);
		if (m_nCurrTime!=nRT)
			m_PieChart.SetBkColor(CPref::g_crPast);
		else
			m_PieChart.SetBkColor(RGB(255,255,255));

		m_PieChart.DrawGraph(pDC);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CViewPie diagnostics

#ifdef _DEBUG
void CViewPie::AssertValid() const
{
	CView::AssertValid();
}

void CViewPie::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CSimulDoc* CViewPie::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSimulDoc)));
	return (CSimulDoc*)m_pDocument;
}
#endif //_DEBUG

CFormatChart* CViewPie::GetFormats()
{
	CFormatChart* pDlg = NULL;

	if (!m_pER) return pDlg;
	int nbF = m_pER->m_cFormatSet.GetSize();
	if (nbF)
	{
		CFormat *pPage = m_pER->m_cFormatSet.GetAt(0);
		pDlg = DYNAMIC_DOWNCAST(CFormatChart,pPage);
	}

	return pDlg;
}

int	CViewPie::GetTranslation()
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
// CViewPie Updates
int CViewPie::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	if (!m_cToolBar.CreateEx(GetParent(), TBSTYLE_FLAT | TBSTYLE_TOOLTIPS, 
			WS_CHILD | WS_VISIBLE | CBRS_TOP | 
			/*CBRS_GRIPPER | */CBRS_TOOLTIPS | CBRS_FLYBY | CCS_ADJUSTABLE,
			CRect(0, 0, 0, 0), 120) ||
		!m_cToolBar.LoadToolBar(IDR_SIMULGRAPH))
	{
		TRACE0("Failed to create toolbar1\n");
		return -1;      // fail to create  
	}
	
	return 0;
}

void CViewPie::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
//	if (m_graphComplete) 
//		m_testGraph.MoveWindow(0,0,cx,cy);
	if (m_cToolBar && ::IsWindow(m_cToolBar.m_hWnd))
	{
		UINT tt = ID_TASK_CANCELTASK;
		UINT nID,nStyle;
		int nImage;
		m_cToolBar.GetButtonInfo(2,nID,nStyle,nImage);
		m_cToolBar.SetButtonInfo(2,nID,TBBS_SEPARATOR,cx-100);

	}
}

/////////////////////////////////////////////////////////////////////////////
/// The framework calls this function after receiving a DOC_UPDATE_VIEWCONTENT notification.
///
/// Called just before the view is displayed for the first time, this function just
/// assignes to the data member m_pER a pointer to the Learning Unit (see CLearningUnit)
/// associated with that view.
/////////////////////////////////////////////////////////////////////////////
LRESULT CViewPie::OnUpdateInitContent(WPARAM wp, LPARAM lp)
{
	m_pER = (CExternRepUnit*)lp;
	return 0;
}

LRESULT CViewPie::OnActivateER(WPARAM wp, LPARAM lp)
{
	static BOOL bFirst = TRUE;

	if (!bFirst) CTrace::T_SWITCH(this, (CWnd*)wp);
	bFirst = FALSE;
	return 0;
}


void CViewPie::OnInitialUpdate() 
{
	int col=0;
	m_nCurrTime=0;

	m_bComplete = true;	
	CView::OnInitialUpdate();
	if (m_pER)
		GetParentFrame()->SetWindowText(m_pER->GetName());
	CTrace::T_OPEN(GetParentFrame());
//	CSimulDoc *pDoc = GetDocument();
	m_nCurrTime = GetDocument()->m_currTimer;

	CFormatChart *pForm = GetFormats();
	if (pForm && pForm->m_nChartKind==CFormatChart::CHART_PIE)
		OnChartPie();
	else
		OnChartBar();

	
	POSITION pos = GetDocument()->m_cUserInput.GetStartPosition();
	while (pos)
	{
		CUserOutput pUser;
		int nb;
		GetDocument()->m_cUserInput.GetNextAssoc(pos,nb,pUser);
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

	m_PieChart.SetXAxisAlignment(0);
	CString strTime;
	strTime.Format(_T("Time : %d"),m_nCurrTime);
	m_PieChart.SetGraphTitle(strTime);
	m_PieChart.SetTickSpace(5);
	m_PieChart.SetTickRange(0,50);
	//OnUpdate(NULL,DOC_UPDATE_TIMERDATA,NULL);
	//GetDocument()->UpdateTimerDoc(this);
	OnUpdateTimer();

	CWnd *pWnd = GetParent();
	CRect mrect;
	if (pWnd)
	{
		pWnd->GetWindowRect(mrect);
		pWnd->SetWindowPos(NULL,0,0,mrect.Width()-1,mrect.Height(),SWP_NOZORDER|SWP_NOMOVE);
	}
}

/////////////////////////////////////////////////////////////////////////////
/// The framework calls this function after receiving a DOC_UPDATE_TIMERDATA notification.
///
/// Response to data update
/////////////////////////////////////////////////////////////////////////////
BOOL CViewPie::OnUpdateTimer()
{
	CLearningUnit *pLU = GetDocument()->GetCurrentLU();
	if (!pLU) return FALSE;
	if (!m_pER) return FALSE;

	int nbExpSet = pLU->m_cExpSet.GetSize();
	int nbOutput = m_pER->m_cOutputSet.GetSize();

	int nGraphItem = 0;
	double dMax = -1e10;
	double dMin = +1e10;
	for (int hh = 0;hh < nbOutput ;hh++)
	{
		COutcomes *pCo= m_pER->m_cOutputSet.GetAt(hh);
		//if (!pCo->m_nSelected) continue;
		if (pCo->m_nExpSet) continue;
	
		CMdData *pData = pLU->GetDataAt(pCo->m_nData);
		if (!pData) continue;

		for (int i=0;i<nbExpSet;i++)
		{
			double lMin,lMax;
			pData->GetMinMax(&lMin,&lMax,i);
			if (lMin < dMin) dMin = lMin;
			if (lMax > dMax) dMax = lMax;
			double dt = pData->GetAt(m_nCurrTime,i);
			CGraphSeries* pSerie = m_PieChart.GetSeries(i);
			if (pSerie)
				pSerie->SetData(nGraphItem,pCo->m_nData,dt);
		}
		nGraphItem++;
	}

	int nTicks = 10;
	{
		double range = dMax-dMin;
		double cc = log10(range);
		int dec = (int)floor(cc);
		double mul = pow(10.0, dec);
		int rrr = (int)(ceil(range / mul) * mul);
		nTicks = (int)(rrr / mul);
		if (nTicks<=2) nTicks*=5;
		if (nTicks<=5) nTicks*=2;

		nTicks =(int)mul;
		if (nTicks==100) nTicks/=2;
		//if (nTicks=50) nTicks/=2;
		if (!nTicks) nTicks = 1;
		int nn = (int)(dMin /mul); 
		int yMin = (int)(nn * mul);
		int idMax = (int)(ceil(dMax / mul) * mul);
		int idMin = (int)(floor(dMin / mul) * mul);
		if (idMin > dMin) idMin--;
		if (idMax < dMax) idMax++;
		dMax = idMax;
		dMin = idMin;
//		nTicks = (int)((dMax-dMin)/mul);
//	if (nTicks<=2) nTicks*=5;
//		if (nTicks<=5) nTicks*=2;
	}
	if (dMax==1 && dMin==0)
		m_PieChart.SetTickSpace(0.2/*nTicks*/);
	else
		m_PieChart.SetTickSpace(nTicks);
	m_PieChart.SetTickRange((int)dMin,(int)dMax);

	m_nMaxRange = (int)(dMax+1);
	m_nMinRange = (int)dMin;

	CString strTime;
	strTime.Format(_T("Time : %d"),m_nCurrTime);
	m_PieChart.SetGraphTitle(strTime);

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
/// The framework calls this function after receiving a DOC_UPDATE_ALLDATA notification.
///
/// Response to data update
/////////////////////////////////////////////////////////////////////////////
BOOL CViewPie::OnUpdateData()
{
	CLearningUnit *pLU = GetDocument()->GetCurrentLU();
	if (!pLU) return FALSE;
	if (!m_pER) return FALSE;

	int nbExpSet = pLU->m_cExpSet.GetSize();
	int nbOutput = m_pER->m_cOutputSet.GetSize();

	CTypedPtrArray<CObArray,CGraphSeries*> cSeriesSet;	// List of Outcomes
	for (int i=0;i<nbExpSet;i++)
	{
		CGraphSeries* series = new CGraphSeries();
		series->SetGroupIdx(i);
		CString cs = pLU->m_cExpSet.GetAt(i)->GetName();
		series->SetLabel(cs);
		cSeriesSet.Add(series);
	}
	int nGraphItem = 0;
	for (int hh = 0;hh < nbOutput ;hh++)
	{
		COutcomes *pCo= m_pER->m_cOutputSet.GetAt(hh);
		//if (!pCo->m_nSelected) continue;
		if (pCo->m_nExpSet) continue;
	
		CMdData *pData = pLU->GetDataAt(pCo->m_nData);
		if (!pData) continue;

		for (int i=0;i<nbExpSet;i++)
		{
			double dt = pData->GetAt(0,i);

			CModelObj *pObj = pData->GetDataInfo();
			
			CGraphSeries* pSerie = cSeriesSet.GetAt(i);
			pSerie->SetData(nGraphItem,pCo->m_nData,dt);
			m_PieChart.SetLegend(nGraphItem, pData->GetDataName(pLU->m_bShowAbbrev));
			m_PieChart.SetColor(nGraphItem,pObj->GetColor());
		}
		nGraphItem++;
	}
	m_PieChart.RemoveSeries();
	for (i=0;i<nbExpSet;i++)
	{
		CGraphSeries* pSerie = cSeriesSet.GetAt(i);
		if (pSerie->GetNumData())
		{
			m_PieChart.AddSeries(pSerie);
			pSerie->SetSelected();
		}
		else
			delete pSerie;
	}
	cSeriesSet.RemoveAll();
	OnUpdateTimer();
	m_PieChart.ResetSelection();

	//OnUpdate(NULL,DOC_UPDATE_TIMERDATA,NULL);
	//GetDocument()->UpdateTimerDoc(this);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
/// The framework calls this function after receiving a DOC_UPDATE_BREAKPOINT notification.
///
/// Response to data update
/////////////////////////////////////////////////////////////////////////////
BOOL CViewPie::OnUpdateBreakPoint(int nBPTime,int nBPType)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
/// The framework calls this function after receiving a TRANSLATION_ACTION notification.
///
/// Response to data update
/////////////////////////////////////////////////////////////////////////////
BOOL CViewPie::OnUpdateAction(CUserData *pData)
{
	int nbTrans = GetTranslation();
	BOOL bCanProceed = (nbTrans>=TRANS_DYNAL);

	if (pData && !pData->m_bUpdateNow)
	{
		if (!bCanProceed) return FALSE;

		for (int i=0;i<m_PieChart.GetSeriesSize();i++)
		{
			CGraphSeries* pSer =  m_PieChart.GetSeries(i);
			if (pSer && pSer->m_nGroup == pData->m_nExpSet)
			{
				int myOutcome=-1;
				int nGraphItem=0;
				int nbOutput = m_pER->m_cOutputSet.GetSize();
				for (int hh = 0;hh < nbOutput ;hh++)
				{
					COutcomes *pCo= m_pER->m_cOutputSet.GetAt(hh);
					if (pCo->m_nExpSet) continue;
		
					if (pData->m_nOutcome==pCo->m_nData)
					{
						myOutcome=nGraphItem;
						break;
					}
					nGraphItem++;

				}
				if (myOutcome!=-1) 
				{
					pSer->SetData(myOutcome,pData->m_nOutcome,pData->m_dValue);
					break;
				}
			}
		}
	}
	else
		OnUpdateData();
	OnUpdateTimer();
	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
/// The framework calls this function after receiving a TRANSLATION_HYPOTHESIS notification.
///
/// Response to data update
/////////////////////////////////////////////////////////////////////////////
BOOL CViewPie::OnUpdateHypothesis(CUserData *pData)
{
	int nbTrans = GetTranslation();
	if (nbTrans<TRANS_DYNAL) return FALSE;

	if (!m_pER) return FALSE;
	CLearningUnit *pLU = GetDocument()->GetCurrentLU();
	int nbOutput = m_pER->m_cOutputSet.GetSize();

	BOOL bCreated = FALSE;
	CGraphSeries* pSerie = NULL;

	for (int i=0;i<m_PieChart.GetSeriesSize();i++)
	{
		CGraphSeries* pSer =  m_PieChart.GetSeries(i);
		if (pSer && pSer->m_nGroup == pData->m_nExpSet)
		{
			pSerie = pSer;
			break;
		}

	}
	if (!pSerie) return FALSE;

	int nGraphItem=0;
	int myOutcome=-1;
	for (int hh = 0;hh < nbOutput ;hh++)
	{
		COutcomes *pCo= m_pER->m_cOutputSet.GetAt(hh);
		if (pCo->m_nExpSet) continue;
		
		//CMdData *pData = pLU->GetDataAt(pCo->m_nData);
		//if (!pData) continue;
		//CString name = pData->GetDataName();
		if (pData->m_nOutcome==pCo->m_nData)
		{
			myOutcome=nGraphItem;
			break;
		}
		nGraphItem++;
	}
	if (myOutcome!=-1)
	{
		pSerie->SetPrediction(myOutcome,pData->m_dValue);
	}

/*	for (int i=0;i<m_PieChart.GetSeriesSize();i++)
	{
		CGraphSeries* pSer =  m_PieChart.GetSeries(i);
		if (pSer && pSer->m_bPred && pSer->m_nGroup == pData->m_nExpSet)
		{
			pSerie = pSer;
			break;
		}

	}

	if (!pSerie)
	{
		bCreated = TRUE;
		CGraphSeries* pOldSerie =  m_PieChart.GetSeries(pData->m_nExpSet);
		pSerie = pOldSerie->Clone();
		CString cs = pLU->m_cExpSet.GetAt(pData->m_nExpSet)->GetName();
		CString strHyp;
		strHyp.Format(_T(" (T = %d)"),pData->m_nTime);
		cs += strHyp;
		pSerie->SetLabel(cs);
		pSerie->ResetDataPred();
		pSerie->SetSelected();
	}
	pSerie->m_bPred = pData->m_nTime;
	int nGraphItem=0;
	int myOutcome=-1;
	for (int hh = 0;hh < nbOutput ;hh++)
	{
		COutcomes *pCo= m_pER->m_cOutputSet.GetAt(hh);
		if (pCo->m_nExpSet) continue;
		
		//CMdData *pData = pLU->GetDataAt(pCo->m_nData);
		//if (!pData) continue;
		//CString name = pData->GetDataName();
		if (pData->m_nOutcome==pCo->m_nData)
		{
			myOutcome=nGraphItem;
			break;
		}
		nGraphItem++;
	}
	if (myOutcome!=-1)
	{
		pSerie->SetData(myOutcome,pData->m_nOutcome,pData->m_dValue);
		pSerie->SetDataPred(myOutcome,TRUE);
		if (bCreated) 
			m_PieChart.AddSeries(pSerie);
	}
	else if (bCreated) 
		delete pSerie;


	nGraphItem=0;
	myOutcome=-1;
	for (hh = 0;hh < nbOutput ;hh++)
	{
		COutcomes *pCo= m_pER->m_cOutputSet.GetAt(hh);
		if (pCo->m_nExpSet) continue;
		
		if (pCo->m_nData==pData->m_nOutcome)
		{
			myOutcome=nGraphItem;
			break;
		}
		nGraphItem++;
	}
	if (myOutcome!=-1)
	{
		m_PieChart.SetPrediction(pData->m_nExpSet,myOutcome,pData->m_dValue);
		m_PieChart.SetTickRange(m_nMinRange,max((int)(pData->m_dValue+1),m_nMaxRange));
	}*/
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
/// The framework calls this function after receiving a TRANSLATION_MAPRELATION notification.
///
/// Response to data update
/////////////////////////////////////////////////////////////////////////////
BOOL CViewPie::OnUpdateMapRelation(CTranslationInfo *mTInfo )
{
	int nbTrans = GetTranslation();
	if (nbTrans<TRANS_MAPRELAT) return FALSE;

	//m_nSelExp = m_nSelOut = -1;
	CLearningUnit *pLU = GetDocument()->GetCurrentLU();
	if (!pLU) return FALSE;
	if (!m_pER) return FALSE;

	int nbExpSet = pLU->m_cExpSet.GetSize();
	int nbOutput = m_pER->m_cOutputSet.GetSize();

	int nTime = -1;//mTInfo->m_nTime;
	int nExpSet = -1;//mTInfo->m_nExpSet;
	int nData = -1;//mTInfo->m_nData;

	//if (nTime==-1 || nExpSet==-1 || nData==-1) return;
	//if (!m_pER) return;
	if (!mTInfo)
	{
		m_PieChart.ResetSelection();
	}
	else 
	{
		nTime = mTInfo->m_nTime;
		m_nCurrTime = nTime;
		int nbD = mTInfo->m_nDatas.GetSize();

		m_PieChart.ResetSelection();
		for (int i=0;i<nbD;i++)
		{
			CTranslationInfo::CTInfo tt1 = mTInfo->m_nDatas.GetAt(i);
			nData = tt1.nData;
			nExpSet = tt1.nExpSet;
			nTime = tt1.nTime;

			m_nCurrTime = nTime;
			int nGraphItem=0;
			int myOutcome=-1;
			for (int hh = 0;hh < nbOutput ;hh++)
			{
				COutcomes *pCo= m_pER->m_cOutputSet.GetAt(hh);
				if (pCo->m_nExpSet) continue;
		
				//CMdData *pData = pLU->GetDataAt(pCo->m_nData);
				//if (!pData) continue;
				//CString name = pData->GetDataName();
				if (nData==pCo->m_nData)
				{
					myOutcome=nGraphItem;
					break;
				}
				nGraphItem++;
			}
			if (myOutcome!=-1)
			{
				m_nCurrTime = nTime;
				m_PieChart.ResetSelection(nExpSet,myOutcome,TRUE);
				//m_nSelExp = nExpSet;
				//m_nSelOut = myOutcome;
				/*for (int j=0;j<m_PieChart.GetSeriesSize();j++)
				{
					CGraphSeries* pSer =  m_PieChart.GetSeries(j);
					if (pSer && pSer->m_bPred && pSer->m_bPred==nTime)
					{
						m_PieChart.ResetSelection(nExpSet+j,myOutcome,FALSE);
					}
				}*/

				//m_PieChart.ResetSelection(nExpSet+nbExpSet,myOutcome,FALSE);
			}
		}
		//else
		//{
		//	m_nCurrTime = nTime;
		//	m_PieChart.ResetSelection();
		//}
	}

	OnUpdateTimer();
/*	//OnUpdate(NULL,DOC_UPDATE_TIMERDATA,NULL);
	//GetDocument()->UpdateTimerDoc(this);
	int nGraphItem = 0;
	double dMax = -1e10;
	double dMin = +1e10;
	for (int hh = 0;hh < nbOutput ;hh++)
	{
		COutcomes *pCo= m_pER->m_cOutputSet.GetAt(hh);
		//if (!pCo->m_nSelected) continue;
		if (pCo->m_nExpSet) continue;
	
		CMdData *pData = pLU->GetDataAt(pCo->m_nData);
		if (!pData) continue;

		for (int i=0;i<nbExpSet;i++)
		{
			double lMin,lMax;
			pData->GetMinMax(&lMin,&lMax,i);
			if (lMin < dMin) dMin = lMin;
			if (lMax > dMax) dMax = lMax;
			double dt = pData->GetAt(m_nCurrTime,i);
			CGraphSeries* pSerie = m_PieChart.GetSeries(i);
			if (pSerie)
				pSerie->SetData(nGraphItem,dt);
		}
		nGraphItem++;
	}
	m_PieChart.SetTickSpace(5);
	m_PieChart.SetTickRange((int)(dMax+1));
	CString strTime;
	strTime.Format(_T("Time : %d"),m_nCurrTime);
	m_PieChart.SetGraphTitle(strTime);
*/
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
void CViewPie::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (!m_bComplete || !m_pER) return;

	int nbTrans = GetTranslation();

	if (!lHint || lHint == DOC_UPDATE_ALLDATA)
	{
		OnUpdateData();
	}
	else if (lHint == TRANSLATION_MAPRELATION)
	{
		CTranslationInfo *mTInfo = (CTranslationInfo *)pHint;
		//if (nbTrans>=3)
			OnUpdateMapRelation(mTInfo );
		//else
		//	OnUpdateMapRelation(NULL);
	}
	else if (lHint==TRANSLATION_ACTION)
	{
		CUserData *pData = (CUserData *)pHint;
		OnUpdateAction(pData);
		//Invalidate();
	}

	if (lHint == DOC_UPDATE_TIMERDATA)
	{
		//int nRunMode = *((int*)pHint);
		m_nCurrTime = GetDocument()->m_currTimer;
		OnUpdateTimer();
		//m_PieChart.ResetSelection();
	}
	if (lHint == DOC_UPDATE_RESTARTSIMUL)
	{
		m_nCurrTime = GetDocument()->m_currTimer;
		m_PieChart.RemoveSeries();
		OnUpdateData();
		OnUpdateTimer();
		m_PieChart.ResetSelection();
	}
	if (lHint == TRANSLATION_HYPOTHESIS )
	{
		CUserData *pData = (CUserData *)pHint;
		OnUpdateHypothesis(pData);
	}


	Invalidate();
	UpdateWindow();
}

/////////////////////////////////////////////////////////////////////////////
// CViewPie message handlers
void CViewPie::OnChartBar() 
{
	// TODO: Add your command handler code here
	m_PieChart.SetGraphType(0);
	Invalidate();
}

void CViewPie::OnChartLine() 
{
	// TODO: Add your command handler code here
	m_PieChart.SetGraphType(1);
	Invalidate();
}

void CViewPie::OnChartPie() 
{
	// TODO: Add your command handler code here
	m_PieChart.SetGraphType(2);
	Invalidate();
}

void CViewPie::OnUpdateCharts(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if (!m_bComplete) return;
	int nf = m_PieChart.GetGraphType();
	int nb = pCmdUI->m_nID - ID_CHART_BAR;
	pCmdUI->SetCheck(nf == nb);	
	pCmdUI->Enable(FALSE);
}

void CViewPie::OnContextMenu(CWnd* pWnd, CPoint point) 
{
   // make sure window is active
/*   GetParentFrame()->ActivateFrame();
   CMenu menu;
   if (menu.LoadMenu(IDR_SIMULPIE))
   {
	   CMenu* pPopup = menu.GetSubMenu(3);
	   ASSERT(pPopup != NULL);
	   
	   pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON,
		   point.x, point.y,
		   AfxGetMainWnd()); // use main window for cmds
   }*/
}


void CViewPie::OnEditCopy() 
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

	m_PieChart.DrawGraph(&memDC);

	OpenClipboard() ;
	EmptyClipboard() ;
	SetClipboardData (CF_BITMAP, bitmap.GetSafeHandle() ) ;
	CloseClipboard () ;

	bitmap.Detach();

	EndWaitCursor();

}

void CViewPie::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	int nSerie=-1,
		nData=-1;
	CView::OnLButtonDblClk(nFlags, point);

	BOOL bRes = m_PieChart.OnHitTest(point,nSerie,nData);

	int oData = nData;
	int nbTrans = GetTranslation();
	if (nbTrans<TRANS_MAPRELAT)
	{
		//GetDocument()->UpdateMapRelation(NULL,NULL);
		return;
	}

	if (nSerie!=-1 && nData!=-1)
	{
		int nGraphItem=0;
		CLearningUnit *pLU = GetDocument()->GetCurrentLU();

		int nbExpSet = pLU->m_cExpSet.GetSize();
		int nbOutput = m_pER->m_cOutputSet.GetSize();

		int myOutcome=-1;

		for (int hh = 0;hh < nbOutput ;hh++)
		{
			COutcomes *pCo= m_pER->m_cOutputSet.GetAt(hh);
			if (pCo->m_nExpSet) continue;
		
			//CMdData *pData = pLU->GetDataAt(pCo->m_nData);
			//if (!pData) continue;

			//CString name = pData->GetDataName();
			if (nData==nGraphItem)
			{
				myOutcome=hh;
				break;
			}
			nGraphItem++;
		}
		if (myOutcome!=-1)
		{
			COutcomes *pCo= m_pER->m_cOutputSet.GetAt(hh);
			nData = pCo->m_nData;
			//nSerie = pCo->m_nExpSet;
		}
		//GetDocument()->UpdateAllViews(this,TRANSLATION_MAPFACT,(CObject*)&mTInfo);
		CGraphSeries *pSerie = m_PieChart.GetSeries(nSerie);
		if (pSerie->m_bPred)
		{
			m_nSelExp = m_nSelOut = -1;
			GetDocument()->UpdateMapRelation(NULL,NULL);
		}
		else
		{
			CTranslationInfo mTInfo;
			mTInfo.m_nTransLvl = GetTranslation();
			mTInfo.m_nTime=m_nCurrTime;
			mTInfo.m_nDatas.RemoveAll();
			CTranslationInfo::CTInfo tt={m_nCurrTime,nData,pSerie->m_nGroup};
			mTInfo.m_nDatas.Add(tt);
		
			GetDocument()->UpdateMapRelation(this,&mTInfo);

			if (m_nSelExp ==pSerie->m_nGroup && m_nSelOut == nData &&
				nbTrans==TRANS_DYNAL)
			{
				int nCBP = GetDocument()->m_nCurrBP;
				int nNBP = GetDocument()->m_nNextBP;
				if (nCBP != m_nCurrTime || nNBP != m_nCurrTime) return;

				CTimerBreakPoint pBP;
				CLearningUnit*	pLU = GetDocument()->GetCurrentLU();
				BOOL bEditable = pLU->GetTimerBPAt(nCBP,pBP);
				if (!bEditable) return;
				if (pBP.m_nBPType != BREAKP_ACTION &&
					pBP.m_nBPType != BREAKP_HYPOT) return;

				CUserOutput pT2;
				BOOL bOutput = GetDocument()->m_cUserInput.Lookup(m_nCurrTime,pT2);
				if (!bOutput) return;

				bEditable = pLU->GetTimerBPAt(pT2.m_nTime,pBP);
				if (!bEditable) return;

				double nValue = pSerie->GetData(oData);

				CUserData pUData;
				pUData.m_nExpSet = m_nSelExp; 
				pUData.m_nOutcome = m_nSelOut;
				pUData.m_nTime = pT2.m_nTime;//pgvItem->row-1;
				pUData.m_dOldValue = nValue;
				pUData.m_dValue = nValue;

				CEditValueDlg pDlg(this,point);
				CString name;

				CMdData *pData = pLU->GetDataAt(m_nSelOut);

				CModelObj* pObj = pData->GetDataInfo();
				CModelPar *pParObj = DYNAMIC_DOWNCAST( CModelPar, pObj);
				if (pParObj)
				{
					pDlg.m_dMin = pParObj->GetMin();
					pDlg.m_dMax = pParObj->GetMax();
				}

				if (pData) name = pData->GetDataName(pLU->m_bShowAbbrev);
				pDlg.m_strName = name;

				pDlg.m_strOldV.Format(_T("%.4g"),pUData.m_dOldValue);
				pDlg.m_strValue.Format(_T("%.4g"),pUData.m_dValue);
				if (pDlg.DoModal()!=IDOK) return;

				CString mstr = pDlg.m_strValue;
				double nVal = atof(mstr.GetBuffer(mstr.GetLength()));
				//pUData.m_dOldValue = nValue;
				pUData.m_dValue = nVal;

				GetDocument()->UpdateUserData(this,&pUData);
			}
			else
			{
				m_nSelExp = pSerie->m_nGroup;
				m_nSelOut = nData;
			}
		}
	}
	else
	{
		m_nSelExp = m_nSelOut = -1;
		GetDocument()->UpdateMapRelation(NULL,NULL);
	}

	if (bRes)
	{
		Invalidate();
		UpdateWindow();
	}
}

void CViewPie::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
//	if (m_PieChart.ResetSelection())
//	{
//		Invalidate();
//		UpdateWindow();
//	}
	CView::OnLButtonDown(nFlags, point);
}

void CViewPie::OnTranslationTasks(UINT nID)
{
	// TODO: Add your command handler code here
	switch (nID) {
	case ID_TASK_MAPFACT:
	case ID_TASK_MAPRELATION:
	case ID_TASK_SUPPORTACTION:
	//	m_nCurrTTask = nID - ID_TASK_MAPFACT+1;
		break;
	case ID_TASK_CANCELTASK:
	default:
		//m_nCurrTTask = 0;
		break;
	}

}

void CViewPie::OnUpdateTransTasks(CCmdUI* pCmdUI) 
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

void CViewPie::OnDestroy() 
{
	CTrace::T_CLOSE(GetParentFrame());
	CView::OnDestroy();
	
	// TODO: Add your message handler code here
	
}
