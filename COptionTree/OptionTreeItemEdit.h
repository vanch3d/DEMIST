// COptionTree
//
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
//	5/7/2002		Initial implementation.	
//
//	5/10/2002 		Fix problems with icon and color items. Also fixed problems
//				with expanding columns and column size. Added TAB support, 
//				hit TAB to activate next item, and SHIFT TAB to activate previous item.
//				NOTE: You must commit current item with the ENTER key before tabbing, 
//				this is because some controls use TAB i.e.) edit, control box, radio, and so on.
//
//	5/11/2002		Fixed IsWindow() issues, which are the causes for a lot of Win98 problems. 
//				Fixed IDC_HAND problem for non Win 2K or XP users. Fixed some resource and 
//				overhead problems. Also added expand all feature. Huge thanks go out to all 
//				bug reporters, especially YoSilver.
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


#if !defined(AFX_OPTIONTREEITEMEDIT_H__5277AFB1_A6A4_4792_8610_AE3E503F3D20__INCLUDED_)
#define AFX_OPTIONTREEITEMEDIT_H__5277AFB1_A6A4_4792_8610_AE3E503F3D20__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptionTreeItemEdit.h : header file
//

// Added Headers
#include "OptionTreeDef.h"
#include "OptionTreeItem.h"

// Classes
class COptionTree;

/////////////////////////////////////////////////////////////////////////////
// COptionTreeItemEdit window

class COptionTreeItemEdit : public CEdit, public COptionTreeItem
{
// Construction
public:
	COptionTreeItemEdit();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COptionTreeItemEdit)
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetNumerical(BOOL bNumerical);
	void SetPassword(BOOL bPassword);
	void SetEditLong(long lValue);
	void SetEditDword(DWORD dwValue);
	void SetEditFloat(float fValue);
	void SetEditInt(int nValue);
	void SetEditDouble(double dValue);
	void SetMultiline(BOOL bMultiline = FALSE);
	BOOL GetPassword();
	BOOL GetNumerical();
	BOOL GetMultiline();
	BOOL CreateEditWindow(BOOL bMultiline, BOOL bPassword = FALSE, BOOL bNumerical = FALSE, DWORD dwAddStyle = 0);
	virtual void OnMove();
	virtual void OnRefresh();
	virtual void OnCommit();
	virtual void OnActivate();
	virtual void CleanDestroyWindow();
	virtual void OnDeSelect();
	virtual void OnSelect();
	virtual void DrawAttribute(CDC *pDC, const RECT &rcRect);

	BOOL GetEditDword(DWORD &dwReturn);
	BOOL GetEditInt(int &nReturn);
	BOOL GetEditLong(long &lReturn);
	BOOL GetEditDouble(double &dReturn);
	BOOL GetEditFloat(float &fReturn);
	virtual ~COptionTreeItemEdit();

	// Generated message map functions
protected:
	BOOL IsStringNumeric(CString strString);
	//{{AFX_MSG(COptionTreeItemEdit)
	afx_msg UINT OnGetDlgCode();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	CString m_strEdit;
	BOOL m_bFocus;
	BOOL m_bMultiline;
	BOOL m_bPassword;
	BOOL m_bNumerical;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTIONTREEITEMEDIT_H__5277AFB1_A6A4_4792_8610_AE3E503F3D20__INCLUDED_)
