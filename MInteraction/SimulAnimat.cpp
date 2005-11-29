// SimulAnimat.cpp : implementation file
//

#include "stdafx.h"
#include "Simul.h"
#include "SimulDoc.h"

#include "MainFrm.h"
#include "ChildFrm.h"

#include <MSimulation\Model.h>

//#include "MRTracker.h"
#include "Animation\AnimObject.h"
#include "Animation\AnimControl.h"
#include "Animation\AnimSelectDlg.h"

#include "GridCtrl\MemDC.h"
#include "SimulAnimat.h"

#include <Prefs\Pref.h>
#include "Format.h"
#include "LearnerTrace.h"

//#include "DlgTranslation.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define GRIDSIZE 1500
/////////////////////////////////////////////////////////////////////////////
// CViewAnimat

IMPLEMENT_DYNCREATE(CViewAnimat, CScrollView)

CViewAnimat::CViewAnimat()
{
	//m_nCurrTTask=0;
	m_pBuiltInSector = NULL;
	m_pER = NULL;
	m_bShowGrid = false;
	EnableToolTips(true);
	m_bFitToView = FALSE;

	m_currTime = 0;
	m_currExpSet = 0;
	m_bShowProp = FALSE;

	m_pSelectDlg = NULL;

	m_nPredDraw = 0;
}

CViewAnimat::~CViewAnimat()
{
	CFormatAnimation *pAnimPage = GetFormats();
	if (pAnimPage)
	{
		int nb = pAnimPage->m_cObjSet.GetSize();
		for (int i=0;i<nb;i++)
		{
			CAnimObject *pObj = pAnimPage->m_cObjSet.GetAt(i);
			if (!pObj) continue;
			pObj->OnClose();
		}
	}

	int nbPred = m_cPredObjSet.GetSize();
	for (int i=0;i<nbPred;i++)
	{
		CAnimObject *pObj = m_cPredObjSet.GetAt(i);
		delete pObj;
	}
	m_cPredObjSet.RemoveAll();

	m_cVisuObjSet.RemoveAll();

	if (m_pSelectDlg)
	{
		delete m_pSelectDlg;
		m_pSelectDlg = NULL;
	}
}

void CViewAnimat::OnDestroy() 
{
	CTrace::T_CLOSE(GetParentFrame());
	CScrollView::OnDestroy();
}

void CViewAnimat::Serialize(CArchive& ar) 
{
	if (ar.IsStoring())
	{	// storing code
	}
	else
	{	// loading code
	}
}


BEGIN_MESSAGE_MAP(CViewAnimat, CScrollView)
	//{{AFX_MSG_MAP(CViewAnimat)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_SETCURSOR()
	ON_UPDATE_COMMAND_UI(ID_ANIMATION_SHOWGRID, OnUpdateHSGrid)
	ON_COMMAND(ID_ANIMATION_SHOWGRID, OnHideShowGrid)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, OnUpdateEditCmd)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_COMMAND(ID_EDIT_CLEAR, OnEditDelete)
	ON_COMMAND(ID_EDIT_SELECTALL, OnEditSelectAll)
	ON_UPDATE_COMMAND_UI(ID_ANIMATION_FORWARD, OnUpdateComponent)
	ON_COMMAND(ID_ANIMATION_NEXTEXPSET, OnNextExpSet)
	ON_COMMAND(ID_ANIMATION_PREVEXPSET, OnPrevExpSet)
	ON_COMMAND(ID_ANIMATION_FORWARD, OnSendForward)
	ON_COMMAND(ID_ANIMATION_BACKWARD, OnSendBackward)
	ON_UPDATE_COMMAND_UI(ID_ANIMATION_ADDSECTOR, OnUpdateAddInSector)
	ON_COMMAND(ID_ANIMATION_ADDSECTOR, OnAddInSector)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_ANIM_ALIGN, OnShowAlignTool)
	ON_UPDATE_COMMAND_UI(ID_ANIM_ALIGN, OnUpdateAlignTool)
	ON_COMMAND(ID_ANIM_CONSTRUCTION, OnShowComponentTool)
	ON_UPDATE_COMMAND_UI(ID_ANIM_CONSTRUCTION, OnUpdateComponentTool)
	ON_WM_KILLFOCUS()
	ON_WM_KEYDOWN()
	ON_WM_SETFOCUS()
	ON_WM_DESTROY()
	ON_UPDATE_COMMAND_UI(ID_EDIT_CLEAR, OnUpdateEditCmd)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCmd)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditCmd)
	ON_UPDATE_COMMAND_UI(ID_ANIMATION_BACKWARD, OnUpdateComponent)
	ON_COMMAND(ID_ANIMATION_SHOWPROPERTIES, OnShowProperties)
	ON_UPDATE_COMMAND_UI(ID_ANIMATION_SHOWPROPERTIES, OnUpdateShowProperties)
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(ID_ANIMATION_TEXT,ID_ANIMATION_SECTOR, OnNewComponent)
	ON_COMMAND_RANGE(ID_ANIMATION_CATER,ID_ANIMATION_CATER, OnNewComponent)
	ON_COMMAND_RANGE(ID_ANIMATION_POLYGON,ID_ANIMATION_POLYGON, OnNewComponent)
	ON_COMMAND_RANGE(ID_ANIM_ALIGNLEFT,ID_ANIM_ALIGNVER, OnSetAlignment)
	ON_MESSAGE(TRACE_VIEW_ACTIVATE, OnActivateER)
	ON_UPDATE_COMMAND_UI_RANGE(ID_ANIM_ALIGNLEFT,ID_ANIM_ALIGNVER, OnUpdateSetAlignment)
	ON_UPDATE_COMMAND_UI_RANGE(ID_ANIMATION_TEXT,ID_ANIMATION_SECTOR, OnUpdateComponent)
	ON_UPDATE_COMMAND_UI_RANGE(ID_ANIMATION_CATER,ID_ANIMATION_CATER, OnUpdateComponent)
	ON_UPDATE_COMMAND_UI_RANGE(ID_ANIMATION_POLYGON,ID_ANIMATION_POLYGON, OnUpdateComponent)
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
	ON_MESSAGE(DOC_UPDATE_VIEWCONTENT, OnUpdateInitContent)
	ON_CBN_SELCHANGE(ID_ANIMATION_SELECTEXPSET, OnSelChangeExpSet)
	//ON_COMMAND_RANGE(ID_TRANS_INDEPENDENT,ID_TRANS_DYNALINKED,OnTranslationTasks)
	ON_UPDATE_COMMAND_UI_RANGE(ID_TRANS_INDEPENDENT,ID_TRANS_DYNALINKED,OnUpdateTransTasks)
	ON_COMMAND(ID_ANIMATION_SELECTOBJECT,OnSelectComponent)
	ON_UPDATE_COMMAND_UI(ID_ANIMATION_SELECTOBJECT, OnUpdateSelectComponent)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewAnimat Create and Initialise

BOOL CViewAnimat::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	return CScrollView::PreCreateWindow(cs);
}

BOOL CViewAnimat::CreateStyleBar()
{
	const int nDropHeight = 100;

		
	if (!m_wndAnimBar.CreateEx(GetParent(), TBSTYLE_FLAT | TBSTYLE_TOOLTIPS, 
			WS_CHILD | WS_VISIBLE | CBRS_TOP | 
			CBRS_TOOLTIPS | CBRS_FLYBY | CCS_ADJUSTABLE, // | CBRS_GRIPPER,
			CRect(0, 0, 0, 0), 120) ||
		!m_wndAnimBar.LoadToolBar(IDR_SIMULANIM))
	{
		TRACE0("Failed to create stylebar\n");
		return FALSE;       // fail to create
	}

	// Create the combo box
	m_wndAnimBar.SetButtonInfo(1, ID_ANIMATION_SELECTEXPSET, TBBS_SEPARATOR, 100);

	// Design guide advises 12 pixel gap between combos and buttons
//	m_wndAnimBar.SetButtonInfo(1, ID_SEPARATOR, TBBS_SEPARATOR, 12);
	CRect rect;
	m_wndAnimBar.GetItemRect(1, &rect);
	rect.top = 3;
	rect.bottom = rect.top + nDropHeight;
	if (!m_wndAnimBar.m_comboBox.Create(
			CBS_DROPDOWNLIST|WS_VISIBLE|WS_TABSTOP,
			rect, &m_wndAnimBar, ID_ANIMATION_SELECTEXPSET))
	{
		TRACE0("Failed to create combo-box\n");
		return FALSE;
	}

	//  Fill the combo box
/*	CString szStyle;
	if (szStyle.LoadString(ID_VIEW_ANIM))
		m_wndAnimBar.m_comboBox.AddString((LPCTSTR)szStyle);
	if (szStyle.LoadString(ID_VIEW_TIMER))
		m_wndAnimBar.m_comboBox.AddString((LPCTSTR)szStyle);
	if (szStyle.LoadString(ID_VIEW_XYGRAPH))
		m_wndAnimBar.m_comboBox.AddString((LPCTSTR)szStyle);
	if (szStyle.LoadString(ID_VIEW_DESIGN_SIMULATION))
		m_wndAnimBar.m_comboBox.AddString((LPCTSTR)szStyle);*/

	//  Create a font for the combobox
	LOGFONT logFont;
	memset(&logFont, 0, sizeof(logFont));

	if (!::GetSystemMetrics(SM_DBCSENABLED))
	{
		// Since design guide says toolbars are fixed height so is the font.
		logFont.lfHeight = -11;
		logFont.lfWeight = FW_NORMAL;
		logFont.lfPitchAndFamily = VARIABLE_PITCH | FF_SWISS;
		CString strDefaultFont;
//		strDefaultFont.LoadString(IDS_DEFAULT_FONT);
		strDefaultFont = _T("MS Sans Serif");
		lstrcpy(logFont.lfFaceName, strDefaultFont);
		if (!m_wndAnimBar.m_font.CreateFontIndirect(&logFont))
			TRACE0("Could Not create font for combo\n");
		else
			m_wndAnimBar.m_comboBox.SetFont(&m_wndAnimBar.m_font);
	}
	else
	{
		m_wndAnimBar.m_font.Attach(::GetStockObject(SYSTEM_FONT));
		m_wndAnimBar.m_comboBox.SetFont(&m_wndAnimBar.m_font);
	}

	return TRUE;

}


int CViewAnimat::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CScrollView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// ****** Add your code below this line ********** //
/*	if (!m_ToolTip.Create(this))//, TTS_ALWAYSTIP))
	{
		TRACE0("Failed to create ToolTip\n");
		return -1;      // fail to create  
	}
	m_ToolTip.SetDelayTime(TTDT_AUTOPOP,1000);//SHRT_MAX);
	m_ToolTip.SetDelayTime(TTDT_INITIAL,200);
	m_ToolTip.SetDelayTime(TTDT_RESHOW,2);
	m_ToolTip.SetMaxTipWidth(SHRT_MAX);*/
	
	if (!CreateStyleBar())
	{
		return -1;
	}

	if (!CPref::g_nMode)
	{
		CMDIChildWnd *pMDICh = DYNAMIC_DOWNCAST(CMDIChildWnd,GetParent());
		if (!m_wndToolBar.CreateEx(GetParent(), TBSTYLE_FLAT | TBSTYLE_TOOLTIPS, 
				WS_CHILD | WS_VISIBLE | CBRS_LEFT | 
				CBRS_TOOLTIPS | CBRS_FLYBY | CCS_ADJUSTABLE| CBRS_GRIPPER | CBRS_SIZE_DYNAMIC,
				CRect(0, 0, 0, 0), 120) ||
			!m_wndToolBar.LoadToolBar(IDR_SIMULANIM_CPNT))
		{
			TRACE0("Failed to create ToolBar\n");
			return -1;      // fail to create  
		}
		if (pMDICh)
		{
			m_wndToolBar.EnableDocking(CBRS_ALIGN_LEFT);
			pMDICh->EnableDocking(CBRS_ALIGN_LEFT);
			pMDICh->DockControlBar(&m_wndToolBar);
			CString mstr;
			mstr = _T("Components");
			m_wndToolBar.SetWindowText(mstr);

		}
	
		if (!m_wndAlignBar.CreateEx(GetParent(), TBSTYLE_FLAT | TBSTYLE_TOOLTIPS, 
			WS_CHILD | WS_VISIBLE | CBRS_RIGHT | 
			CBRS_TOOLTIPS | CBRS_FLYBY | CCS_ADJUSTABLE | CBRS_GRIPPER | CBRS_SIZE_DYNAMIC,
			CRect(0, 0, 0, 0), 130) ||
			!m_wndAlignBar.LoadToolBar(IDR_SIMULANIM_ALIGN))
		{
			TRACE0("Failed to create Align Tool Bar\n");
			return -1;      // fail to create  
		}
		if (pMDICh)
		{
			m_wndAlignBar.EnableDocking(CBRS_ALIGN_RIGHT);
			pMDICh->EnableDocking(CBRS_ALIGN_RIGHT);
			pMDICh->DockControlBar(&m_wndAlignBar);
			CString mstr;
			mstr = _T("Alignment");
			m_wndAlignBar.SetWindowText(mstr);
		}
		//EnableDocking(CBRS_ALIGN_ANY);
		//DockControlBar(&m_wndToolBar);
	}

	//m_wndToolBar.SetBarStyle(CBRS_ALIGN_LEFT | CBRS_TOOLTIPS | CBRS_FLYBY);
	//m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	//GetParentFrame()->RecalcLayout();

	// ****** Add your code above this line ********** //
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CViewAnimat diagnostics

