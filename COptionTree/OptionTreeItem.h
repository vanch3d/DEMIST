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

// OptionTreeItem.h: interface for the COptionTreeItem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OPTIONTREEITEM_H__503FD01F_4C4F_4E64_96D3_C400FBC94D26__INCLUDED_)
#define AFX_OPTIONTREEITEM_H__503FD01F_4C4F_4E64_96D3_C400FBC94D26__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// Added Headers
#include "OptionTreeDef.h"

// Classes
class COptionTree;

class COptionTreeItem  
{
public:
	int GetItemType();
	void SetItemType(int nType);
	void SetLabelRect(CRect rcLabel);
	CRect GetLabelRect();
	virtual void DrawAttribute(CDC *pDC, const RECT &rcRect);
	virtual void OnActivate();
	virtual void OnMove();
	virtual void OnRefresh();
	virtual void OnCommit();
	virtual void CleanDestroyWindow();
	virtual void OnDeSelect();
	virtual void OnSelect();


	void SetItemHeight(long lHeight);
	void CommitChanges();
	void Activate();
	long GetHeight();
	long DrawItem(CDC* pDC, const RECT &rcRect, long x, long y);
	void SetOptionsOwner(COptionTree* otOption);
	long GetTotalHeight();
	BOOL IsRootLevel();
	BOOL HitExpand(const POINT& pt);
	void ReadOnly(BOOL bReadOnly = TRUE);
	void Expand(BOOL bExpand = TRUE);
	void Select(BOOL bSelect = TRUE);
	BOOL IsActivated();
	BOOL IsReadOnly();
	BOOL IsSelected();
	BOOL IsExpanded();
	void SetNextVisible(COptionTreeItem *otiNextVisible);
	COptionTreeItem * GetNextVisible();
	void SetChild(COptionTreeItem *otiChild);
	COptionTreeItem * GetChild();
	void SetSibling(COptionTreeItem *otiSibling);
	COptionTreeItem * GetSibling();
	void SetParent(COptionTreeItem *otiParent);
	COptionTreeItem * GetParent();
	UINT GetCtrlID();
	void SetCtrlID(UINT nID);
	CString GetLabelText();
	void SetLabelText(CString strLabel);
	CString GetInfoText();
	void SetInfoText(CString strText);
	const POINT& GetLocation();
	COptionTreeItem();
	virtual ~COptionTreeItem();

protected:
	void _DrawExpand(HDC hdc, long lX, long lY, BOOL bExpand, BOOL bFill);
	BOOL IsStringEmpty(CString strString);

protected:
	CString m_strLabel;
	CString m_strInfoText;
	UINT m_uControlID;
	COptionTreeItem *m_otiParent;
	COptionTreeItem *m_otiSibling;
	COptionTreeItem *m_otiChild;
	COptionTreeItem *m_otiNextVisible;
	CRect m_rcExpand;
	COptionTree *m_otOption;
	CPoint m_ptLocation;
	CRect m_rcAttribute;
	BOOL m_bCommitOnce;
	LPARAM m_lParam;
	long m_lItemHeight;
	CRect m_rcLabelRect;
	BOOL m_bSelected;
	BOOL m_bExpanded;
	BOOL m_bActivated;
	BOOL m_bReadOnly;
	int m_nItemType;

};

#endif // !defined(AFX_OPTIONTREEITEM_H__503FD01F_4C4F_4E64_96D3_C400FBC94D26__INCLUDED_)
