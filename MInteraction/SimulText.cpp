// SimulText.cpp : implementation file
//

#include "stdafx.h"
#include "Simul.h"

#include "LearnerTrace.h"

#include "simulDoc.h"
#include <MSimulation\Model.h>
#include "SimulText.h"
#include "Format.h"

#include <Prefs\Pref.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CViewText

IMPLEMENT_DYNCREATE(CViewText, CView)

CViewText::CViewText()
{
	m_pER = NULL;
//	m_nCurrTTask=0;
}

CViewText::~CViewText()
{
	CTrace::T_CLOSE(GetParentFrame());
}

/////////////////////////////////////////////////////////////////////////////
/// The framework calls this function after receiving a DOC_UPDATE_VIEWCONTENT notification.
///
/// Called just before the view is displayed for the first time, this function just
/// assignes to the data member m_pER a pointer to the Learning Unit (see CLearningUnit)
/// associated with that view.
/////////////////////////////////////////////////////////////////////////////
LRESULT CViewText::OnUpdateInitContent(WPARAM wp, LPARAM lp)
{
	m_pER = (CExternRepUnit*)lp;
	return 0;
}

BEGIN_MESSAGE_MAP(CViewText, CView)
	//{{AFX_MSG_MAP(CViewText)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_WM_DESTROY()
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	//}}AFX_MSG_MAP
	ON_MESSAGE(TRACE_VIEW_ACTIVATE, OnActivateER)
	ON_MESSAGE(DOC_UPDATE_VIEWCONTENT, OnUpdateInitContent)
	//ON_COMMAND_RANGE(ID_TRANS_INDEPENDENT,ID_TRANS_DYNALINKED,OnTranslationTasks)
	ON_UPDATE_COMMAND_UI_RANGE(ID_TRANS_INDEPENDENT,ID_TRANS_DYNALINKED,OnUpdateTransTasks)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewText drawing

void CViewText::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CViewText diagnostics

#ifdef _DEBUG
void CViewText::AssertValid() const
{
	CView::AssertValid();
}

void CViewText::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CSimulDoc* CViewText::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSimulDoc)));
	return (CSimulDoc*)m_pDocument;
}
#endif //_DEBUG

CFormatText* CViewText::GetFormats()
{
	CFormatText* pDlg = NULL;

	if (!m_pER) return pDlg;
	int nbF = m_pER->m_cFormatSet.GetSize();
	if (nbF)
	{
		CFormat *pPage = m_pER->m_cFormatSet.GetAt(0);
		pDlg = DYNAMIC_DOWNCAST(CFormatText,pPage);
	}

	return pDlg;
}

/////////////////////////////////////////////////////////////////////////////
// CViewText message handlers
LRESULT CViewText::OnActivateER(WPARAM wp, LPARAM lp)
{
	static BOOL bFirst = TRUE;

	if (!bFirst) CTrace::T_SWITCH(this, (CWnd*)wp);
	bFirst = FALSE;
	return 0;
}

