#if !defined(AFX_ANIMDENSITYPAGE_H__74438D40_3C63_11D4_A2FA_00D0B71C8709__INCLUDED_)
#define AFX_ANIMDENSITYPAGE_H__74438D40_3C63_11D4_A2FA_00D0B71C8709__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AnimDensityPage.h : header file
//

#include "AnimPage.h"

#include "Tools\ColourPicker.h"
#include "Tools\BitmapPickerCombo.h"
//#include "IconComboBox.h"

class CDensityObject;
class CDensityData;

//////////////////////////////////////////////////////////////////////
/// Interface for the configuration of the Population Density component.
///
/// This pane is available for the following component(s):
///		- CDensityObject
//////////////////////////////////////////////////////////////////////
class CAnimDensityPage : public CAnimPage
{
// Construction
public:
	CAnimDensityPage();   // standard constructor
	virtual ~CAnimDensityPage();

// Dialog Data
	//{{AFX_DATA(CAnimDensityPage)
	enum { IDD = IDD_ANIMDENSITY_PAGE };
	CComboBoxEx			m_cOutcomeEx;
	CComboBoxEx			m_cNewIcon;
	CTreeCtrl			m_popTree;
	//CComboBox			m_OutcomeCB;
	CColourPicker		m_clrDef;
	CEdit				m_Prec;
	double				m_nPrec;
	int					m_nOutcomeCB;
	BOOL				m_bShowLegend;
		int				m_bShowValues;
	int		m_nNewIcon;
	//}}AFX_DATA
	CDensityObject*		m_pObjects;
	CStringArray		m_sOutcomeArray;
	CUIntArray			m_sOutcomeIcon;
	CImageList			m_ImageList;
	CImageList			m_ImageAggList;
	CArray<CBitmap*,CBitmap*> m_cBitmapArray;
	CArray<UINT,UINT>		  m_cBitIdxArray;
	CStringArray			  m_cBitStrArray;
	//CBitmap				*m_bmp1,*m_bmp2;

// Overrides
	virtual void OnOK();
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAnimDensityPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAnimDensityPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangingPopTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnShowLegend();
	//}}AFX_MSG
	virtual BOOL OnInitObject(CDensityData *pData);
	virtual BOOL OnValidObject(CDensityData *pData);

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ANIMDENSITYPAGE_H__74438D40_3C63_11D4_A2FA_00D0B71C8709__INCLUDED_)
