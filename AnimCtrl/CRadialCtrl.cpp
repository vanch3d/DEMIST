//////////////////////////////////////////////////////////////////////////////
// CRadialCtrl.cpp : implementation file
// Author:	David M. Flores
// Date:	10/07/99
// Remarks:
//////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "..\resource.h"
#include "CRadialCtrl.h"

#include "GridCtrl\MemDC.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

////////////////////////////////////////////////////////////////////////////
// CRadialCtrl()
// Parameters:	None.
// Action:		Constructor. Initialize all variables.
// Returns:		Nothing.
////////////////////////////////////////////////////////////////////////////
CRadialCtrl::CRadialCtrl() :
m_dAngle(0.0),
m_dTolerance(360.0),
m_nSmallMove(1),
m_nLargeMove(45),
m_nArrowOffset(8),
m_nArrowHeadLength(5),
m_nArrowTailLength(10),
m_nArrowTrim(5),
m_nMinRange(0),
m_nMaxRange(255),
m_nCurrentPos(0),
m_bCapture(false),
m_bFirstTime(true),
m_bDrawFocusBox(true),
m_bDrawText(false)
{
	m_colorTransparent = RGB(0,255,0);
	m_colorHiLight = RGB(255,255,255);
	m_colorShadow = RGB(128,128,128);
	m_colorState = ::GetSysColor(COLOR_BTNTEXT);
	m_nBitmapID = IDB_RADIAL_BACKGROUND_BITMAP;

	HFONT font = ::CreateFont(	15, 0, 
								0, 0, 0, 0, 0, 0, 
								ANSI_CHARSET, 
								OUT_DEFAULT_PRECIS, 
								CLIP_DEFAULT_PRECIS,
								PROOF_QUALITY,
								DEFAULT_PITCH | FF_DONTCARE,
								"MS Sans Serif");

	m_font.Attach(font);
}

////////////////////////////////////////////////////////////////////////////
// ~CRadialCtrl()
// Parameters:	None.
// Action:		Deconstructor.
// Returns:		Nothing.
////////////////////////////////////////////////////////////////////////////
CRadialCtrl::~CRadialCtrl()
{
	m_bmpBackground.DeleteObject();
	m_font.DeleteObject();
}

/////////////////////////////////////////////////////////////////////////////
//  Message Map
/////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CRadialCtrl, CStatic)
	//{{AFX_MSG_MAP(CRadialCtrl)
	ON_WM_PAINT()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_SYSCOLORCHANGE()
	ON_WM_ENABLE()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_MESSAGE(WM_PRINT,OnPrint)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

////////////////////////////////////////////////////////////////////////////
// Refresh()
// Parameters:	bErase -	Specifies whether the background within the update 
//							region is to be erased
// Action:		Delete background before repainting.
// Returns:		Nothing.
////////////////////////////////////////////////////////////////////////////
void CRadialCtrl::Refresh(BOOL bErase)
{
	m_bmpBackground.DeleteObject();
	m_bFirstTime = true;
	Invalidate(bErase);
}

