#if !defined(AFX_SIMULTEXT_H__40951584_F048_11D3_A2F4_00D0B71C8709__INCLUDED_)
#define AFX_SIMULTEXT_H__40951584_F048_11D3_A2F4_00D0B71C8709__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SimulText.h : header file
//

#include "Tools\TWScriptEdit.h"
#include <MInstruction\LearningUnit.h>

class CFormatText;


/////////////////////////////////////////////////////////////////////////////
/// Class used to implement the interface of a Static Text ER.
///
///
/////////////////////////////////////////////////////////////////////////////
class CViewText : public CView
{
protected:
	CViewText();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CViewText)

// Attributes
public:

protected:
	CToolBar			m_cToolBar;
	CExternRepUnit*	m_pER;
	CTWScriptEdit	m_wndScriptEdit;
//	int				m_nCurrTTask;

// Operations
public:
	CSimulDoc*		GetDocument();
	CFormatText*	GetFormats();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewText)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CViewText();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CViewText)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnEditCopy();
	afx_msg void OnDestroy();
	afx_msg void OnEditPaste();
	//}}AFX_MSG
	afx_msg LRESULT OnActivateER(WPARAM, LPARAM); 
	afx_msg LRESULT OnUpdateInitContent(WPARAM, LPARAM); 
	afx_msg void OnTranslationTasks(UINT nID);
	afx_msg void OnUpdateTransTasks(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in simulView.cpp
inline CSimulDoc* CViewText::GetDocument()
   { return (CSimulDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SIMULTEXT_H__40951584_F048_11D3_A2F4_00D0B71C8709__INCLUDED_)
