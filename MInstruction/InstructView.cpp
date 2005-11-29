// InstructView.cpp : implementation file
//

#include "stdafx.h"
#include "simul.h"

#include "MainFrm.h"
#include "LearnerTrace.h"

#include <Prefs\Pref.h>


#include "GridCtrl\MemDC.h"
#include "SimulDoc.h"
#include <MInstruction\InstructView.h>

#include <MSimulation\Model.h>
#include <MInstruction\LearningUnit.h>
//#include <MInstruction\DefineMERDlg.h>
//#include <MInstruction\DefineModelDlg.h>

#include <MInstruction\DefineLUPropSheet.h>
#include <MInstruction\DefineERPropSheet.h>

#include "SortERDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CViewCDI

IMPLEMENT_DYNCREATE(CViewCDI, CScrollView)

CViewCDI::CViewCDI()
{
	m_DocSize = CSize(-1,-1);

	m_CurrUnit = m_CurrER = 0;

	m_Tracker.m_nStyle = /*CRectTracker::resizeInside | CRectTracker::dottedLine| */CRectTracker::hatchedBorder ;

	strcpy(m_logFont.lfFaceName, "Arial");
	m_logFont.lfHeight=-11;
	m_logFont.lfWidth=0;
	m_logFont.lfEscapement=0;
	m_logFont.lfOrientation=0;
	m_logFont.lfWeight=FW_BOLD;
	m_logFont.lfItalic=FALSE;
	m_logFont.lfUnderline=FALSE;
	m_logFont.lfStrikeOut=FALSE;
	m_logFont.lfCharSet=0;//ANSI_CHARSET;
	m_logFont.lfOutPrecision=3;//OUT_OUTLINE_PRECIS;
	m_logFont.lfClipPrecision=2;//OUT_DEFAULT_PRECIS; 
	m_logFont.lfQuality=1;//OUT_DEFAULT_PRECIS; 
	m_logFont.lfPitchAndFamily=34;//FF_MODERN|DEFAULT_PITCH;

// NNN	EnableToolTips(true);

	m_ImageList.Create(IDB_ER_ICONS,32,0,RGB(192,192,192));
}

CViewCDI::~CViewCDI()
{
}

BOOL CViewCDI::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (IsViewActive())
	{
// NNN		m_ToolTip.RelayEvent(pMsg);

		if(pMsg->message== WM_LBUTTONDOWN ||
			pMsg->message== WM_LBUTTONUP ||
			pMsg->message== WM_MOUSEMOVE)
			m_wndToolTip.RelayEvent(pMsg);
	}


/*	CSimulDoc* pDoc = GetDocument();
	if (pDoc)
	{
		CClientDC dc(this);
		OnPrepareDC(&dc); // set up mapping mode and viewport origin
		CPoint point = pMsg->pt;
		dc.LPtoDP(&point);
		pMsg->pt = point;
		CInstModel *pInst = pDoc->m_pInstruct;
		for (int i=0;i<pInst->m_cLUSet.GetSize();i++)
		{
			CLearningUnit *pLU = pInst->m_cLUSet.GetAt(i);
			pLU->PreTranslateMessage(pMsg);
		}
	}*/
	return CScrollView::PreTranslateMessage(pMsg);
}

BEGIN_MESSAGE_MAP(CViewCDI, CScrollView)
	//{{AFX_MSG_MAP(CViewCDI)
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
	ON_WM_LBUTTONUP()
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateCmdEdit)
	ON_UPDATE_COMMAND_UI(ID_INSTRUCTION_ADDER, OnUpdateCmdER)
	ON_UPDATE_COMMAND_UI(ID_INSTRUCTION_ADDUNIT, OnUpdateCmdUnit)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_CLEAR, OnEditDelete)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_COMMAND(ID_INSTRUCTION_ADDER, OnAddER)
	ON_COMMAND(ID_INSTRUCTION_ADDUNIT, OnAddUnit)
	ON_COMMAND(ID_INSTRUCTION_TRYER, OnLaunchER)
	ON_COMMAND(ID_INSTRUCTION_UNITPROP, OnLUProperty)
	ON_COMMAND(ID_INSTRUCTION_ERPROP,	OnERProperty)
	ON_COMMAND(ID_INSTRUCTION_SAVEPOS, OnSaveERPos)
	ON_COMMAND(ID_INSTRUCTION_NEXTLU, OnActivateNextLU)
	ON_COMMAND(ID_INSTRUCTION_PREVLU, OnActivatePrevLU)
	ON_COMMAND(ID_INSTRUCTION_AUTHMODE, OnActivateCurrentLU)
	ON_UPDATE_COMMAND_UI(ID_INSTRUCTION_NEXTLU, OnUpdateLU)
	ON_WM_SIZE()
	ON_COMMAND(ID_INSTRUCTION_SORTER, OnSortER)
	ON_UPDATE_COMMAND_UI(ID_INSTRUCTION_SORTER, OnUpdateSortER)
	ON_UPDATE_COMMAND_UI(ID_INSTRUCTION_TRYER, OnUpdateCmdER)
	ON_UPDATE_COMMAND_UI(ID_INSTRUCTION_ERPROP, OnUpdateCmdER)
	ON_UPDATE_COMMAND_UI(ID_INSTRUCTION_UNITPROP, OnUpdateCmdUnit)
	ON_UPDATE_COMMAND_UI(ID_INSTRUCTION_SAVEPOS, OnUpdateCmdUnit)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CLEAR, OnUpdateCmdEdit)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateCmdEdit)
	ON_UPDATE_COMMAND_UI(ID_INSTRUCTION_PREVLU, OnUpdateLU)
	ON_UPDATE_COMMAND_UI(ID_INSTRUCTION_AUTHMODE, OnUpdateLU)
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewCDI drawing
BOOL CViewCDI::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	return TRUE;
	//return CScrollView::OnEraseBkgnd(pDC);
}

