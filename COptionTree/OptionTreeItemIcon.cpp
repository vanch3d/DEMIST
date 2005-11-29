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

// OptionTreeItemIcon.cpp : implementation file
//

#include "stdafx.h"
#include "OptionTreeItemIcon.h"

// Added Headers
#include "OptionTree.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptionTreeItemIcon

COptionTreeItemIcon::COptionTreeItemIcon()
{
	// Initialize variables
	m_bFocus = FALSE;
	m_bShowText = TRUE;
	m_nIconSize = 16;
	m_nNumColumns = 3;
	m_nAllIcons = NULL;
	m_rcHitTest = CRect(0, 0, 0, 0);

	// Set item type
	SetItemType(OT_ITEM_ICON);
}

COptionTreeItemIcon::~COptionTreeItemIcon()
{
	// Delete all icons
	Node_DeleteAll();
}


BEGIN_MESSAGE_MAP(COptionTreeItemIcon, CWnd)
	//{{AFX_MSG_MAP(COptionTreeItemIcon)
	ON_WM_LBUTTONUP()
	ON_WM_KILLFOCUS()
	ON_WM_SETFOCUS()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
	ON_MESSAGE(OT_ICON_CLOSE, OnPopUpClose)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// COptionTreeItemIcon message handlers

void COptionTreeItemIcon::DrawAttribute(CDC *pDC, const RECT &rcRect)
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
	COLORREF crOld;
	int nOldBack;
	CRect rcText, rcIcon, rcClient;
	CString strText;
	CBrush bBrush;
	HICON hGreyScale = NULL;
	OT_ICON_NODE *nIconNode;

	// Get window rect
	GetClientRect(rcClient);

	// Clear hit test rectangle
	m_rcHitTest = CRect(0, 0, 0, 0);

	// Select font
	hOld = pDC->SelectObject(m_otOption->GetNormalFont());

	// Get selected icon
	nIconNode = GetSelectedNode();
	
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

	// Make sure we have a selected item
	if (nIconNode != NULL)
	{
		// -- Get icon rectangle
		rcIcon.left  = rcRect.left + 1;
		rcIcon.right = rcIcon.left + m_nIconSize;
		rcIcon.top = rcRect.top + OT_ICON_ICONSPACE;
		rcIcon.bottom = rcIcon.top + m_nIconSize;

		// -- Draw icon
		if (IsReadOnly() == TRUE || m_otOption->IsWindowEnabled() == FALSE)
		{
			hGreyScale = CreateGreyScaleIcon(nIconNode->m_hIcon);
			pDC->DrawState(CPoint(rcIcon.left, rcIcon.top), CSize(m_nIconSize, m_nIconSize), hGreyScale, DSS_NORMAL, (CBrush*)NULL);
		}
		else
		{
			pDC->DrawState(CPoint(rcIcon.left, rcIcon.top), CSize(m_nIconSize, m_nIconSize), nIconNode->m_hIcon, DSS_NORMAL, (CBrush*)NULL);
		}


		// -- Get text rectangle
		rcText.left  = rcIcon.right + OT_SPACE;
		rcText.right = rcRect.right;
		rcText.top = rcRect.top;
		rcText.bottom = rcRect.bottom;

		// -- Get text
		if (m_bShowText == TRUE)
		{
			strText = nIconNode->m_strText;
		}
	}
	else
	{
		// -- Get text rectangle
		rcText.left  = rcRect.left + 1;
		rcText.right = rcRect.right;
		rcText.top = rcRect.top;
		rcText.bottom = rcRect.bottom;

		// -- No icon selected
		strText = OT_ICON_NOSELECTION;
	}


	// Draw text
	pDC->DrawText(strText, rcText, DT_SINGLELINE | DT_VCENTER);
	pDC->DrawText(strText, rcText, DT_SINGLELINE | DT_VCENTER | DT_CALCRECT);

	// Calculate hit test rectangle
	m_rcHitTest.left = rcClient.left;
	m_rcHitTest.top = rcClient.top;
	m_rcHitTest.bottom = rcClient.bottom;
	m_rcHitTest.right = m_rcHitTest.left + rcText.right;

	// Delete grey icon
	if (hGreyScale != NULL)
	{
		::DestroyIcon(hGreyScale);
	}

	// Restore GDI ojects
	pDC->SelectObject(hOld);
	pDC->SetTextColor(crOld);
	pDC->SetBkMode(nOldBack);
}

