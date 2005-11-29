// ViewModelDiag.cpp : implementation file
//

#include "stdafx.h"
#include "simul.h"

#include "SimulDoc.h"
#include "ViewModelDiag.h"

#include <MSimulation\PreyPredModel.h>
#include "GridCtrl\MemDC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CViewSDIdiag

IMPLEMENT_DYNCREATE(CViewSDIdiag, CScrollView)

CViewSDIdiag::CViewSDIdiag()
{
	m_nBitmapID = -1;
	m_nPrevMod = -1;
	m_rBound.SetRectEmpty();
}

CViewSDIdiag::~CViewSDIdiag()
{
}


BEGIN_MESSAGE_MAP(CViewSDIdiag, CScrollView)
	//{{AFX_MSG_MAP(CViewSDIdiag)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewSDIdiag drawing

void CViewSDIdiag::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	m_rBound.SetRectEmpty();
	// TODO: calculate the total size of this view
	SetScrollSizes(MM_TEXT, m_rBound.Size());
}

/////////////////////////////////////////////////////////////////////////////
// CViewSDIdiag diagnostics

#ifdef _DEBUG
void CViewSDIdiag::AssertValid() const
{
	CScrollView::AssertValid();
}

void CViewSDIdiag::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CSimulDoc* CViewSDIdiag::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSimulDoc)));
	return (CSimulDoc*)m_pDocument;
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CViewSDIdiag message handlers

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
void CViewSDIdiag::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	// TODO: Add your specialized code here and/or call the base class
	int nCurrModel = GetDocument()->m_nModSelItem;

	if (m_nPrevMod == nCurrModel) return;
	CMdEquation* pEq = GetDocument()->m_pSimul->GetEquation(nCurrModel);
	if (!pEq) return;

	m_nPrevMod = nCurrModel;
	Invalidate();
}

void CViewSDIdiag::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	CMemDC MemDC(pDC);

	CRect  ClipRect;
	CBrush br(::GetSysColor(COLOR_WINDOW));
	MemDC.GetClipBox(ClipRect);
	MemDC.FillRect(ClipRect,&br);

	CMdEquation* pEq = GetDocument()->m_pSimul->GetEquation(m_nPrevMod);
	if (pEq)
		m_rBound = pEq->DrawFlowChart(&MemDC);

	//if (!m_bDiag.GetSafeHandle()) return;
/*	if (m_nBitmapID == -1) 
	{
		return;
	}
	//HIMAGELIST pList = m_pImageList.GetSafeHandle();
	//if (pList)
	{
		//m_pImageList.Add(&m_bDiag,RGB(0,0,0));
		//m_pImageList.Draw(&MemDC,0,CPoint(0,0),ILD_NORMAL);
	//	m_pImageList.Draw(&MemDC pDC,0,CPoint(0,0),ILD_TRANSPARENT);
	}
	CBitmap tBitmap;
	BITMAP pBitMap;
	tBitmap.LoadBitmap(m_nBitmapID);
	tBitmap.GetBitmap(&pBitMap );

	CSize sz = CSize(pBitMap.bmWidth,pBitMap.bmHeight);
	CDC triMem;                  // memory device contexts

	triMem.CreateCompatibleDC(pDC);
	//CBitmap m_tribitmap;
	//int bv = m_tribitmap.CreateCompatibleBitmap(pDC, ClipRect.Width(), ClipRect.Height());
	CBitmap* pOldBmp2 = (CBitmap*) triMem.SelectObject(&tBitmap);

	MemDC.BitBlt(0, 0, sz.cx,sz.cy, &triMem,0,0,SRCCOPY);
	triMem.SelectObject(pOldBmp2);*/

}

void CViewSDIdiag::OnSize(UINT nType, int cx, int cy) 
{
	CScrollView::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	SetScrollSizes(MM_TEXT, CSize(m_rBound.BottomRight()+CPoint(20,20)));
}