void CViewCDI::OnDraw(CDC* pDC)
{
	CSimulDoc* pDoc = GetDocument();
	if (!pDoc) 	return;

	CMemDC MemDC(pDC);

	COLORREF bcolor = ::GetSysColor(COLOR_WINDOW/*COLOR_3DFACE*/); 
	CBrush FixedBack(bcolor);
	CRect ClipRect;
	if (/*!pDC->IsPrinting() && */(MemDC.GetClipBox(ClipRect) != ERROR))
	{
		//pDC->LPtoDP(&ClipRect);
		ClipRect.InflateRect(1, 1); // avoid rounding to nothing
		MemDC.FillRect(ClipRect,&FixedBack);
		//pDC->Rectangle(ClipRect);
	}

/*	MemDC.MoveTo(0,150);
	MemDC.LineTo(800,150);
	MemDC.MoveTo(50,0);
	MemDC.LineTo(50,800);*/

	CFont pfont,*oldf;
	pfont.CreateFontIndirect(&m_logFont);

	oldf= ( CFont* ) MemDC->SelectObject(&pfont);

	pDoc->m_pInstruct->OnDraw(&MemDC,&m_ImageList);
	if (m_CurrUnit)
	{
		//CInstModel *pInst = pDoc->m_pInstruct;
		//CLearningUnit *pLU = pInst->m_cLUSet.GetAt(m_CurrUnit-1);
		//CRect mrect = pLU->GetBoundRect();
		//MemDC.LPtoDP(&(v));
		//m_Tracker.Draw(&MemDC);
		MemDC.DrawFocusRect(m_Tracker.m_rect);
	}

	MemDC.SelectObject(oldf);
}

/////////////////////////////////////////////////////////////////////////////
// CViewCDI diagnostics

bool CViewCDI::IsViewActive()
{
	//Get MDI frame window
	CMDIFrameWnd *pFrame = (CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;
	// Get the active MDI child window.
	CMDIChildWnd *pChild = (CMDIChildWnd *) pFrame->GetActiveFrame();
	// or CMDIChildWnd *pChild = pFrame->MDIGetActive();
	
	// Get the active view attached to the active MDI child window.
	CView *pView = pChild->GetActiveView();

	return (pView == this);
}

#ifdef _DEBUG
void CViewCDI::AssertValid() const
{
	CScrollView::AssertValid();
}

void CViewCDI::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CSimulDoc* CViewCDI::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSimulDoc)));
	return (CSimulDoc*)m_pDocument;
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CViewCDI message handlers

void CViewCDI::OnInitialUpdate() 
{
	CScrollView::OnInitialUpdate();

	m_wndToolTip.Create(this);
	m_wndToolTip.AddTool(this);
	m_wndToolTip.SetDelayTime(TTDT_INITIAL,200);
	m_wndToolTip.SetDelayTime(TTDT_RESHOW,100);

	CSimulDoc* pDoc = GetDocument();
	if (!pDoc) 	return;

//	pDoc->m_pInstruct->OnSetToolTip(this,&m_ToolTip);

/*	if (m_DocSize.cx == -1)
	{
		CClientDC dc(this);
		//OnPrepareDC(&dc); // set up mapping mode and viewport origin
		m_DocSize=CSize(dc.GetDeviceCaps(HORZRES), dc.GetDeviceCaps(VERTRES));
		m_DocSize=CSize(600,300);
		m_DocSize=CSize(0,0);
		SetScrollSizes( MM_TEXT, m_DocSize);
	}*/
	m_DocSize=CSize(0,0);
	SetScrollSizes( MM_TEXT, m_DocSize);
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
void CViewCDI::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	// TODO: Add your specialized code here and/or call the base class
}

int CViewCDI::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CScrollView::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_cToolBar.CreateEx(GetParent(), TBSTYLE_FLAT | TBSTYLE_TOOLTIPS, 
			WS_CHILD | WS_VISIBLE | CBRS_TOP | 
			/*CBRS_GRIPPER | */CBRS_TOOLTIPS | CBRS_FLYBY | CCS_ADJUSTABLE,
			CRect(0, 0, 0, 0), 120) ||
		!m_cToolBar.LoadToolBar(IDR_SIMUL_CONTEXT))
	{
		TRACE0("Failed to create toolbar1\n");
		return -1;      // fail to create  
	}

/* NNN
	if (!m_ToolTip.Create(this))//, TTS_ALWAYSTIP))
	{
		TRACE0("Failed to create ToolTip\n");
		return -1;      // fail to create  
	}
	m_ToolTip.SetDelayTime(TTDT_AUTOPOP,1000);//SHRT_MAX);
	m_ToolTip.SetDelayTime(TTDT_INITIAL,200);
	m_ToolTip.SetDelayTime(TTDT_RESHOW,2);
	m_ToolTip.SetMaxTipWidth(SHRT_MAX);
*/

	CSimulDoc* pDoc = GetDocument();
	if (pDoc)
	{
		CInstModel *pInst = pDoc->m_pInstruct;
		for (int i=0;i<pInst->m_cLUSet.GetSize();i++)
		{
			CLearningUnit *pLU = pInst->m_cLUSet.GetAt(i);
			//pLU->CreateToolTip(this,);
// NNN			pLU->OnUpdateToolTip(this,&m_ToolTip);
		}
	}

	return 0;
}

void CViewCDI::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CScrollView::OnLButtonUp(nFlags, point);
	CSimulDoc* pDoc = GetDocument();
	if (!pDoc) 	return;

	CClientDC dc(this);
	OnPrepareDC(&dc); // set up mapping mode and viewport origin
	dc.DPtoLP(&point);

	CRect mrect;
	if (m_CurrER && m_CurrUnit)
	{
		CInstModel *pInst = pDoc->m_pInstruct;
		int nb1 = pInst->m_cLUSet.GetSize();
		CLearningUnit *pLU = NULL;
		CExternRepUnit *pER = NULL;
		if (m_CurrUnit-1 < nb1)
			pLU = pInst->m_cLUSet.GetAt(m_CurrUnit-1);
		if (pLU)
		{
			int nb2 = pLU->m_cMERSet.GetSize();
			if (m_CurrER-1 < nb2)
				pER = pLU->m_cMERSet.GetAt(m_CurrER-1);
			if (pER)
				pER->SetSelected(FALSE);
		}
	}

	CPoint rep = pDoc->m_pInstruct->HitTest(point,mrect);
	//int rep = pDoc->m_pInstruct->HitTest(point,mrect);
