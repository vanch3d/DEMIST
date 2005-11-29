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


// OptionTreeList.cpp : implementation file
//

#include "stdafx.h"
#include "OptionTreeList.h"

// Added Headers
#include "resource.h"
#include "OptionTree.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptionTreeList

COptionTreeList::COptionTreeList()
{
	// Initialize variables
	m_lPrevCol = 0;
	m_bColDrag = FALSE;
	m_hSplitter = NULL;
	m_hHand = NULL;
}

COptionTreeList::~COptionTreeList()
{
	// Delete objects
	// -- Splitter cursor
	if (m_hSplitter != NULL)
	{
		::DestroyCursor(m_hSplitter);
	}
	// -- Hand cursor
	if (m_hHand != NULL)
	{
		::DestroyCursor(m_hHand);
	}
}


BEGIN_MESSAGE_MAP(COptionTreeList, CWnd)
	//{{AFX_MSG_MAP(COptionTreeList)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_KEYDOWN()
	ON_WM_GETDLGCODE()
	ON_WM_ERASEBKGND()
	ON_WM_VSCROLL()
	ON_WM_KILLFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// COptionTreeList message handlers

BOOL COptionTreeList::Create(DWORD dwStyle, RECT rcRect, CWnd* pParentWnd, UINT nID)
{
	
	// Load cursors
	// -- Splitter
	if (m_hSplitter == NULL)
	{
		m_hSplitter = ::LoadCursor(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_SPLITTER));
	}
	// -- Get Hand Cursor
	GetHandCursor();

	// Create the window
	return CWnd::Create(AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS, ::LoadCursor(NULL, IDC_ARROW)), _T(""), dwStyle, rcRect, pParentWnd, nID);
}

void COptionTreeList::OnSize(UINT nType, int cx, int cy) 
{
	// Make sure we have an option tree
	if (m_otOption != NULL)
	{
		// -- Update resize
		UpdateResize();

		// -- Redraw
		Invalidate();

		// -- Update window
		UpdateWindow();

		// -- Inform all items that a resize has been made
		m_otOption->UpdateMoveAllItems();
	}

	CWnd::OnSize(nType, cx, cy);	
}

void COptionTreeList::UpdateResize()
{
	// Make sure not NULL
	if (m_otOption == NULL)
	{
		return;
	}

	// Declare variables
	SCROLLINFO si;
	int nHeight;
	long lHeight = 0;
	CRect rcClient;
	COptionTreeItem *otiRoot, *otiItem;

	// Get client rectangle
	GetClientRect(rcClient);
	nHeight = rcClient.Height() + 1;

	// Get root item
	otiRoot = m_otOption->GetRootItem();
	if (otiRoot == NULL)
	{
		return;
	}

	// Get total height
	otiItem = otiRoot->GetChild();
	while (otiItem != NULL)
	{
		// -- Get height
		lHeight	+= otiItem->GetTotalHeight();

		// -- Get next item
		otiItem = otiItem->GetSibling();
	}

	// Setup scroll info
	ZeroMemory(&si, sizeof(SCROLLINFO));
	si.cbSize = sizeof(SCROLLINFO);
	si.fMask = SIF_RANGE | SIF_PAGE;
	si.nMin = 0;
	si.nMax = (int) lHeight;
	si.nPage = nHeight;

	// Set scroll info
	if ((int)si.nPage > si.nMax)
	{
		m_otOption->SetOriginOffset(0);
	}
	SetScrollInfo(SB_VERT, &si, TRUE);

	// Set column
	m_otOption->SetColumn(m_otOption->GetColumn());
}

