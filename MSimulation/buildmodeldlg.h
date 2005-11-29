#if !defined(AFX_BUILDMODELDLG_H__3396E1E1_807A_11D6_A2F8_F2DDCDCA2028__INCLUDED_)
#define AFX_BUILDMODELDLG_H__3396E1E1_807A_11D6_A2F8_F2DDCDCA2028__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BuildModelDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBuildModelDlg dialog

class CBuildModelDlg : public CDialog
{
// Construction
public:
	CBuildModelDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CBuildModelDlg)
	enum { IDD = IDD_MODEL_BUILDPROP };
	double	m_nCoeff;
	int		m_cNumMethod;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBuildModelDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CBuildModelDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BUILDMODELDLG_H__3396E1E1_807A_11D6_A2F8_F2DDCDCA2028__INCLUDED_)
