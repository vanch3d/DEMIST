// PreferencePage.cpp : implementation file
//

#include "stdafx.h"
#include "simul.h"
#include "PreferencePage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPreferencePage property page

IMPLEMENT_DYNCREATE(CPreferencePage, CPropertyPage)

CPreferencePage::CPreferencePage() : CPropertyPage()
{
	//{{AFX_DATA_INIT(CPreferencePage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CPreferencePage::CPreferencePage(UINT nIDTemplate) : CPropertyPage(nIDTemplate)
{
	//{{AFX_DATA_INIT(CPreferencePage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CPreferencePage::~CPreferencePage()
{
}

void CPreferencePage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPreferencePage)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPreferencePage, CPropertyPage)
	//{{AFX_MSG_MAP(CPreferencePage)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPreferencePage message handlers
