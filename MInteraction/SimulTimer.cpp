// SimulTimer.cpp : implementation file
//

#include "stdafx.h"
#include "Simul.h"

#include "LearnerTrace.h"

#include <MInstruction\LearningUnit.h>

#include "simulDoc.h"
#include "SimulTimer.h"

#include <Prefs\Pref.h>

#include "Format.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	TIMER_RUN		120
#define TIMER_REVIEW	130
//SYSTEMTIME lpTime1,lpTime2;

/////////////////////////////////////////////////////////////////////////////
// CViewTimer

IMPLEMENT_DYNCREATE(CViewTimer, CFormView)

CViewTimer::CViewTimer()
	: CFormView(CViewTimer::IDD)
{
	//{{AFX_DATA_INIT(CViewTimer)
	//}}AFX_DATA_INIT

//	m_hIRun = NULL;
///	m_hIEnd = NULL;
//	m_hIBeg = NULL;
	m_pER = NULL;
	m_bIsRunning=FALSE;
	m_nBackReview = 0;
//	m_nCurrTTask=0;
}

CViewTimer::~CViewTimer()
{
	CTrace::T_CLOSE(GetParentFrame());
}

void CViewTimer::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CViewTimer)
	DDX_Control(pDX, IDC_TIMESLIDER, m_Slider);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CViewTimer, CFormView)
	//{{AFX_MSG_MAP(CViewTimer)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_TIMESLIDER, OnReleasedSlider)
	ON_NOTIFY(TB_THUMBTRACK, IDC_TIMESLIDER, OnTrackTimer)
	ON_WM_HSCROLL()
	ON_WM_GETMINMAXINFO()
	ON_WM_VSCROLL()
	ON_WM_MOVE()
	ON_WM_CREATE()
	ON_WM_KEYDOWN()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_MESSAGE(TRACE_VIEW_ACTIVATE, OnActivateER)
	ON_MESSAGE(DOC_UPDATE_VIEWCONTENT, OnUpdateInitContent)
	//ON_COMMAND_RANGE(ID_TRANS_INDEPENDENT,ID_TRANS_DYNALINKED,OnTranslationTasks)
	ON_UPDATE_COMMAND_UI_RANGE(ID_TRANS_INDEPENDENT,ID_TRANS_DYNALINKED,OnUpdateTransTasks)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewTimer diagnostics

#ifdef _DEBUG
void CViewTimer::AssertValid() const
{
	CFormView::AssertValid();
}

void CViewTimer::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CSimulDoc* CViewTimer::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSimulDoc)));
	return (CSimulDoc*)m_pDocument;
}
#endif //_DEBUG

int	CViewTimer::GetTranslation()
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
// CViewTimer message handlers

/////////////////////////////////////////////////////////////////////////////
/// The framework calls this function after receiving a DOC_UPDATE_VIEWCONTENT notification.
///
/// Called just before the view is displayed for the first time, this function just
/// assignes to the data member m_pER a pointer to the Learning Unit (see CLearningUnit)
/// associated with that view.
/////////////////////////////////////////////////////////////////////////////
LRESULT CViewTimer::OnUpdateInitContent(WPARAM wp, LPARAM lp)
{
	m_pER = (CExternRepUnit*)lp;
	return 0;
}

void CViewTimer::OnTrackTimer(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}

void CViewTimer::OnReleasedSlider(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}

LRESULT CViewTimer::OnActivateER(WPARAM wp, LPARAM lp)
{
	static BOOL bFirst = TRUE;

	if (!bFirst) CTrace::T_SWITCH(this, (CWnd*)wp);
	bFirst = FALSE;
	return 0;
}

void CViewTimer::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	if (m_pER)
		GetParentFrame()->SetWindowText(m_pER->GetName());
	CTrace::T_OPEN(GetParentFrame());

	m_Slider.SetRange(0, GetDocument()->GetMaxTime(), true);
	m_Slider.SetPos(0);

	m_Slider.SetPastColour(CPref::g_crPast);
	m_Slider.SetFutureColour(CPref::g_crFuture);


	CLearningUnit *pLU = GetDocument()->GetCurrentLU();
	if (pLU)
	{
		int nbBP = pLU->m_cTBreakSet.GetSize();
		if (nbBP)
		{
			m_Slider.SetCurrentBreakPoint(-1);
			for (int i=0;i<nbBP;i++)
			{
				CTimerBreakPoint *pBP = pLU->m_cTBreakSet.GetAt(i);
				if (pBP)
					m_Slider.AddBreakPoint(pBP->m_tBreakPt);
			}
		}
	}

	int range = GetDocument()->GetMaxTime();

	int interval = 1;
	if(range%50 == 0 && range != 50)
		interval = 50;
	if(range%10 == 0 && range != 10)
		interval = 10;
	else if(range%7 == 0 && range != 7)
		interval = 7;
	else if(range%5 == 0 && range != 5)
		interval = 5;
	else if(range%4 == 0 && range != 4)
		interval = 4;
	else if(range%3 == 0 && range != 3)
		interval = 3;
	else if(range%2 == 0 && range != 2)
		interval = 2;
	m_Slider.SetTicFreq(interval);	

	//m_Slider.SetTicFreq(50);

	CRect rect,rect2;
	CWnd *par = GetParent( );
	if (!par) return;
	
	par->GetWindowRect(&rect);
	par->GetParent()->GetWindowRect(&rect2);
	par->MoveWindow(rect.left-rect2.left-2,rect.top-rect2.top-2,
		350,102);
	
	int nbTr = GetTranslation();
	m_Slider.SetSelection(0,GetDocument()->m_runTimer);
	m_Slider.SetPos(GetDocument()->m_currTimer);
	m_Slider.EnableWindow(FALSE);//nbTr);
	m_nMaxTime = 0;
