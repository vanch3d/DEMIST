#if !defined(AFX_COMPONENTSTATIC_H__F8E9A061_B40E_11D5_A2FB_00D0B71C8709__INCLUDED_)
#define AFX_COMPONENTSTATIC_H__F8E9A061_B40E_11D5_A2FB_00D0B71C8709__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ComponentStatic.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CComponentStatic window

class CComponentStatic : public CStatic
{
// Construction
public:
	CComponentStatic();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CComponentStatic)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CComponentStatic();

    virtual void Draw (CDC* pDC,CRect mrect) {};

	// Generated message map functions
protected:
	//{{AFX_MSG(CComponentStatic)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COMPONENTSTATIC_H__F8E9A061_B40E_11D5_A2FB_00D0B71C8709__INCLUDED_)