////////////////////////////////////////////////////////////////////////////
// DrawBitmap()
// Parameters:	dc - Device context to draw to.
// Action:		Calculate the window size to match the size of the bitmap.
// Returns:		Nothing.
////////////////////////////////////////////////////////////////////////////
void CRadialCtrl::DrawBitmap(CDC* dc)
{
	//Get the position of the control
	GetWindowRect(&m_rect);
	ScreenToClient(&m_rect);

	//Load bitmap
	if ((HBITMAP)m_bmpBackground == NULL)
	{
		if (m_bmpBackground.LoadBitmap(m_nBitmapID) == FALSE)
		{
			TRACE0("Loading bitmap failed!\n");
		}
		else
		{
			//Get size of bitmap and set the rect size to the bitmap
			BITMAP bmpSize;
			m_bmpBackground.GetBitmap(&bmpSize);
			m_rectBitmap.left = m_rect.left;
			m_rectBitmap.right = m_rect.left + bmpSize.bmWidth;
			m_rectBitmap.top = m_rect.top;
			m_rectBitmap.bottom = m_rect.top + bmpSize.bmHeight;
			m_rect = m_rectBitmap;

			if (m_bDrawText)
			{
				//rect for text
				CString str;
				GetWindowText(str);
				CSize size = dc->GetTextExtent(str);
				m_rectText = m_rectBitmap;
				m_rectText.top = m_rectBitmap.bottom;
				m_rectText.bottom = m_rectBitmap.bottom + size.cy - 1;
				m_rect.bottom =  m_rectText.bottom;
			}
			
			CDC tmpDC;
			VERIFY(tmpDC.CreateCompatibleDC(dc));

			// select bitmap into the temp memory DC 
			CBitmap* pOld = tmpDC.SelectObject(&m_bmpBackground);

			// nested loop for width & height replacing any pixel that matches the 
			// transparency color with color COLOR_BTNFACE and replacing the color highlights
			for (int x = 0; x < bmpSize.bmWidth; x++)
			{
				for (int y = 0; y < bmpSize.bmHeight; y++)
				{
					COLORREF color_value = tmpDC.GetPixel(x, y);
					if (color_value == m_colorTransparent)
					{
						tmpDC.SetPixel(x, y, ::GetSysColor(COLOR_BTNFACE));
					}
					else if (color_value == m_colorHiLight)
					{
						tmpDC.SetPixel(x, y, ::GetSysColor(COLOR_BTNHILIGHT));
					}
					else if (color_value == m_colorShadow)
					{
						tmpDC.SetPixel(x, y, m_colorState);
					}
				}
			}
			// pu the old one back
			tmpDC.SelectObject(pOld);
			// the DC's destructor will call DeleteDC.
			// VERIFY(tmpDC.DeleteDC());

			//Set the control to the exact size of the bitmap
			UINT flags = SWP_NOMOVE | SWP_NOZORDER;
			SetWindowPos(NULL,0,0, m_rect.right, m_rect.bottom, flags);
			m_ptCenter.x = m_ptCenter.y = m_nArrowTailLength = (m_rectBitmap.Width() / 2) - 1;
		}
	}
}

////////////////////////////////////////////////////////////////////////////
// DrawAngle()
// Parameters:	dc - device content to draw to
// Action:		Draw the current angle and the border
// Returns:		Nothing.
////////////////////////////////////////////////////////////////////////////
void CRadialCtrl::DrawAngle(CDC* dc)
{
	CFont* fontOld = dc->SelectObject(&m_font);

	CBrush brush(m_colorState);
	CBrush* brushOld = dc->SelectObject(&brush);
	CPen pen(PS_SOLID, 1, m_colorState);
	CPen* penOld = dc->SelectObject(&pen);

	CBrush brushBack(::GetSysColor(COLOR_BTNFACE));
	CPen penShadow(PS_SOLID, 1,::GetSysColor(COLOR_BTNSHADOW));
	CPen penHiLight(PS_SOLID, 1,::GetSysColor(COLOR_BTNHILIGHT));

	//background of text
	dc->FillRect(&m_rectText, &brushBack);

	//sunken boarder
	dc->SelectObject(penShadow);
	dc->MoveTo(m_rectText.left, m_rectText.bottom);
	dc->LineTo(m_rectText.left, m_rectText.top);
	dc->LineTo(m_rectText.right, m_rectText.top);
	dc->SelectObject(penHiLight);
	dc->LineTo(m_rectText.right, m_rectText.bottom);
	dc->LineTo(m_rectText.left, m_rectText.bottom);

	//Draw text
    CString str;
	//double pct = (double)(m_nMaxRange - m_nMinRange) / 360.0;
	//double value = m_nMinRange + GetAngle() * pct;
	str.Format("%3.3d",(int)GetAngle());
	SetWindowText(str);

    int nFormat = DT_NOPREFIX | DT_NOCLIP | DT_WORDBREAK | DT_SINGLELINE | DT_VCENTER | DT_CENTER;
	dc->DrawText(str, m_rectText, nFormat);

	dc->SelectObject(penOld);
	dc->SelectObject(brushOld);
	dc->SelectObject(fontOld);

	VERIFY(pen.DeleteObject());
	VERIFY(brush.DeleteObject());

	VERIFY(penShadow.DeleteObject());
	VERIFY(penHiLight.DeleteObject());
	VERIFY(brushBack.DeleteObject());
}

