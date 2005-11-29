#if !defined(AFX_SIMULTABLE_H__40951581_F048_11D3_A2F4_00D0B71C8709__INCLUDED_)
#define AFX_SIMULTABLE_H__40951581_F048_11D3_A2F4_00D0B71C8709__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SimulTable.h : header file
//

#include "GridCtrl\GridCtrl.h"

#include <MInstruction\LearningUnit.h>

class CFormatTable;

/////////////////////////////////////////////////////////////////////////////
/// Class used to implement the interface of a Table ER.
///
///
/////////////////////////////////////////////////////////////////////////////
class CViewTable : public CFormView
{
protected:

	CViewTable();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CViewTable)

// Form Data
public:
	//{{AFX_DATA(CViewTable)
	enum { IDD = IDD_SIMULTABLE_FORM };
	//}}AFX_DATA

// Attributes
public:
	CSimulDoc*			GetDocument();
	CFormatTable*		GetFormats();
	int					GetTranslation();
protected:
	CExternRepUnit*	m_pER;
	CToolBar		m_cToolBar;
	CGridCtrl		m_pGridCtrl;
	CImageList		m_pImageList;
//	int				m_ShowData[20];
//	CArray<TableInfo,TableInfo>		m_info;
	CString			m_strModifiedCell;
//	int				m_nCurrTTask;
	BOOL			m_bIsInEdit;
	int				m_nOldTimer;
	double			m_nDeltaTime;
// Operations
public:

	BOOL FormatCells(int nTime,BOOL nEditBP=FALSE);
	BOOL FormatEditCells(int nTime,BOOL nEditBP=TRUE);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewTable)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

	BOOL OnUpdateData();									// Response to data update
	BOOL OnUpdateTimer();									// Response to timer update
	BOOL OnUpdateBreakPoint(int nBPTime,int nBPType);		// Response to a breakpoint
	BOOL OnUpdateHypothesis(CUserData *pData);				// Response to an hypothesis
	BOOL OnUpdateAction(CUserData *pData);					// Response to an action
	BOOL OnUpdateMapRelation(CTranslationInfo *mTInfo );	// Response to a MAP RELATION 

// Implementation
protected:
	virtual ~CViewTable();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	void InitView();

	// Generated message map functions
protected:
	//{{AFX_MSG(CViewTable)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSelectdata();
	afx_msg void OnEditCopy();
	afx_msg void OnDestroy();
	//}}AFX_MSG
    afx_msg void OnGridDblClick(NMHDR *pNotifyStruct, LRESULT* pResult);
    afx_msg void OnGridSelChanged(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnGridSelChanging(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnGridBeginEdit(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnGridEndEdit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg LRESULT OnUpdateInitContent(WPARAM, LPARAM); 
	afx_msg LRESULT OnActivateER(WPARAM, LPARAM); 
	afx_msg void OnTranslationTasks(UINT nID);
	afx_msg void OnUpdateTransTasks(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in simulView.cpp
inline CSimulDoc* CViewTable::GetDocument()
   { return (CSimulDoc*)m_pDocument; }
#endif
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SIMULTABLE_H__40951581_F048_11D3_A2F4_00D0B71C8709__INCLUDED_)
