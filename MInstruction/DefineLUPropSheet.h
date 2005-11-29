//////////////////////////////////////////////////////////////////////
/// @file DefineLUPropSheet.h
/// @brief Interface for the CLUPropertyPage and LU's property classes
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DEFINELUPROPSHEET_H__D7C37E05_8AE7_11D6_A2F8_B28674E49B2B__INCLUDED_)
#define AFX_DEFINELUPROPSHEET_H__D7C37E05_8AE7_11D6_A2F8_B28674E49B2B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Tools\HelpPrSheet.h"
#include "MultiSlider\multislider.h"
#include "Tools\BtnST.h"

class CSimModel;
class CLearningUnit;
class CDefineLUPropSheet;


/////////////////////////////////////////////////////////////////////////////
/// Generic class covering the configuration interfaces for Learning Units.
/// Inherited classes cover the various aspects of LU's definition (see CLearningUnit) :
///		- CLUModelPage for the 
///		- CLUExpSetPage for the 
///		- CLUInstPage for the 
///		- CLUMERsPage for the 
/// 
/// All these pages are gathered in the CDefineLUPropSheet Property Sheet dialog
/////////////////////////////////////////////////////////////////////////////
class CLUPropertyPage : public CPropertyPage
{
// Construction
public:
	CLUPropertyPage();
	CLUPropertyPage(UINT nIDTemplate);
	~CLUPropertyPage();
};


/////////////////////////////////////////////////////////////////////////////
/// Interface for the configuration of the Simulation Model associated with a LU.
/////////////////////////////////////////////////////////////////////////////
class CLUModelPage : public CLUPropertyPage
{
// Construction
public:
	CLUModelPage(CSimModel *pSim = NULL,CLearningUnit *pLU = NULL);   // standard constructor
	virtual ~CLUModelPage();

// Dialog Data
	//{{AFX_DATA(CLUModelPage)
	enum { IDD = IDD_LU_MODELPAGE };
	CComboBox		m_wndModel;
	CComboBox		m_wndNumMethod;
	CEdit			m_wndHEuler;
	CSpinButtonCtrl	m_wndSpinTimer;
	double			m_dRangeX;
	double			m_dRangeY;
	//int				m_nModel;
	//int				m_nTimer;
	//}}AFX_DATA

	CDefineLUPropSheet	*m_pSheet;	///< Pointer to the parent's dialog box
	CSimModel			*m_pSim;	///< Pointer to the related simulation model
	CLearningUnit		*m_pLU;		///< Pointer to the related Learning Unit

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLUModelPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLUModelPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeModel();
	afx_msg void OnEditupdateModel();
	afx_msg void OnKillFocusTimer();
	afx_msg void OnKillFocusInc();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
/// Interface for the configuration of the Experimental Sets associated with a Learning Unit.
/////////////////////////////////////////////////////////////////////////////
class CLUExpSetPage : public CLUPropertyPage
{
// Construction
public:
	CLUExpSetPage(CSimModel *pSim = NULL,CLearningUnit *pLU = NULL);   // standard constructor
	virtual ~CLUExpSetPage();

// Dialog Data
	//{{AFX_DATA(CLUExpSetPage)
	enum { IDD = IDD_LU_EXPSETPAGE };
	CListBox			m_wndExpSet;
	CButtonST			m_wndESAdd;
	CButtonST			m_wndESDel;
	CButtonST			m_wndESDup;
	CButtonST			m_wndESDef;
	//}}AFX_DATA
	CImageList			m_ImageIcon;

	CDefineLUPropSheet	*m_pSheet;	///< Pointer to the parent's dialog box
	CSimModel			*m_pSim;	///< Pointer to the related simulation model
	CLearningUnit		*m_pLU;		///< Pointer to the related Learning Unit


	void UpdateExpSet();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLUExpSetPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLUExpSetPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnDblclkExpSet();
	afx_msg void OnExpSetDef();
	afx_msg void OnExpSetAdd();
	afx_msg void OnExpSetDup();
	afx_msg void OnExpSetDel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
/// Interface for the configuration of the instructional activities associated with a LU.
/////////////////////////////////////////////////////////////////////////////
class CLUInstPage : public CLUPropertyPage
{
// Construction
public:
	CLUInstPage(CSimModel *pSim = NULL,CLearningUnit *pLU = NULL);   // standard constructor
	virtual ~CLUInstPage();

// Dialog Data
	//{{AFX_DATA(CLUInstPage)
	enum { IDD = IDD_LU_INSTRUCTPAGE };
	CMultiSlider		m_wndBreakSlider;
	CButton				m_wndFirstRun;
	BOOL				m_bFirstRun;
	CButton				m_wndActions;
	BOOL				m_bActions;
	//}}AFX_DATA
	BOOL				m_bEdited;	///< TRUE if Instructional Activities have been added or modified
	int					m_nTimer;	///< Duration of the Learning Unit (Max Time, see CSimModel)
	CSimModel			*m_pSim;	///< Pointer to the related simulation model
	CLearningUnit		*m_pLU;		///< Pointer to the related Learning Unit
	CDefineLUPropSheet	*m_pSheet;	///< Pointer to the parent's dialog box

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLUInstPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	void SetSliders();


// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLUInstPage)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
/// Interface for the configuration of the MERs associated with a LU.
/////////////////////////////////////////////////////////////////////////////
class CLUMERsPage : public CLUPropertyPage
{
// Construction
public:
	CLUMERsPage(CSimModel *pSim = NULL,CLearningUnit *pLU = NULL);   // standard constructor
	virtual ~CLUMERsPage();

// Dialog Data
	//{{AFX_DATA(CLUMERsPage)
	enum { IDD = IDD_LU_MERPAGE };
	CSpinButtonCtrl		m_wndERSpin;
	//}}AFX_DATA
	CDefineLUPropSheet	*m_pSheet;	///< Pointer to the parent's dialog box
	CSimModel			*m_pSim;	///< Pointer to the related simulation model
	CLearningUnit		*m_pLU;		///< Pointer to the related Learning Unit

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLUMERsPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLUMERsPage)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};



/////////////////////////////////////////////////////////////////////////////
/// Property Sheet gathering the panes used for the configuration of a Learning Unit.
/////////////////////////////////////////////////////////////////////////////
class CDefineLUPropSheet : public CHelpPrSheet
{
	DECLARE_DYNAMIC(CDefineLUPropSheet)

// Construction
public:
	CDefineLUPropSheet(	CWnd* pParentWnd = NULL, 
						CSimModel *pSim = NULL,
						CLearningUnit *pLU = NULL,
						UINT iSelectPage = 0);

// Attributes
public:
	CSimModel		*m_pSim;	///< Pointer to the related simulation model
	CLearningUnit	*m_pLU;		///< Pointer to the related Learning Unit

protected:
	CLUModelPage	*m_pModPage;	///< Pointer to LU's Simulation Model configuration dialog box
	CLUExpSetPage	*m_pESPage;	///< Pointer to LU's Experimental Sets configuration dialog box
	CLUInstPage		*m_pInstPage;	///< Pointer to LU's Instructional Activities configuration dialog box
	CLUMERsPage		*m_pMERPage;	///< Pointer to LU's MERs configuration dialog box
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDefineLUPropSheet)
	public:
	virtual int DoModal();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDefineLUPropSheet();

	void InitPropertySheet();
	void SetPages();

	// Generated message map functions
protected:
	//{{AFX_MSG(CDefineLUPropSheet)
	virtual BOOL OnInitDialog();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DEFINELUPROPSHEET_H__D7C37E05_8AE7_11D6_A2F8_B28674E49B2B__INCLUDED_)
