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


// OptionTreeItemStatic.cpp: implementation of the COptionTreeItemStatic class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "OptionTreeItemStatic.h"

// Added Headers
#include "OptionTree.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COptionTreeItemStatic::COptionTreeItemStatic()
{
	// Initialize variables
	m_strStaticText = "";

	// Set item type
	SetItemType(OT_ITEM_STATIC);
}

COptionTreeItemStatic::~COptionTreeItemStatic()
{

}

void COptionTreeItemStatic::DrawAttribute(CDC *pDC, const RECT &rcRect)
{
	// Make sure option is not NULL
	if (m_otOption == NULL)
	{
		return;
	}

	// Declare variables
	HGDIOBJ hOld;
	COLORREF crOld;
	int nOldBack;
	CRect rcText;

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
	pDC->DrawText(m_strStaticText, rcText, DT_SINGLELINE | DT_VCENTER);

	// Restore GDI ojects
	pDC->SelectObject(hOld);
	pDC->SetTextColor(crOld);
	pDC->SetBkMode(nOldBack);
}	

CString COptionTreeItemStatic::GetStaticText()
{
	// Return text
	return m_strStaticText;
}

void COptionTreeItemStatic::SetStaticText(CString strStaticText)
{
	// Set variables
	m_strStaticText = strStaticText;
}

void COptionTreeItemStatic::CleanDestroyWindow()
{
	// Do nothing here
}

void COptionTreeItemStatic::OnCommit()
{
	// Do nothing here
}

void COptionTreeItemStatic::OnRefresh()
{
	// Do nothing here
}

void COptionTreeItemStatic::OnMove()
{
	// Do nothing here
}

void COptionTreeItemStatic::OnActivate()
{
	// Do nothing here
}

void COptionTreeItemStatic::OnDeSelect()
{
	// Do nothing here
}

void COptionTreeItemStatic::OnSelect()
{
	// Do nothing here
}
