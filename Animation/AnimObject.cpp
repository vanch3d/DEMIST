// AnimObject.cpp: implementation of the CAnimObject class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <wingdi.h>
#include <math.h>

#include "simul.h"

//#include "MRTracker.h"
#include "SimulDoc.h"
#include "AnimObject.h"
#include <MInstruction\LearningUnit.h>
#include <MSimulation\Model.h>

#include <Prefs\Pref.h>

#include <MInteraction\SimulAnimat.h>
#include <MInteraction\Format.h>

#include "Tools\HelpPrSheet.h"


#include "AnimObjectPage.h"
#include "AnimObjSizePage.h"
#include "AnimShapePage.h"
#include "AnimDensityPage.h"
#include "AnimInterPage.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// CEditValueDlg
//////////////////////////////////////////////////////////////////////

CEditValueDlg::CEditValueDlg(CWnd* pParent /*=NULL*/,CPoint pt)
	: CDialog(CEditValueDlg::IDD, pParent)
{
	m_pt = pt;
	m_pWndParent = pParent;
	//{{AFX_DATA_INIT(CEditValueDlg)
	m_strValue = _T("");
	m_strName = _T("");
	m_strOldV = _T("");
	//}}AFX_DATA_INIT
	m_dMin = m_dMax = 0.0;
}

void CEditValueDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEditValueDlg)
	DDX_Text(pDX, IDC_VALUE, m_strValue);
	DDX_Text(pDX, IDC_OLDVALUE, m_strOldV);
	DDX_Text(pDX, IDC_NAME, m_strName);
	//}}AFX_DATA_MAP
	if (pDX->m_bSaveAndValidate && (m_dMin != m_dMax) && (m_dMin != 0.0))
	{
		double nVal = atof(m_strValue.GetBuffer(m_strValue.GetLength()));
		if (nVal<m_dMin|| nVal>m_dMax)
		{
			int precision = 15;
			UINT nIDPrompt = AFX_IDP_PARSE_REAL_RANGE;
			TCHAR szMin[32], szMax[32];
			CString prompt;

			_stprintf(szMin, _T("%.*g"), precision, m_dMin);
			_stprintf(szMax, _T("%.*g"), precision, m_dMax);
			AfxFormatString2(prompt, nIDPrompt, szMin, szMax);

			AfxMessageBox(prompt, MB_ICONEXCLAMATION, nIDPrompt);
			pDX->Fail();            // throws exception
		}

	}
}

BEGIN_MESSAGE_MAP(CEditValueDlg, CDialog)
	//{{AFX_MSG_MAP(CEditValueDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CEditValueDlg::OnInitDialog() 
{
	BOOL bRes = CDialog::OnInitDialog();
	if (m_pt != CPoint(0,0))
	{
		CRect mrect,mrect2;
		CWnd *pWnd = GetParentFrame();
		m_pWndParent->GetWindowRect(mrect);
		GetWindowRect(mrect2);
		SetWindowPos(NULL,m_pt.x+mrect.left-(mrect2.right-mrect2.left)/2,m_pt.y+mrect.top+15,0,0,SWP_NOZORDER|SWP_NOSIZE);
	}

	return bRes;
}
//////////////////////////////////////////////////////////////////////
// CAnimObject
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CAnimObject, CObject, VERSIONABLE_SCHEMA | 2)

CAnimObject::CAnimObject()
{
//	m_pView=0;
	m_pDoc = 0;
	m_pPosPage = NULL;
	m_pSizePage = NULL;
	m_pShapPage = NULL;
	m_pIntPage = NULL;
	m_currExpSet = 0;
	m_currTime = 0;

	m_bResize = true;
	m_bIsSelected = FALSE;
	m_bShowTrace = FALSE;
	m_bShowAxes = FALSE;
	m_nDDPos.m_ptDefPos = CPoint(100,100);
	m_nDDsize.m_ptDefSize = CSize(50,50);//mSW;//m_ObjRect.Size();
	m_ObjRect = CRect(100,100,150,150);
	SetRect(m_ObjRect);
	//m_ObjRect = CRect(100,100,150,150);
	//m_ObjPosition = m_ObjRect.TopLeft();
	m_ObjSize = m_ObjRect.Size();

	//m_pPen.CreatePen(PS_INSIDEFRAME,1,RGB(0,0,0));
	m_pLogPen.lopnColor = RGB(0,0,0);
	m_pLogPen.lopnStyle = PS_INSIDEFRAME;
	m_pLogPen.lopnWidth.x = 1;
	//m_pBrush.CreateSolidBrush(CLR_DEFAULT);
	m_pLogBrush.lbStyle = BS_SOLID;
	m_pLogBrush.lbColor = CLR_DEFAULT;
	m_pLogBrush.lbHatch = 0;


	m_nDDsize.m_nParamW = -1;
	m_nDDsize.m_nParamH = -1;
	m_nDDsize.m_nFactW = 0.0;
	m_nDDsize.m_nFactH = 0.0;
	m_nDDPos.m_nParamX = -1;
	m_nDDPos.m_nParamY = -1;
	m_nDDPos.m_nFactX = 0.0;
	m_nDDPos.m_nFactY = 0.0;
//	m_xFact = m_yFact = 1.0;
//	m_PosParam = m_SizParam = -1;
	m_ptXLim = m_ptYLim = CPoint(-1,-1);

	m_nShape = CSHAPE_RECT;
	m_nDepth = -1;
	m_nRelatedID = -1;
	m_pRelatedObj = NULL;
//	m_cObjSet = NULL;
	m_pSector = NULL;

//	SetObjectDef(_T(""));
	m_ObjectDef = _T("");
}


CAnimObject::CAnimObject(int ID,const CRect &position)
{
	m_pDoc = NULL;
	m_currExpSet = 0;
	m_currTime = 0;
	m_pPosPage = NULL;
	m_pSizePage = NULL;
	m_pShapPage = NULL;
	m_pIntPage = NULL;
	m_ObjectID = ID;
//	m_ObjectDef.Format(_T("Object n.%d"),m_ObjectID);
//	SetObjectDef(_T(""));
	m_ObjectDef = _T("");
	m_bResize = true;
	m_bIsSelected = FALSE;
	m_bShowTrace = FALSE;
	m_bShowAxes = FALSE;
	m_nDDPos.m_ptDefPos = position.TopLeft();
	m_nDDsize.m_ptDefSize = position.Size();//CSize(50,50);//mSW;//m_ObjRect.Size();
	m_ObjRect = position;
	SetRect(m_ObjRect);
	//m_ObjRect = position;
	//m_ObjPosition = m_ObjRect.TopLeft();
	m_ObjSize = m_ObjRect.Size();
	
	//m_pPen.CreatePen(PS_INSIDEFRAME,1,RGB(0,0,0));
	m_pLogPen.lopnColor = RGB(0,0,0);
	m_pLogPen.lopnStyle = PS_INSIDEFRAME;
	m_pLogPen.lopnWidth.x = 1;
	//m_pBrush.CreateSolidBrush(CLR_DEFAULT);
	//m_pBrush.CreateSolidBrush(CLR_DEFAULT);
	m_pLogBrush.lbStyle = BS_SOLID;
	m_pLogBrush.lbColor = CLR_DEFAULT;
	m_pLogBrush.lbHatch = 0;

	m_nDDsize.m_nParamW = -1;
	m_nDDsize.m_nParamH = -1;
	m_nDDsize.m_nFactW = 0.0;
	m_nDDsize.m_nFactH = 0.0;
	m_nDDPos.m_nParamX = -1;
	m_nDDPos.m_nParamY = -1;
	m_nDDPos.m_nFactX = 0.0;
	m_nDDPos.m_nFactY = 0.0;
//	m_xFact = m_yFact = 1.0;
//	m_PosParam = m_SizParam = -1;
	m_ptXLim = m_ptYLim = CPoint(-1,-1);
	m_nDepth = -1;
	m_nRelatedID = -1;
	m_pRelatedObj = NULL;
//	m_cObjSet = NULL;
	m_pSector = NULL;

	m_nShape = CSHAPE_RECT;
}

CAnimObject::CAnimObject(CAnimObject* pObj)
{
	m_bResize = TRUE;
	m_ObjectID = pObj->m_ObjectID;
	m_ObjPosition = pObj->m_ObjPosition;
	m_ObjectDef = pObj->m_ObjectDef;
	m_pDoc = pObj->m_pDoc ;
	m_currExpSet = pObj->m_currExpSet ;
	m_currTime = pObj->m_currTime ;
//	SetObjectDef(_T(""));
	m_bIsSelected = FALSE;
	m_bShowTrace = pObj->m_bShowTrace;
	m_bShowAxes = pObj->m_bShowAxes;
	m_nDDPos = pObj->m_nDDPos;
	m_nDDsize = pObj->m_nDDsize;
	m_ObjRect = pObj->m_ObjRect;
	m_ObjSize = pObj->m_ObjSize;
	m_ObjectDef = _T("");
	//LOGBRUSH plogBrush;
	//LOGPEN	 plogPen;
	//pObj->m_pPen.GetLogPen(&plogPen);
	m_pPen.DeleteObject();
	//m_pPen.CreatePenIndirect(&plogPen);

	m_pBrush.DeleteObject();
	m_pLogBrush = pObj->m_pLogBrush;
	m_pLogPen = pObj->m_pLogPen;
	//pObj->m_pBrush.GetLogBrush(&plogBrush);
	//m_pBrush.Detach();
	//m_pBrush.CreateBrushIndirect(&plogBrush);

	m_nShape = pObj->m_nShape;
	m_ptXLim = pObj->m_ptXLim;
	m_ptYLim = pObj->m_ptYLim ;
	m_nDepth = pObj->m_nDepth;
	m_nRelatedID = pObj->m_nRelatedID;
	m_pRelatedObj = NULL;//m_pRelatedObj ;
//	m_cObjSet = NULL;
	m_pSector = NULL;//pObj->m_pSector;

	m_pPosPage = NULL;
	m_pSizePage = NULL;
	m_pShapPage = NULL;
	m_pIntPage = NULL;
}

