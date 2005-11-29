// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__40951557_F048_11D3_A2F4_00D0B71C8709__INCLUDED_)
#define AFX_MAINFRM_H__40951557_F048_11D3_A2F4_00D0B71C8709__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "mybar.h"
#include "Tools\CopyPasteBuffer.h"

class CLearningUnit;

/////////////////////////////////////////////////////////////////////////////
/// Multiple Document Interface (MDI) frame window of the application.
/////////////////////////////////////////////////////////////////////////////
class CMainFrame : public CMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)

// Attributes
public:
	CCopyPaste		m_cpyBuffer;		///< ???????????
	CStringArray	m_strToolTip;		///< ????

protected:
	CRect			m_mainRect;			///< ???????????
	bool			m_bChildMax;		///< ???????????
	bool			m_FullScreenMode;	///< ???????????
	CStatusBar		m_wndStatusBar;		///< ???????????
	CToolBar		m_wndToolBar;		///< ???????????
	CToolBar		m_wndTaskBar;		///< ???????????
	CToolBar		m_wndTools;			///< ???????????

	CDocument*		m_pDocClass ;		///< ???????????

// Operations
public:
	CCopyPaste*	GetCopyBuffer() { return &m_cpyBuffer;};


	void OnSetMERIcons(CLearningUnit *pLU = NULL);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL DestroyWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	CMainFrame();
	virtual ~CMainFrame();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// Generated message map functions
protected:
	BOOL VerifyBarState(LPCTSTR lpszProfileName);	// 
	void FullScreenModeOn();
	void FullScreenModeOff();

	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnViewFullscreen();
	afx_msg void OnUpdateViewFullscreen(CCmdUI* pCmdUI);
	afx_msg void OnToolbarDropDown(NMTOOLBAR* pnmh, LRESULT* plRes);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnFilePref();
	afx_msg void OnFileSwitchMode();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnViewTaskbar();
	afx_msg void OnUpdateViewTaskbar(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFilePref(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSwitchMode(CCmdUI* pCmdUI);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnViewERs();
	afx_msg void OnUpdateViewERs(CCmdUI* pCmdUI);
	//}}AFX_MSG
	afx_msg void OnUpdateMode(CCmdUI *pCmdUI);
	afx_msg BOOL OnMERIconsToolTips( UINT id, NMHDR * pTTTStruct, LRESULT * pResult );
	afx_msg BOOL OnToolTipText(UINT nID, NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg LRESULT OnSetMessageString(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__40951557_F048_11D3_A2F4_00D0B71C8709__INCLUDED_)
