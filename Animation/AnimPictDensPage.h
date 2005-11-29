#if !defined(AFX_ANIMPICTDENSPAGE_H__80447489_EAE4_11D4_A2FA_00D0B71C8709__INCLUDED_)
#define AFX_ANIMPICTDENSPAGE_H__80447489_EAE4_11D4_A2FA_00D0B71C8709__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AnimPictDensPage.h : header file
//

#include "AnimPage.h"

//////////////////////////////////////////////////////////////////////
/// Interface for the configuration of the Caterpillar component.
///
///
/// This page is available for the following component(s):
///		- CPictDensityObject
//////////////////////////////////////////////////////////////////////
class CAnimPictDensPage : public CAnimPage
{
// Construction
public:
	CAnimPictDensPage();   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAnimPictDensPage)
	enum { IDD = IDD_ANIMPICTDENS_PAGE };
	CComboBox	m_cDataList;
	int		m_nData;
	int		m_nDelta;
	int		m_nAlign;
	BOOL	m_bLog;
	BOOL	m_bShow;
	int		m_nDX;
	int		m_nDY;
	//}}AFX_DATA
	CStringArray	m_sVarList;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAnimPictDensPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAnimPictDensPage)
	afx_msg void OnSetLog();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ANIMPICTDENSPAGE_H__80447489_EAE4_11D4_A2FA_00D0B71C8709__INCLUDED_)
