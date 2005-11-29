/////////////////////////////////////////////////////////////////////////////
// Copyright (C) 1999 by Pedro Pombeiro
// All rights reserved
//
// Distribute freely, except: don't remove my name from the source or
// documentation (don't take credit for my work), mark your changes (don't
// get me blamed for your possible bugs), don't alter or remove this
// notice.
// No warrantee of any kind, express or implied, is included with this
// software; use at your own risk, responsibility for damages (if any) to
// anyone resulting from the use of this software rests entirely with the
// user.
//
// Send bug reports, bug fixes, enhancements, requests, flames, etc., and
// I'll try to keep a version up to date.  I can be reached as follows:
//    PPombeiro@AntaresLda.pt         (company mail account)
/////////////////////////////////////////////////////////////////////////////


// SelectionSliderCtrl.h : header file
//

#if !defined(AFX_SELECTIONSLIDERCTRL_H__1E0F37F5_4020_11D1_9FB1_444553540000__INCLUDED_)
#define AFX_SELECTIONSLIDERCTRL_H__1E0F37F5_4020_11D1_9FB1_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// CSelSliderCtrl window

class CSelSliderCtrl : public CSliderCtrl
{
// Construction
public:
	CSelSliderCtrl();

// Attributes
public:

// Operations
public:
	void AddBreakPoint(int nTime);
	int  GetBreakPoint(int nb);
	void RemoveBreakPoint(int nTime);

	int  GetCurrentBreakPoint();
	void SetCurrentBreakPoint(int nb);
	void SetNextBreakPoint();

	void SetPastColour(COLORREF clr){m_crSelect=clr;};
	void SetFutureColour(COLORREF clr){m_crBackGrd=clr;};

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSelSliderCtrl)
	public:
	protected:
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSelSliderCtrl();

protected:
	CUIntArray	m_cBreakPt;			// List of BreakPoint
	int		m_nCurrBreakPoint;		// Current breakPoint
	BOOL	m_bSelecting;
	BOOL	m_bAnchorIsValid;
	int		m_iAnchorSelection;
	COLORREF	m_crBackGrd;
	COLORREF	m_crSelect;


	// Generated message map functions
protected:
	//{{AFX_MSG(CSelSliderCtrl)
	afx_msg void HScroll(UINT nTBCode, UINT nPos);
	afx_msg void VScroll(UINT nTBCode, UINT nPos);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SELECTIONSLIDERCTRL_H__1E0F37F5_4020_11D1_9FB1_444553540000__INCLUDED_)
