// ChildFrm.cpp : implementation of the CChildFrame class
//

#include "stdafx.h"
#include "Simul.h"

#include "MainFrm.h"

//#include "LearnerTrace.h"


#include "ChildFrm.h"
#include "SimulDoc.h"

#include <MSimulation\SimulView.h>
#include <MSimulation\ViewListModel.h>
#include <MSimulation\ViewAggreg.h>
#include <MSimulation\ViewModelDiag.h>
#include <MSimulation\ViewBuildModel.h>
#include <MInteraction\ViewController.h>
#include <MInteraction\SimulAnimat.h>
#include <MInteraction\SimulGraph.h>
#include <MInteraction\SimulTable.h>
#include <MInteraction\SimulPie.h>

#include "Tools\ST_SplitterWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWnd)
	//{{AFX_MSG_MAP(CChildFrame)
	ON_WM_GETMINMAXINFO()
	ON_WM_SIZE()
	ON_WM_MDIACTIVATE()
	ON_WM_CLOSE()
	ON_WM_CREATE()
	ON_WM_MOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChildFrame construction/destruction

CChildFrame::CChildFrame()
{
	// TODO: add member initialization code here
}

CChildFrame::~CChildFrame()
{
}

BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	cs.style &= ~FWS_ADDTOTITLE;
	cs.style |= FWS_PREFIXTITLE;

	if( !CMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CChildFrame diagnostics

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CChildFrame message handlers

void CChildFrame::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	// TODO: Add your message handler code here and/or call default
	CView *mv = GetActiveView( ) ;
	CMDIChildWnd::OnGetMinMaxInfo(lpMMI);
	if (mv)
//	if (!mv  || (IsIconic() || IsZoomed()))
//		CMDIChildWnd::OnGetMinMaxInfo(lpMMI);
//	else
		mv->SendMessage(WM_GETMINMAXINFO,0,(LPARAM)lpMMI);
}

void CChildFrame::OnSize(UINT nType, int cx, int cy) 
{
	CMDIChildWnd::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	
}


void CChildFrame::OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd) 
{
	CMDIChildWnd::OnMDIActivate(bActivate, pActivateWnd, pDeactivateWnd);
	
	// TODO: Add your message handler code here
	if (bActivate)
	{
		CView *pView = GetActiveView( ) ;
		if (pView)
		{
			CDocument *pDoc = pView->GetDocument();
			CSimulDoc *pMDoc = DYNAMIC_DOWNCAST( CSimulDoc, pDoc);
			if (pMDoc)
			{
				pView->SendMessage(TRACE_VIEW_ACTIVATE,(WPARAM)pDeactivateWnd);
				pMDoc->UpdateViewMenu();
			}
		}
/*	CWnd *pWnd = AfxGetMainWnd();
	CMainFrame *pMainFrm = DYNAMIC_DOWNCAST( CMainFrame, pWnd);
	if (pMainFrm)
	{
		pMainFrm->SendMessage(DOC_CHANGE_DOCKABLEVIEW);
	}*/
	}
	else 
	{
		CWnd *pWnd = AfxGetMainWnd();
		CMainFrame *pMainFrm = DYNAMIC_DOWNCAST( CMainFrame, pWnd);
		if (pMainFrm)
		{
			pMainFrm->OnSetMERIcons(NULL);
		}
	}
}

void CChildFrame::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	//AfxGetMainWnd()->SendMessage(DOC_CHANGE_DOCKABLEVIEW,1);

	//BOOL bCanClose = TRUE;
	CView *pView = GetActiveView( ) ;
	if (pView)
	{
		CDocument *pDoc = pView->GetDocument();
		CSimulDoc *pMDoc = DYNAMIC_DOWNCAST( CSimulDoc, pDoc);
		if (pMDoc)
			/*bCanClose = */pMDoc->OnCloseER(pView);
	}
	CMDIChildWnd::OnClose();
}


/////////////////////////////////////////////////////////////////////////////
// CChildFrameFix

IMPLEMENT_DYNCREATE(CChildFrameFix, CChildFrame)

BEGIN_MESSAGE_MAP(CChildFrameFix, CChildFrame)
	//{{AFX_MSG_MAP(CChildFrameFix)
	ON_WM_CLOSE()
	ON_WM_MOVING()
	ON_WM_MOVE()
	ON_WM_GETMINMAXINFO()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChildFrame construction/destruction

CChildFrameFix::CChildFrameFix()
{
	// TODO: add member initialization code here
	
}

