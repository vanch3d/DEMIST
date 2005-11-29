// BPInfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "simul.h"
#include "BPInfoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBPInfoDlg dialog


CBPInfoDlg::CBPInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBPInfoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBPInfoDlg)
	m_strDesc = _T("");
	m_strTitle = _T("");
	//}}AFX_DATA_INIT
	m_nIcon = 0;
}


void CBPInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBPInfoDlg)
	DDX_Control(pDX, IDC_BP_DESC, m_wndDesc);
	DDX_Control(pDX, IDC_BP_TITLE, m_wndTitle);
	DDX_Control(pDX, IDC_BP_ICON, m_wndIcon);
	DDX_Text(pDX, IDC_BP_DESC, m_strDesc);
	DDX_Text(pDX, IDC_BP_TITLE, m_strTitle);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBPInfoDlg, CDialog)
	//{{AFX_MSG_MAP(CBPInfoDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBPInfoDlg message handlers

BOOL CBPInfoDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CHARFORMAT cfDefault;

	cfDefault.cbSize = sizeof(cfDefault);
	cfDefault.dwEffects = CFE_PROTECTED|CFE_BOLD; 
	cfDefault.dwMask = CFM_BOLD | CFM_FACE | CFM_SIZE | CFM_CHARSET | CFM_PROTECTED;
	cfDefault.yHeight = 240;
	cfDefault.bCharSet = 0xEE; 
	strcpy(cfDefault.szFaceName, _T("Arial")); 

	m_wndTitle.SetDefaultCharFormat(cfDefault);

	//m_wndTitle.SetWindowText("fgf ggh gh fgh fdhfd");
	//m_wndTitle.FormatRange(NULL,TRUE);
	m_wndTitle.SetBackgroundColor(FALSE,RGB(239,247,247));

	m_wndDesc.SetBackgroundColor(FALSE,::GetSysColor(COLOR_BTNFACE));

	if (m_nIcon)
	{
		m_wndBitmap.LoadBitmap(m_nIcon);
		m_wndIcon.SetBitmap((HBITMAP)m_wndBitmap);
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