//	WORD aaa = LOWORD(rep);
//	WORD bbb= HIWORD(rep);
//	CString mstrrrr;
//	mstrrrr.Format(_T("Unit : %d , ER : %d"),aaa,bbb);
//	MessageBox(mstrrrr);
	if (rep == CPoint(0,0))
	//if (rep == 0)
	{
		m_CurrUnit=m_CurrER=0;
		m_Tracker.m_rect = CRect(-1,-1,-1,-1);
	}
	else
	{
		//m_CurrUnit = LOWORD(rep);
		//m_CurrER = HIWORD(rep);
		m_CurrUnit = rep.x;
		m_CurrER = rep.y;
		//if (m_CurrER == 1) m_CurrER=0;
		TRACE( "click in unit %d, ER %d\n", m_CurrUnit+1, m_CurrER);
		if (m_CurrER != 0)
		{
			CInstModel *pInst = pDoc->m_pInstruct;
			CLearningUnit *pLU = pInst->m_cLUSet.GetAt(m_CurrUnit-1);
			CExternRepUnit *pER = pLU->m_cMERSet.GetAt(m_CurrER-1);
			pER->SetSelected(TRUE);
			CWnd* pMessageBar = GetParentFrame()->GetMessageBar();
			if (pMessageBar != NULL)
			{
				CString strMessage;

				strMessage = pER->GetName();
				pMessageBar->SetWindowText(strMessage);
			}

		}
		m_Tracker.m_rect = mrect;
	}
	Invalidate();
	UpdateWindow( );
}

void CViewCDI::OnAddER() 
{
	// TODO: Add your command handler code here
	if (!m_CurrUnit) return;
	CSimulDoc* pDoc = GetDocument();
	if (!pDoc) 	return;
	CInstModel *pInst = pDoc->m_pInstruct;
	CLearningUnit *pLU = pInst->m_cLUSet.GetAt(m_CurrUnit-1);
	if (!pLU) return;

	if (pLU->m_nSimulID==-1) 
	{
		MessageError(ERR_NOSIMMODEL);
		return;
	}

	int nbER = pLU->AddExternalRep();
	if (nbER == -1) return;

	//pLU->OnUpdateUnit();
	pInst->OnUpdatePosition();
/*	int h = 0,w=0;
	for (int i=0;i<pInst->m_cLUSet.GetSize();i++)
	{
		CLearningUnit *pLU2 = pInst->m_cLUSet.GetAt(i);
		pLU2->OnUpdateToolTip(this,&m_ToolTip);
		CRect mrect = pLU2->GetLURect();
		if (h < mrect.bottom) h = mrect.bottom;
		if (w < mrect.right) w = mrect.right;

	}
	m_Tracker.m_rect = pLU->GetBoundRect();
	m_DocSize=CSize(w+50,300);
	m_DocSize=CSize(0,0);
	SetScrollSizes( MM_TEXT, m_DocSize);*/
	if (m_CurrER)
	{
		CExternRepUnit *pER = pLU->m_cMERSet.GetAt(m_CurrER-1);
		if (pER) pER->SetSelected(FALSE);
	}
	m_CurrER = nbER;
	CExternRepUnit *pER = pLU->m_cMERSet.GetAt(m_CurrER-1);
	if (pER) pER->SetSelected(TRUE);

	pDoc->SetModifiedFlag();
	Invalidate();
	UpdateWindow( );
	OnERProperty() ;
//	pDoc->CheckPoint();
}

void CViewCDI::OnAddUnit() 
{
	// TODO: Add your command handler code here
	CSimulDoc* pDoc = GetDocument();
	if (!pDoc) 	return;
	CInstModel *pInst = pDoc->m_pInstruct;
	CLearningUnit* pLU = pInst->AddLearningUnit();
	//pLU->CreateToolTip(this);
	//pLU->OnUpdateToolTip(this,&m_ToolTip);

/*	CRect mrect = pLU->GetLURect();
	m_DocSize=CSize(mrect.right+50,300);
	m_DocSize=CSize(0,0);
	SetScrollSizes( MM_TEXT, m_DocSize);*/
	m_Tracker.m_rect = pLU->GetBoundRect();
	//MemDC.LPtoDP(&(m_Tracker.m_rect));

	CSimModel *pSim = pDoc->m_pSimul;
	CMdEquation *pEqu = pSim->GetEquation(pLU->m_nSimulID);
	if (pEqu) pEqu->OnUpdate(/*pSim,*/pLU,TRUE);

	pDoc->SetModifiedFlag();
	pDoc->UpdateAllViews(this,DOC_UPDATE_LUNIT);
	Invalidate();
	UpdateWindow( );
//	pDoc->CheckPoint();
}


void CViewCDI::OnContextMenu(CWnd* pWnd, CPoint point) 
{
   // make sure window is active
   GetParentFrame()->ActivateFrame();
   CMenu menu;
   if (menu.LoadMenu(IDR_SIMUL_CONTEXT))
   {
	   CMenu* pPopup = menu.GetSubMenu(3);
	   ASSERT(pPopup != NULL);
	   
	   pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON,
		   point.x, point.y,
		   AfxGetMainWnd()); // use main window for cmds
   }
	
}

void CViewCDI::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CScrollView::OnRButtonDown(nFlags, point);
	CSimulDoc* pDoc = GetDocument();
	if (!pDoc) 	return;

	CClientDC dc(this);
	OnPrepareDC(&dc); // set up mapping mode and viewport origin
	dc.DPtoLP(&point);

	CRect mrect;
	if (m_CurrER && m_CurrUnit)
	{
		CInstModel *pInst = pDoc->m_pInstruct;
		CLearningUnit *pLU = pInst->m_cLUSet.GetAt(m_CurrUnit-1);
		CExternRepUnit *pER = pLU->m_cMERSet.GetAt(m_CurrER-1);
		pER->SetSelected(FALSE);
	}

	//int rep = pDoc->m_pInstruct->HitTest(point,mrect);
	CPoint rep = pDoc->m_pInstruct->HitTest(point,mrect);