void COptionTreeList::OnPaint() 
{
	// Make sure valid
	if (m_otOption == NULL)
	{
		return;
	}

	// Declare variables
	CPaintDC dc(this);
	CDC* pDCMem = new CDC;
	CBitmap bpMem;
	CBitmap *bmOld;
	COptionTreeItem* otiItem;
	CRect rcClient;
	HGDIOBJ hOldBrush;
	long lTotal, lHeight;
	HRGN hRgn;


	// Get client rectangle
	GetClientRect(rcClient);

	// Clear visible list
	m_otOption->ClearVisibleList();

	// Clear all label rectangle
	m_otOption->ClearAllLabelRect();

	// Create DC
	pDCMem->CreateCompatibleDC(&dc);

	// Create bitmap
	bpMem.CreateCompatibleBitmap(&dc, rcClient.Width(), rcClient.Height());

	// Select bitmap
	bmOld = pDCMem->SelectObject(&bpMem);

	// Draw control background
	hOldBrush = pDCMem->SelectObject(GetSysColorBrush(COLOR_BTNFACE));
	pDCMem->PatBlt(rcClient.left, rcClient.top, rcClient.Width(), rcClient.Height(), PATCOPY);

	// Draw control inside fill color
	rcClient.DeflateRect(2, 2);
	if (m_otOption->IsWindowEnabled() == TRUE)
	{
		pDCMem->SelectObject(GetSysColorBrush(COLOR_WINDOW));
	}
	else
	{
		pDCMem->SelectObject(GetSysColorBrush(COLOR_3DFACE));
	}
	pDCMem->PatBlt(rcClient.left, rcClient.top, rcClient.Width(), rcClient.Height(), PATCOPY);
	rcClient.InflateRect(2, 2);

	// Draw expand column	
	if (m_otOption->GetShadeExpandColumn() == TRUE || m_otOption->IsWindowEnabled() == FALSE)
	{
		pDCMem->SelectObject(GetSysColorBrush(COLOR_BTNFACE));
	}
	else
	{
		pDCMem->SelectObject(GetSysColorBrush(COLOR_WINDOW));
	}
	pDCMem->PatBlt(0, 0, OT_EXPANDCOLUMN, rcClient.Height(), PATCOPY);

	// Draw vertical sep
	_DrawDarkVLine(pDCMem->GetSafeHdc(), OT_EXPANDCOLUMN, 0, rcClient.bottom);

	// Draw edge
	pDCMem->DrawEdge(&rcClient, BDR_SUNKENOUTER, BF_RECT);

	// Deflate rectangle
	rcClient.DeflateRect(2, 2);

	// Create clip region
	hRgn = CreateRectRgn(rcClient.left, rcClient.top, rcClient.right, rcClient.bottom);
	SelectClipRgn(pDCMem->m_hDC, hRgn);

	// Inflate rectangle
	rcClient.InflateRect(2, 2);

	// Draw all items
	lTotal = 0;
	for (otiItem = m_otOption->GetRootItem()->GetChild(); otiItem != NULL; otiItem = otiItem->GetSibling())
	{
		lHeight = otiItem->DrawItem(pDCMem, rcClient, 0, lTotal);
		lTotal += lHeight;
	}

	// Remove clip region
	SelectClipRgn(pDCMem->GetSafeHdc(), NULL);
	DeleteObject(hRgn);

	// Copy back buffer to the display
	dc.GetClipBox(&rcClient);
	dc.BitBlt(rcClient.left, rcClient.top, rcClient.Width(), rcClient.Height(), pDCMem, rcClient.left, rcClient.top, SRCCOPY);
	
	// Select old objects
	pDCMem->SelectObject(hOldBrush);
	pDCMem->SelectObject(bmOld);
	
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

BOOL COptionTreeList::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	// Hit test
	if (nHitTest == HTCLIENT)
	{
		// -- Declare variables
		CPoint ptPoint;

		// -- Validate option
		if (m_otOption == NULL)
		{
			return CWnd::OnSetCursor(pWnd, nHitTest, message);
		}

		// Get cursor position
		GetCursorPos(&ptPoint);
		ScreenToClient(&ptPoint);

		// -- Run hit test and set cursor
		switch (m_otOption->HitTest(ptPoint))
		{
			case OT_HIT_COLUMN:
				SetCursor(m_hSplitter);
				return TRUE;

			case OT_HIT_EXPAND:
				SetCursor(m_hHand);
				return TRUE;
		}
	}
	
	return CWnd::OnSetCursor(pWnd, nHitTest, message);
}

