// ViewController.cpp : implementation file
//

#include "stdafx.h"
#include "simul.h"

#include "LearnerTrace.h"
#include "simulDoc.h"
#include "ViewController.h"

#include <MInstruction\LearningUnit.h>
#include <MSimulation\Model.h>

#include <Prefs\Pref.h>

#include "Float.H"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define	TIMER_RUN		120
#define TIMER_REVIEW	130

#define TIMER_ELAPSE_AUTH	CPref::g_nSpeedAcc
#define TIMER_ELAPSE_LEARN	CPref::g_nSpeedNorm

/////////////////////////////////////////////////////////////////////////////
// CViewController

IMPLEMENT_DYNCREATE(CViewController, CFormView)

CViewController::CViewController()
	: CFormView(CViewController::IDD)
{
	//{{AFX_DATA_INIT(CViewController)
	//}}AFX_DATA_INIT
	m_bIsRunning  = FALSE;
	m_bIsExpanded = FALSE;
	m_bIsLocked = FALSE;
	//GetDocument()->m_nRunMode = 0;
	m_ImageList.Create(IDB_VAR_ICONS,14,0,RGB(128,128,0));
	m_ImageTaskList.Create(IDB_TIME_BREAKPOINT,16,0,RGB(255,0,255));
	m_ImageRunList.Create(IDB_CONTR_TIMER,12,0,RGB(255,0,255));
	m_ImageUnitList.Create(IDB_CONTROLLER,16,0,RGB(192,192,192));
	m_ImageAddBPList.Create(IDB_TIME_ADDBP,21,0,RGB(255,0,255));

 	m_cExpand.SetBitmapIDs(IDB_PUSHPIN);
}

void CViewController::OnDestroy() 
{
	CTrace::T_CLOSE(GetParentFrame());
	CFormView::OnDestroy();
}

CViewController::~CViewController()
{
	CTrace::T_CLOSE(GetParentFrame());
}

LRESULT CViewController::OnActivateER(WPARAM wp, LPARAM lp)
{
	static BOOL bFirst = TRUE;

	if (!bFirst) CTrace::T_SWITCH(this, (CWnd*)wp);
	bFirst = FALSE;
	return 0;
}

BOOL MyAfxSimpleFloatParse(LPCTSTR lpszText, double& d)
{
	ASSERT(lpszText != NULL);
	while (*lpszText == ' ' || *lpszText == '\t')
		lpszText++;

	TCHAR chFirst = lpszText[0];
	d = _tcstod(lpszText, (LPTSTR*)&lpszText);
	if (d == 0.0 && chFirst != '0')
		return FALSE;   // could not convert
	while (*lpszText == ' ' || *lpszText == '\t')
		lpszText++;

	if (*lpszText != '\0')
		return FALSE;   // not terminated properly

	return TRUE;
}

void CViewController::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CViewController)
	DDX_Control(pDX, IDC_RESET, m_cReset);
	DDX_Control(pDX, IDC_APPLY, m_cApply);
	DDX_Control(pDX, IDC_NEXTUNIT, m_cNextUnit);
	DDX_Control(pDX, IDC_TASK, m_cTask);
	DDX_Control(pDX, IDC_STOP, m_cStop);
	DDX_Control(pDX, IDC_START, m_cStart);
	DDX_Control(pDX, IDC_RESTART, m_cRestart);
	DDX_Control(pDX, IDC_RUN, m_cRun);
	DDX_Control(pDX, IDC_PREV, m_cPrev);
	DDX_Control(pDX, IDC_NEXT, m_cNext);
	DDX_Control(pDX, IDC_END, m_cEnd);
	DDX_Control(pDX, IDC_BEGIN, m_cBegin);
	DDX_Control(pDX, IDC_NEXTSTEP, m_cNextStep);
	DDX_Control(pDX, IDC_PREVSTEP, m_cPrevStep);
	DDX_Control(pDX, IDC_EXPAND, m_cExpand);
	DDX_Control(pDX, IDC_NEWBP_A, m_cAddBPA);
	DDX_Control(pDX, IDC_NEWBP_H, m_cAddBPH);
	//}}AFX_DATA_MAP
	DDX_GridControl(pDX, IDC_EXPSET_PARAM, m_Parlist);

	if (pDX->m_bSaveAndValidate)
	{
		CLearningUnit*	pLU = GetDocument()->GetCurrentLU();
		if (!pLU) return;
		CMdEquation*	m_pEqu = GetDocument()->GetCurrentModel();
		if (!m_pEqu) return;

		int nbC = m_Parlist.GetColumnCount();
		int nbR = m_Parlist.GetRowCount();
		for (int i=1;i<nbC;i++)
			for (int j=1;j<nbR;j++)
			{
				LPARAM nItem = m_Parlist.GetItemData(j,i);
				UINT   nState = m_Parlist.GetItemState(j,i);
				BOOL bMod = nState & GVIS_MODIFIED;
				//bMod = nState | GVIS_MODIFIED;
				CModelPar *pPar= m_pEqu->m_cParSet.GetAt((int)nItem);
				if (pPar)
				{
					CString mstr = m_Parlist.GetItemText(j,i);
					double val = atol(mstr.GetBuffer(mstr.GetLength()));
					BOOL mb = MyAfxSimpleFloatParse(mstr,val);
					if (!mb)
					{
						m_Parlist.SetFocusCell(j,i);
						AfxMessageBox(AFX_IDP_PARSE_REAL);
						pDX->Fail();            // throws exception
					}
					double min = pPar->GetMin();
					double max = pPar->GetMax();
					//DDV_MinMaxDouble(pDX, val, min, max);

					if (val < min || val > max)
					{
						//_AfxFailMinMaxReal(pDX, (double)minVal, (double)maxVal, DBL_DIG,
						//AFX_IDP_PARSE_REAL_RANGE);
						m_Parlist.SetFocusCell(j,i);
						int precision = DBL_DIG;
						UINT nIDPrompt = AFX_IDP_PARSE_REAL_RANGE;
						TCHAR szMin[32], szMax[32];
						CString prompt;

						_stprintf(szMin, _T("%.*g"), precision, min);
						_stprintf(szMax, _T("%.*g"), precision, max);
						AfxFormatString2(prompt, nIDPrompt, szMin, szMax);

						AfxMessageBox(prompt, MB_ICONEXCLAMATION, nIDPrompt);
						prompt.Empty(); // exception prep
						pDX->Fail();	
					}

					CMdExpertSet*	pExpSet = pLU->m_cExpSet.GetAt(i-1);
					double ddd = pExpSet->GetValue(nItem);
					pExpSet->SetValue(nItem,val);

				}

			}
		m_Parlist.SetFocusCell(-1,-1);
	}
}