//////////////////////////////////////////////////////////////////////
/// Set the background and line color of the component, as well as the width of its border.
/// \param clrLin	The color (in RGB values) of the border line of the component.
/// \param nSize	The size (in pixels) of the border line of the component.
/// \param clrBru	The color (in RGB values) of the content of the component.
//////////////////////////////////////////////////////////////////////
void CAnimObject::SetObjectColor(COLORREF clrLin,int nSize,COLORREF clrBru)
{
	//LOGBRUSH pLogB;
	//m_pBrush.GetLogBrush(&pLogB);
	//LOGPEN pLogP;
	//m_pPen.GetLogPen(&pLogP);
	
	//pLogB.lbColor = clrBru;
	m_pLogBrush.lbColor = clrBru;
	m_pLogPen.lopnColor = clrLin;
	m_pLogPen.lopnWidth.x = nSize;

	m_pBrush.DeleteObject();
	//m_pBrush.CreateBrushIndirect(&pLogB);
	m_pPen.DeleteObject();
	//m_pPen.CreatePenIndirect(&pLogP);
}


CAnimObject* CAnimObject::Clone()
{
	CAnimObject *pObj = new CAnimObject(this);
	return pObj;
}


CAnimObject::~CAnimObject()
{
	DisposeProperty();
	for (int i=0;i<m_cConnectors.GetSize();i++)
	{
		CAnimObject *pObj = m_cConnectors.GetAt(i);
		CLinkObject *pLink = DYNAMIC_DOWNCAST(CLinkObject, pObj);
		if (pLink)
		{
			pLink->UnlinkObject(this);
		}
	}
	m_cConnectors.RemoveAll();
}

CString	CAnimObject::GetObjectDef()
{
	return m_ObjectDef;
}

//////////////////////////////////////////////////////////////////////
/// Return the icon associated with the component. 
/// \return The zero-based index of the icon used to represent this component, 
///			-1 if no icon is associated with the component. 
///
/// The zero-based index returned by this function refers to the position of the relevant
/// icon stored in the bitmap (bitmap resource IDB_ANIM_COMPONENT). See the #TSCptIcon
/// enum type for the relevant indexes. This function is overriden for every component.
//////////////////////////////////////////////////////////////////////
int	 CAnimObject::GetObjectIcon()
{
	return CICON_SHAPE;
}

//////////////////////////////////////////////////////////////////////
/// Return a default textual description of the component. 
/// \return A string containing the nature of the component and its ID. 
//////////////////////////////////////////////////////////////////////
CString CAnimObject::SetObjectType()
{
	CString mstr;
	mstr.Format(_T("Shape n.%d"),m_ObjectID);
	return mstr;
}

//////////////////////////////////////////////////////////////////////
/// Set the name (description) of the component. 
/// \param mstr	A string containing the new name of the component or, if empty, 
///				its default name (see #SetObjectType()).
//////////////////////////////////////////////////////////////////////
void CAnimObject::SetObjectDef(CString mstr)
{
	if (mstr.GetLength() == 0)
	{
		m_ObjectDef = SetObjectType();// .Format(_T("Shape n.%d"),m_ObjectID);
	}
	else
		m_ObjectDef = mstr;
}

//////////////////////////////////////////////////////////////////////
/// Set the name (description) of the component. 
/// \param nID	A reference to the current ID of the component
/// 
/// This function is used recursively on all the component of the animation ER to update their ID, 
/// i.e. the zero-based index of the component in the list (see CFormatAnimation::m_cObjSet). 
//////////////////////////////////////////////////////////////////////
void CAnimObject::UpdateID(int &nID)
{
	m_ObjectID = nID++;
	//SetObjectDef(_T(""));
}

//////////////////////////////////////////////////////////////////////
/// Assign the current Experimental Set to the component. 
/// \param nExpSet	The zero-based index of the Experimental Set.
/// 
///
//////////////////////////////////////////////////////////////////////
void CAnimObject::SetExpSet(int nExpSet)
{
	m_currExpSet = nExpSet;
}

//////////////////////////////////////////////////////////////////////
/// Assign the current Time to the component. 
/// \param nCurrTime	The zero-based time-step to be assigned.
/// 
///
//////////////////////////////////////////////////////////////////////
void CAnimObject::SetCurrTime(int nCurrTime)
{
	m_currTime = nCurrTime;
}


/*
int CAnimObject::GetHandleCount()
{
	ASSERT_VALID(this);
	return 8;
}

HCURSOR CAnimObject::GetHandleCursor(int nHandle)
{
	ASSERT_VALID(this);

	LPCTSTR id;
	switch (nHandle)
	{
	default:
		ASSERT(FALSE);

	case 1:
	case 5:
		id = IDC_SIZENWSE;
		break;

	case 2:
	case 6:
		id = IDC_SIZENS;
		break;

	case 3:
	case 7:
		id = IDC_SIZENESW;
		break;

	case 4:
	case 8:
		id = IDC_SIZEWE;
		break;
	}

	return AfxGetApp()->LoadStandardCursor(id);
}

// return rectange of handle in logical coords
CRect CAnimObject::GetHandleRect(int nHandleID, CView* pView)
{
	ASSERT_VALID(this);
	ASSERT(pView != NULL);

	CRect rect;
	// get the center of the handle in logical coords
	CPoint point = GetHandle(nHandleID);
	// convert to client/device coords
//	pView->DocToClient(point);
	// return CRect of handle in device coords
	rect.SetRect(point.x-3, point.y-3, point.x+3, point.y+3);
//	pView->ClientToDoc(rect);

	return rect;
}



// returns logical coords of center of handle
CPoint CAnimObject::GetHandle(int nHandle)
{
	ASSERT_VALID(this);
	int x, y, xCenter, yCenter;
	CRect m_position = GetRect();
	// this gets the center regardless of left/right and top/bottom ordering
	xCenter = m_position.left + m_position.Width() / 2;
	yCenter = m_position.top + m_position.Height() / 2;

	switch (nHandle)
	{
	default:
		ASSERT(FALSE);

	case 1:
		x = m_position.left;
		y = m_position.top;
		break;

	case 2:
		x = xCenter;
		y = m_position.top;
		break;

	case 3:
		x = m_position.right;
		y = m_position.top;
		break;

	case 4:
		x = m_position.right;
		y = yCenter;
		break;

	case 5:
		x = m_position.right;
		y = m_position.bottom;
		break;

	case 6:
		x = xCenter;
		y = m_position.bottom;
		break;

	case 7:
		x = m_position.left;
		y = m_position.bottom;
		break;

	case 8:
		x = m_position.left;
		y = yCenter;
		break;
	}

	return CPoint(x, y);
}*/


/*void CAnimObject::DrawTracker(CDC* pDC, TrackerState state)
{
	ASSERT_VALID(this);

	switch (state)
	{
	case normal:
		break;

	case selected:
	case active:
		{
			//int nHandleCount = GetHandleCount();
			//for (int nHandle = 1; nHandle <= nHandleCount; nHandle += 1)
			//{
			//	CPoint handle = GetHandle(nHandle);
			//	pDC->PatBlt(handle.x - 3, handle.y - 3, 7, 7, DSTINVERT);
			}
		}
		break;
	}
}*/