#ifdef _DEBUG
void CViewAnimat::AssertValid() const
{
	CScrollView::AssertValid();
}

void CViewAnimat::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}


CSimulDoc* CViewAnimat::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSimulDoc)));
	return (CSimulDoc*)m_pDocument;
}
#endif //_DEBUG

CFormatAnimation* CViewAnimat::GetFormats()
{
	CFormatAnimation* pDlg = NULL;

	if (!m_pER) return pDlg;
	int nbF = m_pER->m_cFormatSet.GetSize();
	if (nbF)
	{
		CFormat* pPage = m_pER->m_cFormatSet.GetAt(0);
		pDlg = DYNAMIC_DOWNCAST(CFormatAnimation,pPage);
	}

	return pDlg;
}

int	CViewAnimat::GetTranslation()
{
	int nTrans = 0;
	CFormatTranslation* pDlg = NULL;

	if (!m_pER) return nTrans;
	int nbF = m_pER->m_cFormatSet.GetSize();
	if (nbF>=2)
	{
		CFormat *pPage = m_pER->m_cFormatSet.GetAt(1);
		pDlg = DYNAMIC_DOWNCAST(CFormatTranslation,pPage);
	}
	if (pDlg) 
		nTrans = pDlg->m_nTranslation;
	return nTrans;
}

/////////////////////////////////////////////////////////////////////////////
// CViewTable Printing handlers

BOOL CViewAnimat::OnPreparePrinting(CPrintInfo* pInfo) 
{
	// TODO: call DoPreparePrinting to invoke the Print dialog box
	return DoPreparePrinting(pInfo);
	//return CScrollView::OnPreparePrinting(pInfo);
}

void CViewAnimat::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo) 
{
	// TODO: Add your specialized code here and/or call the base class
	CScrollView::OnEndPrinting(pDC, pInfo);
}

void CViewAnimat::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CScrollView::OnBeginPrinting(pDC, pInfo);
}

void CViewAnimat::OnPrint(CDC* pDC, CPrintInfo* pInfo) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	// Get a DC for the current window (will be a screen DC for print previewing)
	CDC *pCurrentDC = GetDC();        // will have dimensions of the client area
	if (!pCurrentDC) return;
	
	CSize PaperPixelsPerInch(pDC->GetDeviceCaps(LOGPIXELSX), pDC->GetDeviceCaps(LOGPIXELSY));
	CSize ScreenPixelsPerInch(pCurrentDC->GetDeviceCaps(LOGPIXELSX), pCurrentDC->GetDeviceCaps(LOGPIXELSY));
	CSize m_CharSize = pDC->GetTextExtent(_T("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSATUVWXYZ"),52);
	m_CharSize.cx /= 52;
	
	// Get the page sizes (physical and logical)
	CSize m_PaperSize = CSize(pDC->GetDeviceCaps(HORZRES), pDC->GetDeviceCaps(VERTRES));
	CSize m_LogicalPageSize;
	m_LogicalPageSize.cx = ScreenPixelsPerInch.cx * m_PaperSize.cx / PaperPixelsPerInch.cx * 3 / 3;
	m_LogicalPageSize.cy = ScreenPixelsPerInch.cy * m_PaperSize.cy / PaperPixelsPerInch.cy * 3 / 3;

	pDC->SetMapMode(MM_ISOTROPIC);
	//pDC->SetWindowExt(CSize(1200,600));//m_DocSize);
	pDC->SetWindowExt(m_LogicalPageSize);
	pDC->SetViewportExt(pInfo->m_rectDraw.Width(),pInfo->m_rectDraw.Height());

	//COLORREF bcolor = ::GetSysColor(COLOR_WINDOW/*COLOR_3DFACE*/); 
	COLORREF bcolor = RGB(255,255,255);
	CBrush FixedBack(bcolor);
	CRect ClipRect;
	if (pDC->GetClipBox(ClipRect) != ERROR)
	{
		ClipRect.InflateRect(1, 1); // avoid rounding to nothing
		pDC->FillRect(ClipRect,&FixedBack);
	}

	PaintAnim(pDC);
}

/////////////////////////////////////////////////////////////////////////////
// CViewAnimat Drawing

void CViewAnimat::PaintAnim(CDC* pDC)
{
    //CDemoDoc* pDoc = GetDocument();
	//ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here

	if (!m_pER) return;
	int nbF = m_pER->m_cFormatSet.GetSize();
	if (nbF)
	{
		//C *pPage = m_pER->m_cFormatSet.GetAt(0);
		//CDlgFormatAnimation *pAnimPage = DYNAMIC_DOWNCAST( CDlgFormatAnimation, pPage );
		CFormatAnimation *pAnimPage = GetFormats();
		if (pAnimPage)
		{
			int nb = pAnimPage->m_cObjSet.GetSize();
			//nb = m_cVisObjSet.GetSize();
			for (int i=0;i<nb;i++)
			{
				CAnimObject *pObj = pAnimPage->m_cObjSet.GetAt(i);
				//CAnimObject *pObj = m_cVisObjSet.GetAt(i);
				if (pObj)
					pObj->Draw(pDC);
			}
				
		}
	}
	int nbPred = m_cPredObjSet.GetSize();
	if (nbPred)
	{
		CRect mrect;
		GetClientRect(mrect);
		//m_sDrawSize ;

		CRect rHyp = mrect;

		//m_nPredDraw = (m_sDrawSize.cx >= m_sDrawSize.cy);
		if (m_nPredDraw)
		{
			pDC->MoveTo(mrect.left,m_sDrawSize.cy + 40);
			pDC->LineTo(max(mrect.right,m_sDrawSize.cx),m_sDrawSize.cy + 40);
			rHyp.top = m_sDrawSize.cy  + 41;
			rHyp.right = max(rHyp.right,m_sDrawSize.cx);
			rHyp.bottom = max(rHyp.top+m_sDrawSize.cy ,rHyp.bottom);
		}
		else
		{
			pDC->MoveTo(m_sDrawSize.cx  + 40,mrect.top);
			pDC->LineTo(m_sDrawSize.cx  + 40,mrect.bottom);
			rHyp.left = m_sDrawSize.cx  + 41;
			rHyp.right = max(rHyp.left+m_sDrawSize.cx ,rHyp.right);
		}
		CBrush mBrush(CPref::g_crFuture);//RGB(235,235,235));
		pDC->FillRect(rHyp,&mBrush);

		//mrect.right = m_sDrawSize.cx + 20
		int  nTime = 0;
		for (int i=0;i<nbPred;i++)
		{
			CAnimObject *pObj = m_cPredObjSet.GetAt(i);
			if (!pObj) continue;
			pObj->Draw(pDC);
			nTime = pObj->m_currTime;
		}

		rHyp.InflateRect(0,0,0,-4);
		CString mstr;
		mstr.Format(_T("Hypothesis (T = %d)"),nTime);

		CFont titleFont;
		titleFont.CreateFont(-10, 0, 0, 0, FW_ULTRABOLD, FALSE, FALSE, 0,
			ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN,"Arial");
		CFont* pOldFont = (CFont*) pDC->SelectObject(&titleFont);
		int oldBk = pDC->SetBkMode(TRANSPARENT);
		pDC->DrawText(mstr,rHyp,DT_SINGLELINE|DT_BOTTOM|DT_CENTER);
		pDC->SetBkMode(oldBk);
		pDC->SelectObject(pOldFont);
	}
}

void CViewAnimat::OnDraw(CDC* pDC)
{
	//CDocument* pDoc = GetDocument();
	// TODO: add draw code here
    CMemDC MemDC(pDC);
	CRect  ClipRect;

	COLORREF mClr = RGB(255,255,255);
	int nRT = GetDocument()->GetRunTime();
	if (m_currTime!=nRT)
		mClr = CPref::g_crPast;//RGB(0xFF, 0xFF, 0xE0);

	CBrush br(mClr);//::GetSysColor(COLOR_WINDOW));
	MemDC.GetClipBox(ClipRect);
	MemDC.FillRect(ClipRect,&br);
	if (m_bShowGrid)
	{
		CPen gridPen(PS_SOLID,1,RGB(192,192,192));
		CPen *oldPen = MemDC.SelectObject(&gridPen);

		CRect m_rectArea ;
		GetClientRect( m_rectArea );
		int xmax = GRIDSIZE;//m_rectArea.Width();
		int ymax = GRIDSIZE;//m_rectArea.Height();

		for (int x=0;x<xmax;x+=10)
		{
			MemDC.MoveTo(x,0);
			MemDC.LineTo(x,ymax);
		}
		for (int y=0;y<ymax;y+=10)
		{
			MemDC.MoveTo(0,y);
			MemDC.LineTo(ymax,y);
		}
		MemDC.SelectObject(oldPen);
	}

	PaintAnim(&MemDC);
	// Don't forget the multitrack
    m_wndMultiTrack.Draw(&MemDC);

	if (m_pBuiltInSector)
	{
		LOGBRUSH pLogBrush;

		pLogBrush.lbStyle = BS_HATCHED;
		pLogBrush.lbColor = RGB(128,128,128);
		pLogBrush.lbHatch = HS_DIAGCROSS;

		CBrush pBrush;
		pBrush.CreateBrushIndirect(&pLogBrush);
		CRgn rRgn1,rRgn2;
		CRgn resRgn;

		CRect rect1 = ClipRect;
		CRect rect2 = m_pBuiltInSector->GetRect();
		rect2.InflateRect(1,1,6,6);

		resRgn.CreateRectRgn(0,0,1,1);
		BOOL b1 = rRgn1.CreateRectRgn(rect1.left,rect1.top,rect1.right,rect1.bottom);
		BOOL b2 = rRgn2.CreateRectRgn(rect2.left,rect2.top,rect2.right,rect2.bottom);

		int nt = resRgn.CombineRgn(&rRgn1,&rRgn2,RGN_DIFF);
		int mode = MemDC.SetBkMode(TRANSPARENT);
		MemDC.FillRgn(&resRgn,&pBrush);
		MemDC.SetBkMode(mode);
	}

}

BOOL CViewAnimat::OnEraseBkgnd(CDC* pDC) 
{
	//return CScrollView::OnEraseBkgnd(pDC);
	return false;
}


/////////////////////////////////////////////////////////////////////////////
// CViewAnimat message handlers

BOOL CViewAnimat::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if ((IsViewActive() && !CPref::g_nMode) && 
		(pMsg->message== WM_LBUTTONDOWN || pMsg->message== WM_LBUTTONUP || pMsg->message== WM_MOUSEMOVE))
			m_wndToolTip.RelayEvent(pMsg);

/*	if (IsViewActive() && !CPref::g_nMode)
		m_ToolTip.RelayEvent(pMsg);*/
	return CScrollView::PreTranslateMessage(pMsg);
}

/////////////////////////////////////////////////////////////////////////////
/// The framework calls this function after receiving a DOC_UPDATE_VIEWCONTENT notification.
///
/// Called just before the view is displayed for the first time, this function just
/// assign to the data member m_pER a pointer to the Learning Unit (see CLearningUnit)
/// associated with that view.
/////////////////////////////////////////////////////////////////////////////
LRESULT CViewAnimat::OnUpdateInitContent(WPARAM wp, LPARAM lp)
{
	m_pER = (CExternRepUnit*)lp;
	return 0;
}

LRESULT CViewAnimat::OnActivateER(WPARAM wp, LPARAM lp)
{
	static BOOL bFirst = TRUE;

	if (!bFirst) CTrace::T_SWITCH(this, (CWnd*)wp);
	bFirst = FALSE;
	return 0;
}


