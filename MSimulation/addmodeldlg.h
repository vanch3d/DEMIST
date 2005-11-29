#if !defined(AFX_ADDMODELDLG_H__108411EC_B33F_11D5_A2FB_00D0B71C8709__INCLUDED_)
#define AFX_ADDMODELDLG_H__108411EC_B33F_11D5_A2FB_00D0B71C8709__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AddModelDlg.h : header file
//

#include "Tools\HelpDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CAddModelDlg dialog
class CMdEquation;
class CSimModel;

class CAddModelDlg : public CHelpDlg
{
// Construction
public:
	CAddModelDlg(CWnd* pParent = NULL);   // standard constructor
	~CAddModelDlg(); 

	enum {ADD_TEMPLATE,ADD_RECENT};

// Dialog Data
	//{{AFX_DATA(CAddModelDlg)
	enum { IDD = IDD_MODEL_ADD };
	CStatic	m_ctrlDesc;
	CListCtrl	m_ctrlList;
	CTabCtrl	m_ctrlTab;
	//}}AFX_DATA

	CTypedPtrArray<CObArray,CMdEquation*>	m_cEquSet;	// List of equations
	int				m_nCat;
	CMdEquation		*m_pEq;
	CSimModel		*m_pSimul;		// Simulation Model
	CImageList		m_ImageList;
	CImageList		m_TabImageList;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAddModelDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void UpdateModel(int nCat);

	// Generated message map functions
	//{{AFX_MSG(CAddModelDlg)
	virtual void OnOK();
	afx_msg void OnSelChangeTab(NMHDR* pNMHDR, LRESULT* pResult);
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADDMODELDLG_H__108411EC_B33F_11D5_A2FB_00D0B71C8709__INCLUDED_)