//////////////////////////////////////////////////////////////////////
/// Draw the component on the device context of the view. 
/// \param pDC		A pointer to the relevant device context.
/// \param bParent	TRUE if the background shape has to be drawn.
///
//////////////////////////////////////////////////////////////////////
void CAnimObject::Draw (CDC* pDC,BOOL bParent)
{
	//if (CPref::nMode)
	{
		CBrush *oldB = NULL;
		CPen *oldP = NULL;

		//CBrush pBrush(::GetSysColor(COLOR_WINDOW));
		LOGBRUSH lpLogBrush = {BS_HOLLOW,0,HS_VERTICAL};
		CBrush pBrush;
		pBrush.CreateBrushIndirect(&lpLogBrush);
		CPen pPen(PS_NULL,0,CLR_DEFAULT);

		//LOGBRUSH pLogB;
		//m_pBrush.GetLogBrush(&pLogB);
		if (!m_pBrush.GetSafeHandle() || !m_pPen.GetSafeHandle())
			OnInitialUpdate();

		if (m_pLogBrush.lbColor != CLR_DEFAULT)
			oldB = pDC->SelectObject(&m_pBrush);
		else
			oldB = pDC->SelectObject(&pBrush);

		//LOGPEN pLogP;
		//m_pPen.GetLogPen(&pLogP);
		if (m_pLogPen.lopnColor != CLR_DEFAULT)
			oldP = pDC->SelectObject(&m_pPen);
		else
			oldP = pDC->SelectObject(&pPen);
	
		//pDC->Rectangle(m_ObjRect);
		CPoint cp = m_ObjRect.CenterPoint();
		CPoint lpPoints[10];
		switch (m_nShape){
		case CSHAPE_ELLIPSE:		// ELLIPSE
			pDC->Ellipse(m_ObjRect);
			break;
		case CSHAPE_DIAMOND:		// DIAMOND
			//pDC->MoveTo(cp.x,m_ObjRect.top);
			//pDC->LineTo(m_ObjRect.right,cp.y);
			//pDC->LineTo(cp.x,m_ObjRect.bottom);
			//pDC->LineTo(m_ObjRect.left,cp.y);
			//pDC->LineTo(cp.x,m_ObjRect.top);
			lpPoints[0] = CPoint(cp.x,m_ObjRect.top);
			lpPoints[1] = CPoint(m_ObjRect.right,cp.y);
			lpPoints[2] = CPoint(cp.x,m_ObjRect.bottom);
			lpPoints[3] = CPoint(m_ObjRect.left,cp.y);
			pDC->Polygon(lpPoints,4);
			break;
		case CSHAPE_TRIANGLEU:		// TRIANGLE (UP)
			//pDC->MoveTo(cp.x,m_ObjRect.top);
			//pDC->LineTo(m_ObjRect.right,m_ObjRect.bottom);
			//pDC->LineTo(m_ObjRect.left,m_ObjRect.bottom);
			//pDC->LineTo(cp.x,m_ObjRect.top);
			lpPoints[0] = CPoint(cp.x,m_ObjRect.top);
			lpPoints[1] = CPoint(m_ObjRect.right,m_ObjRect.bottom);
			lpPoints[2] = CPoint(m_ObjRect.left,m_ObjRect.bottom);
			pDC->Polygon(lpPoints,3);
			break;
		case CSHAPE_TRIANGLED:		// TRIANGLE (DOWN)
			//pDC->MoveTo(m_ObjRect.TopLeft());
			//pDC->LineTo(m_ObjRect.right,m_ObjRect.top);
			//pDC->LineTo(cp.x,m_ObjRect.bottom);
			//pDC->LineTo(m_ObjRect.left,m_ObjRect.top);
			lpPoints[0] = CPoint(m_ObjRect.TopLeft());
			lpPoints[1] = CPoint(m_ObjRect.right,m_ObjRect.top);
			lpPoints[2] = CPoint(cp.x,m_ObjRect.bottom);
			pDC->Polygon(lpPoints,3);
			break;
		default:	// CSHAPE_RECT
			//pDC->MoveTo(m_ObjRect.TopLeft());
			//pDC->LineTo(m_ObjRect.right,m_ObjRect.top);
			//pDC->LineTo(m_ObjRect.right,m_ObjRect.bottom);
			//pDC->LineTo(m_ObjRect.left,m_ObjRect.bottom);
			//pDC->LineTo(m_ObjRect.left,m_ObjRect.top);
			lpPoints[0] = CPoint(m_ObjRect.TopLeft());
			lpPoints[1] = CPoint(m_ObjRect.right,m_ObjRect.top);
			lpPoints[2] = CPoint(m_ObjRect.right,m_ObjRect.bottom);
			lpPoints[3] = CPoint(m_ObjRect.left,m_ObjRect.bottom);
			pDC->Polygon(lpPoints,4);
		}
		if (oldP)
			pDC->SelectObject(oldP);
		if (oldB)
			pDC->SelectObject(oldB);
	}
	if (m_bShowTrace)
	{
		int nb = min(m_currTime/2,m_cTracePt.GetSize());
		for (int i=0;i<nb;i++)
		{
			CPoint pt = m_cTracePt.GetAt(i);
			pt += m_nDDPos.m_ptDefPos;
			CRect mrect(pt,pt);
			mrect.InflateRect(1,1);
			pDC->Rectangle(mrect);
		}
	}
	//pDC->FillRect(m_ObjRect, &m_pBrush);
	if (m_bShowAxes)//(CPref::nMode == 0)
	{
		CPoint locPt = m_nDDPos.m_ptDefPos;
		CPoint curPt = m_ptXLim;
		if (m_ptXLim != CPoint(-1,-1))
		{
			int x = min(curPt.x+m_nDDPos.m_ptDefPos.x,m_nDDPos.m_ptDefPos.x);
			int y = max(curPt.y+m_nDDPos.m_ptDefPos.x,m_nDDPos.m_ptDefPos.x);
			pDC->MoveTo(x,locPt.y);
			pDC->LineTo(y,locPt.y);

		}
		curPt = m_ptYLim;
		if (m_ptYLim != CPoint(-1,-1))
		{
			int x = min(curPt.x+m_nDDPos.m_ptDefPos.y,m_nDDPos.m_ptDefPos.y);
			int y = max(curPt.y+m_nDDPos.m_ptDefPos.y,m_nDDPos.m_ptDefPos.y);
			pDC->MoveTo(locPt.x,x);
			pDC->LineTo(locPt.x,y);
		}

		if (m_nDDPos.m_ptDefPos != GetRect().TopLeft())
		{
			CRect mrect(m_nDDPos.m_ptDefPos,m_nDDPos.m_ptDefPos);
			mrect.InflateRect(2,2);
			pDC->Ellipse(mrect);
		}
	}
	if (m_bIsSelected)
	{
		CRect selRect = m_ObjRect;
		selRect.InflateRect(1,1,1,1);
		pDC->DrawFocusRect(selRect);
	}
}

void CAnimObject::Serialize(CArchive& ar)
{
	//CObject::Serialize(ar);
	if( ar.IsStoring( ) )
	{
		ar << m_bResize ;
		ar << m_ObjectID;
		ar << m_ObjPosition;
		ar << m_ObjectDef;
		ar << m_ObjRect;
		ar << m_ObjSize;
		ar << m_nRelatedID;
		ar << m_bShowAxes;
		ar << m_nShape;
		
		ar << m_nDDPos.m_ptDefPos;
		ar << m_nDDPos.m_ptPos;
		ar << m_nDDPos.m_nFactX;
		ar << m_nDDPos.m_nFactY;
		ar << m_nDDPos.m_nParamX;
		ar << m_nDDPos.m_nParamY;
		
		ar << m_nDDsize.m_ptDefSize;
		ar << m_nDDsize.m_szSize;
		ar << m_nDDsize.m_nFactW;
		ar << m_nDDsize.m_nFactH;
		ar << m_nDDsize.m_nParamW;
		ar << m_nDDsize.m_nParamH;

		LOGBRUSH plogBrush = m_pLogBrush;
		LOGPEN	 plogPen = m_pLogPen;
		//m_pPen.GetLogPen(&plogPen);
		//m_pBrush.GetLogBrush(&plogBrush);
		ar << plogBrush.lbColor;
		ar << plogBrush.lbHatch;
		ar << plogBrush.lbStyle;
		ar << plogPen.lopnColor;
		ar << plogPen.lopnStyle;
		ar << plogPen.lopnWidth;
	}
	else
	{
		int nb = ar.GetObjectSchema();
		ar >> m_bResize ;
		ar >> m_ObjectID;
		ar >> m_ObjPosition;
		ar >> m_ObjectDef;
		ar >> m_ObjRect;
		ar >> m_ObjSize;
		ar >> m_nRelatedID;

		ar >> m_bShowAxes;
		ar >> m_nShape;

		ar >> m_nDDPos.m_ptDefPos;
		ar >> m_nDDPos.m_ptPos;
		ar >> m_nDDPos.m_nFactX;
		ar >> m_nDDPos.m_nFactY;
		ar >> m_nDDPos.m_nParamX;
		ar >> m_nDDPos.m_nParamY;
		
		ar >> m_nDDsize.m_ptDefSize;
		ar >> m_nDDsize.m_szSize;
		ar >> m_nDDsize.m_nFactW;
		ar >> m_nDDsize.m_nFactH;
		ar >> m_nDDsize.m_nParamW;
		ar >> m_nDDsize.m_nParamH;

		LOGBRUSH plogBrush;
		LOGPEN	 plogPen;

		ar >> plogBrush.lbColor;
		ar >> plogBrush.lbHatch;
		ar >> plogBrush.lbStyle;
		ar >> plogPen.lopnColor;
		ar >> plogPen.lopnStyle;
		ar >> plogPen.lopnWidth;

		m_pLogBrush = plogBrush;
		m_pLogPen = plogPen;
	//	m_pPen.Detach();
	//	m_pPen.CreatePenIndirect(&plogPen);
	//	m_pBrush.Detach();
	//	m_pBrush.CreateBrushIndirect(&plogBrush);
		
	}
}

