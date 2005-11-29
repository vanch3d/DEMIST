#if !defined(AFX_PREFSIMULPAGE_H__5F1471C4_B672_11D5_A2FB_00D0B71C8709__INCLUDED_)
#define AFX_PREFSIMULPAGE_H__5F1471C4_B672_11D5_A2FB_00D0B71C8709__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PrefSimulPage.h : header file
//

#include "PreferencePage.h"
#include "Tools\ColourPicker.h"

/////////////////////////////////////////////////////////////////////////////
// CPrefSimulPage dialog

class CPrefSimulPage : public CPreferencePage
{
// Construction
public:
	CPrefSimulPage();   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPrefSimulPage)
	enum { IDD = IDD_PREF_SIMUL_PAGE };
	CColourPicker	m_cPast;
	CColourPicker	m_cFuture;
	int		m_nSpeedAuth;
	int		m_nSpeedLearn;
	//}}AFX_DATA
	COLORREF	m_clrPast;
	COLORREF	m_clrFuture;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPrefSimulPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPrefSimulPage)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PREFSIMULPAGE_H__5F1471C4_B672_11D5_A2FB_00D0B71C8709__INCLUDED_)
