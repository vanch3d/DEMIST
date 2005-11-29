// FormatPage.cpp : implementation file
//

#include "stdafx.h"
#include "simul.h"
#include "FormatPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFormatPage property page

IMPLEMENT_DYNCREATE(CFormatPage, CPropertyPage)

CFormatPage::CFormatPage() : CPropertyPage()
{
	//{{AFX_DATA_INIT(CFormatPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CFormatPage::CFormatPage(UINT nIDTemplate) : CPropertyPage(nIDTemplate)
{
}


CFormatPage::~CFormatPage()
{
}

void CFormatPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFormatPage)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFormatPage, CPropertyPage)
	//{{AFX_MSG_MAP(CFormatPage)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFormatPage message handlers