void COptionTreeList::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// Validate option
	if (m_otOption == NULL)
	{
		return;
	}

	// Is disabled
	if (m_otOption->IsDisableInput() == TRUE)
	{
		return;
	}

	// Send notify to user
	m_otOption->SendNotify(NM_CLICK);

	// Make sure window is enabled
	if (!m_otOption->IsWindowEnabled())
	{
		return;
	}

	// Declare variables
	long lHit;
	COptionTreeItem *otiItem;
	COptionTreeItem *oliOldFocus;
	CRect rcClient, rcLabel;
	CDC* pDC;

	// Get client rectangle
	GetClientRect(rcClient);

	// Set focus to window
	SetFocus();

	// Hit test
	lHit = m_otOption->HitTest(point);
	switch (lHit)
	{
		case OT_HIT_COLUMN:
			
			if (m_otOption->SendNotify(OT_NOTIFY_COLUMNCLICK))
			{
				break;
			}

			// -- Set capture
			m_bColDrag = TRUE;
			SetCapture();

			m_lPrevCol = m_otOption->GetOrigin().x;

			// -- Paint drag line
			pDC = GetDC();
			pDC->PatBlt(m_lPrevCol - OT_COLRNG / 2, 0, OT_COLRNG, rcClient.bottom, PATINVERT);
			ReleaseDC(pDC);

			break;

		case OT_HIT_EXPAND:

			if ((otiItem = m_otOption->FindItem(point)) != NULL)
			{
				if (otiItem->GetChild() && !m_otOption->SendNotify(OT_NOTIFY_ITEMEXPANDING, otiItem))
				{
					// -- Expand
					otiItem->Expand(!otiItem->IsExpanded());

					//  -- Update items
					m_otOption->UpdatedItems();

					// -- Update resize
					UpdateResize();

					// -- Force redraw
					Invalidate();

					// -- Update window
					UpdateWindow();

					// -- Check visible
					CheckVisibleFocus();

					// -- Get largest label
					rcLabel = m_otOption->GetLargestLabel();

					// -- Resize limit
					// -- -- Left
					if (m_otOption->GetColumn() < (rcLabel.BottomRight().x + OT_SPACE))
					{
						// -- -- -- Set column
						m_otOption->SetColumn(rcLabel.BottomRight().x + OT_SPACE);
					}
					// -- -- Right
					else if (m_otOption->GetColumn() > (rcClient.BottomRight().x - OT_RIGHTRESIZELIMIT))
					{
						// -- -- -- Set column
						m_otOption->SetColumn(rcClient.BottomRight().x - OT_RIGHTRESIZELIMIT);
					}

				}
			}
			break;

		default:

			if ((otiItem = m_otOption->FindItem(point)) != NULL)
			{
				// -- Get old focus
				oliOldFocus = m_otOption->GetFocusedItem();

				// -- Select items
				m_otOption->SelectItems(NULL, FALSE);

				// -- Set focus item
				m_otOption->SetFocusedItem(otiItem);

				// -- Select
				otiItem->Select();

				// -- Force redraw
				Invalidate();

				// -- Make sure new item
				if (otiItem != oliOldFocus)
				{
					m_otOption->SendNotify(OT_NOTIFY_SELCHANGE, otiItem);
				}

				// -- Send notify
				if (lHit == OT_HIT_ATTRIBUTE && !otiItem->IsRootLevel())
				{
					if (!m_otOption->SendNotify(OT_NOTIFY_PROPCLICK, otiItem) && !otiItem->IsReadOnly())
					{
						otiItem->Activate();
					}
				}
			}
			else
			{
				// -- Selct items
				m_otOption->SelectItems(NULL, FALSE);

				// -- Set focus item
				m_otOption->SetFocusedItem(NULL);

				// -- Send notify
				m_otOption->SendNotify(OT_NOTIFY_SELCHANGE);

				// -- Force redraw
				Invalidate();
			}
			break;
	}


	CWnd::OnLButtonDown(nFlags, point);
}

void COptionTreeList::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// Draw column
	if (m_bColDrag == TRUE)
	{
		// -- Declare variables
		CDC* pDC;
		CRect rcClient;
		CRect rcLabel;

		// -- Get client rectangle
		GetClientRect(rcClient);

		// -- Paint drag line
		pDC = GetDC();
		pDC->PatBlt(m_lPrevCol - OT_COLRNG / 2, 0, OT_COLRNG, rcClient.bottom, PATINVERT);
		ReleaseDC(pDC);

		// -- Release capture
		m_bColDrag = FALSE;

		// -- Release capture
		ReleaseCapture();

		// -- Get largest label
		rcLabel = m_otOption->GetLargestLabel();

		// -- Resize limit
		// -- -- Left
		if (point.x < (rcLabel.BottomRight().x + OT_SPACE))
		{
			// -- -- -- Set column
			m_otOption->SetColumn(rcLabel.BottomRight().x + OT_SPACE);
		}
		// -- -- Right
		else if (point.x > (rcClient.BottomRight().x - OT_RIGHTRESIZELIMIT))
		{
			// -- -- -- Set column
			m_otOption->SetColumn(rcClient.BottomRight().x - OT_RIGHTRESIZELIMIT);
		}
		else
		{
			// -- -- -- Set column
			m_otOption->SetColumn(point.x);
		}

		// -- Update move items
		m_otOption->UpdateMoveAllItems();

		// -- Force redraw
		Invalidate();
	}
	
	CWnd::OnLButtonUp(nFlags, point);
}

