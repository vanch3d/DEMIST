//////////////////////////////////////////////////////////////////////
/// @file DefineERPropSheet.h
/// @brief Interface for the CDefineERPropSheet and ER's property classes
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DEFINEERPROPSHEET_H__5143F4E4_8BC3_11D6_A2F8_955B81D9FB2B__INCLUDED_)
#define AFX_DEFINEERPROPSHEET_H__5143F4E4_8BC3_11D6_A2F8_955B81D9FB2B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Tools\HelpPrSheet.h"
#include "GridCtrl\GridCtrl.h"

class CExternRepUnit;
class CMdEquation;
class CLearningUnit;
class CDefineERPropSheet;


/////////////////////////////////////////////////////////////////////////////
/// Generic class covering the configuration interfaces for the External Representations.
/// Inherited classes cover the various aspects of LU's definition (see CLearningUnit) :
///		- CEROutcomePage for the 
///		- CEROutcomeXYPage for the 
///		- CERDefinePage for the 
/// 
/// All these pages are collected in the CDefineLUPropSheet Property Sheet dialog
/////////////////////////////////////////////////////////////////////////////
class CERPropertyPage : public CPropertyPage
{
// Construction
public:
	CERPropertyPage();
	CERPropertyPage(UINT nIDTemplate);
	~CERPropertyPage();
};

/////////////////////////////////////////////////////////////////////////////
/// Interface for the configuration of an External Representation.
/////////////////////////////////////////////////////////////////////////////
class CERDefinePage : public CERPropertyPage
{
// Construction
public:
	CERDefinePage(CExternRepUnit *pER = NULL,
				CMdEquation *pEqu = NULL,
				CLearningUnit *pLU = NULL);   // standard constructor
	virtual ~CERDefinePage();

// Dialog Data
	//{{AFX_DATA(CERDefinePage)
	enum { IDD = IDD_ER_EXTREP };
	CComboBox		m_wndERType;
	CComboBox		m_wndERCat;
	CEdit			m_wndERName;
	CSpinButtonCtrl	m_wndERIconSpin;
	CStatic			m_wndERIcon;
	CButton			m_wndEROpen;
	CButton			m_wndERKeep;
	BOOL			m_bStartOpen;
	BOOL			m_bKeepOpen;
	CString			m_strName;
	int				m_nCategory;
	int				m_nExtRep;
	//}}AFX_DATA

	CArray<UINT,UINT>	m_nIconIDList;
	int					m_nCurrIcon;

	CDefineERPropSheet	*m_pSheet;
	CExternRepUnit		*m_pER;
	CMdEquation			*m_pEqu;
	CLearningUnit		*m_pLU;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CERDefinePage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void SetMERIcon(UINT id);
	void OnPreparePages();

	// Generated message map functions
	//{{AFX_MSG(CERDefinePage)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeER();
	afx_msg void OnChangeIcon(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelectSart();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
/// Interface for the configuration of an ER's outcomes.
/////////////////////////////////////////////////////////////////////////////
class CEROutcomePage : public CERPropertyPage
{
// Construction
public:
	CEROutcomePage(CExternRepUnit *pER = NULL,
				CMdEquation *pEqu = NULL,
				CLearningUnit *pLU = NULL);   // standard constructor
	virtual ~CEROutcomePage();

// Dialog Data
	//{{AFX_DATA(CEROutcomePage)
	enum { IDD = IDD_ER_OUTCOME };
	BOOL			m_bSyncDef;
	BOOL			m_bSetDefault;
	CGridCtrl		m_wndOutGrid;
	//}}AFX_DATA
	CImageList		m_pImageList;

	BOOL				m_bForceSync;
	CDefineERPropSheet	*m_pSheet;
	CExternRepUnit		*m_pER;
	CMdEquation			*m_pEqu;
	CLearningUnit		*m_pLU;

// Overrides
public:
	virtual void OnOK();

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEROutcomePage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CEROutcomePage)
	virtual BOOL OnInitDialog();
	afx_msg void OnSynchronise();
	afx_msg void OnRestoreDefault();
	afx_msg void OnGridClick(NMHDR *pNotifyStruct, LRESULT* pResult);
	afx_msg void OnGridRClick(NMHDR *pNotifyStruct, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////
/// Interface for the configuration of the XY-Graph's outcomes.
/////////////////////////////////////////////////////////////////////////////
class CEROutcomeXYPage : public CERPropertyPage
{
// Construction
public:
	CEROutcomeXYPage(CExternRepUnit *pER = NULL,
				CMdEquation *pEqu = NULL,
				CLearningUnit *pLU = NULL);   // standard constructor
	virtual ~CEROutcomeXYPage();

// Dialog Data
	//{{AFX_DATA(CEROutcomeXYPage)
	enum { IDD = IDD_ER_OUTCOME_XY };

	CComboBoxEx			m_wndOutY;
	CComboBoxEx			m_wndOutX;
	CComboBoxEx			m_wndStabY;
	CComboBoxEx			m_wndStabX;
	CComboBoxEx			m_wndIso1Y;
	CComboBoxEx			m_wndIso1X;
	CComboBoxEx			m_wndIso2Y;
	CComboBoxEx			m_wndIso2X;
	CGridCtrl			m_wndOutGrid;
	int					m_nXOutcome;
	int					m_nYOutcome;
	int					m_nIso1X;
	int					m_nIso1Y;
	int					m_nIso2X;
	int					m_nIso2Y;
	int					m_nStabX;
	int					m_nStabY;
	//}}AFX_DATA
	CImageList			m_ImageList;
	CImageList			m_ImageAggList;

	CDefineERPropSheet	*m_pSheet;
	CExternRepUnit		*m_pER;
	CMdEquation			*m_pEqu;
	CLearningUnit		*m_pLU;

// Overrides
public:
	virtual void OnOK();

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEROutcomeXYPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CEROutcomeXYPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnGridClick(NMHDR *pNotifyStruct, LRESULT* pResult);
	afx_msg void OnGridRClick(NMHDR *pNotifyStruct, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};




/////////////////////////////////////////////////////////////////////////////
/// Property Sheet gathering the panes used for the configuration of an External Representation.
/////////////////////////////////////////////////////////////////////////////
class CDefineERPropSheet : public CHelpPrSheet
{
	DECLARE_DYNAMIC(CDefineERPropSheet)

// Construction
public:
	CDefineERPropSheet(
				CWnd* pParentWnd = NULL,
				CExternRepUnit *pER = NULL,
				CMdEquation *pEqu = NULL,
				CLearningUnit *pLU = NULL,
				UINT iSelectPage = 0);

// Attributes
public:
	CExternRepUnit	*m_pER;
	CMdEquation		*m_pEqu;
	CLearningUnit	*m_pLU;
	UINT			m_uID;
	CString			m_strTitle;
	

protected:
	CERDefinePage			*pLUProp;
	CEROutcomePage		*pLUOut;
	CEROutcomeXYPage	*pLUOutXY;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDefineERPropSheet)
	public:
	virtual int DoModal();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDefineERPropSheet();
	void InitPropertySheet();
	void SetPages(BOOL bAdd);

	// Generated message map functions
protected:
	//{{AFX_MSG(CDefineERPropSheet)
		// NOTE - the ClassWizard will add and remove member functions here.
	virtual BOOL OnInitDialog();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DEFINEERPROPSHEET_H__5143F4E4_8BC3_11D6_A2F8_955B81D9FB2B__INCLUDED_)
