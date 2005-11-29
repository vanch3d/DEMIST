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


#if !defined(AFX_OPTIONTREEINFO_H__181F86D7_18EB_4ECE_81F9_489121BB0984__INCLUDED_)
#define AFX_OPTIONTREEINFO_H__181F86D7_18EB_4ECE_81F9_489121BB0984__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptionTreeInfo.h : header file
//

// Added Headers
#include "OptionTreeDef.h"

/////////////////////////////////////////////////////////////////////////////
// COptionTreeInfo window

// Classes
class COptionTree;

class COptionTreeInfo : public CStatic
{
// Construction
public:
	COptionTreeInfo();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COptionTreeInfo)
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetOptionsOwner(COptionTree *otOption);
	virtual ~COptionTreeInfo();

	// Generated message map functions
protected:
	//{{AFX_MSG(COptionTreeInfo)
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	COptionTree *m_otOption;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTIONTREEINFO_H__181F86D7_18EB_4ECE_81F9_489121BB0984__INCLUDED_)
