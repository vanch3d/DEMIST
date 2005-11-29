// COptionTree
//
// License
// -------
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
//	See License.txt for full history ifnormation.
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


// OptionTreeSpinnerButton.cpp : implementation file
//

#include "stdafx.h"
#include "OptionTreeSpinnerButton.h"

// Added Headers
#include "OptionTree.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptionTreeSpinnerButton

COptionTreeSpinnerButton::COptionTreeSpinnerButton()
{
	// Intialize variables
	m_bWrapAround = FALSE;
	m_otSpinnerOption = NULL;
	m_dRangeTop = 100;
	m_dRangeBottom = 0;
	m_rcButtonTop = CRect(0, 0, 0, 0);
	m_rcButtonBottom = CRect(0, 0, 0, 0);
	m_bBottomPressed = FALSE;
	m_bTopPressed = FALSE;
	m_bUserEdit = FALSE;
}

COptionTreeSpinnerButton::~COptionTreeSpinnerButton()
{
}


BEGIN_MESSAGE_MAP(COptionTreeSpinnerButton, CWnd)
	//{{AFX_MSG_MAP(COptionTreeSpinnerButton)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_MOVE()
	ON_WM_SIZE()
	ON_MESSAGE(OT_NOTIFY_FORCEREDRAW, WM_ForceRedraw)
	ON_MESSAGE(OT_NOTIFY_UP, WM_EditUp)
	ON_MESSAGE(OT_NOTIFY_DOWN, WM_EditDown)	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// COptionTreeSpinnerButton message handlers

void COptionTreeSpinnerButton::SetSpinnerOptionsOwner(COptionTree *otOption)
{
	// Save pointer
	m_otSpinnerOption = otOption;
}

void COptionTreeSpinnerButton::SetWrapAround(BOOL bWrap)
{
	// Save variable
	m_bWrapAround = bWrap;
}

BOOL COptionTreeSpinnerButton::GetWrapAround()
{
	// Return variable
	return m_bWrapAround;
}

BOOL COptionTreeSpinnerButton::OnEraseBkgnd(CDC* pDC) 
{
	// Naa, we like flicker free better
	return FALSE;
}

void COptionTreeSpinnerButton::SetRange(double dBottom, double dTop)
{
	// Save variables
	m_dRangeBottom = dBottom;
	m_dRangeTop = dTop;
}