BEGIN_MESSAGE_MAP(CViewController, CFormView)
	//{{AFX_MSG_MAP(CViewController)
	ON_BN_CLICKED(IDC_START, OnTimerStart)
	ON_BN_CLICKED(IDC_STOP, OnTimerStop)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_PREV, OnReviewPrev)
	ON_BN_CLICKED(IDC_RUN, OnReviewStop)
	ON_BN_CLICKED(IDC_NEXT, OnReviewNext)
	ON_BN_CLICKED(IDC_BEGIN, OnReviewBegin)
	ON_BN_CLICKED(IDC_END, OnReviewEnd)
	ON_BN_CLICKED(IDC_EXPAND, OnExpandDlg)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_APPLY, OnExpSetApply)
	ON_BN_CLICKED(IDC_RESET, OnExpSetReset)
	ON_BN_CLICKED(IDC_ADD, OnExpSetAdd)
	ON_BN_CLICKED(IDC_DELETE, OnExpSetDelete)
	ON_COMMAND(ID_TIMER_RESTART, OnTimerRestart)
	ON_BN_CLICKED(IDC_TASK, OnReadTask)
	ON_COMMAND(ID_CONTROLLER_NEXTUNIT, OnNextUnit)
	ON_UPDATE_COMMAND_UI(ID_CONTROLLER_NEXTUNIT, OnUpdateNextUnit)
	ON_BN_CLICKED(IDC_PREVSTEP, OnReviewPrevStep)
	ON_BN_CLICKED(IDC_NEXTSTEP, OnReviewNextStep)
	ON_UPDATE_COMMAND_UI(ID_PARAMETERS_UPDATECHANGES, OnUpdateApplyChange)
	ON_WM_CLOSE()
	ON_COMMAND(ID_TIMER_PLAY, OnTimerStart)
	ON_COMMAND(ID_TIMER_STOP, OnTimerStop)
	ON_COMMAND(ID_PARAMETERS_UPDATECHANGES, OnExpSetApply)
	ON_COMMAND(ID_PARAMETERS_RESETVALUES, OnExpSetReset)
	ON_COMMAND(ID_PARAMETERS_REMOVEEXPSET, OnExpSetDelete)
	ON_COMMAND(ID_PARAMETERS_ADDEXPSET, OnExpSetAdd)
	ON_BN_CLICKED(IDC_RESTART, OnTimerRestart)
	ON_UPDATE_COMMAND_UI(ID_PARAMETERS_RESETVALUES, OnUpdateApplyChange)
	ON_UPDATE_COMMAND_UI(ID_PARAMETERS_ADDEXPSET, OnUpdateApplyChange)
	ON_UPDATE_COMMAND_UI(ID_PARAMETERS_REMOVEEXPSET, OnUpdateApplyChange)
	ON_BN_CLICKED(IDC_NEXTUNIT, OnNextUnit)
	ON_BN_CLICKED(IDC_NEWBP, OnNewBP)
	//}}AFX_MSG_MAP
	ON_MESSAGE(TRACE_VIEW_ACTIVATE, OnActivateER)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewController diagnostics

#ifdef _DEBUG
void CViewController::AssertValid() const
{
	CFormView::AssertValid();
}

void CViewController::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CSimulDoc* CViewController::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSimulDoc)));
	return (CSimulDoc*)m_pDocument;
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CViewController message handlers


/////////////////////////////////////////////////////////////////////////////
/// Used by the framework to enable/disable the various controls in the Controller.
/////////////////////////////////////////////////////////////////////////////
void CViewController::EnableButtons()
{
	int nCT = GetDocument()->GetCurrTime();
	int nRT = GetDocument()->GetRunTime();
	int nMT = GetDocument()->GetMaxTime();
	int nNBP = GetDocument()->m_nNextBP;

	CLearningUnit*	pLU = GetDocument()->GetCurrentLU();
	BOOL	bAddTask = FALSE;
	if (pLU)
		bAddTask = pLU->m_bActRequest;


	BOOL bIsStop = GetDocument()->m_nRunMode == RUN_NONE;
	BOOL bIsRev = GetDocument()->m_nRunMode > RUN_RUN;
	BOOL bIsRun = GetDocument()->m_nRunMode == RUN_RUN;
	if (nRT != nCT)
		bIsStop = FALSE;
	else if (nRT == nNBP)
		bIsStop &= GetDocument()->m_bTaskRead;
	else if (nCT==nRT && nCT==nMT)
		bIsStop = FALSE;
	else
		bIsStop &= TRUE;

	// START Button
	m_cStart.EnableWindow(bIsStop);
	// STOP Button
	m_cStop.EnableWindow(!bIsStop);
	// REVIEW Buttons
	m_cPrev.EnableWindow(!bIsRun && nRT);
	m_cNext.EnableWindow(!bIsRun && nRT);
	m_cRun.EnableWindow(!bIsRun && nRT);
	m_cBegin.EnableWindow(!bIsRun && nRT);
	m_cEnd.EnableWindow(!bIsRun && nRT);
	m_cPrevStep.EnableWindow(!bIsRun && nRT);
	m_cNextStep.EnableWindow(!bIsRun && nRT);
	// TASK Button
	m_cTask.EnableWindow(nRT == nNBP);

	m_cNextUnit.EnableWindow(nRT == nMT);

	m_cAddBPA.EnableWindow(bAddTask && bIsStop && nCT && (nCT==nRT) && (nCT!=nNBP));
	m_cAddBPH.EnableWindow(bAddTask && bIsStop && nCT && (nCT==nRT) && (nCT!=nNBP));
}

