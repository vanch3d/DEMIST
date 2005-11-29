#if !defined(AFX_VARIABLEDEFDLG_H__09288792_1513_11D4_A2F8_00D0B71C8709__INCLUDED_)
#define AFX_VARIABLEDEFDLG_H__09288792_1513_11D4_A2F8_00D0B71C8709__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// VariableDefDlg.h : header file
//


//#include "GridCtrl\GridCtrl.h"
//#include "GridCtrl\GridBtnCell\GridBtnCellBase.h"

#include "ColourPicker.h"
#include "IconComboBox.h"

class CMdEquation;
class CModelObj;
/////////////////////////////////////////////////////////////////////////////
// CVariableDefDlg dialog

class CVariableDefDlg : public CDialog
{
// Construction
public:
	CVariableDefDlg(CWnd* pParent = NULL,const CMdEquation *pEqu= NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CVariableDefDlg)
	enum { IDD = IDD_MODEL_VARIABLE };
	CSmallIconComboBox	m_DefIcon;
	CComboBox	m_DefUnit;
	CEdit	m_Desc;
	CEdit	m_MinVal;
	CEdit	m_MaxVal;
	CEdit	m_InitVal;
	CTreeCtrl	m_parTree;
	CColourPicker	m_clrDefault;
	CString	m_sDesc;
	double	m_nInitVal;
	double	m_nMaxVal;
	double	m_nMinVal;
	CString	m_sDefUnit;
	UINT	m_nDefIcon;
	//}}AFX_DATA
	const CMdEquation	*m_pEquation;
	CImageList			m_pImageList;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVariableDefDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL
	virtual void OnOK( );


// Implementation
protected:
	void SetProperties(CModelObj* pObj);
	void GetProperties(CModelObj* pObj);

	// Generated message map functions
	//{{AFX_MSG(CVariableDefDlg)
	virtual BOOL OnInitDialog();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	afx_msg void OnSelchangingTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangedTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEndlabeleditTree(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VARIABLEDEFDLG_H__09288792_1513_11D4_A2F8_00D0B71C8709__INCLUDED_)
