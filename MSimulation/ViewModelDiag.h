#if !defined(AFX_VIEWMODELDIAG_H__84920069_B665_11D4_A2FA_00D0B71C8709__INCLUDED_)
#define AFX_VIEWMODELDIAG_H__84920069_B665_11D4_A2FA_00D0B71C8709__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ViewModelDiag.h : header file
//

/////////////////////////////////////////////////////////////////////////////
/// Interface for the model's flow-chart in the Simulation Design Interface.
/////////////////////////////////////////////////////////////////////////////
class CViewSDIdiag : public CScrollView
{
protected:
	CViewSDIdiag();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CViewSDIdiag)

// Attributes
public:
protected:
	int			m_nPrevMod;
	CBitmap		m_bDiag;
	int			m_nBitmapID;
	CImageList	m_pImageList;

	CRect		m_rBound;
// Operations
public:
	CSimulDoc* GetDocument();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewSDIdiag)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnInitialUpdate();     // first time after construct
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CViewSDIdiag();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CViewSDIdiag)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in SimulView.cpp
inline CSimulDoc* CViewSDIdiag::GetDocument()
   { return (CSimulDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEWMODELDIAG_H__84920069_B665_11D4_A2FA_00D0B71C8709__INCLUDED_)