void COptionTreeList::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// Validate option
	if (m_otOption == NULL)
	{
		return;
	}

	// Declare variables
	COptionTreeItem *otiItem;
	COptionTreeItem *oliOldFocus;
	CRect rcLabel, rcClient;

	// Send notify to user
	m_otOption->SendNotify(NM_DBLCLK);

	// Get client rect
	GetClientRect(rcClient);

	// Hit test
	if ((otiItem = m_otOption->FindItem(point)) != NULL && otiItem->GetChild())
	{
		switch (m_otOption->HitTest(point))
		{
			case OT_HIT_COLUMN:
				break;

			case OT_HIT_ATTRIBUTE:

				if (!otiItem->IsRootLevel())
				{
					break;
				}

			default:
				// -- Get focus item
				oliOldFocus = m_otOption->GetFocusedItem();

				// -- Select items
				m_otOption->SelectItems(NULL, FALSE);

				// -- Set focus item
				m_otOption->SetFocusedItem(otiItem);

				// -- Select
				otiItem->Select();
			
				// -- Send notify to user
				if (otiItem != oliOldFocus)
				{
					m_otOption->SendNotify(OT_NOTIFY_SELCHANGE, otiItem);
				}

			case OT_HIT_EXPAND:

				if (!m_otOption->SendNotify(OT_NOTIFY_ITEMEXPANDING, otiItem))
				{
					// -- Expand
					otiItem->Expand(!otiItem->IsExpanded());

					//  -- Update items
					m_otOption->UpdatedItems();

					// -- Update resize
					UpdateResize();

					// -- Force redraw
					Invalidate();

					// -- Update window
					UpdateWindow();

					// -- Check visible
					CheckVisibleFocus();

					// -- Get largest label
					rcLabel = m_otOption->GetLargestLabel();

					// -- Resize limit
					// -- -- Left
					if (m_otOption->GetColumn() < (rcLabel.BottomRight().x + OT_SPACE))
					{
						// -- -- -- Set column
						m_otOption->SetColumn(rcLabel.BottomRight().x + OT_SPACE);
					}
					// -- -- Right
					else if (m_otOption->GetColumn() > (rcClient.BottomRight().x - OT_RIGHTRESIZELIMIT))
					{
						// -- -- -- Set column
						m_otOption->SetColumn(rcClient.BottomRight().x - OT_RIGHTRESIZELIMIT);
					}
				}
				break;
		}
	}
	
	CWnd::OnLButtonDblClk(nFlags, point);
}

void COptionTreeList::OnMouseMove(UINT nFlags, CPoint point) 
{
	// Make sure option is not NULL
	if (m_otOption == NULL)
	{
		return;
	}

	// Drag mode
	if (m_bColDrag == TRUE)
	{
		// -- Declare variables
		CDC *pDC = GetDC();
		CRect rcClient;

		// -- Get client rectnagle
		GetClientRect(rcClient);

		// -- Draw previous drag line
		pDC->PatBlt(m_lPrevCol - OT_COLRNG/2, 0, OT_COLRNG, rcClient.bottom, PATINVERT);
		
		// -- Paint current drag line
		pDC->PatBlt(point.x - OT_COLRNG/2, 0, OT_COLRNG, rcClient.bottom, PATINVERT);	

		// -- Save previous point
		m_lPrevCol = point.x;

		// -- Release DC
		ReleaseDC(pDC);		
	}
	
	CWnd::OnMouseMove(nFlags, point);
}

BOOL COptionTreeList::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	// Declare variables
	SCROLLINFO si;
	CRect rcClient;

	// Setup scrollbar info
	ZeroMemory(&si, sizeof(SCROLLINFO));
	si.cbSize = sizeof(SCROLLINFO);
	si.fMask = SIF_RANGE;

	// Get scrollbar info
	GetScrollInfo(SB_VERT, &si);

	// Get client rectnagle
	GetClientRect(rcClient);

	// Validate
	if (si.nMax - si.nMin < rcClient.Height())
	{
		return TRUE;
	}

	// Set focus
	SetFocus();

	// Vertical scroll
	OnVScroll(zDelta < 0 ? SB_LINEDOWN : SB_LINEUP, 0, NULL);
	
	return TRUE;
	//return CWnd::OnMouseWheel(nFlags, zDelta, pt);
}

