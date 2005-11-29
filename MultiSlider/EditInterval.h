#if !defined(AFX_EDITINTERVAL_H__274F44B4_B484_11D3_B7E7_00C04F0DA600__INCLUDED_)
#define AFX_EDITINTERVAL_H__274F44B4_B484_11D3_B7E7_00C04F0DA600__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditInterval.h : header file
//

#include "Tools\HelpDlg.h"


/////////////////////////////////////////////////////////////////////////////
// CEditInterval dialog

class CEditInterval : public CHelpDlg
{
// Construction
public:
	CImageList		m_pImageList;
	float m_Min, m_Max;
	CEditInterval(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CEditInterval)
	enum { IDD = IDD_EDITINTERVAL };
	CStatic	m_cIconTest;
	CStatic	m_cIconPred;
	CStatic	m_cIconAct;
	CStatic	m_cIconInf;
	float	m_Value;
	CString	m_strDescr;
	int		m_nStyle;
	CString	m_strHTML;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditInterval)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CEditInterval)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnBrowse();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITINTERVAL_H__274F44B4_B484_11D3_B7E7_00C04F0DA600__INCLUDED_)