//////////////////////////////////////////////////////////////////////
/// Called by the framework to create and initialise the component's configuration dialog.
///		\param pView		ss
///		\param nShowSheet	ss
//////////////////////////////////////////////////////////////////////
int CAnimObject::OnEditProperty(CView *pView,int nShowSheet)
{
	int res = -1;

	//CAnimObjPropDlg mSheet(_T("Object n. 54"),pView);
	//CAnimObjPropDlg mSheet(_T("Object n. 54"),pView);
	CHelpPrSheet mSheet(_T("Object n. 54"),pView,0,TRUE);
	//mSheet.m_psh.dwFlags |= PSH_NOAPPLYNOW;
	
	mSheet.SetTitle(GetObjectDef());//,PSH_PROPTITLE);

	CViewAnimat *pAnimView = DYNAMIC_DOWNCAST(CViewAnimat, pView);
	CxAnimObjectList *myList = NULL;
	if (pAnimView)
	{
		CFormatAnimation *pAnimPage = pAnimView->GetFormats();
		if (pAnimPage)
		{
			//myList = &(pAnimView->m_cVisObjSet);
			int nb = pAnimPage->m_cObjSet.GetSize();
			if (nb)
				myList = &(pAnimPage->m_cObjSet);
		}
	}

	//mSheet.EnableStackedTabs(FALSE);
	SetPropSheetPages(&mSheet,myList);

	if ((res = mSheet.DoModal()) == IDOK)
	{
		ModifyObjectProperty();
		if ((m_nRelatedID != -1) && myList)
		{
			m_pRelatedObj = myList->GetAt(m_nRelatedID);
		}
		else
			m_pRelatedObj = NULL;
		//OnUpdate();
		//if (pView)
		//	pView->Invalidate();
	}
	DisposeProperty();

/*	if (m_pPosPage)
	{	
		delete m_pPosPage;
		m_pPosPage = NULL;
	}
	if (m_pSizePage) 
	{
		delete m_pSizePage;
		m_pSizePage = NULL;
	}
	if (m_pShapPage) 
	{
		delete m_pShapPage;
		m_pShapPage=NULL;
	}*/

	return res;

}

//////////////////////////////////////////////////////////////////////
/// Setup the configuration pane for the component's shape (see #m_pShapPage)
/// \param pSheet	A pointer to the property sheet that will contains the page.
/// \param pList	A pointer to the list of components of the view.
///
//////////////////////////////////////////////////////////////////////
void CAnimObject::SetPropSheetShape(CPropertySheet *pSheet,CxAnimObjectList *pList)
{
	if (!m_pShapPage)
		m_pShapPage = new CAnimShapePage();

	////// Setup Shape Page
	//LOGBRUSH pLogB;
	//m_pBrush.GetLogBrush(&pLogB);
	//LOGPEN pLogP;
	//m_pPen.GetLogPen(&pLogP);

	m_pShapPage->m_clrBk = m_pLogBrush.lbColor;
	m_pShapPage->m_clrLine = m_pLogPen.lopnColor;
	m_pShapPage->m_nLSize = m_pLogPen.lopnWidth.x;

	m_pShapPage->m_nShape = m_nShape;
	m_pShapPage->m_nLineStyle = (CDashLine::DashStyle) m_pLogPen.lopnStyle;
}

//////////////////////////////////////////////////////////////////////
/// Setup the configuration pane for the component's position (see #m_pPosPage)
/// \param pSheet	A pointer to the property sheet that will contains the page.
/// \param pList	A pointer to the list of components of the view.
///
//////////////////////////////////////////////////////////////////////
void CAnimObject::SetPropSheetPos(CPropertySheet *pSheet,CxAnimObjectList *pList)
{
	if (!m_pPosPage)
		m_pPosPage = new CAnimObjPosPage();

	////// Setup Linkable Components
	m_pPosPage->m_sCpntList.RemoveAll();
	if (pList)
	{
		BOOL bFound = FALSE;
		int nbC = pList->GetSize();
		for (int i=0;i<nbC && !bFound;i++)
		{
			CAnimObject *pObj = pList->GetAt(i);
			if (!pObj) continue;
			if (pObj != this)
			{
				m_pPosPage->m_sCpntList.Add(pObj->GetObjectDef());
			}
			else
				bFound = TRUE;
		}
	}
	m_pPosPage->m_nComponent = m_nRelatedID;
	m_pPosPage->m_bAttached = (BOOL)(m_nRelatedID != -1);

	////// Setup Pos Page
	m_pPosPage->m_bShowTrace = m_bShowTrace;
	m_pPosPage->m_bShowAxes = m_bShowAxes;
	// Update position specifications
	m_pPosPage->m_nDefXPos = m_nDDPos.m_ptDefPos.x;
	m_pPosPage->m_nDefYPos = m_nDDPos.m_ptDefPos.y;
	m_pPosPage->m_vHoriz = m_nDDPos.m_nParamX +1;
	m_pPosPage->m_vVert	= m_nDDPos.m_nParamY +1;
	m_pPosPage->m_xScale = m_nDDPos.m_nFactX;
	m_pPosPage->m_yScale = m_nDDPos.m_nFactY;
	//m_pPosPage->m_vHoriz = m_SizParam+1;
	//m_pPosPage->m_xScale = m_xFact;


}

//////////////////////////////////////////////////////////////////////
/// Setup the configuration pane for the component's size (see #m_pSizePage)
/// \param pSheet	A pointer to the property sheet that will contains the page.
/// \param pList	A pointer to the list of components of the view.
///
//////////////////////////////////////////////////////////////////////
void CAnimObject::SetPropSheetSize(CPropertySheet *pSheet,CxAnimObjectList *pList)
{
	if (!m_pSizePage)
		m_pSizePage = new CAnimObjSizePage();

	////// Setup Size Page
	// Update size specifications
	m_pSizePage->m_nDefWidth = m_nDDsize.m_ptDefSize.cx;
	m_pSizePage->m_nDefHeight = m_nDDsize.m_ptDefSize.cy;
	m_pSizePage->m_nDefHScale = m_nDDsize.m_nFactH;
	m_pSizePage->m_nDefWScale = m_nDDsize.m_nFactW;
	m_pSizePage->m_nHeigth = m_nDDsize.m_nParamH +1;
	m_pSizePage->m_nWidth = m_nDDsize.m_nParamW +1;
	m_pSizePage->m_bResize = m_bResize;
}


//////////////////////////////////////////////////////////////////////
/// Setup all the configuration panes used to define the component
/// \param pSheet	A pointer to the property sheet that will contains the page.
/// \param pList	A pointer to the list of components of the view.
///
//////////////////////////////////////////////////////////////////////
void CAnimObject::SetPropSheetPages(CPropertySheet *pSheet,CxAnimObjectList *pList)
{
	if (!m_pIntPage)
		m_pIntPage = new CAnimInterPage();

	SetPropSheetShape(pSheet,pList);
	SetPropSheetPos(pSheet,pList);
	SetPropSheetSize(pSheet,pList);

	////// Setup Pos/Size Outcomes Lists
	m_pPosPage->m_sVarList.RemoveAll();
	m_pSizePage->m_sVarList.RemoveAll();
	CLearningUnit *pLU= m_pDoc->GetCurrentLU();
	int nb = pLU->GetDataSize();
	for (int i=0;i<nb;i++)
	{
		CMdData *pData = pLU->GetDataAt(i);
		CString mstr = pData->GetDataName(pLU->m_bShowAbbrev);
		COLORREF mclr = pData->GetDataInfo()->GetColor();
		m_pPosPage->m_sVarList.Add(mstr);
		m_pPosPage->m_sOutcomeIcon.Add(pData->GetDataInfo()->GetIcon());
		m_pSizePage->m_sVarList.Add(mstr);
		m_pSizePage->m_sOutcomeIcon.Add(pData->GetDataInfo()->GetIcon());
		m_pShapPage->m_sVarList.Add(mstr);
		m_pShapPage->m_sVarColorList.Add(mclr);
		m_pShapPage->m_sOutcomeIcon.Add(pData->GetDataInfo()->GetIcon());
		
		m_pIntPage->m_sVarList.Add(mstr);
		m_pIntPage->m_sOutcomeIcon.Add(pData->GetDataInfo()->GetIcon());
	}

	////// Add Pages' Icon
	CImageList pImageList;
	pImageList.Create(IDB_ANIM_CPNTPAGE,16,0,RGB(192,192,192));

	m_pPosPage->m_psp.dwFlags |= PSP_USEHICON ;
	m_pPosPage->m_psp.hIcon = pImageList.ExtractIcon(7);

	m_pSizePage->m_psp.dwFlags |= PSP_USEHICON ;
	m_pSizePage->m_psp.hIcon = pImageList.ExtractIcon(8);

	m_pShapPage->m_psp.dwFlags |= PSP_USEHICON ;
	m_pShapPage->m_psp.hIcon = pImageList.ExtractIcon(2);

	m_pIntPage->m_psp.dwFlags |= PSP_USEHICON ;
	m_pIntPage->m_psp.hIcon = pImageList.ExtractIcon(11);

	////// Add all Pages
	pSheet->AddPage(m_pPosPage);
	pSheet->AddPage(m_pSizePage);
	pSheet->AddPage(m_pShapPage);
	pSheet->AddPage(m_pIntPage);
}

