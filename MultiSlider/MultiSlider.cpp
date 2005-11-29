// MultiSlider.cpp : implementation file
//

#include "stdafx.h"
//#include "multisliderdemo.h"
#include "..\resource.h"
#include "MultiSlider.h"
#include "EditInterval.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMultiSlider

CMultiSlider::CMultiSlider()
{
	CWinApp *theApp = AfxGetApp();

	m_hCursorDelete = theApp->LoadCursor (IDC_DELETE);
	m_hCursorArrow = ::LoadCursor (NULL, IDC_ARROW);

	LOGFONT lf;
 	memset(&lf, 0, sizeof(LOGFONT));
 	_tcscpy(lf.lfFaceName, _T("Arial"));
 	lf.lfHeight = 12;
 	m_labelFont.CreateFontIndirect(&lf); 
	fFocus = false;
	m_nLabelWidth = 0;

	m_NumMarkers = 1;
	m_Colors.InsertAt(0, (UINT)RGB(255, 255, 255));

	m_TTRect.SetRectEmpty();

	fDisplayColors = false;
	fInit = false;
	fInteger = false;

	iToolTipEventDelay = 1000;
	uiToolTipEventId = 2;
	
	m_pdraggedMarker = NULL;
	m_pnewMarker = NULL;
	m_pSelectedMarker = NULL;
	m_pTTMarker = NULL;

	//m_wndInfo.Create (this, FALSE/* Shadow */);
	//m_wndInfo.SetOwner (this);
	//m_wndInfo.Hide ();

	//m_pImageList.Create(IDB_TIME_BREAKPOINT,16,0,RGB(255, 0, 255));
	m_pImageList.Create(IDB_TIME_SMBREAKPOINT,10,0,RGB(255, 0, 255));
}

CMultiSlider::~CMultiSlider()
{
    //m_wndInfo.DestroyWindow();
	CleanUp ();
	DestroyCursor (m_hCursorDelete);
}


