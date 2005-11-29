#if !defined(AFX_BPINFODLG_H__688B9640_067F_11D6_A2FC_00D0B71C8709__INCLUDED_)
#define AFX_BPINFODLG_H__688B9640_067F_11D6_A2FC_00D0B71C8709__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BPInfoDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBPInfoDlg dialog

class CBPInfoDlg : public CDialog
{
// Construction
public:
	CBPInfoDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CBPInfoDlg)
	enum { IDD = IDD_BPMESSAGE_DLG };
	CRichEditCtrl	m_wndDesc;
	CRichEditCtrl	m_wndTitle;
	CStatic	m_wndIcon;
	CString	m_strDesc;
	CString	m_strTitle;
	//}}AFX_DATA
	UINT	m_nIcon;
	CBitmap	m_wndBitmap;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBPInfoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CBPInfoDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BPINFODLG_H__688B9640_067F_11D6_A2FC_00D0B71C8709__INCLUDED_)