void COptionTreeItemIcon::OnCommit()
{
	// Hide window
	if (IsWindow(GetSafeHwnd()))
	{
		// -- Show window
		ShowWindow(SW_HIDE);
	}
}

void COptionTreeItemIcon::OnRefresh()
{
	// Set the window positiion
	if (IsWindow(GetSafeHwnd()))
	{
		SetWindowPos(NULL, m_rcAttribute.left, m_rcAttribute.top, m_rcAttribute.Width(), m_rcAttribute.Height(), SWP_NOZORDER | SWP_NOACTIVATE);
	}
}

void COptionTreeItemIcon::OnMove()
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

void COptionTreeItemIcon::OnActivate()
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

void COptionTreeItemIcon::CleanDestroyWindow()
{
	// Destroy window
	if (IsWindow(GetSafeHwnd()))
	{
		// -- Destroy window
		DestroyWindow();
	}
}

void COptionTreeItemIcon::OnDeSelect()
{
	// Hide window
	if (IsWindow(GetSafeHwnd()))
	{
		// -- Show window
		ShowWindow(SW_HIDE);
	}
}

void COptionTreeItemIcon::OnSelect()
{

	/*// Make sure window is valid
	if (IsWindow(GetSafeHwnd()))
	{
		// -- Show window
		ShowWindow(SW_SHOW);

		// -- Set window position
		SetWindowPos(NULL, m_rcAttribute.left, m_rcAttribute.top, m_rcAttribute.Width(), m_rcAttribute.Height(), SWP_NOZORDER | SWP_SHOWWINDOW);

		// -- Set focus
		SetFocus();
	}*/
}


void COptionTreeItemIcon::OnLButtonUp(UINT nFlags, CPoint point) 
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

	// Show icon popup
	if (m_rcHitTest.PtInRect(point) == TRUE && IsReadOnly() == FALSE)
	{
		new COptionTreeIconPopUp(CPoint(rcClient.left, rcClient.bottom), m_nIconSize, m_nNumColumns, m_nAllIcons, m_otOption, this);
	}

	// Update items
	if (m_otOption != NULL)
	{
		m_otOption->UpdatedItems();
	}

	CWnd::OnLButtonUp(nFlags, point);
}

void COptionTreeItemIcon::OnKillFocus(CWnd* pNewWnd) 
{
	// Mark focus
	m_bFocus = FALSE;

	// Commit changes
	CommitChanges();	
	
	CWnd::OnKillFocus(pNewWnd);	
}

void COptionTreeItemIcon::OnSetFocus(CWnd* pOldWnd) 
{
	// Mark focus
	m_bFocus = TRUE;
	
	CWnd::OnSetFocus(pOldWnd);
}

void COptionTreeItemIcon::OnPaint() 
{
	// Do nothing
}

BOOL COptionTreeItemIcon::OnEraseBkgnd(CDC* pDC) 
{
	// Naa, we like flicker free better
	return FALSE;
}

BOOL COptionTreeItemIcon::CreateIconWindow(BOOL bShowText, int nIconSize, int nNumberColumns)
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

			// -- -- Set show text
			SetShowText(bShowText);

			// -- -- Set item height
			SetItemHeight((long) nIconSize + (OT_ICON_ICONSPACE * 2));

			// -- -- Set number of columns
			SetNumberColumns(nNumberColumns);

			// Set icon height
			SetIconSize(nIconSize);

			// -- -- Set window position
			SetWindowPos(NULL, m_rcAttribute.left, m_rcAttribute.top, m_rcAttribute.Width(), m_rcAttribute.Height(), SWP_NOZORDER | SWP_SHOWWINDOW);

			// -- -- Hide window
			ShowWindow(SW_HIDE);
		}
	}

	return bRet;
}

void COptionTreeItemIcon::SetShowText(BOOL bShow)
{
	// Save variable
	m_bShowText = bShow;
}

BOOL COptionTreeItemIcon::GetShowText()
{
	// Return variable
	return m_bShowText;
}

void COptionTreeItemIcon::SetIconSize(int nIconSize)
{
	// Save variable
	m_nIconSize = nIconSize;
}

