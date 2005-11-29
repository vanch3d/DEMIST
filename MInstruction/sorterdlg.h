#if !defined(AFX_SORTERDLG_H__1977F504_DF4F_11D5_A2FC_00D0B71C8709__INCLUDED_)
#define AFX_SORTERDLG_H__1977F504_DF4F_11D5_A2FC_00D0B71C8709__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SortERDlg.h : header file
//

#include <MInstruction\LearningUnit.h>
#include "Tools\BtnST.h"

/////////////////////////////////////////////////////////////////////////////
// CSortERDlg dialog

class CSortERDlg : public CDialog
{
// Construction
public:
	CSortERDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSortERDlg)
	enum { IDD = IDD_SORTERDLG_DIALOG };
	CListCtrl	m_cERList;
	CButtonST	m_cERUp;
	CButtonST	m_cERDown;
	//}}AFX_DATA

	CImageList	m_ImageList;
	CImageList	m_IconList;

	//CTypedPtrArray<CObArray,CExternRepUnit*>* m_pMER;
	CxMERSet *m_pMER;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSortERDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSortERDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelectMERUp();
	afx_msg void OnSelectMERDown();
	afx_msg void OnItemchangedMerList(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SORTERDLG_H__1977F504_DF4F_11D5_A2FC_00D0B71C8709__INCLUDED_)
