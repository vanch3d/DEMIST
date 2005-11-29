// Simul.h : main header file for the SIMUL application
//

#if !defined(AFX_SIMUL_H__40951553_F048_11D3_A2F4_00D0B71C8709__INCLUDED_)
#define AFX_SIMUL_H__40951553_F048_11D3_A2F4_00D0B71C8709__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

class CMvDocTemplate;


/////////////////////////////////////////////////////////////////////////////
/// Provides the functionality for initializing and running DEMIST.
/////////////////////////////////////////////////////////////////////////////
class CSimulApp : public CWinApp
{
private:
	CMvDocTemplate *m_pDocTmpl;	///< Pointer to the Document Manager implemented in DEMIST.
public:
	CSimulApp();

	CMvDocTemplate *GetDocManager() {return m_pDocTmpl;};

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSimulApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CSimulApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	afx_msg void OnUpdateFileNew(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewDesign(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SIMUL_H__40951553_F048_11D3_A2F4_00D0B71C8709__INCLUDED_)