int COptionTreeItemIcon::GetIconSize()
{
	// Return variable
	return m_nIconSize;
}

void COptionTreeItemIcon::SetNumberColumns(int nNumber)
{
	// Save variable
	m_nNumColumns = nNumber;
}

int COptionTreeItemIcon::GetNumberColumns()
{
	// Return variable
	return m_nNumColumns;
}

HICON COptionTreeItemIcon::CreateGreyScaleIcon(HICON hIcon)
{
	// Declare variables
    HICON hGrayIcon = NULL;
    HDC hMainDC = NULL, hMemDC1 = NULL, hMemDC2 = NULL;
    BITMAP bmp;
    HBITMAP hOldBmp1 = NULL, hOldBmp2 = NULL;
    ICONINFO csII, csGrayII;
    BOOL bRetValue = FALSE;

    bRetValue = ::GetIconInfo(hIcon, &csII);
    if (bRetValue == FALSE) return NULL;

	hMainDC = ::GetDC(m_hWnd);  
	if (hMainDC == NULL) 
		return NULL;  
	hMemDC1 = ::CreateCompatibleDC(hMainDC);  
	hMemDC2 = ::CreateCompatibleDC(hMainDC);  
	if (hMemDC1 == NULL || hMemDC2 == NULL) 
		return NULL; 
  
    if (::GetObject(csII.hbmColor, sizeof(BITMAP), &bmp))
    {
        csGrayII.hbmColor = ::CreateBitmap(csII.xHotspot*2, csII.yHotspot*2, 
                                           bmp.bmPlanes, bmp.bmBitsPixel, NULL);
        if (csGrayII.hbmColor)
        {
            hOldBmp1 = (HBITMAP)::SelectObject(hMemDC1, csII.hbmColor);
            hOldBmp2 = (HBITMAP)::SelectObject(hMemDC2, csGrayII.hbmColor);

            ::BitBlt(hMemDC2, 0, 0, csII.xHotspot*2, csII.yHotspot*2, hMemDC1, 
                     0, 0, SRCCOPY);

            DWORD    dwLoopY = 0, dwLoopX = 0;
            COLORREF crPixel = 0;
            BYTE     byNewPixel = 0;

            for (dwLoopY = 0; dwLoopY < csII.yHotspot*2; dwLoopY++)
            {
                for (dwLoopX = 0; dwLoopX < csII.xHotspot*2; dwLoopX++)
                {
                    crPixel = ::GetPixel(hMemDC2, dwLoopX, dwLoopY);

                    byNewPixel = (BYTE)((GetRValue(crPixel) * 0.299) + 
                                        (GetGValue(crPixel) * 0.587) + 
                                        (GetBValue(crPixel) * 0.114));
                    if (crPixel) ::SetPixel(hMemDC2, dwLoopX, dwLoopY, 
                                            RGB(byNewPixel, byNewPixel, byNewPixel));
                } // for
            } // for

            ::SelectObject(hMemDC1, hOldBmp1);
            ::SelectObject(hMemDC2, hOldBmp2);

            csGrayII.hbmMask = csII.hbmMask;

            csGrayII.fIcon = TRUE;
            hGrayIcon = ::CreateIconIndirect(&csGrayII);
        } // if

        ::DeleteObject(csGrayII.hbmColor);
        //::DeleteObject(csGrayII.hbmMask);
    } // if

    ::DeleteObject(csII.hbmColor);
    ::DeleteObject(csII.hbmMask);
    ::DeleteDC(hMemDC1);
    ::DeleteDC(hMemDC2);
    ::ReleaseDC(m_hWnd, hMainDC);

    return hGrayIcon;
}

OT_ICON_NODE * COptionTreeItemIcon::GetSelectedNode()
{
	// Cycle through all of the nodes
	OT_ICON_NODE *curr = m_nAllIcons;
	while (curr != NULL)
	{
		// -- See if this is us
		if (curr->m_bSelected == TRUE)
		{
			return curr;
		}
		// -- Follow the link to the next node
		curr = curr->m_nNextNode;
	}

	return NULL;
}

