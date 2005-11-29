//////////////////////////////////////////////////////////////////////
/// @file LearningUnit.h
/// @brief Interface for the Instructional Model.
///
/// This file defines the various elements required by the Instructional Model, i.e. 
/// The Intructional Model itself (CInstModel), the Learning Units (CLearningUnit), 
/// the Instructional Activities (or breakpoint, CTimerBreakPoint), the External 
/// Representations (CExternRepUnit) and the Ouctomes (COutcomes).
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_LEARNINGUNIT_H__156A3488_1C20_11D4_A2FA_00D0B71C8709__INCLUDED_)
#define AFX_LEARNINGUNIT_H__156A3488_1C20_11D4_A2FA_00D0B71C8709__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "UserOutput.h"
#include <MInteraction/Format.h>
#include <MSimulation/Model.h>


class CSimulDoc;
class COutcomes;
class CExternRepUnit;
class CTimerBreakPoint;
class CLearningUnit;

typedef CTypedPtrArray<CObArray,COutcomes*>			CxOutcomes;		///< Type defining a set of Outcomes (array of pointers)
typedef CTypedPtrArray<CObArray,CExternRepUnit*>	CxMERSet;		///< Type defining a set of External Representations (array of pointers)
typedef CTypedPtrArray<CObArray,CTimerBreakPoint*>	CxTBreakSet;	///< Type defining a set of BreakPoint (array of pointers)
typedef	CTypedPtrArray<CObArray,CLearningUnit*>		CxLearnUnit;		///< Type defining a set of Learning Unit

//////////////////////////////////////////////////////////////////////
/// Class storing the information about the outcomes to be displayed in an External Representation.
///
/// Outcomes refer to a combination of an entity extracted from the simulation model (i.e. 
/// a variable, a parameter, an aggregate) and of one of the Experimental set defined in the
/// Learning Unit. Each outcome specifies which information calculated according to which 
/// initial condition will be made available to the learner in this ER. 
/// Each outcome can be displayed (the #m_nSelected attribute) or not in the ER and this status 
/// can be modified (the #m_nAllow attributre) or not by the learner (i.e. the outcome can be freely 
///	displayed at learner's initiative).
//////////////////////////////////////////////////////////////////////
class COutcomes : public CObject
{
public:
	DECLARE_SERIAL(COutcomes)

	COutcomes();
	COutcomes*	Clone();
	void	Serialize( CArchive& ar );
public:
	int			m_nExpSet;			///< Zero-based index of the Experimental Set (see CMdExpertSet) associated with this outcome.
	int			m_nData;			///< Zero-based index of the Entity (see CModelObj) associated with this outcome.
	int			m_nSelected;		///< If TRUE, the outcome will be displayed in the ER.
	int			m_nAllow;			///< If TRUE, the user will be able to select or not this outcome in the ER.
	//CObject*	m_pFormat;
};

//////////////////////////////////////////////////////////////////////
// OBOSLETE : used for backward compatibility
//////////////////////////////////////////////////////////////////////
class COutputs : public COutcomes
{
public:
	DECLARE_SERIAL(COutputs)
	COutputs() : COutcomes() {};
};


//////////////////////////////////////////////////////////////////////
/// Class implementing an External Representation (ER) and gathering all the information required to define it.
///
/// This class defines an External Representation that will be used in a Learning Unit
/// to present to the learner the data computed from the Simulation Model.
/// Each ER is defined by its type, stored in the #m_IdMER attribute:
///		- IDR_SIMULPIE refers to a Pie Chart.
///		- IDR_SIMULTEXT refers to a Static Equation of the model.
///		- IDR_SIMULGRAPH refers to a X-Time graph.
///		- IDR_SIMULANIM refers to a generic 2D Animation.
///		- IDR_SIMULTABLE refers to a Table.
///		- IDR_SIMULHISTOGRAM refers to a Bar Chart.
///		- IDR_SIMULXYGRAPH refers to a X-Y Graph.
///		- IDR_SIMULTIMER refers to a Timer (i.e. simnple representation of time).
///
/// Each of the ERs added in a Learning Unit is then specified by the format of the ER
/// (a CFormat instance stored in the #m_cFormatSet attribute), the outcomes made available 
/// to the learner (the #m_cOutputSet attribute) and the level of translation supported by the
/// ER (a CFormatTranslation also stored in the #m_cFormatSet attribute).
//////////////////////////////////////////////////////////////////////
class CExternRepUnit : public CObject  
{
public:
	DECLARE_SERIAL(CExternRepUnit)