void CViewController::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	CTrace::T_OPEN(GetParentFrame());

	GetDocument()->m_nRunMode = RUN_NONE;

	CTimerBreakPoint pBP;
	CLearningUnit*	pLU = GetDocument()->GetCurrentLU();
	BOOL bEditable = pLU->GetTimerBPAt(GetDocument()->m_nNextBP,pBP);
	if (bEditable)
		m_cTask.SetIcon(m_ImageTaskList.ExtractIcon(pBP.m_nBPType));
	else
		m_cTask.SetIcon(m_ImageTaskList.ExtractIcon(0));
	m_cTask.SetAlign(CButtonST::ST_ALIGN_VERT);
	m_cTask.SetTooltipText(ID_CONTR_TASK);

	m_cNextUnit.SetIcon(m_ImageUnitList.ExtractIcon(2));
	m_cNextUnit.SetAlign(CButtonST::ST_ALIGN_HORIZ);
	m_cNextUnit.SetTooltipText(ID_WELCOME_NEXT);

	m_cAddBPA.SetIcon(m_ImageAddBPList.ExtractIcon(0));
	m_cAddBPA.SetAlign(CButtonST::ST_ALIGN_VERT);
	m_cAddBPA.SetShowText(FALSE);
	m_cAddBPA.SetTooltipText(ID_CONTR_ADDA);
	m_cAddBPH.SetIcon(m_ImageAddBPList.ExtractIcon(1));
	m_cAddBPH.SetAlign(CButtonST::ST_ALIGN_VERT);
	m_cAddBPH.SetShowText(FALSE);
	m_cAddBPH.SetTooltipText(ID_CONTR_ADDH);

	

	m_cStop.SetIcon(m_ImageRunList.ExtractIcon(5));
	m_cStop.SetTooltipText(ID_CONTR_STOP);
	m_cStart.SetIcon(m_ImageRunList.ExtractIcon(4));
	m_cStart.SetTooltipText(ID_CONTR_START);
	m_cRestart.SetIcon(m_ImageRunList.ExtractIcon(7));
	m_cRestart.SetTooltipText(ID_CONTR_RESTART);
	//m_cRestart.SetFlat(FALSE);

	m_cApply.SetIcon(m_ImageUnitList.ExtractIcon(3));
	m_cApply.SetAlign(CButtonST::ST_ALIGN_HORIZ);
	m_cReset.SetIcon(m_ImageUnitList.ExtractIcon(4));
	m_cReset.SetAlign(CButtonST::ST_ALIGN_HORIZ);

	m_cPrev.SetIcon(m_ImageRunList.ExtractIcon(2));
	m_cPrev.SetTooltipText(ID_CONTR_BACK);
	m_cNext.SetIcon(m_ImageRunList.ExtractIcon(3));
	m_cNext.SetTooltipText(ID_CONTR_FOR);
	m_cRun.SetIcon(m_ImageRunList.ExtractIcon(6));
	m_cRun.SetTooltipText(ID_CONTR_PAUSE);
	m_cBegin.SetIcon(m_ImageRunList.ExtractIcon(0));
	m_cBegin.SetTooltipText(ID_CONTR_BEGIN);
	m_cEnd.SetIcon(m_ImageRunList.ExtractIcon(1));
	m_cEnd.SetTooltipText(ID_CONTR_END);

	m_cPrevStep.SetIcon(m_ImageRunList.ExtractIcon(8));
	m_cPrevStep.SetTooltipText(ID_CONTR_STEPP);
	m_cNextStep.SetIcon(m_ImageRunList.ExtractIcon(9));
	m_cNextStep.SetTooltipText(ID_CONTR_STEPN);

	m_cPrev.SetShowText(FALSE);
	m_cNext.SetShowText(FALSE);
	m_cPrevStep.SetShowText(FALSE);
	m_cNextStep.SetShowText(FALSE);
	m_cRun.SetShowText(FALSE);
	m_cBegin.SetShowText(FALSE);
	m_cEnd.SetShowText(FALSE);

	m_nMaxTime = GetDocument()->m_runTimer;//max(m_nMaxTime,GetDocument()->m_currTimer);

	// update Parameters
	m_Parlist.SetTextBkColor(RGB(0xFF, 0xFF, 0xE0));
	m_Parlist.EnableSelection(FALSE);
	m_Parlist.SetRowResize(FALSE);
	m_Parlist.SetColumnResize(TRUE);
	m_Parlist.SetImageList(&m_ImageList);


	// Update Form
	GetDeviceScrollSizes(m_nMapMode,m_siTotal,m_siPage,m_siLine);
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit(FALSE);

	// Update Timer
	OnTimerInit();
	if (GetDocument()->m_currTimer)
	{
		m_bIsLocked = TRUE;
		OnUpdate(NULL,0,NULL);
	}
	m_bIsExpanded=FALSE;
	m_cExpand.SetPinned(FALSE);
	OnExpandDlg();

	//CLearningUnit*	pLU = GetDocument()->GetCurrentLU();
	//if (!pLU) return;

	if (CPref::g_nMode)
		m_cExpand.EnableWindow(!pLU->m_bExpSetLocked);

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
void CViewController::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	// TODO: Add your specialized code here and/or call the base class
	CSimulDoc *pDoc = GetDocument();
	if (!pDoc) return; 

	if (lHint == DOC_UPDATE_TIMERDATA || lHint == TRANSLATION_MAPRELATION)
	{
		int nb = GetDocument()->m_currTimer;
		EnableButtons();
		return;
	}
	else if (lHint == DOC_UPDATE_TIMERSTOP)
	{
	}
	else if (lHint == DOC_UPDATE_RESTARTSIMUL)
	{
		CTrace::g_bTrace = FALSE;
		OnTimerStop();
		int nb = GetDocument()->m_currTimer;
		int nbR = GetDocument()->GetCurrTime();
		m_nMaxTime = 0;

		if (m_bIsLocked)
			m_bIsLocked = FALSE;
		GetDocument()->m_bTaskDone = TRUE;
		GetDocument()->m_nCurrBP = -1;

		OnTimerInit();
		CTrace::g_bTrace = TRUE;
		CTrace::T_INITAT(GetDocument()->m_currTimer,GetDocument()->m_runTimer,GetDocument()->GetMaxTime());
	}
	else if (LOWORD(lHint) == DOC_UPDATE_ER)
	{
		UINT nState = m_Parlist.GetItemState(1,1);
		m_Parlist.SetItemState(1,1,nState | GVIS_FIXED);
		return;
	}
	
	CMdEquation*	m_pEqu = GetDocument()->GetCurrentModel();
	if (!m_pEqu) return;
	CLearningUnit*	pLU = GetDocument()->GetCurrentLU();
	if (!pLU) return;


	int nbExpSet =  pLU->m_cExpSet.GetSize();
	int colN,colD,colF;

	colN = nbExpSet;
	colD = 0;
	colF = nbExpSet;

	m_Parlist.SetColumnCount(1+colN);
	m_Parlist.SetFixedColumnCount(1);

	CMdExpertSet*	m_pExpSet = pLU->m_cExpSet.GetAt(0);
	int nbparam = m_pExpSet->GetSize();
	int nbinit = 0;
	int nb2 = m_pEqu->m_cParSet.GetSize();

	for (int row = 0; row < nb2; row++)
	{
		CModelPar *pPar= m_pEqu->m_cParSet.GetAt(row);
		if (pPar->m_InitValue != -1)
		{
			nbparam--;
			nbinit++;
		}
	}

	m_Parlist.SetRowCount(nbparam+nbinit+1);
	m_Parlist.SetFixedRowCount(1);

	for (int col = colD;col < colF; col++)
	{
		CMdExpertSet*	pExpSet = pLU->m_cExpSet.GetAt(col);
		DWORD dwTextStyle = DT_LEFT|DT_VCENTER|DT_SINGLELINE;
		int row1=1,row2=1;
		for (row = 0; row < nb2; row++)
		{
			CModelPar *pPar= m_pEqu->m_cParSet.GetAt(row);
			CString name = pPar->GetFormatName(pLU->m_bShowAbbrev);
			double	val = pExpSet->GetValue(row);

			GV_ITEM Item,Item2,Item3;
			Item.mask = GVIF_TEXT|GVIF_FORMAT|GVIF_PARAM;
			Item.nFormat = dwTextStyle;
			Item.lParam = row;
			Item2 = Item;
			Item3 = Item;

			Item.col = 0;
			Item.strText = name;
			Item.mask |= GVIF_IMAGE;


			Item2.col = col-colD+1;
			name.Format(_T("%.4g"),val);
			Item2.strText = name;

			Item3.col = col-colD+1;
			Item3.row = 0;
			Item3.strText = pExpSet->GetName();

			Item.row = row1;
			Item2.row = row1;
			Item.iImage = pPar->GetIcon();
			/*if (pPar->m_InitValue != -1)
				Item.iImage = 2;
			else
				Item.iImage = 1;*/
			if (m_bIsLocked)
			{
				Item2.mask |= GVIF_STATE | GVIF_FGCLR;
				Item2.nState = GVNI_READONLY;
				Item2.crFgClr = RGB(128,128,128);
			}
			else
			{
				Item2.mask |= GVIF_STATE | GVIF_FGCLR;
				Item2.nState = 0;
				Item2.crFgClr = CLR_DEFAULT;
			}
			m_Parlist.SetItem(&Item);
			m_Parlist.SetItem(&Item2);
			m_Parlist.SetItem(&Item3);

			row1++;
		}

	}

	CWnd *pWnd = GetDlgItem(IDC_APPLY);
	if (pWnd) pWnd->EnableWindow(!m_bIsLocked);
	pWnd = GetDlgItem(IDC_RESET);
	if (pWnd) pWnd->EnableWindow(!m_bIsLocked);
	pWnd = GetDlgItem(IDC_ADD);
	if (pWnd) pWnd->EnableWindow(!m_bIsLocked);
	pWnd = GetDlgItem(IDC_DELETE);
	if (pWnd) pWnd->EnableWindow(!m_bIsLocked);

	if (LOWORD(lHint) != DOC_UPDATE_ER)
	{
		m_Parlist.AutoSizeColumn(0);
		int s2 = m_Parlist.GetColumnWidth(0) + 15;
		m_Parlist.SetColumnWidth(0,s2);

	//	m_Parlist.ExpandColumnsToFit();
		for (int i=0;i<m_Parlist.GetRowCount();i++)
			m_Parlist.SetRowHeight(i,17);

		m_Parlist.Invalidate();

		m_Parlist.AutoSizeColumns();

		//m_Parlist.AutoSizeColumn(0);
		s2 = m_Parlist.GetColumnWidth(0) + 15;
		m_Parlist.SetColumnWidth(0,s2);
	}
}

