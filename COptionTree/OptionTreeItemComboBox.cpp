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


// OptionTreeItemComboBox.cpp : implementation file
//

#include "stdafx.h"
#include "OptionTreeItemComboBox.h"

// Added Headers
#include "OptionTree.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptionTreeItemComboBox

COptionTreeItemComboBox::COptionTreeItemComboBox()
{
	// Initialize variables
	m_bFocus = FALSE;
	m_lDropDownHeight = OT_COMBO_DROPDOWNHEIGHT;
}

COptionTreeItemComboBox::~COptionTreeItemComboBox()
{
}


BEGIN_MESSAGE_MAP(COptionTreeItemComboBox, CComboBox)
	//{{AFX_MSG_MAP(COptionTreeItemComboBox)
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptionTreeItemComboBox message handlers


void COptionTreeItemComboBox::DrawAttribute(CDC *pDC, const RECT &rcRect)
{
	m_nSelect = CComboBox::GetCurSel();
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
	CString strWindowText;

	// Get window text
	GetWindowText(strWindowText);

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
	pDC->DrawText(strWindowText, rcText, DT_SINGLELINE | DT_VCENTER);

	// Restore GDI ojects
	pDC->SelectObject(hOld);
	pDC->SetTextColor(crOld);
	pDC->SetBkMode(nOldBack);

}

int COptionTreeItemComboBox::GetCurSel() const
{
	return m_nSelect;
}


void COptionTreeItemComboBox::OnCommit()
{
	// Hide edit control
	if (IsWindow(GetSafeHwnd()))
	{
		// -- Show window
		ShowWindow(SW_HIDE);
	}
}

void COptionTreeItemComboBox::OnRefresh()
{
	// Set the window text
	if (IsWindow(GetSafeHwnd()))
	{
		SetWindowPos(NULL, m_rcAttribute.left, m_rcAttribute.top, m_rcAttribute.Width(), m_rcAttribute.Height(), SWP_NOZORDER | SWP_NOACTIVATE);
	}
}

void COptionTreeItemComboBox::OnMove()
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

void COptionTreeItemComboBox::OnActivate()
{
	// Make sure window is valid
	if (IsWindow(GetSafeHwnd()))
	{
		// -- Show window
		ShowWindow(SW_SHOW);

		// -- Set window position
		SetWindowPos(NULL, m_rcAttribute.left, m_rcAttribute.top, m_rcAttribute.Width(), m_rcAttribute.Height() + m_lDropDownHeight, SWP_NOZORDER | SWP_SHOWWINDOW);

		// -- Set focus
		SetFocus();
	}
}

void COptionTreeItemComboBox::OnSetFocus(CWnd* pOldWnd) 
{
	// Mark focus
	m_bFocus = TRUE;

	CComboBox::OnSetFocus(pOldWnd);	
}

void COptionTreeItemComboBox::OnKillFocus(CWnd* pNewWnd) 
{
	// Validate
	if (m_otOption == NULL)
	{
		CComboBox::OnKillFocus(pNewWnd);
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

	m_nSelect = CComboBox::GetCurSel();

	CComboBox::OnKillFocus(pNewWnd);	
}

BOOL COptionTreeItemComboBox::CreateComboWindow(DWORD dwAddStyle)
{
	// Declare variables
	DWORD dwStyle = WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_VSCROLL;
	BOOL bRet = FALSE;

	// Make sure options is not NULL
	if (m_otOption == NULL)
	{
		return FALSE;
	}

	// Create edit control
	if (!IsWindow(GetSafeHwnd()))
	{
		// -- Add style
		if (dwAddStyle != 0)
		{
			dwStyle |= dwAddStyle;
		}
		
		// -- Create the combo box
		bRet = Create(dwStyle, m_rcAttribute, m_otOption->GetCtrlParent(), GetCtrlID());

		// -- Setup combo
		if (bRet == TRUE)
		{
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

void COptionTreeItemComboBox::SetDropDownHeight(long lHeight)
{
	// Save variable
	m_lDropDownHeight = lHeight;
}

long COptionTreeItemComboBox::GetDropDownHeight()
{
	// Return variable
	return m_lDropDownHeight;
}

void COptionTreeItemComboBox::CleanDestroyWindow()
{
	// Destroy window
	if (IsWindow(GetSafeHwnd()))
	{
		// -- Destroy window
		DestroyWindow();
	}
}
void COptionTreeItemComboBox::OnDeSelect()
{
	// Hide window
	if (IsWindow(GetSafeHwnd()))
	{
		// -- Show window
		ShowWindow(SW_HIDE);
		m_nSelect = CComboBox::GetCurSel();
	}
}

void COptionTreeItemComboBox::OnSelect()
{
	// Do nothing here
	m_nSelect = CComboBox::GetCurSel();

}
