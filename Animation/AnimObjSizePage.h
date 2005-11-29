#if !defined(AFX_ANIMOBJSIZEPAGE_H__8E0B5B87_69F5_11D4_A2FA_00D0B71C8709__INCLUDED_)
#define AFX_ANIMOBJSIZEPAGE_H__8E0B5B87_69F5_11D4_A2FA_00D0B71C8709__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AnimObjSizePage.h : header file
//

#include "AnimPage.h"

#define SEL_SIZE_NONE	0x0000
#define SEL_SIZE_DIM	0x0001
#define SEL_SIZE_AREA	0x0002
#define SEL_SIZE_FIT	0x0004
#define SEL_SIZE_ALL	0x0007


//////////////////////////////////////////////////////////////////////
/// Interface for the configuration of the component's size.
///
///
/// This page is available for the following component(s):
///		- CAnimObject
///		- CLinkObject
///		- CPictDensityObject
///		- CPictObject
///		- CPolygonObject
///		- CSectorObject
///		- CValueObject
///		- CVectorObject
//////////////////////////////////////////////////////////////////////
class CAnimObjSizePage : public CAnimPage
{
// Construction
public:
	CAnimObjSizePage();   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAnimObjSizePage)
	enum { IDD = IDD_ANIMOBJSIZE_PAGE };
	CComboBoxEx	m_cbArea;
	CComboBoxEx	m_cbWidth;
	CComboBoxEx	m_cbHeigth;
	int		m_nHeigth;
	int		m_nWidth;
	int		m_nDefHeight;
	int		m_nDefWidth;
	double	m_nDefHScale;
	double	m_nDefWScale;
	int		m_nDefArea;
	double	m_nDefAScale;
	int		m_nArea;
	int		m_nDimension;
	//}}AFX_DATA
	CStringArray	m_sVarList;
	CUIntArray		m_sOutcomeIcon;
	CImageList		m_ImageAggList;
	BOOL	m_bResize;
	int		m_nSizeType;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAnimObjSizePage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	void EnableComponent(int nb);

	// Generated message map functions
	//{{AFX_MSG(CAnimObjSizePage)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeWidth();
	afx_msg void OnSelchangeHeight();
	afx_msg void OnSelchangeArea();
	afx_msg void OnSelectDim();
	afx_msg void OnSelectArea();
	afx_msg void OnSelectFit();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ANIMOBJSIZEPAGE_H__8E0B5B87_69F5_11D4_A2FA_00D0B71C8709__INCLUDED_)
