// ViewWelcome.cpp : implementation file
//

#include "stdafx.h"
#include "simul.h"

#include "simulDoc.h"
#include "ViewWelcome.h"

#include <MInstruction\LearningUnit.h>
#include <io.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CViewWelcome

CWelcomeDialogBar::CWelcomeDialogBar() : CDialogBar()
{
	m_ImageList.Create(IDB_CONTROLLER,16,0,RGB(192,192,192));

	//{{AFX_DATA_INIT(CWelcomeDialogBar)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

BOOL CWelcomeDialogBar::Create(CWnd* pParentWnd, UINT nStyle, UINT nID)
{
	BOOL bRes =  CDialogBar::Create(pParentWnd, IDD,nStyle, nID);
	return bRes;
}

void CWelcomeDialogBar::DoDataExchange(CDataExchange* pDX)
{
	CDialogBar::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWelcomeDialogBar)
	DDX_Control(pDX, ID_WELCOME_START, m_cStart);
	DDX_Control(pDX, ID_WELCOME_NEXT, m_cNext);
	DDX_Control(pDX, ID_WELCOME_CURRENT, m_cCurrent);
	//DDX_Control(pDX, IDC_DESCRIPTION, m_cNumber);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWelcomeDialogBar, CDialogBar)
	//{{AFX_MSG_MAP(CWelcomeDialogBar)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	ON_MESSAGE(WM_INITDIALOG, HandleInitDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

LRESULT CWelcomeDialogBar::HandleInitDialog(WPARAM, LPARAM)
{
	Default();  // allow default to initialize first (common dialogs/etc)

	CDataExchange dx(this, FALSE);

	DoDataExchange(&dx);

	m_cStart.SetIcon(m_ImageList.ExtractIcon(1));
	m_cNext.SetIcon(m_ImageList.ExtractIcon(2));
	m_cCurrent.SetIcon(m_ImageList.ExtractIcon(1));

	//m_cStart.SetTooltipText(ID_CONTR_TASK);
	//m_cNext.SetTooltipText(ID_CONTR_TASK);

	m_cStart.SetShowText(TRUE);
	m_cNext.SetShowText(TRUE);
	m_cCurrent.SetShowText(TRUE);

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CViewWelcome

IMPLEMENT_DYNCREATE(CViewWelcome, CHtmlView)

CViewWelcome::CViewWelcome()
{
	//{{AFX_DATA_INIT(CViewWelcome)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CViewWelcome::~CViewWelcome()
{
}

BOOL CViewWelcome::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	
	return CHtmlView::PreCreateWindow(cs);
}

int CViewWelcome::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CHtmlView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// ****** Add your code below this line ********** //

	if (!m_cToolBar.Create(GetParent(),CBRS_TOP, 120) )
	{
		TRACE0("Failed to create ToolBar\n");
		return -1;      // fail to create  
	}
	//m_cToolBar.SetBarStyle(CBRS_ALIGN_LEFT | CBRS_TOOLTIPS | CBRS_FLYBY);
	//m_cToolBar.EnableDocking(CBRS_ALIGN_ANY);
	//GetParentFrame()->RecalcLayout();
	// ****** Add your code above this line ********** //
	return 0;
}



void CViewWelcome::DoDataExchange(CDataExchange* pDX)
{
	CHtmlView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CViewWelcome)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CViewWelcome, CHtmlView)
	//{{AFX_MSG_MAP(CViewWelcome)
	ON_WM_CREATE()
	ON_WM_CONTEXTMENU()
	//}}AFX_MSG_MAP
	// Standard printing commands
	//ON_COMMAND(ID_WELCOME_NEXT, OnNextLU)
	//ON_COMMAND(ID_WELCOME_CURRENT, OnStartLU)
	//ON_UPDATE_COMMAND_UI(ID_WELCOME_NEXT,OnUpdateTools)
	//ON_UPDATE_COMMAND_UI(ID_WELCOME_CURRENT,OnUpdateTools)
	ON_COMMAND(ID_WELCOME_START, OnStartSession)
	ON_UPDATE_COMMAND_UI(ID_WELCOME_START,OnUpdateTools)
	ON_COMMAND(ID_FILE_PRINT, CHtmlView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CHtmlView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CHtmlView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewWelcome diagnostics

#ifdef _DEBUG
void CViewWelcome::AssertValid() const
{
	CHtmlView::AssertValid();
}

void CViewWelcome::Dump(CDumpContext& dc) const
{
	CHtmlView::Dump(dc);
}

CSimulDoc* CViewWelcome::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSimulDoc)));
	return (CSimulDoc*)m_pDocument;
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CViewWelcome printing


void CViewWelcome::OnFilePrintPreview() 
{
	//BCGPrintPreview (this);
}

void CViewWelcome::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CHtmlView::OnBeginPrinting(pDC, pInfo);
}

void CViewWelcome::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CHtmlView::OnEndPrinting(pDC, pInfo);
}

BOOL CViewWelcome::OnPreparePrinting(CPrintInfo* pInfo) 
{
	// TODO: call DoPreparePrinting to invoke the Print dialog box
	
	return DoPreparePrinting(pInfo);
	//return CHtmlView::OnPreparePrinting(pInfo);
}

/////////////////////////////////////////////////////////////////////////////
// CViewWelcome message handlers