BEGIN_MESSAGE_MAP(CMultiSlider, CSliderCtrl)
	//{{AFX_MSG_MAP(CMultiSlider)
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
	ON_WM_RBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMultiSlider message handlers

 void CMultiSlider::OnPaint() 
 {
 	CPaintDC dc(this); // device context for painting
	if(!fInit)
		Initialize();
 	
	CRect mainRect, rightRect, leftRect;
	CRect labelRect, intervalRect;
 	CString text;
	float rangemin, rangemax, rangewidth;
	float slidermin, slidermax, sliderwidth;
 	BOOL m_bMemDC = FALSE;
   	CDC* pDC = &dc;
   	CDC dcMem;
   	CBitmap bmp;
   	CBitmap* pOldBmp = NULL;
   	float i;
   	CPen* pOldPen;
   	CPen penDark(PS_SOLID, 1, GetSysColor(COLOR_3DDKSHADOW));
	int width;
	float x;
	CString str;
	CPoint pt, prevPt;
	POSITION pos;
	CRect rect;
   
   	//Try to create memory dc and bitmap
   	if(dcMem.CreateCompatibleDC(&dc) && bmp.CreateCompatibleBitmap(&dc, clientRect.Width(), 
		clientRect.Height()))
   	{
   		m_bMemDC = TRUE;
   		pOldBmp = dcMem.SelectObject(&bmp);
   		pDC = &dcMem;
   	}
   
   	pOldPen = (CPen*)pDC->SelectStockObject(NULL_PEN);
   
   	//Fill background, draw border and thumb
   	pDC->FillSolidRect(&clientRect, GetSysColor(COLOR_BTNFACE));
   	pDC->FillSolidRect(&channelRect, GetSysColor(COLOR_WINDOW));
   	pDC->DrawEdge(&channelRect, BDR_SUNKENOUTER|BDR_SUNKENINNER, BF_RECT);

  	if(fFocus)
   	{
		pDC->DrawFocusRect(&clientRect);
   	}
   
  //Set text properties
 	TEXTMETRIC tm;
   	pDC->SetBkMode(TRANSPARENT);
   	pDC->SetTextColor(GetSysColor(COLOR_BTNTEXT));
   	CFont* pOldFont = (CFont*)pDC->SelectObject(&m_labelFont);
 	pDC->GetTextMetrics (&tm);

	rangemin = m_numMin;
	rangemax = m_numMax;
	rangewidth = rangemax - rangemin + 1;
	slidermax = (float)sliderRect.right;
	slidermin = (float)sliderRect.left;
	sliderwidth = (float)sliderRect.Width();

	//Set min/max text
	if(m_nLabelWidth == 0)//Calculate maximum label width
	{
 		for(i=GetMinRange();i<=GetMaxRange();i+=1.0f)
 		{
			if(!fInteger)
				text.Format("%.1lf",i);
			else
				text.Format("%.0lf",i);
 			width = dc.GetTextExtent(text).cx;
 			if(width > m_nLabelWidth)
 			{
 				m_nLabelWidth = width;
 			}
 		}
	}

	int dist = clientRect.bottom - channelRect.bottom;
	labelRect.top = channelRect.bottom;
	labelRect.left = channelRect.left;
	labelRect.bottom = (int)RoundTo((float)labelRect.top + (float)dist/2.0f, 0);
   	labelRect.right = channelRect.left+m_nLabelWidth;

   	pDC->SelectObject(&penDark);
	pDC->SetTextAlign(TA_CENTER|TA_BOTTOM);

	BOOL bMarked = FALSE;
	for(i=rangemin;i<=rangemax;i+=(float)m_nAlignment)
	{
		//Calculate zero-based index of this tickmark
		x = (((i - rangemin) / (float)rangewidth) * (float)sliderwidth) + slidermin;
		x = RoundTo(x, 0);
   		pDC->MoveTo((int)x, labelRect.top);
   		pDC->LineTo((int)x, labelRect.bottom);
		if(i == rangemin || i == rangemax)
		{
			if(!fInteger)
				str.Format("%.1f",i);
			else
				str.Format("%.0lf",i);
			pDC->TextOut((int)x, labelRect.bottom + dist/2, str);
			if (i==rangemax) bMarked=TRUE;
		}
	}
	if (!bMarked)
	{
		x = (((rangemax - rangemin) / (float)rangewidth) * (float)sliderwidth) + slidermin;
		x = RoundTo(x, 0);
   		pDC->MoveTo((int)x, labelRect.top);
   		pDC->LineTo((int)x, labelRect.bottom);
		if(!fInteger)
			str.Format("%.1f",rangemax);
		else
			str.Format("%.0lf",rangemax);
		pDC->TextOut((int)x, labelRect.bottom + dist/2, str);
	}

	//Mark intervals
	for(pos=m_Markers.GetHeadPosition();pos != NULL;)
	{
		CMarker* pMarker = (CMarker*)m_Markers.GetNext(pos);
		ASSERT(pMarker != NULL);
//		if(pMarker != m_pdraggedMarker)
		{
			rect = pMarker->m_mainRect;
			x = (((pMarker->m_Pos - (float)rangemin) / (float)rangewidth) * (float)sliderwidth) + slidermin;
			x = RoundTo(x, 0);
   			pDC->MoveTo((int)x, labelRect.top);
   			pDC->LineTo((int)x, labelRect.bottom);
			if(!fInteger)
				str.Format("%.1f",pMarker->m_Pos);
			else
				str.Format("%.0lf",pMarker->m_Pos);
			pDC->TextOut((int)x, labelRect.bottom + dist/2, str);
		}
	}

	if(fDisplayColors)//Color Interval
	{
		int index = 0;
		intervalRect = channelRect;
		intervalRect.DeflateRect(2,2);
		x = (((0.0f) / (float)rangewidth) * (float)sliderwidth) + slidermin;
		intervalRect.left = (int)RoundTo(x, 0);
		for(pos=m_Markers.GetHeadPosition();pos != NULL;)
		{
			CMarker* pMarker = (CMarker*)m_Markers.GetNext(pos);
			ASSERT(pMarker != NULL);
			intervalRect.right = (int)RoundTo(pMarker->m_leftRect.left + 6.0f, 0);
			pDC->FillSolidRect(intervalRect, m_Colors.GetAt(index));
			intervalRect.left = pMarker->m_leftRect.right;
			index++;
		}
		//Last interval
		x = (((rangemax - rangemin) / (float)rangewidth) * (float)sliderwidth) + slidermin;
		intervalRect.right = (int)RoundTo(x, 0);
		pDC->FillSolidRect(intervalRect, m_Colors.GetAt(index));
	}

	//Draw thumb
	for(pos=m_Markers.GetHeadPosition();pos != NULL;)
	{
		CMarker* pMarker = (CMarker*)m_Markers.GetNext(pos);
		ASSERT(pMarker != NULL);
		mainRect = pMarker->m_mainRect;
		rightRect = pMarker->m_rightRect;
		leftRect = pMarker->m_leftRect;

		CPoint mtr = mainRect.TopLeft();
		mtr.y += (mainRect.Height()-12)/2;

		//->AbortDoc';
	  	pDC->DrawEdge(&mainRect, BDR_RAISEDOUTER|BDR_RAISEDINNER, BF_MIDDLE|BF_LEFT|BF_TOP|BF_RIGHT);
		pDC->DrawEdge(&leftRect, BDR_RAISEDOUTER|BDR_RAISEDINNER, BF_DIAGONAL_ENDTOPLEFT|BF_MIDDLE);
		pDC->DrawEdge(&rightRect, BDR_RAISEDOUTER|BDR_RAISEDINNER, BF_DIAGONAL_ENDBOTTOMLEFT|BF_MIDDLE);
		//m_pImageList.Draw(pDC,pMarker->m_nType,mainRect.TopLeft(),ILD_TRANSPARENT);
		m_pImageList.DrawIndirect(pDC,pMarker->m_nType,mtr,CSize(10,12),CPoint(0,0),ILD_TRANSPARENT);
	}

	//Restore DC settings
	pDC->SelectObject(pOldPen);

	if(m_bMemDC)
	{
		CRect clipRect;
		dc.GetClipBox(&clipRect);
		dc.BitBlt(clipRect.left, clipRect.top, clipRect.Width(), clipRect.Height(),
   			&dcMem, clipRect.left, clipRect.top, SRCCOPY);
		dcMem.SelectObject(pOldBmp);
	}	
	// Do not call CSliderCtrl::OnPaint() for painting messages
}

