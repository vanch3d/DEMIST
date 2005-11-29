/*
Module : ICONCOMBOBOX.CPP
Purpose: Defines the implementation for 2 MFC classes
         to implement an icon selection combo box
Created: PJN / 23-08-1998
History: PJN / 12-09-1998 1) Removed some files from the workspace which were not being used.
                          2) Updated workspace to use folders.
                          3) Added AddString, InsertString & DeleteString functions. These 
                          functions ASSERT to ensure that the icon combo box operates correctly
                          4) Added DeleteIcon function. This should be called instead of DeleteString
                          5) Modfied internal implementation of InsertIcon & AddIcon for both classes

Copyright (c) 1998 by PJ Naughter.  
All rights reserved.

*/


/////////////////////////////////  Includes  //////////////////////////////////
#include "stdafx.h"
#include "IconComboBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


////////////////////////////// Locals /////////////////////////////////////////

struct _IconComboData
{
  HICON m_hIcon;
  UINT  m_nIconID;
};


////////////////////////////// Implementation /////////////////////////////////

void DDX_CBIcon(CDataExchange* pDX, int nIDC, UINT& nIconID)
{
	HWND hWndCtrl = pDX->PrepareCtrl(nIDC);
	CSmallIconComboBox* pCombo = (CSmallIconComboBox*) CWnd::FromHandle(hWndCtrl);
	ASSERT(pCombo);
	ASSERT(pCombo->IsKindOf(RUNTIME_CLASS(CSmallIconComboBox)));

	if (pDX->m_bSaveAndValidate)
    nIconID = pCombo->GetCurSelIcon();
	else
	  pCombo->SetCurSelIcon(nIconID);
}






IMPLEMENT_DYNAMIC(CSmallIconComboBox, CComboBox)

BEGIN_MESSAGE_MAP(CSmallIconComboBox, CComboBox)
	//{{AFX_MSG_MAP(CSmallIconComboBox)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CSmallIconComboBox::PreSubclassWindow() 
{
  //Let the parent do its thing
	CComboBox::PreSubclassWindow();

  //combo box must manage the strings
  ASSERT(GetWindowLong(m_hWnd, GWL_STYLE) & CBS_HASSTRINGS);

  //combo box must be owner draw variable
  ASSERT(GetWindowLong(m_hWnd, GWL_STYLE) & CBS_OWNERDRAWVARIABLE);

  //Set the Height of the combo box to just contain one small icon
  ::SendMessage(m_hWnd, CB_SETITEMHEIGHT, (WPARAM)-1, 18L);
}

int CSmallIconComboBox::AddIcon(UINT nIconID)
{
  //Load up the icon
  HICON hIcon = (HICON) LoadImage(AfxGetResourceHandle(), MAKEINTRESOURCE(nIconID), IMAGE_ICON, 16, 16, 0);
  ASSERT(hIcon != NULL);
  if (hIcon == NULL)
    return CB_ERR;

  //Add it to the combo box
  int nOffset = CComboBox::AddString(_T(""));

	//Hive away the info in the item data field
	_IconComboData* pData = new _IconComboData;
	pData->m_hIcon = hIcon;
	pData->m_nIconID = nIconID;
  SetItemData(nOffset, (DWORD) pData);

  //Return the offset at which it was added
	return nOffset;
}

int CSmallIconComboBox::AddImage(UINT nIconID)
{
  //Load up the icon
  HICON hIcon = (HICON) LoadImage(AfxGetResourceHandle(), MAKEINTRESOURCE(nIconID), IMAGE_BITMAP, 16, 16, 0);
  ASSERT(hIcon != NULL);
  if (hIcon == NULL)
    return CB_ERR;

  //Add it to the combo box
  int nOffset = CComboBox::AddString(_T(""));

	//Hive away the info in the item data field
	_IconComboData* pData = new _IconComboData;
	pData->m_hIcon = hIcon;
	pData->m_nIconID = nIconID;
  SetItemData(nOffset, (DWORD) pData);

  //Return the offset at which it was added
	return nOffset;
}

