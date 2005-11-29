/////////////////////////////////////////////////////////////////////////////
// Copyright (C) 1999 by Pedro Pombeiro
// All rights reserved
//
// Distribute freely, except: don't remove my name from the source or
// documentation (don't take credit for my work), mark your changes (don't
// get me blamed for your possible bugs), don't alter or remove this
// notice.
// No warrantee of any kind, express or implied, is included with this
// software; use at your own risk, responsibility for damages (if any) to
// anyone resulting from the use of this software rests entirely with the
// user.
//
// Send bug reports, bug fixes, enhancements, requests, flames, etc., and
// I'll try to keep a version up to date.  I can be reached as follows:
//    PPombeiro@AntaresLda.pt         (company mail account)
/////////////////////////////////////////////////////////////////////////////


// SelectionSliderCtrl.cpp : implementation file
//

#include "StdAFX.h"
#include "SelSliderCtrl.h"
#include "GridCtrl\MemDC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CSelSliderCtrl

CSelSliderCtrl::CSelSliderCtrl()
{
	m_bSelecting		= FALSE;
	m_bAnchorIsValid	= FALSE;
	m_iAnchorSelection	= -1;
	m_nCurrBreakPoint = -1;

	m_crBackGrd = GetSysColor(COLOR_HIGHLIGHT);
	m_crSelect = GetSysColor(COLOR_WINDOW);

}

CSelSliderCtrl::~CSelSliderCtrl()
{
}


BEGIN_MESSAGE_MAP(CSelSliderCtrl, CSliderCtrl)
	//{{AFX_MSG_MAP(CSelSliderCtrl)
	ON_WM_HSCROLL_REFLECT()
	ON_WM_VSCROLL_REFLECT()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_KEYDOWN()
	ON_WM_HSCROLL()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSelSliderCtrl message handlers

void CSelSliderCtrl::HScroll(UINT nTBCode, UINT /*nPos*/)
{
/*	if (GetStyle() & TBS_ENABLESELRANGE)
	{
		BOOL	bEndScroll	= FALSE;
		int		iPos		= GetPos();
		int		iPrevPos	= iPos;

		switch (nTBCode)
		{
			case TB_PAGEUP		: iPrevPos	= iPos + GetPageSize(); break;
			case TB_PAGEDOWN	: iPrevPos	= iPos - GetPageSize(); break;
			case TB_LINEUP		: iPrevPos	= iPos + GetLineSize(); break;
			case TB_LINEDOWN	: iPrevPos	= iPos - GetLineSize(); break;
		}

		switch (nTBCode)
		{
			case TB_ENDTRACK		:
				bEndScroll	= TRUE;
				break;
			case TB_PAGEUP			:
			case TB_PAGEDOWN		:
			case TB_LINEUP			:
			case TB_LINEDOWN		:
			case TB_THUMBPOSITION	:
			case TB_THUMBTRACK		:
			{
				const BOOL	bShiftIsPressed	= (::GetKeyState(VK_SHIFT) < 0);

				if (!m_bSelecting && !bShiftIsPressed)
				{
					m_iAnchorSelection	= -1;
					m_bAnchorIsValid	= FALSE;
				}
				if (!m_bSelecting && !m_bAnchorIsValid)
				{
					int	iMin, iMax;
					GetSelection(iMin, iMax);
					if (iMin != -1 || iMax != -1)
						ClearSel(TRUE);
				}
				if (!m_bSelecting && bShiftIsPressed)
				{
					m_bSelecting	= TRUE;
					if (!m_bAnchorIsValid)
					{
						m_iAnchorSelection	= iPrevPos;
						m_bAnchorIsValid	= TRUE;
					}
				}
				if (m_bSelecting)
				{
					SetSelection(min(m_iAnchorSelection, iPos), max(m_iAnchorSelection, iPos));
					Invalidate(FALSE);
				}
				break;
			}
			default	:
				//ASSERT(FALSE);
				break;
		}

		if (bEndScroll)
		{
			if (!m_bSelecting || ::GetKeyState(VK_SHIFT) >= 0)
			{
				m_bAnchorIsValid	= FALSE;
				m_iAnchorSelection	= -1;
			}
			m_bSelecting	= FALSE;
		}
	}*/
}

void CSelSliderCtrl::VScroll(UINT nTBCode, UINT nPos)
{
	HScroll(nTBCode, nPos);
}