void CMultiSlider::OnSetFocus(CWnd* pOldWnd) 
{
	CSliderCtrl::OnSetFocus(pOldWnd);
	
	fFocus = true;
	
}

void CMultiSlider::OnKillFocus(CWnd* pNewWnd) 
{
	CSliderCtrl::OnKillFocus(pNewWnd);
	
	fFocus = false;
	
}

void CMultiSlider::OnLButtonDown(UINT nFlags, CPoint point) 
{
	float rangemin, rangemax, rangewidth;
	float slidermin, slidermax, sliderwidth;
	CRect tempRect;
	CString strText;
	CPoint pt;
	float x;
	
	slidermin = (float)sliderRect.left;
	slidermax = (float)sliderRect.right;
	sliderwidth = (float)sliderRect.Width();
	rangemin = m_numMin;
	rangemax = m_numMax;
	rangewidth = rangemax - rangemin + 1;

	//m_wndInfo.Hide ();
	m_TTRect.SetRectEmpty ();

	if(sliderRect.PtInRect(point))
	{
		m_pdraggedMarker = MarkerHitTest(point);

		if(m_pdraggedMarker == NULL && m_Markers.GetCount() < m_NumMarkers)	//Adding interval
		{
			m_pnewMarker = new CMarker;
			x = ((float)(point.x - slidermin)/(float)sliderwidth * (float)rangewidth) + rangemin;
			x = RoundTo(x, 0); //// NVL 1
			m_pnewMarker->m_Pos = x;
			SetRectangles(m_pnewMarker);
		}
		else
			GetMinMax(point);
		/*BuildTooltipText(m_pdraggedMarker, TRUE, strText);

		::GetCursorPos (&pt);
		pt += CPoint (
					GetSystemMetrics (SM_CXCURSOR) / 2, 
					GetSystemMetrics (SM_CYCURSOR) / 2);
		m_wndInfo.Track (pt, strText);*/
	}
	
	CSliderCtrl::OnLButtonDown(nFlags, point);
}

