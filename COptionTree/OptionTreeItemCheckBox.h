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


#if !defined(AFX_OPTIONTREEITEMCHECKBOX_H__85D4A80F_9A68_4310_9E67_D75D32A32EF9__INCLUDED_)
#define AFX_OPTIONTREEITEMCHECKBOX_H__85D4A80F_9A68_4310_9E67_D75D32A32EF9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptionTreeItemCheckBox.h : header file
//

// Added Headers
#include "OptionTreeDef.h"
#include "OptionTreeItem.h"
#include "OptionTreeCheckButton.h"

// Classes
class COptionTree;

/////////////////////////////////////////////////////////////////////////////
// COptionTreeItemCheckBox window

class COptionTreeItemCheckBox : public COptionTreeCheckButton, public COptionTreeItem
{
// Construction
public:
	COptionTreeItemCheckBox();
	virtual void OnMove();
	virtual void OnRefresh();
	virtual void OnCommit();
	virtual void OnActivate();
	virtual void CleanDestroyWindow();
	virtual void DrawAttribute(CDC *pDC, const RECT &rcRect);
	virtual void OnDeSelect();
	virtual void OnSelect();

	BOOL CreateCheckBoxWindow(BOOL bChecked, BOOL bShowCheck = TRUE, BOOL bShowText = FALSE);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COptionTreeItemCheckBox)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~COptionTreeItemCheckBox();

	// Generated message map functions
protected:
	BOOL m_bFocus;
	CRect m_rcCheck;
	//{{AFX_MSG(COptionTreeItemCheckBox)
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTIONTREEITEMCHECKBOX_H__85D4A80F_9A68_4310_9E67_D75D32A32EF9__INCLUDED_)
