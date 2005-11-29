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

// OptionTreeIconPopUp.cpp : implementation file
//

#include "stdafx.h"
#include "OptionTreeIconPopUp.h"

// Added Headers
#include "OptionTree.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptionTreeIconPopUp

COptionTreeIconPopUp::COptionTreeIconPopUp(CPoint pPoint, int nIconSize, int nNumColumns, OT_ICON_NODE *nIcons, COptionTree *otIconOption, CWnd* pParentWnd)
{
	// Save all icon nodes
	m_nAllIcons = nIcons;

	// Initialize variables
	m_nNumRows = 0;
	m_pHover = CPoint(0, 0);
	m_otIconOption = otIconOption;
	m_wndParent = pParentWnd;

	// Validate number of columns
	if (nNumColumns >= 1)
	{
		m_nNumColumns = nNumColumns;
	}
	else
	{
		m_nNumColumns = 3;
	}

	// Validate icon size
	if (nIconSize == 48 || nIconSize == 32 || nIconSize == 16)
	{
		m_nIconSize = nIconSize;
	}
	else 
	{
		m_nIconSize = 16;
	}

	// Create
	COptionTreeIconPopUp::Create(pPoint, nIconSize, nNumColumns, nIcons, otIconOption, pParentWnd);
}

COptionTreeIconPopUp::~COptionTreeIconPopUp()
{
}


BEGIN_MESSAGE_MAP(COptionTreeIconPopUp, CWnd)
	//{{AFX_MSG_MAP(COptionTreeIconPopUp)
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_KILLFOCUS()
	ON_WM_NCDESTROY()
	ON_WM_ACTIVATEAPP()
	ON_WM_ERASEBKGND()
	ON_WM_DESTROY()
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// COptionTreeIconPopUp message handlers

BOOL COptionTreeIconPopUp::Create(CPoint pPoint, int nIconSize, int nNumColumns, OT_ICON_NODE *nIcons, COptionTree *otIconOption, CWnd *pParentWnd)
{
	// Declare variables
	CString strClassName;

	// Initialize variables
	m_nNumRows = 0;

	// Save all icon nodes
	m_nAllIcons = nIcons;
	m_pHover = CPoint(0, 0);
	m_otIconOption = otIconOption;
	m_wndParent = pParentWnd;

	// Validate number of columns
	if (nNumColumns >= 1)
	{
		m_nNumColumns = nNumColumns;
	}
	else
	{
		m_nNumColumns = 3;
	}

	// Validate icon size
	if (nIconSize == 48 || nIconSize == 32 || nIconSize == 16)
	{
		m_nIconSize = nIconSize;
	}
	else 
	{
		m_nIconSize = 16;
	}

    // Get the class name and create the window
    strClassName = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW, 0, (HBRUSH) (COLOR_BTNFACE+1), 0);

	// Create window
    if (!CWnd::CreateEx(0, strClassName, _T(""), WS_POPUP | WS_VISIBLE, pPoint.x, pPoint.y, 100, 100, pParentWnd->GetSafeHwnd(), 0, NULL))
	{
        return FALSE;
	}

	// Show window
	ShowWindow(SW_SHOWNA);

    // Set the window size
    SetWindowSize();
	
	// Setup icons
	SetUpIcons();

	// Setup tooltips
	SetUpToolTips();

    // Capture all mouse events for the life of this window
    SetCapture();

	return TRUE;
}

void COptionTreeIconPopUp::SetWindowSize()
{
	// Declare variables
	int nNumIcons;
	CRect rcClient, rcMove;
	int nRow, nCol, nIcon = 1;

	// Get window rect
	GetWindowRect(rcClient);
	rcMove = rcClient;

	// Get the number of icons
	nNumIcons = GetNumberIcons();

    // Get the number of rows
	// -- No icons make square
    if (nNumIcons == 0)
	{
		m_nNumRows = m_nNumColumns;
	}
	// -- Find number of rows
	else
	{
		m_nNumRows = nNumIcons / m_nNumColumns;
		if (nNumIcons % m_nNumColumns) 
		{
			m_nNumRows++;
		}
	}
	
	// Go through all rows
	rcMove.bottom = rcClient.top + OT_ICON_MARGIN;
	for (nRow = 1; nRow <= m_nNumRows; nRow++)
	{
		// -- Add size of icon
		rcMove.bottom += m_nIconSize;

		// -- Add the icon space
		if (nRow < m_nNumRows)
		{
			rcMove.bottom += OT_ICON_ICONSPACE;
		}
	}
	rcMove.bottom += OT_ICON_MARGIN;

	// Go through all columns
	rcMove.right = rcClient.left +  OT_ICON_MARGIN;
	for (nCol = 1; nCol <= m_nNumColumns; nCol++)
	{
		// -- Add size of icon
		rcMove.right += m_nIconSize;

		// -- Add the icon space
		if (nCol < m_nNumColumns)
		{
			rcMove.right += OT_ICON_ICONSPACE;
		}
	}
	rcMove.right += OT_ICON_MARGIN;

    // Set the window size and position
    MoveWindow(rcMove, TRUE);
}

