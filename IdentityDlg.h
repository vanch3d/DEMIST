#if !defined(AFX_IDENTITYDLG_H__211CAA48_5C9B_11D4_A2FA_00D0B71C8709__INCLUDED_)
#define AFX_IDENTITYDLG_H__211CAA48_5C9B_11D4_A2FA_00D0B71C8709__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// IdentityDlg.h : header file
//


/////////////////////////////////////////////////////////////////////////////
// CComboBoxEx window

class CCompletionCBox : public CComboBox
{
// Construction
public:
	CCompletionCBox();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCompletionCBox)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCompletionCBox();

	BOOL m_bAutoComplete;

	// Generated message map functions
protected:
	//{{AFX_MSG(CCompletionCBox)
	afx_msg void OnEditUpdate();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CIdentityDlg dialog

class CIdentityDlg : public CDialog
{
// Construction
public:
	CIdentityDlg(CWnd* pParent = NULL,BOOL bAuthMode = FALSE);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CIdentityDlg)
	enum { IDD = IDD_IDENTITY };
	CCompletionCBox	m_cName;
	CEdit			m_cPwd;
	CString			m_sName;
	CString			m_sPwd;
	int		m_nSelMode;
	//}}AFX_DATA

	BOOL			m_bMode;	// Actual Mode (0 : author, 1 : learn) ?
	BOOL			m_bForceId;	// Force identification
// Overrides
protected:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIdentityDlg)
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnOK();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CIdentityDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnCloseupComboName();
	afx_msg void OnEditchangeComboName();
	afx_msg void OnTrick();
	afx_msg void OnAuthoringMode();
	afx_msg void OnLearningMode();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IDENTITYDLG_H__211CAA48_5C9B_11D4_A2FA_00D0B71C8709__INCLUDED_)
