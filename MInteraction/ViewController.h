#if !defined(AFX_VIEWCONTROLLER_H__BC222C05_3403_11D5_A2FA_00D0B71C8709__INCLUDED_)
#define AFX_VIEWCONTROLLER_H__BC222C05_3403_11D5_A2FA_00D0B71C8709__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ViewController.h : header file
//

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

//#include "SelSliderCtrl.h"
#include "GridCtrl\GridCtrl.h"
#include "Tools\pushpin.h"
#include "Tools\BtnST.h"

/////////////////////////////////////////////////////////////////////////////
/// Class used to implement the interface for the Controller.
///
/// The Controller is the primary tool used in DEMIST to run, review, restart, the simulation.
/////////////////////////////////////////////////////////////////////////////
class CViewController : public CFormView
{
protected:
	CViewController();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CViewController)

// Form Data
public:
	//{{AFX_DATA(CViewController)
	enum { IDD = IDD_VIEW_CONTROLLER };
	CButtonST	m_cReset;
	CButtonST	m_cApply;
	CButtonST	m_cNextUnit;
	CButtonST	m_cTask;
	CButtonST	m_cStop;
	CButtonST	m_cStart;
	CButtonST	m_cRestart;
	CButtonST	m_cRun;
	CButtonST	m_cPrev;
	CButtonST	m_cNext;
	CButtonST	m_cEnd;
	CButtonST	m_cBegin;
	CButtonST	m_cNextStep;
	CButtonST	m_cPrevStep;
	CButtonST	m_cAddBPA;
	CButtonST	m_cAddBPH;
	CPushPinButton	m_cExpand;
	//}}AFX_DATA
	CGridCtrl	m_Parlist;

// Attributes
public:
	CImageList	m_ImageList;
	CImageList	m_ImageTaskList;
	CImageList	m_ImageRunList;
	CImageList	m_ImageUnitList;
	CImageList	m_ImageAddBPList;
	int			m_nMaxTime;	// Time reached by run mode
	BOOL		m_bIsRunning;
	BOOL		m_bIsExpanded;
	//int			m_nBackReview;	// Review Mode (0 None, 1 back, 2 for)
	int			m_nMapMode;
	CSize		m_siTotal,
				m_siPage,
				m_siLine;
protected:
	BOOL		m_bIsLocked;	// Init. Values modifiable

// Operations
public:
	CSimulDoc* GetDocument();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewController)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CViewController();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	void EnableButtons();
	void OnTimerInit();
	void OnRunSimulation(UINT nIDEvent);
	void OnStopSimulation(BOOL bBP);
	void OnStopSimulationAtBP(int nBP);
	void OnPrepareNextBP();
	// Generated message map functions
	//{{AFX_MSG(CViewController)
	afx_msg void OnTimerStart();
	afx_msg void OnTimerStop();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnReviewPrev();
	afx_msg void OnReviewStop();
	afx_msg void OnReviewNext();
	afx_msg void OnReviewBegin();
	afx_msg void OnReviewEnd();
	afx_msg void OnExpandDlg();
	afx_msg void OnDestroy();
	afx_msg void OnExpSetApply();
	afx_msg void OnExpSetReset();
	afx_msg void OnExpSetAdd();
	afx_msg void OnExpSetDelete();
	afx_msg void OnTimerRestart();
	afx_msg void OnReadTask();
	afx_msg void OnNextUnit();
	afx_msg void OnUpdateNextUnit(CCmdUI* pCmdUI);
	afx_msg void OnReviewPrevStep();
	afx_msg void OnReviewNextStep();
	afx_msg void OnUpdateApplyChange(CCmdUI* pCmdUI);
	afx_msg void OnClose();
	afx_msg void OnNewBP();
	//}}AFX_MSG
	afx_msg LRESULT OnActivateER(WPARAM, LPARAM); 
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in simulView.cpp
inline CSimulDoc* CViewController::GetDocument()
   { return (CSimulDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEWCONTROLLER_H__BC222C05_3403_11D5_A2FA_00D0B71C8709__INCLUDED_)
