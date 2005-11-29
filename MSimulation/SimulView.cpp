// SimulView.cpp : implementation of the CViewSDI class
//

#include "stdafx.h"
#include "Simul.h"

#include "GridCtrl\MemDC.h"

#include "SimulDoc.h"
#include <MSimulation\PreyPredModel.h>
//#include <MInstruction\LearningUnit.h>
#include "SimulView.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CViewSDI

IMPLEMENT_DYNCREATE(CViewSDI, CScrollView)

BEGIN_MESSAGE_MAP(CViewSDI, CScrollView)
	//{{AFX_MSG_MAP(CViewSDI)
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_WM_SETCURSOR()
	ON_WM_MOUSEMOVE()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(ID_SIMULATION_UPDATEMODELS, OnSimulationUpdatemodels)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewSDI construction/destruction

CViewSDI::CViewSDI()
{
	// TODO: add construction code here
	m_DocSize = CSize(-1,-1);
	//m_Tracker.m_nStyle = CRectTracker::resizeInside | CRectTracker::dottedLine | CRectTracker::hatchedBorder   ;
	m_CurrModel = 0;

	strcpy(m_logFont.lfFaceName, "Times New Roman");
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

	m_ImageList.Create(IDB_ER_ICONS,32,0,RGB(192,192,192));

	m_Tracker.m_nStyle = /*CRectTracker::resizeInside | CRectTracker::dottedLine |*/ CRectTracker::hatchedBorder   ;
}

CViewSDI::~CViewSDI()
{
}

BOOL CViewSDI::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CScrollView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CViewSDI drawing
BOOL CViewSDI::OnEraseBkgnd(CDC* pDC) 
{
	return true;
//	return CScrollView::OnEraseBkgnd(pDC);
}


/*CRect CViewSDI::DrawIcon(CDC* pDC,CPoint sPt,int nIcon,CString sTitle,BOOL sel,int width)
{
	CRect	TextRect(0,0,width,40),
			IconRect(0,0,40,40),
			CpnRect;

	
	int widT = pDC->DrawText(sTitle,&TextRect, DT_CALCRECT| DT_VCENTER | DT_CENTER | DT_NOPREFIX | DT_WORDBREAK);

	widT+=3*5+IconRect.Height();

	IconRect.OffsetRect(sPt.x+(TextRect.Width()+10-IconRect.Width())/2,sPt.y+5);
	TextRect.OffsetRect(sPt.x+5,IconRect.bottom+5);

	CpnRect.SetRect(sPt.x,sPt.y,sPt.x+TextRect.Width()+10,TextRect.bottom+5);

//	pDC->Draw3dRect(IconRect, RGB(255,255,255), RGB(0,0,0));
	pDC->DrawEdge(IconRect,(sel) ? EDGE_SUNKEN : EDGE_RAISED,BF_RECT   );

	COLORREF bcolor = ::GetSysColor(COLOR_WINDOW);//COLOR_3DFACE); 
	COLORREF oldc= m_ImageList.SetBkColor(CLR_NONE);
	m_ImageList.Draw(pDC,nIcon,CPoint(IconRect.left+4,IconRect.top+4),
		//ILD_NORMAL);
		(sel) ? ILD_NORMAL | ILD_SELECTED : ILD_NORMAL);
	m_ImageList.SetBkColor(oldc);

	COLORREF ref=pDC->GetTextColor();
	if (sel)
	{
		ref = pDC->SetTextColor(RGB(255,0,0));
	}
	int oldBkmod = pDC->SetBkMode(TRANSPARENT);
	pDC->DrawText(sTitle,&TextRect, DT_VCENTER   | DT_CENTER | DT_NOPREFIX | DT_WORDBREAK);
	pDC->SetBkMode(oldBkmod);
	if (sel)
		pDC->SetTextColor(ref);

	return CpnRect;
}*/