int COptionTreeIconPopUp::GetNumberIcons()
{
	// Declate variables
	int nNum = 0;

	// Cycle through all of the nodes
	OT_ICON_NODE *curr = m_nAllIcons;
	while (curr != NULL)
	{
		// -- Count
		nNum++;

		// -- Follow the link to the next node
		curr = curr->m_nNextNode;
	}

	return nNum;
}

void COptionTreeIconPopUp::SetUpIcons()
{
	// Declare variables
	OT_ICON_NODE *nIconNode = NULL;
	CRect rcClient, rcTotalIcon;
	int nRow, nCol, nIcon = 0;
	long lTop, lLeft;

	// Get window rect
	GetClientRect(rcClient);

	// Setup top
	lTop = rcClient.top +  OT_ICON_MARGIN;
	
	// Go through all rows
	for (nRow = 1; nRow <= m_nNumRows; nRow++)
	{
		// -- Reset left
		lLeft = rcClient.left + OT_ICON_MARGIN;

		// -- Go through all columns
		for (nCol = 1; nCol <= m_nNumColumns; nCol++)
		{
			// -- -- Get icon
			nIconNode = GetIconNode(nIcon);
			nIcon++;

			// -- -- Make sure we have an icon
			if (nIconNode == NULL)
			{
				continue;
			}

			// -- -- Set icon rect
			nIconNode->m_rcHitRect.SetRect(lLeft, lTop, lLeft + m_nIconSize, lTop + m_nIconSize);

			// -- -- Add to left
			lLeft += m_nIconSize;
			if (nCol < m_nNumColumns)
			{
				lLeft += OT_ICON_ICONSPACE;
			}

		}

		// -- Add to top of total icon rectangle
		if (nRow < m_nNumRows)
		{
			lTop += m_nIconSize + OT_ICON_ICONSPACE;
		}
	}
}

OT_ICON_NODE * COptionTreeIconPopUp::GetIconNode(int nIndex)
{
	// Declate variables
	int i = 0;

	// Cycle through all of the nodes
	OT_ICON_NODE *curr = m_nAllIcons;
	while (curr != NULL)
	{
		// -- See if this is us
		if (i == nIndex)
		{
			return curr;
		}
			
		// -- Count
		i++;

		// -- Follow the link to the next node
		curr = curr->m_nNextNode;
	}

	return NULL;
}

void COptionTreeIconPopUp::SetUpToolTips()
{
	// Declare variables
	OT_ICON_NODE *nIconNode = NULL;
	int nIndex = 0;

	// Create toop tip
	if (m_ttToolTip.Create(this) == FALSE)
	{
		return;
	}

	// Go through all icons
	nIconNode = GetIconNode(nIndex);
	while (nIconNode != NULL)
	{
		// -- Set tooltip
		m_ttToolTip.AddTool(this, (LPCTSTR)nIconNode->m_strText, nIconNode->m_rcHitRect, 1);

		// -- Increase index
		nIndex++;

		// -- Get next node
		nIconNode = GetIconNode(nIndex);
	}

	// Create inactive
	m_ttToolTip.Activate(TRUE);	
}

BOOL COptionTreeIconPopUp::PreTranslateMessage(MSG* pMsg) 
{
	// Relay tooltip
	if (m_ttToolTip.GetSafeHwnd() != NULL)
	{
		m_ttToolTip.RelayEvent(pMsg);
	}

    // Sometimes if the picker loses focus it is never destroyed
    if (GetCapture()->GetSafeHwnd() != GetSafeHwnd())
    {   
		SetCapture(); 
	}
	
	return CWnd::PreTranslateMessage(pMsg);
}

