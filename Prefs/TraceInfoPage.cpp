// TraceInfoPage.cpp : implementation file
//

#include "stdafx.h"
#include "simul.h"
#include "TraceInfoPage.h"
#include <Prefs\Pref.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPrefCategoryPage dialog


CPrefCategoryPage::CPrefCategoryPage()
	: CPreferencePage(CPrefCategoryPage::IDD)
{
	//{{AFX_DATA_INIT(CPrefCategoryPage)
	m_strFolder = CPref::g_sTraceRep;
	//}}AFX_DATA_INIT
	m_nSel = LB_ERR;
}


void CPrefCategoryPage::DoDataExchange(CDataExchange* pDX)
{
	CPreferencePage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPrefCategoryPage)
	DDX_Control(pDX, IDC_CATEGORY, m_cCatString);
	DDX_Control(pDX, IDC_CATLIST, m_cCatList);
	DDX_Text(pDX, IDC_FOLDER, m_strFolder);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPrefCategoryPage, CPreferencePage)
	//{{AFX_MSG_MAP(CPrefCategoryPage)
	ON_BN_CLICKED(IDC_BROWSE, OnBrowse)
	ON_BN_CLICKED(IDC_ADD, OnAddCat)
	ON_BN_CLICKED(IDC_DEL, OnDelCat)
	ON_LBN_SELCHANGE(IDC_CATLIST, OnSelchangeCat)
	ON_EN_CHANGE(IDC_CATEGORY, OnChangeCategory)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPrefCategoryPage message handlers
CString strTmpPath;

int CALLBACK BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
	TCHAR szDir[MAX_PATH];
	switch(uMsg){
	case BFFM_INITIALIZED:
		if (lpData){
			strcpy(szDir, strTmpPath.GetBuffer(strTmpPath.GetLength()));
			SendMessage(hwnd,BFFM_SETSELECTION,TRUE,(LPARAM)szDir);
		}
		break;
	case BFFM_SELCHANGED: {
	   if (SHGetPathFromIDList((LPITEMIDLIST) lParam ,szDir)){
		  SendMessage(hwnd,BFFM_SETSTATUSTEXT,0,(LPARAM)szDir);
	   }
	   break;
	}
	default:
	   break;
	}
         
	return 0;
}

BOOL GetFolder(CString* strSelectedFolder,
				   const char* lpszTitle,
				   const HWND hwndOwner, 
				   const char* strRootFolder, 
				   const char* strStartFolder)
{
	char pszDisplayName[MAX_PATH];
	LPITEMIDLIST lpID;
	BROWSEINFOA bi;
	
	bi.hwndOwner = hwndOwner;
	if (strRootFolder == NULL){
		bi.pidlRoot = NULL;
	}else{
	   LPITEMIDLIST  pIdl = NULL;
	   IShellFolder* pDesktopFolder;
	   char          szPath[MAX_PATH];
	   OLECHAR       olePath[MAX_PATH];
	   ULONG         chEaten;
	   ULONG         dwAttributes;

	   strcpy(szPath, (LPCTSTR)strRootFolder);
	   if (SUCCEEDED(SHGetDesktopFolder(&pDesktopFolder)))
	   {
		   MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, szPath, -1, olePath, MAX_PATH);
		   pDesktopFolder->ParseDisplayName(NULL, NULL, olePath, &chEaten, &pIdl, &dwAttributes);
		   pDesktopFolder->Release();
	   }
	   bi.pidlRoot = pIdl;
	}
	bi.pszDisplayName = pszDisplayName;
	bi.lpszTitle = lpszTitle;
	bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_STATUSTEXT;
	bi.lpfn = BrowseCallbackProc;
	if (strStartFolder == NULL){
		bi.lParam = FALSE;
	}else{
		strTmpPath.Format("%s", strStartFolder);
		bi.lParam = TRUE;
	}
	bi.iImage = NULL;
	lpID = SHBrowseForFolderA(&bi);
	if (lpID != NULL){
		BOOL b = SHGetPathFromIDList(lpID, pszDisplayName);
		if (b == TRUE){
			strSelectedFolder->Format("%s",pszDisplayName);
			return TRUE;
		}
	}else{
		strSelectedFolder->Empty();
	}
	return FALSE;
}

void CPrefCategoryPage::OnBrowse() 
{
	// TODO: Add your control notification handler code here
	CString strFolderPath;
	if (GetFolder(&strFolderPath, "Sample of  getting folder.", this->m_hWnd, NULL, CPref::g_sTraceRep)){
		if (!strFolderPath.IsEmpty()){
			m_strFolder = strFolderPath;
			UpdateData(FALSE);
		}
	}
}

void CPrefCategoryPage::OnAddCat() 
{
	// TODO: Add your control notification handler code here
	CString mstr;
	m_cCatString.GetWindowText(mstr);

	if (!mstr.IsEmpty() && (m_cCatList.FindStringExact	(-1,mstr)==LB_ERR))
	{
		m_cCatList.AddString(mstr);
	}
	mstr.Empty();
	m_cCatString.SetWindowText(mstr);
	
}

void CPrefCategoryPage::OnDelCat() 
{
	// TODO: Add your control notification handler code here
	if (m_nSel != LB_ERR)
	{
		m_cCatList.DeleteString(m_nSel);
		m_nSel = LB_ERR;
		m_cCatList.SetCurSel(m_nSel);
	}
}

void CPrefCategoryPage::OnSelchangeCat() 
{
	// TODO: Add your control notification handler code here
	m_nSel = m_cCatList.GetCurSel();
	if (m_nSel != LB_ERR)
	{
		CWnd *pWn = GetDlgItem(IDC_DEL);
		if (pWn) pWn->EnableWindow(m_nSel != LB_ERR);
	}
	
}

void CPrefCategoryPage::OnChangeCategory() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CPropertyPage::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	
}

BOOL CPrefCategoryPage::OnInitDialog() 
{
	CPreferencePage::OnInitDialog();
	
	// TODO: Add extra initialization here
	int nb = CPref::g_cCatList.GetSize();
	for (int i=0;i<nb;i++)
	{
		CString mstr = CPref::g_cCatList.GetAt(i);
		m_cCatList.AddString(mstr);
	}

	OnSelchangeCat();	

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
