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


#if !defined(AFX_OPTIONTREERADIOBUTTON_H__74D99300_C483_420E_B884_D0B34F419250__INCLUDED_)
#define AFX_OPTIONTREERADIOBUTTON_H__74D99300_C483_420E_B884_D0B34F419250__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptionTreeRadioButton.h : header file
//


// Added Headers
#include "OptionTreeDef.h"
#include "OptionTreeItem.h"


// Radio Button Node
struct OT_RADIO_NODE
{
	CString m_strText;

	BOOL m_bChecked;

	CRect m_rcHitRect;

	OT_RADIO_NODE *m_nNextNode;
};


// Classes
class COptionTree;

/////////////////////////////////////////////////////////////////////////////
// COptionTreeRadioButton window

class COptionTreeRadioButton : public CWnd
{
// Construction
public:
	COptionTreeRadioButton();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COptionTreeRadioButton)
	//}}AFX_VIRTUAL

// Implementation
public:
	int Node_GetChecked();
	void Node_UnCheckAll();
	void SetRadioOptionsOwner(COptionTree *otOption);
	OT_RADIO_NODE *Node_FindNode(CString strText);
	OT_RADIO_NODE *Node_FindNode(int nIndex);
	void Node_DeleteAll();
	void Node_Insert(CString strText, BOOL bChecked);
	virtual ~COptionTreeRadioButton();

protected:
	

	// Generated message map functions
protected:
	OT_RADIO_NODE *m_nAllNodes;
	COptionTree *m_otRadioOption;
	//{{AFX_MSG(COptionTreeRadioButton)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMove(int x, int y);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTIONTREERADIOBUTTON_H__74D99300_C483_420E_B884_D0B34F419250__INCLUDED_)
