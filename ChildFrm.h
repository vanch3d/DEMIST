// ChildFrm.h : interface of the CChildFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHILDFRM_H__40951559_F048_11D3_A2F4_00D0B71C8709__INCLUDED_)
#define AFX_CHILDFRM_H__40951559_F048_11D3_A2F4_00D0B71C8709__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "mybar.h"
#include "VisualFx/VisualFx.h"
//#include "ST_SplitterWnd.h"

#include <afxcview.h>


//////////////////////////////////////////////////////////////////////
/// Generic class implementing the Multiple Document Interface (MDI) child windows used in DEMIST. 
//////////////////////////////////////////////////////////////////////
class CChildFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CChildFrame)
public:
	CChildFrame();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChildFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CChildFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// Generated message map functions
protected:
	//{{AFX_MSG(CChildFrame)
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd);
	afx_msg void OnClose();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnMove(int x, int y);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////
/// Multiple Document Interface (MDI) child window for the External Representations.
//////////////////////////////////////////////////////////////////////
class CChildFrameFix : public CChildFrame
{
		DECLARE_DYNCREATE(CChildFrameFix)
public:
	CChildFrameFix();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChildFrameFix)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CChildFrameFix)
	afx_msg void OnClose();
	afx_msg void OnMoving(UINT fwSide, LPRECT pRect);
	afx_msg void OnMove(int x, int y);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


//////////////////////////////////////////////////////////////////////
/// Multiple Document Interface (MDI) child window for the Controller.
//////////////////////////////////////////////////////////////////////
class CChildController : public CChildFrameFix
{
		DECLARE_DYNCREATE(CChildController)
public:
	CChildController();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChildController)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CChildController)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////
/// Multiple Document Interface (MDI) child window used for the Simulation Design Interface.
///
/// It consists on a list box containing the models defined in the scenario and 4 panes presenting 
/// the different information extracted from the selected model:
///		- CViewSDIlist implements the list
///		- CViewModel implements.
///		- CViewSDIbuild implements .
///		- CViewSDIaggreg implements .
///		- CViewSDIdiag implements. 
///
/// The construction of the interface, using the TVisualFramework class, is handled by the 
/// OnCreateClient() function.
//////////////////////////////////////////////////////////////////////
class CChildFrameSplit : public CChildFrame
{
		DECLARE_DYNCREATE(CChildFrameSplit)
public:
	TVisualFramework	m_Framework;	///< Interface framework allowing the construction and the manipulation of panes. 
	CImageList			m_pImageList;	///< Image list containing the icons used for the panes' label.


	CChildFrameSplit();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChildFrameSplit)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CChildFrameSplit)
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


//////////////////////////////////////////////////////////////////////
// OBSOLETE - Multiple Document Interface (MDI) child window for the multi-panes interface.
//////////////////////////////////////////////////////////////////////
class CChildFramePanes : public CChildFrame
{
		DECLARE_DYNCREATE(CChildFramePanes)
public:
	CPtrArray m_pSplitterWnds;

	CChildFramePanes();
	~CChildFramePanes();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChildFramePanes)
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CChildFramePanes)
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	//}}AFX_MSG
	afx_msg LRESULT OnDoContextMenu(WPARAM, LPARAM); 
	DECLARE_MESSAGE_MAP()
};



/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHILDFRM_H__40951559_F048_11D3_A2F4_00D0B71C8709__INCLUDED_)