//	CString mstr;
//	mstr.Format(_T("unit : %d\nEr : %d"),rep.x,rep.y);
//	AfxGetMainWnd()->MessageBox(mstr,"Warning",MB_OK|MB_ICONWARNING);
//
	//if (rep != 0)
	if (rep != CPoint(0,0))
	{
		//m_CurrUnit = LOWORD(rep);
		//m_CurrER = HIWORD(rep);
		m_CurrUnit = rep.x;
		m_CurrER = rep.y;
		//if (m_CurrER == 1) m_CurrER=0;
		TRACE( "Rclick in unit %d, ER %d\n", m_CurrUnit+1, m_CurrER);
		if (m_CurrER != 0)
		{
			CInstModel *pInst = pDoc->m_pInstruct;
			CLearningUnit *pLU = pInst->m_cLUSet.GetAt(m_CurrUnit-1);
			CExternRepUnit *pER = pLU->m_cMERSet.GetAt(m_CurrER-1);
			pER->SetSelected(TRUE);
		}

		m_Tracker.m_rect = mrect;
	}
	else 
	{
		m_CurrUnit=m_CurrER=0;
		m_Tracker.m_rect = CRect(-1,-1,-1,-1);
	}
	Invalidate();
	UpdateWindow( );
}

void CViewCDI::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CScrollView::OnLButtonDblClk(nFlags, point);
	CSimulDoc* pDoc = GetDocument();
	if (!pDoc) 	return;

	CClientDC dc(this);
	OnPrepareDC(&dc); // set up mapping mode and viewport origin
	dc.DPtoLP(&point);

	if (!m_CurrUnit) return;
	if (m_CurrER > 1)
		OnERProperty() ;
	else
		OnLUProperty();
}

void CViewCDI::OnLUProperty() 
{
	// TODO: Add your command handler code here
	CSimulDoc* pDoc = GetDocument();
	if (!pDoc) 	return;

	CInstModel *pInst = pDoc->m_pInstruct;
	CLearningUnit *pLU = pInst->m_cLUSet.GetAt(m_CurrUnit-1);
	if (!pLU) return;

	pLU->OnCloseAllER();

	CSimModel *pSim = pDoc->m_pSimul;


	CDefineLUPropSheet mSheet(this,pSim,pLU);

	mSheet.DoModal();

	for(int i = 0; i < pLU->m_cDefOutputSet.GetSize();i++ )
		delete pLU->m_cDefOutputSet.GetAt(i);
	pLU->m_cDefOutputSet.RemoveAll();

	pDoc->SetModifiedFlag();
	pDoc->UpdateDoc(this);


/*	return;


	CDefineLUDlg dlg(this,pSim,pLU);
	if (dlg.DoModal() == IDOK)
	{
		bool modchg = (pLU->m_simulID != dlg.m_nModel);
		bool expchg = dlg.m_bModified;
		pLU->m_nbERMax = dlg.m_nERSimNb;
		pLU->m_simulID = dlg.m_nModel;
		pLU->m_sHTML =  dlg.m_sHTMLDoc;
		pLU->m_sName =  dlg.m_strName;
		pLU->m_nTimer = dlg.m_nTimer;
		pLU->m_bShowAbbrev = dlg.m_bShowAbb;
		pLU->m_bExpSetLocked = dlg.m_bExpSetLocked;

		pLU->m_bIsReady = TRUE;

// MMM		pLU->OnUpdateToolTip(this,&m_ToolTip);


		CMdEquation *pEqu = pSim->GetEquation(pLU->m_simulID);
		if (pEqu) pEqu->OnUpdate(pLU,TRUE); //pSim,pLU,TRUE);

		if (modchg || (!modchg && expchg))
		{
			//pLU->m_cDefOutputSet;
			for(int i = 0; i < pLU->m_cDefOutputSet.GetSize();i++ )
				delete pLU->m_cDefOutputSet.GetAt(i);
			pLU->m_cDefOutputSet.RemoveAll();
		}

		for(int i = 0; i < pLU->m_cTBreakSet.GetSize();i++ )
		delete pLU->m_cTBreakSet.GetAt(i);
		pLU->m_cTBreakSet.RemoveAll();
//		pLU->m_cTTTT.RemoveAll();
		int nbTB = dlg.m_BreakSlider.m_Markers.GetCount();
		POSITION pos = dlg.m_BreakSlider.m_Markers.GetHeadPosition();
		int Idx = 0;
		while (pos != NULL)
		{
			CMarker* pMarker = (CMarker*)dlg.m_BreakSlider.m_Markers.GetNext(pos);
			//CString mstr = dlg.m_BreakSlider.m_cStrDesc.GetAt(Idx);
	//		if (pMarker->m_Pos)
			{
				CString mstr = pMarker->m_strDesc;
				CString mstrH = pMarker->m_strHTML;
				//int nStyle = dlg.m_BreakSlider.m_cIntStyle.GetAt(Idx);
				int nStyle = pMarker->m_nType;

				CTimerBreakPoint *pTB = new CTimerBreakPoint();
				//pTB->m_sDesc = (bAct) ? tempSTR2 : tempSTR1;
				pTB->m_sDesc = mstr;
				pTB->m_sHTML= mstrH;
				pTB->m_nBPType = nStyle;
				pTB->m_tBreakPt = (int)pMarker->m_Pos;//-1;
				pLU->m_cTBreakSet.Add(pTB);
//				pLU->m_cTTTT.SetAt(pTB->m_tBreakPt,*pTB);
				Idx++;
			}
		}
		pDoc->SetModifiedFlag();
		pDoc->UpdateDoc(this);
	}*/
}


/*#define ID_FIRST	IDR_SIMULTABLE
#define ID_LAST		IDR_SIMULXYGRAPH

UINT	MyCmdList[] ={
	ID_VIEW_TABLE,
	ID_VIEW_CHART,
	ID_VIEW_MODEL,
	ID_VIEW_GRAPH,
	ID_VIEW_ANIM,
	ID_VIEW_PARAM,
	ID_VIEW_TIMER,
	ID_VIEW_XYGRAPH
};
*/