LRESULT CViewAnimat::OnUpdateObjTooltip(WPARAM wp, LPARAM lp)
{
	if (!lp || !wp)
	{
		m_wndToolTip.Activate(FALSE);
		return 0L;
	}

	CAnimObject *pObj = (CAnimObject*)wp;

	CString mstr = pObj->GetObjectDef();

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


void CViewAnimat::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	m_wndToolTip.Create(this);
	m_wndToolTip.AddTool(this);
	m_wndToolTip.SetDelayTime(TTDT_INITIAL,200);
	m_wndToolTip.SetDelayTime(TTDT_RESHOW,100);
	m_wndToolTip.SetDelayTime(TTDT_AUTOPOP,1000);//SHRT_MAX);
	m_wndToolTip.SetDelayTime(TTDT_RESHOW,2);
	m_wndToolTip.SetMaxTipWidth(SHRT_MAX);	
	
	if (m_pER)
		GetParentFrame()->SetWindowText(m_pER->GetName());

	CTrace::T_OPEN(GetParentFrame());

	CSimulDoc *pDoc = GetDocument();

	m_bDrawPred = FALSE;
	m_bHidePred = FALSE;
	m_pObjDblClick = NULL;

	m_wndMultiTrack.SetTrackerView(this);

	CSize sizeTotal;
	// TODO: calculate the total size of this view
	sizeTotal.cx = sizeTotal.cy = GRIDSIZE;
	//SetScrollSizes(MM_TEXT, sizeTotal);
	m_sDrawSize = CSize(0,0);
	SetScrollSizes(MM_TEXT,m_sDrawSize );

	if (!m_pER) return;
	int nbF = m_pER->m_cFormatSet.GetSize();
	if (nbF)
	{
		//CPropertyPage *pPage = m_pER->m_cFormatSet.GetAt(0);
		//CDlgFormatAnimation *pAnimPage = DYNAMIC_DOWNCAST( CDlgFormatAnimation, pPage );

		CFormatAnimation *pAnimPage = GetFormats();
		if (pAnimPage)
		{
			m_nPredDraw = pAnimPage->m_nAnimCat;
			m_bFitToView = pAnimPage->m_bFitToView;
			int nb = pAnimPage->m_cObjSet.GetSize();
			/*if (!nb)
			{
				pAnimPage->m_cObjSet.Add(new CTextObject(1,CRect(40,20,300,40), 
								"NewPreys = PreyIncrease - PreyEaten"));
				pAnimPage->m_cObjSet.Add(new CValueObject(2,CRect(40,40,250,60), 8));
				pAnimPage->m_cObjSet.Add(new CValueObject(3,CRect(130,40,160,60), 5));
				pAnimPage->m_cObjSet.Add(new CValueObject(4,CRect(220,40,250,60), 4));
				pAnimPage->m_cObjSet.Add(new CDensityObject(155,CRect(400,100,600,250)));
				pAnimPage->m_cObjSet.Add(new CPictObject(154,CRect(300,200,350,250), IDB_BOOKS));

			}
			nb = pAnimPage->m_cObjSet.GetSize();*/
			CRect mExtRect(0,0,0,0);
			mExtRect.NormalizeRect();
			//m_cVisObjSet.RemoveAll();
			for (int i=0;i<nb;i++)
			{
				CAnimObject *pObj = pAnimPage->m_cObjSet.GetAt(i);
				if (!pObj) continue;
				pObj->SetObjectDoc(pDoc);
				pObj->OnInitialUpdate();
				CRect objRect = pObj->GetRect();
/*				m_ToolTip.AddTool(this,
					pObj->GetObjectDef(),
					objRect,
					pObj->m_ObjectID);*/
				objRect.NormalizeRect();
				mExtRect |= objRect;

				//if (pObj->m_nDepth == -1)
				//	m_cVisObjSet.Add(pObj);
				//else
				//	m_cVisObjSet.InsertAt(pObj->m_nDepth,pObj);
			}
			CSize mSize(mExtRect.right+10,mExtRect.bottom+10);
			if (!m_bFitToView)
			{
				m_sDrawSize = mSize;
				//m_nPredDraw = !(m_sDrawSize.cx >= m_sDrawSize.cy);
				SetScrollSizes(MM_TEXT,mSize);
			}
		}
	}

	CLearningUnit *pLU = pDoc->GetCurrentLU();
	if (!pLU) return;

	int nbExpSet = pLU->m_cExpSet.GetSize();
	m_wndAnimBar.m_comboBox.ResetContent();
	for (int k = 0;k<nbExpSet;k++)
	{
		CMdExpertSet *PeXP = pLU->m_cExpSet.GetAt(k);
		if (PeXP)
		{
			CString mstr = PeXP->GetName();
			m_wndAnimBar.m_comboBox.AddString((LPCTSTR)mstr);
		}

	}
	if (m_currExpSet >= nbExpSet)
		OnNextExpSet();
	m_wndAnimBar.m_comboBox.SetCurSel(m_currExpSet);

	int nbPred = m_cPredObjSet.GetSize();
	if (!nbPred)
	{
		POSITION pos = pDoc->m_cUserInput.GetStartPosition();
		while (pos)
		{
			CUserOutput pUser;
			int nb;
			pDoc->m_cUserInput.GetNextAssoc(pos,nb,pUser);
			POSITION pos2 = pUser.m_lUserInput.GetStartPosition();
			while (pos2)
			{
				CUserData pUserData;
				unsigned long ptime;
				pUser.m_lUserInput.GetNextAssoc(pos2,ptime,pUserData);
				if (pUserData.m_nType == BREAKP_TEST)
				{
					OnUpdateHypothesis(&pUserData);
				}
			}
		}
	}


	//OnUpdate(0,DOC_UPDATE_TIMERDATA,0);
	//GetDocument()->UpdateTimerDoc(this);
	//OnUpdate(NULL,0,NULL);
		OnUpdateTimer();
		OnUpdateData();
		Invalidate();
		UpdateWindow();

	CWnd *pWnd = GetParent();
	CRect mrect;
	if (pWnd)
	{
		pWnd->GetWindowRect(mrect);
		pWnd->SetWindowPos(NULL,0,0,mrect.Width()-1,mrect.Height(),SWP_NOZORDER|SWP_NOMOVE);
	}

}