///////////////////////////////////////
// RESTART THE SIMULATION
///////////////////////////////////////
void CViewController::OnTimerRestart() 
{
	// TODO: Add your command handler code here
	GetDocument()->UpdateRestartSimul();	
}

///////////////////////////////////////
// REINITIALISE THE SIMULATION
///////////////////////////////////////
void CViewController::OnTimerInit() 
{
	// TODO: Add your control notification handler code here
	KillTimer(TIMER_RUN);
	KillTimer(TIMER_REVIEW);

	m_bIsRunning = FALSE;
	GetDocument()->m_nRunMode = RUN_NONE;
	m_cStop.ShowWindow(SW_HIDE);
	m_cStart.ShowWindow(SW_SHOW);
	EnableButtons();
}


///////////////////////////////////////
// START THE SIMULATION
///////////////////////////////////////
void CViewController::OnTimerStart() 
{
	// TODO: Add your control notification handler code here
	CSimulDoc *pDoc = GetDocument();

	if (pDoc->m_currTimer == pDoc->GetMaxTime()-1)
		pDoc->m_currTimer =0;

	
	if (CPref::g_nMode)
		SetTimer(TIMER_RUN,TIMER_ELAPSE_LEARN,NULL);
	else
		SetTimer(TIMER_RUN,TIMER_ELAPSE_AUTH,NULL);

	KillTimer(TIMER_REVIEW);

	if (!m_bIsLocked)
	{
		m_bIsLocked = TRUE;
		OnUpdate(NULL,0,NULL);
	}

	m_cStop.ShowWindow(SW_SHOW);
	m_cStart.ShowWindow(SW_HIDE);
	EnableButtons();

	m_bIsRunning = TRUE;
	GetDocument()->m_nRunMode = RUN_RUN;

	CLearningUnit*	pLU = GetDocument()->GetCurrentLU();
	BOOL bDoBP = !pLU->m_bFirstRun || (pLU->m_bFirstRun && !GetDocument()->m_bFirstRun);
	if (!GetDocument()->m_bTaskDone && bDoBP)
	{
		OnPrepareNextBP();
	}
	GetDocument()->m_bTaskDone = TRUE;

	CTrace::T_RUN(GetDocument()->m_currTimer,GetDocument()->m_runTimer,GetDocument()->GetMaxTime());
}