	CExternRepUnit(UINT id=-1);
	virtual ~CExternRepUnit();
	virtual CExternRepUnit*	Clone();

	void	OnDraw(CDC* pDC,BOOL bUserMode=FALSE,CImageList *m_ImageList=NULL);
	void	Serialize( CArchive& ar );
	int		HitTest(CPoint Hitpt,CRect &mrect);

	//BOOL	GetTranslation() { return m_bTrans;};
	//void	SetTranslation(BOOL tr=TRUE) { m_bTrans=tr;};
	int		GetTranslationLevel();
	CString	GetName() {return m_sDesc;};
	void	SetName(CString mstr) { m_sDesc = mstr;};
	void	SetSelected(BOOL sel=TRUE) {m_bSelected=sel;};
	void	SetID(UINT mId);
	UINT	GetID(){return m_IdMER;};
	void	SetIcon(UINT mId) {m_nIcon = mId;};
	UINT	GetIcon(){return m_nIcon;};

	void	SetERRect(CRect mrect) { mrect.NormalizeRect();m_locRect = mrect;};
	CRect	GetERRect() { return m_locRect;};
protected:
	CString		m_sDesc;		///< ER's Description (title)
	UINT		m_IdMER;		///< ER's Resource ID (type of the ER)
	UINT		m_nIcon;		///< ER's Icon ID
	BOOL		m_bTrans;		///< OBSOLETE : Is Translation allowed in the ER ?
private:
	CRect		m_locRect;		///< Bounding Rect of the ER in the view (cf. CViewCDI)
	BOOL		m_bSelected;	///< TRUE if the ER is selected in the CViewCDI, FALSE otherwise
public:
	int			m_nMode;		///< OBSOLETE : Simulation Mode (0: RUN, 1: REVIEW)
	BOOL		m_bIsReady;		///< TRUE if the ER is ready to be used (i.e. totally defined), FALSE otherwise
	BOOL		m_bOpenStart;	///< TRUE if the ER is automatically launched at the start of the Learning unit
	BOOL		m_bKeepOpen;	///< TRUE if the ER cannot be closed by the learner (m_bOpenStart has to be TRUE as well)
	CString		m_sCategory;	///< Category assigned to this ER (for analysis purpose)

	UINT		m_CmDId;		///< Command ID associated with this ER (i.e. the command in the ER Selector or in the ToolBar)
	CView*		m_pExtView;		///< Pointer to the CView-derived interface of the ER (NULL if the ER is not yet open)
	
	WINDOWPLACEMENT	m_pExtViewPl;	///< Position of the ER in the workspace (stored to keep them systematically at the same location)

	CxOutcomes		m_cOutputSet;	///< List of ER's Outcomes
	CxFormat		m_cFormatSet;	///< List of ER's Formats & Translation
	CxUserOutput	m_cUserInput;	///< List of user's production realised in this ER.

	//**********	CTypedPtrArray<CObArray,CPropertyPage*>	m_cFormatSet;	// List of Formats/Interactions
};


//////////////////////////////////////////////////////////////////////
/// Class implementing an activity that will take place at a given time during the simulation.
/// 
/// This class represents the simple Instructional Activities defined in DEMIST.
/// It specifies a time-point (#m_tBreakPt) where the simulation will stop and require the intervention
/// of the learner. There is 4 types of intervention in DEMIST (see #m_nBPType and the #TSBreakPt enum type) :
///		- BREAKP_INFO, a simple information is provided to the learner without further action.
///		- BREAKP_ACTION, an action on the data-points at the current time is required from the learner.
///		- BREAKP_HYPOT, the learner is required to predict the value of a data-point in the future.
///		- BREAKP_TEST, after a prediction, the learner is required to check his/her guess.
///
/// Each of these interventions is associated with a text displayed in a dialog box (#m_sDesc) and, 
/// if required, to the part of an HTML file containing more information about the requirements (#m_sHTML).
/// If the breakpoint is created by learners (#m_bUserDefined set to TRUE), then it is automatically destroyed 
/// when the simulation is reinitialised. If it is created by the authors (#m_bUserDefined set to FALSE),
/// the breakpoint remains available after a reinitialisation.
//////////////////////////////////////////////////////////////////////
class CTimerBreakPoint : public CObject
{
public:
	DECLARE_SERIAL(CTimerBreakPoint)

	CTimerBreakPoint();

    void operator=(const CTimerBreakPoint& cTB);