/////////////////////////////////////////////////////////////////////////////
/// The framework calls this function after receiving a DOC_UPDATE_ALLDATA notification.
///
/// Response to data update
/////////////////////////////////////////////////////////////////////////////
BOOL CViewAnimat::OnUpdateData()
{
	CSimulDoc *pDoc = GetDocument();
	//m_currTime = pDoc->m_currTimer;

	CFormatAnimation *pAnimPage = GetFormats();
	if (!pAnimPage) return FALSE;

	int nb = pAnimPage->m_cObjSet.GetSize();
	for (int i=0;i<nb;i++)
	{
		CAnimObject *pObj = pAnimPage->m_cObjSet.GetAt(i);
		if (!pObj) continue;
		pObj->SetCurrTime(m_currTime);
		pObj->OnUpdate();
	}

//	int nbPred = m_cPredObjSet.GetSize();
//	for (i=0;i<nbPred;i++)
//	{
//		CAnimObject *pObj = m_cPredObjSet.GetAt(i);
//		if (!pObj) continue;
//		pObj->SetCurrTime(m_currTime);
//		pObj->OnUpdate();
//	}

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
/// The framework calls this function after receiving a DOC_UPDATE_TIMERDATA notification.
///
/// Response to data update
/////////////////////////////////////////////////////////////////////////////
BOOL CViewAnimat::OnUpdateTimer()
{
	CSimulDoc *pDoc = GetDocument();
/*	if (m_bHidePred && (pDoc->m_nRunMode==RUN_RUN))
	{	
		int nbPred = m_cPredObjSet.GetSize();
		for (int i=0;i<nbPred;i++)
		{
			CAnimObject *pObj = m_cPredObjSet.GetAt(i);
			delete pObj;
		}
		m_cPredObjSet.RemoveAll();
	}*/

	//m_currTime = pDoc->m_currTimer;
	m_currTime = pDoc->GetCurrTime();

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
/// The framework calls this function after receiving a DOC_UPDATE_BREAKPOINT notification.
///
/// Response to data update
/////////////////////////////////////////////////////////////////////////////
BOOL CViewAnimat::OnUpdateBreakPoint(int nBPTime,int nBPType)
{
	if (nBPType==BREAKP_HYPOT)
	{
		m_bDrawPred = TRUE;
		m_bHidePred = FALSE;
	}
	if (nBPType==BREAKP_TEST)
	{
		m_bDrawPred = TRUE;
		m_bHidePred = TRUE;
	}
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
/// The framework calls this function after receiving a TRANSLATION_ACTION notification.
///
/// Response to data update
/////////////////////////////////////////////////////////////////////////////
BOOL CViewAnimat::OnUpdateAction(CUserData *pData)
{
	int nbTrans = GetTranslation();
	BOOL bTrans = (nbTrans<TRANS_DYNAL);

	if (!pData) return FALSE;
	
	if (pData->m_bUpdateNow)
		OnUpdateData();
	else if (!bTrans)
		OnUpdateData();

	//OnUpdateData();
	//if (pData)
	//	FormatCells(pData->m_nTime,TRUE);
//	OnUpdateData();
	//OnUpdateTimer();
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
/// The framework calls this function after receiving a TRANSLATION_HYPOTHESIS notification.
///
/// Response to data update
/////////////////////////////////////////////////////////////////////////////
BOOL CViewAnimat::OnUpdateHypothesis(CUserData *pData)
{
	int nbTrans = GetTranslation();
	if (nbTrans<TRANS_DYNAL) return FALSE;
	
	int nbF = m_pER->m_cFormatSet.GetSize();
	if (!nbF) return FALSE;

	CFormatAnimation *pAnimPage = GetFormats();
	if (!pAnimPage) return FALSE;

	int nbPred = m_cPredObjSet.GetSize();

//	for (int i=0;i<nbPred;i++)
//	{
//		CAnimObject *pObj = m_cPredObjSet.GetAt(i);
//		delete pObj;
//	}
//	m_cPredObjSet.RemoveAll();
	int nb = pAnimPage->m_cObjSet.GetSize();
	if (!nbPred)
	{
		for (int i=0;i<nb;i++)
		{
			CAnimObject *pObj = pAnimPage->m_cObjSet.GetAt(i);
			//CAnimObject *pObj = m_cVisObjSet.GetAt(i);
			if (!pObj) continue;
			CAnimObject *pCopyObj = pObj->Clone();

			pCopyObj->OnUpdateHypothesis(NULL);

			CRect mrect = pCopyObj->GetRect();
			//m_nPredDraw = (m_sDrawSize.cx >= m_sDrawSize.cy);
			if (m_nPredDraw)
				mrect.OffsetRect(0,m_sDrawSize.cy + 40);
			else
				mrect.OffsetRect(m_sDrawSize.cx + 40,0);
			pCopyObj->SetRect(mrect);

			m_cPredObjSet.Add(pCopyObj);
		}
	}
	for (int i=0;i<	m_cPredObjSet.GetSize();i++)
	{
		CAnimObject *pObj = m_cPredObjSet.GetAt(i);
		if (!pObj) continue;
		pObj->OnUpdateHypothesis(pData);
		pObj->m_currTime = pData->m_nTime;
	}

	OnUpdate(0,VIEW_UPDATE_ANIMTOOLTIP,0);


	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
/// The framework calls this function after receiving a TRANSLATION_MAPRELATION notification.
///
/// Response to data update
/////////////////////////////////////////////////////////////////////////////
BOOL CViewAnimat::OnUpdateMapRelation(CTranslationInfo *mTInfo )
{
	//CTranslationInfo *mTInfo = (CTranslationInfo *)pHint;
	int nbTrans = GetTranslation();
	if (nbTrans<TRANS_MAPRELAT) return FALSE;

	int nTranLevel = GetTranslation();
	int nTime = -1;
	int nExpSet = -1;
	int nData = -1;
	int nTLevel = -1;

	CFormatAnimation *pAnimPage = GetFormats();
	if (!pAnimPage) return FALSE;

	int nb = pAnimPage->m_cObjSet.GetSize();

	for (int i=0;i<nb;i++)
	{
		CAnimObject *pObj = pAnimPage->m_cObjSet.GetAt(i);
		if (!pObj) continue;
		//pObj->SetExpSet(m_currExpSet);
		//pObj->SetCurrTime(m_currTime);
		//pObj->OnUpdate();
		pObj->OnUpdateSelection(-1);
	}

	if (mTInfo)
	{
		m_currTime = mTInfo->m_nTime;
		int nbD = mTInfo->m_nDatas.GetSize();
		if (!nbD)
		{
			for (int i=0;i<nb;i++)
			{
				CAnimObject *pObj = pAnimPage->m_cObjSet.GetAt(i);
				if (!pObj) continue;
				//pObj->SetExpSet(m_currExpSet);
				pObj->SetCurrTime(m_currTime);
				pObj->OnUpdate();
				//pObj->OnUpdateSelection(nData);
			}
		}
		else for (int jj=0;jj<nbD;jj++)
		{
			CTranslationInfo::CTInfo tt1 = mTInfo->m_nDatas.GetAt(jj);
			nTime = tt1.nTime;
			nExpSet = tt1.nExpSet;
			nData = tt1.nData;
			if (nTime!=-1 || nExpSet!=-1 || nData!=-1)
			{
				m_currTime = nTime;
				if (nExpSet != -1) m_currExpSet = nExpSet;
				m_wndAnimBar.m_comboBox.SetCurSel(m_currExpSet);
			}
			for (int i=0;i<nb;i++)
			{
				CAnimObject *pObj = pAnimPage->m_cObjSet.GetAt(i);
				if (!pObj) continue;
				pObj->SetExpSet(m_currExpSet);
				pObj->SetCurrTime(m_currTime);
				pObj->OnUpdate();
				pObj->OnUpdateSelection(nData);
			}
			int nbPred = m_cPredObjSet.GetSize();
			for (i=0;i<nbPred;i++)
			{
				CAnimObject *pObj = m_cPredObjSet.GetAt(i);
				if (!pObj) continue;
				pObj->SetExpSet(m_currExpSet);
				pObj->OnUpdateHypothesis(NULL);
			}
			if (nbPred)
			{
				POSITION pos = GetDocument()->m_cUserInput.GetStartPosition();
				while (pos)
				{
					CUserOutput pUser;
					int nb;
					GetDocument()->m_cUserInput.GetNextAssoc(pos,nb,pUser);
					POSITION pos2 = pUser.m_lUserInput.GetStartPosition();
					while (pos2)
					{
						CUserData pUserData;
						unsigned long ptime;
						pUser.m_lUserInput.GetNextAssoc(pos2,ptime,pUserData);
						if (pUserData.m_nType == BREAKP_TEST)
						{
							OnUpdateHypothesis(&pUserData);
						}
					}
				}
			}
		}

	}
	else
	{
/*		for (int i=0;i<nb;i++)
		{
			CAnimObject *pObj = pAnimPage->m_cObjSet.GetAt(i);
			if (!pObj) continue;
			//pObj->SetExpSet(m_currExpSet);
			//pObj->SetCurrTime(m_currTime);
			//pObj->OnUpdate();
			pObj->OnUpdateSelection(-1);
		}*/
	}
	return TRUE;
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
void CViewAnimat::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	CLearningUnit *pLU = GetDocument()->GetCurrentLU();
	if (!pLU) return;

	if (!m_pER) return;
	int nbF = m_pER->m_cFormatSet.GetSize();
	if (!nbF) return;

	CFormatAnimation *pAnimPage = GetFormats();
	if (!pAnimPage) return;

	int nb = pAnimPage->m_cObjSet.GetSize();
	if (nb)
	{
		CAnimObject *pObj = pAnimPage->m_cObjSet.GetAt(0);
		if (pObj && (pObj->GetObjectDoc() == NULL))
		{
			for (int i=0;i<nb;i++)
			{
				CAnimObject *pObj = pAnimPage->m_cObjSet.GetAt(i);
				if (!pObj) continue;
				pObj->SetObjectDoc(GetDocument());
				pObj->SetObjectView(this);
			}
		}
	}
	if (/*(lHint == TRANSLATION_MAPFACT) || */
		(lHint == TRANSLATION_MAPRELATION))
	{
		CTranslationInfo *mTInfo = (CTranslationInfo *)pHint;
		OnUpdateMapRelation(mTInfo );
		Invalidate();
		UpdateWindow();
	}	
	else if (lHint==TRANSLATION_HYPOTHESIS)
	{
		CUserData *pData = (CUserData *)pHint;
		OnUpdateHypothesis(pData);
		Invalidate();
		UpdateWindow();
	}
	else if (lHint==TRANSLATION_ACTION)
	{
		CUserData *pData = (CUserData *)pHint;
		OnUpdateAction(pData);
		Invalidate();
		UpdateWindow();
	}

	else if (lHint == DOC_UPDATE_BREAKPOINT )
	{
		CTimerBreakPoint *mTInfo = (CTimerBreakPoint *)pHint;
		int nBPTime = mTInfo->m_tBreakPt;
		int nBPType = mTInfo->m_nBPType;
		OnUpdateBreakPoint(nBPTime,nBPType);
	}
	else if (lHint == DOC_UPDATE_RESTARTSIMUL)
	{
		int nbPred = m_cPredObjSet.GetSize();
		for (int i=0;i<nbPred;i++)
		{
			CAnimObject *pObj = m_cPredObjSet.GetAt(i);
			delete pObj;
		}
		m_cPredObjSet.RemoveAll();
		OnUpdateTimer();
		OnUpdateData();
		Invalidate();
		UpdateWindow();
	}
	else if (lHint == DOC_UPDATE_TIMERDATA)

	{
		OnUpdateTimer();
		OnUpdateData();
		Invalidate();
		UpdateWindow();
	}
	else if (lHint == DOC_UPDATE_ALLDATA)
	{
		OnUpdateData();
		Invalidate();
		UpdateWindow();
	}
	else if (lHint==VIEW_UPDATE_ANIMTOOLTIP)
	{
		CRect mExtRect(0,0,10,10);
		mExtRect.NormalizeRect();

		for (int i=0;i<nb;i++)
		{
			CAnimObject *pObj = pAnimPage->m_cObjSet.GetAt(i);
			if (!pObj) continue;
//			CToolInfo toolinfo;
//			m_ToolTip.GetToolInfo(toolinfo,this,pObj->m_ObjectID);
			CRect objRect = /*toolinfo.rect = */pObj->GetRect();
//			CString mstr = pObj->GetObjectDef();
//			toolinfo.lpszText = mstr.GetBuffer(mstr.GetLength());
			pObj->OnUpdate();
			objRect = /*toolinfo.rect = */pObj->GetRect();
//			m_ToolTip.SetToolInfo(&toolinfo);
			objRect.NormalizeRect();
			mExtRect |= objRect;
		}

		CRect mExtRect2 = mExtRect;
		for (i=0;i<m_cPredObjSet.GetSize();i++)
		{
			CAnimObject *pObj = m_cPredObjSet.GetAt(i);
			if (!pObj) continue;
			CRect objRect = pObj->GetRect();
			objRect.NormalizeRect();
			mExtRect2 |= objRect;
		}
		

		CSize mSize(mExtRect2.Width()+10,mExtRect2.Height()+10);
		if (mExtRect.left < 0 || mExtRect.top < 0)
		{
			CSize msize;
			msize.cx = (mExtRect.left < 0) ? -mExtRect.left+2 : 0;
			msize.cy = (mExtRect.top < 0) ? -mExtRect.top+2 : 0;
			for (int i=0;i<nb;i++)
			{
				CAnimObject *pObj = pAnimPage->m_cObjSet.GetAt(i);
				if (!pObj) continue;
//				CToolInfo toolinfo;
//				m_ToolTip.GetToolInfo(toolinfo,this,pObj->m_ObjectID);
				CRect objRect = pObj->GetRect();
				objRect.OffsetRect(msize);
				objRect.NormalizeRect();
//				toolinfo.rect = objRect;
//				m_ToolTip.SetToolInfo(&toolinfo);
				if (pObj->m_nRelatedID == -1)
				{
					pObj->SetRect(objRect);
				}
				pObj->OnUpdate();
			}
		}
		Invalidate();
		SetScrollSizes(MM_TEXT,CSize(0,0));
		if (!m_bFitToView)
		{
			//m_sDrawSize  = mSize;
			//m_nPredDraw = (m_sDrawSize.cx >= m_sDrawSize.cy);
			SetScrollSizes(MM_TEXT,mSize);
		}
		int nbPred = m_cPredObjSet.GetSize();
		for (i=0;i<nbPred;i++)
		{
			CAnimObject *pObj = m_cPredObjSet.GetAt(i);
			CAnimObject *pObj2 = pAnimPage->m_cObjSet.GetAt(i);
			if (!pObj) continue;
			if (!pObj2) continue;
			CRect mrect = pObj2->GetRect();
			//m_nPredDraw = (m_sDrawSize.cx >= m_sDrawSize.cy);
			if (m_nPredDraw)
				mrect.OffsetRect(0,m_sDrawSize.cy + 40);
			else
				mrect.OffsetRect(m_sDrawSize.cx + 40,0);
			pObj->SetRect(mrect);
		}


	}
}


/////////////////////////////////////////////////////////////////////////////
// CViewAnimat message handlers

BOOL CViewAnimat::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	// forward to multitracker
	if (pWnd == this && m_wndMultiTrack.SetCursor(this, nHitTest))
		return TRUE;
	
	return CScrollView::OnSetCursor(pWnd, nHitTest, message);
}

void CViewAnimat::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CScrollView::OnMouseMove(nFlags, point);

	CClientDC dc(this);
	OnPrepareDC(&dc);
	dc.DPtoLP(&point);

	if (CPref::g_nMode || !m_bShowProp) return;

	CFormatAnimation *pAnimPage = GetFormats();
	if (!pAnimPage) return;
	int nb = pAnimPage->m_cObjSet.GetSize();
	for (int i=0;i<nb;i++)
	{
		CAnimObject *pObj = pAnimPage->m_cObjSet.GetAt(i);
		if (!pObj) continue;

		CxAnimObjectList pList;
		CRect mrect(point,point);
		mrect.InflateRect(2,2);
		CAnimObject *pObj2 = pObj->OnHitTest(mrect,&pList);

		if (pObj2)
		{
			OnUpdateObjTooltip((WPARAM)pObj2,TRUE);
			return;
		}
	}
	OnUpdateObjTooltip((WPARAM)NULL,FALSE);


}

void CViewAnimat::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CClientDC dc(this);
	OnPrepareDC(&dc);
	dc.DPtoLP(&point);

 	if (!m_pER) return;
	int nbF = m_pER->m_cFormatSet.GetSize();
	if (!nbF) return;

	CFormatAnimation *pAnimPage = GetFormats();
	if (!pAnimPage) return;
	int nbTrans = GetTranslation();

	BOOL bIsTrans = FALSE;

	int nb = pAnimPage->m_cObjSet.GetSize();
	CxAnimObjectList mList;
	for	(int i=nb-1;i>=0;i--)
	{
		CAnimObject *pObj = pAnimPage->m_cObjSet.GetAt(i);
		if (!pObj) continue;
		CAnimObject *pObj2 = pObj->OnHitTest(point,&mList);
		if (!pObj2) continue;

		// Check if a sector is to be open/close
		CSectorObject *pSecObj = DYNAMIC_DOWNCAST(CSectorObject, pObj2);
		if (pSecObj)
		{
			if (pSecObj->IsCloseBtnHit(point) ||
					pSecObj->IsOPenBtnHit(point))
			{
				pSecObj->ToggleState();
				OnUpdate(0,DOC_UPDATE_ALLDATA,0);
				break;
			}
		}

		// If in author mode, check properties
		if (m_bShowProp)
		{
			////// Edit Object's properties
			m_wndMultiTrack.RemoveAll();
			m_wndMultiTrack.Add(pObj2);

			if (pObj2->OnEditProperty(this) == IDOK)
			{
				OnUpdate(0,VIEW_UPDATE_ANIMTOOLTIP,0);
				GetDocument()->SetModifiedFlag();
			}
			else
				Invalidate();
		}
		else
		{
			////// Handle Translation
			int nOutcome = -1;
			if (nbTrans<TRANS_MAPRELAT)
			{
				bIsTrans = TRUE;
				break;

			}
			if (nbTrans>=TRANS_MAPRELAT && (m_pObjDblClick != pObj2) &&
				pObj2->OnDoTranslation(point,nOutcome))
			{
				CTranslationInfo mTInfo;
				mTInfo.m_nTransLvl = TRANS_MAPRELAT;
				mTInfo.m_nDatas.RemoveAll();
				CTranslationInfo::CTInfo tt={m_currTime,nOutcome,m_currExpSet};
				mTInfo.m_nDatas.Add(tt);
				mTInfo.m_nTime = m_currTime;
				if (nOutcome!=-1)
				{
					GetDocument()->UpdateMapRelation(this,&mTInfo);
					//GetDocument()->UpdateAllViews(NULL,TRANSLATION_MAPRELATION,(CObject*)&mTInfo);
					bIsTrans = TRUE;
					m_pObjDblClick = pObj2;
				}
			}
			else if (nbTrans==TRANS_DYNAL && (m_pObjDblClick == pObj2) && pObj2->m_bIsSelected)
			{
				int nCBP = GetDocument()->m_nCurrBP;
				int nNBP = GetDocument()->m_nNextBP;
				if (nCBP != m_currTime || nNBP != m_currTime) break;

				CTimerBreakPoint pBP;
				CLearningUnit*	pLU = GetDocument()->GetCurrentLU();
				BOOL bEditable = pLU->GetTimerBPAt(nCBP,pBP);
				if (!bEditable) break;
				if (pBP.m_nBPType != BREAKP_ACTION &&
					pBP.m_nBPType != BREAKP_HYPOT) break;

				CUserOutput pT2;
				BOOL bOutput = GetDocument()->m_cUserInput.Lookup(m_currTime,pT2);
				if (!bOutput) break;

				bEditable = pLU->GetTimerBPAt(pT2.m_nTime,pBP);
				if (!bEditable) break;
	
				double oVal,nVal;
				if (pObj2->OnDoEdit(point,nOutcome,nVal,oVal,this))
				{
					CUserData pData;
					pData.m_nExpSet = m_currExpSet; 
					pData.m_nOutcome = nOutcome;
					pData.m_nTime = pT2.m_nTime;
					pData.m_dValue = nVal;
					pData.m_dOldValue = oVal;
	
					GetDocument()->UpdateUserData(this,&pData);
				}

		
			
				
				
				
				
				
				
/*				
				double oVal,nVal;
				CTimerBreakPoint pBP;
				CLearningUnit*	pLU = GetDocument()->GetCurrentLU();
				BOOL bEditable = pLU->GetTimerBPAt(m_currTime,pBP);

				if (!bEditable) break;

				CUserOutput pT;
				BOOL bGet = GetDocument()->m_cUserInput.Lookup(m_currTime,pT);
				if (!bGet) break;

				if (pObj2->OnDoEdit(point,nOutcome,nVal,oVal,this))
				{
					//CLearningUnit *pLU = GetDocument()->GetCurrentLU();
				/*	CMdEquation*	pEqu = GetDocument()->GetCurrentModel();
					CMdData *PreyInit= pLU->m_cOutComesSet.GetAt(nOutcome);
					CString mstr=PreyInit->GetDataName();
					double ddt = PreyInit->GetAt(m_currTime,m_currExpSet);
					PreyInit->SetAt(oVal,m_currTime,m_currExpSet);
					ddt = PreyInit->GetAt(m_currTime,m_currExpSet);

					pEqu->OnUpdateFrom(pLU,m_currExpSet,m_currTime);
					GetDocument()->UpdateAllViews(this,DOC_UPDATE_ALLDATA);
					OnUpdate(NULL,DOC_UPDATE_ALLDATA,NULL);*/

/*					CUserData pData;
					pData.m_nExpSet = m_currExpSet; 
					pData.m_nOutcome = nOutcome;
					pData.m_nTime = m_currTime;//pgvItem->row-1;
					pData.m_nType = pBP.m_nBPType;
					pData.m_dValue = nVal;
					pData.m_dOldValue = oVal;

					if (pBP.m_nBPType != BREAKP_ACTION)	// ACTION
						pData.m_nType = BREAKP_TEST;

					CUserOutput pUserData;
					BOOL bGet = GetDocument()->m_cUserInput.Lookup(pT.m_nTime,pUserData);
					if (!bGet)
					{
						int nbD = pUserData.m_lUserInput.GetCount();
						pUserData.SetAt(pData.m_nExpSet,pData.m_nOutcome,pData);
						nbD = pUserData.m_lUserInput.GetCount();
						pUserData.m_nTime = pT.m_nTime;
						GetDocument()->m_cUserInput.SetAt(pT.m_nTime,pUserData);
					}
					else
					{
						CUserData pData2;
						BOOL bRes = pUserData.GetAt(pData.m_nExpSet,pData.m_nOutcome,pData2);
						if (bRes)
						{
							pData.m_dOldValue = pData2.m_dOldValue;
						}
						pUserData.SetAt(pData.m_nExpSet,pData.m_nOutcome,pData);
						GetDocument()->m_cUserInput.SetAt(pUserData.m_nTime,pUserData);
					}
					if (pBP.m_nBPType == BREAKP_ACTION)	// ACTION
					{
						GetDocument()->UpdateAction(NULL,&pData);
					}
					else	// HYPOTHESIS
					{
						pData.m_nType = BREAKP_TEST;
						pData.m_nTime = pUserData.m_nTime;//pgvItem->row-1;
						GetDocument()->UpdateHypothesis(NULL,&pData);
					}
				}*/
			}
			if (bIsTrans) break;
		}
	}
	if (!bIsTrans)
	{
		m_pObjDblClick = NULL;

		//GetDocument()->UpdateMapRelation(NULL,NULL);
		//OnUpdate(0,DOC_UPDATE_ALLDATA,0);
		OnUpdateMapRelation(NULL);
		Invalidate();
		UpdateWindow();

	}


	CScrollView::OnLButtonDblClk(nFlags, point);
}

void CViewAnimat::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// CScrollView changes the viewport origin and mapping mode.
	// It's necessary to convert the point from device coordinates
	// to logical coordinates, such as are stored in the document.
	CClientDC dc(this);
	OnPrepareDC(&dc);
	dc.DPtoLP(&point);


	if (!m_pER) return;
	int nbF = m_pER->m_cFormatSet.GetSize();
	if (!nbF) return;

	//CPropertyPage *pPage = m_pER->m_cFormatSet.GetAt(0);
	//CDlgFormatAnimation *pAnimPage = DYNAMIC_DOWNCAST( CDlgFormatAnimation, pPage );
	CFormatAnimation *pAnimPage = GetFormats();
	if (!pAnimPage) return;

	int nb = pAnimPage->m_cObjSet.GetSize();

	//m_ToolTip.Activate(false);
    // Ask the multitrack if an object is already selected
    // or a handle. If not, start the local tracker.
    if (m_wndMultiTrack.HitTest(point) < 0 && m_bShowProp)//!CPref::g_nMode)
	{
        // Reset the multitrack only if there
        // is no CTRL key.
        if (!(nFlags & MK_CONTROL))
            m_wndMultiTrack.RemoveAll();

		CRect rcObject;

        // local tracker...
        CRectTracker tracker;
        if (tracker.TrackRubberBand(this, point, TRUE))
		{
            // see if rubber band intersects with the objects
			CRect rectT;
			tracker.m_rect.NormalizeRect();
			for (int i=nb-1;i>=0;i--)
			{
				CAnimObject *pObj = pAnimPage->m_cObjSet.GetAt(i);
				if (!pObj) continue;

				CxAnimObjectList pList;
				CAnimObject *pObj2 = pObj->OnHitTest(tracker.m_rect,&pList);

				if (pList.GetSize())
				{
					for (int j=0;j<pList.GetSize();j++)
						m_wndMultiTrack.Add(pList.GetAt(j));
				}

			/*	//if (pObj->m_nRelatedID != -1) continue;
				rcObject = pObj->GetRect();
				rcObject.NormalizeRect();

				dc.LPtoDP(&rcObject);
				BOOL res= rectT.IntersectRect(tracker.m_rect, rcObject);
				//if (rectT == rcObject) {
				if (res)
				{
					m_wndMultiTrack.Add(pObj); // add it to the multitrack,
                                             // and continue the loop
				}*/
			}
        }
		else
		{
            // No rubber band, see if the point selects an object.
			for (int i=nb-1;i>=0;i--)
			{
				CAnimObject *pObj = pAnimPage->m_cObjSet.GetAt(i);
				if (!pObj) continue;
				CAnimObject *pObj2 = pObj->OnHitTest(point);
				if (pObj2)
				{
                    m_wndMultiTrack.Add(pObj2); // add just one object to the multitrack
					break;
				}

 /*               rcObject = pObj->GetRect();
				rcObject.NormalizeRect();
				if (rcObject.PtInRect(point))
				{
                    m_wndMultiTrack.Add(pObj); // add just one object to the multitrack
					break;
				}*/
			}
		}
    }
	else if (m_bShowProp)//!CPref::g_nMode)
	{
        // Click on the multitrack, forward actions to it.
/*		if (nFlags & MK_SHIFT)
		{
			CRect rcObject;
			for (int i=nb-1;i>=0;i--)
			{
				CAnimObject *pObj = pAnimPage->m_cObjSet.GetAt(i);
				if (!pObj) continue;
                rcObject = pObj->GetRect();
				if (rcObject.PtInRect(point))
				{
                    m_wndMultiTrack.Remove(pObj); // add just one object to the multitrack
					break;
				}
			}
		}
		else*/
        if (m_wndMultiTrack.Track(this, point, FALSE))
		{
            //pDoc->SetModifiedFlag();
			OnUpdate(0,VIEW_UPDATE_ANIMTOOLTIP,0);
		}
    }
    // Update drawing.
	Invalidate();
	UpdateWindow();
	
	CScrollView::OnLButtonDown(nFlags, point);
}

bool CViewAnimat::IsViewActive()
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

void CViewAnimat::OnEditPaste() 
{
	//CPropertyPage *pPage = m_pER->m_cFormatSet.GetAt(0);
	//CDlgFormatAnimation *pAnimPage = DYNAMIC_DOWNCAST( CDlgFormatAnimation, pPage );
	CFormatAnimation *pAnimPage = GetFormats();
	if (!pAnimPage) return;

	CMainFrame *pFrame = DYNAMIC_DOWNCAST(CMainFrame, AfxGetMainWnd());
	if (!pFrame) return;
	CCopyPaste *pCopy = pFrame->GetCopyBuffer();
	if (!pCopy) return;
	int nb = pCopy->GetBufferType();
	if (nb != CCopyPaste::COPY_ANIMCPNT) return;
	nb = pCopy->GetSize();

	int nTotObj = pAnimPage->m_cObjSet.GetSize();

	m_wndMultiTrack.RemoveAll();
	for (int i=0;i<nb;i++)
	{
		CObject *pObj = pCopy->GetAt(i);
		if (!pObj) continue;
		CAnimObject *pAnimObj2 = DYNAMIC_DOWNCAST(CAnimObject, pObj);
		if (!pAnimObj2) continue;
		CAnimObject *pAnimObj = pAnimObj2->Clone();
		if (!pAnimObj) continue;

		CRect mrect = pAnimObj->GetRect();
		mrect.OffsetRect(15,15);
		pAnimObj->SetRect(mrect);

		if (m_pBuiltInSector)
		{
			int nb = m_pBuiltInSector->AddObjectInSector(pAnimObj);
		}
		else
		{
			//int nnb = pAnimPage->m_cObjSet.GetSize();
			pAnimPage->m_cObjSet.Add(pAnimObj);

			int nRel = pAnimObj->GetRelatedIdx();
			if (nRel!=-1)
			{
				//CObject *pRelObj = pCopy->GetAt(nRel);
				//if (!pRelObj) continue;
				//CAnimObject *pRelAnimObj2 = DYNAMIC_DOWNCAST(CAnimObject, pRelObj);
				//if (!pRelAnimObj2) continue;

				pAnimObj->SetRelatedObj(nTotObj+nRel);
				nRel = pAnimObj->GetRelatedIdx();
			}
		}


		//m_cVisObjSet.Add(pObj);
		//pAnimObj->m_ObjectID = nnb+1;
		//pAnimObj->SetObjectDef(_T(""));
		pAnimObj->SetObjectDoc(GetDocument());
		m_wndMultiTrack.Add(pAnimObj);
	}
	int nID = 1;
	for (i=0;i<pAnimPage->m_cObjSet.GetSize();i++)
	{
		CAnimObject *pObj = pAnimPage->m_cObjSet.GetAt(i);
		if (!pObj) continue;
		pObj->UpdateID(nID);

		pObj->SetRelatedObj(pObj->GetRelatedIdx(),&(pAnimPage->m_cObjSet));


		CString mstr = pObj->GetObjectDef();
		pObj->SetObjectDef(_T(""));
		pObj->SetObjectDef(mstr);
	}
	Invalidate();
	UpdateWindow();
}

void CViewAnimat::OnEditCopy() 
{
	static int rr = 1;

//	if (!m_pER) return;
//	CPropertyPage *pPage = m_pER->m_cFormatSet.GetAt(0);
//	CDlgFormatAnimation *pAnimPage = DYNAMIC_DOWNCAST( CDlgFormatAnimation, pPage );
//	if (!pAnimPage ) return;

	CMainFrame *pFrame = DYNAMIC_DOWNCAST(CMainFrame, AfxGetMainWnd());
	if (pFrame)
	{
		CCopyPaste *pCopy = pFrame->GetCopyBuffer();
		pCopy->ClearBuffer();
		pCopy->SetBufferType(CCopyPaste::COPY_ANIMCPNT);
		pCopy->SetOwner();
		int nb = m_wndMultiTrack.GetSize();

		CxAnimObjectList myList;
		for (int i=nb-1;i>=0;i--)
		{
			CAnimObject *pObj = m_wndMultiTrack.GetAt(i);
			if (!pObj) continue;

			//CAnimObject *pRelObj = pObj->GetRelatedObj();
			CAnimObject *pCopyObj = pObj->Clone();
			//if (pCopyObj)
			//	pAnimPage->m_cObjSet.Add(pCopyObj);
			//pCopyObj->m_pRelatedObj = pRelObj;
			myList.Add(pCopyObj);
		}

		for (i=0;i<myList.GetSize();i++)
		{
			CAnimObject *pObj = myList.GetAt(i);
			if (!pObj) continue;

			pObj->m_ObjectID;

			//CAnimObject *pRelObj = pObj->GetRelatedObj();
			int nRel = pObj->GetRelatedIdx();
			pObj->SetRelatedObj();
			if (nRel!=-1)
			{
				for (int kk=0;kk<=i;kk++)
				{
					CAnimObject *pObj2 = myList.GetAt(kk);
					if (!pObj2) continue;
					if (pObj2->m_ObjectID==nRel+1)
					{
						int nNewRel = pObj->GetRelatedIdx();
						pObj->SetRelatedObj(kk);
						nNewRel = pObj->GetRelatedIdx();
						break;
					}
				}
			}

			pCopy->Add(pObj);
		}
	}

	if (!rr)
	{
	CMetaFileDC * m_pMetaDC = new CMetaFileDC();
	m_pMetaDC->CreateEnhanced(GetDC(),NULL,NULL,"whatever");

	CClientDC clientDC(this) ; 
	m_pMetaDC->m_hAttribDC = clientDC.m_hDC; 

	//m_pMetaDC->SetMapMode(MM_TEXT   );
	//m_pMetaDC->SetViewportExt(m_pMetaDC->GetDeviceCaps(LOGPIXELSX),
	//	m_pMetaDC->GetDeviceCaps(LOGPIXELSY));
	//m_pMetaDC->SetWindowExt(100, -100);
	//draw meta file
	//OnDraw(m_pMetaDC);
	PaintAnim(m_pMetaDC);

	//close meta file dc and prepare for clipboard;
	HENHMETAFILE hMF = m_pMetaDC->CloseEnhanced();

	//copy to clipboard
	OpenClipboard();
	EmptyClipboard();
	::SetClipboardData(CF_ENHMETAFILE,hMF);
	CloseClipboard();
	//DeleteMetaFile(hMF);
	delete m_pMetaDC;
	}
	else
	{
	CBitmap         bitmap;
	CClientDC       dc(this);
	CDC             memDC;

	BeginWaitCursor();

	memDC.CreateCompatibleDC(&dc);

	CRect m_rectArea;
	GetClientRect(&m_rectArea);

	bitmap.CreateCompatibleBitmap(&dc, m_rectArea.Width(),m_rectArea.Height() );

	CBitmap* pOldBitmap = memDC.SelectObject(&bitmap);

	//OnDraw(&memDC);
	PaintAnim(&memDC);

	OpenClipboard() ;
	EmptyClipboard() ;
	SetClipboardData (CF_BITMAP, bitmap.GetSafeHandle() ) ;
	CloseClipboard () ;

	bitmap.Detach();

	EndWaitCursor();
	}

	rr = !rr;
}

void CViewAnimat::OnHideShowGrid() 
{
	// TODO: Add your command handler code here
	int gg = m_bShowGrid;
	m_bShowGrid = !gg;
	if (m_bShowGrid)
	{
		m_wndMultiTrack.SetGridSize(CSize(10,10));
	}
	else
	{
		m_wndMultiTrack.SetGridSize();
	}
	Invalidate();
}

void CViewAnimat::OnUpdateHSGrid(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_bShowGrid);
}