/////////////////////////////////////////////////////////////////////////////
/// Open the given HTML document in the Assignment View..
/// \param lpStr	The path of the file to open, or NULL if none.
/// \return	TRUE if the document is open, FALSE otherwise
///
/// This function is called when the Assignment's content needs to be updated, i.e. when starting a 
/// new Learning Unit (see CLearningUnit::m_sLUHTML) or when a breakpoint is reached 
/// (see  CTimerBreakPoint::m_sHTML). The HTML tag stored in both variables is passed to this function.
/// If the argument is NULL (i.e. when no tag was associated with thw LU or thebreakpoint, or when the
/// instructional scenario is started), then this function tries to open the file associated with
/// the DEMIST document, looking first for an HTML file with the same name (i.e. <document>.html) and,
/// if failed, a file called demist.html. Both of them are assumed to be in the same directory as 
/// the DEMISt document.
/////////////////////////////////////////////////////////////////////////////
BOOL CViewWelcome::OpenHTMLDoc(LPTSTR lpStr)
{
	CString strPath;

	TCHAR lpszFilePath [_MAX_PATH];
	CString mstr = GetDocument()->GetPathName();

	TCHAR path_buffer[_MAX_PATH];
	TCHAR drive[_MAX_DRIVE];   
	TCHAR dir[_MAX_DIR];
	TCHAR name[_MAX_FNAME];

	_tsplitpath (mstr, drive, dir, name, NULL);

	if (lpStr)
	{
		if (lpStr[0]=='#')
		{
			TCHAR mstr2[_MAX_PATH];

			_tmakepath (mstr2, drive, dir, name,  _T("html"));

			strcpy(path_buffer,mstr2);
			strcat(path_buffer,lpStr);
			//_tmakepath (path_buffer, NULL,NULL,mstr, lpStr);
		}
		else
			_tmakepath (path_buffer, drive, dir, lpStr, NULL);
	}
	else
	{	/// Try to open the file <file>.html
		_tmakepath (path_buffer, drive, dir, name,  _T("html"));
		if (_access(path_buffer,00)==-1)
		{
			/// If <file>.html does not exist, then open demist.html
			TCHAR name2[_MAX_FNAME];
			strcpy(name2,_T("demist"));
			_tmakepath (path_buffer, drive, dir, name2,  _T("html"));
		}
	}
	strPath = path_buffer;
	Navigate2(strPath, NULL, NULL);
	return TRUE;
}



void CViewWelcome::OnInitialUpdate() 
{
	//TODO: This code navigates to a popular spot on the web.
	//Change the code to go where you'd like.
	SetOffline(TRUE);
	SetRegisterAsDropTarget(FALSE);

	CLearningUnit *pLU = GetDocument()->GetCurrentLU();
	if (pLU)
	{
		CString mstr = pLU->m_sLUHTML;
		if (!mstr.IsEmpty())
		{
			OpenHTMLDoc(mstr.GetBuffer(mstr.GetLength()));
			return;
		}
	}

	OpenHTMLDoc();
}


void CViewWelcome::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	// TODO: Add your specialized code here and/or call the base class
	//Invalidate();
	//UpdateWindow();
//	Refresh( );
	if (lHint == DOC_UPDATE_BREAKPOINT )
	{
		CTimerBreakPoint *mTInfo = (CTimerBreakPoint *)pHint;
		if (!mTInfo) return;
		int nbBP = mTInfo->m_tBreakPt;

		CTimerBreakPoint pBP;
		BOOL bEditable = GetDocument()->GetCurrentLU()->GetTimerBPAt(nbBP,pBP);
		if (bEditable)
		{
			CString mstr = pBP.m_sHTML;
			if (!mstr.IsEmpty())
				OpenHTMLDoc(mstr.GetBuffer(mstr.GetLength()));
		}

	}
	if (lHint == DOC_UPDATE_RESTARTSIMUL )
	{
		int i=0;
		CLearningUnit *pLU = GetDocument()->GetCurrentLU();
		if (!pLU) return;
		CString mstr = pLU->m_sLUHTML;
		//mstr = _T("simul1.html#UNIT1");
		if (!mstr.IsEmpty())
			OpenHTMLDoc(mstr.GetBuffer(mstr.GetLength()));
	}
}

void CViewWelcome::OnDownloadComplete()
{
	CHtmlView::OnDownloadComplete();
}

void CViewWelcome::OnUpdateTools(CCmdUI* pCmdUI)
{
	BOOL bEnab = FALSE;
	BOOL bCheck = FALSE;

	int nMT = GetDocument()->GetMaxTime();
	int nCT = GetDocument()->GetCurrTime();

	CLearningUnit *pLU = GetDocument()->GetCurrentLU();

	switch (pCmdUI->m_nID){
	case ID_WELCOME_START:
		bEnab = (!pLU);
		break;
	case ID_WELCOME_CURRENT:
		//bEnab = (pLU && !pLU->m_bCanStart);
		//break;
	case ID_WELCOME_NEXT:
		//bEnab = (pLU && nMT==nCT);
		//break;
	default:
		return;
	}
	pCmdUI->Enable(bEnab);
	pCmdUI->SetCheck(bCheck);

}

/////////////////////////////////////////////////////////////////////////////
/// Called by the framework when the user clicks on the "start Scenario" button.
///
/// This function is the entry point for the exploration of an instructional scenario.
/// It initialises and starts the first Learning Unit in the scenario.
/////////////////////////////////////////////////////////////////////////////
void CViewWelcome::OnStartSession()
{
	//GetDocument()->m_CurrInst = 0;
	GetDocument()->OnActivateInitLU();
	GetDocument()->OnActivateCurrentLU(FALSE);
	CFrameWnd *pWnd = GetParentFrame();
	if (pWnd)
		pWnd->DestroyWindow();
}

void CViewWelcome::OnStartLU()
{
	//GetDocument()->m_CurrInst = 0;
	//GetDocument()->OnActivateCurrentLU(FALSE);
}

void CViewWelcome::OnNextLU()
{
	//GetDocument()->OnActivateNextLU();
}


void CViewWelcome::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	// TODO: Add your message handler code here
}
