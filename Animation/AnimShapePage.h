#if !defined(AFX_ANIMSHAPEPAGE_H__8A3BFB60_0418_11D4_A2F6_00D0B71C8709__INCLUDED_)
#define AFX_ANIMSHAPEPAGE_H__8A3BFB60_0418_11D4_A2F6_00D0B71C8709__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AnimShapePage.h : header file
//

#include "Tools\ColourPicker.h"
#include "Tools\ColorPickerCB.h"
#include "Tools\line.h"
#include "Tools\linePicker.h"

#include "AnimPage.h"

//////////////////////////////////////////////////////////////////////
/// Interface for the configuration of the component's basic shape.
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
class CAnimShapePage : public CAnimPage
{
// Construction
public:
	CAnimShapePage();   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAnimShapePage)
	enum { IDD = IDD_ANIMSHAPE_PAGE };
	CColorPickerCB		m_BackColorObj;
	CColorPickerCB		m_LineColorObj;
	CLinePicker		m_cLineShape;
	CComboBox		m_cShape;
	CSpinButtonCtrl	m_cSSpin;
	CColourPicker	m_BackColor;
	CColourPicker	m_LineColor;
	int				m_nLSize;
	int		m_nShape;
	//}}AFX_DATA
	CDashLine::DashStyle m_nLineStyle;

	COLORREF		m_clrBk,m_clrLine;
	BOOL			m_bAllowBk;
	CStringArray	m_sVarList;
	CUIntArray		m_sOutcomeIcon;
	CImageList		m_ImageAggList;
	CArray<COLORREF,COLORREF>	m_sVarColorList;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAnimShapePage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAnimShapePage)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeLcolorObj();
	afx_msg void OnSelchangeBcolorObj();
	//}}AFX_MSG
    afx_msg LONG OnSelChange(UINT lParam, LONG wParam);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ANIMSHAPEPAGE_H__8A3BFB60_0418_11D4_A2F6_00D0B71C8709__INCLUDED_)