////////////////////////////////////////////////////////////////////////////
// DrawArrow()
// Parameters:	dc - device content to draw to
// Action:		Draw the arrow and tail for the current angle
// Returns:		Nothing.
////////////////////////////////////////////////////////////////////////////
void CRadialCtrl::DrawArrow(CDC* dc)
{
	CBrush brush(m_colorState);
	CBrush* brushOld = dc->SelectObject(&brush);
	CPen pen(PS_SOLID, 1, m_colorState);
	CPen* penOld = dc->SelectObject(&pen);

	//Calculate the position of the needle/arrow
	CPoint ptStartTip = ConvertToPoint(m_ptCenter, m_dAngle, m_nArrowOffset);
	CPoint ptEndTip = ConvertToPoint(m_ptCenter, m_dAngle, m_nArrowTailLength - m_nArrowTrim);
	CPoint ptArrow1 = ConvertToPoint(ptEndTip, m_dAngle + 135, m_nArrowHeadLength);
	CPoint ptArrow2 = ConvertToPoint(ptEndTip, m_dAngle + 225, m_nArrowHeadLength);

	//Draw Arrow Tail
	dc->MoveTo(ptStartTip);
	dc->LineTo(ptEndTip);

	//Draw Arrow Head
	dc->BeginPath();
	dc->MoveTo(ptEndTip);
	dc->LineTo(ptArrow1);
	dc->LineTo(ptArrow2);
	dc->LineTo(ptEndTip);
	dc->EndPath();
	dc->StrokeAndFillPath();

	dc->SelectObject(penOld);
	dc->SelectObject(brushOld);
	VERIFY(pen.DeleteObject());
	VERIFY(brush.DeleteObject());
}

////////////////////////////////////////////////////////////////////////////
// OnPaint()
// Parameters:	None.
// Action:		Draw bitmap and pointer
// Returns:		Nothing.
////////////////////////////////////////////////////////////////////////////

LRESULT CRadialCtrl::OnPrint(WPARAM wp,LPARAM lp)
{
	HDC hDC = (HDC)wp;

	CDC mDC;
	CDC *dc = mDC.FromHandle(hDC);
	if (!dc) return FALSE;
	
	dc->SetBkColor(::GetSysColor(COLOR_BTNFACE));
	dc->SetTextColor(m_colorState);

	//Draw bitmap
	if (m_bFirstTime)
	{
		DrawBitmap(dc);
		m_bFirstTime = false;
	}

	CDC memDC;
	VERIFY(memDC.CreateCompatibleDC(dc));
	CBitmap* pOld = NULL;
	if ((HBITMAP)m_bmpBackground != NULL)
	{
		// select current bitmap into the dc
		pOld = memDC.SelectObject(&m_bmpBackground);

		// bitblt using input dc
		dc->BitBlt(0, 0, m_rectBitmap.Width(), m_rectBitmap.Height(), &memDC, 0, 0, SRCCOPY);
	}

	DrawArrow(dc);

	if (m_bDrawText)
	{
		DrawAngle(dc);
		if (m_bDrawFocusBox)
		{
			CRect tmpRect(m_rectText);
			tmpRect.DeflateRect(1,1);
			dc->DrawFocusRect(&tmpRect);
		}
	}
	else
	{
		if (m_bDrawFocusBox)
		{
			CRect tmpRect(m_rect);
			tmpRect.DeflateRect(1,1);
			dc->DrawFocusRect(&tmpRect);
		}		
	}



	//clean up
	if(pOld != NULL)
	{
		memDC.SelectObject(memDC);
	}
	return TRUE;
}

