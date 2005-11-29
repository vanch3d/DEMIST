//----------------------------------------------------------------------------
// N O L D U S   I N F O R M A T I O N   T E C H N O L O G Y   B . V .
//----------------------------------------------------------------------------
// Filename:      BitmapPickerCombo.cpp
// Project:       EthoVision
// Module:        BitmapPicker
// Programmer:    Anneke Sicherer-Roetman
// Version:       1.00
// Revision Date: 06-10-1999
//----------------------------------------------------------------------------
// Description:   Definition of class CBitmapPickerCombo
//                See CBitmapPickerCombo.h
//----------------------------------------------------------------------------
// Acknowledgements: based on Joel Wahlberg's CIconComboBox
//                  (joel.wahlberg@enator.se)
//----------------------------------------------------------------------------
// Revision history:
// 06-10-1999 - First implementation
//----------------------------------------------------------------------------
// Bugs: ........
//----------------------------------------------------------------------------
// @doc
//----------------------------------------------------------------------------

#include "stdafx.h"
#include "BitmapPickerCombo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
                   
//----------------------------------------------------------------------------
// Function DrawBitmap
// @func    draws bitmap at specified point in specified device context
// @rdesc   nothing
// @parm    const CBitmap | *bitmap | bitmap to draw
// @parm    const CDC     | *pDC    | device context to draw in
// @parm    const CPoint  | &point  | top left point of bitmap
//----------------------------------------------------------------------------
// @prog 
// Anneke Sicherer-Roetman
// @revs 
// 06-10-1999 - First implementation
//----------------------------------------------------------------------------
// @todo 
//----------------------------------------------------------------------------
static void DrawBitmap(const CBitmap *bitmap, const CDC *pDC, const CPoint &point)
{
  BITMAP bm; ((CBitmap*)bitmap)->GetBitmap(&bm);
  int w = bm.bmWidth; 
  int h = bm.bmHeight;
  CDC memDC; memDC.CreateCompatibleDC((CDC*)pDC);
  CBitmap *pBmp = memDC.SelectObject((CBitmap*)bitmap);
  ((CDC*)pDC)->BitBlt(point.x, point.y, w, h, &memDC, 0, 0, SRCCOPY);
  memDC.SelectObject(pBmp);
}

//----------------------------------------------------------------------------
// Function DrawBitmap
// @func    draws bitmap centered in specified rectangle in specified device context
// @rdesc   nothing
// @parm    const CBitmap | *bitmap | bitmap to draw
// @parm    const CDC     | *pDC    | device context to draw in
// @parm    const CRect   | &rect   | rectangle to center in
//----------------------------------------------------------------------------
// @prog 
// Anneke Sicherer-Roetman
// @revs 
// 06-10-1999 - First implementation
//----------------------------------------------------------------------------
// @todo 
//----------------------------------------------------------------------------
static void DrawBitmap(const CBitmap *bitmap, const CDC *pDC, const CRect &rect)
{
  BITMAP bm; ((CBitmap*)bitmap)->GetBitmap(&bm);
  int w = bm.bmWidth; 
  int h = bm.bmHeight;
  CPoint point;
  point.x = rect.left + ((rect.right - rect.left) / 2) - (w / 2);
  point.y = rect.top + ((rect.bottom - rect.top) / 2) - (h / 2);
  DrawBitmap(bitmap, pDC, point);
}

//----------------------------------------------------------------------------
// Function CBitmapPickerCombo::CBitmapPickerCombo
// @mfunc   constructor
// @xref    <c CBitmapPickerCombo>
//----------------------------------------------------------------------------
// @prog 
// Anneke Sicherer-Roetman
// @revs 
// 06-10-1999 - First implementation
//----------------------------------------------------------------------------
// @todo 
//----------------------------------------------------------------------------
CBitmapPickerCombo::CBitmapPickerCombo():
  CComboBox(),
    m_nItemWidth(0),
    m_nItemHeight(0)
{
	  m_pImageList = NULL;
}

//----------------------------------------------------------------------------
// Function CBitmapPickerCombo::AddBitmap
// @mfunc   adds bitmap (and string) item to combobox
// @rdesc   index of item (-1 on failure) (int)
// @parm    const CBitmap | *bitmap | bitmap to add
// @parm    const char    | *string | string to add (default NULL)
// @xref    <c CBitmapPickerCombo>
//----------------------------------------------------------------------------
// @prog 
// Anneke Sicherer-Roetman
// @revs 
// 06-10-1999 - First implementation
//----------------------------------------------------------------------------
// @todo 
//----------------------------------------------------------------------------

int CBitmapPickerCombo::AddBitmap(int nBitmapID, const char *string)
{
  return InsertBitmap(GetCount(), nBitmapID, string);
}