/*CRect CViewSDI::DrawParam(CDC* pDC,CPoint sPt)
{
	CRect	mDrawRect;
	CSize	ss;
	CPoint	pSt=sPt,ePt(-10,-10);

	CSimulDoc* pDoc = GetDocument();

	mDrawRect.SetRectEmpty();
	m_datapos.RemoveAll();
	////// PARAMETERS


	const CMdEquation* pEquation = pDoc->m_pSimul->GetEquation(m_CurrModel);
	if (!pEquation) return mDrawRect;

	int nb = pEquation->m_cParSet.GetSize();
	int nb1 = pEquation->m_cVarSet.GetSize();
	for (int i=0;i<(nb+nb1);i++)
	{
		CString tt;
		if (i == nb)
			sPt.Offset(0,ss.cy+5);

		if (i < nb)
			tt = pEquation->m_cParSet.GetAt(i)->GetName();
		else
			tt = pEquation->m_cVarSet.GetAt(i-nb)->GetName();
		ss = pDC->GetTextExtent(tt);

		CRect tRect(sPt+CSize(4,2),sPt+ss+CSize(4,2));
		CRect iRect(sPt,sPt+ss+CSize(8,4));
		//MemDC.Rectangle(rect3);

		int nBkMode = pDC->SetBkMode(TRANSPARENT);
		pDC->DrawEdge(iRect,EDGE_RAISED,BF_RECT   );

		pDC->DrawText(tt,&tRect, DT_VCENTER | DT_LEFT | DT_NOPREFIX | DT_WORDBREAK);
		m_datapos.Add(iRect);
		if (ePt.x < iRect.right) ePt = iRect.BottomRight();
		pDC->SetBkMode(nBkMode);
		sPt.Offset(0,ss.cy+6);
	}
	ePt.y=sPt.y;
	return mDrawRect = CRect(pSt,ePt);


}*/


/*CRect CViewSDI::DrawModels(CDC* pDC,CPoint sPt)
{
	CSimulDoc* pDoc = GetDocument();
	int nb2 = pDoc->m_pSimul->m_cEquSet.GetSize();
	CPoint	pSt = sPt,
			ePt(-10,-10);

	m_simpos.RemoveAll();
	////// MODELS
	for (int i=0;i<nb2;i++)
	{
		CString tt = pDoc->m_pSimul->m_cEquSet.GetAt(i)->GetSimulationName();

		CRect CpnRect = DrawIcon(pDC,sPt,1,tt,m_CurrModel == i,100);
		m_simpos.Add(CpnRect);
		if (ePt.x < CpnRect.right) ePt = CpnRect.BottomRight();

		sPt.Offset(0,20+CpnRect.Height());
	}
	ePt.y=sPt.y;
	return CRect(pSt,ePt);
}*/

/*CRect CViewSDI::DrawGrid(CDC *pDC, CPoint sPt)
{
	CRect modr = m_simpos.GetAt(m_CurrModel);
	CRect topr = m_simpos.GetAt(0);

	CString mstring("width");
	CSize ts = pDC->GetTextExtent(mstring);

	CPoint m0 = sPt;
	m0.y = topr.top;

	CPoint m1 = modr.CenterPoint();
	m1.x = sPt.x;

	CPoint m2 = modr.CenterPoint();
	m2.x = modr.right;

	pDC->MoveTo(m0);
	pDC->LineTo(m1);
	pDC->LineTo(sPt);

	pDC->MoveTo(m1);
	pDC->LineTo(m2);

	int nb = m_datapos.GetSize();
	for (int i=0;i<nb;i++)
	{
		CRect prect = m_datapos.GetAt(i);
		CPoint st = prect.CenterPoint();
		CPoint en = st;
		st.x=sPt.x;
		en.x=prect.left-2;
		pDC->MoveTo(st);
		pDC->LineTo(en);
	}

	return CRect();
}*/



