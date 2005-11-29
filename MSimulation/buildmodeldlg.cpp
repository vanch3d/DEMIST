// BuildModelDlg.cpp : implementation file
//

#include "stdafx.h"
#include "simul.h"
#include "BuildModelDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBuildModelDlg dialog


CBuildModelDlg::CBuildModelDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBuildModelDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBuildModelDlg)
	m_nCoeff = 0.0;
	m_cNumMethod = -1;
	//}}AFX_DATA_INIT
}


void CBuildModelDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBuildModelDlg)
	DDX_Text(pDX, IDD_MODEL_BUILDPROP_H, m_nCoeff);
	DDV_MinMaxDouble(pDX, m_nCoeff, 1.e-003, 1.);
	DDX_CBIndex(pDX, IDD_MODEL_BUILDPROP_NUM, m_cNumMethod);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBuildModelDlg, CDialog)
	//{{AFX_MSG_MAP(CBuildModelDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBuildModelDlg message handlers
