#if !defined(AFX_PREFERENCEPAGE_H__019BFD81_F6FC_11D6_B8E1_00D0B71C8709__INCLUDED_)
#define AFX_PREFERENCEPAGE_H__019BFD81_F6FC_11D6_B8E1_00D0B71C8709__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PreferencePage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
/// Generic class covering the interfaces for DEMIST's Preferences.
/// Inherited classes cover the various aspects of component's definition:
///		- CAnimDensityPage for the 
///		- CAnimLinkPage for the 
/////////////////////////////////////////////////////////////////////////////
class CPreferencePage : public CPropertyPage
{
	DECLARE_DYNCREATE(CPreferencePage)

// Construction
public:
	CPreferencePage();
	CPreferencePage(UINT nIDTemplate);
	~CPreferencePage();

// Dialog Data
	//{{AFX_DATA(CPreferencePage)
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPreferencePage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPreferencePage)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PREFERENCEPAGE_H__019BFD81_F6FC_11D6_B8E1_00D0B71C8709__INCLUDED_)
