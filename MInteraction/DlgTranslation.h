#if !defined(AFX_DLGTRANSLATION_H__9ECB7B20_F5DD_11D4_A2FA_00D0B71C8709__INCLUDED_)
#define AFX_DLGTRANSLATION_H__9ECB7B20_F5DD_11D4_A2FA_00D0B71C8709__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgTranslation.h : header file
//

#include <Minteraction\FormatPage.h>
#include "Tools\ColourPicker.h"


/////////////////////////////////////////////////////////////////////////////
/// CERTranslationPage dialog
/////////////////////////////////////////////////////////////////////////////
class CERTranslationPage : public CFormatPage
{
// Construction
public:
	DECLARE_SERIAL(CERTranslationPage)
	CERTranslationPage();   // standard constructor

// Dialog Data
	//{{AFX_DATA(CERTranslationPage)
	enum { IDD = IDD_ER_TRANSLATION };
	int		m_nTranslation;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CERTranslationPage)
	public:
	virtual void Serialize(CArchive& ar);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CERTranslationPage)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CDlgFormatText dialog

class CDlgFormatText : public CFormatPage
{
// Construction
public:
	DECLARE_SERIAL(CDlgFormatText)
	CDlgFormatText();   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgFormatText)
	enum { IDD = IDD_FORMAT_TEXT };
	CColourPicker	m_cVar;
	CColourPicker	m_cParam;
	CColourPicker	m_cConst;
	CStatic			m_cSample;
	int				m_nDisplayMode;
	//}}AFX_DATA
	COLORREF		m_clrParam,
					m_clrVar,
					m_clrCst;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgFormatText)
	public:
	virtual void Serialize(CArchive& ar);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgFormatText)
	afx_msg void OnSetFont();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGTRANSLATION_H__9ECB7B20_F5DD_11D4_A2FA_00D0B71C8709__INCLUDED_)
