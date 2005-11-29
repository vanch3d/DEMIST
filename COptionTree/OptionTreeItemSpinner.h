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


#if !defined(AFX_OPTIONTREEITEMSPINNER_H__6777FD93_E030_4B21_85B7_482A5FEB3399__INCLUDED_)
#define AFX_OPTIONTREEITEMSPINNER_H__6777FD93_E030_4B21_85B7_482A5FEB3399__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptionTreeItemSpinner.h : header file
//


// Added Headers
#include "OptionTreeDef.h"
#include "OptionTreeItem.h"
#include "OptionTreeSpinnerButton.h"

// Classes
class COptionTree;


/////////////////////////////////////////////////////////////////////////////
// COptionTreeItemSpinner window

class COptionTreeItemSpinner : public COptionTreeSpinnerButton, public COptionTreeItem
{
// Construction
public:
	COptionTreeItemSpinner();
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
	//{{AFX_VIRTUAL(COptionTreeItemSpinner)
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL CreateSpinnerWindow(BOOL bWrapAround, BOOL bUserEdit, double dValue, double dRangeBottom, double dRangeTop);
	virtual ~COptionTreeItemSpinner();

protected:
	LRESULT WM_CommitChanges(WPARAM wParam, LPARAM lParam);

	// Generated message map functions
protected:
	BOOL m_bFocus;

	//{{AFX_MSG(COptionTreeItemSpinner)
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTIONTREEITEMSPINNER_H__6777FD93_E030_4B21_85B7_482A5FEB3399__INCLUDED_)