//////////////////////////////////////////////////////////////////////
/// Called by the framework when a component's configuration has been modified.
///
//////////////////////////////////////////////////////////////////////
void CAnimObject::ModifyObjectProperty()
{
	if (m_pPosPage)
	{
		m_bShowTrace = m_pPosPage->m_bShowTrace;
		m_bShowAxes = m_pPosPage->m_bShowAxes;
		//m_nDDsize.m_ptDefSize.cx = m_pSizePage->m_nDefHeight;
		//m_nDDsize.m_ptDefSize.cy = m_pSizePage->m_nDefWidth;

		//m_SizParam = m_pPosPage->m_vHoriz -1;
		//m_xFact = m_pPosPage->m_xScale;
		m_nDDPos.m_nParamX = m_pPosPage->m_vHoriz -1;
		m_nDDPos.m_nParamY = m_pPosPage->m_vVert -1;
		m_nDDPos.m_nFactX = m_pPosPage->m_xScale;
		m_nDDPos.m_nFactY = m_pPosPage->m_yScale;
		m_nDDPos.m_ptDefPos.x = m_pPosPage->m_nDefXPos;
		m_nDDPos.m_ptDefPos.y = m_pPosPage->m_nDefYPos;

		if (m_pPosPage->m_bAttached)
			m_nRelatedID = m_pPosPage->m_nComponent;
		else
			m_nRelatedID = -1;
	}
	if (m_pSizePage)
	{
		m_nDDsize.m_nFactH = m_pSizePage->m_nDefHScale;
		m_nDDsize.m_nFactW = m_pSizePage->m_nDefWScale;
		m_nDDsize.m_nParamH = m_pSizePage->m_nHeigth -1;
		m_nDDsize.m_nParamW = m_pSizePage->m_nWidth -1;
		if (m_bResize)
		{
			m_nDDsize.m_ptDefSize.cx = m_pSizePage->m_nDefWidth;
			m_nDDsize.m_ptDefSize.cy = m_pSizePage->m_nDefHeight;
		}	
	}

	if (m_pShapPage)
	{
		//LOGBRUSH pLogB;
		//LOGPEN pLogP;

		//m_pBrush.GetLogBrush(&pLogB);
		//m_pPen.GetLogPen(&pLogP);

		if (m_pShapPage->m_bAllowBk)
		{
			m_nShape = m_pShapPage->m_nShape;
			//pLogB.lbColor = m_pShapPage->m_clrBk;
			m_pLogBrush.lbColor = m_pShapPage->m_clrBk;
		}
		m_pLogPen.lopnColor = m_pShapPage->m_clrLine;
		m_pLogPen.lopnWidth.x = m_pShapPage->m_nLSize;
		m_pLogPen.lopnStyle = (m_pShapPage->m_nLineStyle>=CDashLine::DL_DASHDOTDOT) ?
								PS_INSIDEFRAME  : m_pShapPage->m_nLineStyle;
		
		m_pBrush.DeleteObject();
		//m_pBrush.CreateBrushIndirect(&pLogB);
		m_pPen.DeleteObject();
		//m_pPen.CreatePenIndirect(&pLogP);
	}
}

//////////////////////////////////////////////////////////////////////
/// Called by the framework to clean up all the interface associated with the component's configuration.
///
//////////////////////////////////////////////////////////////////////
void CAnimObject::DisposeProperty()
{
	if (m_pPosPage) 
	{
		delete m_pPosPage;
		m_pPosPage=NULL;
	}
	if (m_pSizePage)
	{
		delete m_pSizePage;
		m_pSizePage=NULL;
	}
	if (m_pShapPage) 
	{
		delete m_pShapPage;
		m_pShapPage=NULL;
	}
	if (m_pIntPage) 
	{
		delete m_pIntPage;
		m_pIntPage=NULL;
	}
}


//////////////////////////////////////////////////////////////////////
/// Return the bounding rectangle of the component
//////////////////////////////////////////////////////////////////////
CRect CAnimObject::GetRect()
{ 
	return m_ObjRect;
}


//////////////////////////////////////////////////////////////////////
/// Set the new bounding rectangle of the component
//////////////////////////////////////////////////////////////////////
void CAnimObject::SetRect(CRect lpSrcRect)
{ 
	CSize mP = lpSrcRect.TopLeft() - m_ObjRect.TopLeft();
	CSize mSW = lpSrcRect.Size() - m_ObjRect.Size();
	CSize mSh = lpSrcRect.Size();
	m_ObjRect = lpSrcRect;
	m_ObjPosition = m_ObjRect.TopLeft();

	//m_nDDPos.m_ptDefPos = m_ObjPosition;
	m_nDDPos.m_ptDefPos.Offset(mP);
	m_nDDsize.m_ptDefSize += mSW;//m_ObjRect.Size();
}

//////////////////////////////////////////////////////////////////////
/// Move the bounding rectangle of the component to a new position.
//////////////////////////////////////////////////////////////////////
void CAnimObject::MoveRect(CRect lpSrcRect)
{ 
	m_ObjRect = lpSrcRect;
	//m_ObjPosition = m_ObjRect.TopLeft();
	//m_nDDPos.m_ptDefPos = m_ObjPosition;
	//m_nDDsize.m_ptDefSize = m_ObjRect.Size();
}

//////////////////////////////////////////////////////////////////////
/// Change the dimensions (width and height) of the component.
//////////////////////////////////////////////////////////////////////
void CAnimObject::ResizeRect (CSize lpSize)
{
	CRect lpSrcRect(m_ObjRect.TopLeft(),m_ObjRect.TopLeft());

	lpSrcRect.InflateRect(0,0,m_nDDsize.m_szSize.cx,m_nDDsize.m_szSize.cy);
	m_ObjRect = lpSrcRect;
}

//////////////////////////////////////////////////////////////////////
/// Called by the framework when the learner has selected an entity in an External Representation (translation).
/// \param nData	Zero-based index of the selected entity, -1 if no data-point is selected.
///
/// Usually called to propagate a translation request, rhis function (overrifen in most of the components)
/// will check if this component is configured to represent the relevant entity (\a nData).
/// If so, then the component will be displayed as selected (by switching the #m_bIsSelected attributes to TRUE).
//////////////////////////////////////////////////////////////////////
void CAnimObject::OnUpdateSelection(int nData)
{
}

//////////////////////////////////////////////////////////////////////
/// Define which components, if any, are at the specified location.
///		\param pt		A point (in view coordinates) to be tested
///		\param pList	A pointer to a list that will contain all the component at this location.
///		\return			A pointer to this component if it is a the location, NULL otherwise
//////////////////////////////////////////////////////////////////////
CAnimObject* CAnimObject::OnHitTest(CPoint pt,CxAnimObjectList *pList)
{
	CAnimObject* pObj = NULL;
	CRect mrect = GetRect();

	if (mrect.PtInRect(pt))
	{
		pObj = this;
		if (pList)
			pList->Add(pObj);
	}
	return pObj;
}

//////////////////////////////////////////////////////////////////////
/// Define which components, if any, are within the specified area.
///		\param rct		A rectangle (in view coordinates) to be tested
///		\param pList	A pointer to a list that will contain all the component at this location.
///		\return			A pointer to this component if it is a the location, NULL otherwise
//////////////////////////////////////////////////////////////////////
CAnimObject* CAnimObject::OnHitTest(CRect rct,CxAnimObjectList *pList)
{
	CAnimObject* pObj = NULL;
	CRect mrect = GetRect();
	CRect resRect;

//	if (resRect.IntersectRect(mrect,rct))
	if (resRect.IntersectRect(rct,mrect))
	//if (rct.PtInRect(mrect.CenterPoint()))
	{
		pObj = this;
		if (pList)
			pList->Add(pObj);
	}
	return pObj;
}




//////////////////////////////////////////////////////////////////////
/// Called by the framework when the position of the component has to be recomputed.
///
//////////////////////////////////////////////////////////////////////
void CAnimObject::OnUpdatePosition()
{
	CLearningUnit *pLU= m_pDoc->GetCurrentLU();
	if (!pLU)		return;
	int nExpSet = m_currExpSet;

	if (m_nDDPos.m_nParamX >= pLU->GetDataSize())
		m_nDDPos.m_nParamX = -1;
	if (m_nDDPos.m_nParamY >= pLU->GetDataSize())
		m_nDDPos.m_nParamY = -1;
	//if (m_SizParam==-1) return;

	m_ptXLim = m_ptYLim = CPoint(-1,-1);
/*	if ((m_nDDPos.m_nParam.X == -1) && (m_nDDPos.m_nParam.Y == -1))
	{
		return;
	}*/
	double dXValue = 0.0;
	double dYValue = 0.0;

	double xmin,xmax,ymin,ymax;

	if (m_nDDPos.m_nParamX == -1)
	{
		dXValue = m_currTime;
		xmin = 0;
		xmax = pLU->GetMaxTimer();
		m_ptXLim = CPoint(0,0);
	}
	else if (m_nDDPos.m_nParamX >= 0)
	{
		CMdData *pData = pLU->GetDataAt(m_nDDPos.m_nParamX);
		if (!pData) return;
		double iValue = pData->GetAt(0,nExpSet);
		dXValue = pData->GetAt(m_currTime,nExpSet);
		//m_scaleFactor = 1+ m_xFact * (dValue/*-iValue*/) ;
		pData->GetMinMax(&xmin,&xmax,nExpSet);
		m_ptXLim = CPoint(0,0);
	}
	if (m_nDDPos.m_nParamY == -1)
	{
		dYValue = m_currTime;
		ymin = 0;
		ymax = pLU->GetMaxTimer();
		m_ptYLim = CPoint(0,0);
	}
	else if (m_nDDPos.m_nParamY >= 0)
	{
		CMdData *pData = pLU->GetDataAt(m_nDDPos.m_nParamY);
		if (!pData) return;
		double iValue = pData->GetAt(0,nExpSet);
		dYValue = pData->GetAt(m_currTime,nExpSet);
		//m_scaleFactor = 1+ m_xFact * (dValue/*-iValue*/) ;
		pData->GetMinMax(&ymin,&ymax,nExpSet);
		m_ptYLim = CPoint(0,0);
	}

	CPoint pt;
	pt.x = (long)(m_nDDPos.m_nFactX * (dXValue/*-iValue*/));
	pt.y = (long)(m_nDDPos.m_nFactY * (dYValue/*-iValue*/));

	if (m_ptXLim != CPoint(-1,-1))
	{
		m_ptXLim.x = (long)(m_nDDPos.m_nFactX * (xmin/*-iValue*/));
		m_ptXLim.y = (long)(m_nDDPos.m_nFactX * (xmax/*-iValue*/) /*+ m_nDDsize.m_ptDefSize.cx*/);
	}
	if (m_ptYLim != CPoint(-1,-1))
	{
		m_ptYLim.x = (long)(m_nDDPos.m_nFactY * (ymin/*-iValue*/) /*+ m_nDDsize.m_ptDefSize.cy*/);
		m_ptYLim.y = (long)(m_nDDPos.m_nFactY * (ymax/*-iValue*/) );
	}

	CRect mrect = m_ObjRect;
	CPoint pt2 = m_nDDPos.m_ptDefPos;

	pt+= m_nDDPos.m_ptDefPos;

	CPoint pt3 = pt;
//	pt3 += CPoint(m_nDDsize.m_szSize.cx/2,m_nDDsize.m_szSize.cy/2);
	if (m_nRelatedID != -1)
	{
		if (m_pRelatedObj)
		{
			pt += m_pRelatedObj->GetRect().TopLeft();
			pt3 += m_pRelatedObj->GetRect().TopLeft();
		}
	}
	m_cTracePt.SetAtGrow(m_currTime/2,pt3);

//	if (pt != pt2)
	{
//		pt+= m_nDDPos.m_ptDefPos;
//		if (m_nRelatedID != -1)
//		{
//			if (m_pRelatedObj)
//				pt += m_pRelatedObj->GetRect().TopLeft();
//		}
		pt2 = pt;
		pt2.x += mrect.Width();
		pt2.y += mrect.Height();
		//mrect.OffsetRect(pt-pt2);
		MoveRect(CRect(pt,pt2));
		//m_ObjRect = mrect;
	}
}