void CViewText::OnInitialUpdate() 
{
	if (m_pER)
		GetParentFrame()->SetWindowText(m_pER->GetName());
	CTrace::T_OPEN(GetParentFrame());
// TWSCRIPTEDIT	->
	//ModifyStyle(0, WS_CLIPCHILDREN);

	//m_wndScriptEdit.SubclassDlgItem(IDC_RICHEDIT1, this);
	m_wndScriptEdit.Initialize();

	m_wndScriptEdit.SetCaseSensitive(FALSE);
	m_wndScriptEdit.SetStringQuotes(_T("\""));
	m_wndScriptEdit.SetSLComment(_T(';'));
	//m_wndScriptEdit.SetSLComment(_T("rem"));

	COLORREF	m_clrVar = RGB(0,0,255),
				m_clrParam = RGB(128,0,128),
				m_clrCst = RGB(0,128,0);

	PARAFORMAT pf;
	pf.cbSize = sizeof(PARAFORMAT);
	pf.dwMask = PFM_TABSTOPS ;
	pf.cTabCount = 2;
	pf.rgxTabs[0] = 5 * 1440/5 ;
	pf.rgxTabs[1] = 15 * 1440/5 ;

	m_wndScriptEdit.SetParaFormat( pf );

	CFormatText *pForm = NULL;
	int nLevel = 0;
	
	if (pForm = GetFormats())
	{
		nLevel = pForm->m_nDisplayMode;
		m_clrVar = pForm->m_clrVar;
		m_clrParam = pForm->m_clrParam;
		m_clrCst = pForm->m_clrCst;
	}

	m_wndScriptEdit.SetConstantColor(m_clrVar, true);
	m_wndScriptEdit.SetKeywordColor(m_clrParam, true);
	m_wndScriptEdit.SetNumberColor(m_clrCst, true);

	CMdEquation *pEqu = GetDocument()->GetCurrentModel();
	if (!pEqu) return;

	CLearningUnit *pLU = GetDocument()->GetCurrentLU();
	if (!pLU) return;


	CString sKeyword(_T(" "));
	for (int i=0;i<pEqu->m_cParSet.GetSize();i++)
	{
		CModelPar *pPar = pEqu->m_cParSet.GetAt(i);
		//if (pPar->GetInitValue() == -1)
		//{
			sKeyword += pPar->GetFormatName(pLU->m_bShowAbbrev);
			sKeyword += _T(" ");
		//}
	}
	m_wndScriptEdit.AddKeywords(sKeyword);

	CString sConstant(_T(" "));
	for (i=0;i<pEqu->m_cVarSet.GetSize();i++)
	{
		CModelVar *pVar = pEqu->m_cVarSet.GetAt(i);
		if (!pVar->GetIcon())
		//if (pVar->GetInitValue() != -1)
		{
			sConstant += pVar->GetFormatName(pLU->m_bShowAbbrev);
			sConstant += _T(" ");
		}
	}
	m_wndScriptEdit.AddConstants(sConstant);
	CView::OnInitialUpdate();

//	m_wndScriptEdit.SetWindowText(pEqu->GetFormatedText());

//	m_wndScriptEdit.FormatAll();
//	m_wndScriptEdit.SetOptions(ECOOP_OR,ECO_READONLY );
// <- TWSCRIPTEDIT	
	CWnd *pWnd = GetParent();
	CRect mrect;
	if (pWnd)
	{
		pWnd->GetWindowRect(mrect);
		pWnd->SetWindowPos(NULL,0,0,mrect.Width()-1,mrect.Height(),SWP_NOZORDER|SWP_NOMOVE);
	}
}

/////////////////////////////////////////////////////////////////////////////
/// The framework calls this function after receiving a notification message.
///
/// \param pSender Points to the view that modified the document, or NULL if all views are to be updated.
/// \param lHint Contains information about the modifications.
/// \param pHint Points to an object storing information about the modifications.
///
/// This member function proceeds and/or dispatches all the messages sent by the framework
/// to the relevant function.
/////////////////////////////////////////////////////////////////////////////
void CViewText::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	if (!m_pER) return;
	CLearningUnit *pLU = GetDocument()->GetCurrentLU();
	if (!pLU) return;

	int nbOutput = m_pER->m_cOutputSet.GetSize();

	CFormatText *pForm = NULL;
	int nLevel = 0;
	
	if (pForm = GetFormats())
	{
		nLevel = pForm->m_nDisplayMode;
	}

	
	if (!lHint || lHint == DOC_UPDATE_ALLDATA || 
		(lHint == DOC_UPDATE_ER && (pHint==this)))
	{
		CString mstr("");
		CMdEquation *pEqu = GetDocument()->GetCurrentModel();
		if (!pEqu) return;

		pEqu->GetFormatedText(nLevel);

		if (nLevel)
		{
			mstr = _T("\r\n");
			mstr +=  pEqu->GetFormatedEquation();
		}

		for (int hh = 0;hh < nbOutput ;hh++)
		{
			COutcomes *pCo= m_pER->m_cOutputSet.GetAt(hh);
			if (pCo->m_nExpSet) continue;
		
			CMdData *pData = pLU->GetDataAt(pCo->m_nData);
			if (!pData) continue;

			CModelObj *pObj = pData->GetDataInfo();
			if (!pObj) continue;

			CString strName = pObj->GetFormatName(pLU->m_bShowAbbrev);
			CString strDef = pObj->GetDef();
			CString strDes = pObj->GetDesc();

			//CModelPar *pPar = DYNAMIC_DOWNCAST(CModelPar,pObj);
			//CModelVar *pVar = DYNAMIC_DOWNCAST(CModelVar,pObj);

			int nb = pObj->GetIcon();
			CString strItem;
			if (!nLevel /*&& (nb !=3)*/)
			{
				strItem.Format(_T(" %s \t\t; %s \r\n"),strName,strDes);
			}
			else
			{
				strItem.Format(_T(" %s \t<=>  %s \t; %s \r\n"),strName,strDef,strDes);
			}
			mstr += strItem;
		}

		m_wndScriptEdit.SetWindowText(mstr);
		m_wndScriptEdit.FormatAll();

	}
	else if (lHint == DOC_UPDATE_TIMERDATA)
	{

	}
	else
	{
		//CString cd;
		//m_wndScriptEdit.GetWindowText(cd);
	}

	int nCT = GetDocument()->GetCurrTime();
	int nRT = GetDocument()->GetRunTime();
	COLORREF oldClr;
	if (nCT==nRT)
		oldClr=m_wndScriptEdit.SetBackgroundColor(FALSE,RGB(255,255,255));
	else
		oldClr=m_wndScriptEdit.SetBackgroundColor(FALSE,CPref::g_crPast);
	//Invalidate();
	//UpdateWindow();
}