void CSelSliderCtrl::OnPaint() 
{
	// TODO: Add your message handler code here
	CPaintDC dc(this); // device context for painting
    CMemDC MemDC(&dc);
	
	CRect labelRect, intervalRect;
	CString text;
	int rangemin, rangemax, rangewidth;
	int slidermin, slidermax, sliderwidth;
	CString str;
//	CPoint pt, prevPt;
//	CRect rect;

	CRect clientRect,channelRect,thumbRect;

   	GetClientRect(&clientRect);
   	GetChannelRect(&channelRect);
   	GetThumbRect(&thumbRect);

	GetRange(rangemin, rangemax ) ;
	rangewidth = rangemax - rangemin;// + 1;
	int nbPos = GetPos();

	CRect sliderRect = channelRect;
	sliderRect.DeflateRect(2, 0);

	slidermax = sliderRect.right;
	slidermin = sliderRect.left;
	sliderwidth = sliderRect.Width();

	CPen* pOldPen;
	CPen penDark(PS_SOLID, 1, GetSysColor(COLOR_3DDKSHADOW));

	LOGFONT lf;
	CFont m_labelFont;
	CFont m_CurrlabelFont;
 	memset(&lf, 0, sizeof(LOGFONT));
 	_tcscpy(lf.lfFaceName, _T("Small Fonts"));
 	lf.lfHeight = -9;
 	m_labelFont.CreateFontIndirect(&lf); 
	lf.lfWeight = FW_BOLD;
 	m_CurrlabelFont.CreateFontIndirect(&lf); 


	pOldPen = (CPen*)MemDC.SelectStockObject(NULL_PEN);
   
   	//Fill background, draw border and thumb
   	MemDC.FillSolidRect(&clientRect, GetSysColor(COLOR_BTNFACE));
   	MemDC.FillSolidRect(&channelRect,GetSysColor(COLOR_WINDOW));

	// Draw Selection area
	{
		int selMin,selMax;
		double ddd= .5 * sliderwidth / rangewidth;
		GetSelection(selMin,selMax );
		int hh1 =  (int)(((selMin - rangemin) / (float)rangewidth) * (float)sliderwidth) + slidermin;
		int hh2=  (int)(((selMax - rangemin) / (float)rangewidth) * (float)sliderwidth) + slidermin - ddd;
		int hh3 = (int)(((selMax - rangemin) / (float)rangewidth) * (float)sliderwidth) + slidermin + ddd;
		int hh4=  (int)(((rangemax - rangemin) / (float)rangewidth) * (float)sliderwidth) + slidermin;
		CRect selRect = channelRect;
		selRect.left = hh1;
		selRect.right= hh2;
		selRect.DeflateRect(0,3,-1,3);
		//MemDC.FillSolidRect(&selRect, GetSysColor(COLOR_HIGHLIGHT));
	  	MemDC.FillSolidRect(&selRect, m_crSelect);
		CRect selRect2 = channelRect;
		selRect2.left = hh3;
		selRect2.right= hh4;
		selRect2.DeflateRect(0,3,-1,3);
	  	MemDC.FillSolidRect(&selRect2, m_crBackGrd);
	}


   	MemDC.DrawEdge(&channelRect, BDR_SUNKENOUTER|BDR_SUNKENINNER, BF_RECT);
   
	//Set text properties
 	TEXTMETRIC tm;
   	MemDC.SetBkMode(TRANSPARENT);
   	MemDC.SetTextColor(GetSysColor(COLOR_BTNTEXT));
   	CFont* pOldFont = (CFont*)MemDC.SelectObject(&m_labelFont);
 	MemDC.GetTextMetrics (&tm);

	//Set min/max text
	int m_nLabelWidth =0;
	if(m_nLabelWidth == 0)//Calculate maximum label width
	{
 		for(int i=rangemin;i<=rangemax;i++)
 		{
			text.Format("%.1lf",i);
 			int width = dc.GetTextExtent(text).cx;
 			if(width > m_nLabelWidth)
 			{
 				m_nLabelWidth = width;
 			}
 		}
	}

	int dist = clientRect.bottom - channelRect.bottom;
	labelRect.top = channelRect.bottom;
	labelRect.left = channelRect.left;
	labelRect.bottom = labelRect.top + dist/2;
   	labelRect.right = channelRect.left+m_nLabelWidth;

	int nbBP = m_cBreakPt.GetSize();
	UINT rectright=channelRect.left;
	for (int i= 0;i<nbBP;i++)
	{
		int poss =m_cBreakPt.GetAt(i); 
		int hh =  (int)(((poss - rangemin) / (float)rangewidth) * (float)sliderwidth) + slidermin;
//
//		(poss*channelRect.Width()) / rangewidth;
		CRect myrect = channelRect;
		myrect.left = rectright;
		myrect.right = hh+1;
	   	MemDC.DrawEdge(&myrect, BDR_SUNKENOUTER|BDR_SUNKENINNER, BF_RECT);

		//x = RoundTo(x, 0);
	   	MemDC.SelectObject(&penDark);
   		MemDC.MoveTo((int)hh, labelRect.top);
   		MemDC.LineTo((int)hh, labelRect.bottom);
		MemDC.SetTextAlign(TA_CENTER|TA_BOTTOM);
		str.Format("%d",poss);
		MemDC.TextOut((int)hh, labelRect.bottom + dist/2, str);
	   	MemDC.SelectObject(pOldPen);

		rectright = hh+1;
	}
	if (nbBP)
	{
		CRect myrect = channelRect;
		myrect.left = rectright;
	   	MemDC.DrawEdge(&myrect, BDR_SUNKENOUTER|BDR_SUNKENINNER, BF_RECT);
	}

   	MemDC.SelectObject(&penDark);
	MemDC.SetTextAlign(TA_CENTER|TA_BOTTOM);

   	int m_nAlignment = GetNumTics()-1;
	m_nAlignment = (rangemax - rangemin) / m_nAlignment;// +1;

	for(i=rangemin;i<=rangemax;i+=m_nAlignment)
	{
		//Calculate zero-based index of this tickmark
		int xPos = (int)(((i - rangemin) / (float)rangewidth) * (float)sliderwidth) + slidermin;
		//x = RoundTo(x, 0);
   		MemDC.MoveTo(xPos, labelRect.top);
   		MemDC.LineTo(xPos, labelRect.bottom);
		if(i == rangemin || i == rangemax)
		{
			str.Format("%d",i);
			MemDC.TextOut(xPos, labelRect.bottom + dist/2, str);
		}
	}
	if (i != rangemax)
	{
		int xPos = (int)(((rangemax - rangemin) / (float)rangewidth) * (float)sliderwidth) + slidermin;
   		MemDC.MoveTo(xPos, labelRect.top);
   		MemDC.LineTo(xPos, labelRect.bottom);
		str.Format("%d",rangemax);
		MemDC.TextOut(xPos, labelRect.bottom + dist/2, str);
	}

	//Draw Thumb's Tick
	{
		MemDC.SetBkColor(GetSysColor(COLOR_BTNFACE));
	   	MemDC.SetBkMode(OPAQUE);
	   	MemDC.SelectObject(&m_CurrlabelFont);
		int xPos = (int)(((nbPos - rangemin) / (float)rangewidth) * (float)sliderwidth) + slidermin;
		MemDC.MoveTo(xPos, labelRect.top);
		MemDC.LineTo(xPos, labelRect.bottom);
		str.Format("%d",nbPos);
		MemDC.TextOut(xPos, labelRect.bottom + dist/2, str);
	}

/*	if(fDisplayColors)//Color Interval
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
	}*/

	//Draw thumb
//	if (IsWindowEnabled())
	{
		CRect top, left, right;
		int xPos = (int)(((nbPos - rangemin) / (float)rangewidth) * (float)sliderwidth) + slidermin;

		top = thumbRect;
		top.left = xPos - 6;
		top.right = xPos + 6;
		top.bottom = channelRect.bottom;
		left = thumbRect;
		left.top = channelRect.bottom;
		left.left = top.left;
		left.right = xPos;
		right = thumbRect;
		right.top = channelRect.bottom;
		right.left = xPos;
		right.right = top.right;

		UINT nEdge = (IsWindowEnabled()) ? BDR_RAISEDOUTER|BDR_RAISEDINNER : 
										   EDGE_ETCHED   ;

	  	MemDC.DrawEdge(&top, nEdge, BF_MIDDLE|BF_LEFT|BF_TOP|BF_RIGHT);
		MemDC.DrawEdge(&left, nEdge, BF_DIAGONAL_ENDTOPLEFT|BF_MIDDLE);
		MemDC.DrawEdge(&right, nEdge, BF_DIAGONAL_ENDBOTTOMLEFT|BF_MIDDLE);
	}

	BOOL fFocus = (BOOL)(GetFocus()==this);
  	if(fFocus)
   	{
		MemDC.DrawFocusRect(&clientRect);
   	}

	//Restore DC settings
	MemDC.SelectObject(pOldPen);
   	MemDC.SelectObject(pOldFont);

	// Do not call CSliderCtrl::OnPaint() for painting messages
}