void CMultiSlider::OnLButtonUp(UINT nFlags, CPoint point) 
{
	float x;
	float rangemin, rangemax, rangewidth;
	float slidermin, slidermax, sliderwidth;

	SetFocus ();
	//m_wndInfo.Hide ();
	::SetCursor (m_hCursorArrow);

	if (m_pdraggedMarker != NULL)
	{
		ReleaseCapture ();
	}
	slidermin = (float)sliderRect.left;
	slidermax = (float)sliderRect.right;
	sliderwidth = (float)sliderRect.Width();
	rangemin = m_numMin;
	rangemax = m_numMax;
	rangewidth = rangemax - rangemin + 1;
	x = ((float)(point.x - slidermin)/(float)sliderwidth * (float)rangewidth) + rangemin;
	x = RoundTo(x, 0);  /// NVL 1
	if(m_pnewMarker != NULL && channelRect.PtInRect(point))
	{
		if(x <= rangemax && x >= rangemin)
		{
			InsertMarker(m_pnewMarker);
			Invalidate();
		}
		m_pnewMarker = NULL;
	}
	else if(m_pdraggedMarker != NULL)
	{
		if(!clientRect.PtInRect(point))
		{
			RemoveLevelMarker(m_pdraggedMarker);
			Invalidate();
		}
		else if(x <= rangemax && x >= rangemin)
		{
			CString strText;
			CPoint pt;
			CMarker* pMarker = new CMarker;
			pMarker->m_Pos = x;
			pMarker->m_nType = m_pdraggedMarker->m_nType;
			pMarker->m_strHTML = m_pdraggedMarker->m_strHTML;
			pMarker->m_strDesc= m_pdraggedMarker->m_strDesc;

			SetRectangles(pMarker);
			RemoveLevelMarker(m_pdraggedMarker);
			InsertMarker(pMarker);
			Invalidate();

/*			BuildTooltipText(pMarker, TRUE, strText);

			::GetCursorPos (&pt);
			pt += CPoint (
						GetSystemMetrics (SM_CXCURSOR) / 2, 
						GetSystemMetrics (SM_CYCURSOR) / 2);
			m_wndInfo.Track (pt, strText);
*/
		}
		m_pdraggedMarker = NULL;
	}
	
	CSliderCtrl::OnLButtonUp(nFlags, point);
}

CMarker* CMultiSlider::MarkerHitTest(CPoint pt)
{
	float x;
	CRect mainRect;
	float slidermin, slidermax, sliderwidth;
	float rangemin, rangemax, rangewidth;
	POSITION pos;

	slidermin = (float)sliderRect.left;
	slidermax = (float)sliderRect.right;
	sliderwidth = (float)sliderRect.Width();
	rangemin = m_numMin;
	rangemax = m_numMax;
	rangewidth = rangemax - rangemin + 1;

	if(m_Markers.GetCount() == 0)
		return NULL;

	for(pos = m_Markers.GetHeadPosition();pos!=NULL;)
	{
		CMarker* pMarker = (CMarker*)m_Markers.GetNext(pos);
		x = (((pMarker->m_Pos - (float)rangemin) / (float)rangewidth) * (float)sliderwidth) + slidermin;
		mainRect = pMarker->m_rightRect;
		mainRect += pMarker->m_mainRect;
		mainRect += pMarker->m_leftRect;
		if(pMarker->m_rightRect.PtInRect(pt)||pMarker->m_leftRect.PtInRect(pt)
			||pMarker->m_mainRect.PtInRect(pt))
		{
			return pMarker;
		}
	}

	return NULL;
}