//////////////////////////////////////////////////////////////////////
/// Called by the framework when the size of the component has to be recomputed.
///		\param rct		dsds
///		\param pList	dsds
///		\return			dsdsds
//////////////////////////////////////////////////////////////////////
void CAnimObject::OnUpdateSize()
{
	CLearningUnit *pLU= m_pDoc->GetCurrentLU();
	if (!pLU)		return;
	int nExpSet = m_currExpSet;

	if (m_nDDsize.m_nParamW >= pLU->GetDataSize())
		m_nDDsize.m_nParamW = -1;
	if (m_nDDsize.m_nParamH >= pLU->GetDataSize())
		m_nDDsize.m_nParamH = -1;

/*	if ((m_nDDPos.m_nParam.X == -1) && (m_nDDPos.m_nParam.Y == -1))
	{
		return;
	}*/
	double dXValue = 0.0;
	double dYValue = 0.0;

	double xmin,xmax,ymin,ymax;

	if (m_nDDsize.m_nParamW == -1)
	{
		dXValue = m_currTime;
		xmin = 0;
		xmax = pLU->GetMaxTimer();
	}
	else if (m_nDDsize.m_nParamW >= 0)
	{
		CMdData *pData = pLU->GetDataAt(m_nDDsize.m_nParamW);
		if (!pData) return;
		double iValue = pData->GetAt(0,nExpSet);
		dXValue = pData->GetAt(m_currTime,nExpSet);
		//m_scaleFactor = 1+ m_xFact * (dValue/*-iValue*/) ;
		pData->GetMinMax(&xmin,&xmax,nExpSet);
	}
	if (m_nDDsize.m_nParamH == -1)
	{
		dYValue = m_currTime;
		ymin = 0;
		ymax = pLU->GetMaxTimer();
	}
	else if (m_nDDsize.m_nParamH >= 0)
	{
		CMdData *pData = pLU->GetDataAt(m_nDDsize.m_nParamH);
		if (!pData) return;
		double iValue = pData->GetAt(0,nExpSet);
		dYValue = pData->GetAt(m_currTime,nExpSet);
		//m_scaleFactor = 1+ m_xFact * (dValue/*-iValue*/) ;
		pData->GetMinMax(&ymin,&ymax,nExpSet);
	}

	CSize ms;
	ms.cx = (long)(m_nDDsize.m_nFactW * (dXValue/*-iValue*/));
	ms.cy = (long)(m_nDDsize.m_nFactH * (dYValue/*-iValue*/));

	CRect mrect = m_ObjRect;
	CPoint pt2 = m_nDDPos.m_ptDefPos;

	m_nDDsize.m_szSize = m_nDDsize.m_ptDefSize + ms;
	//if (ms != CSize(0,0))
		ResizeRect(ms);
/*	if (pt != pt2)
	{
		pt+= m_nDDPos.m_ptDefPos;
		pt2 = pt;
		pt2.x += mrect.Width();
		pt2.y += mrect.Height();
		//mrect.OffsetRect(pt-pt2);
		MoveRect(CRect(pt,pt2));
		//m_ObjRect = mrect;
	}*/
}

//////////////////////////////////////////////////////////////////////
/// Called by the framework when the component is initially displayed.
//////////////////////////////////////////////////////////////////////
void CAnimObject::OnInitialUpdate()
{
	m_pBrush.DeleteObject();
	m_pBrush.CreateBrushIndirect(&m_pLogBrush);

	m_pPen.DeleteObject();
	m_pPen.CreatePenIndirect(&m_pLogPen);
//m_pLogBrush.lbStyle = BS_SOLID;
	//m_pLogBrush.lbColor = CLR_DEFAULT;
}

//////////////////////////////////////////////////////////////////////
/// Called by the framework when the component is closed.
//////////////////////////////////////////////////////////////////////
void CAnimObject::OnClose()
{
	m_pBrush.DeleteObject();
}


//////////////////////////////////////////////////////////////////////
/// Called by the framework when the component's definition (position, size, etc) has to be recomputed.
//////////////////////////////////////////////////////////////////////
void CAnimObject::OnUpdate()
{
	//m_currTime = m_pDoc->m_currTimer;

	OnUpdatePosition();
	OnUpdateSize();
}

//////////////////////////////////////////////////////////////////////
/// Called by the framework when a prediction has been produced by learner
///		\param pData	ddd
//////////////////////////////////////////////////////////////////////
void CAnimObject::OnUpdateHypothesis(CUserData *pData)
{
	//m_currTime = m_pDoc->m_currTimer;

	OnUpdatePosition();
	OnUpdateSize();
}

//////////////////////////////////////////////////////////////////////
/// Called by the framework when the current time has been modified by the learner of by the 
/// system.
//////////////////////////////////////////////////////////////////////
void CAnimObject::OnUpdateTimer()
{

}

//////////////////////////////////////////////////////////////////////
/// Called by the framework when the user requests a translation from this component.
//////////////////////////////////////////////////////////////////////
BOOL CAnimObject::OnDoTranslation(CPoint pt,int& nOutcome) 
{
	return FALSE;
}

//////////////////////////////////////////////////////////////////////
/// Called by the framework when the user acts on the component (for prediction or action).
//////////////////////////////////////////////////////////////////////
BOOL CAnimObject::OnDoEdit(CPoint pt,int& nOutcome,double& nval,double& oval,CWnd* pParent/*=NULL*/) 
{
	return FALSE;
}

//////////////////////////////////////////////////////////////////////
/// Used to setup or remove the positional relationship between two components.
///		\param nIdx		ss
///		\param myList	ss
//////////////////////////////////////////////////////////////////////
void CAnimObject::SetRelatedObj(int nIdx/*=-1*/,CxAnimObjectList *myList /*= NULL*/)
{ 
	m_nRelatedID = nIdx;
	m_pRelatedObj = NULL;
	if (nIdx != -1 && myList)
	{
		int nb = myList->GetSize();
		if (nIdx<nb)
			m_pRelatedObj = myList->GetAt(nIdx);
	}
}


//////////////////////////////////////////////////////////////////////
// CDensityData
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CDensityData, CObject, VERSIONABLE_SCHEMA | 4)

CDensityData::CDensityData()
{
	m_nOutcome = -1;
	m_oPict = NULL;
	m_nPrec = 1.0;
}

CDensityData::~CDensityData()
{
	delete m_oPict;
	m_ptArray.RemoveAll();
}

void CDensityData::Serialize(CArchive &ar)
{
	if( ar.IsStoring( ) )
	{
		ar << m_nOutcome;
		if (m_oPict)
			ar << m_oPict->m_nBitmap;
		else
			ar << 0;
		ar << m_nPrec;
	}
	else
	{
		ar >> m_nOutcome;
		UINT nAr = ar.GetObjectSchema();
		if (nAr>=3)
		{
			m_oPict = NULL;
			int nb;
			ar >> nb;
			m_oPict= (CPictObject*)nb;
		}
		if (nAr>=4)
		{
			ar >> m_nPrec;
		}
	}
}

void CDensityData::ClearData()
{
	m_nOutcome = -1;
	m_ptArray.RemoveAll();
}

//////////////////////////////////////////////////////////////////////
// CDensityObject
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CDensityObject, CAnimObject, VERSIONABLE_SCHEMA | 2)