/*void CViewCDI::OnLaunchStartER()
{
	CSimulDoc* pDoc = GetDocument();
	pDoc->OnLaunchStartER();
	CLearningUnit *pLU = pDoc->GetCurrentLU();
	if (!pLU) return;

	int nb = pLU->m_cMERSet.GetSize();
	for (int i=0;i<nb;i++)
	{
		CExternRepUnit *pER=pLU->m_cMERSet.GetAt(i);
		if (!pER) continue;
		if (pER->m_bOpenStart)
			pLU->LauchER(pER->m_CmDId);
	}
}*/

void CViewCDI::OnLaunchER() 
{
	CSimulDoc* pDoc = GetDocument();
	CInstModel *pInst = pDoc->m_pInstruct;
	CLearningUnit *pLU = pInst->m_cLUSet.GetAt(m_CurrUnit-1);
	if (!pLU) return;
	
	CExternRepUnit *pER = pLU->m_cMERSet.GetAt(m_CurrER-1);
	if (!pER) return;

/*	UINT nERID = pER->GetID();
	if (nERID != -1)
		AfxGetMainWnd()->SendMessage(WM_COMMAND,MyCmdList[nERID-ID_FIRST],0);*/

	pLU->LauchER(pER->m_CmDId);
	//return;
}

void CViewCDI::MessageError(UINT msg)
{
	CString mstr;
	mstr.LoadString(msg);
	MessageBox(mstr,NULL,MB_OK|MB_ICONERROR);
}

void CViewCDI::OnERProperty() 
{
	// TODO: Add your command handler code here
	CSimulDoc* pDoc = GetDocument();
	if (!pDoc) 	return;

	CInstModel *pInst = pDoc->m_pInstruct;
	CLearningUnit *pLU = pInst->m_cLUSet.GetAt(m_CurrUnit-1);
	if (!pLU) return;

	if (pLU->m_nSimulID==-1) 
	{
		MessageError(ERR_NOSIMMODEL);
		return;
	}
	
	CExternRepUnit *pER = pLU->m_cMERSet.GetAt(m_CurrER-1);
	CSimModel *pSim = pDoc->m_pSimul;
	CMdEquation *pEqu = pSim->GetEquation(pLU->m_nSimulID);


	CDefineERPropSheet dlg2(this,pER,pEqu,pLU);

	int nRes = dlg2.DoModal();


	if (nRes==IDOK)
	{
		Invalidate();
		if (pER->m_pExtView)
		{
			pDoc->UpdateAllViews(this,DOC_UPDATE_ER,(CObject*)pER->m_pExtView);
		}
		pDoc->SetModifiedFlag();
	}


/*	return;

	CDefineMERDlg dlg(this,pER,pEqu,pLU);

	int nb = CPref::g_cCatList.GetSize();
	for (int i=0;i<nb;i++)
	{
		CString mstr= CPref::g_cCatList.GetAt(i);
		if (mstr == pER->m_sCategory)
		{
			dlg.m_nCategory = i;
			break;
		}
	}

	dlg.m_bStartOpen = pER->m_bOpenStart;
	//dlg.m_nCurrIcon = pER->GetIcon();
	if (dlg.DoModal() == IDOK)
	{
		pER->SetID(dlg.m_nExtRep);
		pER->SetIcon(dlg.m_nIconIDList.GetAt(dlg.m_nCurrIcon));
		CString mstr;
		mstr = dlg.m_sName;
		//mstr = "fdsf\nfdfdfdfd";
		pER->SetName(mstr);
		pER->m_bIsReady = TRUE;
		pER->m_nMode = dlg.m_nRunMode;
		//pER->SetTranslation(dlg.m_bTransMode);
		pER->m_bOpenStart = dlg.m_bStartOpen;

		if (dlg.m_nCategory != LB_ERR && dlg.m_nCategory < CPref::g_cCatList.GetSize()) 
			pER->m_sCategory = CPref::g_cCatList.GetAt(dlg.m_nCategory);
		else
			pER->m_sCategory.Empty();

// NNN		pLU->OnUpdateToolTip(this,&m_ToolTip);
		pLU->UpdateERMenu(NULL);
		Invalidate();
		if (pER->m_pExtView)
		{
			//pDoc->UpdateThisView(pER->m_pExtView,DOC_UPDATE_ER);
			//pER->m_pExtView->UpdOnUpdate(this,DOC_UPDATE_ER,NULL);
			///pDoc->UpdateDoc(this);
			pDoc->UpdateAllViews(this,DOC_UPDATE_ER,(CObject*)pER->m_pExtView);

		}

		CWnd *pWnd = AfxGetMainWnd();
		if (!pWnd) return;

		CMenu *pTopMenu = pWnd->GetMenu();
		if (!pTopMenu) return;
	
		CMenu* pColorMenu = NULL;

		int iPos;
		for (iPos = pTopMenu->GetMenuItemCount()-1; iPos >= 0; iPos--)
		{
			CMenu* pMenu = pTopMenu->GetSubMenu(iPos);
			if (pMenu && pMenu->GetMenuItemID(0) == ID_VIEW_TOOLBAR)
			{
				pColorMenu = pMenu;
				break;
			}
		}
		if (!pColorMenu) return;
		int nnnn = pColorMenu->GetMenuItemCount();
		pColorMenu->AppendMenu(MF_STRING,
			MyCmdList[pER->GetID()-ID_FIRST],
			"&1 fds dsaf ads");

		pDoc->SetModifiedFlag();
	}*/
}

void CViewCDI::OnUpdateCmdEdit(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CMainFrame *pFrame = DYNAMIC_DOWNCAST(CMainFrame, AfxGetMainWnd());
	CCopyPaste *pCopy = NULL;
	if (pFrame)
		pCopy = pFrame->GetCopyBuffer();

	BOOL bsel=FALSE;
	switch(pCmdUI->m_nID) {
	case ID_EDIT_PASTE :
		bsel = (BOOL) (pCopy && pCopy->GetBufferType() == CCopyPaste::COPY_LUNIT);
		break;
	case ID_EDIT_COPY :
		bsel = (BOOL) pCopy && (m_CurrUnit  || m_CurrER) ;
		break;
	case ID_EDIT_CLEAR :
		bsel = (BOOL)(m_CurrUnit || m_CurrER );
		break;
	default:
		break;
	}
	pCmdUI->Enable(bsel);

}

