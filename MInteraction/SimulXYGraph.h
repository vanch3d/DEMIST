#if !defined(AFX_SIMULXYGRAPH_H__6653B9A0_0ECB_11D4_A2F6_00D0B71C8709__INCLUDED_)
#define AFX_SIMULXYGRAPH_H__6653B9A0_0ECB_11D4_A2F6_00D0B71C8709__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SimulXYGraph.h : header file
//

//#include "graph\\graphwnd.h"
#include "Tools\Scatter.h"
#include "XYGraphData.h"

class CExternRepUnit;
class CFormatPhasePlot;
class CGraphics;

/////////////////////////////////////////////////////////////////////////////
/// A CScrollView-derived class used to implement the interface of a X-Y Graph ER.
///
///
/////////////////////////////////////////////////////////////////////////////
class CViewPhasePlot : public CScrollView
{
protected:
	CViewPhasePlot();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CViewPhasePlot)

	CFormatPhasePlot* GetFormats();
	//CDlgActionGraph* GetActions();

// Attributes
public:
	BOOL				m_bCanEdit;
	CPhasePlotData*		m_pSelData;
	CPhasePlotData*		m_pHypData;
	CPhasePlotData*		m_pHypData2;
	CPhasePlotData*		m_pHypData3;

	CxPPDataSet			m_cPPData;			// List of MER
	CxPPDataSet			m_cUserData;		// List of UserData
	CxPPDataSet			m_cIsocline;		// List of UserData
	
	CLinear<double>			m_Linear;
	CXLogYLinear<double>	m_XLogYLinear;
	CXLinearYLog<double>	m_XLinearYLog;
	CXLogYLog<double>		m_XLogYLog;
	CLinear<double>*		m_pGraph;

	double				m_nDeltaTime;

//	float  m_fXScale;
//	float  m_fYScale;
	float  m_fDelta;
	int	   m_nMAPMode;
	int	   m_nTransData;

//	double x[500];
//	double y[500];
//	double x1[500];
//	double y1[500];
	
	CTPoint<double> xRange,
					yRange;

	int		m_nMaxTime;
	int		m_nXData;
	int		m_nYData;

	int	 m_nScale;
	BOOL m_bCanDraw;
	bool m_bMemDraw;
	BOOL m_bTitle;
	bool m_bLegend;
	BOOL m_bBoundary;
	BOOL m_bGrid;
	BOOL m_bStabPt;
	BOOL m_bCurrT;
	COLORREF m_nBkColor;

	CString			m_strTitle,
					m_strXPlot,
					m_strYPlot;

	CToolTipCtrl	myToolTip;
protected:
	CExternRepUnit*		m_pER;
	CToolBar			m_cToolBar;
//	int				m_nCurrTTask;
	int				m_nCurrTime;

// Operations
public:
	CSimulDoc* GetDocument();
	int					GetTranslation();

	BOOL CanEdit();
	int  HitTest(CPoint mPt,CPhasePlotData *pData,int nLim=-1);


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewPhasePlot)
	public:
	virtual void OnInitialUpdate();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrintPreview(CDC* pDC, CPrintInfo* pInfo, POINT point, CPreviewView* pView);
	//}}AFX_VIRTUAL

	BOOL OnMoveData(CPhasePlotData* m_pHypData,CPoint point);
	LRESULT OnUpdateObjTooltip(WPARAM wp, LPARAM lp);

	BOOL OnUpdateData();									// Response to data update
	BOOL OnUpdateTimer();									// Response to timer update
	BOOL OnUpdateBreakPoint(int nBPTime,int nBPType);		// Response to a breakpoint
	BOOL OnUpdateHypothesis(CUserData *pData);				// Response to an hypothesis
	BOOL OnUpdateAction(CUserData *pData);					// Response to an action
	BOOL OnUpdateMapRelation(CTranslationInfo *mTInfo );	// Response to a MAP RELATION 


	void DrawFrame(CRect& rect, COLORREF cr, const char* Title);
	void RecalcSize();

	BOOL SetScaleType(int nScale=0);
// Implementation
protected:
	virtual ~CViewPhasePlot();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CViewPhasePlot)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnFilePrintPreview();
	afx_msg void OnSelectData();
	afx_msg void OnDestroy();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnEditCopy();
	//}}AFX_MSG
	afx_msg void OnSetScaleType(UINT nID);
	afx_msg void OnUpdateScaleType(CCmdUI* pCmdUI);
	afx_msg LRESULT OnUpdateInitContent(WPARAM, LPARAM); 
	afx_msg LRESULT OnActivateER(WPARAM, LPARAM); 
	afx_msg void OnTranslationTasks(UINT nID);
	afx_msg void OnUpdateTransTasks(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in SimulView.cpp
inline CSimulDoc* CViewPhasePlot::GetDocument()
   { return (CSimulDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SIMULXYGRAPH_H__6653B9A0_0ECB_11D4_A2F6_00D0B71C8709__INCLUDED_)
