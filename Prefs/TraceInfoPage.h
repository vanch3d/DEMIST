#if !defined(AFX_TRACEINFOPAGE_H__587B58E8_2849_11D5_A2FA_00D0B71C8709__INCLUDED_)
#define AFX_TRACEINFOPAGE_H__587B58E8_2849_11D5_A2FA_00D0B71C8709__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TraceInfoPage.h : header file
//

#include "PreferencePage.h"

/////////////////////////////////////////////////////////////////////////////
// CPrefCategoryPage dialog

class CPrefCategoryPage : public CPreferencePage
{
// Construction
public:
	CPrefCategoryPage();   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPrefCategoryPage)
	enum { IDD = IDD_PREF_CAT_PAGE };
	CEdit	m_cCatString;
	CListBox	m_cCatList;
	CString	m_strFolder;
	//}}AFX_DATA
	int		m_nSel;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPrefCategoryPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPrefCategoryPage)
	afx_msg void OnBrowse();
	afx_msg void OnAddCat();
	afx_msg void OnDelCat();
	afx_msg void OnSelchangeCat();
	afx_msg void OnChangeCategory();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRACEINFOPAGE_H__587B58E8_2849_11D5_A2FA_00D0B71C8709__INCLUDED_)