void CRadialCtrl::Draw(CDC* pDC,CRect mrect)
{
}

void CRadialCtrl::OnPaint() 
{
	CPaintDC MemDC(this); // device context for painting
//    CMemDC MemDC(&dc);
//	return;


//	dc.SetBkColor(::GetSysColor(COLOR_BTNFACE));
//	dc.SetTextColor(m_colorState);
	MemDC.SetBkColor(::GetSysColor(COLOR_BTNFACE));
	MemDC.SetTextColor(m_colorState);

	//Draw bitmap
	if (m_bFirstTime)
	{
		DrawBitmap(&MemDC);
		m_bFirstTime = false;
	}

	CDC memDC;
	VERIFY(memDC.CreateCompatibleDC(&MemDC));
	CBitmap* pOld = NULL;
	if ((HBITMAP)m_bmpBackground != NULL)
	{
		// select current bitmap into the dc
		pOld = memDC.SelectObject(&m_bmpBackground);

		// bitblt using input dc
		MemDC.BitBlt(0, 0, m_rectBitmap.Width(), m_rectBitmap.Height(), &memDC, 0, 0, SRCCOPY);
	}

	DrawArrow(&MemDC);

	if (m_bDrawText)
	{
		DrawAngle(&MemDC);
		if (m_bDrawFocusBox)
		{
			CRect tmpRect(m_rectText);
			tmpRect.DeflateRect(1,1);
			MemDC.DrawFocusRect(&tmpRect);
		}
	}
	else
	{
		if (m_bDrawFocusBox)
		{
			CRect tmpRect(m_rect);
			tmpRect.DeflateRect(1,1);
			MemDC.DrawFocusRect(&tmpRect);
		}		
	}



	//clean up
	if(pOld != NULL)
	{
		memDC.SelectObject(pOld);
	}
	// the DC's destructor will call DeleteDC.
	// VERIFY(memDC.DeleteDC());
}

////////////////////////////////////////////////////////////////////////////
// OnLButtonUp()
// Parameters:	nFlags	- Which virtual keys are down.
//				point	- Specifies the x- and y-coordinate of the cursor.
// Action:		Reset capture flag
// Returns:		Nothing.
////////////////////////////////////////////////////////////////////////////
void CRadialCtrl::OnLButtonUp(UINT nFlags, CPoint point) 
{
	m_bCapture = false;
	ReleaseCapture();
	CStatic::OnLButtonUp(nFlags, point);
}

////////////////////////////////////////////////////////////////////////////
// OnMouseMove()
// Parameters:	nFlags	- Which virtual keys are down.
//				point	- Specifies the x- and y-coordinate of the cursor.
// Action:		Process user changing arrow position
// Returns:		Nothing.
////////////////////////////////////////////////////////////////////////////
void CRadialCtrl::OnMouseMove(UINT nFlags, CPoint point) 
{
	if ((nFlags & MK_LBUTTON) &&
		(m_bCapture))
	{
		//Convert xy position to an angle.
		m_dAngle = ConvertToAngle(point, m_ptCenter);

		double pos = (GetAngle() / 360.0) * static_cast<double>(m_nMaxRange - m_nMinRange);

		m_nCurrentPos = m_nMinRange + static_cast<int>(pos);

		::SendMessage(	GetParent()->GetSafeHwnd(), 
						WM_HSCROLL, 
						MAKEWPARAM(SB_THUMBPOSITION, m_nCurrentPos),
						(LPARAM)GetSafeHwnd());
		Invalidate();
	}

	CStatic::OnMouseMove(nFlags, point);
}

////////////////////////////////////////////////////////////////////////////
// OnLButtonDown()
// Parameters:	nFlags	- Which virtual keys are down.
//				point	- Specifies the x- and y-coordinate of the cursor.
// Action:		
// Returns:		Nothing.
////////////////////////////////////////////////////////////////////////////
void CRadialCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	//Convert xy position to an angle.
	double ang = ConvertToAngle(point, m_ptCenter);

	//User clicked close enough to the needle to allow them to start moving it
	if (fabs(ang - m_dAngle) < m_dTolerance)
	{
		m_bCapture = true;
		if( GetSafeHwnd() != ::GetCapture() )
		{
			SetCapture();
		}
	}
	SetFocus();
	CStatic::OnLButtonDown(nFlags, point);
}