///////////////////////////////////////
// STOP THE SIMULATION
///////////////////////////////////////
void CViewController::OnTimerStop() 
{
	OnStopSimulation(FALSE);
}

//////////////////////////////////////////////////
// SIMULATION IS STOPPED MANUALLY OR BY BREAKPOINT
//////////////////////////////////////////////////
void CViewController::OnStopSimulation(BOOL bBP)
{
	KillTimer(TIMER_RUN);
	KillTimer(TIMER_REVIEW);

	m_bIsRunning = FALSE;
	GetDocument()->m_nRunMode = RUN_NONE;

	CTrace::T_STOPAT(GetDocument()->m_currTimer,GetDocument()->m_runTimer,GetDocument()->GetMaxTime(),bBP);
	m_cStart.ShowWindow(SW_SHOW);
	m_cStop.ShowWindow(SW_HIDE);
	EnableButtons();
}

//////////////////////////////////////////////////
// REVIEW THE SIMULATION BACKWARD
///////////////////////////////////////
void CViewController::OnReviewPrev() 
{
	// TODO: Add your control notification handler code here
	m_cNext.SetCheck(FALSE);
	int nb = GetDocument()->m_currTimer;
	int nCheck = m_cPrev.GetCheck();
	if (!nCheck)
	{
		OnReviewStop();
		return;
	}
	if (nb)
	{
		if (CPref::g_nMode)
			SetTimer(TIMER_REVIEW,TIMER_ELAPSE_LEARN,NULL);
		else
			SetTimer(TIMER_REVIEW,TIMER_ELAPSE_AUTH,NULL);

		GetDocument()->m_nRunMode = RUN_BACKWARD;
		CTrace::T_REVIEW(GetDocument()->m_currTimer,GetDocument()->m_nRunMode,GetDocument()->m_runTimer,GetDocument()->GetMaxTime());
		EnableButtons();
		m_cPrev.SetCheck(TRUE);
	}
	else
	{
		//OnReviewStop();
		m_cPrev.SetCheck(FALSE);
	}
}

//////////////////////////////////////////////////
// STOP REVIEWING THE SIMULATION
///////////////////////////////////////
void CViewController::OnReviewStop() 
{
	// TODO: Add your control notification handler code here
	KillTimer(TIMER_REVIEW);

	if (GetDocument()->m_nRunMode && GetDocument()->m_nRunMode < RUN_PREVSTEP)
		CTrace::T_STOPAT(GetDocument()->m_currTimer,GetDocument()->m_runTimer,GetDocument()->GetMaxTime(),FALSE);
	GetDocument()->m_nRunMode = RUN_NONE;

	m_cNext.SetCheck(FALSE);
	m_cPrev.SetCheck(FALSE);

	EnableButtons();
}

