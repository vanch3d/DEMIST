// DlgFormat.cpp : implementation file
//

#include "stdafx.h"
#include "..\simul.h"
#include "DlgFormat.h"

#include "Format.h"
#include "..\resource.hm"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CERFormatPage dialog

CERFormatPage::CERFormatPage(CFormat *pFormat)
	: CFormatPage(CERFormatPage::IDD)
{
	m_pFormat = pFormat;
	//{{AFX_DATA_INIT(CERFormatPage)
	//}}AFX_DATA_INIT
}


void CERFormatPage::DoDataExchange(CDataExchange* pDX)
{
	CFormatPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CERFormatPage)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CERFormatPage, CFormatPage)
	//{{AFX_MSG_MAP(CERFormatPage)
	ON_WM_HELPINFO()
	//}}AFX_MSG_MAP
	ON_NOTIFY(OT_NOTIFY_ITEMCHANGED, IDC_OPTIONSTREE_OPTIONS, OnTreeItemChanged)
	//ON_NOTIFY(OT_NOTIFY_ITEMBUTTONCLICK, IDC_OPTIONSTREE_OPTIONS, OnTreeItemChanged)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CERFormatPage message handlers

void CERFormatPage::OnTreeItemChanged(NMHDR* pNotifyStruct, LRESULT* plResult)
{
    // Declare variables
	LPNMOPTIONTREE pNMOptionTree = (LPNMOPTIONTREE)pNotifyStruct;

    // Validate
	if (pNMOptionTree->otiItem != NULL)
    {
        // -- Use item
		int nb=0;
		nb++;
	}

    *plResult = 0;
}


/////////////////////////////////////////////////////////////////////////////
/// Initialise the content of the dialog immediately before being displayed.
/// \return TRUE if the dialog can be created, FALSE otherwise
///
/// This function, called by the framework just before displaying the dialog, create the
/// option-tree control and fills its content by callin the CFormat::UpdatePageContent() function.
/////////////////////////////////////////////////////////////////////////////
BOOL CERFormatPage::OnInitDialog() 
{
	CFormatPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	CRect rcClient;
	DWORD dwStyle, dwOptions;

	// Get the clients rectangle
	GetClientRect(rcClient);

	// Setup the window style
	dwStyle = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	// Setup the tree options 
	dwOptions = OT_OPTIONS_NOTIFY | OT_OPTIONS_SHADEEXPANDCOLUMN | OT_OPTIONS_SHADEROOTITEMS | OT_OPTIONS_SHOWINFOWINDOW | OT_OPTIONS_DEFINFOTEXTNOSEL;

	// Create tree options
	if (!m_pFormat || m_otTree.Create(dwStyle, rcClient, this, dwOptions, IDC_OPTIONSTREE_OPTIONS) == FALSE)
	{
		TRACE0("Failed to create options control.\r\n");
		return FALSE;
	}

	DWORD dwExStyle = m_otTree.GetExStyle();
	m_otTree.ModifyStyleEx(0,WS_EX_CONTEXTHELP);
	dwExStyle = m_otTree.GetExStyle();
	// Setup options tree
	m_pFormat->UpdatePageContent(&m_otTree);

	//m_otTree.ExpandAllItems();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


BOOL CERFormatPage::OnHelpInfo(HELPINFO* pHelpInfo) 
{

	static const DWORD aMenuHelpIDs[] =
	   {
      IDC_OPTIONSTREE_OPTIONS, HIDOK,
	   };

	// TODO: Add your message handler code here and/or call default
	if (pHelpInfo->iContextType == HELPINFO_WINDOW)   // must be for a control
    {
	//	::WinHelp ((HWND)pHelpInfo->hItemHandle,
      //            "GEN32.HLP",
        //          HELP_WM_HELP,
          //        (DWORD)(LPVOID)aMenuHelpIDs);


		AfxGetApp()->WinHelp(HIDC_OPTIONSTREE_OPTIONS, HELP_CONTEXTPOPUP);
		return TRUE;

    }
      
	return CFormatPage::OnHelpInfo(pHelpInfo);
}
