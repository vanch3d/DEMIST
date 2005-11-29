#if !defined(AFX_ANIMSECTORPAGE_H__310258E4_9393_11D4_A2FA_00D0B71C8709__INCLUDED_)
#define AFX_ANIMSECTORPAGE_H__310258E4_9393_11D4_A2FA_00D0B71C8709__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AnimSectorPage.h : header file
//

#include "AnimPage.h"

//////////////////////////////////////////////////////////////////////
/// Interface for the configuration of the Sector component.
///
/// This page is available for the following component(s):
///		- CSectorObject
//////////////////////////////////////////////////////////////////////
class CAnimSectorPage : public CAnimPage
{
// Construction
public:
	CAnimSectorPage();   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAnimSectorPage)
	enum { IDD = IDD_ANIMSECTOR_PAGE };
	CSpinButtonCtrl	m_cSpinPrec;
	CListBox	m_cValues;
	BOOL	m_bLocked;
	int		m_bTitleText;
	int		m_nValueID;
	BOOL	m_bDisplayUnit;
	BOOL	m_bPrevTime;
	CString	m_sTitle;
	int		m_nPrec;
	//}}AFX_DATA
	CFont		m_font;
	COLORREF	m_color;
	LOGFONT		m_lfDefFont;
	CStringArray	m_sVarList;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAnimSectorPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAnimSectorPage)
	afx_msg void OnSetFont();
	afx_msg void OnSetTitleType();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ANIMSECTORPAGE_H__310258E4_9393_11D4_A2FA_00D0B71C8709__INCLUDED_)
