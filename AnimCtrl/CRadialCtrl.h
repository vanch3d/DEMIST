#ifndef CRadialCtrl_h
#define CRadialCtrl_h 1
///////////////////////////////////////////////////////////////////////////////
//  Filename:	CRadialCtrl.h
//	Author:		David M. Flores
//	Date:		10/07/99
//  Remarks:	Defines the CRadialCtrl class.
///////////////////////////////////////////////////////////////////////////////
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "ComponentStatic.h"

/////////////////////////////////////////////////////////////////////////////
// CRadialCtrl window
class CRadialCtrl : public CComponentStatic
{
///////////////////////////////////////////////////////////////////////////////
//	Constructors and Destructors
///////////////////////////////////////////////////////////////////////////////
public:
	CRadialCtrl();
	virtual ~CRadialCtrl();

///////////////////////////////////////////////////////////////////////////////
//	Operations
///////////////////////////////////////////////////////////////////////////////
public:
	void DisplayAngle(bool bEnable);
	int GetAngle() const;
	void SetAngle(const int nDegrees);
	void SetTolerance(const int nTolerance);
	void SetTransparentColor(COLORREF color);
	void SetHiLightColor(COLORREF color);
	void SetShadowColor(COLORREF color);
	void SetArrow(int* offset = NULL, int* tail_length = NULL, int* head_length = NULL, int* trim = NULL);
	BOOL SetBitmap(UINT nID);
	void Refresh(BOOL bErase = TRUE);
	void SetRange(int nMin, int nMax);
	void SetMinRange(int nMin);
	void SetMaxRange(int nMax);
	void SetPos(int nPos);
	void GetRange(int &nMin, int &nMax);
	int GetMinRange() const;
	int GetMaxRange() const;
	int GetPos() const;
	void VerifyPos();


    virtual void Draw (CDC* pDC,CRect mrect);

protected:
	void DrawBitmap(CDC* dc);
	void DrawAngle(CDC* dc);
	void DrawArrow(CDC* dc);

private:
	CPoint ConvertToPoint(CPoint offset, const double angle, const double radius) const;
	double ConvertToAngle(CPoint point, CPoint center) const;
	const double ArcTan(const double ratio) const;
	const double ArcTan(const double x, const double y) const;

///////////////////////////////////////////////////////////////////////////////
//	Virtual Operations
///////////////////////////////////////////////////////////////////////////////
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRadialCtrl)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void PreSubclassWindow();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

///////////////////////////////////////////////////////////////////////////////
//	Message Maps
///////////////////////////////////////////////////////////////////////////////
	//{{AFX_MSG(CRadialCtrl)
	afx_msg void OnPaint();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSysColorChange();
	afx_msg void OnEnable(BOOL bEnable);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg LRESULT OnPrint(WPARAM wp,LPARAM lp);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()


///////////////////////////////////////////////////////////////////////////////
//	Attributes
///////////////////////////////////////////////////////////////////////////////
private:
	CBitmap m_bmpBackground;
	UINT m_nBitmapID;

	double m_dAngle;
	double m_dTolerance;
	int m_nSmallMove;
	int m_nLargeMove;
	int m_nArrowOffset;
	int m_nArrowHeadLength;
	int m_nArrowTailLength;
	int m_nArrowTrim;

	int m_nMinRange;
	int m_nMaxRange;
	int m_nCurrentPos;

	CRect m_rect;
	CRect m_rectBitmap;
	CRect m_rectText;
	CFont m_font;
	CPoint m_ptCenter;
	bool m_bCapture;
	bool m_bFirstTime;
	bool m_bDrawFocusBox;
	bool m_bDrawText;

	COLORREF	m_colorTransparent,
				m_colorHiLight,
				m_colorShadow;
	COLORREF	m_colorState;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(CRadialCtrl_h)
