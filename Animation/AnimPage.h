#if !defined(AFX_ANIMPAGE_H__7B59FA80_EC0E_11D6_B8E1_00D0B71C8709__INCLUDED_)
#define AFX_ANIMPAGE_H__7B59FA80_EC0E_11D6_B8E1_00D0B71C8709__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AnimPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
/// Generic class derived for every configuration panes for the Animation Components.
/// Inherited classes cover the various aspects of component's definition:
///		- CAnimDensityPage for the 
///		- CAnimLinkPage for the 
///		- CAnimObjPosPage for the 
///		- CAnimObjSizePage for the 
///		- CAnimPictDensPage for the 
///		- CAnimPictPage for the 
///		- CAnimInterPage for the 
///		- CAnimPolygonPage for the 
///		- CAnimSectorPage for the 
///		- CAnimShapePage for the 
///		- CAnimTextPage for the 
///		- CAnimValuePage for the 
/// 
/////////////////////////////////////////////////////////////////////////////
class CAnimPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CAnimPage)

// Construction
public:
	CAnimPage();
	CAnimPage(UINT nIDTemplate);
	~CAnimPage();

// Dialog Data
	//{{AFX_DATA(CAnimPage)
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CAnimPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CAnimPage)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ANIMPAGE_H__7B59FA80_EC0E_11D6_B8E1_00D0B71C8709__INCLUDED_)
