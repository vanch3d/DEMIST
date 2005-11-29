// SimulDoc.cpp : implementation of the CSimulDoc class
//

#include "stdafx.h"
#include "Simul.h"

#include "MainFrm.h"

#include "SimulDoc.h"
#include "Tools\MvDocTemplate.h"

#include <MSimulation\PreyPredModel.h>
#include <MInstruction\LearningUnit.h>

#include <Prefs\Pref.h>

#include "LearnerTrace.h"
#include "BPInfoDlg.h"


#include "Tools\ST_SplitterWnd.h"

#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define NBDATA 500

/////////////////////////////////////////////////////////////////////////////
// CSimulDoc

IMPLEMENT_DYNCREATE(CSimulDoc, CDocument)

BEGIN_MESSAGE_MAP(CSimulDoc, CDocument)
	//{{AFX_MSG_MAP(CSimulDoc)
	ON_COMMAND_RANGE(ID_VIEW_EXTREP1,ID_VIEW_EXTREP20, OnCmdLauchER)
	ON_UPDATE_COMMAND_UI_RANGE( ID_VIEW_EXTREP1, ID_VIEW_EXTREP20, OnUpdateCmdLauchER )
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSimulDoc construction/destruction

CSimulDoc::CSimulDoc()
{
	m_bFirstRun = TRUE;
	m_nPredMode = PRED_NONE;
	m_bTaskDone = TRUE;
	m_bTaskRead = TRUE;
	m_nModSelItem = 0;
	m_currTimer = 0;
	m_runTimer = 0;
	m_nCurrBP = -1;
	m_nNextBP = -1;
	m_nRunMode = RUN_NONE;
	// MODELS
	m_pSimul = new CPreyPredModel();
	m_pInstruct = new CInstModel(this);

	//m_CurrModel = 0;
	if (CPref::g_nMode==1)
		m_CurrInst = -1;
	else
		m_CurrInst = 0;
}

CSimulDoc::~CSimulDoc()
{
	if (m_pSimul) delete m_pSimul;
	if (m_pInstruct) delete m_pInstruct;

	m_cUserInput.RemoveAll();
/*	CWnd *pWnd = AfxGetMainWnd();
	CMainFrame *pMainFrm = DYNAMIC_DOWNCAST( CMainFrame, pWnd);
	if (pMainFrm)
	{
		pMainFrm->OnSetMERIcons(NULL);
	}*/
}

/////////////////////////////////////////////////////////////////////////////
/// Called by the framework when a document is open.
/// \param lpszPathName Path of the DEMIST file to open.
/// \return TRUE if the document is open successfully, FALSE otherwise.
///
/// This function updates the simulation and the instructional models and calls
/// UpdateRestartSimul() to start the scenario.
/////////////////////////////////////////////////////////////////////////////
BOOL CSimulDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
	
	// TODO: Add your specialized creation code here
	m_pSimul->OnInitiaUpdate();
	m_pInstruct->OnInitiaUpdate();
	m_Model =	"Prey and Predator model\r\n";

	SetPathName( lpszPathName );
	CTrace::WriteDocHeader(this);

	UpdateRestartSimul(FALSE);

/*	if (CPref::g_nMode)
	{
		//BOOL bOld = m_bAutoDelete;
		//m_bAutoDelete = FALSE;
		OnActivateInitLU();
		OnActivateCurrentLU(FALSE);
		//m_bAutoDelete = m_bAutoDelete;
	}*/
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
/// Called by the framework when a new document is open.
/// \return TRUE if the document is created successfully, FALSE otherwise.
///
/// This function creates and initialises new empty simulation and instructional models.
/////////////////////////////////////////////////////////////////////////////
BOOL CSimulDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	m_pSimul->OnNewModel();
	m_pInstruct->OnNewModel();

	m_Model =	"Prey and Predator model\r\n";
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
/// Called by the framework when a document is closed.
///
/////////////////////////////////////////////////////////////////////////////
void CSimulDoc::OnCloseDocument() 
{
	// TODO: Add your specialized code here and/or call the base class
	//SaveModified();
	CString strDocName = GetPathName();

	CDocument::OnCloseDocument();
	CTrace::WriteDocEnd(strDocName);
}


/////////////////////////////////////////////////////////////////////////////
// CSimulDoc serialization

void CSimulDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
		ar << m_Model;
	}
	else
	{
		// TODO: add loading code here
		ar >> m_Model;
	}
	m_pSimul->Serialize(ar);
	m_pInstruct->Serialize(ar);
}

/////////////////////////////////////////////////////////////////////////////
// CSimulDoc diagnostics

#ifdef _DEBUG
void CSimulDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CSimulDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSimulDoc commands

/////////////////////////////////////////////////////////////////////////////
/// Called by the framework when the mathematical models are modified.
/// \param pSender Pointer to the view where the intervention took place.
///
/// This function simply dispatches the message (#DOC_UPDATE_ALLDATA) to all the 
/// views in order to proceed the relevant feedback.
/////////////////////////////////////////////////////////////////////////////
void CSimulDoc::UpdateDoc(CView* pSender)
{
	UpdateAllViews(NULL,DOC_UPDATE_ALLDATA);
}

/////////////////////////////////////////////////////////////////////////////
/// Called by the framework when the simulation is ran or reviewed.
/// \param pSender Pointer to the view where the intervention took place.
///
/// This function simply dispatches the message (#DOC_UPDATE_TIMERDATA) to all the 
/// views in order to proceed the relevant feedback.
/////////////////////////////////////////////////////////////////////////////
void CSimulDoc::UpdateTimerDoc(CView* pSender)
{
	if (m_nRunMode == RUN_RUN && m_nPredMode == PRED_TEST)
		m_nPredMode = PRED_NONE;
	//CTimerBreakPoint mInfo;
	//mInfo.m_nBPType = GetCurrTime();
	//mInfo.m_tBreakPt = m_nRunMode;
	//UpdateAllViews(NULL,DOC_UPDATE_TIMERDATA,&mInfo);
	UpdateAllViews(NULL,DOC_UPDATE_TIMERDATA,(CObject*)&m_nRunMode);
//	UpdateAllViews(NULL,DOC_UPDATE_TIMERDATA);
}

/////////////////////////////////////////////////////////////////////////////
/// Called by the framework when the simulation performs a Map Relation
/// \param pSender Pointer to the view where the action took place
/// \param pTransInfo Pointer to the data structure detailling the relation.
///
/// This function updates the Current Time (#m_currTimer) and dispatches 
/// the message (#TRANSLATION_MAPRELATION) to all the views in order to proceed 
/// the relevant feedback.
/////////////////////////////////////////////////////////////////////////////
void CSimulDoc::UpdateMapRelation(CView* pSender, CTranslationInfo* pTransInfo)
{
	if (pTransInfo && pSender)
		pTransInfo->m_pWndTarget = pSender;
	
	if (pTransInfo && pTransInfo->m_nTime!=-1)	
		m_currTimer = pTransInfo->m_nTime;

	UpdateAllViews(NULL,TRANSLATION_MAPRELATION,(CObject*)pTransInfo);
	CTrace::T_MAPRELATION(this,pTransInfo);
}


/////////////////////////////////////////////////////////////////////////////
/// Called by the framework when the user (or the system) is restarting the simulation from scratch.
/// \param bAlert Specify if a confirmation message is proposed to the user (default=TRUE),
///
/// This function reinitialise the simulation by cleaning ALL the user's actions stored in the ERs 
/// (CExternRepUnit::m_cUserInput) or in the document (#m_cUserInput), by re-assigning 
/// the breakpoints (#m_nCurrBP and #m_nNextBP) and the Timer (#m_currTimer and #m_runTimer) and
/// by recalculating the mathematical models (CMdEquation::OnUpdate(CLearningUnit*,BOOL)).
/// It finally dispatches the message (#DOC_UPDATE_RESTARTSIMUL) to all the 
/// views in order to proceed the relevant feedback.
/////////////////////////////////////////////////////////////////////////////
void CSimulDoc::UpdateRestartSimul(BOOL bAlert)
{
	CString mstr;
	mstr.LoadString(ERR_SIMULATION_RESTART);
	BOOL bOK = TRUE;
	if (bAlert)
		bOK = (AfxGetMainWnd()->MessageBox(mstr,NULL,MB_OKCANCEL|MB_ICONWARNING) == IDOK);
	if (bOK)
	{
		m_currTimer = 0;
		m_runTimer = 0;
		m_nCurrBP = -1;
		m_nNextBP = -1;

		CLearningUnit *pLU = GetCurrentLU();
		if (pLU)
		{
			for (int i=0;i<pLU->m_cMERSet.GetSize();i++)
			{
				CExternRepUnit* pExtRep = pLU->m_cMERSet.GetAt(i);
				if (pExtRep)
					pExtRep->m_cUserInput.RemoveAll();
			}
			pLU->CleanTimerBP();
			int nbBP = pLU->m_cTBreakSet.GetSize();
			if (nbBP)
			{
				//m_cSlider.SetCurrentBreakPoint(-1);
				CTimerBreakPoint *pBP = pLU->m_cTBreakSet.GetAt(0);
				BOOL bDoBP = !pLU->m_bFirstRun || (pLU->m_bFirstRun && !m_bFirstRun);
				if (pBP && bDoBP)
					m_nNextBP = pBP->m_tBreakPt;
			}
			else
				m_nNextBP = -1;
			CMdEquation *pEqu = GetCurrentModel();
			if (pEqu)
				pEqu->OnUpdate(pLU,TRUE);
		}
		m_cUserInput.RemoveAll();
		UpdateAllViews(NULL,DOC_UPDATE_RESTARTSIMUL);
	}

}