void CViewCDI::OnUpdateCmdER(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	bool bsel=true;
	int nCLU = GetDocument()->m_CurrInst;

	switch(pCmdUI->m_nID) {
	case ID_INSTRUCTION_ADDER :
		bsel = (bool)(m_CurrUnit && m_CurrER <= 1);
		break;
	case ID_INSTRUCTION_ERPROP:
		bsel = (bool)(m_CurrUnit && m_CurrER > 1) ;
		break;
	case ID_INSTRUCTION_TRYER:
		bsel = (bool)((m_CurrUnit-1)==nCLU && m_CurrER >= 1) ;
		break;
	default :
		break;

	}
	pCmdUI->Enable(bsel);
	
}

void CViewCDI::OnUpdateCmdUnit(CCmdUI* pCmdUI) 
{
	BOOL bsel=FALSE;

	switch(pCmdUI->m_nID) {
	case ID_INSTRUCTION_ADDUNIT :
		bsel = !((BOOL)m_CurrUnit);
		break;
	case ID_INSTRUCTION_UNITPROP:
		bsel = (BOOL)(m_CurrUnit && (!m_CurrER || m_CurrER==1)) ;
		break;
	case ID_INSTRUCTION_SAVEPOS:
		{
			bsel = (BOOL)(m_CurrUnit && !m_CurrER) ;
			if (!bsel) break;
			CSimulDoc* pDoc = GetDocument();
			if (!pDoc) 	break;
			CInstModel *pInst = pDoc->m_pInstruct;
			if (!pInst)	break;
			CLearningUnit *pLU = pInst->m_cLUSet.GetAt(m_CurrUnit-1);
			if (!pLU) break;

			bsel = FALSE;
			for (int i=0;i<pLU->m_cMERSet.GetSize() && !bsel;i++)
			{
				CExternRepUnit *pER = pLU->m_cMERSet.GetAt(i);
				if (pER && pER->m_pExtView)
				{
					bsel = TRUE;
				}
			}
		}
		break;
	default :
		break;

	}
	pCmdUI->Enable(bsel);

}


void CViewCDI::OnEditCopy() 
{
	// TODO: Add your command handler code here
	CSimulDoc* pDoc = GetDocument();
	if (!pDoc) 	return;
	CInstModel *pInst = pDoc->m_pInstruct;
	if (!pInst)	return;
	CLearningUnit *pLU = pInst->m_cLUSet.GetAt(m_CurrUnit-1);
	if (!pLU) return;

	CMainFrame *pFrame = DYNAMIC_DOWNCAST(CMainFrame, AfxGetMainWnd());
	if (!pFrame) return;

	CCopyPaste *pCopy = pFrame->GetCopyBuffer();
	if (!pCopy) return;

	CSimModel *pSim = pDoc->m_pSimul;
	CMdEquation *pEqu = pSim->GetEquation(pLU->m_nSimulID);
	if (!pEqu) return;

	pCopy->ClearBuffer();
	pCopy->SetBufferType(CCopyPaste::COPY_LUNIT);
	pCopy->SetOwner();

	CLearningUnit *pNewLU = pLU->Clone();
	CMdEquation *pNewEqu = pEqu->Clone();

	pNewLU->m_pEquModel = pEqu;

	pCopy->Add(pNewEqu);
	pCopy->Add(pNewLU);
}

