// AnimObject.cpp: implementation of the CAnimObject class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <wingdi.h>
#include <math.h>

#include "Simul.h"

//#include "MRTracker.h"
#include "SimulDoc.h"
#include "AnimObject.h"
#include <MInstruction\LearningUnit.h>
#include <MSimulation\Model.h>

#include <Prefs\Pref.h>

#include <MInteraction\SimulAnimat.h>
//#include "DlgFormatAnimation.h"


//#include "AnimObjectPage.h"
//#include "AnimObjSizePage.h"
//#include "AnimShapePage.h"
//#include "AnimPictPage.h"
//#include "AnimTextPage.h"
//#include "AnimValuePage.h"
//#include "AnimDensityPage.h"
//#include "AnimLinkPage.h"
#include "AnimSectorPage.h"
//#include "AnimPictDensPage.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// CSectorObject
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CSectorObject, CAnimObject, VERSIONABLE_SCHEMA | 0)

CSectorObject::CSectorObject():
	CAnimObject ()
{
//	SetObjectDef(_T(""));
	m_bIsOpen = TRUE;
	m_bIsLocked = FALSE;
	m_bIsValue = TRUE;

//	m_pTextPage = NULL;
//	m_pValuePage = NULL;
	m_pSectorPage = NULL;

	m_nValueName = -1;
	m_nPrecision = 4;
	m_bDisplayUnit = FALSE;
	m_bPrevTime = FALSE;

	strcpy(m_logFont.lfFaceName, "MS Sans Serif");
	m_logFont.lfWeight=FW_REGULAR;
	m_logFont.lfWidth=0;
	m_logFont.lfHeight=-11;
	m_logFont.lfItalic=FALSE;
	m_logFont.lfOrientation=0;
	m_logFont.lfEscapement=0;
	m_logFont.lfUnderline=FALSE;
	m_logFont.lfStrikeOut=FALSE;
	m_logFont.lfCharSet=ANSI_CHARSET;
	m_logFont.lfOutPrecision=1; //OUT_OUTLINE_PRECIS;
	m_logFont.lfQuality = 1;
	m_logFont.lfClipPrecision= 2;//OUT_DEFAULT_PRECIS; 
	m_logFont.lfPitchAndFamily=34;//FF_MODERN|DEFAULT_PITCH;

	m_clrText = RGB(0,0,0);
	m_strLabel = _T("???");

	//	LOGPEN pLogP;
	//m_pPen.GetLogPen(&pLogP);
	
	//pLogP.lopnColor = RGB(0,0,0);
	//pLogP.lopnWidth.x = 1;
	//m_pPen.DeleteObject();
	//m_pPen.CreatePenIndirect(&pLogP);

	m_rReduced = GetRect();
}

CSectorObject::CSectorObject(int ID):
	CAnimObject ()
{
//	m_ObjectDef.Format(_T("Value n.%d"),m_ObjectID);
//	SetObjectDef(_T(""));
	m_bIsOpen = TRUE;
	m_bIsLocked = FALSE;
	m_bIsValue = TRUE;

//	m_pTextPage = NULL;
//	m_pValuePage = NULL;
	m_pSectorPage = NULL;

	m_nValueName = -1;
	m_nPrecision = 4;
	m_bDisplayUnit = FALSE;
	m_bPrevTime = FALSE;

	strcpy(m_logFont.lfFaceName, "MS Sans Serif");
	m_logFont.lfWeight=FW_REGULAR;
	m_logFont.lfWidth=0;
	m_logFont.lfHeight=-11;
	m_logFont.lfItalic=FALSE;
	m_logFont.lfOrientation=0;
	m_logFont.lfEscapement=0;
	m_logFont.lfUnderline=FALSE;
	m_logFont.lfStrikeOut=FALSE;
	m_logFont.lfCharSet=ANSI_CHARSET;
	m_logFont.lfOutPrecision=1; //OUT_OUTLINE_PRECIS;
	m_logFont.lfQuality = 1;
	m_logFont.lfClipPrecision= 2;//OUT_DEFAULT_PRECIS; 
	m_logFont.lfPitchAndFamily=34;//FF_MODERN|DEFAULT_PITCH;

	m_clrText = RGB(0,0,0);
	m_strLabel = _T("???");

//	LOGPEN pLogP;
//	m_pPen.GetLogPen(&pLogP);
//	
//	pLogP.lopnColor = RGB(0,0,0);
//	pLogP.lopnWidth.x = 1;
//	m_pPen.DeleteObject();
//	m_pPen.CreatePenIndirect(&pLogP);

	m_rReduced = GetRect();
//	SetRect(CRect(50,50,350,350));
//
//	CAnimObject *pObj = new CValueObject(10,CRect(70,70,100,100),-2);
//	AddObjectInSector(pObj);
//	pObj = new CSectorObject();
//	pObj->SetRect(CRect(90,150,190,250));
//	AddObjectInSector(pObj);
}


