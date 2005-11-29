#if !defined(AFX_ANIMOBJECTPAGE_H__67CA51C0_FF2D_11D3_A2F4_00D0B71C8709__INCLUDED_)
#define AFX_ANIMOBJECTPAGE_H__67CA51C0_FF2D_11D3_A2F4_00D0B71C8709__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AnimObjectPage.h : header file
//

#include "AnimPage.h"

//////////////////////////////////////////////////////////////////////
/// Interface for the configuration of the component's position.
///
///
/// This pane is available for the following component(s):
///		- CAnimObject
///		- CLinkObject
///		- CPictDensityObject
///		- CPictObject
///		- CPolygonObject
///		- CSectorObject
///		- CValueObject
///		- CVectorObject
//////////////////////////////////////////////////////////////////////
class CAnimObjPosPage : public CAnimPage
{
// Construction
public:
	CAnimObjPosPage();   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAnimObjPosPage)
	enum { IDD = IDD_ANIMOBJPOS_PAGE };
	CComboBox	m_cComponent;
	CComboBoxEx	m_cVert;
	CComboBoxEx	m_cHoriz;
	double	m_xScale;
	double	m_yScale;
	int		m_vHoriz;
	int		m_vVert;
	int		m_nDefYPos;
	int		m_nDefXPos;
	BOOL	m_bShowTrace;
	BOOL	m_bAttached;
	int		m_nComponent;
	BOOL	m_bShowAxes;
	//}}AFX_DATA
	CStringArray	m_sVarList;
	CStringArray	m_sCpntList;
	CUIntArray		m_sOutcomeIcon;
	CImageList		m_ImageAggList;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAnimObjPosPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAnimObjPosPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeHoriz();
	afx_msg void OnSelchangeVertic();
	afx_msg void OnSelectAttached();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ANIMOBJECTPAGE_H__67CA51C0_FF2D_11D3_A2F4_00D0B71C8709__INCLUDED_)
