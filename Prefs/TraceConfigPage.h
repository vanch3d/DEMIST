#if !defined(AFX_TRACECONFIGPAGE_H__587B58E9_2849_11D5_A2FA_00D0B71C8709__INCLUDED_)
#define AFX_TRACECONFIGPAGE_H__587B58E9_2849_11D5_A2FA_00D0B71C8709__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TraceConfigPage.h : header file
//

#include "PreferencePage.h"

/////////////////////////////////////////////////////////////////////////////
// CPrefTracePage dialog

class CPrefTracePage : public CPreferencePage
{
// Construction
public:
	CPrefTracePage();   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPrefTracePage)
	enum { IDD = IDD_PREF_CONFIG_PAGE };
	BOOL	m_bClose;
	BOOL	m_bOpen;
	BOOL	m_bSwitch;
	BOOL	m_bPause;
	BOOL	m_bReview;
	BOOL	m_bRun;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPrefTracePage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPrefTracePage)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRACECONFIGPAGE_H__587B58E9_2849_11D5_A2FA_00D0B71C8709__INCLUDED_)