//----------------------------------------------------------------------------
// Function CBitmapPickerCombo::InsertBitmap
// @mfunc   adds bitmap (and string) item to combobox at specified index
// @rdesc   index of item (-1 on failure) (int)
// @parm    int           | nIndex  | index at which to insert
// @parm    const CBitmap | *bitmap | bitmap to add
// @parm    const char    | *string | string to add (default NULL)
// @xref    <c CBitmapPickerCombo>
//----------------------------------------------------------------------------
// @prog 
// Anneke Sicherer-Roetman <nl>
// after: Icon Picker Combo Box by Joel Wahlberg <nl>
// http://www.codeguru.com/combobox/icon_combobox.shtml
// @revs 
// 06-10-1999 - First implementation
//----------------------------------------------------------------------------
// @todo 
//----------------------------------------------------------------------------
int CBitmapPickerCombo::InsertBitmap(int nIndex, int nBitmapID, const char *string)
{
	if (!m_pImageList) return -1;
	int n = CComboBox::InsertString(nIndex, string ? string : "");
	if (n != CB_ERR && n != CB_ERRSPACE) {
	    SetItemData(n, (DWORD)nBitmapID);
		IMAGEINFO pImageInfo;
		int nnb = (nBitmapID==-1)? 0 : nBitmapID;
		m_pImageList->GetImageInfo(nnb,&pImageInfo);
		CRect mrct = pImageInfo.rcImage;

	    SetSize(mrct.Width(), mrct.Height());
	}
  return n;
}


//----------------------------------------------------------------------------
// Function CBitmapPickerCombo::MeasureItem
// @mfunc   Called by MFC when combo box is created
// @rdesc   nothing
// @parm    LPMEASUREITEMSTRUCT | lpMIS | standard parameter
// @xref    <c CBitmapPickerCombo>
//----------------------------------------------------------------------------
// @prog 
// Anneke Sicherer-Roetman
// @revs 
// 06-10-1999 - First implementation
//----------------------------------------------------------------------------
// @todo 
//----------------------------------------------------------------------------
void CBitmapPickerCombo::MeasureItem(LPMEASUREITEMSTRUCT lpMIS)
{ 
  lpMIS->itemWidth = (m_nItemWidth /*+ 2*/);
  lpMIS->itemHeight = (m_nItemHeight /*+ 2*/);
}

//----------------------------------------------------------------------------
// Function CBitmapPickerCombo::DrawItem
// @mfunc   Called by MFC when visual aspect of combo box changes 
// @rdesc   nothing
// @parm    LPDRAWITEMSTRUCT | lpDIS | standard parameter
// @xref    <c CBitmapPickerCombo>
//----------------------------------------------------------------------------
// @prog 
// Anneke Sicherer-Roetman <nl>
// after: Icon Picker Combo Box by Joel Wahlberg <nl>
// http://www.codeguru.com/combobox/icon_combobox.shtml
// @revs 
// 06-10-1999 - First implementation
//----------------------------------------------------------------------------
// @todo 
//----------------------------------------------------------------------------
void CBitmapPickerCombo::DrawItem(LPDRAWITEMSTRUCT lpDIS)
{
  CDC* pDC = CDC::FromHandle(lpDIS->hDC);

  if (!IsWindowEnabled()) {
    CBrush brDisabled(RGB(192,192,192)); // light gray
    CBrush* pOldBrush = pDC->SelectObject(&brDisabled);
    CPen penDisabled(PS_SOLID, 1, RGB(192,192,192));
    CPen* pOldPen = pDC->SelectObject(&penDisabled);
    OutputBitmap(lpDIS, false);
    pDC->SelectObject(pOldBrush);
    pDC->SelectObject(pOldPen);
    return;
  }

  // Selected
  if ((lpDIS->itemState & ODS_SELECTED) 
    && (lpDIS->itemAction & (ODA_SELECT | ODA_DRAWENTIRE))) {
    CBrush brHighlight(::GetSysColor(COLOR_HIGHLIGHT)); 
    CBrush* pOldBrush = pDC->SelectObject(&brHighlight);
    CPen penHighlight(PS_SOLID, 1, ::GetSysColor(COLOR_HIGHLIGHT));
    CPen* pOldPen = pDC->SelectObject(&penHighlight);
    pDC->Rectangle(&lpDIS->rcItem);
    pDC->SetBkColor(::GetSysColor(COLOR_HIGHLIGHT));
    pDC->SetTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));
    OutputBitmap(lpDIS, true);
    pDC->SelectObject(pOldBrush);
    pDC->SelectObject(pOldPen);
  }

  // De-Selected
  if (!(lpDIS->itemState & ODS_SELECTED) 
    && (lpDIS->itemAction & (ODA_SELECT | ODA_DRAWENTIRE))) {
    CBrush brWindow(::GetSysColor(COLOR_WINDOW)); 
    CBrush* pOldBrush = pDC->SelectObject(&brWindow);
    CPen penHighlight(PS_SOLID, 1, ::GetSysColor(COLOR_WINDOW));
    CPen* pOldPen = pDC->SelectObject(&penHighlight);
    pDC->Rectangle(&lpDIS->rcItem);
    pDC->SetBkColor(::GetSysColor(COLOR_WINDOW));
    pDC->SetTextColor(::GetSysColor(COLOR_WINDOWTEXT));
    OutputBitmap(lpDIS, false);
    pDC->SelectObject(pOldBrush);
    pDC->SelectObject(pOldPen);
  }

  // Focus
  if (lpDIS->itemAction & ODA_FOCUS) 
    pDC->DrawFocusRect(&lpDIS->rcItem);
}