void COptionTreeIconPopUp::OnPaint() 
{
	// Make sure options aren't NULL
	if (m_otIconOption == NULL)
	{
		return;
	}

	// Declare variables
	CPaintDC dc(this);
	CDC* pDCMem = new CDC;
	CBitmap bpMem;
	CBitmap *bmOld;
	CRect rcClient, rcButton;
	COLORREF crOld;
	HGDIOBJ hOldBrush;
	HGDIOBJ hOld;
	int nOldBack;
	OT_ICON_NODE *nIconNode = m_nAllIcons;

	// Get client rectangle
	GetClientRect(rcClient);

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
	hOld = pDCMem->SelectObject(m_otIconOption->GetNormalFont());

	// Select brush
	hOldBrush = pDCMem->SelectObject(GetSysColorBrush(COLOR_BTNFACE));

	// Fill background
	pDCMem->PatBlt(rcClient.left, rcClient.top, rcClient.right, rcClient.bottom, PATCOPY);

	// Go through all icons
	while (nIconNode != NULL)
	{
		// -- Button
		// -- -- Hover
		if (nIconNode->m_rcHitRect.PtInRect(m_pHover) == TRUE)
		{
			rcButton = nIconNode->m_rcHitRect;
			rcButton.InflateRect(3, 3, 3, 3);

			pDCMem->DrawEdge(rcButton, BDR_RAISEDINNER, BF_RECT);
		}
		// -- -- Selected
		else if (nIconNode->m_bSelected == TRUE)
		{
			rcButton = nIconNode->m_rcHitRect;
			rcButton.InflateRect(3, 3, 3, 3);

			pDCMem->FillSolidRect(rcButton, ::GetSysColor(COLOR_3DHILIGHT));
			pDCMem->DrawEdge(rcButton, BDR_SUNKENOUTER, BF_RECT);
		}
		
		// -- Draw icon
		pDCMem->DrawState(CPoint(nIconNode->m_rcHitRect.left, nIconNode->m_rcHitRect.top), CSize(m_nIconSize, m_nIconSize), nIconNode->m_hIcon, DSS_NORMAL, (CBrush*)NULL);

		// -- Follow the link to the next node
		nIconNode = nIconNode->m_nNextNode;
	}

	// Draw text
	if (GetNumberIcons() == 0)
	{
		pDCMem->DrawText(_T("No Icons."), rcClient, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	}

	// Draw edge
	pDCMem->DrawEdge(rcClient, EDGE_RAISED, BF_RECT);

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

void COptionTreeIconPopUp::OnMouseMove(UINT nFlags, CPoint point) 
{
	// Declare variables
	OT_ICON_NODE *otiIconNode = m_nAllIcons;

	// Hit test
	while (otiIconNode != NULL)
	{
		if (otiIconNode->m_rcHitRect.PtInRect(point))
		{
			break;
		}
		else
		{
			otiIconNode = otiIconNode->m_nNextNode;
		}
	}

	// Redraw
	if (m_otiLastNode != otiIconNode)
	{
		// -- Save point
		m_pHover = point;
		m_otiLastNode = otiIconNode;

		// -- Force redraw
		Invalidate();
	}

	CWnd::OnMouseMove(nFlags, point);
}

void COptionTreeIconPopUp::DeSelectAll()
{
	// Cycle through all of the nodes
	OT_ICON_NODE *curr = m_nAllIcons;
	while (curr != NULL)
	{
		// -- Deselect
		curr->m_bSelected = FALSE;

		// -- Follow the link to the next node
		curr = curr->m_nNextNode;
	}
}

void COptionTreeIconPopUp::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// Declare variables
	OT_ICON_NODE *nIconNode = m_nAllIcons;
	BOOL bFound = FALSE;

	// Go through all icons
	while (nIconNode != NULL)
	{
		// -- Hit test
		if (nIconNode->m_rcHitRect.PtInRect(point) == TRUE)
		{
			// -- -- Deselect all
			DeSelectAll();

			// -- -- Select this
			nIconNode->m_bSelected = TRUE;

			// -- -- Set found
			bFound = FALSE;

			break;
		}
		
		// -- Follow the link to the next node
		nIconNode = nIconNode->m_nNextNode;
	}

	// ALWAYS send message to parent
	if (m_wndParent->GetSafeHwnd() != NULL)
	{
		m_wndParent->SendMessage(OT_ICON_CLOSE, 0, 0);
	}

	// Close
	ReleaseCapture();
	DestroyWindow();


	// Handle left click
	if (::IsWindow(GetSafeHwnd()))
	{
		CWnd::OnLButtonUp(nFlags, point);
	}
}

void COptionTreeIconPopUp::OnKillFocus(CWnd* pNewWnd) 
{
	// ALWAYS send message to parent
	if (m_wndParent->GetSafeHwnd() != NULL)
	{
		m_wndParent->SendMessage(OT_ICON_CLOSE, 0, 0);
	}

	// Release capture
	ReleaseCapture();

	CWnd::OnKillFocus(pNewWnd);
}

void COptionTreeIconPopUp::OnNcDestroy() 
{
	CWnd::OnNcDestroy();
	
	// Delete this
	delete this;
	
}

void COptionTreeIconPopUp::OnActivateApp(BOOL bActive, HTASK hTask) 
{
	CWnd::OnActivateApp(bActive, hTask);
	
	// Release capture
	ReleaseCapture();
	
}

BOOL COptionTreeIconPopUp::OnEraseBkgnd(CDC* pDC) 
{
	// Naa, we like flicker free better
	return FALSE;
}

void COptionTreeIconPopUp::OnDestroy() 
{
	// ALWAYS send message to parent
	if (m_wndParent->GetSafeHwnd() != NULL)
	{
		m_wndParent->SendMessage(OT_ICON_CLOSE, 0, 0);
	}	
	
	CWnd::OnDestroy();	
}

void COptionTreeIconPopUp::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// Declare variables
	OT_ICON_NODE *nIconNode = m_nAllIcons;
	BOOL bFound = FALSE;
	int nSelectedIndex;

	// Get selected index
	nSelectedIndex = GetSelIconIndex();

	// Validate selected index
	if (nSelectedIndex == -1)
	{
		CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
		return;
	}

	// Get key pressed
	// -- Enter
	if (nChar == VK_RETURN) 
	{
		// -- ALWAYS send message to parent
		if (m_wndParent->GetSafeHwnd() != NULL)
		{
			m_wndParent->SendMessage(OT_ICON_CLOSE, 0, 0);
		}

		// -- Close
		ReleaseCapture();
		DestroyWindow();
		return;
	}
	// -- Esc
	else if (nChar == VK_ESCAPE) 
	{
		// -- Close
		ReleaseCapture();
		DestroyWindow();
		return;
	}
	// -- Left
	else if (nChar == VK_LEFT) 
	{
		// -- -- Move to the left
		if (nSelectedIndex != 0)
		{
			nSelectedIndex--;

			SelectIcon(nSelectedIndex);
		}

	}
	// -- Right
	else if (nChar == VK_RIGHT) 
	{
		// -- -- Move to the right
		if (nSelectedIndex != (GetNumberIcons() - 1))
		{
			nSelectedIndex++;

			SelectIcon(nSelectedIndex);
		}

	}	
	// -- Up
	else if (nChar == VK_UP) 
	{
		// -- -- Move up
		if (nSelectedIndex > (m_nNumColumns - 1))
		{
			nSelectedIndex -= m_nNumColumns;

			SelectIcon(nSelectedIndex);
		}

	}
	// -- Down
	else if (nChar == VK_DOWN) 
	{
		// -- -- Move down
		nSelectedIndex += m_nNumColumns;
		if (nSelectedIndex <= (GetNumberIcons() -1))
		{

			SelectIcon(nSelectedIndex);
		}

	}

	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

int COptionTreeIconPopUp::GetSelIconIndex()
{
	// Declate variables
	int i = 0;

	// Cycle through all of the nodes
	OT_ICON_NODE *curr = m_nAllIcons;
	while (curr != NULL)
	{
		// -- See if this is us
		if (curr->m_bSelected == TRUE)
		{
			return i;
		}
			
		// -- Count
		i++;

		// -- Follow the link to the next node
		curr = curr->m_nNextNode;
	}

	return -1;
}

void COptionTreeIconPopUp::SelectIcon(int nIndex)
{
	// Declate variables
	int i = 0;

	// Deselect all icons
	DeSelectAll();

	// Cycle through all of the nodes
	OT_ICON_NODE *curr = m_nAllIcons;
	while (curr != NULL)
	{
		// -- See if this is us
		if (i == nIndex)
		{
			curr->m_bSelected = TRUE;

			break;
		}
			
		// -- Count
		i++;

		// -- Follow the link to the next node
		curr = curr->m_nNextNode;
	}

	// Force redraw
	Invalidate();
}
