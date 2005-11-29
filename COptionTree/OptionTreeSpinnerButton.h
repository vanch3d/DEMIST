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



#if !defined(AFX_OPTIONTREESPINNERBUTTON_H__393965BD_ACD7_474B_A42D_EC78BC4A3242__INCLUDED_)
#define AFX_OPTIONTREESPINNERBUTTON_H__393965BD_ACD7_474B_A42D_EC78BC4A3242__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptionTreeSpinnerButton.h : header file
//

// Added Headers
#include "OptionTreeDef.h"
#include "OptionTreeItem.h"
#include "OptionTreeSpinnerEdit.h"

// Classes
class COptionTree;

/////////////////////////////////////////////////////////////////////////////
// COptionTreeSpinnerButton window

class COptionTreeSpinnerButton : public CWnd
{
// Construction
public:
	COptionTreeSpinnerButton();

// Attributes
public:
	CString m_strEdit;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COptionTreeSpinnerButton)
	//}}AFX_VIRTUAL

// Implementation
public:
	CString GetEditText();
	BOOL IsStringNumeric(CString strString);
	CEdit* GetEdit();
	void ResizeEdit();
	BOOL GetEditDword(DWORD &dwReturn);
	BOOL GetEditInt(int &nReturn);
	BOOL GetEditLong(long &lReturn);
	BOOL GetEditDouble(double &dReturn);
	BOOL GetEditFloat(float &fReturn);
	void SetEditLong(long lValue);
	void SetEditDword(DWORD dwValue);
	void SetEditFloat(float fValue);
	void SetEditInt(int nValue);
	void SetEditDouble(double dValue);
	BOOL GetUserEdit();
	void SetUserEdit(BOOL bAllowEdit);
	void GetRange(double &dBottom, double &dTop);
	void SetRange(double dBottom, double dTop);
	BOOL GetWrapAround();
	void SetWrapAround(BOOL bWrap);
	void SetSpinnerOptionsOwner(COptionTree *otOption);
	virtual ~COptionTreeSpinnerButton();

protected:
	LRESULT WM_ForceRedraw(WPARAM wParam, LPARAM lParam);
	LRESULT WM_EditUp(WPARAM wParam, LPARAM lParam);
	LRESULT WM_EditDown(WPARAM wParam, LPARAM lParam);	
	double _GetValue();
	
	// Generated message map functions
protected:
	BOOL m_bWrapAround;
	COptionTree *m_otSpinnerOption;
	double m_dRangeTop;
	double m_dRangeBottom;
	CRect m_rcButtonTop;
	CRect m_rcButtonBottom;
	BOOL m_bBottomPressed;
	BOOL m_bTopPressed;
	BOOL m_bUserEdit;
	COptionTreeSpinnerEdit m_ctlEdit;
	//{{AFX_MSG(COptionTreeSpinnerButton)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnMove(int x, int y);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTIONTREESPINNERBUTTON_H__393965BD_ACD7_474B_A42D_EC78BC4A3242__INCLUDED_)
