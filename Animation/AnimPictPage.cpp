// AnimPictPage.cpp : implementation file
//

#include "stdafx.h"
#include "simul.h"
#include "AnimPictPage.h"
#include "Shlwapi.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


extern CSimulApp theApp;


BEGIN_MESSAGE_MAP(CAnimPictPage, CPropertyPage)
	//{{AFX_MSG_MAP(CAnimPictPage)
	ON_BN_CLICKED(IDC_BROWSE, OnBrowse)
	ON_BN_CLICKED(IDC_BACKGROUND, OnSetBackground)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAnimPictPage

CAnimPictPage::CAnimPictPage()
	: CAnimPage(CAnimPictPage::IDD)
{
	//{{AFX_DATA_INIT(CAnimPictPage)
	m_bTransBk = FALSE;
	m_sFileName = _T("< none >");
	//}}AFX_DATA_INIT
	m_bIsFileSelected = FALSE;
	m_clrTrans = RGB(255,255,255);

}

void CAnimPictPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAnimPictPage)
	DDX_Control(pDX, IDC_TRANSCOLOR, m_TransColor);
	DDX_Check(pDX, IDC_BACKGROUND, m_bTransBk);
	DDX_Text(pDX, IDC_BMPFILE, m_sFileName);
	//}}AFX_DATA_MAP

	if (!pDX->m_bSaveAndValidate)
	{
		m_TransColor.EnableWindow(m_bTransBk);
		m_TransColor.SetBkColour(m_clrTrans);
		//m_TransColor.SetWindowText(_T(""));

	}
	else
	{
		m_clrTrans = m_TransColor.GetBkColour();
	}

}


/////////////////////////////////////////////////////////////////////////////
// CAnimPictPage message handlers

void CAnimPictPage::OnBrowse() 
{
	CFileDialog dlg( TRUE,_T("BMP"),_T("*.BMP"),
					 OFN_FILEMUSTEXIST | OFN_HIDEREADONLY,
					 _T("Bitmap File (*.bmp)|*.BMP|"));
	//dlg.m_ofn.lpstrInitialDir = "C:\\nicolas\\temp\\s3\\";
	if( dlg.DoModal()==IDOK )
	{
		m_sFileName = dlg.GetPathName();
//		UpdateData(FALSE);
		//ApplyChanges();
		char full[_MAX_PATH];//, part[_MAX_PATH];

		CString llll = dlg.GetFileName();
		_fullpath( full, llll, _MAX_PATH );

		CString strApp = theApp.m_pszHelpFilePath;

		CWnd *pWnd = AfxGetMainWnd();
		CMDIFrameWnd *pMainFrm = DYNAMIC_DOWNCAST( CMDIFrameWnd, pWnd);
		if (pMainFrm)
		{
			CMDIChildWnd *pWWW = pMainFrm->MDIGetActive(); 
			CDocument *pDoc = pWWW->GetActiveDocument();
			if (pDoc) strApp = pDoc->GetPathName();
		}
		

		char szOut[MAX_PATH] = "";


		//strApp.MakeUpper();
		CString strApp2 = dlg.GetPathName();
		//strApp2.MakeUpper();

		LPTSTR szFrom = strApp.GetBuffer(strApp.GetLength());
		LPTSTR szTo = strApp2.GetBuffer(strApp2.GetLength());

		PathRelativePathTo(szOut,
                         szFrom,
                         FILE_ATTRIBUTE_NORMAL,
                         szTo,
                         FILE_ATTRIBUTE_NORMAL);


		m_sFileName = CString(szOut);
		UpdateData(FALSE);
		m_bIsFileSelected = TRUE;

		/*BOOL bSame = TRUE;
		int nIdx = 0;.

		while (bSame)
		{
			int nS1 = strApp.Find('\\',nIdx);
			int nS2 = strApp2.Find('\\',nIdx);

			if (nS1!=-1)
			{
				CString S1 = strApp.Left(nS1);
				CString S2 = strApp2.Left(nS1);

				int nres = S1.CompareNoCase(S2);
				bSame = (!nres);
				if (bSame) nIdx=nS1+1;
			}
		}
		if (nIdx!=-1)
		{
			CString S1 = strApp.Left(nIdx);
			CString S2 = strApp;
			S2.Delete(0,nIdx);
			CString S3 = strApp2;
			S3.Delete(0,nIdx);

			CString S4;

			int nS1 = 0;
			nIdx=0;
			
			while (nS1!=-1)
			{
				nS1 = S2.Find('\\');
				if (nS1!=-1)
				{
					nIdx++;
					S2.Delete(0,nS1+1);
					S4 += _T("..\\");
				}

			}

			m_sFileName = S4 + S3;
		}

		UpdateData(FALSE);


		char *token;
		token = strtok( strApp.GetBuffer(strApp.GetLength()), _T("\\") );

		while( token != NULL )
		{
			token = strtok( NULL, _T("\\")  );
		}

		int nb=-1;
		for (int i=0;i<min(strApp.GetLength(),strApp.GetLength()) && nb==-1;i++)
		{
			TCHAR c1 = strApp.GetAt(i);
			TCHAR c2 = strApp2.GetAt(i);
			if (c1!=c2)
			{
				nb=i;
				break;
			}
		}
		if (nb!=-1)
		{
			CString str1 = strApp.Left(nb);
		}

		int nnn = strApp.CompareNoCase(strApp2);

		m_bIsFileSelected = TRUE;*/
	}
	
}

void CAnimPictPage::OnSetBackground() 
{
	CButton *pWnd = (CButton*)GetDlgItem(IDC_BACKGROUND);
	if (pWnd)
	{
		m_TransColor.EnableWindow(pWnd->GetCheck());
		m_bIsFileSelected = TRUE;
	}

}
