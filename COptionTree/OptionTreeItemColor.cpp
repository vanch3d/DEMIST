// COptionTree
//
// This code is provided "as is" with no expressed or implied warranty.
//
// Feel free to modify and/or distribute this file, but
// do not remove the headers in place.
// 
// You may use this code in a commercial product with or without acknowledgement.
// However you may not sell this code or any modification of this code.
//
// I would appreciate a notification of any bugs discovered or 
// improvements that could be made to help the control grow for everyone.
//
// History:
// --------
//	5/7/2002		Initial implementation.	
//
//	5/10/2002 		Fix problems with icon and color items. Also fixed problems
//				with expanding columns and column size. Added TAB support, 
//				hit TAB to activate next item, and SHIFT TAB to activate previous item.
//				NOTE: You must commit current item with the ENTER key before tabbing, 
//				this is because some controls use TAB i.e.) edit, control box, radio, and so on.
//
//	5/11/2002		Fixed IsWindow() issues, which are the causes for a lot of Win98 problems. 
//				Fixed IDC_HAND problem for non Win 2K or XP users. Fixed some resource and 
//				overhead problems. Also added expand all feature. Huge thanks go out to all 
//				bug reporters, especially YoSilver.
//
//
// Copyright (c) 1999-2002 
// Matthew R. Miller 
// mattrmiller@computersmarts.net
//
// 
// Acknowledgements:
// -----------------
// The COptionTree is inspired by code, ideas, and submissions from the following: 
//
// CPropTree v1.0 - Property Tree Control - By Scott Ramsay (http://www.thecodeproject.com/treectrl/proptree.asp)
// A color picker button - By James White (http://www.thecodeproject.com/miscctrl/colorbutton.asp)


// OptionTreeItemColor.cpp : implementation file
//

#include "stdafx.h"
#include "OptionTreeItemColor.h"

// Added Headers
#include "OptionTree.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptionTreeItemColor

COptionTreeItemColor::COptionTreeItemColor()
{
	// Initialize variables
	m_bFocus = FALSE;
	m_rcColor = RGB(0, 0, 0);
	m_crAutomatic = RGB(0, 0, 0);
	m_bShowHex = TRUE;
	m_bLiveUpdate = TRUE;
	m_rcHitTest = CRect(0, 0, 0, 0);
}

COptionTreeItemColor::~COptionTreeItemColor()
{
}


BEGIN_MESSAGE_MAP(COptionTreeItemColor, CWnd)
	//{{AFX_MSG_MAP(COptionTreeItemColor)
	ON_WM_KILLFOCUS()
	ON_WM_SETFOCUS()
	ON_WM_LBUTTONUP()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
    ON_MESSAGE(OT_CPU_SELENDOK, OnSelEndOK)
    ON_MESSAGE(OT_CPU_SELENDCANCEL, OnSelEndCancel)
    ON_MESSAGE(OT_CPU_SELCHANGE, OnSelChange)
	ON_MESSAGE(OT_CPU_CLOSEUP, OnCloseColorPopUp)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// COptionTreeItemColor message handlers

void COptionTreeItemColor::OnKillFocus(CWnd* pNewWnd) 
{
	// Mark focus
	m_bFocus = FALSE;

	// Commit changes
	CommitChanges();

	CWnd::OnKillFocus(pNewWnd);	
}

void COptionTreeItemColor::OnSetFocus(CWnd* pOldWnd) 
{
	// Mark focus
	m_bFocus = TRUE;		
	
	CWnd::OnSetFocus(pOldWnd);	
}

