#if !defined(AFX_SIMULPARAM_H__40951582_F048_11D3_A2F4_00D0B71C8709__INCLUDED_)
#define AFX_SIMULPARAM_H__40951582_F048_11D3_A2F4_00D0B71C8709__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SimulParam.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CViewParam form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "GridCtrl\GridCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// OBSOLETE - 
/////////////////////////////////////////////////////////////////////////////
class CViewParam : public CFormView
{
protected:
	CViewParam();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CViewParam)

// Form Data
public:
	//{{AFX_DATA(CViewParam)
	enum { IDD = IDD_SIMULPARAM_FORM };
	CStatic	m_strParam;
	CStatic	m_strInit;
	//}}AFX_DATA
	CImageList	m_ImageList;
	CGridCtrl	m_Parlist,
				m_InitList;
	CToolBar	m_cToolBar;

// Attributes
public:
	CSimulDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewParam)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CViewParam();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CViewParam)
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnResetParameters();
	afx_msg void OnApplyParameters();
	afx_msg void OnAddExpSet();
	afx_msg void OnRemoveExpSet();
	afx_msg void OnUpdateParametersCmd(CCmdUI* pCmdUI);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	afx_msg LRESULT OnActivateER(WPARAM, LPARAM); 
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in simulView.cpp
inline CSimulDoc* CViewParam::GetDocument()
   { return (CSimulDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SIMULPARAM_H__40951582_F048_11D3_A2F4_00D0B71C8709__INCLUDED_)
