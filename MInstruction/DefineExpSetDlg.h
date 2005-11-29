#if !defined(AFX_DEFINEEXPSETDLG_H__82C65560_268C_11D4_A2FA_00D0B71C8709__INCLUDED_)
#define AFX_DEFINEEXPSETDLG_H__82C65560_268C_11D4_A2FA_00D0B71C8709__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


/////////////////////////////////////////////////////////////////////////////
// CDefineExpSetDlg dialog
#include "GridCtrl\GridCtrl.h"
#include "Tools\HelpDlg.h"

class CMdEquation;
class CMdExpertSet;


/////////////////////////////////////////////////////////////////////////////
// Interface for the configuration of an Experimental Set - OBSOLETE
/////////////////////////////////////////////////////////////////////////////
class CDefineExpSetDlg : public CHelpDlg
{
// Construction
public:
	CDefineExpSetDlg(CWnd* pParent = NULL,
					 CMdEquation *pEqu = NULL,
					 CMdExpertSet *pExpSet = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CDefineExpSetDlg)
	enum { IDD = IDD_DEFINE_EXPSET };
	CString	m_ExpName;
	//}}AFX_DATA
protected:
	CImageList		m_pImageList;
	CGridCtrl	m_Parlist;
	//CGridCtrl	m_InitList;
	CMdEquation *m_pEqu;
	CMdExpertSet *m_pExpSet;


// Overrides
protected:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDefineExpSetDlg)
	virtual void OnOK();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDefineExpSetDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDefaultValue();
	//}}AFX_MSG
    afx_msg void OnParamEditChange(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DEFINEEXPSETDLG_H__82C65560_268C_11D4_A2FA_00D0B71C8709__INCLUDED_)