/////////////////////////////////////////////////////////////////////////////
/// Called by the framework when the user performs a Prediction. 
/// \param pSender Pointer to the view where the action took place.
/// \param pData Pointer to the data structure detailling the action.
///
/// This function simply dispatches the message (#TRANSLATION_HYPOTHESIS) to all the 
/// views in order to proceed the relevant feedback.
/////////////////////////////////////////////////////////////////////////////
void CSimulDoc::UpdateHypothesis(CView* pSender, CUserData *pData)
{
	m_nPredMode = PRED_HYPOT;
	CTrace::T_HYPOTHESIS(pSender,pData);
	UpdateAllViews(NULL,TRANSLATION_HYPOTHESIS,pData);
}

/////////////////////////////////////////////////////////////////////////////
/// Called by the framework when the simulation reaches a TEST breakpoint.
/// \param pSender Pointer to the view where the action took place
/// \param pData Pointer to the data structure summarising the action
///
/// This function simply dispatches the message (#TRANSLATION_TEST) to all the 
/// views in order to proceed the relevant feedback.
/////////////////////////////////////////////////////////////////////////////
void CSimulDoc::UpdateTest(CView* pSender, CUserData *pData)
{
	m_nPredMode = PRED_TEST;
	CTrace::T_HYPO_TEST(pSender,pData);
	UpdateAllViews(pSender,TRANSLATION_TEST,pData);
}

/////////////////////////////////////////////////////////////////////////////
/// Called by the framework when the simulation reaches a breakpoint.
/// \param pSender Pointer to the view where the action took place
/// \param pData Pointer to the data structure summarising the action
///
/// This function simply dispatches the message (#DOC_UPDATE_BREAKPOINT) to all the 
/// views in order to proceed the relevant feedback. 
/////////////////////////////////////////////////////////////////////////////
void CSimulDoc::UpdatebreakPoint(CView* pSender, CTimerBreakPoint *pData)
{
	CTrace::T_BREAKPOINT(pData);
	UpdateAllViews(pSender,DOC_UPDATE_BREAKPOINT,pData);
	if (pData->m_nBPType == BREAKP_TEST)
	{
		UpdateTest(NULL,NULL);
	}
}