BOOL CSelSliderCtrl::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	//return CSliderCtrl::OnEraseBkgnd(pDC);
	return FALSE;
}

void CSelSliderCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
/*	int dt=GetNumTics();
	int rangemin, rangemax ;
	GetRange(rangemin, rangemax ) ;
   	dt = GetNumTics()-1;
	dt = (int)((rangemax - rangemin) / dt +1);
	switch (nChar)
	{
	case VK_PRIOR :
		pos = (pos/dt)*dt-dt;
		SetPos(pos);
		Invalidate();
		UpdateWindow();
		break;
	case VK_NEXT:
		pos = (pos/dt)*dt+dt;
		SetPos(pos);
		Invalidate();
		UpdateWindow();
		break;
	default:*/
		CSliderCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
//		break;
//	}
/*	VerifyPos();

	int nb = GetPos();
	int nBP = GetFirstBreakPoint();
	if (nb > nBP)
	{
		SetPos(nBP);
	}
*/
}


void CSelSliderCtrl::AddBreakPoint(int nTime)
{
	m_cBreakPt.Add(nTime);
}

void CSelSliderCtrl::RemoveBreakPoint(int nTime)
{
	if(m_cBreakPt.GetSize())
		m_cBreakPt.RemoveAt(0);
}

int  CSelSliderCtrl::GetBreakPoint(int nb)
{
	int nBP = -1;
	int nSize = m_cBreakPt.GetSize();
	if ((nb > -1) && nSize && nb < nSize)
		nBP = m_cBreakPt.GetAt(nb);
	return nBP;

}