BOOL CChildFrameFix::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	cs.style &= ~WS_THICKFRAME;
	cs.style |= WS_BORDER;
	cs.style &= ~(WS_MINIMIZEBOX|WS_MAXIMIZEBOX);//|WS_SYSMENU|WS_DLGFRAME|WS_BORDER);
	//cs.dwExStyle |= WS_EX_TOOLWINDOW   ;
	if( !CChildFrame::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CChildFramePanes

IMPLEMENT_DYNCREATE(CChildFramePanes, CChildFrame)

BEGIN_MESSAGE_MAP(CChildFramePanes, CChildFrame)
	//{{AFX_MSG_MAP(CChildFramePanes)
	//ON_WM_CONTEXTMENU()
	//}}AFX_MSG_MAP
	ON_MESSAGE(VIEW_PANES_CONTEXTMENU, OnDoContextMenu)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChildFramePanes construction/destruction

CChildFramePanes::CChildFramePanes()
{
	// TODO: add member initialization code here
	
}

CChildFramePanes::~CChildFramePanes()
{
	int nb = m_pSplitterWnds.GetSize();
	for (int i=0;i<nb;i++)
	{
		ST_SplitterWnd* pWnd =  (ST_SplitterWnd*)m_pSplitterWnds.GetAt(i);
		delete pWnd;
	}
	m_pSplitterWnds.RemoveAll();
}


BOOL CChildFramePanes::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

//	cs.style &= ~WS_THICKFRAME;
//	cs.style |= WS_BORDER;
//	cs.style &= ~(WS_MINIMIZEBOX|WS_MAXIMIZEBOX);
	//cs.dwExStyle |= WS_EX_TOOLWINDOW   ;
	if( !CChildFrame::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}

LRESULT CChildFramePanes::OnDoContextMenu(WPARAM wp, LPARAM lp)
{

	return 0;
}


BOOL CChildFramePanes::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	CSimulDoc *pDoc= DYNAMIC_DOWNCAST( CSimulDoc, pContext->m_pCurrentDoc);

	if (pDoc)
		pDoc->OnCreateMultiPanes(&m_pSplitterWnds,pContext,this);

   return(TRUE);
	//return CChildFrame::OnCreateClient(lpcs, pContext);
}

void CChildFramePanes::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	// TODO: Add your message handler code here
	int nb=0;

	nb++;
}


/////////////////////////////////////////////////////////////////////////////
// CChildController

IMPLEMENT_DYNCREATE(CChildController, CChildFrameFix)

BEGIN_MESSAGE_MAP(CChildController, CChildFrameFix)
	//{{AFX_MSG_MAP(CChildController)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChildFrame construction/destruction

CChildController::CChildController()
{
	// TODO: add member initialization code here
	
}

BOOL CChildController::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	cs.style &= ~WS_THICKFRAME;
	cs.style |= WS_BORDER;
	cs.style &= ~(WS_MINIMIZEBOX|WS_MAXIMIZEBOX);
	cs.dwExStyle |= WS_EX_TOOLWINDOW | WS_EX_PALETTEWINDOW | WS_EX_TOPMOST  ;
	if( !CChildFrame::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CChildFrameSplit

IMPLEMENT_DYNCREATE(CChildFrameSplit, CChildFrame)

BEGIN_MESSAGE_MAP(CChildFrameSplit, CChildFrame)
	//{{AFX_MSG_MAP(CChildFrameSplit)
	ON_WM_DESTROY()
	ON_WM_CONTEXTMENU()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChildFrame construction/destruction

CChildFrameSplit::CChildFrameSplit()
{
	// TODO: add member initialization code here
	m_pImageList.Create(IDB_MODEL_ICON,15,0,RGB(128,128,128));	
}


BOOL CChildFrameSplit::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	cs.style &= ~FWS_ADDTOTITLE;
	cs.style |= FWS_PREFIXTITLE;

	if( !CMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}

//////////////////////////////////////////////////////////////////////
/// Construction/Destruction.
///
/// Create the various panes of the Simulation Design Interface, assign shortcut and icons to each pane 
/// and add them to the Visual Framework (m_Framework).
//////////////////////////////////////////////////////////////////////
BOOL CChildFrameSplit::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	// Create a splitter window (DWORD dwId, LPCTSTR szTitle, int nRows, int nCols, CCreateContext *pContext, DWORD dwStyle = 0);
	TVisualObject *pSplitter = new TVisualObject(1,"Test", 1, 2, pContext);
	TVisualObject *pView1 = new TVisualObject(2,0,0,pContext, RUNTIME_CLASS(CViewSDIlist), CSize(160,0));
	TVisualObject *pTab = new TVisualObject(3,0,1,pContext, RUNTIME_CLASS(TTabWnd), CSize(0,0));
	TVisualObject *pTabView1 = new TVisualObject(4,"&Aggregates", pContext,RUNTIME_CLASS(CViewSDIaggreg));
	TVisualObject *pTabView2 = new TVisualObject(5,"&Description",pContext,RUNTIME_CLASS(CViewSDI));
	TVisualObject *pTabView3 = new TVisualObject(6,"&Flow Chart",pContext,RUNTIME_CLASS(CViewSDIdiag));
	TVisualObject *pTabView4 = new TVisualObject(7,"&Model", pContext,RUNTIME_CLASS(CViewSDIbuild));
	//TVisualObject *pTabView2 = new TVisualObject(5,"Definition of the stuff", pContext,RUNTIME_CLASS(CTreeView));

	pTabView1->SetIcon(m_pImageList.ExtractIcon(0));
	pTabView2->SetIcon(m_pImageList.ExtractIcon(1));
	pTabView3->SetIcon(m_pImageList.ExtractIcon(3));
	pTabView4->SetIcon(m_pImageList.ExtractIcon(2));
	
	pTabView1->SetHotKey('A');
	pTabView2->SetHotKey('D'); 
	pTabView3->SetHotKey('F'); 
	pTabView4->SetHotKey('M'); 


	m_Framework.Add(pSplitter);
	m_Framework.Add(pSplitter, pView1);
	m_Framework.Add(pSplitter, pTab);
	m_Framework.Add(pTab, pTabView4);
	m_Framework.Add(pTab, pTabView1);
	m_Framework.Add(pTab, pTabView2);
	m_Framework.Add(pTab, pTabView3);

	BOOL bRet = m_Framework.Create(this);
	//pTabView3->EnableTab(FALSE);

//	CWnd *pwnd = pSplitter->GetWnd();
//	if (pwnd->IsKindOf(RUNTIME_CLASS(CSplitterWnd)))
//	{
//		((CSplitterWnd*)pwnd)->SetColumnInfo(0,500,230);
//	}


	return bRet;
//	return CChildFrame::OnCreateClient(lpcs, pContext);
}

