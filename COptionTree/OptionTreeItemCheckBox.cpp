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

// OptionTreeItemCheckBox.cpp : implementation file
//

#include "stdafx.h"
#include "OptionTreeItemCheckBox.h"

// Added Headers
#include "OptionTree.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptionTreeItemCheckBox

COptionTreeItemCheckBox::COptionTreeItemCheckBox()
{
	// Initialize variables
	m_bFocus = FALSE;
	m_rcCheck = CRect(0, 0, 0, 0);

	// Set item type
	SetItemType(OT_ITEM_CHECKBOX);
}

COptionTreeItemCheckBox::~COptionTreeItemCheckBox()
{

}


BEGIN_MESSAGE_MAP(COptionTreeItemCheckBox, COptionTreeCheckButton)
	//{{AFX_MSG_MAP(COptionTreeItemCheckBox)
	ON_WM_KILLFOCUS()
	ON_WM_SETFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// COptionTreeItemCheckBox message handlers

void COptionTreeItemCheckBox::DrawAttribute(CDC *pDC, const RECT &rcRect)
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
	CRect rcText, rcCheck;
	CString strText;

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

	// Draw check box
	if (GetShowCheck() == TRUE)
	{
		// -- Get rectangle
		rcCheck.left  = rcRect.left;
		rcCheck.right = rcCheck.left + (long) OT_CHECKBOX_SIZE;
		rcCheck.top = rcRect.top + OT_SPACE - 2;
		rcCheck.bottom = rcCheck.top + (long) OT_CHECKBOX_SIZE;
		
		// -- Draw check
		if (GetCheck() == TRUE)
		{
			pDC->DrawFrameControl(&rcCheck, DFC_BUTTON, DFCS_FLAT | DFCS_BUTTONCHECK | DFCS_CHECKED);
		}
		else
		{
			pDC->DrawFrameControl(&rcCheck, DFC_BUTTON, DFCS_FLAT | DFCS_BUTTONCHECK);
		}
	}

	// Draw text
	if (GetShowText() == TRUE)
	{
		// -- Get text
		if (GetCheck() == TRUE)
		{
			strText = GetCheckedText();
		}
		else
		{
			strText = GetUnCheckedText();
		}

		// -- Get rectangle
		if (GetShowCheck() == TRUE)
		{
			rcText.left  = rcCheck.right + OT_SPACE;
			rcText.right = rcRect.right;
			rcText.top = rcRect.top;
			rcText.bottom = rcRect.bottom;
		}
		else
		{
			rcText  = rcRect;
		}

		// -- Draw text
		pDC->DrawText(strText, rcText, DT_SINGLELINE | DT_VCENTER);
	}

	// Restore GDI ojects
	pDC->SelectObject(hOld);
	pDC->SetTextColor(crOld);
	pDC->SetBkMode(nOldBack);
}

void COptionTreeItemCheckBox::OnCommit()
{
	// Hide edit control
	if (IsWindow(GetSafeHwnd()))
	{
		// -- Show window
		ShowWindow(SW_HIDE);
	}
}

void COptionTreeItemCheckBox::OnRefresh()
{
	// Set the window positiion
	if (IsWindow(GetSafeHwnd()))
	{
		SetWindowPos(NULL, m_rcAttribute.left, m_rcAttribute.top, m_rcAttribute.Width(), m_rcAttribute.Height(), SWP_NOZORDER | SWP_NOACTIVATE);
	}
}

void COptionTreeItemCheckBox::OnMove()
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

void COptionTreeItemCheckBox::OnActivate()
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

void COptionTreeItemCheckBox::OnSetFocus(CWnd* pOldWnd) 
{
	// Mark focus
	m_bFocus = TRUE;	
	
	COptionTreeCheckButton::OnSetFocus(pOldWnd);	
}

void COptionTreeItemCheckBox::OnKillFocus(CWnd* pNewWnd) 
{	
	// Validate
	if (m_otOption == NULL)
	{
		COptionTreeCheckButton::OnKillFocus(pNewWnd);
		return;
	}

	// See if new window is tree of list
	if (m_otOption->IsChild(pNewWnd) == TRUE)
	{
		// -- Mark focus
		m_bFocus = FALSE;

		// -- Commit changes
		CommitChanges();
	}

	COptionTreeCheckButton::OnKillFocus(pNewWnd);	
}


void COptionTreeItemCheckBox::CleanDestroyWindow()
{
	// Destroy window
	if (IsWindow(GetSafeHwnd()))
	{
		// -- Destroy window
		DestroyWindow();
	}
}


BOOL COptionTreeItemCheckBox::CreateCheckBoxWindow(BOOL bChecked, BOOL bShowCheck, BOOL bShowText)
{
	// Declare variables
	DWORD dwStyle = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	BOOL bRet = FALSE;

	// Make sure options is not NULL
	if (m_otOption == NULL)
	{
		return FALSE;
	}

	// Create edit control
	if (!IsWindow(GetSafeHwnd()))
	{
		// -- Create the edit view
		bRet = Create(AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW, ::LoadCursor(NULL, IDC_ARROW)), _T(""), dwStyle, m_rcAttribute, m_otOption->GetCtrlParent(), GetCtrlID());

		// -- Setup window
		if (bRet == TRUE)
		{
			// -- -- Set font
			SetFont(m_otOption->GetNormalFont(), TRUE);

			// -- -- Set show text options
			SetShowText(bShowText);

			// -- -- Set show check
			SetShowCheck(bShowCheck);

			// -- -- Set check
			SetCheck(bChecked);

			// -- Set check options owner
			SetCheckOptionsOwner(m_otOption);

			// -- -- Set window position
			SetWindowPos(NULL, m_rcAttribute.left, m_rcAttribute.top, m_rcAttribute.Width(), m_rcAttribute.Height(), SWP_NOZORDER | SWP_SHOWWINDOW);

			// -- -- Hide window
			ShowWindow(SW_HIDE);
		}
	}

	return bRet;
}

void COptionTreeItemCheckBox::OnDeSelect()
{
	// Hide window
	if (IsWindow(GetSafeHwnd()))
	{
		// -- Show window
		ShowWindow(SW_HIDE);
	}
}

void COptionTreeItemCheckBox::OnSelect()
{
	// Do nothing here
}