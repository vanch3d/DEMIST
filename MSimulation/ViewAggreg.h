#if !defined(AFX_VIEWAGGREG_H__D4002779_7E6A_11D4_A2FA_00D0B71C8709__INCLUDED_)
#define AFX_VIEWAGGREG_H__D4002779_7E6A_11D4_A2FA_00D0B71C8709__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ViewAggreg.h : header file
//

#include "GridCtrl\GridCtrl.h"
#include "GridCtrl\GridBtn\GridBtnCell.h"

/////////////////////////////////////////////////////////////////////////////
/// Interface for the mathematical entities in the Simulation Design Interface.
/////////////////////////////////////////////////////////////////////////////
class CViewSDIaggreg : public CView
{
protected:
	CViewSDIaggreg();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CViewSDIaggreg)

// Attributes
protected:
	CGridCtrl		*m_pGridCtrl;		///< Table containing all the aggregates and their information
    CBtnDataBase	m_BtnDataBase;		///< Interface framework for adding buttons in the table (NYI)
	CImageList		m_pImageList;		///< Image list for the aggregate's icon
public:
	CSimulDoc* GetDocument();
// Operations
public:

	void	OnCellsUpdate(int nModelID);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewSDIaggreg)
	public:
	virtual void OnInitialUpdate();
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CViewSDIaggreg();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CViewSDIaggreg)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
    afx_msg void OnEndGridEdit(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnGridClick(NMHDR *pNotifyStruct, LRESULT* pResult);

	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in SimulView.cpp
inline CSimulDoc* CViewSDIaggreg::GetDocument()
   { return (CSimulDoc*)m_pDocument; }
#endif
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEWAGGREG_H__D4002779_7E6A_11D4_A2FA_00D0B71C8709__INCLUDED_)
