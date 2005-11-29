#if !defined(AFX_SELECTPARAMDLG_H__A92AFC80_F1CE_11D3_A2F4_00D0B71C8709__INCLUDED_)
#define AFX_SELECTPARAMDLG_H__A92AFC80_F1CE_11D3_A2F4_00D0B71C8709__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <MInstruction\LearningUnit.h>
#include "Tools\HelpDlg.h"

#define MAX_ITEMS_IN_LIST 20

/////////////////////////////////////////////////////////////////////////////
/// Interface provided to the user in order to change the outcomes displayed in an ER.
///
/// (see CExternRepUnit and CEROutcomePage).
/// This dialog is available in the following ERs:
///		- X/Time Graph (CViewGraph)
///		- Phaseplot (CViewPhasePlot)
///		- Table	(CViewTable)
/// All the other ERs do not allow such modification,
/////////////////////////////////////////////////////////////////////////////
class CSelectOutcomeDlg : public CHelpDlg
{
// Construction
public:
	CSimulDoc*		GetDocument();

	CSelectOutcomeDlg(CWnd* pParent = NULL,CDocument* pDoc=0);   // standard constructor
	~CSelectOutcomeDlg();

// Dialog Data
	//{{AFX_DATA(CSelectOutcomeDlg)
	enum { IDD = IDD_SELECTPARAM };
	CStatic			m_strDescr;			///< Static Text used to display the description of the selected outcome (see CModelObj::m_sDesc).
	CTabCtrl		m_wndTabExpSet;		///< Tab Control used to display all the Experimental Sets of the Learning Unit.
	CListCtrl		m_wndSelectParam;	///< List Control used to dislpay all the outcomes available for configuration.
	//}}AFX_DATA

	CxOutcomes		m_cLocOutputSet;	///< List of the outcomes that will be available in the ListBox for configuration.
	CStringArray	m_cTabLabel;		///< List of the Experimental Sets to be displayed as several tabs. 
	BOOL			m_bForceSync;		///< Force all the outcomes of an Experimental Set to be defined simultaneously (used for the PhasePlot).
	int				m_nSelected;		///< Refer to the item currently selected by the user (used to update the help field).

private:
	CImageList		m_pImageList;
	CDocument*		m_pDoc;				///< Pointer to the document associated with this ERs.
	BOOL			m_bFirstOpen;		///< FALSE if the dialog is not yet created (used internally to prevent irrelevant updates).

// Overrides
protected:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSelectOutcomeDlg)
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:	
	virtual void	OnOK();
	BOOL			OnInitTab(int nTab=0);

	// Generated message map functions
	//{{AFX_MSG(CSelectOutcomeDlg)
	virtual BOOL	OnInitDialog();
	afx_msg void	OnItemchanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void	OnSelchangeTabExpset(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemclickListparam(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickListparam(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnColumnclickListparam(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SELECTPARAMDLG_H__A92AFC80_F1CE_11D3_A2F4_00D0B71C8709__INCLUDED_)