CDensityObject::CDensityObject (): 
	CAnimObject ()
{
	m_pDensityPage = NULL;
	m_bShowLegend = FALSE;
	m_bShowValues = FALSE;
//	m_ObjectDef.Format(_T("Pop. Density n.%d"),m_ObjectID);
//	SetObjectDef(_T(""));
/*	int nb = m_lData.GetCount();
	if (!nb)
	{
		CDensityData *pData= NULL;
		pData = new CDensityData();
		pData->m_nOutcome=-1;
		pData->m_oPict = new CPictObject(155,CRect(10,10,30,150), IDB_ROUND);
		m_lData.AddHead(pData);
		pData = new CDensityData();
		pData->m_nOutcome=-1;
		pData->m_oPict = new CPictObject(156,CRect(50,50,240,150), IDB_SQUARE);
		//cob->m_scaleFactor=0.75;
		m_lData.AddHead(pData);
	}
	else
	{
		CDensityData *pData = (CDensityData*) m_lData.GetHead();
		pData->m_nOutcome=-1;
		pData->m_oPict = new CPictObject(155,CRect(10,10,30,150), IDB_ROUND);
		pData = (CDensityData*) m_lData.GetTail();
		pData->m_nOutcome=-1;
		pData->m_oPict = new CPictObject(156,CRect(50,50,240,150), IDB_SQUARE);

	}*/
	SetObjectColor();
}

CDensityObject::CDensityObject (int ID,const CRect &position): 
	CAnimObject (ID,position)
{
	m_pDensityPage = NULL;
	m_bShowLegend = FALSE;
	m_bShowValues = FALSE;

//	m_ObjectDef.Format(_T("Pop. Density n.%d"),m_ObjectID);
//	SetObjectDef(_T(""));
	CDensityData *pData= NULL;
	pData = new CDensityData();
	pData->m_nOutcome=7;
	pData->m_oPict = new CPictObject(155,CRect(10,10,30,150), IDB_ROUND);
	m_lData.AddHead(pData);
	pData = new CDensityData();
	pData->m_nOutcome=6;
	pData->m_oPict = new CPictObject(156,CRect(50,50,240,150), IDB_SQUARE);
	//cob->m_scaleFactor=0.75;
	m_lData.AddHead(pData);
	SetObjectColor();
}

CDensityObject::CDensityObject (CDensityObject *pObj) :
		CAnimObject(pObj)
{
	m_pDensityPage = NULL;//pObj->m_pDensityPage;
	m_bShowLegend = pObj->m_bShowLegend;
	m_bShowValues = pObj->m_bShowValues;

	CDensityData *pData= NULL;
	POSITION pos = pObj->m_lData.GetHeadPosition();
	while (pos)
	{
		pData = (CDensityData *)m_lData.GetNext(pos);
		if (pData)
		{
			CDensityData *pData2 = new CDensityData();
			pData2->m_nOutcome=pData->m_nOutcome;
			pData2->m_oPict = (CPictObject*)pData->m_oPict->Clone(); ;
			m_lData.AddTail(pData2);
		}


	}
	/*pData = new CDensityData();
	pData->m_nOutcome=6;
	pData->m_oPict = new CPictObject(155,CRect(10,10,30,150), IDB_ROUND);
	m_lData.AddHead(pData);
	pData = new CDensityData();
	pData->m_nOutcome=7;
	pData->m_oPict = new CPictObject(156,CRect(50,50,240,150), IDB_SQUARE);
	//cob->m_scaleFactor=0.75;
	m_lData.AddHead(pData);*/
}

int	 CDensityObject::GetObjectIcon()
{
	return CICON_POPD;
}


CAnimObject* CDensityObject::Clone()
{
	CDensityObject *pObj = new CDensityObject(this);
	return pObj;
}

CDensityObject::~CDensityObject()
{
	if (m_pDensityPage) delete m_pDensityPage;

    // Garbage section
    POSITION pos = m_lData.GetHeadPosition ();
    while (pos != NULL)
        delete m_lData.GetNext (pos);
    m_lData.RemoveAll ();
}

CString CDensityObject::SetObjectType()
{
	CString mstr;
	mstr.Format(_T("Pop. Density n.%d"),m_ObjectID);
	return mstr;
}



/*void CDensityObject::SetObjectDef(CString mstr)
{
	if (mstr.GetLength() == 0)
	{
	m_ObjectDef.Format(_T("Pop. Density n.%d"),m_ObjectID);
	}
	else
		m_ObjectDef = mstr;
}*/

void CDensityObject::Serialize(CArchive &ar)
{
	CAnimObject::Serialize(ar);
	if( ar.IsStoring( ) )
	{
		ar << m_bShowLegend;
		ar << m_bShowValues;
		m_lData.Serialize(ar);
	}
	else
	{
		int nb = ar.GetObjectSchema();
		ar >> m_bShowLegend;
		ar >> m_bShowValues;
		{
			m_lData.Serialize(ar);
			int nb = m_lData.GetCount();
			if (!nb)
			{
				CDensityData *pData= NULL;
				pData = new CDensityData();
				pData->m_nOutcome=-1;
				pData->m_oPict = new CPictObject(155,CRect(10,10,30,150), IDB_ROUND);
				m_lData.AddHead(pData);
				pData = new CDensityData();
				pData->m_nOutcome=-1;
				pData->m_oPict = new CPictObject(156,CRect(50,50,240,150), IDB_SQUARE);
				//cob->m_scaleFactor=0.75;
				m_lData.AddHead(pData);
			}
			else
			{
			    POSITION pos = m_lData.GetHeadPosition ();
				while (pos != NULL)
				{
			        CDensityData *pData  = (CDensityData *)m_lData.GetNext (pos);
					if (pData)
					{
						int nb = (int)pData->m_oPict;
						if (!nb)
						  pData->m_oPict = new CPictObject(155,CRect(10,10,30,150), IDB_ROUND);
						else
						  pData->m_oPict = new CPictObject(155,CRect(10,10,30,150), nb);
					}
				}
				//CDensityData *pData = (CDensityData*) m_lData.GetHead();
				//pData->m_nOutcome=-1;
				//pData->m_oPict = new CPictObject(155,CRect(10,10,30,150), IDB_ROUND);
				//pData = (CDensityData*) m_lData.GetTail();
				//pData->m_nOutcome=-1;
				//pData->m_oPict = new CPictObject(156,CRect(50,50,240,150), IDB_SQUARE);
			}
		}
	}
}

void CDensityObject::SetPropSheetPages(CPropertySheet *pSheet,CxAnimObjectList *pList)
{
	if (!m_pDensityPage)
		m_pDensityPage = new CAnimDensityPage();
	m_pDensityPage->m_pObjects = this;
	m_pDensityPage->m_bShowLegend = m_bShowLegend;
	m_pDensityPage->m_bShowValues = m_bShowValues;

	m_pDensityPage->m_sOutcomeArray.RemoveAll();
	CLearningUnit *pLU= m_pDoc->GetCurrentLU();
	//m_pDoc->GetCurrentModel();
	int nb = pLU->GetDataSize();
	for (int i=0;i<nb;i++)
	{
		CMdData *pData = pLU->GetDataAt(i);
		CString mstr = pData->GetDataName(pLU->m_bShowAbbrev);
		m_pDensityPage->m_sOutcomeArray.Add(mstr);

		m_pDensityPage->m_sOutcomeIcon.Add(pData->GetDataInfo()->GetIcon());
	}

	CImageList pImageList;
	pImageList.Create(IDB_ANIM_CPNTPAGE,16,0,RGB(192,192,192));

	m_pDensityPage->m_psp.dwFlags |= PSP_USEHICON ;
	m_pDensityPage->m_psp.hIcon = pImageList.ExtractIcon(4);

	pSheet->AddPage(m_pDensityPage);

	pSheet->SetActivePage(m_pDensityPage);
}


void CDensityObject::ModifyObjectProperty()
{
	//CAnimObject::ModifyObjectProperty();
	m_bShowLegend = m_pDensityPage->m_bShowLegend;
	m_bShowValues = m_pDensityPage->m_bShowValues;

	POSITION pos = m_lData.GetHeadPosition ();
    while (pos != NULL)
	{
        CDensityData *pData = (CDensityData*)m_lData.GetNext(pos);
		if (!pData) continue;
		pData->m_oPict->OnClose();
	}

	OnUpdate();
}

void CDensityObject::DisposeProperty()
{
	CAnimObject::DisposeProperty();
	if (m_pDensityPage) 
	{
		delete m_pDensityPage;
		m_pDensityPage=NULL;
	}

}