int CSmallIconComboBox::InsertIcon(int nIndex, UINT nIconID)
{
  //Load up the icon
  HICON hIcon = (HICON) LoadImage(AfxGetResourceHandle(), MAKEINTRESOURCE(nIconID), IMAGE_ICON, 16, 16, 0);
  ASSERT(hIcon != NULL);
  if (hIcon == NULL)
    return CB_ERR;

  //Add it to the combo box at the specified offset
  int nOffset = CComboBox::InsertString(nIndex, _T("DFDDDFDFD"));

	//Hive away the info in the item data field
	_IconComboData* pData = new _IconComboData;
	pData->m_hIcon = hIcon;
	pData->m_nIconID = nIconID;
  SetItemData(nOffset, (DWORD) pData);

  //Return the offset at which it was added
	return nOffset;
}

int CSmallIconComboBox::DeleteIcon(int nIndex)
{
  //just let the parent do its thing
  return CComboBox::DeleteString(nIndex);
}

UINT CSmallIconComboBox::GetCurSelIcon() const
{
  int nSel = GetCurSel();
	if (nSel == CB_ERR)
	  return 0;

	_IconComboData* pData = (_IconComboData*) GetItemData(nSel);
  return pData->m_nIconID;
}

int CSmallIconComboBox::SetCurSelIcon(UINT nIconID)
{
	//iterate through all the CB items and set the one which 
	//has the corresponding item data
	for (int i=0; i<GetCount(); i++)
	{
		_IconComboData* pData = (_IconComboData*) GetItemData(i);
		if (pData->m_nIconID == nIconID)
		  return SetCurSel(i);
	}
	TRACE(_T("Warning: Could not find matching icon in the icon combo box\n"));
	return CB_ERR;
}

void CSmallIconComboBox::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
  lpMeasureItemStruct->itemHeight = 18;
}

void CSmallIconComboBox::DrawItem(LPDRAWITEMSTRUCT lpDIS) 
{
	ASSERT(lpDIS->CtlType == ODT_COMBOBOX);	

  //Create A CDC from the SDK struct passed in
	CDC* pDC = CDC::FromHandle(lpDIS->hDC);

  //If the item is selected, draw the selection background
	if ((lpDIS->itemState & ODS_SELECTED) &&                         
		(lpDIS->itemAction & (ODA_SELECT | ODA_DRAWENTIRE)))
	{
		// item has been selected - draw selection rectangle
		COLORREF crHilite = GetSysColor(COLOR_HIGHLIGHT);
		CBrush br(crHilite);
		pDC->FillRect(&lpDIS->rcItem, &br);
	}

  //If the item is not selected, draw a white background
	if (!(lpDIS->itemState & ODS_SELECTED) &&
		(lpDIS->itemAction & ODA_SELECT))
	{
		// Item has been de-selected -- remove selection rectangle
		CBrush br(RGB(255, 255, 255));
		pDC->FillRect(&lpDIS->rcItem, &br);
	}

  //Draw the icon
	_IconComboData* pData = (_IconComboData*) lpDIS->itemData;
	ASSERT(pData != NULL);
	if (pData && lpDIS->itemData != 0xFFFFFFFF)
    DrawIconEx(pDC->GetSafeHdc(), lpDIS->rcItem.left+1, lpDIS->rcItem.top+1, pData->m_hIcon, 16, 16, 0, NULL, DI_NORMAL);
}

void CSmallIconComboBox::DeleteItem(LPDELETEITEMSTRUCT lpDeleteItemStruct) 
{
  //Free up any existing HICON's used for drawing
	_IconComboData* pData = (_IconComboData*) GetItemData(lpDeleteItemStruct->itemID);
	ASSERT(pData);
	DestroyIcon(pData->m_hIcon);

	//Free up our internal structure which is stored in the item data
	delete pData;
	
	//Let the parent do its thing
	CComboBox::DeleteItem(lpDeleteItemStruct);
}

int CSmallIconComboBox::AddString(LPCTSTR /*lpszString*/)
{
	ASSERT(FALSE); //Use AddIcon not AddString
	return CB_ERR;
}