CSectorObject::CSectorObject(CSectorObject *pObj) : CAnimObject(pObj)
{
	m_bIsOpen = pObj->m_bIsOpen;
	m_bIsLocked = pObj->m_bIsLocked;
	m_logFont = pObj->m_logFont;
	m_clrText = pObj->m_clrText;
	m_strLabel = pObj->m_strLabel;
	m_bIsValue = pObj->m_bIsValue;

	m_nValueName = pObj->m_nValueName;
	m_nPrecision = pObj->m_nPrecision ;
	m_bDisplayUnit = pObj->m_bDisplayUnit ;
	m_bPrevTime = pObj->m_bPrevTime ;

//	m_pTextPage = NULL;
//	m_pValuePage = NULL;
	m_pSectorPage = NULL;

	m_rReduced = pObj->m_rReduced;
	for(int i = 0; i < pObj->m_cSectorObjSet.GetSize();i++ )
	{
		CAnimObject *pNewObj = pObj->m_cSectorObjSet.GetAt(i)->Clone();
		if (pNewObj)
			m_cSectorObjSet.Add(pNewObj);
	}

}

int	 CSectorObject::GetObjectIcon()
{
	return CICON_SECT;
}


CAnimObject* CSectorObject::Clone()
{
	CSectorObject *pObj = new CSectorObject(this);
	return pObj;
}

CSectorObject::~CSectorObject()
{
//	if (m_pTextPage) delete m_pTextPage;
//	if (m_pValuePage) delete m_pValuePage;
	if (m_pSectorPage) delete m_pSectorPage;

	for(int i = 0; i < m_cSectorObjSet.GetSize();i++ )
		delete m_cSectorObjSet.GetAt(i);
	m_cSectorObjSet.RemoveAll();
}

CString CSectorObject::SetObjectType()
{
	CString mstr;
	mstr.Format(_T("Sector n.%d"),m_ObjectID);
	return mstr;
}


/*void CSectorObject::SetObjectDef(CString mstr)
{
	if (mstr.GetLength() == 0)
	{
		m_ObjectDef.Format(_T("Sector %d"),m_ObjectID);
	}
	else
		m_ObjectDef = mstr;
}*/

void CSectorObject::UpdateID(int &nID)
{
	m_ObjectID = nID++;
//	SetObjectDef(_T(""));
	for(int i = 0; i < m_cSectorObjSet.GetSize();i++ )
	{
		CAnimObject *pObj = m_cSectorObjSet.GetAt(i);
		pObj->UpdateID(nID);
		//pObj->m_ObjectID = nID++;
		//pObj->SetObjectDef(_T(""));
	}
}

void CSectorObject::SetExpSet(int nExpSet)
{
	CAnimObject::SetExpSet(nExpSet);
	for(int i = 0; i < m_cSectorObjSet.GetSize();i++ )
	{
		CAnimObject *pObj = m_cSectorObjSet.GetAt(i);
		pObj->SetExpSet(nExpSet);
	}
}

void CSectorObject::SetCurrTime(int nCurrTime)
{
	CAnimObject::SetCurrTime(nCurrTime);
	for(int i = 0; i < m_cSectorObjSet.GetSize();i++ )
	{
		CAnimObject *pObj = m_cSectorObjSet.GetAt(i);
		pObj->SetCurrTime(nCurrTime);
	}
}