////////////////////////////////////////////////////////////////////////////
// OnSysColorChange()
// Parameters:	None.
// Action:		Update with new system colors
// Returns:		Nothing.
////////////////////////////////////////////////////////////////////////////
void CRadialCtrl::OnSysColorChange() 
{
	CStatic::OnSysColorChange();
	Refresh();
}

////////////////////////////////////////////////////////////////////////////
// OnEnable()
// Parameters:	bEnable	- Specifies whether the CWnd object has been enabled or disabled
// Action:		Update with enable/disable colors
// Returns:		Nothing.
////////////////////////////////////////////////////////////////////////////
void CRadialCtrl::OnEnable(BOOL bEnable) 
{
	m_colorState = ::GetSysColor(bEnable ? COLOR_BTNTEXT : COLOR_BTNSHADOW);
	Refresh();
}

////////////////////////////////////////////////////////////////////////////
// OnSetFocus()
// Parameters:	pNewWnd - Contains the CWnd object that loses the input 
//				focus (may be NULL). The pointer may be temporary and should
//				not be stored for later use.
// Action:		Turn on the drawing of the focus box and redraw.
// Returns:		Nothing.
////////////////////////////////////////////////////////////////////////////
void CRadialCtrl::OnSetFocus(CWnd* pOldWnd) 
{
	CStatic::OnSetFocus(pOldWnd);

	m_bDrawFocusBox = true;
	Invalidate();
}


////////////////////////////////////////////////////////////////////////////
// OnKillFocus()
// Parameters:	pNewWnd - Specifies a pointer to the window that receives 
//				the input focus (may be NULL or may be temporary).
// Action:		Turn off the drawing of the focus box and redraw.
// Returns:		Nothing.
////////////////////////////////////////////////////////////////////////////
void CRadialCtrl::OnKillFocus(CWnd* pNewWnd) 
{
	CStatic::OnKillFocus(pNewWnd);
	
	m_bDrawFocusBox = false;
	Invalidate();
}

////////////////////////////////////////////////////////////////////////////
// PreTranslateMessage()
// Parameters:	None.
// Action:		
// Returns:		Nothing.
////////////////////////////////////////////////////////////////////////////
BOOL CRadialCtrl::PreTranslateMessage(MSG* pMsg) 
{
	BOOL bChanged = FALSE;
	if (pMsg->message == WM_KEYDOWN)
	{
		if ((pMsg->wParam == VK_DOWN) || (pMsg->wParam == VK_LEFT))
		{
			m_nCurrentPos -= m_nSmallMove;
			bChanged = TRUE;
		}
		else if ((pMsg->wParam == VK_UP) || (pMsg->wParam == VK_RIGHT))
		{
			m_nCurrentPos += m_nSmallMove;
			bChanged = TRUE;
		}
		else if (pMsg->wParam == VK_NEXT) // page down
		{
			m_nCurrentPos -= m_nLargeMove;
			bChanged = TRUE;
		}
		else if (pMsg->wParam == VK_PRIOR) // page up
		{
			m_nCurrentPos += m_nLargeMove;
			bChanged = TRUE;
		}
		else if (pMsg->wParam == VK_HOME)
		{
			m_nCurrentPos = m_nMinRange;
			bChanged = TRUE;
		}
		else if (pMsg->wParam == VK_END)
		{
			m_nCurrentPos = m_nMaxRange;
			bChanged = TRUE;
		}

		if( bChanged )
		{
			VerifyPos();

			double angle = static_cast<double>(m_nCurrentPos) / static_cast<double>(m_nMaxRange) * 360.0;
			SetAngle(static_cast<int>(angle));

			Invalidate();

			::SendMessage(	GetParent()->GetSafeHwnd(), 
							WM_HSCROLL, 
							MAKEWPARAM(SB_THUMBPOSITION, m_nCurrentPos),
							(LPARAM)GetSafeHwnd());
			// now we can return
			return TRUE;
		}
	}
	// be a good citizen - call the base class	
	return CStatic::PreTranslateMessage(pMsg);
}

