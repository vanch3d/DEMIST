// IdentityDlg.cpp : implementation file
//

#include "stdafx.h"
#include "simul.h"
#include "IdentityDlg.h"

#include <Prefs\Pref.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CComboBoxEx

CCompletionCBox::CCompletionCBox()
{
	m_bAutoComplete = TRUE;
}

CCompletionCBox::~CCompletionCBox()
{
}


BEGIN_MESSAGE_MAP(CCompletionCBox, CComboBox)
	//{{AFX_MSG_MAP(CCompletionCBox)
	ON_CONTROL_REFLECT(CBN_EDITUPDATE, OnEditUpdate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CComboBoxEx message handlers

BOOL CCompletionCBox::PreTranslateMessage(MSG* pMsg)
{
	// Need to check for backspace/delete. These will modify the text in
	// the edit box, causing the auto complete to just add back the text
	// the user has just tried to delete. 

	if (pMsg->message == WM_KEYDOWN)
	{
		m_bAutoComplete = TRUE;

		int nVirtKey = (int) pMsg->wParam;
		if (nVirtKey == VK_DELETE || nVirtKey == VK_BACK)
			m_bAutoComplete = FALSE;
	}

	return CComboBox::PreTranslateMessage(pMsg);
}

void CCompletionCBox::OnEditUpdate() 
{
  // if we are not to auto update the text, get outta here
  if (!m_bAutoComplete) 
      return;

  // Get the text in the edit box
  CString str;
  GetWindowText(str);
  int nLength = str.GetLength();
  
  // Currently selected range
  DWORD dwCurSel = GetEditSel();
  WORD dStart = LOWORD(dwCurSel);
  WORD dEnd   = HIWORD(dwCurSel);

  // Search for, and select in, and string in the combo box that is prefixed
  // by the text in the edit box
  if (SelectString(-1, str) == CB_ERR)
  {
      SetWindowText(str);		// No text selected, so restore what was there before
      if (dwCurSel != CB_ERR)
        SetEditSel(dStart, dEnd);	//restore cursor postion
  }

  // Set the text selection as the additional text that we have added
  if (dEnd < nLength && dwCurSel != CB_ERR)
      SetEditSel(dStart, dEnd);
  else
      SetEditSel(nLength, -1);
}

/////////////////////////////////////////////////////////////////////////////
// CIdentityDlg dialog


CIdentityDlg::CIdentityDlg(CWnd* pParent /*=NULL*/,BOOL bAuthMode /*= FALSE*/)
	: CDialog(CIdentityDlg::IDD, pParent)
{
	m_bMode = bAuthMode;
	m_bForceId= FALSE;

	//{{AFX_DATA_INIT(CIdentityDlg)
	m_sName = _T("");//mstr;
	m_sPwd = _T("");
	m_nSelMode = bAuthMode;
	//}}AFX_DATA_INIT
}


void CIdentityDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(CIdentityDlg)
	DDX_Control(pDX, IDC_COMBO_NAME, m_cName);
	DDX_Control(pDX, IDC_EDIT_PWD, m_cPwd);
	DDX_CBString(pDX, IDC_COMBO_NAME, m_sName);
	DDV_MaxChars(pDX, m_sName, 25);
	DDX_Text(pDX, IDC_EDIT_PWD, m_sPwd);
	DDV_MaxChars(pDX, m_sPwd, 7);
	DDX_Radio(pDX, IDC_MODE, m_nSelMode);
	//}}AFX_DATA_MAP

	if (!pDX->m_bSaveAndValidate)
	{
		if (m_nSelMode)
			OnLearningMode();
		else
			OnAuthoringMode();
	}
	else
	{
		int nbS = m_sName.GetLength();

		BOOL test = m_nSelMode || (!m_nSelMode && (m_sPwd == _T("")));
		if (!test)
		{
			CString mstr;
			mstr = _T("Wrong password : try again !");
			AfxMessageBox(mstr, MB_ICONEXCLAMATION, 0);
			pDX->Fail();

		}
	}
}