void CSectorObject::Serialize(CArchive& ar)
{
	CAnimObject::Serialize(ar);
	if( ar.IsStoring( ) )
	{
		ar << m_bIsOpen;
		ar << m_bIsLocked;
		ar << m_strLabel;
		ar << m_clrText;
		ar << m_rReduced;
//		ar << m_logFont;
		ar << m_bIsValue;
		ar << m_nValueName;
		ar << m_nPrecision;
		ar << m_bDisplayUnit;
		ar << m_bPrevTime;
		CAutoFont mAF(m_logFont);
		CString mstr = mAF.ContractFont();
		ar << mstr;
	}
	else
	{
		ar >> m_bIsOpen;
		ar >> m_bIsLocked;
		ar >> m_strLabel;
		ar >> m_clrText;
		ar >> m_rReduced;
//		ar >> m_logFont;
		ar >> m_bIsValue;
		ar >> m_nValueName;
		ar >> m_nPrecision;
		ar >> m_bDisplayUnit;
		ar >> m_bPrevTime;

		CAutoFont mAF("s");
		CString mstr;
		ar >> mstr;
		mAF.ExtractFont(mstr);
		mAF.GetLogFont(&m_logFont);

	}
	m_cSectorObjSet.Serialize(ar);
}

void CSectorObject::OnUpdateSelection(int nData)
{
	CAnimObject::OnUpdateSelection(nData);
	if (m_bIsOpen) 
	{
		for(int i = 0; i < m_cSectorObjSet.GetSize();i++ )
		{
			CAnimObject *pObj = m_cSectorObjSet.GetAt(i);
			pObj->OnUpdateSelection(nData);
		}
	}

}

void CSectorObject::OnUpdate()
{
	CAnimObject::OnUpdate();
	if (m_bIsOpen) 
	{
		for(int i = 0; i < m_cSectorObjSet.GetSize();i++ )
		{
			CAnimObject *pObj = m_cSectorObjSet.GetAt(i);
			pObj->SetObjectDoc(m_pDoc);
			pObj->m_pSector = this;
			pObj->OnUpdate();
		}
	}

	CString m_sText=_T("####");

	m_currTime = m_pDoc->m_currTimer;
	int nExpSet = m_currExpSet;

	CLearningUnit *pLU= m_pDoc->GetCurrentLU();

	if (m_nValueName >= pLU->GetDataSize())
		m_nValueName = -1;
	if (m_nValueName==-1) return;


	if (m_bIsValue)
	{
		CString form;
		CString name;
		CString unit;
		double m_dValue;
/*		if (m_nValueName==-1) 
		{
			int time = m_currTime;
			if (m_bPrevTime) time--;
			//if (time<0) return;
			if (time<0)
				form = m_sText;
			else
				form.Format(_T("%d"),time);
			name = _T("Time");
			unit = _T("");
		}
		else*/
		{
			CMdData *pData = pLU->GetDataAt(m_nValueName);
			if (!pData) return;

			name = pData->GetDataName(pLU->m_bShowAbbrev);
			CModelObj *pModel = pData->GetDataInfo();
			unit = pModel->GetUnit();
			int time = m_currTime;
			if (m_bPrevTime) time--;
			if (time<0)
				form = m_sText;
			else
			{
				CString dd;
				m_dValue = pData->GetAt(time,nExpSet);
				dd.Format(_T("%d"),m_nPrecision);
				form = "%." + dd + "f";
			}
			//form.Format(_T("%g"),m_iPrecision);
		}
		m_sText.Format(form,m_dValue);
		m_sText = name + _T(" : ") + m_sText;
		if (m_bDisplayUnit)
		{
			m_sText += _T(" ") + unit;
		}
		m_strLabel = m_sText;
	}
//	else
//	{
//	}
}