void CViewCDI::OnEditPaste() 
{
	// TODO: Add your command handler code here
	CMainFrame *pFrame = DYNAMIC_DOWNCAST(CMainFrame, AfxGetMainWnd());
	if (!pFrame) return;
	CCopyPaste *pCopy = pFrame->GetCopyBuffer();
	if (!pCopy) return;
	int nb = pCopy->GetBufferType();
	if (nb != CCopyPaste::COPY_LUNIT) return;

	CSimulDoc* pDoc = GetDocument();
	if (!pDoc) 	return;
	CInstModel *pInst = pDoc->m_pInstruct;
	if (!pInst)	return;
//	CLearningUnit *pLU = pInst->m_cLUSet.GetAt(m_CurrUnit-1);
//	if (!pLU) return;

	nb = pCopy->GetSize();

	CObject *pObj = pCopy->GetAt(0);
	CMdEquation *pNewEqu = DYNAMIC_DOWNCAST(CMdEquation, pObj);

	if (!pNewEqu) return;

	for (int i=1;i<nb;i++)
	{
		CObject *pObj = pCopy->GetAt(i);
		if (!pObj) continue;

		CLearningUnit *pCLU = DYNAMIC_DOWNCAST(CLearningUnit, pObj);
		if (!pCLU) continue;

		if (!pCLU->m_pEquModel) continue;

		CLearningUnit *pLU = pCLU->Clone();
		if (!pLU) continue;

		CSimModel *pSim = pDoc->m_pSimul;
		int nEqu = pSim->m_cEquSet.GetSize();
		BOOL bFound = FALSE;
		BOOL bCopy = FALSE;
		for (int i=0;i<nEqu;i++)
		{
			CMdEquation *pEqu = pSim->GetEquation(i);
			if (!pEqu) continue;
			//CRuntimeClass *pCopyClass = pEqu->GetRuntimeClass();
			//CmdEq *pCopyClass = pEqu;
			if (pEqu==pLU->m_pEquModel)
			{
				bFound = i+1;
				break;
			}
		}
		
		if (!bFound) for (i=0;i<nEqu;i++)
		{
			CMdEquation *pEqu = pSim->GetEquation(i);
			if (!pEqu) continue;
			CRuntimeClass *pCopyClass = pNewEqu->GetRuntimeClass();
			CRuntimeClass *pClass = pEqu->GetRuntimeClass();
			if (pCopyClass == pClass)
			{
				bFound = i+1;
				bCopy = TRUE;
				break;
			}
		}
		CString strModel = pNewEqu->GetSimulationName();
		CString mstrWarning = _T("The Learning Unit you are going to paste was related to the model :\n\t") + 
							  strModel + _T("\n");

		if (bFound && !bCopy)
		{
			mstrWarning += _T("which is already in this file.\n\n");
			mstrWarning += _T("The LU is now related to that model");
			pLU->m_nSimulID = bFound-1;
			AfxMessageBox(mstrWarning);
		}
		else if (bFound && bCopy)
		{
			mstrWarning += _T("A different instance of this model is present in the current file.\n\n");
			mstrWarning += _T("Do you want to add a new copy of the model ?\n\n");
			if (AfxMessageBox(mstrWarning,MB_YESNO)==IDYES)
			{
				int nb = pSim->m_cEquSet.GetSize();
				CMdEquation *pNewEq = pNewEqu->Clone();
				pNewEq->OnNewEquation(pSim);
				pSim->m_cEquSet.Add(pNewEq);
				nb = pSim->m_cEquSet.GetSize();
				pDoc->UpdateAllViews(this,DOC_UPDATE_MODEL,pNewEq);
				pLU->m_nSimulID = nb-1;
			}
			else
			{
				pLU->m_nSimulID = bFound-1;
			}
		}
		else //if (!bFound)
		{
			mstrWarning += _T("which is not present in this file.\n\n"
							  "The model is added to the file");

			int nb = pSim->m_cEquSet.GetSize();
			CMdEquation *pNewEq = pNewEqu->Clone();
			pNewEq->OnNewEquation(pSim);
			pSim->m_cEquSet.Add(pNewEq);
			nb = pSim->m_cEquSet.GetSize();
			pDoc->UpdateAllViews(this,DOC_UPDATE_MODEL,pNewEq);
			pLU->m_nSimulID = nb-1;
		}

/*		
		if (bFound)
		{
			if (!bCopy)
			{
				mstrWarning += _T("which is already in this file.\n\n");
			}
			else
			{
				mstrWarning += _T("A copy of this model is present in the current file.\n\n");
			}
			mstrWarning += _T("The LU is now related to that model");
			pLU->m_simulID = bFound-1;
		}
		else
		{
			mstrWarning += _T("which is not present in this file.\n\n"
							  "The model is added to the file");

			int nb = pSim->m_cEquSet.GetSize();
			CMdEquation *pNewEq = pNewEqu->Clone();
			pNewEq->OnNewEquation(pSim);
			pSim->m_cEquSet.Add(pNewEq);
			nb = pSim->m_cEquSet.GetSize();
			pDoc->UpdateAllViews(this,DOC_UPDATE_MODEL,pNewEq);
			pLU->m_simulID = nb-1;

		}*/

		pLU->m_sLUName = _T("Copy of ") + pLU->m_sLUName;
		pLU->m_pLUDoc = pDoc;
		pLU->m_pEquModel = NULL;
		pInst->AddLearningUnit(pLU);

		m_Tracker.m_rect = pLU->GetBoundRect();
		//MemDC.LPtoDP(&(m_Tracker.m_rect));

		//CSimModel *pSim = pDoc->m_pSimul;
		CMdEquation *pEqu = pSim->GetEquation(pLU->m_nSimulID);
		if (pEqu) pEqu->OnUpdate(/*pSim,*/pLU,TRUE);

		pDoc->SetModifiedFlag();
		pDoc->UpdateAllViews(this,DOC_UPDATE_LUNIT);
		Invalidate();
		UpdateWindow( );

	}



}


void CViewCDI::OnEditDelete() 
{
	if (!m_CurrUnit) return;

	CSimulDoc* pDoc = GetDocument();
	if (!pDoc) 	return;
	CInstModel *pInst = pDoc->m_pInstruct;
	if (!pInst)	return;
	CLearningUnit *pLU = pInst->m_cLUSet.GetAt(m_CurrUnit-1);
	if (!pLU) return;

	CString mstr;
	mstr.LoadString(AFX_IDS_DELETE_COMPONENT);
	if (MessageBox(mstr,NULL,MB_YESNO | MB_ICONQUESTION) != IDYES) return;


	if (m_CurrER > 1)
	{
		BOOL res = pLU->RemoveExternalRep(m_CurrER-1);
		//		OnERProperty() ;
		if (!res) return;
		pInst->OnUpdatePosition();
		pLU->UpdateERMenu(NULL);
		int h = 0,w=0;
		for (int i=0;i<pInst->m_cLUSet.GetSize();i++)
		{
			CLearningUnit *pLU2 = pInst->m_cLUSet.GetAt(i);
// NNN			pLU2->OnUpdateToolTip(this,&m_ToolTip);
			CRect mrect = pLU2->GetLURect();
			if (h < mrect.bottom) h = mrect.bottom;
			if (w < mrect.right) w = mrect.right;
		}
		m_Tracker.m_rect = pLU->GetBoundRect();
		/*	m_DocSize=CSize(w+50,300);
		m_DocSize=CSize(0,0);
		SetScrollSizes( MM_TEXT, m_DocSize);*/
		m_CurrER = 0;
	}
	else
	{
		int nb = pInst->m_cLUSet.GetSize();

		int nbMER = pLU->m_cMERSet.GetSize();
		for (int i = 1;i<nbMER;i++)
		{
			pLU->RemoveExternalRep(1);
		}
		pInst->m_cLUSet.RemoveAt(m_CurrUnit-1);
		delete pLU;

		if (nb == 1)
			pInst->OnNewModel();
		pInst->OnUpdatePosition();
		//pLU->UpdateERMenu(NULL);
	}
	if (pDoc->m_CurrInst == (m_CurrUnit-1))
	{
		pDoc->m_CurrInst =0;
	}
	pDoc->SetModifiedFlag();
	m_CurrUnit = m_CurrER = 0;
	Invalidate();
	UpdateWindow( );
}

void CViewCDI::OnSaveERPos() 
{
	// TODO: Add your command handler code here
	if (!m_CurrUnit) return;

	CSimulDoc* pDoc = GetDocument();
	if (!pDoc) 	return;
	CInstModel *pInst = pDoc->m_pInstruct;
	if (!pInst)	return;

	CLearningUnit *pLU = pInst->m_cLUSet.GetAt(m_CurrUnit-1);
	if (!pLU) return;

	for (int i=0;i<pLU->m_cMERSet.GetSize();i++)
	{
		CExternRepUnit *pER = pLU->m_cMERSet.GetAt(i);
		if (pER && pER->m_pExtView)
		{
			WINDOWPLACEMENT lpwndpl;
			lpwndpl.length = sizeof(WINDOWPLACEMENT);
			pER->m_pExtView->GetParentFrame()->GetWindowPlacement(&lpwndpl);
			pER->m_pExtViewPl = lpwndpl;
		}
	}
	
}