BOOL CViewText::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CView::PreCreateWindow(cs);
}

int CViewText::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// ****** Add your code below this line ********** //
	if (!m_cToolBar.CreateEx(GetParent(), TBSTYLE_FLAT | TBSTYLE_TOOLTIPS, 
			WS_CHILD | WS_VISIBLE | CBRS_TOP | 
			/*CBRS_GRIPPER | */CBRS_TOOLTIPS | CBRS_FLYBY | CCS_ADJUSTABLE,
			CRect(0, 0, 0, 0), 120) ||
		!m_cToolBar.LoadToolBar(IDR_SIMULGRAPH))
	{
		TRACE0("Failed to create toolbar1\n");
		return -1;      // fail to create  
	}

	BOOL bResult = m_wndScriptEdit.Create(
		WS_VISIBLE | WS_HSCROLL   | WS_VSCROLL  | //WS_CHILD | WS_CLIPCHILDREN |
		ES_AUTOHSCROLL | ES_AUTOVSCROLL | 
		ES_MULTILINE | ES_WANTRETURN | ES_READONLY,
		CRect(0,0,1000,1000), this,200
		);

	//m_wndScriptEdit.EnableWindow(false);
			
	// ****** Add your code above this line ********** //
	return (bResult ? 0 : -1);
}

void CViewText::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if (::IsWindow(m_wndScriptEdit.m_hWnd))
		m_wndScriptEdit.MoveWindow(3,3,cx-3,cy-3);

	if (m_cToolBar && ::IsWindow(m_cToolBar.m_hWnd))
	{
		UINT tt = ID_TASK_CANCELTASK;
		UINT nID,nStyle;
		int nImage;
		m_cToolBar.GetButtonInfo(2,nID,nStyle,nImage);
		m_cToolBar.SetButtonInfo(2,nID,TBBS_SEPARATOR,cx-100);

	}
}

void CViewText::OnEditCopy() 
{
	// TODO: Add your command handler code here
	m_wndScriptEdit.Copy();	
}

void CViewText::OnDestroy() 
{
	CTrace::T_CLOSE(GetParentFrame());
	CView::OnDestroy();
	
	// TODO: Add your message handler code here
	
}
void CViewText::OnTranslationTasks(UINT nID)
{
	// TODO: Add your command handler code here
	switch (nID) {
	case ID_TASK_MAPFACT:
	case ID_TASK_MAPRELATION:
	case ID_TASK_SUPPORTACTION:
//		m_nCurrTTask = nID - ID_TASK_MAPFACT+1;
		break;
	case ID_TASK_CANCELTASK:
	default:
//		m_nCurrTTask = 0;
		break;
	}

}

void CViewText::OnUpdateTransTasks(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	BOOL	bEnab = FALSE;
	BOOL	bCheck = FALSE;
	//UINT	nT = GetTranslation();
	//UINT	nCT = m_nCurrTTask;

	switch (pCmdUI->m_nID) {
	case ID_TRANS_INDEPENDENT:
	case ID_TRANS_MAPRELATION:
	case ID_TRANS_DYNALINKED:
		bCheck = (0 == (pCmdUI->m_nID - ID_TRANS_INDEPENDENT));
		bEnab = bCheck;
		break;
	default:
		break;
	}
	pCmdUI->Enable(bEnab);
	pCmdUI->SetCheck(bCheck);

}

void CViewText::OnEditPaste() 
{
	// TODO: Add your command handler code here
	m_wndScriptEdit.Paste();
}