void CSectorObject::Draw(CDC* pDC,BOOL bParent)
{
	CAnimObject::Draw (pDC);

	CFont font;
	CFont *pOldFont=NULL;
	if (font.CreateFontIndirect(&m_logFont))
	{
		pOldFont=(CFont *)pDC->SelectObject(&font);
	}

	int oldMode=pDC->SetBkMode(TRANSPARENT);

	CBrush	mTitleBrush(::GetSysColor(COLOR_3DFACE));
	CPen mActivePen(PS_SOLID,1,::GetSysColor(COLOR_BTNTEXT));
	CPen mInactivePen(PS_SOLID,1,::GetSysColor(COLOR_BTNSHADOW));

	UINT nFormat = DT_SINGLELINE | DT_EXPANDTABS | DT_END_ELLIPSIS | DT_LEFT | DT_TOP;

	CRect txtRect = GetRect();
	txtRect.DeflateRect(2,2,39,2);
	int nHeight = pDC->DrawText( m_strLabel, txtRect, DT_CALCRECT | nFormat);
	nHeight = max(nHeight,16);
	//tmpRect = mrect;
	CRect tmpRect = GetRect();
	tmpRect.bottom = tmpRect.top+nHeight+4;
	tmpRect.right ++;

	CRect closeRect = tmpRect;
	closeRect.top += 3;
	closeRect.bottom = closeRect.top +14;
	closeRect.right -= 5;
	closeRect.left = closeRect.right - 16;
	m_rOBtn = closeRect;


	CBrush *oldBrush = pDC->SelectObject(&mTitleBrush);
	pDC->Rectangle(tmpRect);
	pDC->SelectObject(oldBrush);

	UINT nState = DFCS_BUTTONPUSH;
	CPen *oldPen=NULL;
	if (m_bIsOpen) 
	{
		nState |= DFCS_INACTIVE;
		oldPen = pDC->SelectObject(&mInactivePen);
	}
	else
		oldPen = pDC->SelectObject(&mActivePen);

	pDC->DrawFrameControl(closeRect,DFC_BUTTON,nState);
	pDC->MoveTo(closeRect.left+3,closeRect.top+2);
	pDC->LineTo(closeRect.right-5,closeRect.top+2);
	pDC->LineTo(closeRect.right-5,closeRect.bottom-4);
	pDC->LineTo(closeRect.left+3,closeRect.bottom-4);
	pDC->LineTo(closeRect.left+3,closeRect.top+3);
	pDC->LineTo(closeRect.right-5,closeRect.top+3);

	closeRect.OffsetRect(-16,0);
	m_rCBtn = closeRect;
	nState = DFCS_BUTTONPUSH;
	if (!m_bIsOpen) 
	{
		nState |= DFCS_INACTIVE;
		pDC->SelectObject(&mInactivePen);
	}
	else
		pDC->SelectObject(&mActivePen);
	pDC->DrawFrameControl(closeRect,DFC_BUTTON,nState);
	pDC->MoveTo(closeRect.left+4,closeRect.bottom-4);
	pDC->LineTo(closeRect.right-6,closeRect.bottom-4);
	pDC->MoveTo(closeRect.left+4,closeRect.bottom-5);
	pDC->LineTo(closeRect.right-6,closeRect.bottom-5);
	pDC->SelectObject(oldPen);

    COLORREF oldColor=pDC->SetTextColor(m_clrText);
	pDC->DrawText( m_strLabel, txtRect, nFormat);
    pDC->SetTextColor(oldColor);

	if (m_bIsOpen)
	{
		CPen mpen(PS_SOLID,1,RGB(0,0,0));
		CBrush mbrush(RGB(0,0,0));
		oldPen = pDC->SelectObject(&mpen);
		oldBrush = pDC->SelectObject(&mbrush);

		CRect mrect = GetRect();
		mrect = CRect(mrect.right+1,mrect.top+5,mrect.right+5,mrect.bottom+5);
		pDC->Rectangle(mrect);
		mrect = GetRect();
		mrect = CRect(mrect.left+5,mrect.bottom+1,mrect.right+5,mrect.bottom+5);
		pDC->Rectangle(mrect);
		pDC->SelectObject(oldBrush);
		pDC->SelectObject(oldPen);
	}

	if (m_bIsOpen) 
	{
		for(int i = 0; i < m_cSectorObjSet.GetSize();i++ )
		{
			CAnimObject *pObj = m_cSectorObjSet.GetAt(i);
			pObj->SetObjectDoc(m_pDoc);
			pObj->Draw(pDC);
		}
	}
	pDC->SetBkMode(oldMode);
	if (pOldFont)
		pDC->SelectObject(pOldFont);

}

int CSectorObject::OnEditProperty(CView *pView,int nShowSheet)
{
	return CAnimObject::OnEditProperty(pView,nShowSheet);
}