void CViewCDI::OnActivateNextLU() 
{
	// TODO: Add your command handler code here
	CSimulDoc* pDoc = GetDocument();
	BOOL bRes = pDoc->OnActivateNextLU(FALSE);
	if (bRes)
	{
		m_CurrUnit=m_CurrER=0;
		Invalidate();
		UpdateWindow();
	}
}

void CViewCDI::OnActivatePrevLU() 
{
	// TODO: Add your command handler code here
	CSimulDoc* pDoc = GetDocument();
	BOOL bRes = pDoc->OnActivatePrevLU(FALSE);
	if (bRes)
	{
		m_CurrUnit=m_CurrER=0;
		Invalidate();
		UpdateWindow();
	}
}

void CViewCDI::OnActivateCurrentLU() 
{
	// TODO: Add your command handler code here
	CSimulDoc* pDoc = GetDocument();

	BOOL bRes = pDoc->OnActivateCurrentLU();
	if (bRes)
	{
		m_CurrUnit=m_CurrER=0;
		Invalidate();
		UpdateWindow();
	}
	
}

void CViewCDI::OnUpdateLU(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	BOOL bEnab = FALSE;
	CSimulDoc* pDoc = GetDocument();
	int nb = pDoc->m_pInstruct->m_cLUSet.GetSize();
	CLearningUnit *pLU = pDoc->GetCurrentLU();

	switch (pCmdUI->m_nID){
	case ID_INSTRUCTION_NEXTLU:
		bEnab = (pDoc->m_CurrInst < (nb-1));
		break;
	case ID_INSTRUCTION_PREVLU:
		bEnab = (pDoc->m_CurrInst);
		break;
	case ID_INSTRUCTION_AUTHMODE:
		bEnab = (pLU && pLU->m_bIsReady);
		break;
	}
	pCmdUI->Enable(bEnab);
}

void CViewCDI::OnSize(UINT nType, int cx, int cy) 
{
	CScrollView::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	CSimulDoc* pDoc = GetDocument();
	if (!pDoc) 	return;
//	if (!m_CurrUnit) return;
	

	CInstModel *pInst = pDoc->m_pInstruct;
//	CLearningUnit *pLU = pInst->m_cLUSet.GetAt(m_CurrUnit-1);
//	if (!pLU) return;
	int h = 0,w=0;
	for (int i=0;i<pInst->m_cLUSet.GetSize();i++)
	{
		CLearningUnit *pLU2 = pInst->m_cLUSet.GetAt(i);
		CRect mrect = pLU2->GetLURect();
		if (h < mrect.bottom) h = mrect.bottom;
		if (w < mrect.right) w = mrect.right;

	}
	m_DocSize=CSize(w+50,h);
	//m_DocSize=CSize(0,0);
	SetScrollSizes( MM_TEXT, m_DocSize);
	
}

void CViewCDI::OnSortER() 
{
	// TODO: Add your command handler code here
	CSimulDoc* pDoc = GetDocument();


	CSortERDlg mdlg(this);

	mdlg.m_pMER = &(pDoc->GetCurrentLU()->m_cMERSet);
	mdlg.DoModal();

	pDoc->GetCurrentLU()->UpdateERMenu(NULL);

	CWnd *pWnd = AfxGetMainWnd();
	CMainFrame *pMainFrm = DYNAMIC_DOWNCAST( CMainFrame, pWnd);
	if (pMainFrm)
		pMainFrm->OnSetMERIcons(pDoc->GetCurrentLU());

	return;
	
}

void CViewCDI::OnUpdateSortER(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	BOOL bSel=FALSE;

	int nCLU = GetDocument()->m_CurrInst;

	switch(pCmdUI->m_nID) {
	case ID_INSTRUCTION_SORTER :
		bSel = (BOOL)(m_CurrUnit && m_CurrER <= 1);
		break;
	default :
		break;

	}
	pCmdUI->Enable(bSel);
	
}

LRESULT CViewCDI::OnUpdateObjTooltip(WPARAM wp, LPARAM lp)
{
	if (!wp)
	{
		m_wndToolTip.Activate(FALSE);
		return 0L;
	}

	CExternRepUnit *pER = (CExternRepUnit*)wp;

	CString strNb = pER->GetName();
	CString mstr;
	if (lp)
		mstr.Format(_T("%d - %s"),lp,strNb);
	else
		mstr.Format(_T("%s"),strNb);

	m_wndToolTip.UpdateTipText(mstr,this);
	if (!m_wndToolTip.IsWindowVisible())
	{
		CRect mmmm;
		mmmm.SetRectEmpty();
		m_wndToolTip.GetMargin(mmmm);
		m_wndToolTip.Activate(TRUE);
	}
	return 0L;
}


void CViewCDI::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CScrollView::OnMouseMove(nFlags, point);

	CSimulDoc* pDoc = GetDocument();
	if (!pDoc) 	return;

	CClientDC dc(this);
	OnPrepareDC(&dc); // set up mapping mode and viewport origin
	dc.DPtoLP(&point);

	CRect mrect;

	CPoint rep = pDoc->m_pInstruct->HitTest(point,mrect);

	CExternRepUnit *pER = NULL;
	int nb = 0;
	if (rep != CPoint(0,0))
	{
		//m_CurrUnit = rep.x;
		//m_CurrER = rep.y;
		if (rep.y != 0)
		{
			CInstModel *pInst = pDoc->m_pInstruct;
			CLearningUnit *pLU = pInst->m_cLUSet.GetAt(rep.x-1);
			pER = pLU->m_cMERSet.GetAt(rep.y-1);
			nb = rep.y-1;
		}
	}
	OnUpdateObjTooltip((WPARAM)pER,nb);
}
