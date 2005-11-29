#if !defined(AFX_ANIMLINKPAGE_H__B34862E1_8ED9_11D4_A2FA_00D0B71C8709__INCLUDED_)
#define AFX_ANIMLINKPAGE_H__B34862E1_8ED9_11D4_A2FA_00D0B71C8709__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AnimLinkPage.h : header file
//
#include "AnimPage.h"
#include "AnimObject.h"

//////////////////////////////////////////////////////////////////////
/// Interface for the configuration of the Connector component.
///
/// This pane is available for the following component(s):
///		- CLinkObject
//////////////////////////////////////////////////////////////////////
class CAnimLinkPage : public CAnimPage
{
// Construction
public:
	CAnimLinkPage();   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAnimLinkPage)
	enum { IDD = IDD_ANIMLINK_PAGE };
	CComboBox	m_cStart;
	CComboBox	m_cEnd;
	int		m_nStyleConnector;
	BOOL	m_bArrowEnd;
	BOOL	m_bArrowStart;
	int		m_nIDStart;
	int		m_nIDEnd;
	int		m_nAttachStart;
	int		m_nAttachEnd;
	//}}AFX_DATA
	CStringArray		m_sCpntList;
	CxAnimObjectList*	m_pObjSet;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAnimLinkPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAnimLinkPage)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ANIMLINKPAGE_H__B34862E1_8ED9_11D4_A2FA_00D0B71C8709__INCLUDED_)