BEGIN_MESSAGE_MAP(CIdentityDlg, CDialog)
	//{{AFX_MSG_MAP(CIdentityDlg)
	ON_CBN_CLOSEUP(IDC_COMBO_NAME, OnCloseupComboName)
	ON_CBN_EDITCHANGE(IDC_COMBO_NAME, OnEditchangeComboName)
	ON_BN_CLICKED(IDC_TRICK, OnTrick)
	ON_BN_CLICKED(IDC_MODE, OnAuthoringMode)
	ON_BN_CLICKED(IDC_MODE2, OnLearningMode)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIdentityDlg message handlers
 
BOOL CIdentityDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

//	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CIdentityDlg::OnCloseupComboName() 
{
	// TODO: Add your control notification handler code here
	int m = m_cName.GetCurSel();
	CString mstr;
	m_cName.GetWindowText(mstr);
	CWnd *pWnd = GetDlgItem(IDOK);
	if (pWnd)
		pWnd->EnableWindow(m != -1);
//	m_bAuthMode = (m == 0);
//	m_cPwd.EnableWindow(m_bAuthMode);	
}

void CIdentityDlg::OnEditchangeComboName() 
{
	// TODO: Add your control notification handler code here
	int i= m_cName.GetWindowTextLength();
	CWnd *pWnd = GetDlgItem(IDOK);
	if (pWnd)
		pWnd->EnableWindow(i >= 3);
//	m_bAuthMode = FALSE;
//	m_cPwd.EnableWindow(m_bAuthMode);	
}

void CIdentityDlg::OnTrick() 
{
	// TODO: Add your control notification handler code here
	CString mstr(_T("NVL"));
	m_cPwd.SetWindowText(mstr);
	
}

void CIdentityDlg::OnOK()
{
	CDialog::OnOK();

	if (m_nSelMode)
	{
		CString mstr,mstr2;
		m_cName.GetWindowText(mstr);
		mstr2 = m_sName;
		if (mstr != _T(""))
		{
			CPref::g_sLastUser = mstr;
			CPref::AddUserInList(mstr);
		}
	}
}

void CIdentityDlg::OnAuthoringMode() 
{
	// switch to Authoring mode
	m_cName.ResetContent();

	CString mstr,mstr2;
	mstr2.LoadString(ID_INDICATOR_MODE);
	mstr = _T("< ") + mstr2 + _T(" >");
	m_cName.AddString(mstr);
	CWnd *pWnd = GetDlgItem(IDOK);
	if (pWnd) pWnd->EnableWindow(TRUE);


	m_cPwd.EnableWindow(TRUE);
	m_cName.SetCurSel(0);
	m_cName.EnableWindow(FALSE);
	
	pWnd = GetDlgItem(IDCANCEL);
	if (pWnd) pWnd->EnableWindow(!m_bForceId);
	pWnd = GetDlgItem(IDC_EDIT_PWD);
	if (pWnd) pWnd->SetFocus();
}

void CIdentityDlg::OnLearningMode() 
{
	// switch to Learning mode
	m_cName.ResetContent();
	for (int i=0;i<CPref::g_cUserList.GetSize();i++)
		m_cName.AddString(CPref::g_cUserList.GetAt(i));
	
	m_cPwd.EnableWindow(FALSE);
	m_cName.EnableWindow(TRUE);
	
	CWnd *pWnd = GetDlgItem(IDCANCEL);
	if (pWnd) pWnd->EnableWindow(!m_bForceId);
	pWnd = GetDlgItem(IDOK);
	if (pWnd) pWnd->EnableWindow(FALSE);
	pWnd = GetDlgItem(IDC_COMBO_NAME);
	if (pWnd) pWnd->SetFocus();
	m_cPwd.SetWindowText(_T(""));
}
