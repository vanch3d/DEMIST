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


#if !defined(AFX_OPTIONTREEICONPOPUP_H__584A321D_6272_437D_96BE_7C8E2F2E6239__INCLUDED_)
#define AFX_OPTIONTREEICONPOPUP_H__584A321D_6272_437D_96BE_7C8E2F2E6239__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptionTreeIconPopUp.h : header file
//

// Added Headers
#include "OptionTreeDef.h"
#include "OptionTreeItem.h"

// Icon Popup Node
struct OT_ICON_NODE
{
	CString m_strText;

	HICON m_hIcon;

	CRect m_rcHitRect;

	BOOL m_bSelected;

	OT_ICON_NODE *m_nNextNode;
};

// Classes
class COptionTree;

/////////////////////////////////////////////////////////////////////////////
// COptionTreeIconPopUp window

class COptionTreeIconPopUp : public CWnd
{
// Construction
public:
	COptionTreeIconPopUp(CPoint pPoint, int nIconSize, int nNumColumns, OT_ICON_NODE *nIcons, COptionTree *otIconOption, CWnd* pParentWnd);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COptionTreeIconPopUp)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL Create(CPoint pPoint, int nIconSize, int nNumColumns, OT_ICON_NODE *nIcons, COptionTree *otIconOption, CWnd* pParentWnd);
	virtual ~COptionTreeIconPopUp();

protected:
	OT_ICON_NODE * GetIconNode(int nIndex);
	void SelectIcon(int nIndex);
	int GetSelIconIndex();
	void SetUpToolTips();
	void SetUpIcons();
	int GetNumberIcons();
	void SetWindowSize();
	void DeSelectAll();

	// Generated message map functions
protected:
	int m_nIconSize;
	int m_nNumColumns;
	int m_nNumRows;
	CToolTipCtrl m_ttToolTip;
	OT_ICON_NODE *m_nAllIcons;
	CPoint m_pHover;
	COptionTree *m_otIconOption;
	OT_ICON_NODE *m_otiLastNode;
	CWnd *m_wndParent;
	//{{AFX_MSG(COptionTreeIconPopUp)
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnNcDestroy();
	afx_msg void OnActivateApp(BOOL bActive, HTASK hTask);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnDestroy();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTIONTREEICONPOPUP_H__584A321D_6272_437D_96BE_7C8E2F2E6239__INCLUDED_)
