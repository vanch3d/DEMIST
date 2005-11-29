#if !defined(AFX_INSTRUCTVIEW_H__630A6840_1C43_11D4_A2FA_00D0B71C8709__INCLUDED_)
#define AFX_INSTRUCTVIEW_H__630A6840_1C43_11D4_A2FA_00D0B71C8709__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InstructView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
/// Class implementing the Context Design Interface.
/////////////////////////////////////////////////////////////////////////////
class CViewCDI : public CScrollView//CView
{
protected:
	CViewCDI();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CViewCDI)

// Attributes
public:
	CSimulDoc* GetDocument();

protected:
	CSize			m_DocSize;			// Size for the Scroll Area
	LOGFONT			m_logFont;			// Display & print font
	CImageList		m_ImageList;
	CRectTracker	m_Tracker;			// Tracker for component
	int				m_CurrUnit,			// Current Selected Unit
					m_CurrER;			// Current Selected ER

	CToolTipCtrl	m_wndToolTip;
	CToolBar		m_cToolBar;

// Operations
public:
	bool IsViewActive();
	//void OnLaunchStartER();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewCDI)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CViewCDI();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CViewCDI)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg int	OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnUpdateCmdEdit(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCmdER(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCmdUnit(CCmdUI* pCmdUI);
	afx_msg void OnEditCopy();
	afx_msg void OnEditDelete();
	afx_msg void OnEditPaste();
	afx_msg void OnAddER();
	afx_msg void OnAddUnit();
	afx_msg void OnLaunchER();
	afx_msg void OnLUProperty();
	afx_msg void OnERProperty();
	afx_msg void OnSaveERPos();
	afx_msg void OnActivateNextLU();
	afx_msg void OnActivatePrevLU();
	afx_msg void OnActivateCurrentLU();
	afx_msg void OnUpdateLU(CCmdUI* pCmdUI);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSortER();
	afx_msg void OnUpdateSortER(CCmdUI* pCmdUI);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG

	LRESULT OnUpdateObjTooltip(WPARAM wp, LPARAM lp=0);

	void MessageError(UINT msg);

	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in SimulView.cpp
inline CSimulDoc* CViewCDI::GetDocument()
   { return (CSimulDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INSTRUCTVIEW_H__630A6840_1C43_11D4_A2FA_00D0B71C8709__INCLUDED_)
