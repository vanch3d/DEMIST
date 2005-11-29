#if !defined(AFX_ANIMPOLYGONPAGE_H__E04CB885_98E7_11D6_A2F8_E53F771A9F2B__INCLUDED_)
#define AFX_ANIMPOLYGONPAGE_H__E04CB885_98E7_11D6_A2F8_E53F771A9F2B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AnimPolygonPage.h : header file
//

#include "AnimPage.h"

//////////////////////////////////////////////////////////////////////
/// Interface for the configuration of the Polygon component.
///
///
/// This page is available for the following component(s):
///		- CPolygonObject
//////////////////////////////////////////////////////////////////////
class CAnimPolygonPage : public CAnimPage
{
// Construction
public:
	CAnimPolygonPage();   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAnimPolygonPage)
	enum { IDD = IDD_ANIMPOLYGON_PAGE };
	CComboBox	m_wndPolyPt;
	CComboBox	m_wndPolyLink;
	CTreeCtrl	m_wndPolyList;
	int		m_nPolyLink;
	int		m_nPolyPt;
	//}}AFX_DATA

	CImageList				m_ImageList;
	CArray<CPoint,CPoint>	m_cItems;
	CStringArray			m_sCpntList;


// Overrides
	virtual void OnOK();
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAnimPolygonPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAnimPolygonPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnPolygonAdd();
	afx_msg void OnPolygonDel();
	afx_msg void OnSelchangedPolygon(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ANIMPOLYGONPAGE_H__E04CB885_98E7_11D6_A2F8_E53F771A9F2B__INCLUDED_)