CRect CViewSDI::DrawDef(CDC* pDC,CPoint sPt)
{
	CRect rect3(sPt,sPt+CSize(800,10));
	int nBkMode = pDC->SetBkMode( TRANSPARENT);

	UINT nFormat  = DT_VCENTER   | DT_LEFT| DT_EXPANDTABS   |DT_NOPREFIX ;//| DT_WORDBREAK;
	pDC->DrawText(m_CurrText,&rect3, DT_CALCRECT | nFormat);
//	nFormat |= DT_TABSTOP  | MAKEWORD(0,15);
	pDC->DrawText(m_CurrText,rect3, nFormat);
	pDC->SetBkMode(nBkMode);

	return rect3;
}

void CViewSDI::OnDraw(CDC* pDC)
{
	CMemDC MemDC(pDC);


	CSimulDoc* pDoc = GetDocument();
	if (!pDoc) 	return;

	CFont m_font;
	int nbf = m_font.CreatePointFont (80, _T ("MS Sans Serif"));

	CFont *oldF = MemDC.SelectObject(&m_font);
	DrawView(&MemDC);
	MemDC.SelectObject(oldF);
}



void CViewSDI::DrawView(CDC* pDC)
{
	CFont pfont,*oldf;
	pfont.CreateFontIndirect(&m_logFont);

	CSimulDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (!pDoc->m_pSimul) return;


	COLORREF bcolor = ::GetSysColor(COLOR_WINDOW/*COLOR_3DFACE*/); 
	CBrush FixedBack(bcolor);
	CRect ClipRect;
	if (/*!pDC->IsPrinting() && */(pDC->GetClipBox(ClipRect) != ERROR))
	{
		//pDC->LPtoDP(&ClipRect);
		ClipRect.InflateRect(1, 1); // avoid rounding to nothing
		pDC->FillRect(ClipRect,&FixedBack);
		//pDC->Rectangle(ClipRect);
	}


	CPoint sPt(20,20);

	const CMdEquation* pEquation = pDoc->m_pSimul->GetEquation(m_CurrModel);
	if (!pEquation) return ;


	int nb = pEquation->m_cParSet.GetSize();

	CRect rdef =	DrawDef(pDC, sPt);

	m_DocSize=CSize(rdef.right+20,rdef.bottom+20);
	SetScrollSizes( MM_TEXT, m_DocSize);
}

/////////////////////////////////////////////////////////////////////////////
// CViewSDI printing

void CViewSDI::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo) 
{
	// TODO: Add your specialized code here and/or call the base class
	CScrollView::OnPrepareDC(pDC, pInfo);
}

BOOL CViewSDI::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CViewSDI::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CViewSDI::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CViewSDI diagnostics

#ifdef _DEBUG
void CViewSDI::AssertValid() const
{
	CScrollView::AssertValid();
}

void CViewSDI::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CSimulDoc* CViewSDI::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSimulDoc)));
	return (CSimulDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CViewSDI message handlers


void CViewSDI::OnInitialUpdate() 
{
	CScrollView::OnInitialUpdate();

	if (m_DocSize.cx == -1)
	{
		CClientDC dc(this);
//		OnPrepareDC(&dc); // set up mapping mode and viewport origin

		m_DocSize=CSize(dc.GetDeviceCaps(HORZRES), dc.GetDeviceCaps(VERTRES));
		m_DocSize=CSize(800,600);
		SetScrollSizes( MM_TEXT, m_DocSize);
	}
	//ScrollToPosition(CPoint(0,300));
	// TODO: Add your specialized code here and/or call the base class
//	CSimulDoc* pDoc = GetDocument();
//	m_CurrModel = GetDocument()->m_nModSelItem;
//	m_CurrText = pDoc->m_pSimul->m_cEquSet.GetAt(m_CurrModel)->GetFormatedText(2);
}

void CViewSDI::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CScrollView::OnLButtonUp(nFlags, point);
}

void CViewSDI::OnRButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CScrollView::OnRButtonUp(nFlags, point);
}