void CViewAnimat::OnUpdateComponent(CCmdUI* pCmdUI)
{
	BOOL bOn = FALSE;
	switch(pCmdUI->m_nID){
	case ID_ANIMATION_IMAGE:
	case ID_ANIMATION_RECT:
	case ID_ANIMATION_TEXT:
	case ID_ANIMATION_VALUE:
	case ID_ANIMATION_DENSITY:
	case ID_ANIMATION_CONNECT:
	case ID_ANIMATION_SECTOR:
	case ID_ANIMATION_CATER:
	case ID_ANIMATION_POLYGON:
		bOn = (CPref::g_nMode == 0 && m_bShowProp);
		break;
	case ID_ANIMATION_FORWARD:
	case ID_ANIMATION_BACKWARD:
		{
			bOn = (CPref::g_nMode == 0);
			int nb = m_wndMultiTrack.GetSize();
			bOn = (bOn && (nb == 1));
		}
	default:
		break;
	}
	pCmdUI->Enable(bOn);
}


void CViewAnimat::OnSize(UINT nType, int cx, int cy) 
{
	CScrollView::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if (m_bFitToView)
	{
		//CPropertyPage *pPage = m_pER->m_cFormatSet.GetAt(0);
		//CDlgFormatAnimation *pAnimPage = DYNAMIC_DOWNCAST( CDlgFormatAnimation, pPage );
		CFormatAnimation *pAnimPage = GetFormats();
		if (!pAnimPage) return;

		int nb = pAnimPage->m_cObjSet.GetSize();
		for (int i=0;i<nb;i++)
		{
			CAnimObject *pObj = pAnimPage->m_cObjSet.GetAt(i);
			if (!pObj) return;

			CRect mrect;
			GetWindowRect(&mrect);
			if (m_cPredObjSet.GetSize())
				mrect = CRect(0,0,cx,cy/2);
			else
				mrect = CRect(0,0,cx,cy);
			pObj->SetRect(mrect);
		}

		for (i=0;i<m_cPredObjSet.GetSize();i++)
		{
			CAnimObject *pObj = m_cPredObjSet.GetAt(i);
			if (!pObj) return;

			CRect mrect;
			GetWindowRect(&mrect);
			mrect = CRect(cx,0,cx,cy);
			pObj->SetRect(mrect);
		}
		OnUpdate(0,VIEW_UPDATE_ANIMTOOLTIP,0);
	}

	if (m_wndAnimBar && ::IsWindow(m_wndAnimBar.m_hWnd))
	{
		UINT tt = ID_TASK_CANCELTASK;
		UINT nID,nStyle;
		int nImage;
		int nSize = 175;
		if (m_wndToolBar && m_wndAlignBar)
		{
			if ((m_wndToolBar.IsFloating() && m_wndAlignBar.IsFloating()) ||
				(!m_wndToolBar.IsWindowVisible() && !m_wndAlignBar.IsWindowVisible()))
				nSize = 225;
			else if ((m_wndToolBar.IsFloating() || m_wndAlignBar.IsFloating()) ||
				(!m_wndToolBar.IsWindowVisible() || !m_wndAlignBar.IsWindowVisible()))
				nSize = 200;
		}
		else nSize = 225;

		m_wndAnimBar.GetButtonInfo(4,nID,nStyle,nImage);
		m_wndAnimBar.SetButtonInfo(4,nID,TBBS_SEPARATOR,cx-nSize);

	}

	
}