//////////////////////////////////////////////////
// REVIEW THE SIMULATION FOREWARD
//////////////////////////////////////////////////
void CViewController::OnReviewNext() 
{
	// TODO: Add your control notification handler code here
	m_cPrev.SetCheck(FALSE);

	int nb = GetDocument()->m_currTimer;
	int nbMax = GetDocument()->GetRunTime();
	int nCheck = m_cNext.GetCheck();
	if (!nCheck)
	{
		OnReviewStop();
		m_cNext.SetCheck(FALSE);
		return;
	}
	if (nb != nbMax)
	{
		if (CPref::g_nMode)
			SetTimer(TIMER_REVIEW,TIMER_ELAPSE_LEARN,NULL);
		else
			SetTimer(TIMER_REVIEW,TIMER_ELAPSE_AUTH,NULL);
		GetDocument()->m_nRunMode = RUN_FORWARD;
		CTrace::T_REVIEW(GetDocument()->m_currTimer,GetDocument()->m_nRunMode,GetDocument()->m_runTimer,GetDocument()->GetMaxTime());
		m_cNext.SetCheck(TRUE);
		EnableButtons();
	}
	else
	{
		m_cNext.SetCheck(FALSE);
		//OnReviewStop();
	}
	
}

//////////////////////////////////////////////////
// REVIEW AT THE BEGINNING OF THE SIMULATION
//////////////////////////////////////////////////
void CViewController::OnReviewBegin() 
{
	// TODO: Add your control notification handler code here
	int nb = GetDocument()->m_currTimer;
	if (nb)
	{
		//m_cTask.EnableWindow(FALSE);
		GetDocument()->m_currTimer = 0;
		CTrace::T_GOTO(GetDocument()->m_currTimer,GetDocument()->m_runTimer,GetDocument()->GetMaxTime());
		GetDocument()->UpdateTimerDoc(this);
	}
//	OnReviewStop() ;
}

//////////////////////////////////////////////////
// REVIEW AT THE END OF THE SIMULATION FOREWARD
//////////////////////////////////////////////////
void CViewController::OnReviewEnd() 
{
	// TODO: Add your control notification handler code here
	int nb = GetDocument()->m_currTimer;
	int nbMax = GetDocument()->GetRunTime();
	if (nb !=nbMax)
	{
		//m_cTask.EnableWindow(FALSE);
		GetDocument()->m_currTimer = nbMax;
		CTrace::T_GOTO(GetDocument()->m_currTimer,GetDocument()->m_runTimer,GetDocument()->GetMaxTime());
		GetDocument()->UpdateTimerDoc(this);
	}
//	OnReviewStop() ;
}


void CViewController::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	OnRunSimulation(nIDEvent);
	CFormView::OnTimer(nIDEvent);
}

void CViewController::OnPrepareNextBP()
{
	int nBP = GetDocument()->m_nCurrBP;

	int nNext = -1;
	CLearningUnit*	pLU = GetDocument()->GetCurrentLU();
	int nbBP = pLU->m_cTBreakSet.GetSize();
	for (int i=0;i<nbBP;i++)
	{
		CTimerBreakPoint *pBP = pLU->m_cTBreakSet.GetAt(i);
		if (pBP && pBP->m_tBreakPt > nBP)
		{
			nNext = pBP->m_tBreakPt;
			break;
		}
	}
	GetDocument()->m_nNextBP = nNext;
}

void CViewController::OnStopSimulationAtBP(int nBP)
{
	if (GetDocument()->m_nCurrBP < nBP)
	{
		//GetDocument()->m_nCurrBP = nBP;
		GetDocument()->m_bTaskDone = FALSE;
		CTimerBreakPoint pBP;
		CLearningUnit*	pLU = GetDocument()->GetCurrentLU();
		BOOL bEditable = pLU->GetTimerBPAt(nBP,pBP);
		if (bEditable)
		{
			m_cTask.SetIcon(m_ImageTaskList.ExtractIcon(pBP.m_nBPType));
		}
		//m_cTask.EnableWindow(TRUE);
		//GetDocument()->m_bTaskRead = FALSE;
		GetDocument()->m_bTaskRead = TRUE;
		OnReadTask();
	}
	OnStopSimulation(TRUE);
}


void CViewController::OnRunSimulation(UINT nIDEvent)
{
	CLearningUnit*	pLU = GetDocument()->GetCurrentLU();

	if (nIDEvent == TIMER_RUN)
	{
		int nBP = GetDocument()->m_nNextBP;
		if ((nBP != -1) && GetDocument()->m_currTimer >= nBP)
		{
			BOOL bDoBP = !pLU->m_bFirstRun || (pLU->m_bFirstRun && !GetDocument()->m_bFirstRun);
			if (bDoBP) 
				OnStopSimulationAtBP(nBP);
		}
		else if (GetDocument()->m_currTimer >= (GetDocument()->GetMaxTime()))
		{
			OnTimerStop();
			m_nMaxTime = GetDocument()->GetMaxTime();//max(m_nMaxTime,GetDocument()->m_currTimer);
			GetDocument()->m_bFirstRun = FALSE;

			GetDocument()->m_currTimer = m_nMaxTime;
			GetDocument()->m_runTimer = m_nMaxTime;
			GetDocument()->UpdateTimerDoc(this);
		}
		else
		{
			GetDocument()->m_currTimer +=1;
			m_nMaxTime = max(m_nMaxTime,GetDocument()->m_currTimer);
			GetDocument()->m_runTimer = m_nMaxTime;
			GetDocument()->UpdateTimerDoc(this);
		}
	}
	else if (nIDEvent == TIMER_REVIEW)
	{
		int nbM = GetDocument()->GetRunTime();
		int cc = GetDocument()->GetCurrTime();
		if ((GetDocument()->m_nRunMode == RUN_BACKWARD) || 
			(GetDocument()->m_nRunMode == RUN_PREVSTEP))
		{
			if (cc)
			{
				GetDocument()->m_currTimer -=1;
				GetDocument()->UpdateTimerDoc(this);
			}
			else
			{
				OnReviewStop();
			}
		}
		else if ((GetDocument()->m_nRunMode == RUN_FORWARD)|| 
				 (GetDocument()->m_nRunMode == RUN_NEXTSTEP))
		{
			if (cc!=nbM)
			{
				GetDocument()->m_currTimer +=1;
				GetDocument()->UpdateTimerDoc(this);
			}
			else
			{
				OnReviewStop();
			}
		}
		else
		{
			OnReviewStop();
		}
	}
	
}