void COptionTreeSpinnerButton::OnPaint() 
{
	// Make sure options aren't NULL
	if (m_otSpinnerOption == NULL)
	{
		return;
	}

	m_ctlEdit.GetWindowText(m_strEdit);


	// Declare variables
	CPaintDC dc(this);
	CDC* pDCMem = new CDC;
	CBitmap bpMem;
	CBitmap *bmOld;
	COLORREF crOld;
	HGDIOBJ hOldBrush;
	int nOldBack;
	CRect rcButtonTop, rcButtonBottom, rcClient;
	CString strText;
	HGDIOBJ hOld;

	// Get client rectangle
	GetClientRect(rcClient);

	// Calculate rectangle
	// -- Top
	rcButtonTop.top = rcClient.top;
	rcButtonTop.left = (rcClient.right - 2) - OT_SPINNER_WIDTH;
	rcButtonTop.right = rcClient.right - 2;
	rcButtonTop.bottom = rcClient.Height() / 2;
	m_rcButtonTop = rcButtonTop;
	// -- Bottom
	rcButtonBottom.top = rcButtonTop.bottom;
	rcButtonBottom.left = (rcClient.right - 2) - OT_SPINNER_WIDTH;
	rcButtonBottom.right = rcClient.right - 2;
	rcButtonBottom.bottom = rcClient.bottom;
	m_rcButtonBottom = rcButtonBottom;

	// Create DC
	pDCMem->CreateCompatibleDC(&dc);
	
	// Create bitmap
	bpMem.CreateCompatibleBitmap(&dc, rcClient.Width(), rcClient.Height());

	// Select bitmap
	bmOld = pDCMem->SelectObject(&bpMem);

	// Set background mode
	nOldBack = pDCMem->SetBkMode(TRANSPARENT);

	// Set text color
	crOld = pDCMem->SetTextColor(GetSysColor(COLOR_WINDOWTEXT));

	// Select font
	hOld = pDCMem->SelectObject(m_otSpinnerOption->GetNormalFont());

	// Draw control background
	hOldBrush = pDCMem->SelectObject(GetSysColorBrush(COLOR_BTNFACE));
	pDCMem->PatBlt(rcClient.left, rcClient.top, rcClient.Width(), rcClient.Height(), PATCOPY);

	// Wrap around
	if (m_bWrapAround == TRUE)
	{
		// -- Draw top button
		// -- -- Pressed
		if (m_bTopPressed == TRUE)
		{
			pDCMem->DrawFrameControl(&rcButtonTop, DFC_SCROLL, DFCS_PUSHED | DFCS_SCROLLUP); 
		}
		// -- -- UnPressed
		else
		{
			pDCMem->DrawFrameControl(&rcButtonTop, DFC_SCROLL, DFCS_SCROLLUP); 
		}

		// -- Draw bottom button
		// -- -- Pressed
		if (m_bBottomPressed == TRUE)
		{
			pDCMem->DrawFrameControl(&rcButtonBottom, DFC_SCROLL, DFCS_PUSHED | DFCS_SCROLLDOWN); 
		}
		// -- -- UnPressed
		else
		{
			pDCMem->DrawFrameControl(&rcButtonBottom, DFC_SCROLL, DFCS_SCROLLDOWN); 
		}
	}
	// No Wrap around
	else
	{
		// -- Draw top button
		if (_GetValue() >= m_dRangeTop)
		{
			pDCMem->DrawFrameControl(&rcButtonTop, DFC_SCROLL, DFCS_INACTIVE | DFCS_SCROLLUP); 
		}
		else
		{
			// -- -- Pressed
			if (m_bTopPressed == TRUE)
			{
				pDCMem->DrawFrameControl(&rcButtonTop, DFC_SCROLL, DFCS_PUSHED | DFCS_SCROLLUP); 
			}
			// -- -- UnPressed
			else
			{
				pDCMem->DrawFrameControl(&rcButtonTop, DFC_SCROLL, DFCS_SCROLLUP); 
			}
		}

		// -- Draw bottom button
		if (_GetValue() <= m_dRangeBottom)
		{
			pDCMem->DrawFrameControl(&rcButtonBottom, DFC_SCROLL, DFCS_INACTIVE | DFCS_SCROLLDOWN); 
		}
		else
		{
			// -- -- Pressed
			if (m_bBottomPressed == TRUE)
			{
				pDCMem->DrawFrameControl(&rcButtonBottom, DFC_SCROLL, DFCS_PUSHED | DFCS_SCROLLDOWN); 
			}
			// -- -- UnPressed
			else
			{
				pDCMem->DrawFrameControl(&rcButtonBottom, DFC_SCROLL, DFCS_SCROLLDOWN); 
			}
		}
	}

	// Copy to screen
	dc.BitBlt(0, 0, rcClient.Width(), rcClient.Height(), pDCMem, 0, 0, SRCCOPY);

	// Restore GDI ojects
	pDCMem->SelectObject(bmOld);
	pDCMem->SelectObject(hOldBrush);
	pDCMem->SetBkMode(nOldBack);
	pDCMem->SelectObject(hOld);
	pDCMem->SetTextColor(crOld);	

	// Delete objects
	if (pDCMem->GetSafeHdc() != NULL)
	{
		pDCMem->DeleteDC();
	}
	delete pDCMem;
	if (bpMem.GetSafeHandle() != NULL)
	{
		bpMem.DeleteObject();
	}
}

void COptionTreeSpinnerButton::GetRange(double &dBottom, double &dTop)
{
	// Save variables
	dBottom = m_dRangeBottom;
	dTop = m_dRangeTop;
}