void COptionTreeItemColor::DrawAttribute(CDC *pDC, const RECT &rcRect)
{
	// If we don't have focus, text is drawn.
	if (m_bFocus == TRUE)
	{
		return;
	}

	// Make sure options aren't NULL
	if (m_otOption == NULL)
	{
		return;
	}

	// Make sure there is a window
	if (!IsWindow(GetSafeHwnd()))
	{
		return;
	}

	// Set window position
	if (IsWindow(GetSafeHwnd()))
	{
		SetWindowPos(NULL, m_rcAttribute.left, m_rcAttribute.top, m_rcAttribute.Width(), m_rcAttribute.Height(), SWP_NOZORDER | SWP_NOACTIVATE | SWP_HIDEWINDOW);
	}

	// Declare variables
	HGDIOBJ hOld;
	HGDIOBJ hOldBrush;
	COLORREF crOld;
	int nOldBack;
	CRect rcText, rcColor, rcClient;
	CString strText;
	CBrush bBrush;

	// Get window rect
	GetClientRect(rcClient);

	// Clear hit test rectangle
	m_rcHitTest = CRect(0, 0, 0, 0);

	// Select font
	hOld = pDC->SelectObject(m_otOption->GetNormalFont());

	// Create bush
	bBrush.CreateSolidBrush(m_rcColor);
	
	// Set text color
	if (IsReadOnly() == TRUE || m_otOption->IsWindowEnabled() == FALSE)
	{
		crOld = pDC->SetTextColor(GetSysColor(COLOR_GRAYTEXT));
	}
	else
	{
		crOld = pDC->SetTextColor(GetSysColor(COLOR_WINDOWTEXT));
	}

	// Set background mode
	nOldBack = pDC->SetBkMode(TRANSPARENT);

	// Select brush
	hOldBrush = pDC->SelectObject(GetSysColorBrush(COLOR_BTNSHADOW));

	// Get color rectangle
	rcColor.left  = rcRect.left + 1;
	rcColor.right = rcColor.left + (long) OT_COLOR_SIZE;
	rcColor.top = rcRect.top + OT_SPACE - 2;
	rcColor.bottom = rcColor.top + (long) OT_COLOR_SIZE;

	// Draw color border
	rcColor.InflateRect(1, 1, 1, 1);
	pDC->PatBlt(rcColor.left, rcColor.top, rcColor.Width(), rcColor.Height(), PATCOPY);

	// Draw color
	rcColor.DeflateRect(1, 1, 1, 1);
	pDC->FillRect(rcColor, &bBrush);

	// Get text rectangle
	rcText.left  = rcColor.right + OT_SPACE;
	rcText.right = rcRect.right;
	rcText.top = rcRect.top;
	rcText.bottom = rcRect.bottom;

	// Get text
	if (m_bShowHex == TRUE)
	{
		strText.Format("#%.6X", m_rcColor);
	}
	else
	{
		strText.Format("RGB (%d, %d, %d)", GetRValue(m_rcColor), GetGValue(m_rcColor), GetBValue(m_rcColor));
	}

	// Draw text
	pDC->DrawText(strText, rcText, DT_SINGLELINE | DT_VCENTER);
	pDC->DrawText(strText, rcText, DT_SINGLELINE | DT_VCENTER | DT_CALCRECT);

	// Calculate hit test rectangle
	m_rcHitTest.left = rcClient.left;
	m_rcHitTest.top = rcClient.top;
	m_rcHitTest.bottom = rcClient.bottom;
	m_rcHitTest.right = m_rcHitTest.left + rcText.right;
	
	// Delete brush
	if (bBrush.GetSafeHandle() != NULL)
	{
		bBrush.DeleteObject();
	}

	// Restore GDI ojects
	pDC->SelectObject(hOldBrush);
	pDC->SelectObject(hOld);
	pDC->SetTextColor(crOld);
	pDC->SetBkMode(nOldBack);
}

void COptionTreeItemColor::OnCommit()
{
	// Hide window
	if (IsWindow(GetSafeHwnd()))
	{
		// -- Show window
		ShowWindow(SW_HIDE);
	}
}

void COptionTreeItemColor::OnRefresh()
{
	// Set the window positiion
	if (IsWindow(GetSafeHwnd()))
	{
		SetWindowPos(NULL, m_rcAttribute.left, m_rcAttribute.top, m_rcAttribute.Width(), m_rcAttribute.Height(), SWP_NOZORDER | SWP_NOACTIVATE);
	}
}