void CSectorObject::SetPropSheetPages(CPropertySheet *pSheet,CxAnimObjectList *pList)
{
//	if (!m_pTextPage)
//		m_pTextPage = new CAnimTextPage();
//	if (m_pValuePage)
//		m_pValuePage = new CAnimValuePage();
	if (!m_pSectorPage)
		m_pSectorPage = new CAnimSectorPage();

	SetPropSheetShape(pSheet,pList);
//	m_pTextPage->m_TextInput = m_strLabel;
//	m_pTextPage->m_lfDefFont = m_logFont;
//	m_pTextPage->m_color = m_clrText;
//	m_pTextPage->m_Align = 0;

	m_pSectorPage->m_lfDefFont = m_logFont;
	m_pSectorPage->m_color = m_clrText;

	m_pSectorPage->m_sVarList.RemoveAll();
	CLearningUnit *pLU= m_pDoc->GetCurrentLU();
	int nb = pLU->GetDataSize();
	for (int i=0;i<nb;i++)
	{
		CMdData *pData = pLU->GetDataAt(i);
		CString mstr = pData->GetDataName(pLU->m_bShowAbbrev);
		m_pSectorPage->m_sVarList.Add(mstr);
	}
	m_pSectorPage->m_bTitleText = m_bIsValue;
	if (m_bIsValue)
	{
		m_pSectorPage->m_nValueID = m_nValueName+1;
		m_pSectorPage->m_sTitle = _T("");
	}
	else
	{
		m_pSectorPage->m_nValueID = -1;
		m_pSectorPage->m_sTitle = m_strLabel;
	}
	m_pSectorPage->m_bDisplayUnit = m_bDisplayUnit;
	m_pSectorPage->m_bLocked = m_bIsLocked;
	m_pSectorPage->m_bPrevTime =m_bPrevTime;
	m_pSectorPage->m_nPrec = m_nPrecision;

	CImageList pImageList;
	pImageList.Create(IDB_ANIM_CPNTPAGE,16,0,RGB(192,192,192));

	m_pSectorPage->m_psp.dwFlags |= PSP_USEHICON ;
	m_pSectorPage->m_psp.hIcon = pImageList.ExtractIcon(10);


	pSheet->AddPage(m_pSectorPage);

	//	pSheet->AddPage(m_pTextPage);
	CAnimObject::SetPropSheetPages(pSheet,pList);
//	pSheet->AddPage(m_pShapPage);
	pSheet->SetActivePage(m_pSectorPage);
}

void CSectorObject::ModifyObjectProperty()
{
	CAnimObject::ModifyObjectProperty();
//	m_strLabel = m_pTextPage->m_TextInput;
	m_logFont = m_pSectorPage->m_lfDefFont;
	m_clrText = m_pSectorPage->m_color;

	m_bIsValue = (m_pSectorPage->m_bTitleText);
	if (m_bIsValue)
	{
		m_nValueName = m_pSectorPage->m_nValueID-1;
		m_strLabel = _T("");
	}
	else
	{
		m_nValueName = -1;
		m_strLabel = m_pSectorPage->m_sTitle;
	}
	m_bDisplayUnit = m_pSectorPage->m_bDisplayUnit;
	m_bIsLocked = m_pSectorPage->m_bLocked;
	m_bPrevTime = m_pSectorPage->m_bPrevTime;
	m_nPrecision = m_pSectorPage->m_nPrec;

}

void CSectorObject::DisposeProperty()
{
	CAnimObject::DisposeProperty();
	if (m_pSectorPage) 
	{
		delete m_pSectorPage;
		m_pSectorPage=NULL;
	}
}

void CSectorObject::SetRect (CRect lpSrcRect)
{
	CRect mrect = GetRect();
	CSize dPos = lpSrcRect.TopLeft()-mrect.TopLeft();

	CAnimObject::SetRect(lpSrcRect);
	for (int i=0;i<m_cSectorObjSet.GetSize();i++)
	{
		CAnimObject *pObj = m_cSectorObjSet.GetAt(i);
		CRect locRect = pObj->GetRect();
		locRect.OffsetRect(dPos);
		pObj->SetRect(locRect);
	}

}

