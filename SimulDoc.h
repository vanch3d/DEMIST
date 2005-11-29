//////////////////////////////////////////////////////////////////////
/// @file SimulDoc.h
/// @brief Interface for the CSimulDoc class.
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SIMULDOC_H__4095155B_F048_11D3_A2F4_00D0B71C8709__INCLUDED_)
#define AFX_SIMULDOC_H__4095155B_F048_11D3_A2F4_00D0B71C8709__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "UserOutput.h"

//#include "undo.h"

/// @name Notification Messages
/// These messages are send by the framework to all the views currently open in the workspace
/// to notify them of the occurence of a particular event so that, if relevant, they could handle it. 
//@{
#define DOC_UPDATE_LUNIT			(WM_USER+300)	///< Notify a modification of a Learning Unit.
#define DOC_UPDATE_ER				(WM_USER+301)	///< Notify a modification of an External Representation.
#define DOC_UPDATE_MODEL			(WM_USER+302)	///< Notify a modification of a Simulation Model.
#define DOC_UPDATE_EXPSET			(WM_USER+303)	///< Notify a modification of an Experimental Set.
#define DOC_UPDATE_ALLDATA			(WM_USER+304)	///< Notify a modification of all the numerical data of the simulation.
#define DOC_UPDATE_TIMERDATA		(WM_USER+305)	///< Notify a modification of the timer (current time and run time).
#define DOC_UPDATE_RESTARTSIMUL		(WM_USER+306)	///< Notify a request to restart the simulation from scratch.
#define DOC_UPDATE_BREAKPOINT		(WM_USER+307)	///< Notify a breakpoint reached by the simulation.
#define DOC_UPDATE_TIMERSTOP		(WM_USER+308)	///< Notify a pause (both by user and system) in running the simulation.

#define TRANSLATION_MAPFACT			(WM_USER+310)	///< Notify a Map Fact (Not Yet Implemented).
#define TRANSLATION_MAPRELATION		(WM_USER+311)	///< Notify a Map relation.
#define TRANSLATION_ACTION			(WM_USER+312)	///< Notify an Action.
#define TRANSLATION_HYPOTHESIS		(WM_USER+313)	///< Notify a Prediction.
#define TRANSLATION_TEST			(WM_USER+314)	///< Notify a Test.

#define DOC_UPDATE_VIEWCONTENT		(WM_USER+327)	///< Notify the creation of a view asosciated with an ER.

#define TRACE_VIEW_ACTIVATE			(WM_USER+333)	///< Notify the activation of an ER (the view is passed in the foreground).
#define VIEW_UPDATE_ANIMTOOLTIP		(WM_USER+334)	///< Notify the modification of a tooltip in the view.


#define VIEW_PANES_CONTEXTMENU		(WM_USER+350)	///< Notify the activation of the context menu (right click) in the multi-panes frame.
//@}


//////////////////////////////////////////////////////////////////////
/// Used to identify the simulation mode activated by the Controller (running, reviewing backward or forward, etc).
//////////////////////////////////////////////////////////////////////
enum TSRunMode {
		RUN_NONE,		///< The simulation is paused.
		RUN_RUN,		///< The simulation is in Run mode.
		RUN_BACKWARD,	///< The simulation is reviewing backward.
		RUN_FORWARD,	///< The Simulation is reviewing forward.
		RUN_PREVSTEP,	///< The simulation is reviewing backward to the previous time-step.
		RUN_NEXTSTEP,	///< The simulation is reviewing forward to the next time-step.
		RUN_GOBEGIN,	///< The simulation is synchronised with the first time-step.
		RUN_GOEND		///< The simulation is synchronised with the last time-step.
		};
//////////////////////////////////////////////////////////////////////
/// Used to identify the nature of the current Breakpoint.
//////////////////////////////////////////////////////////////////////
enum TSBreakPt {
		BREAKP_INFO,	///< Identify an Information Breakpoint.
		BREAKP_ACTION,	///< Identify an Action Breakpoint.
		BREAKP_HYPOT,	///< Identify a Prediction Breakpoint.
		BREAKP_TEST		///< Identify a Test Breakpoint.
		};
//////////////////////////////////////////////////////////////////////
/// Used to identify the level of the Translation in an External Representation.
//////////////////////////////////////////////////////////////////////
enum TSTranslt {
		TRANS_INDEP,	///< Level of translation set to Independent (0).
		TRANS_MAPFACT,	///< Level of translation set to Map Fact (1).
		TRANS_MAPRELAT,	///< Level of translation set to Map Relation (2).
		TRANS_SUPPACT,	///< Level of translation set to Supplement Action (3).
		TRANS_DYNAL		///< Level of translation set to Fully Dyna-Linked (4).
		};
//////////////////////////////////////////////////////////////////////
/// Used to identify the current step of a Prediction performed by the user.
//////////////////////////////////////////////////////////////////////
enum TSPredict {
		PRED_NONE,		///< No prediction is currently performed.
		PRED_HYPOT,		///< The user is currently building the Hypotheses. 
		PRED_TEST		///< The user is currently verifying his/her prediction.
		};

class CSimModel;
class CInstModel;
class CLearningUnit;
class CMdEquation;
class CTimerBreakPoint;
class CExternRepUnit;

