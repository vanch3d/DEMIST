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


// OptionTreeItemSpinner.cpp : implementation file
//

#include "stdafx.h"
#include "OptionTreeItemSpinner.h"

// Added Headers
#include "OptionTree.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptionTreeItemSpinner

COptionTreeItemSpinner::COptionTreeItemSpinner()
{
	// Initialize variables
	m_bFocus = FALSE;

	// Set item type
	SetItemType(OT_ITEM_SPINNER);
}

COptionTreeItemSpinner::~COptionTreeItemSpinner()
{
}


BEGIN_MESSAGE_MAP(COptionTreeItemSpinner, COptionTreeSpinnerButton)
	//{{AFX_MSG_MAP(COptionTreeItemSpinner)
	ON_WM_KILLFOCUS()
	ON_WM_SETFOCUS()
	ON_MESSAGE(OT_NOTIFY_COMMITCHANGES, WM_CommitChanges)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptionTreeItemSpinner message handlers

void COptionTreeItemSpinner::OnKillFocus(CWnd* pNewWnd) 
{
	// Validate
	if (m_otOption == NULL)
	{
		COptionTreeSpinnerButton::OnKillFocus(pNewWnd);
		return;
	}

	// See if new window is tree of list
	if (m_otOption->IsChild(pNewWnd) == FALSE)
	{
		COptionTreeSpinnerButton::OnKillFocus(pNewWnd);
		return;
	}

	// If new focus is edit, we really aren't loosing focus
	if (pNewWnd->GetSafeHwnd() != GetEdit()->GetSafeHwnd())
	{
		// Mark focus
		m_bFocus = FALSE;

		// Commit changes
		CommitChanges();
	}
	
	COptionTreeSpinnerButton::OnKillFocus(pNewWnd);	
}

void COptionTreeItemSpinner::OnSetFocus(CWnd* pOldWnd) 
{
	// Mark focus
	m_bFocus = TRUE;

	COptionTreeSpinnerButton::OnSetFocus(pOldWnd);	
}

void COptionTreeItemSpinner::OnActivate()
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

void COptionTreeItemSpinner::DrawAttribute(CDC *pDC, const RECT &rcRect)
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
	CRect rcText;
	CString strText;

	// Get window text
	strText = GetEditText();
	m_strEdit = strText;

	// Select font
	hOld = pDC->SelectObject(m_otOption->GetNormalFont());
	
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

	// Get rectangle
	rcText = rcRect;

	// Draw text
	pDC->DrawText(strText, rcText, DT_SINGLELINE | DT_VCENTER);

	// Restore GDI ojects
	pDC->SelectObject(hOld);
	pDC->SetTextColor(crOld);
	pDC->SetBkMode(nOldBack);
}

void COptionTreeItemSpinner::OnCommit()
{
	// Hide edit control
	if (IsWindow(GetSafeHwnd()))
	{
		// -- Show window
		ShowWindow(SW_HIDE);
	}
}

void COptionTreeItemSpinner::OnRefresh()
{
	// Set the window positiion
	if (IsWindow(GetSafeHwnd()))
	{
		SetWindowPos(NULL, m_rcAttribute.left, m_rcAttribute.top, m_rcAttribute.Width(), m_rcAttribute.Height(), SWP_NOZORDER | SWP_NOACTIVATE);
	}
}

void COptionTreeItemSpinner::OnMove()
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

void COptionTreeItemSpinner::CleanDestroyWindow()
{
	// Destroy window
	if (IsWindow(GetSafeHwnd()))
	{
		// -- Destroy window
		DestroyWindow();
	}
}


BOOL COptionTreeItemSpinner::CreateSpinnerWindow(BOOL bWrapAround, BOOL bUserEdit, double dValue, double dRangeBottom, double dRangeTop)
{
	// Declare variables
	DWORD dwStyle = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	BOOL bRet = FALSE;
	CString strText;

	// Make sure options is not NULL
	if (m_otOption == NULL)
	{
		return FALSE;
	}

	// Create edit control
	if (IsWindow(GetSafeHwnd()) == FALSE)
	{
		// -- Create the edit view
		bRet = Create(AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW, ::LoadCursor(NULL, IDC_ARROW)), _T(""), dwStyle, m_rcAttribute, m_otOption->GetCtrlParent(), GetCtrlID());

		// -- Setup window
		if (bRet == TRUE)
		{
			// -- -- Set wrap around
			SetWrapAround(bWrapAround);

			// -- -- Set allow user edit
			SetUserEdit(bUserEdit);

			// -- -- Set range
			SetRange(dRangeBottom, dRangeTop);

			// -- -- Set value
			SetEditDouble(dValue);

			// -- -- Set options owner
			SetSpinnerOptionsOwner(m_otOption);

			// -- -- Set font
			SetFont(m_otOption->GetNormalFont(), TRUE);
			
			// -- -- Set window position
			SetWindowPos(NULL, m_rcAttribute.left, m_rcAttribute.top, m_rcAttribute.Width(), m_rcAttribute.Height(), SWP_NOZORDER | SWP_SHOWWINDOW);

			// -- -- Hide window
			ShowWindow(SW_HIDE);

		}
	}

	return bRet;
}

LRESULT COptionTreeItemSpinner::WM_CommitChanges(WPARAM wParam, LPARAM lParam)
{
	// Mark focus
	m_bFocus = FALSE;

	// Commit changes
	CommitChanges();

	return 0;
}

void COptionTreeItemSpinner::OnDeSelect()
{
	// Hide window
	if (IsWindow(GetSafeHwnd()))
	{
		// -- Show window
		ShowWindow(SW_HIDE);
	}
}

void COptionTreeItemSpinner::OnSelect()
{
	// Do nothing here
}
