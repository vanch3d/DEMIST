#if !defined(AFX_SIMULGRAPH_H__A1BA0980_F1A8_11D3_A2F4_00D0B71C8709__INCLUDED_)
#define AFX_SIMULGRAPH_H__A1BA0980_F1A8_11D3_A2F4_00D0B71C8709__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SimulGraph.h : header file
//

class CExternRepUnit;
class CFormatGraph;

class CPhasePlotData;

#include "Tools\Scatter.h"
#include "XYGraphData.h"


/////////////////////////////////////////////////////////////////////////////
/// Class used to implement the interface of a X-Time Graph ER.
///
///
/////////////////////////////////////////////////////////////////////////////
class CViewGraph : public CScrollView
{
protected:
	CViewGraph();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CViewGraph)

// Attributes
public:
	//int			m_StartData, m_EndData;
protected:
	CToolBar			m_wndToolBar;
	CToolTipCtrl		m_wndToolTip;
	CExternRepUnit*		m_pER;				///< Pointer to 
	CLinear<double>*	m_pGraph;			///< Pointer to the XY-graph displayed in the view

	CxPPDataSet			m_cPPData;			///< List of MER
	CxPPDataSet			m_cUserData;		///< List of UserData

	int					m_nTransData;
	int					m_nHypData;
	int					m_nCurrTime;
	double				m_nDeltaTime;
	BOOL				m_bCanEdit;

	CLinear<double>			m_Linear;
	CXLinearYLog<double>	m_XLinearYLog;	///<

	CTPoint<double> xRange,
					yRange;

	int		m_nMaxTime;

private:
	CPhasePlotData*		m_pSelData;			///< Pointer to the dataset currently selected by the user.
	CPhasePlotData*		m_pHypData;			///< Pointer to the dataset currently used for a prediction.

// Operations
public:
	CSimulDoc*		GetDocument();
	CFormatGraph*	GetFormats();
	int				GetTranslation();

	BOOL IsViewActive();
	void OnDrawFrame(CRect& rect, COLORREF cr, const char* Title);
	void RecalcSize();
	BOOL SetScaleType(int nScale=0);

	BOOL CanEdit();
	int  HitTest(CPoint mPt,CPhasePlotData *pData,int nLim=-1);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewGraph)
	public:
	virtual void OnInitialUpdate();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

	LRESULT OnUpdateObjTooltip(WPARAM wp, LPARAM lp);

	BOOL OnUpdateData();									// Response to data update
	BOOL OnUpdateTimer();									// Response to timer update
	BOOL OnUpdateBreakPoint(int nBPTime,int nBPType);		// Response to a breakpoint
	BOOL OnUpdateHypothesis(CUserData *pData);				// Response to an hypothesis
	BOOL OnUpdateAction(CUserData *pData);					// Response to an action
	BOOL OnUpdateMapRelation(CTranslationInfo *mTInfo );	// Response to a MAP RELATION 

// Implementation
protected:
	virtual ~CViewGraph();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CViewGraph)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSelectdata();
	afx_msg void OnEditCopy();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	afx_msg BOOL	OnToolTipNeedText(UINT id, NMHDR * pNMHDR, LRESULT * pResult);
	afx_msg LRESULT OnUpdateInitContent(WPARAM, LPARAM); 
	afx_msg LRESULT OnActivateER(WPARAM, LPARAM); 
	afx_msg void OnUpdateTransTasks(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in simulView.cpp
inline CSimulDoc* CViewGraph::GetDocument()
   { return (CSimulDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SIMULGRAPH_H__A1BA0980_F1A8_11D3_A2F4_00D0B71C8709__INCLUDED_)
