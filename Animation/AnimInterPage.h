#if !defined(AFX_ANIMINTERPAGE_H__888110E5_A89D_11D6_A2F8_F4A045C59D2B__INCLUDED_)
#define AFX_ANIMINTERPAGE_H__888110E5_A89D_11D6_A2F8_F4A045C59D2B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AnimInterPage.h : header file
//

#include "AnimPage.h"
#include "Tools\BitmapPickerCombo.h"


//////////////////////////////////////////////////////////////////////
/// Interface for the configuration of the interaction with a component.
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
class CAnimInterPage : public CAnimPage
{
// Construction
public:
	CAnimInterPage();   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAnimInterPage)
	enum { IDD = IDD_ANIMINTER_PAGE };
	CBitmapPickerCombo	m_wndDragX;
	CBitmapPickerCombo	m_wndDragY;
	CBitmapPickerCombo	m_wndResizW;
	CBitmapPickerCombo	m_wndResizH;
	CBitmapPickerCombo	m_wndMapRel;
	int		m_nDragX;
	int		m_nDragY;
	int		m_nMapRel;
	int		m_nResH;
	int		m_nResW;
	//}}AFX_DATA

	CImageList		m_pImageList;
	CStringArray	m_sVarList;
	CUIntArray		m_sOutcomeIcon;

	CUIntArray		m_sDragX;
	CUIntArray		m_sDragY;
	CUIntArray		m_sResW;
	CUIntArray		m_sResH;
	CUIntArray		m_sMapRel;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAnimInterPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAnimInterPage)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ANIMINTERPAGE_H__888110E5_A89D_11D6_A2F8_F4A045C59D2B__INCLUDED_)
