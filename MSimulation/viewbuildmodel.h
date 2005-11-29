#if !defined(AFX_VIEWBUILDMODEL_H__5B2ED465_7DFA_11D6_A2FC_00D0B71C8709__INCLUDED_)
#define AFX_VIEWBUILDMODEL_H__5B2ED465_7DFA_11D6_A2FC_00D0B71C8709__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ViewBuildModel.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CViewSDIbuild form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif


#include "Tools\BtnST.h"
#include "Tools\TWScriptEdit.h"

/////////////////////////////////////////////////////////////////////////////
/// Interface for editing and validating the equations in the Simulation Design Interface.
/////////////////////////////////////////////////////////////////////////////
class CViewSDIbuild : public CFormView
{
protected:
	CViewSDIbuild();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CViewSDIbuild)

// Form Data
public:
	//{{AFX_DATA(CViewSDIbuild)
	enum { IDD = IDD_MODEL_BUILD };
	CButtonST		m_cProp;		///< interface for the "Property" button
	CTWScriptEdit	m_wndEditEqn;	///< Interface for the model edition field
	CButtonST		m_cCheck;		///< Interface for the "Check Model" button
	//}}AFX_DATA

// Attributes
public:
	int				m_nPrevMod;
	BOOL			m_bCanProceed;
	CImageList		m_ImageIcon;

// Operations
public:
	CSimulDoc* GetDocument();


	void EnableEdition(BOOL bEnab=TRUE);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewSDIbuild)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CViewSDIbuild();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CViewSDIbuild)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnCheckEquation();
	afx_msg void OnSetSimulationProp();
	afx_msg void OnEditCopy();
	afx_msg void OnEditCut();
	afx_msg void OnEditPaste();
	afx_msg void OnUpdateEdit(CCmdUI* pCmdUI);
	afx_msg void OnEditUndo();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnEditSelectAll();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in SimulView.cpp
inline CSimulDoc* CViewSDIbuild::GetDocument()
   { return (CSimulDoc*)m_pDocument; }
#endif


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEWBUILDMODEL_H__5B2ED465_7DFA_11D6_A2FC_00D0B71C8709__INCLUDED_)
