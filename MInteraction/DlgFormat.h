#if !defined(AFX_DLGFORMAT_H__B4910D66_5FFC_11D5_A2FA_00D0B71C8709__INCLUDED_)
#define AFX_DLGFORMAT_H__B4910D66_5FFC_11D5_A2FA_00D0B71C8709__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgFormat.h : header file
//

#include "COptionTree\OptionTree.h"
#include "FormatPage.h"

class CFormat;

/////////////////////////////////////////////////////////////////////////////
/// Interface for the configuration of an ER's format.
/////////////////////////////////////////////////////////////////////////////
class CERFormatPage : public CFormatPage
{
// Construction
public:
	CERFormatPage(CFormat *pFormat);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CERFormatPage)
	enum { IDD = IDD_ER_FORMAT };
	//}}AFX_DATA
	COptionTree m_otTree;			///< The option-tree control interfacing 
	CFormat*	m_pFormat;			///< A pointer to the class containing the format of an ER.


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CERFormatPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CERFormatPage)
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	//}}AFX_MSG
	void OnTreeItemChanged(NMHDR* pNotifyStruct, LRESULT* plResult);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGFORMAT_H__B4910D66_5FFC_11D5_A2FA_00D0B71C8709__INCLUDED_)