int CSmallIconComboBox::InsertString(int /*nIndex*/, LPCTSTR /*lpszString*/)
{
	ASSERT(FALSE); //Use InsertIcon not InsertString
	return CB_ERR;
}

int CSmallIconComboBox::DeleteString(int /*nIndex*/)
{
	ASSERT(FALSE); //Use DeleteIcon not DeleteString
	return CB_ERR;
}














IMPLEMENT_DYNAMIC(CLargeIconComboBox, CSmallIconComboBox)

BEGIN_MESSAGE_MAP(CLargeIconComboBox, CSmallIconComboBox)
	//{{AFX_MSG_MAP(CLargeIconComboBox)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CLargeIconComboBox::PreSubclassWindow() 
{
  //Let the parent do its thing
	CSmallIconComboBox::PreSubclassWindow();

  //Set the Height of the combo box to just contain one large icon
  ::SendMessage(m_hWnd, CB_SETITEMHEIGHT, (WPARAM)-1, 34L);
}

int CLargeIconComboBox::AddIcon(UINT nIconID)
{
  //Load up the icon
  HICON hIcon = (HICON) LoadImage(AfxGetResourceHandle(), MAKEINTRESOURCE(nIconID), IMAGE_ICON, 32, 32, 0);
  ASSERT(hIcon != NULL);
  if (hIcon == NULL)
    return CB_ERR;

  //Add it to the combo box
  int nOffset = CComboBox::AddString(_T(""));

	//Hive away the info in the item data field
	_IconComboData* pData = new _IconComboData;
	pData->m_hIcon = hIcon;
	pData->m_nIconID = nIconID;
  SetItemData(nOffset, (DWORD) pData);

  //Return the offset at which it was added
	return nOffset;
}

int CLargeIconComboBox::InsertIcon(int nIndex, UINT nIconID)
{
  //Load up the icon
  HICON hIcon = (HICON) LoadImage(AfxGetResourceHandle(), MAKEINTRESOURCE(nIconID), IMAGE_ICON, 32, 32, 0);
  ASSERT(hIcon != NULL);
  if (hIcon == NULL)
    return CB_ERR;

  //Add it to the combo box at the specified offset
  int nOffset = CComboBox::InsertString(nIndex, _T(""));

	//Hive away the info in the item data field
	_IconComboData* pData = new _IconComboData;
	pData->m_hIcon = hIcon;
	pData->m_nIconID = nIconID;
  SetItemData(nOffset, (DWORD) pData);

  //Return the offset at which it was added
	return nOffset;
}


void CLargeIconComboBox::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
  lpMeasureItemStruct->itemHeight = 34;
}

void CLargeIconComboBox::DrawItem(LPDRAWITEMSTRUCT lpDIS) 
{
	ASSERT(lpDIS->CtlType == ODT_COMBOBOX);	

  //Create A CDC from the SDK struct passed in
	CDC* pDC = CDC::FromHandle(lpDIS->hDC);

  //If the item is selected, draw the selection background
	if ((lpDIS->itemState & ODS_SELECTED) &&                         
		(lpDIS->itemAction & (ODA_SELECT | ODA_DRAWENTIRE)))
	{
		// item has been selected - draw selection rectangle
		COLORREF crHilite = GetSysColor(COLOR_HIGHLIGHT);
		CBrush br(crHilite);
		pDC->FillRect(&lpDIS->rcItem, &br);
	}

  //If the item is not selected, draw a white background
	if (!(lpDIS->itemState & ODS_SELECTED) &&
		(lpDIS->itemAction & ODA_SELECT))
	{
		// Item has been de-selected -- remove selection rectangle
		CBrush br(RGB(255, 255, 255));
		pDC->FillRect(&lpDIS->rcItem, &br);
	}

  //Draw the icon
	_IconComboData* pData = (_IconComboData*) lpDIS->itemData;
	ASSERT(pData != NULL);
	if (pData && lpDIS->itemData != 0xFFFFFFFF)
    DrawIconEx(pDC->GetSafeHdc(), lpDIS->rcItem.left+1, lpDIS->rcItem.top+1, pData->m_hIcon, 32, 32, 0, NULL, DI_NORMAL);
}