void COptionTreeItemColor::OnMove()
{
	// Set window position
	if (IsWindow(GetSafeHwnd()))
	{
		SetWindowPos(NULL, m_rcAttribute.left, m_rcAttribute.top, m_rcAttribute.Width(), m_rcAttribute.Height(), SWP_NOZORDER | SWP_NOACTIVATE);
	}

	// Hide window
	if (m_bFocus == FALSE && IsWindow(GetSafeHwnd()))
	{
		// -- Show window
		ShowWindow(SW_HIDE);
	}
}

void COptionTreeItemColor::OnActivate()
{

	// Make sure window is valid
	if (IsWindow(GetSafeHwnd()))
	{
		// -- Show window
		ShowWindow(SW_SHOW);

		// -- Set window position
		SetWindowPos(NULL, m_rcAttribute.left, m_rcAttribute.top, m_rcAttribute.Width(), m_rcAttribute.Height(), SWP_NOZORDER | SWP_SHOWWINDOW);

		// -- Set focus
		SetFocus();
	}
}

void COptionTreeItemColor::CleanDestroyWindow()
{
	// Destroy window
	if (IsWindow(GetSafeHwnd()))
	{
		// -- Destroy window
		DestroyWindow();
	}
}

void COptionTreeItemColor::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// Declare variables
	CRect rcClient;

	// Get client rect
	GetWindowRect(rcClient);

	// Hide window
	if (IsWindow(GetSafeHwnd()))
	{
		// -- Show window
		ShowWindow(SW_HIDE);
	}

	// Show color popup
	if (m_rcHitTest.PtInRect(point) == TRUE && IsReadOnly() == FALSE)
	{
		new COptionTreeColorPopUp(CPoint(rcClient.left, rcClient.bottom), m_rcColor, this, OT_COLOR_AUTOMATIC, OT_COLOR_MORECOLORS);
	}

	// Update items
	if (m_otOption != NULL)
	{
		m_otOption->UpdatedItems();
	}
	
	CWnd::OnLButtonUp(nFlags, point);
}

COLORREF COptionTreeItemColor::GetColor()
{
	// Return variable
	return m_rcColor;
}

void COptionTreeItemColor::SetColor(COLORREF rcColor)
{
	// Save variable
	m_rcColor = rcColor;
}

BOOL COptionTreeItemColor::CreateColorWindow(COLORREF rcColor, COLORREF rcAutomatic, BOOL bShowHex, BOOL bLiveUpdate)
{
	// Declare variables
	DWORD dwStyle = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	BOOL bRet = FALSE;

	// Make sure options is not NULL
	if (m_otOption == NULL)
	{
		return FALSE;
	}

	// Create window
	if (!IsWindow(GetSafeHwnd()))
	{
		// -- Create the edit view
		bRet = Create(AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW, ::LoadCursor(NULL, IDC_ARROW)), _T(""), dwStyle, m_rcAttribute, m_otOption->GetCtrlParent(), GetCtrlID());

		// -- Setup window
		if (bRet == TRUE)
		{
			// -- -- Set font
			SetFont(m_otOption->GetNormalFont(), TRUE);

			// -- -- Set color
			SetColor(rcColor);

			// -- -- Set show hex
			SetShowHex(bShowHex);

			// -- -- Set automatic color
			SetAutomaticColor(rcAutomatic);

			// -- -- Set live update
			SetLiveUpdate(bLiveUpdate);

			// -- -- Set window position
			SetWindowPos(NULL, m_rcAttribute.left, m_rcAttribute.top, m_rcAttribute.Width(), m_rcAttribute.Height(), SWP_NOZORDER | SWP_SHOWWINDOW);

			// -- -- Hide window
			ShowWindow(SW_HIDE);
		}
	}

	return bRet;
}