void CChildFrameSplit::OnDestroy() 
{
	CChildFrame::OnDestroy();
	
	// TODO: Add your message handler code here
	m_Framework.Destroy();
	
}



int CChildFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CMDIChildWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here	
	return 0;
}

void CChildFrameFix::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
//	CChildFrame::OnClose();
	BOOL bCanClose = TRUE;
	CView *pView = GetActiveView( ) ;
	if (pView)
	{
		CDocument *pDoc = pView->GetDocument();
		CSimulDoc *pMDoc = DYNAMIC_DOWNCAST( CSimulDoc, pDoc);
		if (pMDoc)
			bCanClose = pMDoc->OnCloseER(pView);
	}
	if (bCanClose) CChildFrame::OnClose();
}

void CChildFrame::OnMove(int x, int y) 
{
	CMDIChildWnd::OnMove(x, y);

//	CView *pView = GetActiveView( ) ;
//	if (pView)
//	{
//		int i=0;
//	}

	// TODO: Add your message handler code here
	
}

void CChildFrameFix::OnMoving(UINT fwSide, LPRECT pRect) 
{

//	CString mstr;
//	GetWindowText(mstr);

//	int width=pRect->right-pRect->left;

//	if (pRect->right>=600) 
//	{
//		pRect->right = 600;
//		pRect->left=600-width;
//	//	return;
//	}
	
//	TRACE(_T("1moving %s (%d): %d %d %d %d\n"),mstr,fwSide, pRect->left,pRect->top,pRect->right,pRect->bottom);
	CChildFrame::OnMoving(fwSide, pRect);
//	TRACE(_T("2moving %s (%d): %d %d %d %d\n"),mstr,fwSide, pRect->left,pRect->top,pRect->right,pRect->bottom);
	// TODO: Add your message handler code here
	
}

void CChildFrameFix::OnMove(int x, int y) 
{
	CChildFrame::OnMove(x, y);
	
	// TODO: Add your message handler code here
/*	CView *pView = GetActiveView( ) ;
	if (!pView) return;
	CDocument *pDoc = pView->GetDocument();
	if (!pDoc) return;

	CRect fullrect;
	AfxGetMainWnd()->GetClientRect(fullrect);

	CRgn fullRgn;
	BOOL bres = fullRgn.CreateRectRgnIndirect(fullrect);
	POSITION pos = pDoc->GetFirstViewPosition();
	   int n1 = COMPLEXREGION ;
	   int n2= ERROR    ;
	   int n3= NULLREGION    ;
	   int n4= SIMPLEREGION    ;
	while (pos)
	{
	   CView* pViewDoc = pDoc->GetNextView(pos);
	   if (!pViewDoc) continue;

	   CFrameWnd* pFrame = pViewDoc->GetParentFrame();
	   if (!pFrame) continue;


	   CRect mrect,mrect2;
	   pFrame->GetWindowRect(mrect);
	   mrect2 = mrect;

	   CRgn fRgn,nRgn;
	   bres = fRgn.CreateRectRgnIndirect(mrect);
	   int res = fRgn.CopyRgn(&fullRgn);
	   bres = nRgn.CreateRectRgnIndirect(mrect);

	   CRgn resRgn;   
	   bres = resRgn.CreateRectRgnIndirect(mrect);
	   res = resRgn.CombineRgn(&fRgn,&nRgn,RGN_DIFF);

   	   fullRgn.CopyRgn(&resRgn);
	
	}

	CRect mrect3;
	int nres = fullRgn.GetRgnBox (mrect3);
	*/
}

void CChildFrameFix::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	// TODO: Add your message handler code here and/or call default
	
//	lpMMI->ptMaxTrackSize.x = 200;
	CChildFrame::OnGetMinMaxInfo(lpMMI);
//	lpMMI->ptMaxTrackSize.x = 200;
}

BOOL CChildFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CMDIChildWnd::OnCreateClient(lpcs, pContext);
}

BOOL CChildFrameSplit::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	m_Framework.ProcessMessage(pMsg);
	return CChildFrame::PreTranslateMessage(pMsg);
}