CRect CSectorObject::GetRect ()
{
	return CAnimObject::GetRect();
	//CRect mrect = (m_bIsOpen) ? CAnimObject::GetRect () : m_rReduced ;
	//return mrect;
}

void CSectorObject::MoveRect (CRect lpSrcRect)
{
	CRect mrect = GetRect();
	CSize dPos = lpSrcRect.TopLeft()-mrect.TopLeft();
	//CRect mrect = CAnimObject::GetRect ();
	CAnimObject::MoveRect(lpSrcRect);
	//CSize msize = lpSrcRect.TopLeft()-mrect.TopLeft();
	//m_rReduced.OffsetRect(msize);
	for (int i=0;i<m_cSectorObjSet.GetSize();i++)
	{
		CAnimObject *pObj = m_cSectorObjSet.GetAt(i);
		CRect locRect = pObj->GetRect();
		locRect.OffsetRect(dPos);
		locRect = lpSrcRect;
		pObj->MoveRect(locRect);
	}
}

void CSectorObject::ResizeRect(CSize lpSize)
{
	CAnimObject::ResizeRect (lpSize);
	//m_rReduced.InflateRect(0,0,lpSize.cx,lpSize.cy);
}



BOOL CSectorObject::AddObjectInSector(CAnimObject *pObj)
{
	if (!pObj) return FALSE;

	CPoint mPt = GetRect().TopLeft();
	mPt.Offset(15,30);
	pObj->m_nDDPos.m_ptDefPos = mPt;

	m_cSectorObjSet.Add(pObj);
	pObj->m_pSector = this;
	return TRUE;
}

BOOL CSectorObject::RemoveObjectFromSector(CAnimObject *pObj)
{
	if (!pObj) return FALSE;
	for (int i=0;i<m_cSectorObjSet.GetSize();i++)
	{
		CAnimObject *pNObj = m_cSectorObjSet.GetAt(i);
		if (pNObj != pObj) continue;
		m_cSectorObjSet.RemoveAt(i);
		pNObj->m_pSector = NULL;
	}
	return TRUE;;
}

CAnimObject* CSectorObject::GetFromSectorAt(int i)
{
	CAnimObject *pObj = NULL;
	if ((i >= 0) && (i < m_cSectorObjSet.GetSize()))
		pObj = m_cSectorObjSet.GetAt(i);
	return pObj;
}

BOOL CSectorObject::IsCloseBtnHit(CPoint pt)
{
	if (!m_bIsOpen) return FALSE;

	return m_rCBtn.PtInRect(pt);
}

BOOL CSectorObject::IsOPenBtnHit(CPoint pt)
{
	if (m_bIsOpen) return FALSE;

	return m_rOBtn.PtInRect(pt);
}

void CSectorObject::ToggleState()
{
	CRect tmpRect = GetRect();
	CSize dPos = tmpRect.TopLeft()-m_rReduced.TopLeft();
	m_ObjRect = m_rReduced;
	m_rReduced = tmpRect;
	m_ObjRect.OffsetRect(dPos);
	m_bIsOpen = !m_bIsOpen;
}

CAnimObject* CSectorObject::OnHitTest(CPoint pt,CxAnimObjectList *pList)
{
	CAnimObject* pObj = CAnimObject::OnHitTest(pt,pList);
	if (/*pObj && */m_bIsOpen)
	{
		for (int i=0;i<m_cSectorObjSet.GetSize();i++)
		{
			CAnimObject *pNObj = m_cSectorObjSet.GetAt(i);
			if (!pNObj) continue;
			CAnimObject *pRes = pNObj->OnHitTest(pt,pList);
			if (pRes) 
				pObj = pRes;
		}
	}
	return pObj;
}

CAnimObject* CSectorObject::OnHitTest(CRect rct,CxAnimObjectList *pList)
{
	CAnimObject* pObj = CAnimObject::OnHitTest(rct,pList);
	if (/*pObj &&*/ m_bIsOpen)
	{
		for (int i=0;i<m_cSectorObjSet.GetSize();i++)
		{
			CAnimObject *pNObj = m_cSectorObjSet.GetAt(i);
			if (!pNObj) continue;
			CAnimObject *pRes = pNObj->OnHitTest(rct,pList);
			if (pRes) 
				pObj = pRes;
		}
	}
	return pObj;
}