void CMultiSlider::OnMouseMove(UINT nFlags, CPoint point) 
{
	CSliderCtrl::OnMouseMove(nFlags, point);
	float x;
	float rangemin, rangemax, rangewidth;
	float slidermin, slidermax, sliderwidth;
	CRect invalidRect;
	CMarker newMarker;

	if(m_pdraggedMarker == NULL)
	{
		SetInfoWindow (point);
		return;
	}

	if(!clientRect.PtInRect(point))
	{
		//m_wndInfo.Hide ();
		::SetCursor (m_hCursorDelete);
		return;
	}
	else
		::SetCursor (m_hCursorArrow);


	slidermin = (float)sliderRect.left;
	slidermax = (float)sliderRect.right;
	sliderwidth = (float)sliderRect.Width();
	rangemin = m_numMin;
	rangemax = m_numMax;
	rangewidth = rangemax - rangemin + 1;
	CRect oldRect, newRect;

	if(m_pdraggedMarker != NULL)
	{
		if(clientRect.PtInRect(point)) // sliderRect
		{
			x = ((float)(point.x - slidermin)/(float)sliderwidth * (float)rangewidth) + (float)rangemin;
			x = RoundTo(x, 0); /// NVL 1
			if (x < rangemin) x = rangemin;	/// NVL
			if (x > rangemax) x = rangemax;	/// NVL
			if(x >= rangemin && x <= rangemax)
			{
				oldRect = m_pdraggedMarker->m_mainRect;
				oldRect.bottom = clientRect.bottom;
				newMarker.m_Pos = x;
				SetRectangles(&newMarker);
				newRect = newMarker.m_mainRect;
				newRect.bottom = clientRect.bottom;
				m_pdraggedMarker->m_leftRect = newMarker.m_leftRect;
				m_pdraggedMarker->m_rightRect = newMarker.m_rightRect;
				m_pdraggedMarker->m_mainRect = newMarker.m_mainRect;
				m_pdraggedMarker->m_Pos = newMarker.m_Pos;
				if(oldRect != newRect)
				{
					invalidRect.top = min(oldRect.top, newRect.top);
					invalidRect.bottom = max(oldRect.bottom, newRect.bottom);
					invalidRect.left = min(oldRect.left, newRect.left) - 6;
					invalidRect.right = max(oldRect.right, newRect.right) + 6;
					InvalidateRect(invalidRect);
					UpdateWindow();
					SetInfoWindow (point);
				}
			}

/*			CString strText;
			CPoint pt;
			BuildTooltipText(m_pdraggedMarker, TRUE, strText);

			::GetCursorPos (&pt);
			pt += CPoint (
						GetSystemMetrics (SM_CXCURSOR) / 2, 
						GetSystemMetrics (SM_CYCURSOR) / 2);
			m_wndInfo.Track (pt, strText);
*/
		}
	}
}

void CMultiSlider::BuildTooltipText(CMarker* pMI, bool bShowTip, CString &text)
{
	if(pMI == NULL)
		return;

	text = _T("");
 
	if (bShowTip)
	{
		if(!fInteger)
		{
			text.Format(_T("%.1f"), pMI->m_Pos);
		}
		else
		{
			text.Format(_T("%.0f"), pMI->m_Pos);
		}
	}
	else
	{
		//m_wndInfo.Hide ();
	}

}

void CMultiSlider::SetInfoWindow(CPoint &point)
{
	CRect rectI;
	BOOL bShowInfo = TRUE;
	CMarker* pMI;

	if (m_pdraggedMarker != NULL)
	{
		CString strText;
		BuildTooltipText (m_pdraggedMarker, TRUE, strText);
		//m_wndInfo.SetWindowText (strText);
		//m_wndInfo.ShowWindow(SW_SHOWNOACTIVATE);
		//m_wndInfo.Invalidate ();
		//m_wndInfo.UpdateWindow ();
		return;
	}

	if (m_TTRect.PtInRect (point))
	{
		return;
	}

	m_TTRect.SetRectEmpty();
	if ((pMI = MarkerHitTest(point)) == NULL)
	{
		bShowInfo = FALSE;
	}
	else
	{
		rectI = GetMarkerRect(pMI);
		bShowInfo = rectI.PtInRect (point);
	}

	if (!bShowInfo)
	{
		ReleaseCapture ();
		//m_wndInfo.Hide ();

		KillTimer (uiToolTipEventId);
		rectI.SetRectEmpty ();
	}
	else
	{
		SetTimer (uiToolTipEventId, iToolTipEventDelay, NULL);
	}

	m_TTRect = rectI;
	m_pTTMarker = pMI;
}