void CViewAnimat::OnNextExpSet() 
{
	// TODO: Add your command handler code here
	CLearningUnit *pLU = GetDocument()->GetCurrentLU();
	if (!pLU) return;
	if (!m_pER) return;

	int nbExpSet = pLU->m_cExpSet.GetSize();
	m_currExpSet++;

	if (m_currExpSet >=nbExpSet) m_currExpSet = 0;

	//CPropertyPage *pPage = m_pER->m_cFormatSet.GetAt(0);
	//CDlgFormatAnimation *pAnimPage = DYNAMIC_DOWNCAST( CDlgFormatAnimation, pPage );
	CFormatAnimation *pAnimPage = GetFormats();
	if (!pAnimPage) return;

	int nb = pAnimPage->m_cObjSet.GetSize();
	for (int i=0;i<nb;i++)
	{
		CAnimObject *pObj = pAnimPage->m_cObjSet.GetAt(i);
		if (!pObj) return;
		//pObj->m_currExpSet = m_currExpSet;
		pObj->SetExpSet(m_currExpSet);
	}

	int nbPred = m_cPredObjSet.GetSize();
	for (i=0;i<nbPred;i++)
	{
		CAnimObject *pObj = m_cPredObjSet.GetAt(i);
		if (!pObj) continue;
		pObj->SetExpSet(m_currExpSet);
		pObj->OnUpdateHypothesis(NULL);
	}
	if (nbPred)
	{
		POSITION pos = GetDocument()->m_cUserInput.GetStartPosition();
		while (pos)
		{
			CUserOutput pUser;
			int nb;
			GetDocument()->m_cUserInput.GetNextAssoc(pos,nb,pUser);
			POSITION pos2 = pUser.m_lUserInput.GetStartPosition();
			while (pos2)
			{
				CUserData pUserData;
				unsigned long ptime;
				pUser.m_lUserInput.GetNextAssoc(pos2,ptime,pUserData);
				if (pUserData.m_nType == BREAKP_TEST)
				{
					OnUpdateHypothesis(&pUserData);
				}
			}
		}
	}

	m_wndAnimBar.m_comboBox.SetCurSel(m_currExpSet);
	OnUpdateData();
	//OnUpdate(0,DOC_UPDATE_ALLDATA,0);
	CTrace::T_CHANGEEXPSET(this,m_currExpSet);
	Invalidate();
	UpdateWindow();
}

void CViewAnimat::OnPrevExpSet() 
{
	// TODO: Add your command handler code here
	CLearningUnit *pLU = GetDocument()->GetCurrentLU();
	if (!pLU) return;

	int nbExpSet = pLU->m_cExpSet.GetSize();
	m_currExpSet--;
	if (m_currExpSet < 0) m_currExpSet = nbExpSet-1;
	
	//CPropertyPage *pPage = m_pER->m_cFormatSet.GetAt(0);
	//CDlgFormatAnimation *pAnimPage = DYNAMIC_DOWNCAST( CDlgFormatAnimation, pPage );
	CFormatAnimation *pAnimPage = GetFormats();
	if (!pAnimPage) return;

	int nb = pAnimPage->m_cObjSet.GetSize();
	for (int i=0;i<nb;i++)
	{
		CAnimObject *pObj = pAnimPage->m_cObjSet.GetAt(i);
		if (!pObj) return;
		//pObj->m_currExpSet = m_currExpSet;
		pObj->SetExpSet(m_currExpSet);
	}
	int nbPred = m_cPredObjSet.GetSize();
	for (i=0;i<nbPred;i++)
	{
		CAnimObject *pObj = m_cPredObjSet.GetAt(i);
		if (!pObj) continue;
		pObj->SetExpSet(m_currExpSet);
		pObj->OnUpdateHypothesis(NULL);
	}
	if (nbPred)
	{
		POSITION pos = GetDocument()->m_cUserInput.GetStartPosition();
		while (pos)
		{
			CUserOutput pUser;
			int nb;
			GetDocument()->m_cUserInput.GetNextAssoc(pos,nb,pUser);
			POSITION pos2 = pUser.m_lUserInput.GetStartPosition();
			while (pos2)
			{
				CUserData pUserData;
				unsigned long ptime;
				pUser.m_lUserInput.GetNextAssoc(pos2,ptime,pUserData);
				if (pUserData.m_nType == BREAKP_TEST)
				{
					OnUpdateHypothesis(&pUserData);
				}
			}
		}
	}
	m_wndAnimBar.m_comboBox.SetCurSel(m_currExpSet);
	OnUpdateData();
	CTrace::T_CHANGEEXPSET(this,m_currExpSet);
	Invalidate();
	UpdateWindow();
	//OnUpdate(0,DOC_UPDATE_ALLDATA,0);	

/*	CToolBarCtrl &pTool = m_wndToolBar.GetToolBarCtrl();

	int nbs = pTool.AddBitmap(4,IDB_ANIM_COMPONENT);
	nbs = pTool.AddString(ID_SELECTDATA);

	TBBUTTON lpButtons[1] = 
	{1,ID_ANIMATION_NEXTEXPSET,TBSTATE_ENABLED,TBSTYLE_BUTTON,0,0
	};
	nbs = pTool.AddButtons(1,lpButtons);
	pTool.Invalidate();
	pTool.UpdateWindow();*/
}