////////////////////////////////////////////////////////////////////////////
// DisplayAngle()
// Parameters:	bEnable - true/false
// Action:		Enable or disable displaying the current angle in the control
// Returns:		Nothing.
////////////////////////////////////////////////////////////////////////////
void CRadialCtrl::DisplayAngle(bool bEnable)
{
	m_bDrawText = bEnable;
}

////////////////////////////////////////////////////////////////////////////
// GetAngle()
// Parameters:	None.
// Action:		Convert the angle so that 0 degrees is at 12 o'clock
// Returns:		The angle of the needle/arrow.
////////////////////////////////////////////////////////////////////////////
int CRadialCtrl::GetAngle() const
{
	int adjusted = (int)(450.0 - m_dAngle) % 360;

	return adjusted;
};

////////////////////////////////////////////////////////////////////////////
// SetAngle()
// Parameters:	nDegrees	- The new angle.
// Action:		Convert the angle to 0-360.
// Returns:		Nothing.
////////////////////////////////////////////////////////////////////////////
void CRadialCtrl::SetAngle(const int nDegrees)
{
	m_dAngle = (450 - nDegrees) % 360;
}

////////////////////////////////////////////////////////////////////////////
// SetRange()
// Parameters:	nMin - minimum value of control
//				nMax - maximum value of control
// Action:		Sets the min and max range 
// Returns:		Nothing.
////////////////////////////////////////////////////////////////////////////
void CRadialCtrl::SetRange(int nMin, int nMax)
{
	if (nMax > nMin)
	{
		//set the new range
		m_nMinRange = nMin;
		m_nMaxRange = nMax;

		//make sure the current position falls in the range
		VerifyPos();
	}
}

////////////////////////////////////////////////////////////////////////////
// SetMinRange()
// Parameters:	nMin - minimum value of control
// Action:		Sets the minimum range
// Returns:		Nothing.
////////////////////////////////////////////////////////////////////////////
void CRadialCtrl::SetMinRange(int nMin)
{
	if (nMin < m_nMaxRange)
	{
		//set the new range
		m_nMinRange = nMin;

		//make sure the current position falls in the range
		VerifyPos();
	}
}

////////////////////////////////////////////////////////////////////////////
// SetMaxRange()
// Parameters:	nMax - maximum value of control
// Action:		Sets the maximum range
// Returns:		Nothing.
////////////////////////////////////////////////////////////////////////////
void CRadialCtrl::SetMaxRange(int nMax)
{
	if (nMax > m_nMinRange)
	{
		//set the new range
		m_nMaxRange = nMax;

		//make sure the current position falls in the range
		VerifyPos();
	}
}

////////////////////////////////////////////////////////////////////////////
// SetPos()
// Parameters:	nPos - current position
// Action:		Sets the current position if the new position falls outside
//				the min and max range, then it will be set to the range limit
// Returns:		Nothing.
////////////////////////////////////////////////////////////////////////////
void CRadialCtrl::SetPos(int nPos)
{
	//set the new position
	m_nCurrentPos = nPos;

	//verify it
	VerifyPos();

	//set the angle position
	double angle = static_cast<double>(m_nCurrentPos) / static_cast<double>(m_nMaxRange) * 360.0;
	SetAngle(static_cast<int>(angle));
}

////////////////////////////////////////////////////////////////////////////
// GetRange()
// Parameters:	nMin - minimum value of control
//				nMax - maximum value of control
// Action:		Gets the min and max range 
// Returns:		Nothing.
////////////////////////////////////////////////////////////////////////////
void CRadialCtrl::GetRange(int &nMin, int &nMax)
{
	nMin = m_nMinRange;
	nMax = m_nMaxRange;
}