void CDensityObject::Draw (CDC* pDC,BOOL bParent)
{
	bool bIsOK = false;

	CAnimObject::Draw (pDC,bParent);
	if (m_bShowLegend)
	{
		CRect mrect = m_ObjRect;
		mrect.bottom -= 20;

		CRect mrect2 = m_ObjRect;
		mrect2.top = mrect2.bottom-22;

		CBrush pBrush(RGB(192,192,192));
		//pDC->Draw3dRect(mrect,RGB(192,192,192),RGB(255,255,255));
		//pDC->Draw3dRect(mrect2,RGB(192,192,192),RGB(255,255,255));
		pDC->DrawEdge(mrect,EDGE_RAISED,BF_RECT );
		pDC->FillRect(mrect2,&pBrush);
		pDC->DrawEdge(mrect2,EDGE_RAISED,BF_RECT );

	}
	else
		pDC->DrawEdge(m_ObjRect,EDGE_RAISED,BF_RECT );
		//pDC->Draw3dRect(m_ObjRect,RGB(192,192,192),RGB(255,255,255));
	
	CLearningUnit *pLU= m_pDoc->GetCurrentLU();
	CPoint posL(m_ObjRect.left,m_ObjRect.bottom);
	posL.Offset(10,-19);
	POSITION pos = m_lData.GetHeadPosition ();
    while (pos != NULL)
	{
        CDensityData *pData = (CDensityData*)m_lData.GetNext(pos);
		if (!pData) continue;

		if (pData->m_nOutcome >= pLU->GetDataSize())
			pData->ClearData();

		if (pData->m_nOutcome==-1) continue;

		int nb = pData->m_ptArray.GetSize();
		for (int i=0;i<nb;i++)
		{
			CPoint ppt = pData->m_ptArray.GetAt( i );
			CRect mrect = pData->m_oPict->GetRect();
			CPoint ppt1 = m_ObjRect.TopLeft() + ppt;
			CPoint ppt2 = ppt1-mrect.TopLeft();
			mrect.OffsetRect(ppt2);
			pData->m_oPict->SetRect(mrect);
			pData->m_oPict->Draw(pDC);
		}

		if (m_bShowLegend)
		{
			CMdData *pVal = pLU->GetDataAt(pData->m_nOutcome);
			CString mstr = pVal->GetDataName(pLU->m_bShowAbbrev);

			if (pData->m_nPrec>1)
			{
				CString strPrec;
				strPrec.Format(_T("x%d"),(int)pData->m_nPrec);
				mstr += strPrec;
			}

			if (m_bShowValues)
			{
				int nTime = m_currTime;
				int nExpSet = m_currExpSet;

				double vv = pVal->GetAt(nTime,nExpSet);
				CString valstr;

				if (m_bShowValues == 1)
					valstr.Format(_T(" (%.4g)"),vv);
				else
					valstr.Format(_T(" (%d)"),nb);
				mstr+=valstr;
			}
			
			CRect mrect(posL,posL);
			mrect.InflateRect(0,0,16,16);
			pData->m_oPict->SetRect(mrect);
			pData->m_oPict->Draw(pDC);

			mrect.InflateRect(-15,0,500,0);

			int oldMode;
			oldMode=pDC->SetBkMode(TRANSPARENT);

			pDC->DrawText( mstr, mrect, DT_LEFT | DT_SINGLELINE| DT_CALCRECT );
			pDC->DrawText( mstr, mrect, DT_LEFT | DT_SINGLELINE);
			pDC->SetBkMode(oldMode);

			posL.Offset(mrect.Width()+25,0);
		}
		bIsOK=true;
	}
	if (!bIsOK)
	{
		int idx = (m_bShowLegend) ? 20 : 0;
		pDC->MoveTo(m_ObjRect.TopLeft());
		pDC->LineTo(m_ObjRect.right,m_ObjRect.bottom-idx);
		pDC->MoveTo(m_ObjRect.right,m_ObjRect.top);
		pDC->LineTo(m_ObjRect.left,m_ObjRect.bottom-idx);
	}
}

void CDensityObject::OnUpdateHypothesis(CUserData *pData)
{
	//m_currTime = m_pDoc->m_currTimer;
	CAnimObject::OnUpdateHypothesis(pData);
	POSITION pos = m_lData.GetHeadPosition ();
    while (pos != NULL)
	{
		CDensityData *pDData = (CDensityData*)m_lData.GetNext(pos);
		if (!pDData) continue;
		if (!pData)
		{
			pDData->m_ptArray.RemoveAll();
		}
		else if (pDData->m_nOutcome == pData->m_nOutcome && 
				 m_currExpSet == pData->m_nExpSet)
		{

			int nbData = pDData->m_ptArray.GetSize();

			//CMdData *pData = pLU->GetDataAt(pDData->m_nOutcome);
			//double nDVal = pData->GetAt(m_currTime,nExpSet) / pDData->m_nPrec;
			double nDVal = pData->m_dValue;
			int nVal = (int) min(nDVal,1000);
			if (nVal < 0) nVal = 0;

			if (nbData > nVal)
			{
				for (int i =0;i<nbData-nVal;i++)
				{
					int size = pDData->m_ptArray.GetUpperBound();
					int num = 0;
					if (size)
						num = rand() % size;
					pDData->m_ptArray.RemoveAt(num,1);
				}
			}
			else
			{
				int idx = (m_bShowLegend) ? 20 : 0;
				for	(int i=nbData;i<nVal;i++)
				{
					int w = rand() % (m_ObjRect.Width()-16);
					int l = rand() % (m_ObjRect.Height()-16-idx);
					pDData->m_ptArray.Add(CPoint(w,l));
				}
			}
		}
	}
}


BOOL CDensityObject::OnDoTranslation(CPoint pt,int& nOutcome)
{
	BOOL bRedraw = FALSE;
	POSITION pos = m_lData.GetHeadPosition ();
    while (pos != NULL)
	{
		CDensityData *pDData = (CDensityData*)m_lData.GetNext(pos);
		if (!pDData) continue;
		if (pDData->m_nOutcome==-1) continue;

		int nb = pDData->m_ptArray.GetSize();
		CxAnimObjectList pList;
		for (int i=0;i<nb;i++)
		{
			CPoint ppt = pDData->m_ptArray.GetAt( i );
			CRect mrect = pDData->m_oPict->GetRect();
			CPoint ppt1 = m_ObjRect.TopLeft() + ppt;
			CPoint ppt2 = ppt1-mrect.TopLeft();
			mrect.OffsetRect(ppt2);
			pDData->m_oPict->SetRect(mrect);
			//pDData->m_oPict->Draw(pDC);
			CAnimObject* pObj = pDData->m_oPict->OnHitTest(pt,&pList);
		}
		if (pList.GetSize())
		{
			//if (!pDData->m_oPict->m_bIsSelected)
			{
				((CAnimObject* )pDData->m_oPict)->m_bIsSelected = TRUE;
				nOutcome = pDData->m_nOutcome;
			}
				bRedraw |= TRUE;
		}
/*		else
		{
			if (((CAnimObject* )pDData->m_oPict)->m_bIsSelected)
			{
				((CAnimObject* )pDData->m_oPict)->m_bIsSelected = FALSE;
				bRedraw |= TRUE;
			}
		}*/
	}
//	if (bRedraw)
//		OnUpdate();
	return bRedraw;
}

void CDensityObject::OnUpdateSelection(int nData)
{
	CAnimObject::OnUpdateSelection(nData);
	BOOL bRedraw = FALSE;
	POSITION pos = m_lData.GetHeadPosition ();
    while (pos != NULL)
	{
		CDensityData *pDData = (CDensityData*)m_lData.GetNext(pos);
		if (!pDData) continue;
		if (pDData->m_nOutcome==-1) continue;

		if (pDData->m_nOutcome==nData)
		{
			pDData->m_oPict->m_bIsSelected = TRUE;
			//break;
		}
		else if (nData==-1)
			pDData->m_oPict->m_bIsSelected = FALSE;
	}


}



void CDensityObject::OnUpdate()
{
	CAnimObject::OnUpdate();

	CLearningUnit *pLU= m_pDoc->GetCurrentLU();

	//m_currTime  = m_pDoc->m_currTimer;
	int nExpSet = m_currExpSet;

	POSITION pos = m_lData.GetHeadPosition ();
    while (pos != NULL)
	{
		CDensityData *pDData = (CDensityData*)m_lData.GetNext(pos);
		if (!pDData) continue;


		if (pDData->m_nOutcome >= pLU->GetDataSize())
			pDData->ClearData();
		if (pDData->m_nOutcome==-1) continue;

//		pDData->m_oPict->m_bIsSelected = FALSE;
		int nbData = pDData->m_ptArray.GetSize();

		CMdData *pData = pLU->GetDataAt(pDData->m_nOutcome);
		double nDVal = pData->GetAt(m_currTime,nExpSet) / pDData->m_nPrec;
		int nVal = (int) min(nDVal,1000);
		if (nVal < 0) nVal = 0;

		if (nbData > nVal)
		{
			for (int i =0;i<nbData-nVal;i++)
			{
				int size = pDData->m_ptArray.GetUpperBound();
				int num = 0;
				if (size)
					num = rand() % size;
				pDData->m_ptArray.RemoveAt(num,1);
			}
			//pDData->m_ptArray.RemoveAt(nVal,nbData-nVal);
		}
		else
		{
			int idx = (m_bShowLegend) ? 20 : 0;
			for (int i=nbData;i<nVal;i++)
			{
				int w = rand() % (m_ObjRect.Width()-16);
				int l = rand() % (m_ObjRect.Height()-16-idx);
				pDData->m_ptArray.Add(CPoint(w,l));
			}
		}

	}

}

void CDensityObject::SetRect(CRect lpSrcRect)
{
	CRect oldrect = m_ObjRect;
	CAnimObject::SetRect(lpSrcRect);

	POSITION pos = m_lData.GetHeadPosition ();
    while (pos != NULL)
	{
        CDensityData *pData = (CDensityData*)m_lData.GetNext(pos);
		if (!pData) continue;
		if (pData->m_nOutcome==-1) continue;


		int nb = pData->m_ptArray.GetSize();
		for (int i=0;i<nb;i++)
		{
			int idx = (m_bShowLegend) ? 20 : 0;
			CPoint &pt= pData->m_ptArray.ElementAt(i);
			pt.x = (m_ObjRect.Width())*pt.x/oldrect.Width();
			pt.y = (m_ObjRect.Height())*pt.y/oldrect.Height();
		}
	}
}
