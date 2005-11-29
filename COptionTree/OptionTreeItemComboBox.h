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


#if !defined(AFX_OPTIONTREEITEMCOMBOBOX_H__EE91CFB4_1C0D_4D13_AD51_803680F51BE2__INCLUDED_)
#define AFX_OPTIONTREEITEMCOMBOBOX_H__EE91CFB4_1C0D_4D13_AD51_803680F51BE2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptionTreeItemComboBox.h : header file
//

// Added Headers
#include "OptionTreeDef.h"
#include "OptionTreeItem.h"

// Classes
class COptionTree;

/////////////////////////////////////////////////////////////////////////////
// COptionTreeItemComboBox window

class COptionTreeItemComboBox : public CComboBox, public COptionTreeItem
{
// Construction
public:
	COptionTreeItemComboBox();
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
	int m_nSelect;


// Operations
public:

	int GetCurSel() const;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COptionTreeItemComboBox)
	//}}AFX_VIRTUAL

// Implementation
public:
	long GetDropDownHeight();
	void SetDropDownHeight(long lHeight);
	BOOL CreateComboWindow(DWORD dwAddStyle = 0);
	virtual ~COptionTreeItemComboBox();

	// Generated message map functions
protected:
	BOOL m_bFocus;
	long m_lDropDownHeight;
	//{{AFX_MSG(COptionTreeItemComboBox)
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTIONTREEITEMCOMBOBOX_H__EE91CFB4_1C0D_4D13_AD51_803680F51BE2__INCLUDED_)