int COptionTreeItemIcon::GetSelectedIcon()
{
	// Declate variables
	int nNum = 0;

	// Cycle through all of the nodes
	OT_ICON_NODE *curr = m_nAllIcons;
	while (curr != NULL)
	{
		// -- See if this is us
		if(curr->m_bSelected == TRUE)
		{
			return nNum;
		}

		// -- Count
		nNum++;

		// -- Follow the link to the next node
		curr = curr->m_nNextNode;
	}

	return -1;
}

void COptionTreeItemIcon::Node_Insert(CString strText, HICON hIcon, BOOL bSelected)
{
	// Declare variables
	OT_ICON_NODE *NewNode = new OT_ICON_NODE;

	// Set up the New Node structure
	NewNode->m_bSelected = bSelected;
	NewNode->m_hIcon = hIcon;
	NewNode->m_strText = strText;
	NewNode->m_rcHitRect = CRect(0, 0, 0, 0);
	NewNode->m_nNextNode = NULL;

	// Add new node to list
	// -- Do have any other node?
	if (m_nAllIcons == NULL)
	{
		m_nAllIcons = NewNode;
		return;
	}

	// -- Find the end of the list to add the new node to
	OT_ICON_NODE *curr = m_nAllIcons;
	OT_ICON_NODE *last = NULL;
	while (curr != NULL)
	{
		// -- -- Save this node
		last = curr;

		// -- -- Follow the link to the next node
		curr = curr->m_nNextNode;
	}

	// -- Link the new nod to the place we found
	last->m_nNextNode = NewNode;
}

BOOL COptionTreeItemIcon::InsertIcon(CString strText, HICON hIcon, BOOL bSelected)
{
	// Validate icon
	if (hIcon == NULL)
	{
		return FALSE;
	}

	// Deselect all icons
	if (bSelected == TRUE)
	{
		DeSelectAll();
	}

	// Insert node
	Node_Insert(strText, hIcon, bSelected);

	return TRUE;
}

void COptionTreeItemIcon::DeSelectAll()
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

BOOL COptionTreeItemIcon::InsertIcon(CString strText, UINT uIcon, BOOL bSelected)
{
	// Declare variables
	HICON hIcon;

	// Load icon
	hIcon = (HICON) LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(uIcon), IMAGE_ICON, m_nIconSize, m_nIconSize, LR_DEFAULTCOLOR);

	// Validate icon
	if (hIcon == NULL)
	{
		return FALSE;
	}

	// Deselect all icons
	if (bSelected == TRUE)
	{
		DeSelectAll();
	}

	// Insert node
	Node_Insert(strText, hIcon, bSelected);

	return TRUE;
}

void COptionTreeItemIcon::Node_DeleteAll()
{
	// Do have any other results
	if (m_nAllIcons == NULL)
	{
		return;
	}

	
	// Declare variables
	OT_ICON_NODE *curr = m_nAllIcons;
	OT_ICON_NODE *last = NULL;
	
	// Check to see if we have only 1
	if (m_nAllIcons->m_nNextNode == NULL)
	{
		// -- Delete m_AllResults (head)
		curr = m_nAllIcons;
		delete curr;
		m_nAllIcons = NULL;

		return;
	}
	

	// Find the end of the list to add the new result to
	while (curr->m_nNextNode != NULL)
	{
		// -- Save this node
		last = curr;

		// -- Follow the link to the next node
		curr = curr->m_nNextNode;
	}

	// Delete this node and set last->m_nNextNode to NULL
	delete curr;
	last->m_nNextNode = NULL;

	// Check to see if we are at second to beginning
	if (m_nAllIcons->m_nNextNode == last)
	{
		// -- Delete last (which is second to head)
		delete last;

		// -- Delete m_AllResults (head)
		curr = m_nAllIcons;
		delete curr;
		m_nAllIcons = NULL;

	}

	// Call this again
	Node_DeleteAll();
}

LONG COptionTreeItemIcon::OnPopUpClose(UINT lParam, LONG wParam)
{
	// Hide window
	if (IsWindow(GetSafeHwnd()))
	{
		// -- Show window
		ShowWindow(SW_HIDE);
	}

	// Kill focus so we can draw attribute
	m_bFocus = FALSE;

	// Update items
	if (m_otOption != NULL)
	{
		m_otOption->UpdatedItems();
	}

    return TRUE;
}