/////////////////////////////////////////////////////////////////////////////
/// Expand or collapse the Initial Conditions pane of the Controller (as a result of clicking on
/// the pin button).
/////////////////////////////////////////////////////////////////////////////
void CViewController::OnExpandDlg() 
{
	// TODO: Add your control notification handler code here
	m_bIsExpanded = m_cExpand.IsPinned();

	if ( m_bIsExpanded ) 
	{
		SetScrollSizes(m_nMapMode, m_siTotal);
		GetParentFrame()->RecalcLayout();
		ResizeParentToFit(FALSE);
	}
	else
	{
		CRect rectClient;
		GetWindowRect(rectClient);
		CWnd *pWnd = GetDlgItem(IDC_STATIC_PARAM);
		CRect locRect;
		if (pWnd)
		{
			pWnd->GetClientRect(locRect);
			pWnd->GetWindowRect(locRect);
			SetWindowPos(NULL, 0, 0, rectClient.Width(), locRect.top-rectClient.top-5,
							SWP_NOMOVE|SWP_NOZORDER|SWP_NOACTIVATE);
			SetScrollSizes(MM_TEXT, CSize(0,0));

			GetWindowRect(rectClient);
			CRect rect = rectClient;
			CalcWindowRect(rect);
			rectClient.left += rectClient.left - rect.left;
			rectClient.top += rectClient.top - rect.top;
			rectClient.right -= rect.right - rectClient.right;
			rectClient.bottom -= rect.bottom - rectClient.bottom;
			rectClient.OffsetRect(-rectClient.left, -rectClient.top);
			ASSERT(rectClient.left == 0 && rectClient.top == 0);
		
			// determine desired size of the view
			CRect rectView(0, 0, rectClient.right, rectClient.bottom);
			CalcWindowRect(rectView, CWnd::adjustOutside);
			rectView.OffsetRect(-rectView.left, -rectView.top);
			ASSERT(rectView.left == 0 && rectView.top == 0);
			// dermine and set size of frame based on desired size of view
			CRect rectFrame;
			CFrameWnd* pFrame = GetParentFrame();
			ASSERT_VALID(pFrame);
			pFrame->GetWindowRect(rectFrame);
			CSize size = rectFrame.Size();
			size.cx += rectView.right - rectClient.right;
			size.cy =  locRect.top-rectClient.top + rectView.bottom - rectClient.bottom;
			pFrame->SetWindowPos(NULL, 0, 0, size.cx-4, 98,
				SWP_NOMOVE|SWP_NOZORDER|SWP_NOACTIVATE);
		}
	} 
}

void CViewController::OnExpSetApply() 
{
	// TODO: Add your control notification handler code here
	if (!UpdateData()) return;

	CLearningUnit*	pLU = GetDocument()->GetCurrentLU();
	if (!pLU) return;
	CMdEquation*	pEqu = GetDocument()->GetCurrentModel();
	if (!pEqu) return;
	pEqu->OnUpdate(pLU,TRUE);

	CTrace::T_CHANGEINIT(pLU,pEqu);

	GetDocument()->UpdateDoc(this);	
}

void CViewController::OnExpSetReset() 
{
	// TODO: Add your control notification handler code here
	CLearningUnit*	pLU = GetDocument()->GetCurrentLU();
	if (!pLU) return;
	CMdEquation*	pEqu = GetDocument()->GetCurrentModel();
	if (!pEqu) return;

	for(int i = 0; i <  pLU->m_cExpSet.GetSize();i++ )
		delete  pLU->m_cExpSet.GetAt(i);
	pLU->m_cExpSet.RemoveAll();


	for (i=0;i<pLU->m_cDefExpSet.GetSize();i++)
	{
		CMdExpertSet *pSet = pLU->m_cDefExpSet.GetAt(i);
		if (!pSet) continue;
		CMdExpertSet *pNewSet = pSet->Clone();
		pLU->m_cExpSet.Add(pNewSet);
	}
	UpdateData(FALSE);

	pEqu->OnUpdate(pLU,TRUE);
	GetDocument()->UpdateDoc(this);	

}

void CViewController::OnExpSetAdd() 
{
	// TODO: Add your control notification handler code here
	
}

void CViewController::OnExpSetDelete() 
{
	// TODO: Add your control notification handler code here
	
}