////////////////////////////////////////////////////////////////////////////
// GetMinRange()
// Parameters:	nMin - minimum value of control
// Action:		Gets the minimum range
// Returns:		Nothing.
////////////////////////////////////////////////////////////////////////////
int CRadialCtrl::GetMinRange() const
{
	return m_nMinRange;
}

////////////////////////////////////////////////////////////////////////////
// GetMaxRange()
// Parameters:	nMax - maximum value of control
// Action:		Gets the maximum range
// Returns:		Nothing.
////////////////////////////////////////////////////////////////////////////
int CRadialCtrl::GetMaxRange() const
{
	return m_nMaxRange;
}

////////////////////////////////////////////////////////////////////////////
// GetPos()
// Parameters:	None
// Action:		
// Returns:		The current position
////////////////////////////////////////////////////////////////////////////
int CRadialCtrl::GetPos() const
{
	return m_nCurrentPos;
}

////////////////////////////////////////////////////////////////////////////
// VerifyPos()
// Parameters:	None
// Action:		Verifys that the current position is between the min and max
//				range.
// Returns:		Nothing
////////////////////////////////////////////////////////////////////////////
void CRadialCtrl::VerifyPos()
{
	if (m_nCurrentPos < m_nMinRange)
	{
		m_nCurrentPos = m_nMinRange;
	}

	if (m_nCurrentPos > m_nMaxRange)
	{
		m_nCurrentPos = m_nMaxRange;
	}
}

////////////////////////////////////////////////////////////////////////////
// ConvertToPoint()
// Parameters:	offset	- Center position
//				angle	- Angle in degrees;
//				radius	- Distance from offset
// Action:		Calculate x/y position of a given angle and radius from some offset.
// Returns:		Calculate point.
////////////////////////////////////////////////////////////////////////////
CPoint CRadialCtrl::ConvertToPoint(CPoint offset, const double angle, const double radius) const
{
	double radians = angle / (180.0 / 3.141952);
	int x = offset.x + (int)((double)radius * cos(radians));
	int y = offset.y - (int)((double)radius * sin(radians));
	CPoint point(x,y);
	return point;
}

////////////////////////////////////////////////////////////////////////////
// ConvertToAngle()
// Parameters:	point	- Point of the angle to be determined
//				center	- Center point to determine angle from.
// Action:		Convert a xy position to an angle from 0.
// Returns:		The angle.
////////////////////////////////////////////////////////////////////////////
double CRadialCtrl::ConvertToAngle(CPoint point, CPoint center) const
{
	//Calculate the position user click relative to the center
	int x = point.x - center.x;
	int y = center.y - point.y;

	//Convert xy position to an angle.
	double ang = ArcTan((double)x, (double)y);

	return ang;
}

////////////////////////////////////////////////////////////////////////////
// SetTolerance()
// Parameters:	nTolerance	- Number of degrees
// Action:		Set the tolerance from clicking on the arrow
// Returns:		Nothing.
////////////////////////////////////////////////////////////////////////////
void CRadialCtrl::SetTolerance(const int nTolerance)
{
	if (nTolerance > 10)
	{
		m_dTolerance = nTolerance;
	}
}

////////////////////////////////////////////////////////////////////////////
// SetTransparentColor()
// Parameters:	color	- The color in the bitmap to make transparent
// Action:		
// Returns:		Nothing.
////////////////////////////////////////////////////////////////////////////
void CRadialCtrl::SetTransparentColor(COLORREF color)
{
	m_colorTransparent = color;
}

////////////////////////////////////////////////////////////////////////////
// SetHiLightColor()
// Parameters:	color	- The color in the bitmap to make the high light color
// Action:		
// Returns:		Nothing.
////////////////////////////////////////////////////////////////////////////
void CRadialCtrl::SetHiLightColor(COLORREF color)
{
	m_colorHiLight = color;
}

////////////////////////////////////////////////////////////////////////////
// SetShadowColor()
// Parameters:	color	- The color in the bitmap to make the shadow color
// Action:		
// Returns:		Nothing.
////////////////////////////////////////////////////////////////////////////
void CRadialCtrl::SetShadowColor(COLORREF color)
{
	m_colorShadow = color;
}