	virtual CTimerBreakPoint*	Clone();
	void	Serialize( CArchive& ar );
public:
	BOOL	m_bUserDefined;		///< TRUE if defined by learner, FALSE if defined by author.
	int		m_tBreakPt;			///< Zero-based time-step where the breakpoint will oocur.
	int		m_nBPType;			///< Nature of the breakpoint (see #TSBreakPt)
	CString	m_sDesc;			///< Description of the breakpoint, as it will be presented to the learner for support.
	CString	m_sHTML;			///< Breakpoint associated HTML file or anchor.			
};


//////////////////////////////////////////////////////////////////////
/// Class implementing a Learning Unit, the elementary part of an instructional scenario.
///
/// Each Learning Unit (LU) represents an independent step in the scenario, defined by a Simulation 
/// Model to explore, several External Represensations available to support this exploration and
/// some instrutional activities possibly required from the learner.
/// The instructional scenario associated with the LU is referenced by the #m_simulID and 
/// #m_pEquModel attributes, as well as the number of time-steps (#m_nTimer) 
/// that will be computed by the simulation engine.
/// The data generated from the Simulation Model are stored in #m_cOutComesSet and will
/// be accessed by the ERs to be displayed accordingly.
//////////////////////////////////////////////////////////////////////
class CLearningUnit : public CObject  
{
public:
	DECLARE_SERIAL(CLearningUnit)

	CLearningUnit(CSimulDoc *pDoc=0,int SimulId=0);
	CLearningUnit(const CLearningUnit& mLU);
	virtual ~CLearningUnit();
	virtual CLearningUnit*	Clone();

//	BOOL	PreTranslateMessage(MSG* pMsg);
//	BOOL	CreateToolTip(CView *pView);
 	void	OnUpdateToolTip(CView *pView,CToolTipCtrl *pToolTip);
	void	OnDraw(CDC* pDC,BOOL bUserMode=FALSE,BOOL bActive=FALSE,CImageList *pImageList=NULL);
	void	Serialize( CArchive& ar );
	CPoint	HitTest(CPoint Hitpt,CRect &mrect);

	int		UpdateERMenu(CView *pView);
	CMenu*	GetERsMenu();
	BOOL	LauchER(UINT nID);
	BOOL	UpdateLauchER(CCmdUI* pCmdUI);
	BOOL	OnCloseER(CView *pView);
	int		GetActiveERNumber();
	BOOL	OnCreateMultiPanes(CObject *pWndList,CCreateContext* pContext,CMDIChildWnd *pChildWnd);

	CRect	OnNewLearningUnit();
	void	OnUpdateUnit();
	int		AddExternalRep();
	BOOL	RemoveExternalRep(int nbER);
	BOOL	OnCloseAllER();

	void	ClearUnit();
	int		AddDefaultExpSet(int nSimId);
	int		RemoveExpSet(int n_expset);

	int		GetMaxTimer() { return m_nMaxTimer;};
	int		GetDataSize() { return m_cDataPoints.GetSize();};
	CMdData*	GetDataAt(int nIdx=0);

	BOOL	GetTimerBPAt(int nTime,CTimerBreakPoint& pBP);
	void	CleanTimerBP();

	void	TranslateBy(CSize cs);
//	void	SetLocation(CPoint pt) {m_locPt = pt;};
	void	SetLURect(CRect mrect) { mrect.NormalizeRect();m_inRect = mrect;};
	CRect	GetLURect() { return m_inRect;};
	CPoint	GetLUCenter() { return m_locPt;};
	void	SetLUCenter(CPoint mpt) { m_locPt = mpt;};
	CRect	GetBoundRect() { return m_locRect;};
protected:
	CList<CSize,CSize>	m_cDefaultPos;		///< List of available positions for a new ER.
	BOOL				m_bCanCloseAll;		///< TRUE if all the ERs can be closed automatically, FALSe otherwise.
	CPoint				m_locPt;			///< Center-point (in view coordinates) of the Learning Unit.
	CRect				m_locRect,			///< Internal bounding rect of the Learning Unit (excluding the title).
						m_inRect;			///< External bounding rect of the Learning Unit (including the title).
public:
	BOOL			m_bCanStart;			//   NOT YET IMPLEMENTED
	CString			m_sLUDesc;				//	 NOT YET IMPLEMENTED: Description of the Learning Unit.
	CString			m_sLUGoal;				//   NOT YET IMPLEMENTED: Objectives of the Learning Unit.