void COptionTreeSpinnerButton::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// Clear pressed
	m_bBottomPressed = FALSE;
	m_bTopPressed = FALSE;

	// Force redraw
	Invalidate();
	
	CWnd::OnLButtonUp(nFlags, point);
}

void COptionTreeSpinnerButton::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// Clear pressed
	m_bBottomPressed = FALSE;
	m_bTopPressed = FALSE;

	// See if we have pressed a button
	if (m_rcButtonTop.PtInRect(point) == TRUE)
	{
		// -- Mark bool
		m_bTopPressed = TRUE;

		// -- Update value
		// -- -- Wrap around
		if (m_bWrapAround == TRUE)
		{
			if ((_GetValue() + 1) > m_dRangeTop)
			{
				SetEditDouble(m_dRangeBottom);
			}
			else
			{
				SetEditDouble(_GetValue() + 1);
			}

		}
		// -- -- No wrap around
		else
		{
			if ((_GetValue() + 1) <= m_dRangeTop)
			{
				SetEditDouble(_GetValue() + 1);
			}
		}
	}
	else if (m_rcButtonBottom.PtInRect(point) == TRUE)
	{
		// -- Mark bool
		m_bBottomPressed = TRUE;

		// -- Update value
		// -- -- Wrap around
		if (m_bWrapAround == TRUE)
		{
			if ((_GetValue() - 1) < m_dRangeBottom)
			{
				SetEditDouble(m_dRangeTop);
			}
			else
			{
				SetEditDouble(_GetValue() - 1);
			}
		}
		// -- -- No wrap around
		else
		{
			if ((_GetValue() - 1) >= m_dRangeBottom)
			{
				SetEditDouble(_GetValue() - 1);
			}
		}
	}

	// Force redraw
	Invalidate();
	
	CWnd::OnLButtonDown(nFlags, point);
}

void COptionTreeSpinnerButton::SetUserEdit(BOOL bAllowEdit)
{
	// Save variable
	m_bUserEdit = bAllowEdit;
}

BOOL COptionTreeSpinnerButton::GetUserEdit()
{
	// Return variable
	return m_bUserEdit;
}

int COptionTreeSpinnerButton::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	// Declare variables
	CRect rcEdit;
	DWORD dwStyle = WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | ES_NUMBER;
	
	// Create window
	if (CWnd::OnCreate(lpCreateStruct) == -1)
	{
		return -1;
	}

	// Calculate edit rectangle
	GetClientRect(rcEdit);
	rcEdit.right = (rcEdit.right - 2) - OT_SPINNER_WIDTH;
	
	// Create edit
	if(m_ctlEdit.Create(dwStyle, rcEdit, this, 1000) == FALSE)
	{
		return -1;
	}

	// Set selection
	m_ctlEdit.SetSel(0, 0);

	// Set font
	m_ctlEdit.SetFont(m_otSpinnerOption->GetNormalFont(), TRUE);

	//  Modify style
	m_ctlEdit.ModifyStyleEx(0, WS_EX_CLIENTEDGE, SWP_FRAMECHANGED);

	// Set Owner spinner
	m_ctlEdit.SetOwnerSpinner(this);
	
	return 0;
}

void COptionTreeSpinnerButton::SetEditDouble(double dValue)
{
	// Declare variables
	CString strText;

	// Convert string
	strText.Format("%.0f", dValue);

	// Modify style
	if (IsWindow(m_ctlEdit.GetSafeHwnd()))
	{
		m_ctlEdit.SetWindowText(strText);
	}
}

void COptionTreeSpinnerButton::SetEditInt(int nValue)
{
	// Declare variables
	CString strText;

	// Convert string
	strText.Format("%d", nValue);

	// Modify style
	if (IsWindow(m_ctlEdit.GetSafeHwnd()))
	{
		m_ctlEdit.SetWindowText(strText);
	}
}