////////////////////////////////////////////////////////////////////////////
// SetBitmapID()
// Parameters:	nID	- The resource ID number for the background bitmap
// Action:		Delete old bitmap. Redraw with new bitmap
// Returns:		Nothing.
////////////////////////////////////////////////////////////////////////////
BOOL CRadialCtrl::SetBitmap(UINT nID)
{
	m_nBitmapID = nID;
	Refresh();
	return TRUE;
}

////////////////////////////////////////////////////////////////////////////
// SetArrow()
// Parameters:	offset	- Offset from center.
//				tail_length - Arrow tail length
//				head_length - Arrow head length
//				trim	- Pixels to trim off the end
// Action:		Set arrow parameters
// Returns:		Nothing.
////////////////////////////////////////////////////////////////////////////
void CRadialCtrl::SetArrow(int* offset, int* tail_length, int* head_length, int* trim)
{
	if (offset != NULL)
	{
		m_nArrowOffset = *offset;
	}
	if (tail_length != NULL)
	{
		m_nArrowTailLength = *tail_length;
	}
	if (head_length != NULL)
	{
		m_nArrowHeadLength = *head_length;
	}
	if (trim != NULL)
	{
		m_nArrowTrim = *trim;
	}
}

////////////////////////////////////////////////////////////////////////////
// ArcTan()
// Parameters:	ratio - The Y/X ratio 
// Action:		Calculate the arc tangent of y / x
// Returns:		Angle in degrees
////////////////////////////////////////////////////////////////////////////
const double CRadialCtrl::ArcTan(const double ratio) const
{
	double angle = atan(ratio);

	// convert radians to degrees 
	angle = angle * (180.0 / 3.141592);
	return angle;
}

////////////////////////////////////////////////////////////////////////////
// ArcTan()
// Parameters:	x,y - point location
// Action:		Calculate the arc tangent of point x,y.
// Returns:		Angle in degrees
////////////////////////////////////////////////////////////////////////////
const double CRadialCtrl::ArcTan(const double x, const double y) const
{
	double angle = 0.0;

	// both x and y are zero
	if ((x == 0.0) && (y == 0.0))		 
	{
		// the angle is 0 degrees 
		angle = 0.0;
	}
	else if (x == 0.0)
	{
		// if on the y axis line 
		angle = (y < 0.0) ? 270.0 : 90.0;
	}
	else if (y == 0.0)
	{
		// else if on the x axis line 
		angle = (x < 0.0) ? 180.0 : 0.0;
	}
	else
	{
		// else neither x or y is zero
		// find the arc-tangent of y / x in degrees
		angle = ArcTan((y / x));   
		if (x < 0.0)					// if x is negative 
		{
			angle += (y > 0.0) ? 180.0 : -180.0;
		}
		if (angle < 0.0)
		{
			angle = angle + 360.0;
		}
	}

	return angle;
}

////////////////////////////////////////////////////////////////////////////
// PreSubclassWindow()
// Parameters:	None.
// Action:		Force certain styles for the window.
// Returns:		Nothing.
////////////////////////////////////////////////////////////////////////////
void CRadialCtrl::PreSubclassWindow() 
{
	CStatic::PreSubclassWindow();

	ModifyStyle(SS_BITMAP, SS_OWNERDRAW | SS_NOTIFY | WS_TABSTOP);
}


LRESULT CRadialCtrl::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	
/*	if (message == WM_PAINT)
	{
		PAINTSTRUCT ps;
    
		CDC *pDC = BeginPaint(&ps); 
		CDC mdc;
		HDC hDC = (HDC)wParam;
		CDC *rDC = mdc.FromHandle(hDC);
		if (rDC)
		{
			rDC->TextOut(0, 0, "Hello, Windows!", 15); 
		}
		EndPaint(&ps); 
		return 0L; 
	}*/
	return CStatic::WindowProc(message, wParam, lParam);
}