void CViewSDI::OnEditCopy() 
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

	DrawView(&memDC);

	OpenClipboard() ;
	EmptyClipboard() ;
	SetClipboardData (CF_BITMAP, bitmap.GetSafeHandle() ) ;
	CloseClipboard () ;

	bitmap.Detach();

	EndWaitCursor();
	
}




void CViewSDI::OnPrint(CDC* pDC, CPrintInfo* pInfo) 
{
	// TODO: Add your specialized code here and/or call the base class
	pDC->SetMapMode(MM_ISOTROPIC);
	pDC->SetWindowExt(CSize(1200,600));//m_DocSize);
	pDC->SetViewportExt(pInfo->m_rectDraw.Width(),pInfo->m_rectDraw.Height());

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
	m_LogicalPageSize.cx = ScreenPixelsPerInch.cx * m_PaperSize.cx / PaperPixelsPerInch.cx * 3 / 4;
	m_LogicalPageSize.cy = ScreenPixelsPerInch.cy * m_PaperSize.cy / PaperPixelsPerInch.cy * 3 / 4;

	pDC->SetWindowExt(m_LogicalPageSize);//m_DocSize);

 //Cal->Setup(pDC,0);
 // SetCaracteristic(*pDC);
	CBrush mbr(RGB(255,255,255));
	//pDC->FillRect(pInfo->m_rectDraw,&mbr);
	DrawView(pDC);
	
//	CScrollView::OnPrint(pDC, pInfo);
}

BOOL CViewSDI::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	// TODO: Add your message handler code here and/or call default
	
	return CScrollView::OnSetCursor(pWnd, nHitTest, message);
}

void CViewSDI::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CScrollView::OnMouseMove(nFlags, point);
}

int CViewSDI::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CScrollView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	return 0;
}

void CViewSDI::OnSize(UINT nType, int cx, int cy) 
{
	CScrollView::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here

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
void CViewSDI::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	BOOL bRedraw = FALSE;
	// TODO: Add your specialized code here and/or call the base class
	if (lHint == DOC_UPDATE_MODEL || !lHint)
	{
		m_CurrModel = GetDocument()->m_nModSelItem;
		if (m_CurrModel>=GetDocument()->m_pSimul->m_cEquSet.GetSize()) return;
		CMdEquation *pEqu = GetDocument()->m_pSimul->m_cEquSet.GetAt(m_CurrModel);
		if (!pEqu) return;

		pEqu->GetFormatedText();
		int nbP = pEqu->m_cParSet.GetSize();
		int nbV = pEqu->m_cVarSet.GetSize();

		m_CurrText = _T("\r\n");
		m_CurrText +=  pEqu->GetFormatedEquation();
		
		pEqu->GetFormatedText(2);

		for (int i=0;i<nbP;i++)
		{
			CModelPar* pPar = pEqu->m_cParSet.GetAt(i);
			CString mstr = pPar->GetName(),
					mstr3 = pPar->GetDesc(),
					mstr2 = pPar->GetDef();

			CString mmmm;
			mmmm.Format(_T(" %s \t<=>  %s \t; %s \r\n"),mstr,mstr2,mstr3);

			m_CurrText +=mmmm;
		}
		m_CurrText += _T("\r\n");
		BOOL bDone = FALSE;
		for (i=0;i<nbV;i++)
		{
			CModelVar* pVar = pEqu->m_cVarSet.GetAt(i);
			CString mstr = pVar->GetName(),
					mstr3 = pVar->GetDesc(),
					mstr2 = pVar->GetDef();

			if (pVar->GetIcon() && !bDone)
			{
				m_CurrText += _T("\r\n");
				bDone = TRUE;
			}

			CString mmmm;
			mmmm.Format(_T(" %s \t<=>  %s \t; %s \r\n"),mstr,mstr2,mstr3);

			m_CurrText +=mmmm;
		}

		bRedraw = TRUE;
	}
	if (bRedraw)
	{
		Invalidate();
		UpdateWindow();
	}
}

void CViewSDI::OnSimulationUpdatemodels() 
{
	// TODO: Add your command handler code here
	
}
