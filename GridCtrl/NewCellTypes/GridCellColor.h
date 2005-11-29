// GridCellColor.h: interface for the CGridCellColor class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRIDCELLCOLOR_H__E7F672E5_831F_11D4_A2FA_00D0B71C8709__INCLUDED_)
#define AFX_GRIDCELLCOLOR_H__E7F672E5_831F_11D4_A2FA_00D0B71C8709__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\GridCell.h"

class CGridCellColor : public CGridCell  
{
	friend class CGridCtrl;
    DECLARE_DYNCREATE(CGridCellColor)
public:
	CGridCellColor();
	virtual ~CGridCellColor();
// editing cells
public:
    virtual BOOL Edit(int nRow, int nCol, CRect rect, CPoint point, UINT nID, UINT nChar);
    virtual void EndEdit();

// CGridCellCombo specific calls
public:
 
protected:
    virtual BOOL Draw(CDC* pDC, int nRow, int nCol, CRect rect, BOOL bEraseBkgnd = TRUE);
};

#endif // !defined(AFX_GRIDCELLCOLOR_H__E7F672E5_831F_11D4_A2FA_00D0B71C8709__INCLUDED_)
