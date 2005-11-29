#if !defined(AFX_ANIMSELECTDLG_H__DB90E724_B7E8_11D5_A2FB_00D0B71C8709__INCLUDED_)
#define AFX_ANIMSELECTDLG_H__DB90E724_B7E8_11D5_A2FB_00D0B71C8709__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AnimSelectDlg.h : header file
//

#include "Tools\BtnST.h"
#include "Tools\xShadeButton.h"
#include "Tools\HelpDlg.h"

#include "AnimObject.h"

class CMultiRectTracker;
class CViewAnimat;

/////////////////////////////////////////////////////////////////////////////
/// Interface for the Component Selector, available during the Animation's authoring procedure.
///
/////////////////////////////////////////////////////////////////////////////
class CSelectorDlg : public CHelpDlg
{
// Construction
public:
	CSelectorDlg(CView* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSelectorDlg)
	enum { IDD = IDD_ANIMATION_SELECT };
	CButtonST		m_cViewSave;			///< Interface for the Save Components button
	CButtonST		m_cViewLoad;			///< Interface for the Load Components button
	CButtonST		m_cViewIcon;			///< Interface for the Iconic view button
	CListCtrl		m_cObjList;				///< List of the animation components included in the ER
	CButtonST		m_cViewList;			///< Interface for the List View button
	int				m_nView;				///< Zero-based index of the type of view used (0 for icons, 1 for list).
	//}}AFX_DATA

	CViewAnimat*		pView;				///< Pointer to the view containing the components
	CxAnimObjectList*	m_cObjSet;			///< Pointer to the list of components included in the ER.
	CMultiRectTracker*	pmultiTrack;		///< Pointer to the multi-rect tracker used in the Animation ER (see CViewAnimat::m_wndMultiTrack)
	CImageList			m_ImageList;		///< Image list for the components' icons
	CImageList			m_BtnImageList;		///< Inage list for the dialog's buttons

	void UpdateList();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSelectorDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSelectorDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnViewIcon();
	afx_msg void OnViewList();
	afx_msg void OnEndObjLabelEdit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemChanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	afx_msg void OnDblclkObjList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKeydownObjList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLoad();
	afx_msg void OnSave();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ANIMSELECTDLG_H__DB90E724_B7E8_11D5_A2FB_00D0B71C8709__INCLUDED_)