void CViewController::OnReadTask() 
{
	// TODO: Add your control notification handler code here

	GetDocument()->OnShowTask();

/*	CString mstr(_T(""));
	BOOL bNew = FALSE;

	int nCurrT=GetDocument()->m_currTimer;
	int nTBP = GetDocument()->m_nCurrBP;
	int nNBP = GetDocument()->m_nNextBP;
	if (nTBP != nNBP) 
	{
		nTBP = GetDocument()->m_nCurrBP = nNBP;
		bNew = TRUE;
	}
	GetDocument()->m_bTaskRead = TRUE;

	CTimerBreakPoint pBP;
	BOOL bEditable = GetDocument()->GetCurrentLU()->GetTimerBPAt(nTBP,pBP);

	if (!bEditable)*/ EnableButtons();

/*	UINT nType = MB_OK;
	switch (pBP.m_nBPType) {
	case 1:
		nType |= MB_ICONEXCLAMATION;
		mstr =  _T("Action : \n\n");
		break;
	case 2:
		nType |= MB_ICONQUESTION;
		mstr =  _T("Prediction : \n\n");
		break;
	case 3:
		nType |= MB_ICONQUESTION;
		mstr =  _T("Test : \n\n");
		break;
	case 0:
	default:
		nType |= MB_ICONINFORMATION;
		mstr =  _T("Information : \n\n");
		break;
	}
	mstr += pBP.m_sDesc;
	MessageBox(mstr,NULL,nType);*/
	m_cStart.EnableWindow(TRUE);

	//if (!bNew) 
	{
		EnableButtons();
		return;
	}
/*	CUserOutput pUserData;
	int nUser = GetDocument()->m_cUserInput.GetCount();
	BOOL bGet = GetDocument()->m_cUserInput.Lookup(nCurrT,pUserData);
	if (!bGet)
	{
		if (pBP.m_nBPType == BREAKP_ACTION || pBP.m_nBPType == BREAKP_HYPOT)
		{
			if (pBP.m_nBPType == BREAKP_ACTION)
				pUserData.m_nTime = nCurrT;
			else
			{
				CLearningUnit *pLU = GetDocument()->GetCurrentLU();

				int nb = pLU->m_cTBreakSet.GetSize();
				int nKey = -1;
				for (int i=0;i<nb;i++)
				{
					CTimerBreakPoint* pTB = pLU->m_cTBreakSet.GetAt(i);
					if (!pTB) continue;

					if (pTB->m_tBreakPt == nCurrT)
					{
						nKey = i+1;
						break;
					}
				}
				if (nKey!=-1 && nKey<nb)
				{
					CTimerBreakPoint* pTB = pLU->m_cTBreakSet.GetAt(nKey);
					if (pTB)
						pUserData.m_nTime = pTB->m_tBreakPt;
					else
						pUserData.m_nTime = -1;
				}
				else
					pUserData.m_nTime = -1;
			}
			if (pUserData.m_nTime != -1)
				GetDocument()->m_cUserInput.SetAt(nCurrT,pUserData);
		}
		GetDocument()->m_bTaskDone = FALSE;
	}

	GetDocument()->UpdatebreakPoint(this, &pBP);*/
}

void CViewController::OnNextUnit() 
{
	// TODO: Add your command handler code here
	GetDocument()->OnActivateNextLU();
}

void CViewController::OnUpdateNextUnit(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(TRUE);	
}

void CViewController::OnReviewPrevStep() 
{
	// TODO: Add your control notification handler code here
	int nb = GetDocument()->m_currTimer;
	if (nb)
	{
		m_cStart.EnableWindow(FALSE);
		//if (CPref::g_nMode)
		//	SetTimer(TIMER_REVIEW,TIMER_ELAPSE_LEARN,NULL);
		//else
		//	SetTimer(TIMER_REVIEW,TIMER_ELAPSE_AUTH,NULL);
		GetDocument()->m_nRunMode = RUN_PREVSTEP;
		CTrace::T_REVIEW(GetDocument()->m_currTimer,GetDocument()->m_nRunMode,GetDocument()->m_runTimer,GetDocument()->GetMaxTime());
		OnRunSimulation(TIMER_REVIEW);
		OnReviewStop();

	}
//	OnReviewStop() ;	
}

void CViewController::OnReviewNextStep() 
{
	// TODO: Add your control notification handler code here
	int nb = GetDocument()->m_currTimer;
	int nbMax = GetDocument()->GetRunTime();
	if (nb != nbMax)
	{
		m_cStart.EnableWindow(FALSE);
		GetDocument()->m_nRunMode = RUN_NEXTSTEP;
		CTrace::T_REVIEW(GetDocument()->m_currTimer,GetDocument()->m_nRunMode,GetDocument()->m_runTimer,GetDocument()->GetMaxTime());
		OnRunSimulation(TIMER_REVIEW);
		OnReviewStop();
	}
//	OnReviewStop() ;
}

void CViewController::OnUpdateApplyChange(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(!m_bIsLocked);
}

void CViewController::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	CFormView::OnClose();
}

void CViewController::OnNewBP() 
{
	// TODO: Add your control notification handler code here
	CLearningUnit*	pLU = GetDocument()->GetCurrentLU();

	int nCT= GetDocument()->m_currTimer;
	int nRT= GetDocument()->m_runTimer;
	int nCBP = GetDocument()->m_nCurrBP;
	int nNBP = GetDocument()->m_nNextBP;

	CTimerBreakPoint *pBP = new CTimerBreakPoint();

	pBP->m_bUserDefined = TRUE;
	pBP->m_nBPType = BREAKP_ACTION;
	pBP->m_tBreakPt = nCT;
	pBP->m_sDesc = _T("User-defined action");
	pBP->m_sHTML.Empty();


	int nb = pLU->m_cTBreakSet.GetSize();
	pLU->m_cTBreakSet.Add(pBP);

	GetDocument()->m_nNextBP = nCT;

	OnRunSimulation(TIMER_RUN);
	//OnStopSimulationAtBP(nCT);

}
