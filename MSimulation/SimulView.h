// SimulView.h : interface of the CViewSDI class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_SIMULVIEW_H__4095155D_F048_11D3_A2F4_00D0B71C8709__INCLUDED_)
#define AFX_SIMULVIEW_H__4095155D_F048_11D3_A2F4_00D0B71C8709__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
/// Interface for the textual description in the Simulation Design Interface.
/////////////////////////////////////////////////////////////////////////////
class CViewSDI : public CScrollView
{
protected: // create from serialization only
	CViewSDI();
	DECLARE_DYNCREATE(CViewSDI)

// Attributes
public:
	CSimulDoc* GetDocument();
protected:
	CSize			m_DocSize;			///< Size for the Scroll Area
	CRectTracker	m_Tracker;			///< Tracker for component
	LOGFONT			m_logFont;			///< Display & print font
	CString			m_CurrText;			///<
	CImageList		m_ImageList;		///<

	int				m_CurrModel;		///< Current selected model

	CArray<CRect,CRect>  m_datapos;		///<
	CArray<CRect,CRect>  m_simpos;		///<
	CListCtrl		m_cListModel;		///< List of Model


// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewSDI)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
	virtual void OnInitialUpdate();
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
public:
	//CRect DrawParam(CDC* pDC,CPoint sPt);
	//CRect DrawModels(CDC* pDC,CPoint sPt);
	//CRect DrawIcon(CDC* pDC,CPoint sPt,int nIcon,CString sTitle,BOOL sel=FALSE,int width=80);
	//CRect DrawGrid(CDC* pDC,CPoint sPt);
	CRect DrawDef(CDC* pDC,CPoint sPt);
	void  DrawView(CDC* pDC);  // Draw the simulations
	virtual ~CViewSDI();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CViewSDI)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnEditCopy();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSimulationUpdatemodels();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in SimulView.cpp
inline CSimulDoc* CViewSDI::GetDocument()
   { return (CSimulDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SIMULVIEW_H__4095155D_F048_11D3_A2F4_00D0B71C8709__INCLUDED_)
