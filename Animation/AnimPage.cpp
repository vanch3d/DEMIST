// AnimPage.cpp : implementation file
//

#include "stdafx.h"
#include "simul.h"
#include "AnimPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAnimPage property page

IMPLEMENT_DYNCREATE(CAnimPage, CPropertyPage)

CAnimPage::CAnimPage() : CPropertyPage()
{
	//{{AFX_DATA_INIT(CAnimPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CAnimPage::CAnimPage(UINT nIDTemplate) : CPropertyPage(nIDTemplate)
{
}

CAnimPage::~CAnimPage()
{
}

void CAnimPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAnimPage)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAnimPage, CPropertyPage)
	//{{AFX_MSG_MAP(CAnimPage)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAnimPage message handlers
