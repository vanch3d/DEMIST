#if !defined(AFX_ANIMPICTPAGE_H__AD6CEEA6_FF45_11D3_A2F4_00D0B71C8709__INCLUDED_)
#define AFX_ANIMPICTPAGE_H__AD6CEEA6_FF45_11D3_A2F4_00D0B71C8709__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AnimPictPage.h : header file
//
#include "Tools\ColourPicker.h"
#include "AnimPage.h"

//////////////////////////////////////////////////////////////////////
/// Interface for the configuration of the Static Image component.
///
/// This page is available for the following component(s):
///		- CPictDensityObject
///		- CPictObject
//////////////////////////////////////////////////////////////////////
class CAnimPictPage : public CAnimPage
{
// Construction
public:
	CAnimPictPage();    // standard constructor

// Dialog Data
	//{{AFX_DATA(CAnimPictPage)
	enum { IDD = IDD_ANIMPICT_PAGE };
	CColourPicker	m_TransColor;
	BOOL			m_bTransBk;
	CString			m_sFileName;
	//}}AFX_DATA
	BOOL			m_bIsFileSelected;
	COLORREF		m_clrTrans;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAnimPictPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAnimPictPage)
	afx_msg void OnBrowse();
	afx_msg void OnSetBackground();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ANIMPICTPAGE_H__AD6CEEA6_FF45_11D3_A2F4_00D0B71C8709__INCLUDED_)