int CSelSliderCtrl::GetCurrentBreakPoint()
{
	int nb = -1;

	nb = GetBreakPoint(m_nCurrBreakPoint);
	return nb;
}

void CSelSliderCtrl::SetCurrentBreakPoint(int nb)
{
	m_nCurrBreakPoint = nb;
}

void CSelSliderCtrl::SetNextBreakPoint()
{
	int nSize = m_cBreakPt.GetSize();
	m_nCurrBreakPoint++;
	if (m_nCurrBreakPoint >= nSize)  
		m_nCurrBreakPoint = -1;

}



LRESULT CSelSliderCtrl::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (message == TBM_SETPOS)
	{
		int nb = GetPos();
		//int nBP = GetFirstBreakPoint();
		int selMin,selMax;
		GetSelection(selMin,selMax );
		//if (nBP != -1)
		{
			BOOL fPosition = (BOOL) wParam;
			LONG lPosition = (LONG) lParam+nb; 
			if (!wParam)
			{
				if (lPosition > selMax)
				{
					lParam = selMax;
				}
				else lParam = nb;
			}
		}
	}
	
	return CSliderCtrl::DefWindowProc(message, wParam, lParam);
}


void CSelSliderCtrl::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	
	CSliderCtrl::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CSelSliderCtrl::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CSliderCtrl::OnLButtonUp(nFlags, point);
}

void CSelSliderCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
//	int nBP = GetCurrentBreakPoint();
//	if (nBP != -1)
//	{
	   	CRect channelRect,clientRect;
	   	//GetClientRect(&clientRect);
		GetChannelRect(&channelRect);
		channelRect.DeflateRect(2, 0);
		int min,max;
		GetRange(min,max);
		int selmin,selmax;
		GetSelection(selmin,selmax);
		max = max-min+1;

		int locBP = (int)((point.x - min  - channelRect.left) * max / (float)channelRect.Width() + min );
		//int nb2 = (int)(point.x*channelRect.Width() / (float)max);
		//int nb2 = (int)(point.x*max / (float)channelRect.Width());
//		if (locBP >= selmax) return;
//	}
	CSliderCtrl::OnLButtonDown(nFlags, point);
}

void CSelSliderCtrl::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
//	int nBP = GetCurrentBreakPoint();
//	if (nBP != -1)
//	{
	   	CRect channelRect,clientRect;
	   	//GetClientRect(&clientRect);
		GetChannelRect(&channelRect);
		channelRect.DeflateRect(2, 0);
		int min,max;
		GetRange(min,max);
		int selmin,selmax;
		GetSelection(selmin,selmax);
		max = max-min+1;

		int locBP = (int)((point.x - min  - channelRect.left) * max / (float)channelRect.Width() + min );
		//int nb2 = (int)(point.x*channelRect.Width() / (float)max);
		//int nb2 = (int)(point.x*max / (float)channelRect.Width());
		//if (locBP >= nBP) return;
		//if (locBP >= selmax) return;
//			point.x = locBP;
//	}
	CSliderCtrl::OnMouseMove(nFlags, point);
}