/////////////////////////////////////////////////////////////////////////////
/// Called by the framework when the user acts on an ER, by doing either an Action or a Prediction. 
/// \param pExtRep	Pointer to the ER where the action took place
/// \param pData	Pointer to the data structure detailling the action
///
/// It creates a CUserOutput variable based on the information stored in the CUserData structure 
/// and stores it in the CExternRepUnit::m_cUserInput field of the ER.
/////////////////////////////////////////////////////////////////////////////
BOOL CSimulDoc::UpdateERUserData(CExternRepUnit *pExtRep, CUserData *pData)
{
	if (!pExtRep) return FALSE;
	if (!pData) return FALSE;
	CTimerBreakPoint pBP;
	CLearningUnit *pLU = GetCurrentLU();
	BOOL bEditable = pLU->GetTimerBPAt(pData->m_nTime,pBP);
	if (!bEditable) return FALSE;

	pData->m_nType = pBP.m_nBPType;
	
	CUserOutput pUserData;
	BOOL bGet = pExtRep->m_cUserInput.Lookup(pData->m_nTime,pUserData);
	if (!bGet)
	{
		int nbD = pUserData.m_lUserInput.GetCount();
		pUserData.SetAt(pData->m_nExpSet,pData->m_nOutcome,*pData);
		nbD = pUserData.m_lUserInput.GetCount();
		pUserData.m_nTime = pData->m_nTime;
		pExtRep->m_cUserInput.SetAt(pData->m_nTime,pUserData);
	}
	else
	{
		CUserData pData2;
		BOOL bRes = pUserData.GetAt(pData->m_nExpSet,pData->m_nOutcome,pData2);
		if (bRes)
		{
			pData->m_dOldValue = pData2.m_dOldValue;
		}
		pUserData.SetAt(pData->m_nExpSet,pData->m_nOutcome,*pData);
		pExtRep->m_cUserInput.SetAt(pData->m_nTime,pUserData);
	}
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
/// Called by the framework when the user acts on an ER, by doing either an Action or a Prediction. 
/// \param pSender Pointer to the view where the action took place
/// \param pData Pointer to the data structure summarising the action
///
/// It creates a CUserOutput variable based on the information of the CUserData 
/// structure, stores it in the #m_cUserInput field of the document and, depending on the nature
/// of the intervention, call either UpdateAction(CView*,CUserData*) or UpdateHypothesis(CView*,CUserData*)
/// to update all the views.
/////////////////////////////////////////////////////////////////////////////
BOOL CSimulDoc::UpdateUserData(CView* pSender, CUserData *pData)
{
	CTimerBreakPoint pBP;
	CLearningUnit *pLU = GetCurrentLU();
	BOOL bEditable = pLU->GetTimerBPAt(pData->m_nTime,pBP);
	if (!bEditable) return FALSE;

//	CMdEquation*	pEqu = GetDocument()->GetCurrentModel();
//	if (!pEqu) return;

	pData->m_nType = pBP.m_nBPType;
	
	CUserOutput pUserData;
	BOOL bGet = m_cUserInput.Lookup(pData->m_nTime,pUserData);
	if (!bGet)
	{
		int nbD = pUserData.m_lUserInput.GetCount();
		pUserData.SetAt(pData->m_nExpSet,pData->m_nOutcome,*pData);
		nbD = pUserData.m_lUserInput.GetCount();
		pUserData.m_nTime = pData->m_nTime;
		m_cUserInput.SetAt(pData->m_nTime,pUserData);
	}
	else
	{
		CUserData pData2;
		BOOL bRes = pUserData.GetAt(pData->m_nExpSet,pData->m_nOutcome,pData2);
		if (bRes)
		{
			pData->m_dOldValue = pData2.m_dOldValue;
		}
		pUserData.SetAt(pData->m_nExpSet,pData->m_nOutcome,*pData);
		m_cUserInput.SetAt(pData->m_nTime,pUserData);
	}

	if (pBP.m_nBPType == BREAKP_ACTION)	// ACTION
	{
		//CMdData *PreyInit= pLU->m_cDataPoints.GetAt(pData.m_nOutcome);
		//CString mstr=PreyInit->GetDataName();
		//double ddt = PreyInit->GetAt(pData.m_nTime,pData.m_nExpSet);
		//PreyInit->SetAt(pData.m_dValue,pData.m_nTime,pData.m_nExpSet);
		//ddt = PreyInit->GetAt(pData.m_nTime,pData.m_nExpSet);

		//pEqu->OnUpdateFrom(pLU,pData.m_nExpSet,pData.m_nTime);
		//GetDocument()->UpdateAllViews(this,DOC_UPDATE_ALLDATA);
		UpdateAction(pSender,pData);
		//OnUpdateData();
		//FormatCells(pData.m_nTime,TRUE);
	}
	else	// HYPOTHESIS
	{
		//OnUpdate(this,TRANSLATION_HYPOTHESIS,&pData);
		//GetDocument()->UpdateAllViews(this,TRANSLATION_HYPOTHESIS,&pData);
		UpdateHypothesis(pSender,pData);
	}
	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
/// Called by the framework when the user performs an Action. 
/// \param pSender Pointer to the view where the action took place
/// \param pData Pointer to the data structure summarising the action
///
/// It uses the new value defined by the user's action (CUserData::m_dValue) to
/// recalculate the mathematical models (CMdEquation::OnUpdateFrom(CLearningUnit*,int,int,BOOL)), 
/// then dispatches the message (#TRANSLATION_ACTION) to all the 
/// views in order to proceed the relevant feedback.
/////////////////////////////////////////////////////////////////////////////
void CSimulDoc::UpdateAction(CView* pSender, CUserData *pData)
{
	if (!pData) return;

//	if (pData->m_bUpdateNow)
	{
		CLearningUnit* pLU = GetCurrentLU();
		CMdEquation*  pEqu = GetCurrentModel();

		CMdData *pModData= pLU->m_cDataPoints.GetAt(pData->m_nOutcome);
		CString mstr=pModData->GetDataName(pLU->m_bShowAbbrev);
	
		double nOldV = pModData->GetAt(pData->m_nTime,pData->m_nExpSet);
		pModData->SetAt(pData->m_dValue,pData->m_nTime,pData->m_nExpSet);
		double nNewVal = pModData->GetAt(pData->m_nTime,pData->m_nExpSet);

		pEqu->OnUpdateFrom(pLU,pData->m_nExpSet,pData->m_nTime,FALSE);
		pModData->ResetMinMax(pData->m_nExpSet);

		CTrace::T_ACTION(pSender,pData);
	}

	//UpdateAllViews(pSender,DOC_UPDATE_ALLDATA);
	UpdateAllViews(NULL,TRANSLATION_ACTION,pData);
}

/////////////////////////////////////////////////////////////////////////////
/// Get the Learning Unit currently used by the framework.
/// \return A pointer to the relevant LU, NULL if none
/////////////////////////////////////////////////////////////////////////////
CLearningUnit* CSimulDoc::GetCurrentLU()
{
	int nb = m_pInstruct->m_cLUSet.GetSize();
	if (m_CurrInst==-1 || m_CurrInst>=nb)
		return NULL;
	else
		return m_pInstruct->m_cLUSet.GetAt(m_CurrInst);

}

/////////////////////////////////////////////////////////////////////////////
/// Get the Mathematical Model associated with the current Learning Unit.
/// \return A pointer to the model, NULL if none.
/////////////////////////////////////////////////////////////////////////////
CMdEquation* CSimulDoc::GetCurrentModel()
{
	CMdEquation* pEqu = NULL;
	CLearningUnit* pLU = GetCurrentLU();
	if (pLU->m_nSimulID != -1 && pLU->m_nSimulID<m_pSimul->m_cEquSet.GetSize())
		pEqu = m_pSimul->m_cEquSet.GetAt(pLU->m_nSimulID);//GetEquation(m_CurrModel);
	return pEqu;
}

/////////////////////////////////////////////////////////////////////////////
/// Get the number of time-step associated with the current Learning Unit.
/// \return the number of time-step (CLearningUnit::GetMaxTimer()) defined in the current LU, 0 if none.
/////////////////////////////////////////////////////////////////////////////
int CSimulDoc::GetMaxTime()
{
	CLearningUnit* pLU = GetCurrentLU();
	if (pLU) 
		return pLU->GetMaxTimer();
	else
		return 0;
}

/////////////////////////////////////////////////////////////////////////////
/// Get the Current Time associated with the current Learning Unit.
/// \return the current time (as stored in the #m_currTimer data) .
/////////////////////////////////////////////////////////////////////////////
int	CSimulDoc::GetCurrTime()
{
	return m_currTimer;
}

/////////////////////////////////////////////////////////////////////////////
/// Get the Run Time associated with the current Learning Unit.
/// \return the run time (as stored in the #m_runTimer data) .
/////////////////////////////////////////////////////////////////////////////
int	CSimulDoc::GetRunTime()
{
	return m_runTimer;
}


/////////////////////////////////////////////////////////////////////////////
/// Called by the framework when the ER's menu and icons need to be updated. 
///
/// This function calls the CLearningUnit::UpdateERMenu() to modify the content 
/// of the View menu and CMainFrame::OnSetMERIcons() to modify the content of the 
/// ER's toolbar.
/////////////////////////////////////////////////////////////////////////////
void CSimulDoc::UpdateViewMenu()
{
	CLearningUnit* pLU = GetCurrentLU();

	if (pLU)
	{
		pLU->UpdateERMenu(NULL);
	}
	CWnd *pWnd = AfxGetMainWnd();
	CMainFrame *pMainFrm = DYNAMIC_DOWNCAST( CMainFrame, pWnd);
	if (pMainFrm)
	{
		pMainFrm->OnSetMERIcons(pLU);
	}
}

/////////////////////////////////////////////////////////////////////////////
/// Called by the framework when the ER's menu and icons need to be updated. 
///
/////////////////////////////////////////////////////////////////////////////
void CSimulDoc::UpdateViewMenuDrop(CWnd* pWnd,CRect rLoc)
{
	if (!pWnd) return;
	CLearningUnit* pLU = GetCurrentLU();

	if (!pLU) return;
	//pLU->UpdateERMenu(NULL);

//	CRect rc;
//	pWnd->SendMessage(TB_GETRECT, pnmtb->iItem, (LPARAM)&rc);
//	pWnd->ClientToScreen(&rc);
			//ClientToScreen(&point);

	CMenu *pMenu = NULL;
	pMenu = pLU->GetERsMenu();
	if (pMenu)
	{
		pMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,rLoc.left, rLoc.bottom,pWnd,&rLoc);
		pMenu->DestroyMenu();
		delete pMenu;
	}
}

/////////////////////////////////////////////////////////////////////////////
/// Called by the framework when the ER's menu and icons need to be updated. 
///
/////////////////////////////////////////////////////////////////////////////
void CSimulDoc::OnUpdateCmdLauchER(CCmdUI* pCmdUI) 
{
	BOOL bDone = FALSE;
	// TODO: Add your command handler code here
	CLearningUnit* pLU = GetCurrentLU();
	if (pLU) 
		bDone = pLU->UpdateLauchER(pCmdUI);

	if (!bDone)
		pCmdUI->Enable(FALSE);
}

/////////////////////////////////////////////////////////////////////////////
/// Called by the framework when the user activates an ER by using the menu or the icons
/// \param nID ID of the menu command activated by the user.
///
/////////////////////////////////////////////////////////////////////////////
void CSimulDoc::OnCmdLauchER(UINT nID ) 
{
	// TODO: Add your command handler code here
	CLearningUnit* pLU = GetCurrentLU();
	if (pLU) pLU->LauchER(nID);
}

/////////////////////////////////////////////////////////////////////////////
/// Called by the framework when the user closes a view.
/// \param pView Pointer to the view closed by the user.
/// \return TRUE if the framework can destroy the view. FALSE otherwise.
///
/// This function calles the CLearningUnit::OnCloseER(CView) to clean all thie information 
/// related to the External Representation and its view.
/////////////////////////////////////////////////////////////////////////////
BOOL CSimulDoc::OnCloseER(CView *pView)
{
	BOOL bCanClose = TRUE;
	CLearningUnit* pLU = GetCurrentLU();
	if (pLU) 
		bCanClose = pLU->OnCloseER(pView);

	return bCanClose;
}

/////////////////////////////////////////////////////////////////////////////
/// Called by the framework when the user requests the next LU in the scenario.
/// \param bMsg Specify if a confirmation message is presented to the user (Default=TRUE).
/// \return TRUE if the next LU can be initialised, FALSE otherwise.
///
/// This function 
/////////////////////////////////////////////////////////////////////////////
BOOL CSimulDoc::OnActivateNextLU(BOOL bMsg)
{
	CString mstr;
	mstr.LoadString(MSG_LU_ACT_NEXT);

	int nb = m_pInstruct->m_cLUSet.GetSize();
	if (m_CurrInst >= (nb-1)) return FALSE;

	if (bMsg && AfxGetMainWnd()->MessageBox(mstr,NULL,MB_YESNO)!=IDYES) return FALSE;

	CLearningUnit *pLU = GetCurrentLU();
	if (!pLU) return FALSE;

	BOOL bOld = m_bAutoDelete;
	m_bAutoDelete = FALSE;

	pLU->OnCloseAllER();

	m_CurrInst++;
	m_bTaskDone = TRUE;
	m_bTaskRead = TRUE;
	pLU = GetCurrentLU();
//	if (CPref::g_nMode) 
//		pLU->m_bCanStart=FALSE;

	m_bFirstRun = TRUE;

	CTrace::T_STARTUNIT(m_CurrInst,pLU->m_sLUName);
	UpdateRestartSimul(FALSE);
	UpdateViewMenu();
	if (bMsg /*&& pLU->m_bCanStart*/) OnLaunchStartER();

	m_bAutoDelete = bOld;

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
/// Called by the framework when the user requests the previous LU in the scenario.
/// \param bMsg Specify if a confirmation message is presented to the user (Default=TRUE).
/// \return TRUE if the previous LU can be initialised, FALSE otherwise.
///
/// This function 
/////////////////////////////////////////////////////////////////////////////
BOOL CSimulDoc::OnActivatePrevLU(BOOL bMsg)
{
	CString mstr;
	mstr.LoadString(MSG_LU_ACT_PREVIOUS);

	int nb = m_pInstruct->m_cLUSet.GetSize();
	if (!m_CurrInst) return FALSE;

	if (bMsg && AfxGetMainWnd()->MessageBox(mstr,NULL,MB_YESNO)!=IDYES) return FALSE;

	//CInstModel *pInst = pDoc->m_pInstruct;
	CLearningUnit *pLU = GetCurrentLU();
	if (!pLU) return FALSE;

	BOOL bOld = m_bAutoDelete;
	m_bAutoDelete = FALSE;
	pLU->OnCloseAllER();
	m_bAutoDelete = bOld;

	m_CurrInst--;
	m_bTaskDone = TRUE;
	m_bTaskRead = TRUE;
	pLU = GetCurrentLU();

	m_bFirstRun = TRUE;

//	if (CPref::g_nMode) 
//		pLU->m_bCanStart=FALSE;

	CTrace::T_STARTUNIT(m_CurrInst,pLU->m_sLUName);
	UpdateRestartSimul(FALSE);
	UpdateViewMenu();
	if (bMsg) OnLaunchStartER();

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
/// Called by the framework when the user requests the reinitialisation of the current LU.
/// \param bMsg Specify if a confirmation message is presented to the user (Default=TRUE).
/// \return TRUE if the LU can be initialised, FALSE otherwise.
///
/// This function 
/////////////////////////////////////////////////////////////////////////////
BOOL CSimulDoc::OnActivateCurrentLU(BOOL bMsg/*=TRUE*/)
{
	CString mstr;
	mstr.LoadString(MSG_LU_ACT_CURRENT);

	if (bMsg)
		if (AfxGetMainWnd()->MessageBox(mstr,NULL,MB_YESNO)!=IDYES) 
			return FALSE;

	CLearningUnit *pLU = GetCurrentLU();
	if (!pLU) return FALSE;

	pLU->m_bCanStart = TRUE;

	BOOL bOld = m_bAutoDelete;
	m_bAutoDelete = FALSE;
	pLU->OnCloseAllER();
	m_bAutoDelete = bOld;
	m_bFirstRun = TRUE;

	CTrace::T_STARTUNIT(m_CurrInst,pLU->m_sLUName);
	m_bTaskDone = TRUE;
	m_bTaskRead = TRUE;
	UpdateRestartSimul(FALSE);
	UpdateViewMenu();
	OnLaunchStartER();
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
/// Called by the framework when the user requests the reinitialisation of the current LU.
/// \param bMsg Specify if a confirmation message is presented to the user (Default=TRUE).
/// \return TRUE if the LU can be initialised, FALSE otherwise.
///
/// This function 
/////////////////////////////////////////////////////////////////////////////
BOOL CSimulDoc::OnActivateInitLU()
{
	CString mstr;
	mstr.LoadString(MSG_LU_ACT_INIT);

	if (AfxGetMainWnd()->MessageBox(mstr,NULL,MB_YESNO)!=IDYES) return FALSE;

	CLearningUnit *pLU = GetCurrentLU();
	if (pLU)
	{
		BOOL bOld = m_bAutoDelete;
		m_bAutoDelete = FALSE;
		pLU->OnCloseAllER();
		m_bAutoDelete = bOld;
	}

	m_CurrInst = 0;
	pLU = GetCurrentLU();
//	if (CPref::g_nMode)
//		pLU->m_bCanStart = FALSE;

	m_bFirstRun = TRUE;

	CTrace::T_STARTUNIT(m_CurrInst,pLU->m_sLUName);
	m_bTaskDone = TRUE;
	m_bTaskRead = TRUE;
	UpdateRestartSimul(FALSE);
	UpdateViewMenu();
	//OnLaunchStartER();
	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
/// Called by the framework when the default ERs of the LU have to be displayed
/// \return TRUE if the views can be initialised, FALSE otherwise.
///
/// This function 
/////////////////////////////////////////////////////////////////////////////
BOOL CSimulDoc::OnLaunchStartER()
{
	CLearningUnit *pLU = GetCurrentLU();
	if (!pLU) return FALSE;

	int nb = pLU->m_cMERSet.GetSize();
	for (int i=0;i<nb;i++)
	{
		CExternRepUnit *pER=pLU->m_cMERSet.GetAt(i);
		if (!pER) continue;
		if (pER->m_bOpenStart)
			pLU->LauchER(pER->m_CmDId);
	}
	return TRUE;
} 

/////////////////////////////////////////////////////////////////////////////
/// Called by the framework when the default ERs of the LU have to be displayed
/// \return TRUE if the views can be initialised, FALSE otherwise.
///
/// This function 
/////////////////////////////////////////////////////////////////////////////
BOOL CSimulDoc::OnShowTask()
{
	CString mstr(_T(""));
	BOOL bNew = FALSE;

	int nCT= m_currTimer;
	int nCBP = m_nCurrBP;
	int nNBP = m_nNextBP;
	if (nCBP != nNBP) 
	{
		nCBP = m_nCurrBP = nNBP;
		bNew = TRUE;
	}
	
	m_bTaskRead = TRUE;

	CTimerBreakPoint pBP;
	BOOL bEditable = GetCurrentLU()->GetTimerBPAt(nCBP,pBP);

	UINT nType = MB_OK;
	UINT nIcon=0;
	switch (pBP.m_nBPType) {
	case 1:
		nType = MB_ICONEXCLAMATION;
		//tr =  _T("Action : \n\n");
		mstr =  _T("Action");
		nIcon = IDB_BP_ACTION;
		break;
	case 2:
		nType = MB_ICONQUESTION;
		//tr =  _T("Prediction : \n\n");
		mstr =  _T("Prediction");
		nIcon = IDB_BP_HYPOT;
		break;
	case 3:
		nType = MB_ICONQUESTION;
		//tr =  _T("Test : \n\n");
		mstr =  _T("Test");
		nIcon = IDB_BP_TEST;
		break;
	case 0:
	default:
		nType = MB_ICONINFORMATION;
		//mstr =  _T("Information : \n\n");
		mstr =  _T("Information");
		nIcon = IDB_BP_INFO;
		break;
	}
	CBPInfoDlg dlg;
	dlg.m_strTitle = mstr;
	dlg.m_nIcon = nIcon;
	dlg.m_strDesc = pBP.m_sDesc;
	::MessageBeep(nType);
	dlg.DoModal();

	//mstr += pBP.m_sDesc;
	//	AfxMessageBox(mstr,nType);

	CUserOutput pUserData;
	int nUser = m_cUserInput.GetCount();
	BOOL bGet = m_cUserInput.Lookup(nCT,pUserData);
	if (!bGet)
	{
		if (pBP.m_nBPType == BREAKP_ACTION || pBP.m_nBPType == BREAKP_HYPOT)
		{
			if (pBP.m_nBPType == BREAKP_ACTION)
				pUserData.m_nTime = nCT;
			else
			{
				CLearningUnit *pLU = GetCurrentLU();

				int nb = pLU->m_cTBreakSet.GetSize();
				int nKey = -1;
				for (int i=0;i<nb;i++)
				{
					CTimerBreakPoint* pTB = pLU->m_cTBreakSet.GetAt(i);
					if (!pTB) continue;

					if (pTB->m_tBreakPt == nCT)
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
				m_cUserInput.SetAt(nCT,pUserData);
		}
		m_bTaskDone = FALSE;
	}
	UpdatebreakPoint(NULL, &pBP);
	return TRUE;
}



BOOL CSimulDoc::OnCreateMultiPanes(CObject *pWndList,CCreateContext* pContext,CMDIChildWnd *pChildWnd)
{
	CPtrArray *m_pSplitterWnds = DYNAMIC_DOWNCAST( CPtrArray, pWndList);
	
	CLearningUnit *pLU = GetCurrentLU();
	if (!pLU) return FALSE;

	pLU->OnCreateMultiPanes(pWndList,pContext,pChildWnd);

	return TRUE;
}