LONG COptionTreeItemColor::OnSelEndOK(UINT lParam, LONG wParam)
{
	// Get color	
	m_rcColor = (COLORREF)lParam;

	// See if the color is automatic
	if (m_rcColor == CLR_DEFAULT)
	{
		m_rcColor = m_crAutomatic;
	}

	// Hide window
	if (IsWindow(GetSafeHwnd()))
	{
		// -- Show window
		ShowWindow(SW_HIDE);
	}

	// Update items
	if (m_otOption != NULL)
	{
		m_otOption->UpdatedItems();
	}

    return TRUE;
}

LONG COptionTreeItemColor::OnSelEndCancel(UINT lParam, LONG wParam)
{
	// Hide window
	if (IsWindow(GetSafeHwnd()))
	{
		// -- Show window
		ShowWindow(SW_HIDE);
	}

	// Update items
	if (m_otOption != NULL)
	{
		m_otOption->UpdatedItems();
	}

    return TRUE;
}

LONG COptionTreeItemColor::OnCloseColorPopUp(UINT lParam, LONG wParam)
{
	// Get color	
	m_rcColor = (COLORREF)lParam;

	// See if the color is automatic
	if (m_rcColor == CLR_DEFAULT)
	{
		m_rcColor = m_crAutomatic;
	}

	// Hide window
	if (IsWindow(GetSafeHwnd()))
	{
		// -- Show window
		ShowWindow(SW_HIDE);
	}

	// Update items
	if (m_otOption != NULL)
	{
		m_otOption->UpdatedItems();
	}

    return TRUE;
}

LONG COptionTreeItemColor::OnSelChange(UINT lParam, LONG wParam)
{
	// Get color
	if (m_bLiveUpdate == TRUE)
	{
		m_rcColor = (COLORREF)lParam;
	}

	// See if the color is automatic
	if (m_rcColor == CLR_DEFAULT)
	{
		m_rcColor = m_crAutomatic;
	}

	// Hide window
	if (IsWindow(GetSafeHwnd()))
	{
		// -- Show window
		ShowWindow(SW_HIDE);
	}

	// Update items
	if (m_otOption != NULL)
	{
		m_otOption->UpdatedItems();
	}

    return TRUE;
}

void COptionTreeItemColor::SetShowHex(BOOL bShow)
{
	// Set variables
	m_bShowHex = bShow;
}

BOOL COptionTreeItemColor::GetShowHex()
{
	// Return variables
	return m_bShowHex;
}

void COptionTreeItemColor::SetLiveUpdate(BOOL bLive)
{
	// Save variable
	m_bLiveUpdate = bLive;
}

BOOL COptionTreeItemColor::GetLiveUpdate()
{
	// Return variable
	return m_bLiveUpdate;
}

void COptionTreeItemColor::OnDeSelect()
{
	// Hide window
	if (IsWindow(GetSafeHwnd()))
	{
		// -- Show window
		ShowWindow(SW_HIDE);
	}
}

void COptionTreeItemColor::OnSelect()
{
	// Make sure window is valid
	if (IsWindow(GetSafeHwnd()))
	{
		// -- Show window
		ShowWindow(SW_SHOW);

		// -- Set window position
		SetWindowPos(NULL, m_rcAttribute.left, m_rcAttribute.top, m_rcAttribute.Width(), m_rcAttribute.Height(), SWP_NOZORDER | SWP_SHOWWINDOW);

		// -- Set focus
		SetFocus();
	}
}

BOOL COptionTreeItemColor::OnEraseBkgnd(CDC* pDC) 
{
	// Naa, we like flicker free better
	return FALSE;
}

void COptionTreeItemColor::OnPaint() 
{
	// Do nothing here, window is invisible
}

void COptionTreeItemColor::SetAutomaticColor(COLORREF crAutomatic)
{
	// Save variable
	m_crAutomatic = crAutomatic;
}

COLORREF COptionTreeItemColor::GetAutomaticColor()
{
	// Return variable
	return m_crAutomatic;
}
