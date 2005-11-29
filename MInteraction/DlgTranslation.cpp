// DlgTranslation.cpp : implementation file
//

#include "stdafx.h"
#include "simul.h"
#include "DlgTranslation.h"

#include "simulDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CERTranslationPage dialog

IMPLEMENT_SERIAL( CERTranslationPage, CFormatPage, VERSIONABLE_SCHEMA | 1)

CERTranslationPage::CERTranslationPage()
	: CFormatPage(CERTranslationPage::IDD)
{
	//{{AFX_DATA_INIT(CERTranslationPage)
	m_nTranslation = TRANS_INDEP;
	//}}AFX_DATA_INIT
}


void CERTranslationPage::DoDataExchange(CDataExchange* pDX)
{
	CFormatPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CERTranslationPage)
	DDX_Radio(pDX, IDC_TRANSLATION, m_nTranslation);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CERTranslationPage, CFormatPage)
	//{{AFX_MSG_MAP(CERTranslationPage)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CERTranslationPage message handlers

void CERTranslationPage::Serialize(CArchive& ar) 
{
	if (ar.IsStoring())
	{	// storing code
		ar << m_nTranslation;
	}
	else
	{	// loading code
		ar >> m_nTranslation;
	}
}


/////////////////////////////////////////////////////////////////////////////
// CDlgFormatText dialog
IMPLEMENT_SERIAL( CDlgFormatText, CFormatPage, VERSIONABLE_SCHEMA | 1)


CDlgFormatText::CDlgFormatText()
	: CFormatPage(CDlgFormatText::IDD)
{
	//{{AFX_DATA_INIT(CDlgFormatText)
	m_nDisplayMode = -1;
	//}}AFX_DATA_INIT
}


void CDlgFormatText::DoDataExchange(CDataExchange* pDX)
{
	CFormatPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgFormatText)
	DDX_Control(pDX, IDC_VARCOLOR, m_cVar);
	DDX_Control(pDX, IDC_PARAMCOLOR, m_cParam);
	DDX_Control(pDX, IDC_CSTCOLOR, m_cConst);
	DDX_Control(pDX, IDC_SAMPLE, m_cSample);
	DDX_Radio(pDX, IDC_LEVEL, m_nDisplayMode);
	//}}AFX_DATA_MAP
	if (!pDX->m_bSaveAndValidate)
	{
		m_cVar.SetBkColour(m_clrVar);
		m_cParam.SetBkColour(m_clrParam);
		m_cConst.SetBkColour(m_clrCst);
	}
	else
	{
		m_clrVar = m_cVar.GetBkColour();
		m_clrParam = m_cParam.GetBkColour();
		m_clrCst = m_cConst.GetBkColour();
	}
}


BEGIN_MESSAGE_MAP(CDlgFormatText, CFormatPage)
	//{{AFX_MSG_MAP(CDlgFormatText)
	ON_BN_CLICKED(IDC_SETFONT, OnSetFont)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgFormatText message handlers

void CDlgFormatText::Serialize(CArchive& ar) 
{
	if (ar.IsStoring())
	{	// storing code
	}
	else
	{	// loading code
	}
}

void CDlgFormatText::OnSetFont() 
{
	// TODO: Add your control notification handler code here
	
}