void CMultiSlider::OnTimer(UINT nIDEvent) 
{
	return;
 	CPaintDC dc(this);
	CPoint pt;

	if (m_pTTMarker != NULL)
	{
		CString strText;
		TEXTMETRIC tm;
		CSize length;
		int width;

		dc.GetTextMetrics(&tm);
		int nTextHeight = tm.tmHeight + 2;
		int nTextWidth = tm.tmMaxCharWidth + 2;

		if(!fInteger)
		{
			strText.Format("%.1f", m_pTTMarker->m_Pos);
		}
		else
		{
			strText.Format("%.0f", m_pTTMarker->m_Pos);
		}
		length = dc.GetTextExtent(strText);
		width = length.cx;

/*		BuildTooltipText(m_pTTMarker, TRUE, strText);

		CPoint ptCursor;
		::GetCursorPos (&ptCursor);
		ptCursor += CPoint (
					GetSystemMetrics (SM_CXCURSOR) / 2, 
					GetSystemMetrics (SM_CYCURSOR) / 2);
		m_wndInfo.Track (ptCursor, strText);
*/
		SetCapture ();
	}
	else
	{
		//m_wndInfo.Hide ();
		m_TTRect.SetRectEmpty ();
	}

	KillTimer (uiToolTipEventId);	

	CSliderCtrl::OnTimer(nIDEvent);
}

void CMultiSlider::Initialize()
{
	float max, min, width;
	CPoint pt;
	POSITION pos;
	
	CPaintDC dc(this);
	dc.SetMapMode(MM_TEXT);

	//Get control rect, channel rect and thumb rect
   	GetClientRect(&clientRect);
   	GetChannelRect(&channelRect);
   	GetThumbRect(&thumbRect);

	min = m_numMin;
	max = m_numMax;
	//Get tickmark information
 	m_numTicks = GetNumTics();
   	m_nAlignment = (int)((max - min) / (GetNumTics()-1) + .5);
	width = max-min + 1;

	sliderRect = channelRect;
	sliderRect.DeflateRect(2, 0);

	for(pos = m_Markers.GetHeadPosition();pos!=NULL;)
	{
		CMarker* pMarker = (CMarker*)m_Markers.GetNext(pos);
		if(pMarker->m_Pos > m_numMax || pMarker->m_Pos < m_numMin)//Remove any out of range
			RemoveLevelMarker(pMarker);
		else
			SetRectangles(pMarker);	//Reset rectangles
	}
		
	fInit = true;
}

float CMultiSlider::RoundTo(float value, short decimal)
{
	value *= (float)pow(10.0, decimal);
	value += .5f;
	value = (float)floor(value);
	value /= (float)pow(10.0, decimal);
	return value;
}

bool CMultiSlider::SetRanges(float min, float max)
{
	m_numMax = max;
	m_numMin = min;

	fInit = false;

	return true;
}

bool CMultiSlider::GetRanges(float &min, float &max)
{
	min = m_numMin;
	max = m_numMax;

	return true;
}

float CMultiSlider::GetMinRange()
{
	return m_numMin;
}

float CMultiSlider::GetMaxRange()
{
	return m_numMax;
}

bool CMultiSlider::RemoveLevelMarker(CMarker *pRemoved)
{
	ASSERT (pRemoved != NULL);

	CRect rectTI;
	rectTI = pRemoved->m_mainRect;
	rectTI += pRemoved->m_leftRect;
	rectTI += pRemoved->m_rightRect;

	for (POSITION pos = m_Markers.GetHeadPosition (); pos != NULL;)
	{
		POSITION posSave = pos;

		CMarker* pTI = (CMarker*) m_Markers.GetNext (pos);
		ASSERT (pTI != NULL);

		if (pTI == pRemoved)
		{
			m_Markers.RemoveAt (posSave);
			delete pRemoved;

			InvalidateRect(rectTI);
			return true;
		}
	}

	return false;
}

