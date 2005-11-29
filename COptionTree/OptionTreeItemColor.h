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


#if !defined(AFX_OPTIONTREEITEMCOLOR_H__63D039F0_A2E0_4C8F_9DD8_D7BE9A4BE7B0__INCLUDED_)
#define AFX_OPTIONTREEITEMCOLOR_H__63D039F0_A2E0_4C8F_9DD8_D7BE9A4BE7B0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptionTreeItemColor.h : header file
//

// Added Headers
#include "OptionTreeDef.h"
#include "OptionTreeItem.h"
#include "OptionTreeColorPopUp.h"

// Classes
class COptionTree;

/////////////////////////////////////////////////////////////////////////////
// COptionTreeItemColor window

class COptionTreeItemColor : public CWnd, public COptionTreeItem
{
// Construction
public:
	COptionTreeItemColor();
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
	//{{AFX_VIRTUAL(COptionTreeItemColor)
	//}}AFX_VIRTUAL

// Implementation
public:
	COLORREF GetAutomaticColor();
	void SetAutomaticColor(COLORREF crAutomatic);
	BOOL GetLiveUpdate();
	void SetLiveUpdate(BOOL bLive);
	BOOL GetShowHex();
	void SetShowHex(BOOL bShow);
	BOOL CreateColorWindow(COLORREF rcColor, COLORREF rcAutomatic, BOOL bShowHex = TRUE, BOOL bLiveUpdate = TRUE);
	void SetColor(COLORREF rcColor);
	COLORREF GetColor();
	virtual ~COptionTreeItemColor();

	// Generated message map functions
protected:
	BOOL m_bFocus;
	COLORREF m_rcColor;
	COLORREF m_crAutomatic;
	BOOL m_bShowHex;
	BOOL m_bLiveUpdate;
	CRect m_rcHitTest;
	//{{AFX_MSG(COptionTreeItemColor)
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	//}}AFX_MSG
	afx_msg LONG OnSelEndOK(UINT lParam, LONG wParam);
    afx_msg LONG OnSelEndCancel(UINT lParam, LONG wParam);
    afx_msg LONG OnSelChange(UINT lParam, LONG wParam);
	afx_msg LONG OnCloseColorPopUp(UINT lParam, LONG wParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTIONTREEITEMCOLOR_H__63D039F0_A2E0_4C8F_9DD8_D7BE9A4BE7B0__INCLUDED_)