//	m_nBreakPoint = 0;
	/*	SetWindowPos(&wndTopMost,0,0,0,0,SWP_NOMOVE   |SWP_NOSIZE   );*/
//	OnTimerStop();
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
void CViewTimer::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (lHint == DOC_UPDATE_TIMERDATA)
	{
		int nb = GetDocument()->m_currTimer;
		m_Slider.SetPos(nb);
		m_Slider.Invalidate();
		m_Slider.SetSelection(0,GetDocument()->m_runTimer);
	}
	else if (lHint == DOC_UPDATE_RESTARTSIMUL)
	{
		int nb = GetDocument()->m_currTimer;
		int nbR = GetDocument()->GetCurrTime();
		m_nMaxTime = 0;
		m_Slider.SetPos(nb);
		m_Slider.SetSelection(0,nbR);
		m_Slider.Invalidate();
	}
	else if (/*(lHint == TRANSLATION_MAPFACT) || */
		(lHint == TRANSLATION_MAPRELATION))
	{
		if (!pHint) return;
		int nTranLevel = GetTranslation();
		CTranslationInfo *mTInfo = (CTranslationInfo *)pHint;
		int nTLevel = -1;
		int nTime = -1;
		int nExpSet = -1;//mTInfo->m_nExpSet;
		int nData = -1;//mTInfo->m_nData;
		if (mTInfo)
		{
			nTLevel = mTInfo->m_nTransLvl;
			nTime = mTInfo->m_nTime;
			int nbD = mTInfo->m_nDatas.GetSize();
			for (int jj=0;jj<nbD;jj++)
			{
				CTranslationInfo::CTInfo tt1 = mTInfo->m_nDatas.GetAt(jj);
				nTime = tt1.nTime;
				//nExpSet = tt1.nExpSet;
				//nData = tt1.nData;
			}
		}
		if (nTime!=-1)
		{
			m_Slider.SetPos(nTime);
			//m_Slider.SetSelection(0,nbR);
			m_Slider.Invalidate();
		}
	}
	
}



void CViewTimer::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	// TODO: Add your message handler code here and/or call default
	
	CFormView::OnGetMinMaxInfo(lpMMI);

	CRect cr;

	GetClientRect(cr);

	lpMMI->ptMinTrackSize.x = 350;
	lpMMI->ptMinTrackSize.y = 190;

	GetWindowRect(cr);
	m_Slider.GetClientRect(cr);

	CRect rect,rect2;
	CWnd *par = GetParent( );
	if (!par) return;
	
	par->GetWindowRect(&rect);
	par->GetParent()->GetWindowRect(&rect2);
	par->MoveWindow(rect.left-rect2.left-2,rect.top-rect2.top-2,
		350,102);
	//m_Current.MoveWindow(320,5,20,30);

}

void CViewTimer::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	
	CFormView::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CViewTimer::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	
	int nb = m_Slider.GetPos();
	CFormView::OnHScroll(nSBCode, nPos, pScrollBar);
	nb = m_Slider.GetPos();
	m_Slider.VerifyPos();
	nb = m_Slider.GetPos();

	GetDocument()->m_currTimer = nb;
	GetDocument()->UpdateTimerDoc(this);
}

void CViewTimer::OnMove(int x, int y) 
{
	CFormView::OnMove(x, y);
	
	// TODO: Add your message handler code here
	
}

int CViewTimer::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CFormView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
//	if (!m_cToolBar.CreateEx(GetParent(), TBSTYLE_FLAT | TBSTYLE_TOOLTIPS, 
//			WS_CHILD | WS_VISIBLE | CBRS_TOP | 
//			/* CBRS_GRIPPER | */CBRS_TOOLTIPS | CBRS_FLYBY | CCS_ADJUSTABLE,
//			CRect(0, 0, 0, 0), 120) ||
//		!m_cToolBar.LoadToolBar(IDR_SIMULTIMER))
//	{
//		TRACE0("Failed to create toolbar1\n");
//		return -1;      // fail to create  
//	}
	
	return 0;
}

void CViewTimer::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	int r=0;
	switch (nChar)
	{
	case VK_RETURN:
		r++;
		break;
	}
	CFormView::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CViewTimer::OnDestroy() 
{
	CTrace::T_CLOSE(GetParentFrame());
	CFormView::OnDestroy();
	
	// TODO: Add your message handler code here
	
}
void CViewTimer::OnTranslationTasks(UINT nID)
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

void CViewTimer::OnUpdateTransTasks(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	BOOL	bEnab = FALSE;
	BOOL	bCheck = FALSE;
	UINT	nT = GetTranslation();
//	//UINT	nCT = m_nCurrTTask;

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