void COptionTreeList::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// Declare variables
	COptionTreeItem* otiItem;
	CRect rcClient, rcLabel;

	// Get client rectangle
	GetClientRect(rcClient);

	// Validate option
	if (m_otOption == NULL)
	{
		return;
	}

	// See if disabled
	if (m_otOption->IsDisableInput() || !m_otOption->IsWindowEnabled())
	{
		return;
	}

	switch (nChar)
	{
		case VK_TAB:

			// -- Shift
			if (GetKeyState(VK_SHIFT) < 0)
			{
				// -- -- Focus next
				otiItem = m_otOption->GetFocusedItem();
				if (otiItem != NULL && !otiItem->IsRootLevel())
				{
					m_otOption->FocusPrev();
				}

				// -- -- Activate
				otiItem = m_otOption->GetFocusedItem();
				if (otiItem != NULL && !otiItem->IsRootLevel() && !otiItem->IsReadOnly())
				{
					otiItem->Activate();
				}
			}
			// -- No shift
			else
			{
				// -- -- Focus next
				otiItem = m_otOption->GetFocusedItem();
				if (otiItem != NULL && !otiItem->IsRootLevel())
				{
					m_otOption->FocusNext();
				}

				// -- -- Activate
				otiItem = m_otOption->GetFocusedItem();
				if (otiItem != NULL && !otiItem->IsRootLevel() && !otiItem->IsReadOnly())
				{
					otiItem->Activate();
				}
			}

			break;

		case VK_RETURN:

			// -- Activate
			otiItem = m_otOption->GetFocusedItem();
			if (otiItem != NULL && !otiItem->IsRootLevel() && !otiItem->IsReadOnly())
			{
				otiItem->Activate();
			}
			break;

		case VK_HOME:

			// -- Focus on first item
			if (m_otOption->FocusFirst())
			{
				Invalidate();
			}
			break;

		case VK_END:

			// -- Focus on last item
			if (m_otOption->FocusLast())
			{
				Invalidate();
			}
			break;

		case VK_LEFT:

			// -- Get focused item
			otiItem = m_otOption->GetFocusedItem();
			if (otiItem != NULL)
			{
				// -- -- Send notify to user
				if (!m_otOption->SendNotify(OT_NOTIFY_ITEMEXPANDING, otiItem))
				{
					// -- -- -- Validate
					if (otiItem->GetChild() && otiItem->IsExpanded())
					{
						// -- -- -- -- Expand
						otiItem->Expand(FALSE);

						//  -- -- -- -- Update items
						m_otOption->UpdatedItems();

						// -- -- -- -- Resize
						UpdateResize();

						// -- -- -- -- Force redraw
						Invalidate();

						// -- -- -- -- Update window
						UpdateWindow();

						// -- -- -- -- Check visible focus
						CheckVisibleFocus();


						break;
					}
				}
			}
			else
				break;
			
		case VK_UP:
			
			// -- Move focus up
			if (m_otOption->FocusPrev())
			{
				Invalidate();
			}
			break;

		case VK_RIGHT:

			// -- Get focused item
			otiItem = m_otOption->GetFocusedItem();
			if (otiItem != NULL)
			{
				// -- -- Send notify to user
				if (!m_otOption->SendNotify(OT_NOTIFY_ITEMEXPANDING, otiItem))
				{
					// -- -- -- Validate
					if (otiItem->GetChild() && !otiItem->IsExpanded())
					{
						// -- -- -- -- Expand
						otiItem->Expand(TRUE);

						//  -- -- -- -- Update items
						m_otOption->UpdatedItems();

						// -- -- -- -- Resize
						UpdateResize();

						// -- -- -- -- Force redraw
						Invalidate();

						// -- -- -- -- Update window
						UpdateWindow();

						// -- -- -- -- Check visible focus
						CheckVisibleFocus();

						// -- -- -- -- Get largest label
						rcLabel = m_otOption->GetLargestLabel();

						// -- -- -- -- Resize limit
						// -- -- -- -- -- Left
						if (m_otOption->GetColumn() < (rcLabel.BottomRight().x + OT_SPACE))
						{
							// -- -- -- -- -- -- Set column
							m_otOption->SetColumn(rcLabel.BottomRight().x + OT_SPACE);
						}
						// -- -- -- -- -- Right
						else if (m_otOption->GetColumn() > (rcClient.BottomRight().x - OT_RIGHTRESIZELIMIT))
						{
							// -- -- -- -- -- -- Set column
							m_otOption->SetColumn(rcClient.BottomRight().x - OT_RIGHTRESIZELIMIT);
						}

						break;
					}
				}
			}
			else
				break;
			
		case VK_DOWN:

			// -- Move focus down
			if (m_otOption->FocusNext())
			{
				Invalidate();
			}
			break;
	}
	
	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