void COptionTreeSpinnerButton::SetEditFloat(float fValue)
{
	// Declare variables
	CString strText;

	// Convert string
	strText.Format("%.0f", fValue);

	// Modify style
	if (IsWindow(m_ctlEdit.GetSafeHwnd()))
	{
		m_ctlEdit.SetWindowText(strText);
	}
}

void COptionTreeSpinnerButton::SetEditDword(DWORD dwValue)
{
	// Declare variables
	CString strText;

	// Convert string
	strText.Format("%d", dwValue);

	// Modify style
	if (IsWindow(m_ctlEdit.GetSafeHwnd()))
	{
		m_ctlEdit.SetWindowText(strText);
	}
}

void COptionTreeSpinnerButton::SetEditLong(long lValue)
{
	// Declare variables
	CString strText;

	// Convert string
	strText.Format("%.0f", lValue);

	// Modify style
	if (IsWindow(m_ctlEdit.GetSafeHwnd()))
	{
		m_ctlEdit.SetWindowText(strText);
	}
}

BOOL COptionTreeSpinnerButton::GetEditFloat(float &fReturn)
{
	// Declare variables
	CString strWindowText= m_strEdit;

	// Set blank
	fReturn = 0;

/*	// Get window text
	if (IsWindow(m_ctlEdit.GetSafeHwnd()) == TRUE)
	{
		m_ctlEdit.GetWindowText(strWindowText);
	}
	else
	{
		return FALSE;
	}

	// See if string is numeric
	if (!IsWindow(m_ctlEdit.GetSafeHwnd()))
	{
		return FALSE;
	}*/

	// Convert string
	fReturn = (float) atof(strWindowText);

	return TRUE;
}

BOOL COptionTreeSpinnerButton::GetEditDouble(double &dReturn)
{
	// Declare variables
	CString strWindowText = m_strEdit;

	// Set blank
	dReturn = 0;

/*	// Get window text
	if (IsWindow(m_ctlEdit.GetSafeHwnd()))
	{
		m_ctlEdit.GetWindowText(strWindowText);
	}
	else
	{
		return FALSE;
	}

	// See if string is numeric
	if (!IsWindow(m_ctlEdit.GetSafeHwnd()))
	{
		return FALSE;
	}*/

	// Convert string
	dReturn = atof(strWindowText);

	return TRUE;
}

BOOL COptionTreeSpinnerButton::GetEditLong(long &lReturn)
{
	// Declare variables
	CString strWindowText= m_strEdit;

	// Set blank
	lReturn = 0;

/*	// Get window text
	if (IsWindow(m_ctlEdit.GetSafeHwnd()))
	{
		m_ctlEdit.GetWindowText(strWindowText);
	}
	else
	{
		return FALSE;
	}

	// See if string is numeric
	if (!IsWindow(m_ctlEdit.GetSafeHwnd()))
	{
		return FALSE;
	}*/

	// Convert string
	lReturn = atol(strWindowText);

	return TRUE;
}

BOOL COptionTreeSpinnerButton::GetEditInt(int &nReturn)
{
	// Declare variables
	CString strWindowText= m_strEdit;

	// Set blank
	nReturn = 0;

	// Get window text
/*	if (IsWindow(m_ctlEdit.GetSafeHwnd()))
	{
		m_ctlEdit.GetWindowText(strWindowText);
	}
	else
	{
		return FALSE;
	}

	// See if string is numeric
	if (!IsWindow(m_ctlEdit.GetSafeHwnd()))
	{
		return FALSE;
	}*/

	// Convert string
	nReturn = atoi(strWindowText);

	return TRUE;
}

BOOL COptionTreeSpinnerButton::GetEditDword(DWORD &dwReturn)
{
	// Declare variables
	CString strWindowText= m_strEdit;

	// Set blank
	dwReturn = 0;

	// Get window text
/*	if (IsWindow(m_ctlEdit.GetSafeHwnd()))
	{
		m_ctlEdit.GetWindowText(strWindowText);
	}
	else
	{
		return FALSE;
	}

	// See if string is numeric
	if (!IsWindow(m_ctlEdit.GetSafeHwnd()))
	{
		return FALSE;
	}*/

	// Convert string
	dwReturn = (DWORD) atoi(strWindowText);

	return TRUE;
}