void CViewAnimat::OnSelChangeExpSet()
{
	//m_wndAnimBar.m_comboBox.SetCurSel((int)(nID-ID_VIEW_ANIM));
	int nb = m_wndAnimBar.m_comboBox.GetCurSel();
	nb++;
	m_currExpSet = nb;
	OnPrevExpSet();
}

void CViewAnimat::OnEditSelectAll() 
{
	// TODO: Add your command handler code here
	//CPropertyPage *pPage = m_pER->m_cFormatSet.GetAt(0);
	//CDlgFormatAnimation *pAnimPage = DYNAMIC_DOWNCAST( CDlgFormatAnimation, pPage );
	CFormatAnimation *pAnimPage = GetFormats();
	if (!pAnimPage) return;

	int nb = pAnimPage->m_cObjSet.GetSize();

	m_wndMultiTrack.RemoveAll();
	for (int i=0;i<nb;i++)
	{
		CAnimObject *pObj = pAnimPage->m_cObjSet.GetAt(i);
		if (!pObj) return;
		m_wndMultiTrack.Add(pObj);
		//CRect mrect;
		//GetWindowRect(&mrect);
		//mrect = CRect(0,0,cx,cy);
		//pObj->SetRect(mrect);
	}
	Invalidate();
	UpdateWindow();

}

void CViewAnimat::OnEditCut() 
{
	// TODO: Add your command handler code here
	OnEditDelete();
}

void CViewAnimat::OnEditDelete() 
{
	// TODO: Add your command handler code here
	BOOL bVoid = m_wndMultiTrack.IsEmpty();
	if (bVoid) return;
	
	//CPropertyPage *pPage = m_pER->m_cFormatSet.GetAt(0);
	//CDlgFormatAnimation *pAnimPage = DYNAMIC_DOWNCAST( CDlgFormatAnimation, pPage );
	CFormatAnimation *pAnimPage = GetFormats();
	if (!pAnimPage) return;

	CString mstr;
	mstr.LoadString(AFX_IDS_DELETE_COMPONENT);
	if (MessageBox(mstr,NULL,MB_YESNO | MB_ICONQUESTION) != IDYES) return;

	int nbItem = m_wndMultiTrack.GetSize();
	for (int i=0;i<nbItem;i++)
	{
		CAnimObject *pObj = m_wndMultiTrack.GetAt(i);
		if (!pObj) continue;
		if (pObj == m_pBuiltInSector)
			OnAddInSector();
		if (pObj->m_pSector)
		{
			CSectorObject *pSect= pObj->m_pSector;
			pSect->RemoveObjectFromSector(pObj);
			delete pObj;
		}
		else
		{
			int nb = pAnimPage->m_cObjSet.GetSize();
			for (int j=0;j<nb;j++)
			{
				CAnimObject *pObjAnim = pAnimPage->m_cObjSet.GetAt(j);
				if (!pObjAnim) continue;
				if (pObjAnim == pObj)
				{
					pAnimPage->m_cObjSet.RemoveAt(j);
					delete pObjAnim;
					break;
				}
			}
		}
		/*int nb2 = m_cVisObjSet.GetSize();
		for (j=0;j<nb2;j++)
		{
			CAnimObject *pObjAnim = m_cVisObjSet.GetAt(j);
			if (!pObjAnim) continue;
			if (pObjAnim == pObj)
			{
				m_cVisObjSet.RemoveAt(j);
			}
		}*/
	}
//	if (m_pSelectDlg)
//	{
//		m_pSelectDlg->m_cObjSet.RemoveAll();
//	}

	int nb = pAnimPage->m_cObjSet.GetSize();
	int nID = 1;
	for (int j=0;j<nb;j++)
	{
		CAnimObject *pObjAnim = pAnimPage->m_cObjSet.GetAt(j);
		if (!pObjAnim) continue;
		//pObjAnim->m_ObjectID = j+1;
		pObjAnim->UpdateID(nID);
		//pObjAnim->SetObjectDef(_T(""));

		CString mstr = pObjAnim->GetObjectDef();
		pObjAnim->SetObjectDef(_T(""));
		pObjAnim->SetObjectDef(mstr);

//		if (m_pSelectDlg)
//		{
//			m_pSelectDlg->m_cObjSet.Add(pObjAnim);
//		}

	}
	if (m_pSelectDlg)
	{
		m_pSelectDlg->UpdateList();
	}

	GetDocument()->SetModifiedFlag();
	m_wndMultiTrack.RemoveAll();
	Invalidate();
	UpdateWindow();
}

void CViewAnimat::OnUpdateEditCmd(CCmdUI* pCmdUI) 
{
	BOOL bEnab= FALSE;
	int nCPType = 0;
	CMainFrame *pFrame = DYNAMIC_DOWNCAST(CMainFrame, AfxGetMainWnd());
	if (pFrame)
	{
		CCopyPaste *pCopy = pFrame->GetCopyBuffer();
		nCPType = pCopy->GetBufferType();
	}


	// TODO: Add your command update UI handler code here
	switch(pCmdUI->m_nID){
	case ID_EDIT_COPY:
		bEnab = (BOOL)m_wndMultiTrack.GetSize();
		break;
	case ID_EDIT_PASTE:
		bEnab = (nCPType == CCopyPaste::COPY_ANIMCPNT);
		break;
	case ID_EDIT_CLEAR:
	case ID_EDIT_CUT:
		bEnab = !(m_wndMultiTrack.IsEmpty());
		break;
	}
	pCmdUI->Enable(bEnab);
}

BOOL CViewAnimat::CreateNewObject(CAnimObject *pObj)
{
	if (!pObj) return FALSE;

	//CPropertyPage *pPage = m_pER->m_cFormatSet.GetAt(0);
	//CDlgFormatAnimation *pAnimPage = DYNAMIC_DOWNCAST( CDlgFormatAnimation, pPage );
	CFormatAnimation *pAnimPage = GetFormats();
	if (!pAnimPage) 
	{
		delete pObj;
		return FALSE;
	}

	if (m_pBuiltInSector)
	{
		int nb = m_pBuiltInSector->AddObjectInSector(pObj);
	}
	else
	{
		int nb = pAnimPage->m_cObjSet.GetSize()+1;
		pAnimPage->m_cObjSet.Add(pObj);
	}
	//m_cVisObjSet.Add(pObj);

//	if (m_pSelectDlg)
//	{
//		m_pSelectDlg->m_cObjSet.RemoveAll();
//	}

	int nb = 1;
	for (int i=0;i<pAnimPage->m_cObjSet.GetSize();i++)
	{
		CAnimObject *pObj2 = pAnimPage->m_cObjSet.GetAt(i);
		if (!pObj2) continue;
		pObj2->UpdateID(nb);
		//pObj->m_ObjectID = nb+1;

		CString mstr = pObj2->GetObjectDef();
		pObj2->SetObjectDef(_T(""));
		//pObj2->SetObjectDef(mstr);

		//pObj2->SetObjectDef(_T(""));
		pObj2->SetObjectDoc(GetDocument());
		pObj2->SetObjectView(this);
//		if (m_pSelectDlg)
//		{
//			m_pSelectDlg->m_cObjSet.Add(pObj2);
//		}
	}
	if (m_pSelectDlg)
	{
		m_pSelectDlg->UpdateList();
	}
	GetDocument()->SetModifiedFlag();
	Invalidate();
	UpdateWindow();
	return TRUE;
}

void CViewAnimat::OnNewComponent(UINT nID) 
{
	CAnimObject *pObj = NULL;
	switch (nID){
	case ID_ANIMATION_IMAGE:
		pObj = new CPictObject(0,CRect(150,100,200,150), -1);
		break;
	case ID_ANIMATION_DENSITY:
		pObj = new CDensityObject(0,CRect(150,100,450,250));
		break;
	case ID_ANIMATION_CONNECT:
		pObj = new CLinkObject(0);
		break;
	case ID_ANIMATION_POLYGON:
		pObj = new CPolygonObject(0);
		break;
	case ID_ANIMATION_CATER:
		pObj = new CPictDensityObject(0,CRect(150,100,200,150),-1);
		break;
	case ID_ANIMATION_SECTOR:
		//pObj = new CSectorObject(0);
		pObj = new CAnimControl();
		//pObj = new CVectorObject();
		break;
	case ID_ANIMATION_RECT:
		pObj = new CAnimObject(0,CRect(150,100,200,150));
		break;
	case ID_ANIMATION_TEXT:
		pObj = new CTextObject(0,CRect(150,100,200,150),CString(_T("?????")));
		break;
	case ID_ANIMATION_VALUE:
		pObj = new CValueObject(0,CRect(150,100,200,150),-1);
		break;
	}
	// TODO: Add your command handler code here
	CreateNewObject(pObj);
}

void CViewAnimat::OnSendForward()
{
	BOOL bRedraw = FALSE;
/*	CAnimObject *pObj = m_wndMultiTrack.GetAt(0);
	if (!pObj) return;

	CPropertyPage *pPage = m_pER->m_cFormatSet.GetAt(0);
	CDlgFormatAnimation *pAnimPage = DYNAMIC_DOWNCAST( CDlgFormatAnimation, pPage );
	if (!pAnimPage ) return;

	int nb = pAnimPage->m_cObjSet.GetSize();
	//nb = m_cVisObjSet.GetSize();
	for (int i=0;i<nb;i++)
	{
		CAnimObject *pO = pAnimPage->m_cObjSet.GetAt(i);
		//CAnimObject *pO = m_cVisObjSet.GetAt(i);
		if (!pO) continue;
		if (pO == pObj)
		{
			//m_cVisObjSet.RemoveAt(i);
			//m_cVisObjSet.Add(pO);
			bRedraw = TRUE;
			break;
		}
	}
	if (bRedraw)
	{
		Invalidate();
		UpdateWindow();
	}*/
}
void CViewAnimat::OnSendBackward()
{
/*	BOOL bRedraw = FALSE;
	CAnimObject *pObj = m_wndMultiTrack.GetAt(0);
	if (!pObj) return;

	if (!m_pER) return;
	CPropertyPage *pPage = m_pER->m_cFormatSet.GetAt(0);
	CDlgFormatAnimation *pAnimPage = DYNAMIC_DOWNCAST( CDlgFormatAnimation, pPage );
	if (!pAnimPage ) return;

	int nb = pAnimPage->m_cObjSet.GetSize();
	CAnimObject *pCopyObj = pObj->Clone();
	if (pCopyObj)
		pAnimPage->m_cObjSet.Add(pCopyObj);
		Invalidate();
		UpdateWindow();*/

/*	CAnimObject *pObj = m_wndMultiTrack.GetAt(0);
	if (!pObj) return;

	CPropertyPage *pPage = m_pER->m_cFormatSet.GetAt(0);
	CDlgFormatAnimation *pAnimPage = DYNAMIC_DOWNCAST( CDlgFormatAnimation, pPage );
	if (!pAnimPage ) return;

	int nb = pAnimPage->m_cObjSet.GetSize();
	//nb = m_cVisObjSet.GetSize();

	for (int i=0;i<nb;i++)
	{
		CAnimObject *pO = pAnimPage->m_cObjSet.GetAt(i);
		//CAnimObject *pO = m_cVisObjSet.GetAt(i);
		if (!pO) continue;
		if (pO == pObj)
		{
			bRedraw = TRUE;
			//m_cVisObjSet.RemoveAt(i);
			//m_cVisObjSet.InsertAt(0,pO);
			break;
		}
	}

	if (bRedraw)
	{
		Invalidate();
		UpdateWindow();
	}*/
}

void CViewAnimat::OnUpdateAddInSector(CCmdUI* pCmdUI)
{
	BOOL bEnable = FALSE;
	BOOL bChecked = (BOOL)(m_pBuiltInSector != NULL);

	if (bChecked)
	{
		bEnable = TRUE;
	}
	else
	{
		int nb = m_wndMultiTrack.GetSize();
		if (nb==1)
		{
			CAnimObject *pObj = m_wndMultiTrack.GetAt(0);
			CSectorObject *pSector = DYNAMIC_DOWNCAST(CSectorObject, pObj);
			bEnable = (BOOL)pSector;
		}
	}

	pCmdUI->Enable(bEnable);
	pCmdUI->SetCheck(bChecked);
}

void CViewAnimat::OnAddInSector() 
{
	// TODO: Add your command handler code here
	if (m_pBuiltInSector)
	{
		m_pBuiltInSector = NULL;
	}
	else
	{
		int nb = m_wndMultiTrack.GetSize();
		if (nb==1)
		{
			CAnimObject *pObj = m_wndMultiTrack.GetAt(0);
			CSectorObject *pSector = DYNAMIC_DOWNCAST(CSectorObject, pObj);
			if (pSector)
				m_pBuiltInSector = pSector;
		}
	}
	Invalidate();
	UpdateWindow();
}

