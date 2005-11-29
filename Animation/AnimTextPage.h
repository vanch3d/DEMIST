#if !defined(AFX_ANIMTEXTPAGE_H__56EEFB1A_FF08_11D3_A2F4_00D0B71C8709__INCLUDED_)
#define AFX_ANIMTEXTPAGE_H__56EEFB1A_FF08_11D3_A2F4_00D0B71C8709__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AnimTextPage.h : header file
//

#include "AnimPage.h"

//////////////////////////////////////////////////////////////////////
/// Interface for the configuration of the Static Text component.
///
///
/// This page is available for the following component(s):
///		- CTextObject
//////////////////////////////////////////////////////////////////////
class CAnimTextPage : public CAnimPage
{
// Construction
public:
	CAnimTextPage();   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAnimTextPage)
	enum { IDD = IDD_ANIMTEXT_PAGE };
	int		m_Align;
	CString	m_TextInput;
	//}}AFX_DATA
	CFont m_font;
	COLORREF m_color;
	LOGFONT m_lfDefFont;
	//static LOGFONT NEAR m_lfDefFontOld;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAnimTextPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAnimTextPage)
	afx_msg void OnSetfont();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ANIMTEXTPAGE_H__56EEFB1A_FF08_11D3_A2F4_00D0B71C8709__INCLUDED_)