void COptionTreeSpinnerButton::OnDestroy() 
{
	// Destroy edit window
	if (IsWindow(m_ctlEdit.GetSafeHwnd()))
	{
		m_ctlEdit.DestroyWindow();
	}	
	
	CWnd::OnDestroy();	
}

void COptionTreeSpinnerButton::ResizeEdit()
{
	// Declare variables
	CRect rcEdit;

	// Calculate edit rectangle
	GetClientRect(rcEdit);
	rcEdit.right = (rcEdit.right - 2) - OT_SPINNER_WIDTH;

	// Set window position
	if (IsWindow(m_ctlEdit.GetSafeHwnd()))
	{
		m_ctlEdit.SetWindowPos(NULL, rcEdit.left, rcEdit.top, rcEdit.Width(), rcEdit.Height(), SWP_NOZORDER | SWP_SHOWWINDOW);
	}
}

void COptionTreeSpinnerButton::OnMove(int x, int y) 
{
	// Resize edit
	ResizeEdit();

	CWnd::OnMove(x, y);	
}

void COptionTreeSpinnerButton::OnSize(UINT nType, int cx, int cy) 
{	
	// Resize edit
	ResizeEdit();

	CWnd::OnSize(nType, cx, cy);
	
}

CEdit* COptionTreeSpinnerButton::GetEdit()
{
	// Return pointer
	return &m_ctlEdit;
}

BOOL COptionTreeSpinnerButton::IsStringNumeric(CString strString)
{
	// See if string is numeric or not
	if (strString.FindOneOf("1234567890") == -1 || strString.FindOneOf("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ`~[]{}-_=+\\|'/?>,<") != -1)
	{
		return FALSE;
	}

	return TRUE;
}

double COptionTreeSpinnerButton::_GetValue()
{
	// Declare variables
	double dValue;

	// Get double value
	GetEditDouble(dValue);

	return dValue;
}

LRESULT COptionTreeSpinnerButton::WM_ForceRedraw(WPARAM wParam, LPARAM lParam)
{
	// Force redraw
	Invalidate();

	return 0;
}

LRESULT COptionTreeSpinnerButton::WM_EditUp(WPARAM wParam, LPARAM lParam)
{
	// Update value
	// -- Wrap around
	if (m_bWrapAround == TRUE)
	{
		if ((_GetValue() + 1) > m_dRangeTop)
		{
			SetEditDouble(m_dRangeBottom);
		}
		else
		{
			SetEditDouble(_GetValue() + 1);
		}

	}
	// -- No wrap around
	else
	{
		if ((_GetValue() + 1) <= m_dRangeTop)
		{
			SetEditDouble(_GetValue() + 1);
		}
	}

	// Force redraw
	Invalidate();

	return 0;
}

LRESULT COptionTreeSpinnerButton::WM_EditDown(WPARAM wParam, LPARAM lParam)
{
	// Update value
	// -- Wrap around
	if (m_bWrapAround == TRUE)
	{
		if ((_GetValue() - 1) < m_dRangeBottom)
		{
			SetEditDouble(m_dRangeTop);
		}
		else
		{
			SetEditDouble(_GetValue() - 1);
		}
	}
	// -- No wrap around
	else
	{
		if ((_GetValue() - 1) >= m_dRangeBottom)
		{
			SetEditDouble(_GetValue() - 1);
		}
	}

	// Force redraw
	Invalidate();

	return 0;
}

CString COptionTreeSpinnerButton::GetEditText()
{
	// Declare variables
	CString strText = "";

	// Get edit text
	if (IsWindow(m_ctlEdit.GetSafeHwnd()))
	{
		m_ctlEdit.GetWindowText(strText);
	}

	return strText;
}
