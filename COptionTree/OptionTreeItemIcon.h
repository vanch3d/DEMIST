// COptionTree
//
// License
// -------
// This code is provided "as is" with no expressed or implied warranty.
//
// Feel free to modify and/or distribute this file, but
// do not remove the headers in place.
// 
// You may use this code in a commercial product with or without acknowledgement.
// However you may not sell this code or any modification of this code.
//
// I would appreciate a notification of any bugs discovered or 
// improvements that could be made to help the control grow for everyone.
//
// History:
// --------
//	See License.txt for full history ifnormation.
//
//
// Copyright (c) 1999-2002 
// Matthew R. Miller 
// mattrmiller@computersmarts.net
//
// 
// Acknowledgements:
// -----------------
// The COptionTree is inspired by code, ideas, and submissions from the following: 
//
// CPropTree v1.0 - Property Tree Control - By Scott Ramsay (http://www.thecodeproject.com/treectrl/proptree.asp)
// A color picker button - By James White (http://www.thecodeproject.com/miscctrl/colorbutton.asp)


#if !defined(AFX_OPTIONTREEITEMICON_H__DDF80CB1_7024_4BAB_A90E_1FF1E2CBD09B__INCLUDED_)
#define AFX_OPTIONTREEITEMICON_H__DDF80CB1_7024_4BAB_A90E_1FF1E2CBD09B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptionTreeItemIcon.h : header file
//

// Added Headers
#include "OptionTreeDef.h"
#include "OptionTreeItem.h"
#include "OptionTreeIconPopUp.h"

// Classes
class COptionTree;

/////////////////////////////////////////////////////////////////////////////
// COptionTreeItemIcon window

class COptionTreeItemIcon : public CWnd, public COptionTreeItem
{
// Construction
public:
	COptionTreeItemIcon();
	virtual void OnMove();
	virtual void OnRefresh();
	virtual void OnCommit();
	virtual void OnActivate();
	virtual void CleanDestroyWindow();
	virtual void OnDeSelect();
	virtual void OnSelect();
	virtual void DrawAttribute(CDC *pDC, const RECT &rcRect);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COptionTreeItemIcon)
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL InsertIcon(CString strText, UINT uIcon, BOOL bSelected);
	BOOL InsertIcon(CString strText, HICON hIcon, BOOL bSelected);
	int GetSelectedIcon();
	OT_ICON_NODE * GetSelectedNode();
	HICON CreateGreyScaleIcon(HICON hIcon);
	int GetNumberColumns();
	void SetNumberColumns(int nNumber);
	int GetIconSize();
	void SetIconSize(int nIconSize);
	BOOL GetShowText();
	void SetShowText(BOOL bShow);
	BOOL CreateIconWindow(BOOL bShowText, int nIconSize, int nNumberColumns);
	virtual ~COptionTreeItemIcon();


protected:
	void Node_DeleteAll();
	void DeSelectAll();
	void Node_Insert(CString strText, HICON hIcon, BOOL bSelected);
	// Generated message map functions
protected:
	BOOL m_bFocus;
	BOOL m_bShowText;
	int m_nIconSize;
	int m_nNumColumns;
	OT_ICON_NODE *m_nAllIcons;
	CRect m_rcHitTest;
	//{{AFX_MSG(COptionTreeItemIcon)
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	afx_msg LONG OnPopUpClose(UINT lParam, LONG wParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTIONTREEITEMICON_H__DDF80CB1_7024_4BAB_A90E_1FF1E2CBD09B__INCLUDED_)