void CViewAnimat::OnUpdateSelectComponent(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(!CPref::g_nMode);
	BOOL bCheck = m_pSelectDlg!=NULL && m_pSelectDlg->IsWindowVisible();
	pCmdUI->SetCheck(bCheck);
}

void CViewAnimat::OnSelectComponent() 
{
	CAnimObject *pObj = NULL;
	
	CFormatAnimation *pAnimPage = GetFormats();
	if (!pAnimPage) return;

	if (m_pSelectDlg)
	{
		//delete m_pSelectDlg;
		//m_pSelectDlg = NULL;
		if (m_pSelectDlg->IsWindowVisible())
			m_pSelectDlg->ShowWindow(SW_HIDE);
		else
			m_pSelectDlg->ShowWindow(SW_SHOW);
	}
	else
	{
		m_pSelectDlg = new CSelectorDlg();
		m_pSelectDlg->pView = this;
		m_pSelectDlg->pmultiTrack = &m_wndMultiTrack;
		m_pSelectDlg->m_cObjSet = &(pAnimPage->m_cObjSet);
		/*int nb = pAnimPage->m_cObjSet.GetSize();
		for (int i=0;i<nb;i++)
		{
			CAnimObject *pObj = pAnimPage->m_cObjSet.GetAt(i);
			if (!pObj) continue;
			m_pSelectDlg->m_cObjSet.Add(pObj);
		}*/
		m_pSelectDlg->Create(IDD_ANIMATION_SELECT,this);
		m_pSelectDlg->ShowWindow(SW_SHOW);
	}

/*	CSelectorDlg dlg(this);
	int nb = pAnimPage->m_cObjSet.GetSize();
	for (int i=0;i<nb;i++)
	{
		CAnimObject *pObj = pAnimPage->m_cObjSet.GetAt(i);
		if (!pObj) continue;
		dlg.m_cObjSet.Add(pObj);
	}


	dlg.DoModal();*/
}


void CViewAnimat::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	// TODO: Add your message handler code here
   // make sure window is active
	if (CPref::g_nMode) return;

	CFrameWnd * pFWmnd = GetParentFrame();


	if (DYNAMIC_DOWNCAST(CChildFramePanes,pFWmnd))
	{
		pFWmnd->SendMessage(VIEW_PANES_CONTEXTMENU,(WPARAM)this,(LPARAM)120);
		return;
	}

	GetParentFrame()->ActivateFrame();
	CMenu menu;
	if (menu.LoadMenu(IDR_ANIM_CTX))
	{
		CMenu* pPopup = menu.GetSubMenu(0);
		ASSERT(pPopup != NULL);
		
		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON,
			point.x, point.y,
			AfxGetMainWnd()); // use main window for cmds
	}
}


void CViewAnimat::OnShowAlignTool() 
{
	// TODO: Add your command handler code here
	BOOL bVis = ((m_wndAlignBar.GetStyle() & WS_VISIBLE) != 0);
	CMDIChildWnd *pMDICh = DYNAMIC_DOWNCAST(CMDIChildWnd,GetParent());
	if (pMDICh)
	{
		pMDICh->ShowControlBar(&m_wndAlignBar, !bVis, FALSE);
	}
}

void CViewAnimat::OnShowComponentTool() 
{
	// TODO: Add your command handler code here
	BOOL bVis = ((m_wndToolBar.GetStyle() & WS_VISIBLE) != 0);
		CMDIChildWnd *pMDICh = DYNAMIC_DOWNCAST(CMDIChildWnd,GetParent());
	if (pMDICh)
	{
		pMDICh->ShowControlBar(&m_wndToolBar, !bVis, FALSE);
	}
}


void CViewAnimat::OnUpdateAlignTool(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if (m_wndAlignBar.GetSafeHwnd())
	{
		BOOL bVis = ((m_wndAlignBar.GetStyle() & WS_VISIBLE) != 0);
		pCmdUI->Enable();
		pCmdUI->SetCheck(bVis);
	}
	else
		pCmdUI->Enable(FALSE);
}

void CViewAnimat::OnUpdateComponentTool(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if (m_wndToolBar.GetSafeHwnd())
	{
		BOOL bVis = ((m_wndToolBar.GetStyle() & WS_VISIBLE) != 0);
		pCmdUI->Enable();
		pCmdUI->SetCheck(bVis);
	}
	else
		pCmdUI->Enable(FALSE);
}


void CViewAnimat::OnKillFocus(CWnd* pNewWnd) 
{
	CScrollView::OnKillFocus(pNewWnd);
	
	// TODO: Add your message handler code here
	CMDIChildWnd *pMDICh = DYNAMIC_DOWNCAST(CMDIChildWnd,GetParent());
	if (pMDICh)
	{
		if (m_wndAlignBar.IsFloating())
		{
			pMDICh->ShowControlBar(&m_wndAlignBar, FALSE, FALSE);
			//pMDICh->DockControlBar(&m_wndAlignBar,AFX_IDW_DOCKBAR_LEFT);
		}
		if (m_wndToolBar.IsFloating())
		{
			pMDICh->ShowControlBar(&m_wndToolBar, FALSE, FALSE);
			//pMDICh->DockControlBar(&m_wndToolBar,AFX_IDW_DOCKBAR_LEFT);
		}
	}
	
}

void CViewAnimat::OnSetFocus(CWnd* pOldWnd) 
{
	CScrollView::OnSetFocus(pOldWnd);
	
	// TODO: Add your message handler code here
/*	CMDIChildWnd *pMDICh = DYNAMIC_DOWNCAST(CMDIChildWnd,GetParent());
	if (pMDICh)
	{
		if (m_wndAlignBar.IsFloating())
		{
			pMDICh->ShowControlBar(&m_wndAlignBar, TRUE, FALSE);
			//pMDICh->DockControlBar(&m_wndAlignBar,AFX_IDW_DOCKBAR_LEFT);
		}
		if (m_wndToolBar.IsFloating())
		{
			pMDICh->ShowControlBar(&m_wndToolBar, TRUE, FALSE);
			//pMDICh->DockControlBar(&m_wndToolBar,AFX_IDW_DOCKBAR_LEFT);
		}
	}*/	
}


void CViewAnimat::TranslateObjects(int dX,int dY)
{
	int nb = m_wndMultiTrack.GetSize();
//	int nDelta = (m_bShowGrid) ? 10 : 1;
	for (int i=0;i<nb;i++)
	{
		CAnimObject *pObj = m_wndMultiTrack.GetAt(i);
		if (!pObj) continue;
		CRect mrect = pObj->GetRect();
		if (m_bShowGrid)
		{
			CSize ms = mrect.Size();
			if (dX == -1)
				mrect.left = ((mrect.left-10) / 10 ) * 10;
			if (dX == 1)
				mrect.left = ((mrect.left+10) / 10 ) * 10;
			if (dY == -1)
				mrect.top = ((mrect.top-10) / 10 ) * 10;
			if (dY == 1)
				mrect.top = ((mrect.top + 10) / 10 ) * 10;
			mrect.bottom = mrect.top+ms.cy;
			mrect.right = mrect.left+ms.cx;
		}
		else
			mrect.OffsetRect(dX,dY);
		pObj->SetRect(mrect);
	}
	OnUpdate(0,VIEW_UPDATE_ANIMTOOLTIP,0);
}

void CViewAnimat::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	int nb = m_wndMultiTrack.GetSize();
	if (nb && !CPref::g_nMode)
	{
		switch (nChar){
		case VK_LEFT:
			TranslateObjects(-1,0);
			break;
		case VK_RIGHT:
			TranslateObjects(1,0);
			break;
		case VK_UP:
			TranslateObjects(0,-1);
			break;
		case VK_DOWN:
			TranslateObjects(0,1);
			break;
		}
	}
	CScrollView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CViewAnimat::AlignObjects(int dX,int dY)
{
	CAnimObject *pSrcObj = m_wndMultiTrack.GetAt(0);
	if (!pSrcObj) return;

	CRect SrcRect = pSrcObj->GetRect();
	int nb = m_wndMultiTrack.GetSize();
	for (int i=1;i<nb;i++)
	{
		CAnimObject *pObj = m_wndMultiTrack.GetAt(i);
		if (!pObj) continue;
		CRect mrect = pObj->GetRect();
		CSize ms = mrect.Size();
		if (dX == -1)
		{
			mrect.left = SrcRect.left;
			mrect.right = mrect.left+ms.cx;
		}
		else if (dX == 1)
		{
			mrect.right = SrcRect.right;
			mrect.left = mrect.right-ms.cx;
		}
		else if (dX == 0)
		{
			int dx = (ms.cx - SrcRect.Size().cx)/2;
			int dx2 = mrect.left - SrcRect.left;
			mrect.left -= (dx+dx2);
			mrect.right = mrect.left+ms.cx;
		}
		if (dY == -1)
		{
			mrect.top = SrcRect.top;
			mrect.bottom = mrect.top+ms.cy;
		}
		else if (dY == 1)
		{
			mrect.bottom = SrcRect.bottom;
			mrect.top = mrect.bottom-ms.cy;
		}
		else if (dY == 0)
		{
			int dy = (ms.cy - SrcRect.Size().cy)/2;
			int dy2 = mrect.top - SrcRect.top;
			mrect.top -= (dy+dy2);
			mrect.bottom = mrect.top+ms.cy;
		}
		pObj->SetRect(mrect);
	}
	OnUpdate(0,VIEW_UPDATE_ANIMTOOLTIP,0);
}


void CViewAnimat::OnSetAlignment(UINT nID)
{
	switch (nID){
	case ID_ANIM_ALIGNLEFT:
		AlignObjects(-1,999);
		break;
	case ID_ANIM_ALIGNCENTER:
		AlignObjects(0,999);
		break;
	case ID_ANIM_ALIGNRIGHT:
		AlignObjects(1,999);
		break;
	case ID_ANIM_ALIGNTOP:
		AlignObjects(999,-1);
		break;
	case ID_ANIM_ALIGNMIDDLE:
		AlignObjects(999,0);
		break;
	case ID_ANIM_ALIGNBOTTOM:
		AlignObjects(999,1);
		break;
	default:
		break;
	}
}

void CViewAnimat::OnUpdateSetAlignment(CCmdUI* pCmdUI)
{
	int nb = m_wndMultiTrack.GetSize();
	BOOL bEnab = FALSE;

	switch (pCmdUI->m_nID){
	case ID_ANIM_ALIGNLEFT:
	case ID_ANIM_ALIGNCENTER:
	case ID_ANIM_ALIGNRIGHT:
	case ID_ANIM_ALIGNTOP:
	case ID_ANIM_ALIGNMIDDLE:
	case ID_ANIM_ALIGNBOTTOM:
		bEnab = (nb > 1);
		break;
	case ID_ANIM_ALIGNHOR:
	case ID_ANIM_ALIGNVER:
		bEnab = (nb > 2);
		break;
	default:
		break;
	}
	pCmdUI->Enable(bEnab);
}

void CViewAnimat::OnTranslationTasks(UINT nID)
{
	// TODO: Add your command handler code here
	int nTrans = GetTranslation();
	switch (nID) {
	case ID_TRANS_INDEPENDENT:
	case ID_TRANS_MAPRELATION:
	case ID_TRANS_DYNALINKED:
		//m_nCurrTTask = nID - ID_TASK_MAPFACT+1;
		break;
	case ID_TASK_CANCELTASK:
	default:
		//m_nCurrTTask = 0;
		break;
	}

}

void CViewAnimat::OnUpdateTransTasks(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	BOOL	bEnab = FALSE;
	BOOL	bCheck = FALSE;
	UINT	nT = GetTranslation();
	//UINT	nCT = m_nCurrTTask;

	switch (pCmdUI->m_nID) {
	case ID_TRANS_INDEPENDENT:
	case ID_TRANS_MAPRELATION:
	case ID_TRANS_DYNALINKED:
		bCheck = (nT == (pCmdUI->m_nID - ID_TRANS_INDEPENDENT));
		bEnab = bCheck;
		break;
	default:
		break;
	}
	pCmdUI->Enable(bEnab);
	pCmdUI->SetCheck(bCheck);

}

void CViewAnimat::OnShowProperties() 
{
	// TODO: Add your command handler code here
	m_bShowProp = !	m_bShowProp;
	m_wndMultiTrack.RemoveAll();
	Invalidate();
	UpdateWindow();
	
}

void CViewAnimat::OnUpdateShowProperties(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(!CPref::g_nMode);
	pCmdUI->SetCheck(!CPref::g_nMode && m_bShowProp);
}
