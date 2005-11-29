#if !defined(AFX_VIEWLISTMODEL_H__C528B176_6DDF_11D4_A2FA_00D0B71C8709__INCLUDED_)
#define AFX_VIEWLISTMODEL_H__C528B176_6DDF_11D4_A2FA_00D0B71C8709__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ViewListModel.h : header file
//

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "Tools\BtnST.h"

/////////////////////////////////////////////////////////////////////////////
/// Provides the interface for displaying and managing the list of Simulation Models in the SDI.
/////////////////////////////////////////////////////////////////////////////
class CViewSDIlist : public CFormView
{
protected:
	CViewSDIlist();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CViewSDIlist)

// Form Data
public:
	//{{AFX_DATA(CViewSDIlist)
	enum { IDD = IDD_MODEL_MODEL };
	CButtonST	m_ctrDel;				///< Delete Model button
	CButtonST	m_ctrAdd;				///< Add Model button
	CListCtrl	m_ctrModelList;			///< List box containing all the simulation model of the scenario
	//}}AFX_DATA

// Attributes
public:

protected:
	CImageList		m_ImageList;		///< Image list for the simulation models.
	CImageList		m_ImageIcon;		///< Image list for the buttons of the interface.
	BOOL			m_bCanProceed;		///< TRUE if the interface is fully initialised, FALSE otherwise.

// Operations
public:
	CSimulDoc* GetDocument();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewSDIlist)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

	void OnUpdateModels();

// Implementation
protected:
	virtual ~CViewSDIlist();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CViewSDIlist)
	afx_msg void OnModelAdd();
	afx_msg void OnModelDel();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnItemchangedModelList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickModelList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRclickModelList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKeydownModelList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEndlabeleditModelList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkModelList(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in SimulView.cpp
inline CSimulDoc* CViewSDIlist::GetDocument()
   { return (CSimulDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEWLISTMODEL_H__C528B176_6DDF_11D4_A2FA_00D0B71C8709__INCLUDED_)