UINT COptionTreeList::OnGetDlgCode() 
{
	return DLGC_WANTARROWS | DLGC_WANTCHARS | DLGC_WANTALLKEYS;
}

void COptionTreeList::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// Declare variables
	SCROLLINFO si;
	CRect rcClient;
	long lHeight, lNY;

	// Setup scrollbar information
	ZeroMemory(&si, sizeof(SCROLLINFO));
	si.cbSize = sizeof(SCROLLINFO);
	si.fMask = SIF_RANGE;

	// Get client rectangle
	GetClientRect(rcClient);
	
	// Set focus
	SetFocus();

	// Get height
	lHeight = rcClient.Height() + 1;

	// Get scrollbar information
	GetScrollInfo(SB_VERT, &si);

	// Get origin
	lNY = m_otOption->GetOrigin().y;

	// Switch scrollbar code
	switch (nSBCode)
	{
		case SB_LINEDOWN:
			lNY += OT_DEFHEIGHT;
			break;

		case SB_LINEUP:
			lNY -= OT_DEFHEIGHT;
			break;

		case SB_PAGEDOWN:
			lNY += lHeight;
			break;

		case SB_PAGEUP:
			lNY -= lHeight;
			break;

		case SB_THUMBTRACK:
			lNY = nPos;
			break;
	}

	// Calculate
	lNY = __min(__max(lNY, si.nMin), si.nMax - lHeight);

	// Set origin
	m_otOption->SetOriginOffset(lNY);

	// Set scrollbar info
	si.fMask = SIF_POS;
	si.nPos = lNY;
	SetScrollInfo(SB_VERT, &si, TRUE);

	// Force to redraw
	Invalidate();
}

void COptionTreeList::CheckVisibleFocus()
{
	// Declare variables
	COptionTreeItem *otiItem;	

	// Validate option
	if (m_otOption == NULL)
	{
		return;
	}
	
	// Get focused item
	otiItem = m_otOption->GetFocusedItem();
	if (otiItem == NULL)
	{
		return;
	}

	// See if item is visible
	if (!m_otOption->IsItemVisible(otiItem))
	{
		// -- Single select
		if (m_otOption->IsSingleSelection())
		{
			otiItem->Select(FALSE);
		}

		// -- Set focus
		m_otOption->SetFocusedItem(NULL);

		// -- Send notify to user
		m_otOption->SendNotify(OT_NOTIFY_SELCHANGE, NULL);

		// -- Force redraw
		Invalidate();
	}
}

void COptionTreeList::SetOptionsOwner(COptionTree *otOption)
{
	// Save pointer
	m_otOption = otOption;
}

BOOL COptionTreeList::OnEraseBkgnd(CDC* pDC) 
{
	// Ha, Ha
	return FALSE;
}
void COptionTreeList::OnKillFocus(CWnd* pNewWnd) 
{
	CWnd::OnKillFocus(pNewWnd);	
}

void COptionTreeList::GetHandCursor()
{
    // Declare variables
	CString strWndDir;
	HMODULE hModule;
	
	// Get the windows directory
	GetWindowsDirectory(strWndDir.GetBuffer(MAX_PATH), MAX_PATH);
	strWndDir.ReleaseBuffer();
	strWndDir += _T("\\winhlp32.exe");

    // This retrieves cursor #106 from winhlp32.exe, which is a hand pointer
    hModule = LoadLibrary(strWndDir);
    if (hModule) 
	{
        m_hHand = ::LoadCursor(hModule, MAKEINTRESOURCE(106));
    }

	// Free library
    FreeLibrary(hModule);
   
}
