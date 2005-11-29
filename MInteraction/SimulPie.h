#if !defined(AFX_SIMULPIE_H__40951583_F048_11D3_A2F4_00D0B71C8709__INCLUDED_)
#define AFX_SIMULPIE_H__40951583_F048_11D3_A2F4_00D0B71C8709__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SimulPie.h : header file
//

#include "Tools\Graph.h"
class CExternRepUnit;
class CFormatChart;

/////////////////////////////////////////////////////////////////////////////
/// Class used to implement the interface of both the Bar Chart and Pie Chart ER.
///
///
/////////////////////////////////////////////////////////////////////////////
class CViewPie : public CView
{
protected:
	CViewPie();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CViewPie)

// Attributes
public:
	CGraph			m_PieChart;
	BOOL			m_bComplete;
protected:
	CToolBar			m_cToolBar;
	CExternRepUnit	*m_pER;
	int				m_nCurrTime;
//	int				m_nCurrTTask;
	int				m_nMaxRange;
	int				m_nMinRange;

	int				m_nSelExp,m_nSelOut;

// Operations
public:
	CSimulDoc*		GetDocument();
	CFormatChart*	GetFormats();
	int				GetTranslation();
		
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewPie)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

	BOOL OnUpdateData();									// Response to data update
	BOOL OnUpdateTimer();									// Response to timer update
	BOOL OnUpdateBreakPoint(int nBPTime,int nBPType);		// Response to a breakpoint
	BOOL OnUpdateHypothesis(CUserData *pData);				// Response to an hypothesis
	BOOL OnUpdateAction(CUserData *pData);					// Response to an action
	BOOL OnUpdateMapRelation(CTranslationInfo *mTInfo );	// Response to a MAP RELATION 

// Implementation
protected:
	virtual ~CViewPie();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CViewPie)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnChartBar();
	afx_msg void OnChartLine();
	afx_msg void OnChartPie();
	afx_msg void OnUpdateCharts(CCmdUI* pCmdUI);
	afx_msg void OnEditCopy();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	afx_msg LRESULT OnUpdateInitContent(WPARAM, LPARAM); 
	afx_msg LRESULT OnActivateER(WPARAM, LPARAM); 
	afx_msg void OnTranslationTasks(UINT nID);
	afx_msg void OnUpdateTransTasks(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in simulView.cpp
inline CSimulDoc* CViewPie::GetDocument()
   { return (CSimulDoc*)m_pDocument; }
#endif
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SIMULPIE_H__40951583_F048_11D3_A2F4_00D0B71C8709__INCLUDED_)
