// GridCellColor.cpp: implementation of the CGridCellColor class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "simul.h"
#include "GridCellColor.h"

#include "..\GridCell.h"
#include "..\GridCtrl.h"

#include "Tools\ColourPopup.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(CGridCellColor, CGridCell)

CGridCellColor::CGridCellColor()
{

}

CGridCellColor::~CGridCellColor()
{

}

// Create a control to do the editing
BOOL CGridCellColor::Edit(int nRow, int nCol, CRect rect, CPoint /* point */, UINT nID, UINT nChar)
{
    m_bEditing = TRUE;
    
    // CInPlaceList auto-deletes itself
//    m_pEditWnd = new CInPlaceList(GetGrid(), rect, GetStyle(), nID, nRow, nCol, 
  //                                GetTextClr(), GetBackClr(), m_Strings, GetText(), nChar);

	//m_bActive = TRUE;

	m_pEditWnd = new CColourPopup(CPoint(rect.left, rect.bottom),    // Point to display popup
                     RGB(255,0,0),                       // Selected colour
                     GetGrid(),                              // parent
                     "sdds",                  // "Default" text area
                     "efdfd");                  // Custom Text

//    if (pParent)
//        pParent->SendMessage(CPN_DROPDOWN, (LPARAM)GetColour(), (WPARAM) GetDlgCtrlID());

    // Docs say I should return FALSE to stop the parent also getting the message.
    // HA! What a joke.



    return TRUE;
}

// Cancel the editing.
void CGridCellColor::EndEdit()
{
//    if (m_pEditWnd)
  //      ((CInPlaceList*)m_pEditWnd)->EndEdit();
}

// Override draw so that when the cell is selected, a drop arrow is shown in the RHS.
BOOL CGridCellColor::Draw(CDC* pDC, int nRow, int nCol, CRect rect,  BOOL bEraseBkgnd /*=TRUE*/)
{
#ifdef _WIN32_WCE
    return CGridCell::Draw(pDC, nRow, nCol, rect,  bEraseBkgnd);
#else
    // Cell selected?
    //if ( !IsFixed() && IsFocused())
    if (GetGrid()->IsCellEditable(nRow, nCol) && !IsEditing())
    {
        // Get the size of the scroll box
        CSize sizeScroll(GetSystemMetrics(SM_CXVSCROLL), GetSystemMetrics(SM_CYHSCROLL));

        // enough room to draw?
        if (sizeScroll.cy < rect.Width() && sizeScroll.cy < rect.Height())
        {
            // Draw control at RHS of cell
            CRect ScrollRect = rect;
            ScrollRect.left   = rect.right - sizeScroll.cx;
            ScrollRect.bottom = rect.top + sizeScroll.cy;

            // Do the draw 
            pDC->DrawFrameControl(ScrollRect, DFC_SCROLL, DFCS_SCROLLDOWN);

            // Adjust the remaining space in the cell
            rect.right = ScrollRect.left;
        }
    }

    CString strTempText = GetText();
    if (IsEditing())
        SetText(_T(""));

    // drop through and complete the cell drawing using the base class' method
    return CGridCell::Draw(pDC, nRow, nCol, rect,  bEraseBkgnd);

    if (IsEditing())
        SetText(strTempText);
#endif
}
