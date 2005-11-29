#if !defined(AFX_VIEWWELCOME_H__42AD37C6_6194_11D5_A2FA_00D0B71C8709__INCLUDED_)
#define AFX_VIEWWELCOME_H__42AD37C6_6194_11D5_A2FA_00D0B71C8709__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ViewWelcome.h : header file
//

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif
#include <afxhtml.h>

#include "Tools\BtnST.h"

/////////////////////////////////////////////////////////////////////////////
// 
/////////////////////////////////////////////////////////////////////////////
class CWelcomeDialogBar : public CDialogBar
{
// Construction
public:
	CWelcomeDialogBar();

	CImageList	m_ImageList;

// Dialog Data
	//{{AFX_DATA(CWelcomeDialogBar)
	enum { IDD = IDD_WELCOMEBAR };
	CButtonST		m_cStart;
	CButtonST		m_cNext;
	CButtonST		m_cCurrent;
	CStatic			m_cNumber;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWelcomeDialogBar)
	public:
	virtual BOOL Create(CWnd* pParentWnd, UINT nStyle, UINT nID);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL


// Implementation
public:

protected:
	//{{AFX_MSG(CWelcomeDialogBar)
	DECLARE_MESSAGE_MAP()
	//}}AFX_MSG
	afx_msg LRESULT HandleInitDialog(WPARAM, LPARAM);
};

/////////////////////////////////////////////////////////////////////////////
/// Class used to implement the "Welcome" page in DEMIST.
///
/// 
/////////////////////////////////////////////////////////////////////////////
class CViewWelcome : public CHtmlView
{
protected:
	CViewWelcome();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CViewWelcome)

// html Data
public:
	//{{AFX_DATA(CViewWelcome)
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Attributes
public:
	CSimulDoc*			GetDocument();
	CWelcomeDialogBar	m_cToolBar;

// Operations
public:
	BOOL		OpenHTMLDoc(LPTSTR lpStr=NULL);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewWelcome)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
//	virtual void OnDraw(CDC* pDC);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CViewWelcome();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CViewWelcome)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	//}}AFX_MSG
	afx_msg void OnStartSession();
	afx_msg void OnStartLU();
	afx_msg void OnNextLU();
	afx_msg void OnFilePrintPreview();
	afx_msg void OnUpdateTools(CCmdUI* pCmdUI);

	virtual void OnDownloadComplete();
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in simulView.cpp
inline CSimulDoc* CViewWelcome::GetDocument()
   { return (CSimulDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEWWELCOME_H__42AD37C6_6194_11D5_A2FA_00D0B71C8709__INCLUDED_)
