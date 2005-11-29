#if !defined(AFX_FORMATPAGE_H__6DB269A1_F0EE_11D6_A2F8_FB433BD4B52B__INCLUDED_)
#define AFX_FORMATPAGE_H__6DB269A1_F0EE_11D6_A2F8_FB433BD4B52B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FormatPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
/// Generic class covering the panes used to configure the format and translation of an ER.
/// Inherited classes cover the various aspects of component's definition:
///		- CERFormatPage to configuring the format of an ER.
///		- CERTranslationPage to configure the translation level of an ER.
/////////////////////////////////////////////////////////////////////////////
class CFormatPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CFormatPage)

// Construction
public:
	CFormatPage();
	CFormatPage(UINT nIDTemplate);
	~CFormatPage();

// Dialog Data
	//{{AFX_DATA(CFormatPage)
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CFormatPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CFormatPage)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FORMATPAGE_H__6DB269A1_F0EE_11D6_A2F8_FB433BD4B52B__INCLUDED_)