	BOOL			m_bIsReady;				///< TRUE if the LU is ready to be used (i.e. totally defined), FALSE otherwise.
	BOOL			m_bFirstRun;			///< TRUE if the first run of the simulation occurs without breakpoint, FALSE otherwise.
	BOOL			m_bActRequest;			///< TRUE if actions can be defined by learners, FALSE if they are defined by authors.exclusively.
	BOOL			m_bShowAbbrev;			///< TRUE if the ERs use the abbreviations for displaying the entities' name, FALSE if they use the full name.
	BOOL			m_bExpSetLocked;		///< TRUE if the Experimental Sets are not available to learners (in the Controller), FALSE otherwise.
	int				m_nbERMax;				///< Number of maximum simultaneous ERs (0 if no limit).
	int				m_nMaxTimer;			///< Number of data-points generated for this Learning Unit.
	
	CString			m_sLUName;				///< Name of the LU (as displayed in the CDI).
	CString			m_sLUHTML;				///< Link to the HTML file or anchor associated with the LU (if any).
	int				m_nLUID;				///< ID of this Learning Unit in the Instructional Scenario.
	int				m_nSimulID;				///< Zero-based index of the simulation model associated with the Learning Unit.

	CxMERSet		m_cMERSet;				///< List of the ERs (CExternRepUnit) available in the LU.
	CxTBreakSet		m_cTBreakSet;			///< List of BreakPoint (CTimerBreakPoint)
	CxOutcomes		m_cDefOutputSet;		///< List of Outcomes (COutcomes)
	CxExpSet		m_cExpSet;				///< List of Experimental Set (CMdExpertSet)
	CxExpSet		m_cDefExpSet;			///< List of default Experimental Set (CMdExpertSet)
	CxModelData		m_cDataPoints;			///< List of the data-points (CMdData) computed by the simulation model.
	

	CMdEquation		*m_pEquModel;			///< Pointer to the Mathematical Model associated with this LU
	CSimulDoc		*m_pLUDoc;				///< Pointer to the Document associated with this LU

	//CMap<int,int,CTimerBreakPoint,CTimerBreakPoint&>	m_cTTTT;
	//CTypedPtrArray<CObArray,CExternRepUnit*>	m_cMERSet;			
	//CTypedPtrArray<CObArray,CTimerBreakPoint*>	m_cTBreakSet;
	//CTypedPtrArray<CObArray,CMdExpertSet*>		m_cExpSet;
	//CTypedPtrArray<CObArray,CMdExpertSet*>		m_cDefExpSet;
	//CTypedPtrArray<CObArray,CMdData*>			m_cOutComesSet;
};

//////////////////////////////////////////////////////////////////////
/// Class implementing DEMIST's Context Model, the central part of the design process.
///
/// This class simply maintains a list of the Learning Units (#m_cLUSet) which 
/// sequence constitutes the instructional scenario. It also provides the methods used by the 
/// Instructional Design Interface (see CViewCDI) to display the scenario and manage the 
/// interaction with it.
//////////////////////////////////////////////////////////////////////
class CInstModel : public CObject  
{
public:
	DECLARE_SERIAL(CInstModel)

	CInstModel(CSimulDoc *pDoc=0);
	virtual ~CInstModel();

	virtual void	OnNewModel();
	virtual void	OnInitiaUpdate();
	CLearningUnit*	AddLearningUnit(CLearningUnit* pLU=NULL);

	CPoint	HitTest(CPoint Hitpt,CRect &mrect);
//	void	OnSetToolTip(CView *pView,CToolTipCtrl *m_ToolTip);
//	void	OnDrawIcon(CDC* pDC,int iNb,CPoint pos);
	void	OnUpdatePosition();
	void	OnDraw(CDC* pDC,CImageList *pImageList);
	void	Serialize( CArchive& ar );
protected:
	//	CImageList	m_ImageList;	// Icon List

	CString		m_sDesc;		// NOT YET USED: Description of the instructional scenario
	CString		m_sGoal;		// NOT YET USED: General objectives / assignments of the scenario
	CRect		m_BndRect;		///< Bounding Rect of the scenario's interface, used in the CViewCDI.
public:
	CSimulDoc*	m_pDoc;			///< Pointer to the document with which this scenario is associated
	CxLearnUnit	m_cLUSet;		///< List of Learning Units (CLearningUnit) existing in the scenario

	//CTypedPtrArray<CObArray,CLearningUnit*>	m_cLUSet;	///< List of Learning Unit
};

#endif // !defined(AFX_LEARNINGUNIT_H__156A3488_1C20_11D4_A2FA_00D0B71C8709__INCLUDED_)