//////////////////////////////////////////////////////////////////////
/// Used to store information about a translation request and to broadcast it to the every
/// External Representations present in the framework.
//////////////////////////////////////////////////////////////////////
class CTranslationInfo
{
public:
	//////////////////////////////////////////////////////////////////////
	/// Strcture storing, in CTranslationInfo, the specific information about the data-point 
	/// involved in the translation.
	//////////////////////////////////////////////////////////////////////
	struct CTInfo 
		{
			int nTime;		///< Time-step of the simulation at which the translation is taking place.
			int nData;		///< Zero-based index of the entity involved.
			int nExpSet;	///< Zero-based index of the Experimental Set involved.
		};

public:
	CView*		m_pWndTarget;	///< A pointer to the view where the translation is initiated
	int			m_nTransLvl;	///< Level of Translation of the associated External Representation.
	int			m_nTime;		///< Time-step of the simulation at which the translation is taking place.
	
	CArray<CTInfo,CTInfo>	m_nDatas;	///< List of the data-points translated by learner.

// Construction
public:
	CTranslationInfo() 
		{	
			m_nTransLvl = m_nTime = -1;
			m_pWndTarget = NULL;
		};	
};

/////////////////////////////////////////////////////////////////////////////
/// Used in the Document/View architecture to provide the basic functionality of a DEMIST document.
///
/// The document provides for every views the information related to the current state of the simulation,
/// such as the Current Time (#m_currTimer), Run Time (#m_runTimer) and Max Time ().
/// It also stores information about the 
/// 
/// Being related to the Simulation Model (#m_pSimul) and the Instructional Model (#m_pInstruct), 
/// the document
/////////////////////////////////////////////////////////////////////////////
class CSimulDoc : public CDocument/*,public CUndo*/
{
protected: 
	CSimulDoc();
	DECLARE_DYNCREATE(CSimulDoc)

// Attributes
public:
	BOOL		m_bFirstRun;	///< TRUE if the simulation is in the first run, FALSE if the learner has already reinitialised it.
	BOOL		m_bTaskDone;	///< TRUE if the task (action or hypothesis) has been completetly performed, FALSE otherwise.
	BOOL		m_bTaskRead;	///< TRUE if the task (action or hypothesis) has been read (in the Controller) by the learner, FALSE otherwise.

	int			m_nCurrBP;		///< Zero-based time-step corresponding to the last Breakpoint reached by the simulation.
	int			m_nNextBP;		///< Zero-based time-step corresponding to the next Breakpoint to stop at
	int			m_nRunMode;		///< ID of the current Run/Review mode of the controller (see #TSRunMode)
	int			m_currTimer;	///< Current sample for timer
	int			m_runTimer;		///< Current time in run-mode
	int			m_nPredMode;	///< ID of the current Prediction Mode (see #TSPredict)

	int			m_nModSelItem;	///< Zero-based index of the current Simulation Model (used in the Simulation Design Interface).
	int			m_CurrInst;		///< Zero-based index of the current Instructional Unit (used in the Context Design Interface).
	CSimModel	*m_pSimul;		///< Pointer to the Simulation Model associated with this DEMIST document.
	CInstModel	*m_pInstruct;	///< Pointer to the Context Model associated with this DEMIST document.

	CxUserOutput	m_cUserInput;	///< List of user's production realised in all the ERs.
	//typedef CMap<int,int,CUserOutput,CUserOutput&> CTUserOutput;
protected:
	CString		m_Model;		///< Description of this DEMIST document (internal use only)
private:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSimulDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual void OnCloseDocument();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSimulDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	CLearningUnit*	GetCurrentLU();
	CMdEquation*	GetCurrentModel();
	int				GetMaxTime();
	int				GetCurrTime();
	int				GetRunTime();

	BOOL OnCreateMultiPanes(CObject *pWndList,CCreateContext* pContext,CMDIChildWnd *pChildWnd);

	void UpdateViewMenu();
	void UpdateViewMenuDrop(CWnd* pWnd,CRect rLoc);
	BOOL OnCloseER(CView *pView);
	void UpdateDoc(CView* pSender);
	void UpdateTimerDoc(CView* pSender);
	void UpdateRestartSimul(BOOL bAlert=TRUE);
	void UpdateMapRelation(CView* pSender, CTranslationInfo* pTransInfo);
	void UpdateHypothesis(CView* pSender, CUserData *pData);
	void UpdateTest(CView* pSender, CUserData *pData);
	void UpdateAction(CView* pSender, CUserData *pData);
	void UpdatebreakPoint(CView* pSender, CTimerBreakPoint *pData);
	BOOL UpdateUserData(CView* pSender, CUserData *pData);					
	BOOL UpdateERUserData(CExternRepUnit *pExtRep, CUserData *pData);		
	BOOL OnActivateNextLU(BOOL bMsg=TRUE);
	BOOL OnActivatePrevLU(BOOL bMsg=TRUE);
	BOOL OnActivateCurrentLU(BOOL bMsg=TRUE);
	BOOL OnActivateInitLU();
	BOOL OnLaunchStartER();
	BOOL OnShowTask();
//	int GetInfoSize() { return m_NbSet;};
//	void OpenNewViewSet();
	//void CalculateModel();

protected:
	//void InitDocument();

// Generated message map functions
protected:
	//{{AFX_MSG(CSimulDoc)
	afx_msg void OnCmdLauchER(UINT nID);
	afx_msg void OnUpdateCmdLauchER(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SIMULDOC_H__4095155B_F048_11D3_A2F4_00D0B71C8709__INCLUDED_)
