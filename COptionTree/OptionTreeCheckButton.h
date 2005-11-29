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


#if !defined(AFX_OPTIONTREECHECKBUTTON_H__FE7C3F3B_2FB4_400A_8C31_A3388699687B__INCLUDED_)
#define AFX_OPTIONTREECHECKBUTTON_H__FE7C3F3B_2FB4_400A_8C31_A3388699687B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptionTreeCheckButton.h : header file
//

// Added Headers
#include "OptionTreeDef.h"
#include "OptionTreeItem.h"

// Classes
class COptionTree;

/////////////////////////////////////////////////////////////////////////////
// COptionTreeCheckButton window

class COptionTreeCheckButton : public CWnd
{
// Construction
public:
	COptionTreeCheckButton();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COptionTreeCheckButton)
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetCheckOptionsOwner(COptionTree *otOption);
	BOOL GetShowCheck();
	void SetShowCheck(BOOL bShowCheck);
	BOOL GetCheck();
	void SetCheck(BOOL bCheck);
	void SetShowText(BOOL bText);
	BOOL GetShowText();
	CString GetCheckedText();
	CString GetUnCheckedText();
	void SetCheckText(CString strChecked, CString strUnChecked);
	virtual ~COptionTreeCheckButton();

	// Generated message map functions
protected:
	CString m_strUnChecked;
	CString m_strChecked;
	BOOL m_bShowText;
	BOOL m_nbShowCheck;
	BOOL m_bCheck;
	CRect m_rcCheck;
	COptionTree *m_otCheckOption;
	//{{AFX_MSG(COptionTreeCheckButton)
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTIONTREECHECKBUTTON_H__FE7C3F3B_2FB4_400A_8C31_A3388699687B__INCLUDED_)
