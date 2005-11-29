#if !defined(AFX_SIMULTIMER_H__40951585_F048_11D3_A2F4_00D0B71C8709__INCLUDED_)
#define AFX_SIMULTIMER_H__40951585_F048_11D3_A2F4_00D0B71C8709__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SimulTimer.h : header file
//

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "Tools\SelSliderCtrl.h"
class CExternRepUnit;

/////////////////////////////////////////////////////////////////////////////
/// Class used to implement the interface of a Timer ER.
///
///
/////////////////////////////////////////////////////////////////////////////
class CViewTimer : public CFormView
{
protected:
	CViewTimer();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CViewTimer)

// Form Data
public:
	//{{AFX_DATA(CViewTimer)
	enum { IDD = IDD_SIMULTIMER_FORM };
	CSelSliderCtrl	m_Slider;
	//}}AFX_DATA
	int m_nMaxTime;	// Time reached by run mode
	BOOL	m_bIsRunning;
	int 	m_nBackReview;	// Review Mode (0 None, 1 back, 2 for)
//	int				m_nCurrTTask;

	// Attributes
protected:
	CToolBar		m_cToolBar;
	CExternRepUnit	*m_pER;
public:
	CSimulDoc* GetDocument();
	int					GetTranslation();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewTimer)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CViewTimer();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CViewTimer)
	afx_msg void OnReleasedSlider(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTrackTimer(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnMove(int x, int y);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	afx_msg LRESULT OnActivateER(WPARAM, LPARAM); 
	afx_msg LRESULT OnUpdateInitContent(WPARAM, LPARAM); 
	afx_msg void OnTranslationTasks(UINT nID);
	afx_msg void OnUpdateTransTasks(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in simulView.cpp
inline CSimulDoc* CViewTimer::GetDocument()
   { return (CSimulDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SIMULTIMER_H__40951585_F048_11D3_A2F4_00D0B71C8709__INCLUDED_)