int CMultiSlider::InsertMarker(CMarker *pNewMarker)
{
	bool bInserted = false;
	int i = 0;
	int iIndex =  -1;

	for(POSITION pos = m_Markers.GetHeadPosition(); pos != NULL && !bInserted; i++)
	{
		POSITION posSave = pos;
		CMarker* pMarker = (CMarker*)m_Markers.GetNext(pos);
		ASSERT(pMarker != NULL);

		if(pMarker->m_Pos >= pNewMarker->m_Pos)
		{
			m_Markers.InsertBefore(posSave, pNewMarker);
			iIndex = i;
			bInserted = true;
		}
	}
	
	if(!bInserted)
	{
		m_Markers.AddTail(pNewMarker);
		iIndex = m_Markers.GetCount() - 1;
		CString mstr(_T(""));
		//m_cStrDesc.SetAtGrow(iIndex,mstr);
		//m_cIntStyle.SetAtGrow(iIndex,0);
	}

	return iIndex;
}

void CMultiSlider::CleanUp()
{
	while (!m_Markers.IsEmpty ())
	{
		delete m_Markers.RemoveHead ();
	}

	m_pnewMarker = NULL;
	m_pdraggedMarker = NULL;
	m_pSelectedMarker = NULL;
	m_pTTMarker = NULL;

	m_Colors.RemoveAll();
}

void CMultiSlider::SetRectangles(CMarker *pMarker)
{
	float rangemin, rangemax, rangewidth;
	float slidermin, slidermax, sliderwidth;
	CRect top, left, right;
	float x;

	rangemin = m_numMin;
	rangemax = m_numMax;
	rangewidth = rangemax - rangemin + 1;
	slidermax = (float)sliderRect.right;
	slidermin = (float)sliderRect.left;
	sliderwidth = (float)sliderRect.Width();
	x = (((pMarker->m_Pos - (float)rangemin) / (float)rangewidth) * (float)sliderwidth) + slidermin;
	x = RoundTo(x, 0);
	top = thumbRect;
	top.left = (int)(x - 6.0f);
	top.right = (int)(x + 6.0f);
	top.bottom = channelRect.bottom;
	left = thumbRect;
	left.top = channelRect.bottom;
	left.left = top.left;
	left.right = (int)x;
	right = thumbRect;
	right.top = channelRect.bottom;
	right.left = (int)x;
	right.right = top.right;

	pMarker->m_leftRect = left;
	pMarker->m_mainRect = top;
	pMarker->m_rightRect = right;
}

int CMultiSlider::GetNumMarkers()
{
	return m_Markers.GetCount();
}

int CMultiSlider::GetMarkers(float *value)
{
	int count = 0;
	POSITION pos;

	for(pos=m_Markers.GetHeadPosition();pos != NULL;)
	{
		CMarker* pMarker = (CMarker*)m_Markers.GetNext(pos);
		ASSERT(pMarker != NULL);
		value[count] = pMarker->m_Pos;
		count++;
	}

	return count;
}

CRect CMultiSlider::GetMarkerRect(CMarker *pMI)
{
	CRect rect;

	rect = pMI->m_mainRect;
	rect.bottom = clientRect.bottom;

	return rect;
}