//----------------------------------------------------------------------------
// Function CBitmapPickerCombo::OutputBitmap
// @mfunc   draws bitmap (and string) in item
// @rdesc   nothing
// @parm    LPDRAWITEMSTRUCT | lpDIS    | item data
// @parm    bool             | selected | is the item selected?
// @xref    <c CBitmapPickerCombo>
//----------------------------------------------------------------------------
// @prog 
// Anneke Sicherer-Roetman <nl>
// after: Icon Picker Combo Box by Joel Wahlberg <nl>
// http://www.codeguru.com/combobox/icon_combobox.shtml
// @revs 
// 06-10-1999 - First implementation
//----------------------------------------------------------------------------
// @todo 
//----------------------------------------------------------------------------
void CBitmapPickerCombo::OutputBitmap(LPDRAWITEMSTRUCT lpDIS, bool selected)
{
	if (m_pImageList)
	{
		int nb = (int)lpDIS->itemData;
		CDC* pDC = CDC::FromHandle(lpDIS->hDC);
    CString string; 
    if (lpDIS->itemID != -1) 
      GetLBText(lpDIS->itemID, string); 
    if (string.IsEmpty()) 
	{
	      CPoint point;
		  point.x = lpDIS->rcItem.left + 2;
		point.y = lpDIS->rcItem.top + ((lpDIS->rcItem.bottom - lpDIS->rcItem.top) / 2) - (m_nItemHeight / 2); 
		if (nb!=-1)
			m_pImageList->DrawIndirect(pDC,nb,point,CSize(0,0),CPoint(0,0),ILD_TRANSPARENT);

	}
	else
	{
      CPoint point;
      point.x = lpDIS->rcItem.left + 2;
      point.y = -2+lpDIS->rcItem.top + ((lpDIS->rcItem.bottom - lpDIS->rcItem.top) / 2) - (m_nItemHeight / 2); 
		if (nb!=-1)
		m_pImageList->DrawIndirect(pDC,nb,point,CSize(0,0),CPoint(0,0),ILD_TRANSPARENT);
      CRect rcText(lpDIS->rcItem); 
      rcText.DeflateRect(m_nItemWidth + 7, 0, 0, 0);
      pDC->DrawText(string, rcText, DT_SINGLELINE |DT_VCENTER ); 
	}

		return;
	}

  const CBitmap *bitmap = (const CBitmap*)(lpDIS->itemData);
  if (bitmap && bitmap != (const CBitmap *)(0xffffffff)) {
    CDC* pDC = CDC::FromHandle(lpDIS->hDC);
    CString string; 
    if (lpDIS->itemID != -1) 
      GetLBText(lpDIS->itemID, string); 
    if (string.IsEmpty()) 
      DrawBitmap(bitmap, pDC, lpDIS->rcItem);
    else {
      CPoint point;
      point.x = lpDIS->rcItem.left + 2;
      point.y = lpDIS->rcItem.top + ((lpDIS->rcItem.bottom - lpDIS->rcItem.top) / 2) - (m_nItemHeight / 2); 
      DrawBitmap(bitmap, pDC, point);
      CRect rcText(lpDIS->rcItem); 
      rcText.DeflateRect(m_nItemWidth + 4, 0, 0, 0);
      pDC->DrawText(string, rcText, DT_SINGLELINE |DT_VCENTER ); 
    }
  }
}               

//----------------------------------------------------------------------------
// Function CBitmapPickerCombo::SetSize
// @mfunc   sets overall item size
// @rdesc   nothing
// @parm    int | width  | width of item
// @parm    int | height | height of item
// @xref    <c CBitmapPickerCombo>
//----------------------------------------------------------------------------
// @prog 
// Anneke Sicherer-Roetman
// @revs 
// 06-10-1999 - First implementation
//----------------------------------------------------------------------------
// @todo 
//----------------------------------------------------------------------------
void CBitmapPickerCombo::SetSize(int width, int height)
{
  if (width > m_nItemWidth)
    m_nItemWidth = width-1;
  if (height > m_nItemHeight)
    m_nItemHeight = height-3;
  for (int i = -1; i < GetCount(); i++) 
    SetItemHeight(i, m_nItemHeight + 6);
}

//----------------------------------------------------------------------------

#ifdef _DEBUG
void CBitmapPickerCombo::PreSubclassWindow() 
{
  CComboBox::PreSubclassWindow();

  // ensure some styles are set
  // modifying style here has NO effect!?!
  ASSERT(GetStyle() & CBS_DROPDOWNLIST);
  ASSERT(GetStyle() & CBS_OWNERDRAWVARIABLE);
  ASSERT(GetStyle() & CBS_HASSTRINGS);
}
#endif

//----------------------------------------------------------------------------
