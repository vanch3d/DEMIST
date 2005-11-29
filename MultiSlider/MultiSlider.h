#if !defined(AFX_MULTISLIDER_H__AC2B03B4_ABE0_11D3_9121_006008682811__INCLUDED_)
#define AFX_MULTISLIDER_H__AC2B03B4_ABE0_11D3_9121_006008682811__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MultiSlider.h : header file
//

//#ifndef __AFXTEMPL_H__
//	#include "afxtempl.h"
//#endif

#include "Tools\Bubble.h"

/////////////////////////////////////////////////////////////////////////////
// CMultiSlider window
class CMarker : public CObject
{
public:
	CMarker () 
	{
		m_Pos = -1;
		m_nType = 0;
		m_strDesc = m_strHTML = _T("");
		m_mainRect.SetRectEmpty();
		m_rightRect.SetRectEmpty();
		m_leftRect.SetRectEmpty();
	}

	CMarker (const CMarker& src)
	{
		m_Pos = src.m_Pos;
		m_nType = src.m_nType;
		m_strDesc = src.m_strDesc;
		m_strHTML = src.m_strHTML;
		m_mainRect = src.m_mainRect;
		m_rightRect = src.m_rightRect;
		m_leftRect = src.m_leftRect;
	}

	float	m_Pos;
	CString m_strDesc;
	CString m_strHTML;
	int		m_nType;
	CRect m_mainRect;
	CRect m_rightRect;
	CRect m_leftRect;

	const BOOL operator == (const CMarker& other)
	{
		return other.m_Pos == m_Pos && other.m_mainRect == m_mainRect
			&& other.m_rightRect == m_rightRect && other.m_leftRect == m_leftRect;
	}

	const BOOL operator != (const CMarker& other)
	{
		return !(*this == other);
	}

	const CMarker& operator=(const CMarker& src)
	{
		m_Pos = src.m_Pos;
		m_nType = src.m_nType;
		m_strDesc = src.m_strDesc;
		m_strHTML = src.m_strHTML;
		m_mainRect = src.m_mainRect;
		m_rightRect = src.m_rightRect;
		m_leftRect = src.m_leftRect;

		return *this;
	}
};

class CMultiSlider : public CSliderCtrl
{
// Construction
public:
	CMultiSlider();

// Attributes
public:
	CImageList		m_pImageList;

//	CStringArray m_cStrDesc;
//	CUIntArray   m_cIntStyle;
	CUIntArray m_Colors;
	CObList m_Markers;
	CMarker* m_pnewMarker;
	CMarker* m_pdraggedMarker;
	CMarker* m_pSelectedMarker;
	CMarker* m_pTTMarker;
	CRect m_TTRect;
	float prevMin, prevMax;
	BOOL fInteger;
	UINT uiToolTipEventId;
	int iToolTipEventDelay;
	float m_numMin;
	float m_numMax;
	CRect sliderRect, channelRect, clientRect, thumbRect;
	int m_numTicks;
	bool fInit;
	CRect m_TTWindow;
	HCURSOR			m_hCursorDelete;
	HCURSOR			m_hCursorArrow;	// Standard Windows cursor
	CBubble			m_wndInfo;
	bool fFocus;
	BOOL fDisplayColors;
	int m_nAlignment;
	int m_nLabelWidth;
	CFont m_labelFont;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMultiSlider)
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	int m_NumMarkers;
	int GetMarkers(float *value);
	int GetNumMarkers();
	float GetMaxRange();
	float GetMinRange();
	bool GetRanges(float &min, float &max);
	bool SetRanges(float max, float min);
	virtual ~CMultiSlider();

	// Generated message map functions
	int InsertMarker(CMarker *pMarker);
	void SetRectangles(CMarker* pMarker);
protected:
	void GetMinMax(CPoint pt);
	CRect GetMarkerRect(CMarker* pMI);
	void CleanUp();
	bool RemoveLevelMarker(CMarker *pRemoved);
	float RoundTo(float value, short decimal);
	void Initialize();
	void SetInfoWindow(CPoint &point);
	void BuildTooltipText(CMarker* pMI, bool bShowTip, CString &text);
	CMarker* MarkerHitTest(CPoint pt);	//returns index of interval or -1 if not an interval
	//{{AFX_MSG(CMultiSlider)
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MULTISLIDER_H__AC2B03B4_ABE0_11D3_9121_006008682811__INCLUDED_)
