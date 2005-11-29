#if !defined(AFX_ANIMVALUEPAGE_H__48D14505_03C2_11D4_A2F6_00D0B71C8709__INCLUDED_)
#define AFX_ANIMVALUEPAGE_H__48D14505_03C2_11D4_A2F6_00D0B71C8709__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AnimValuePage.h : header file
//

#include "AnimPage.h"

//////////////////////////////////////////////////////////////////////
/// Interface for the configuration of the Dynamic Value component.
///
///
/// This page is available for the following component(s):
///		- CValueObject
//////////////////////////////////////////////////////////////////////
class CAnimValuePage : public CAnimPage
{
// Construction
public:
	CAnimValuePage();   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAnimValuePage)
	enum { IDD = IDD_ANIMVALUE_PAGE };
	CComboBoxEx		m_cValueEx;
	CSpinButtonCtrl	m_cSpinPrec;
	int				m_iPrecision;
	int				m_Align;
	int				m_vValue;
	BOOL			m_bDisplayName;
	BOOL			m_bDisplayUnit;
	BOOL			m_bPrevTime;
	//}}AFX_DATA
	COLORREF		m_color;
	CFont			m_font;
	LOGFONT			m_lfDefFont;
	CStringArray	m_sVarList;
	CUIntArray		m_sOutcomeIcon;
	CImageList		m_ImageAggList;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAnimValuePage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:

	// Generated message map functions
	//{{AFX_MSG(CAnimValuePage)
	afx_msg void OnSetfont();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ANIMVALUEPAGE_H__48D14505_03C2_11D4_A2F6_00D0B71C8709__INCLUDED_)