void CMultiSlider::OnRButtonUp(UINT nFlags, CPoint point) 
{
	float rangemin, rangemax, rangewidth;
	float slidermin, slidermax, sliderwidth;
	CRect tempRect;
	CString strText;
	CPoint pt;
	CEditInterval dlg;
	float x;
	
	slidermin = (float)sliderRect.left;
	slidermax = (float)sliderRect.right;
	sliderwidth = (float)sliderRect.Width();
	rangemin = m_numMin;
	rangemax = m_numMax;
	rangewidth = rangemax - rangemin + 1;

//	m_wndInfo.Hide ();
	m_TTRect.SetRectEmpty ();

	if(sliderRect.PtInRect(point))	//Verify in window
	{
		m_pSelectedMarker = MarkerHitTest(point);

		if(m_pSelectedMarker != NULL)//Editing
		{
			POSITION pos;
			int i=0;
			for(pos=m_Markers.GetHeadPosition();pos != NULL;)
			{
				CMarker* pMar = (CMarker*)m_Markers.GetNext(pos);
				if (pMar == m_pSelectedMarker)
					break;
				i++;
			}

			dlg.m_strDescr = m_pSelectedMarker->m_strDesc;
			dlg.m_nStyle = m_pSelectedMarker->m_nType;
			dlg.m_strHTML = m_pSelectedMarker->m_strHTML;
			dlg.m_Value = m_pSelectedMarker->m_Pos;
			dlg.m_Min = rangemin;
			dlg.m_Max = rangemax;
			if(dlg.DoModal() == IDOK)
			{
				m_pSelectedMarker->m_Pos = dlg.m_Value;
				m_pSelectedMarker->m_strDesc = dlg.m_strDescr;
				m_pSelectedMarker->m_strHTML = dlg.m_strHTML;
				m_pSelectedMarker->m_nType = dlg.m_nStyle;
				SetRectangles(m_pSelectedMarker);
				Invalidate();
				x = (((float)(m_pSelectedMarker->m_Pos - rangemin) / (float)rangewidth) * (float)sliderwidth) + slidermin;
				x = RoundTo(x, 0);
/*				BuildTooltipText(m_pSelectedMarker, TRUE, strText);
				::GetCursorPos (&pt);
				pt += CPoint (
							GetSystemMetrics (SM_CXCURSOR) / 2, 
							GetSystemMetrics (SM_CYCURSOR) / 2);
				m_wndInfo.Track (pt, strText);
*/
//				m_cStrDesc.SetAt(i,dlg.m_strDescr);
//				m_cIntStyle.SetAt(i,dlg.m_nStyle) ;

			}
			m_pSelectedMarker = NULL;
		}
		else if(fDisplayColors)	//Changing color of interval
		{
			int i, count, set;
			float pt;
			float *values;

			values = (float*)malloc(sizeof(float) * m_NumMarkers);
			pt = ((float)(point.x - slidermin)/(float)sliderwidth * (float)rangewidth) + rangemin;
			pt = RoundTo(pt, 1);
			count = GetMarkers(values);
			set = -1;
			if(pt <= values[0])
				set = 0;
			else if(pt >= values[count-1])
				set = count;
			else
			{
				for(i=0;i<=count;i++)
				{
					if(pt >= values[i] && pt <= values[i+1])
					{
						set = i+1;
						break;
					}
				}
			}
			if(set != -1)
			{
				CColorDialog dlg(m_Colors.GetAt(set), 0, NULL);
				if(dlg.DoModal() == IDOK)
				{
					m_Colors.SetAt(set, dlg.GetColor());
					Invalidate();
				}
			}
			free(values);
		}
	}	
	CSliderCtrl::OnRButtonUp(nFlags, point);
}

void CMultiSlider::GetMinMax(CPoint point)
{
	int i, count;
	float x;
	float rangemin, rangemax, rangewidth;
	float slidermin, slidermax, sliderwidth;
	float *values;

	values = (float*)malloc(sizeof(float) * m_NumMarkers);

	slidermin = (float)sliderRect.left;
	slidermax = (float)sliderRect.right;
	sliderwidth = (float)sliderRect.Width();
	rangemin = m_numMin;
	rangemax = m_numMax;
	rangewidth = rangemax - rangemin + 1;


	x = ((float)(point.x - slidermin)/(float)sliderwidth * (float)rangewidth) + (float)rangemin;
	x = RoundTo(x, 1);

	prevMin = rangemin;
	prevMax = rangemax;

	count = GetMarkers(values);
	for(i=0;i<count;i++)
	{
		if(values[i] < x)
			prevMin = values[i];
	}
	for(i=count-1;i>=0;i--)
	{
		if(values[i] > x)
			prevMax = values[i];
	}

	free(values);
	return;
}
