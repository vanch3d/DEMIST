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

//#include <MInteraction\SimulAnimat.h>
//#include "DlgFormatAnimation.h"

//#include "AnimObjectPage.h"
//#include "AnimObjSizePage.h"
//#include "AnimShapePage.h"
//#include "AnimPictPage.h"
#include "AnimTextPage.h"
#include "AnimValuePage.h"
//#include "AnimDensityPage.h"
//#include "AnimLinkPage.h"
//#include "AnimSectorPage.h"
//#include "AnimPictDensPage.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// CTextObject
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CTextObject, CAnimObject, VERSIONABLE_SCHEMA | 3)

CTextObject::CTextObject(int ID,const CRect& position, CString mtext):
	CAnimObject (ID,position), 
	m_sText(mtext) 
{
	m_pTextPage = 0;
//	m_ObjectDef.Format(_T("Text n.%d"),m_ObjectID);
//	SetObjectDef(_T(""));
	strcpy(m_logFont.lfFaceName, "Times New Roman");
	m_logFont.lfWeight=FW_REGULAR;
	m_logFont.lfWidth=0;
	m_logFont.lfHeight=-16;
	m_logFont.lfItalic=FALSE;
	m_logFont.lfOrientation=0;
	m_logFont.lfEscapement=0;
	m_logFont.lfUnderline=FALSE;
	m_logFont.lfStrikeOut=FALSE;
	m_logFont.lfOutPrecision=OUT_OUTLINE_PRECIS;
	m_logFont.lfCharSet=ANSI_CHARSET;
	m_logFont.lfOutPrecision=OUT_DEFAULT_PRECIS; 
	m_logFont.lfPitchAndFamily=FF_MODERN|DEFAULT_PITCH;

	m_clrText = RGB(0,0,0);
	m_nAlign = 0;
	SetObjectColor();
}

CTextObject::CTextObject():
	CAnimObject (), 
	m_sText("") 
{
	m_pTextPage = 0;
//	m_ObjectDef.Format(_T("Text n.%d"),m_ObjectID);
//	SetObjectDef(_T(""));
	strcpy(m_logFont.lfFaceName, "Times New Roman");
	m_logFont.lfWeight=FW_REGULAR;
	m_logFont.lfWidth=0;
	m_logFont.lfHeight=-16;
	m_logFont.lfItalic=FALSE;
	m_logFont.lfOrientation=0;
	m_logFont.lfEscapement=0;
	m_logFont.lfUnderline=FALSE;
	m_logFont.lfStrikeOut=FALSE;
	m_logFont.lfOutPrecision=OUT_OUTLINE_PRECIS;
	m_logFont.lfCharSet=ANSI_CHARSET;
	m_logFont.lfOutPrecision=OUT_DEFAULT_PRECIS; 
	m_logFont.lfPitchAndFamily=FF_MODERN|DEFAULT_PITCH;

	m_clrText = RGB(0,0,0);
	m_nAlign = 0;
	SetObjectColor();
}

CTextObject::CTextObject (CTextObject *pObj) : CAnimObject (pObj) 
{
	m_sText = pObj->m_sText;
	m_logFont = pObj->m_logFont;
	m_clrText = pObj->m_clrText;
	m_nAlign = pObj->m_nAlign;
	m_pTextPage = NULL;
}


CAnimObject* CTextObject::Clone()
{
	CTextObject *pObj = new CTextObject(this);
	return pObj;
}

int	 CTextObject::GetObjectIcon()
{
	return CICON_TEXT;
}


CTextObject::~CTextObject()
{
	if (m_pTextPage) delete m_pTextPage;
}


CString CTextObject::SetObjectType()
{
	CString mstr;
	mstr.Format(_T("Text n.%d"),m_ObjectID);
	return mstr;
}

/*void CTextObject::SetObjectDef(CString mstr)
{
	if (mstr.GetLength() == 0)
	{
		m_ObjectDef.Format(_T("Text n.%d"),m_ObjectID);
	}
	else
		m_ObjectDef = mstr;
}*/

void CTextObject::Serialize(CArchive& ar)
{
	CAnimObject::Serialize(ar);
	if( ar.IsStoring( ) )
	{
		ar << m_sText;
		CAutoFont mAF(m_logFont);
		CString mstr = mAF.ContractFont();
		ar << mstr;
		ar << m_clrText;
		ar << m_nAlign;
	}
	else
	{
		int nb = ar.GetObjectSchema();
//		if (nb >= 1)
		{
			ar >> m_sText;
//			ar >> m_logFont;
		}
//		if (nb >= 2)
		{
			CAutoFont mAF("DSDSDS");
			CString mstr;
			ar >> mstr;
			mAF.ExtractFont(mstr);
			mAF.GetLogFont(&m_logFont);
			ar >> m_clrText;
		}
//		if (nb >= 3)
		{
			ar >> m_nAlign;
		}
	}
}


void CTextObject::Draw (CDC* pDC,BOOL bParent)
{
	CFont font;

	if (!m_pBrush.GetSafeHandle() || !m_pPen.GetSafeHandle())
		OnInitialUpdate();

	if (!font.CreateFontIndirect(&m_logFont))
		return;
    CFont *pOldFont=(CFont *)pDC->SelectObject(&font);
	int x=(m_ObjRect.left < m_ObjRect.right)? m_ObjRect.left: m_ObjRect.right;
	int y=(m_ObjRect.top > m_ObjRect.bottom)? m_ObjRect.top:m_ObjRect.bottom;
	
	LOGPEN pLogPen;
	int ff = m_pPen.GetLogPen(&pLogPen);
	//calculate the x, y
	int dy=abs(m_ObjRect.top - m_ObjRect.bottom);
	int dx=abs(m_ObjRect.left -m_ObjRect.right);
	int lx= TextLen(pDC);//length of your string;
    int ly= MaxHeight(pDC);//hight of the string;

    /*switch(m_allignment){
	case 0: //middle
		x+=(dx-lx)/2;
		y-=(dy-ly)/2;
		break;
	case 1: //left
		x+=m_logpen.lopnWidth.x + PADDING;
		y-=(dy-ly)/2;
		//y-=m_logpen.lopnWidth.x + PADDING;
		break;
	case 2: //right
		x+=dx-lx-PADDING-m_logpen.lopnWidth.x;
		y-=(dy-ly)/2;
		//y-=(dy-ly+PADDING+m_logpen.lopnWidth.x);
		break;
	default: //middle
		x+=(dx-lx)/2;
		y-=(dy-ly)/2;
		break;
	}*/
/*	LOGPEN m_logpen;

	m_logpen.lopnStyle = PS_SOLID;
	m_logpen.lopnWidth.x = 1;
	m_logpen.lopnWidth.y = 1;
	m_logpen.lopnColor = RGB(0, 0, 0);*/

	x+=(dx-lx)/2;
	y-=(dy+ly)/2;

	int oldMode;
	oldMode=pDC->SetBkMode(TRANSPARENT);

	//COLORREF clrBkOld = dc.SetBkColor( m_clrBkGnd );
	//pFontOld = dc.SelectObject( &font );

    //COLORREF oldColor2=pDC->SetTextColor(pLogPen.lopnColor);
    COLORREF oldColor2=pDC->SetTextColor(m_clrText);

	UINT nFormat = DT_WORDBREAK | DT_EXPANDTABS | DT_VCENTER;
	if (m_nAlign == 1) nFormat |= DT_LEFT;
	else if (m_nAlign == 3) nFormat |= DT_RIGHT;
	else /*if (m_nAlign == 2)*/ nFormat |= DT_CENTER;

	CRect tmpRect = m_ObjRect;
	CRect tmpRect2 = m_ObjRect;
	tmpRect = GetRect();
	int nHeight = pDC->DrawText( m_sText, &tmpRect, DT_CALCRECT | nFormat);
	if (!m_nAlign)
	{
		tmpRect2 = tmpRect;
	}
	else
	{
		tmpRect2 |= tmpRect;
	}
	SetRect(tmpRect2);
	CAnimObject::Draw (pDC);

	pDC->DrawText( m_sText, m_ObjRect, nFormat);
	//SetRect(m_ObjRect);
//	pDC->ExtTextOut(x,y,ETO_OPAQUE,NULL,m_sText,NULL);

    pDC->SetTextColor(oldColor2);
    pDC->SelectObject(pOldFont);
	pDC->SetBkMode(oldMode);
/*	CRectTracker tracker;
	tracker.m_nStyle = CRectTracker::dottedLine | 	CRectTracker::resizeOutside  ;
	tracker.m_rect = m_ObjRect;
	tracker.Draw(pDC);*/
}

int CTextObject::TextLen(CDC *pDC)
{
	CFont font;
	if (!font.CreateFontIndirect(&m_logFont))
		return 0;
    CFont *pOldFont=(CFont *)pDC->SelectObject(&font);
	CSize sz=pDC->GetTextExtent(m_sText);
    pDC->SelectObject(pOldFont);
	return sz.cx;
}

int CTextObject::MaxHeight(CDC *pDC)
{
	CFont font;
	TEXTMETRIC TM;
	if (!font.CreateFontIndirect(&m_logFont))
		return 0;
    CFont *pOldFont=(CFont *)pDC->SelectObject(&font);
	pDC->GetTextMetrics(&TM);
    int x=TM.tmHeight+TM.tmExternalLeading;	
    pDC->SelectObject(pOldFont);
    return x;
}

void CTextObject::SetPropSheetPages(CPropertySheet *pSheet,CxAnimObjectList *pList)
{
	if (!m_pTextPage)
		m_pTextPage = new CAnimTextPage();

	((CAnimTextPage*)m_pTextPage)->m_TextInput = m_sText;
	((CAnimTextPage*)m_pTextPage)->m_lfDefFont = m_logFont;
	((CAnimTextPage*)m_pTextPage)->m_color = m_clrText;
	((CAnimTextPage*)m_pTextPage)->m_Align = m_nAlign;

	CImageList pImageList;
	pImageList.Create(IDB_ANIM_CPNTPAGE,16,0,RGB(192,192,192));

	m_pTextPage->m_psp.dwFlags |= PSP_USEHICON ;
	m_pTextPage->m_psp.hIcon = pImageList.ExtractIcon(0);

	pSheet->AddPage(m_pTextPage);

	CAnimObject::SetPropSheetPages(pSheet,pList);
	pSheet->SetActivePage(m_pTextPage);
}

void CTextObject::ModifyObjectProperty()
{
	CAnimObject::ModifyObjectProperty();
	m_sText = ((CAnimTextPage*)m_pTextPage)->m_TextInput;
	m_logFont = ((CAnimTextPage*)m_pTextPage)->m_lfDefFont;
	m_clrText = ((CAnimTextPage*)m_pTextPage)->m_color;
	m_nAlign = ((CAnimTextPage*)m_pTextPage)->m_Align;
}

void CTextObject::DisposeProperty()
{
	CAnimObject::DisposeProperty();
	if (m_pTextPage) 
	{
		delete m_pTextPage;
		m_pTextPage=NULL;
	}
}

//////////////////////////////////////////////////////////////////////
// CValueObject
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CValueObject, CTextObject, VERSIONABLE_SCHEMA | 2)

CValueObject::CValueObject():
	CTextObject ()
{
//	m_ObjectDef.Format(_T("Value n.%d"),m_ObjectID);
//	SetObjectDef(_T(""));
	m_iValueName = -1;
	m_dValue=0.0;
	m_iPrecision=4;
	m_bDisplayUnit = m_bDisplayName = FALSE;
	m_bPrevTime = FALSE;
}

CValueObject::CValueObject(int ID,const CRect& position, int vName):
	CTextObject (ID,position,"####"), 
	m_iValueName(vName)
{
//	m_ObjectDef.Format(_T("Value n.%d"),m_ObjectID);
//	SetObjectDef(_T(""));
	m_dValue=0.0;
	m_iPrecision=4;
	m_bDisplayName = FALSE;
	m_bDisplayUnit = FALSE;
	m_bPrevTime = FALSE;
}


CValueObject::CValueObject(CValueObject *pObj) : CTextObject(pObj)
{
	m_iValueName = pObj->m_iValueName;
	m_dValue=pObj->m_dValue;
	m_iPrecision=pObj->m_iPrecision;
	m_bDisplayName = pObj->m_bDisplayName;
	m_bDisplayUnit = pObj->m_bDisplayUnit;
	m_bPrevTime = pObj->m_bPrevTime;
}

int	 CValueObject::GetObjectIcon()
{
	return CICON_VAL;
}

CAnimObject* CValueObject::Clone()
{
	CValueObject *pObj = new CValueObject(this);
	return pObj;
}

CValueObject::~CValueObject()
{
}

CString CValueObject::SetObjectType()
{
	CString mstr;
	mstr.Format(_T("Value n.%d"),m_ObjectID);
	return mstr;
}


/*void CValueObject::SetObjectDef(CString mstr)
{
	if (mstr.GetLength() == 0)
	{
		m_ObjectDef.Format(_T("Value n.%d"),m_ObjectID);
	}
	else
		m_ObjectDef = mstr;
}*/

void CValueObject::Serialize(CArchive& ar)
{
	CTextObject::Serialize(ar);
	if( ar.IsStoring( ) )
	{
		ar << m_iValueName;
		ar << m_dValue;
		ar << m_iPrecision;
		ar << m_bDisplayName;
		ar << m_bDisplayUnit;
		ar << m_bPrevTime;
	}
	else
	{
//		if (ar.GetObjectSchema() >= 1)
		{
			ar >> m_iValueName;
			ar >> m_dValue;
			ar >> m_iPrecision;
			ar >> m_bDisplayName;
			ar >> m_bDisplayUnit;
			ar >> m_bPrevTime;
		}
	}
}


void CValueObject::OnUpdateHypothesis(CUserData *pData)
{
	CAnimObject::OnUpdateHypothesis(pData);

	if (!pData)
	{
		m_sText="####";
		return;
	}

	CLearningUnit *pLU= m_pDoc->GetCurrentLU();
	CMdEquation* pEq = m_pDoc->GetCurrentModel();
	int nPar = pEq->m_cParSet.GetSize();
	CMdData *pModData = pLU->GetDataAt(m_iValueName);
	CModelObj *pModel = pModData->GetDataInfo();

	if (pModel->GetIcon()==1 || pModel->GetIcon()==2)
	{
		CMdData *pRData = pLU->GetDataAt(m_iValueName);

		CString dd,form;
		CString name;
		CString unit;

		name = pModData->GetDataName(pLU->m_bShowAbbrev);
		unit = pModel->GetUnit();

		dd.Format(_T("%d"),m_iPrecision);
		form = "%." + dd + "f";
		m_dValue = pRData->GetAt(m_iValueName,pData->m_nExpSet);
		m_sText.Format(form,m_dValue);
		if (m_bDisplayName)
		{
			m_sText = name + _T(" : ") + m_sText;
		}
		if (m_bDisplayUnit)
		{
			m_sText += _T(" ") + unit;
		}
	}
	else if ((pData->m_nOutcome == m_iValueName && 
				pData->m_nExpSet == m_currExpSet)|| 
				m_iValueName==nPar)
	{
		CString dd,form;
		CString name;
		CString unit;

		if (pData) name = pModData->GetDataName(pLU->m_bShowAbbrev);
		unit = pModel->GetUnit();

		dd.Format(_T("%d"),m_iPrecision);
		form = "%." + dd + "f";
		if (m_iValueName==nPar)
			m_dValue = pData->m_nTime;
		else
			m_dValue = pData->m_dValue;
		m_sText.Format(form,m_dValue);
		if (m_bDisplayName)
		{
			m_sText = name + _T(" : ") + m_sText;
		}
		if (m_bDisplayUnit)
		{
			m_sText += _T(" ") + unit;
		}
	}
}


void CValueObject::OnUpdate()
{
	CAnimObject::OnUpdate();
	m_sText="####";

	//m_currTime = m_pDoc->m_currTimer;
	int nExpSet = m_currExpSet;

	CLearningUnit *pLU= m_pDoc->GetCurrentLU();

	if (m_iValueName >= pLU->GetDataSize())
		m_iValueName = -1;
	if (m_iValueName==-1) return;

	CString form;
	CString name;
	CString unit;
/*	if (m_iValueName==-1) 
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
		CMdData *pData = pLU->GetDataAt(m_iValueName);
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
			if (m_dValue == (int)m_dValue)
				dd.Format(_T("%d"),0);
			else
				dd.Format(_T("%d"),m_iPrecision);
			form = "%." + dd + "f";
		}
		//form.Format(_T("%g"),m_iPrecision);
	}

	m_sText.Format(form,m_dValue);
	if (m_bDisplayName)
	{
		m_sText = name + _T(" : ") + m_sText;
	}
	if (m_bDisplayUnit)
	{
		m_sText += _T(" ") + unit;
	}
}

void CValueObject::SetPropSheetPages(CPropertySheet *pSheet,CxAnimObjectList *pList)
{
	if (!m_pTextPage)
		m_pTextPage = new CAnimValuePage();

	((CAnimValuePage*)m_pTextPage)->m_sVarList.RemoveAll();
	CLearningUnit *pLU= m_pDoc->GetCurrentLU();
	int nb = pLU->GetDataSize();
	for (int i=0;i<nb;i++)
	{
		CMdData *pData = pLU->GetDataAt(i);
		CString mstr = pData->GetDataName(pLU->m_bShowAbbrev);
		((CAnimValuePage*)m_pTextPage)->m_sVarList.Add(mstr);
		((CAnimValuePage*)m_pTextPage)->m_sOutcomeIcon.Add(pData->GetDataInfo()->GetIcon());
	}

	((CAnimValuePage*)m_pTextPage)->m_iPrecision= m_iPrecision;
	((CAnimValuePage*)m_pTextPage)->m_lfDefFont = m_logFont;
	((CAnimValuePage*)m_pTextPage)->m_color = m_clrText;
	((CAnimValuePage*)m_pTextPage)->m_vValue = m_iValueName+1;
	((CAnimValuePage*)m_pTextPage)->m_Align = m_nAlign;
	((CAnimValuePage*)m_pTextPage)->m_bDisplayName = m_bDisplayName;
	((CAnimValuePage*)m_pTextPage)->m_bDisplayUnit = m_bDisplayUnit;
	((CAnimValuePage*)m_pTextPage)->m_bPrevTime = m_bPrevTime;

	CImageList pImageList;
	pImageList.Create(IDB_ANIM_CPNTPAGE,16,0,RGB(192,192,192));

	m_pTextPage->m_psp.dwFlags |= PSP_USEHICON ;
	m_pTextPage->m_psp.hIcon = pImageList.ExtractIcon(1);

	pSheet->AddPage(m_pTextPage);

	CAnimObject::SetPropSheetPages(pSheet,pList);
	pSheet->SetActivePage(m_pTextPage);
}

void CValueObject::ModifyObjectProperty()
{
	CAnimObject::ModifyObjectProperty();

	m_iPrecision =((CAnimValuePage*)m_pTextPage)->m_iPrecision;
	m_logFont = ((CAnimValuePage*)m_pTextPage)->m_lfDefFont;
	m_iValueName = ((CAnimValuePage*)m_pTextPage)->m_vValue -1;
	m_clrText = ((CAnimValuePage*)m_pTextPage)->m_color;
	m_nAlign = ((CAnimValuePage*)m_pTextPage)->m_Align;
	m_bDisplayName = ((CAnimValuePage*)m_pTextPage)->m_bDisplayName;
	m_bDisplayUnit = ((CAnimValuePage*)m_pTextPage)->m_bDisplayUnit;
	m_bPrevTime = ((CAnimValuePage*)m_pTextPage)->m_bPrevTime;
//	OnUpdate();
}



void CValueObject::OnUpdateSelection(int nData)
{
	CTextObject::OnUpdateSelection(nData);
	if (nData==-1)
		m_bIsSelected = FALSE;
	else if (nData == m_iValueName)
		m_bIsSelected = TRUE;

	//m_bIsSelected = (nData == m_iValueName);
}

BOOL CValueObject::OnDoEdit(CPoint pt,int& nOutcome,double& nval,double& oval,CWnd* pParent)
{
	//CxAnimObjectList pList;
	//CAnimObject* pObj = OnHitTest(pt,&pList);
	//if (!pList.GetSize()) return FALSE;
	CLearningUnit *pLU= m_pDoc->GetCurrentLU();
	CMdData *pData = pLU->GetDataAt(m_iValueName);
	if (!pData) return FALSE;

	CModelObj* pObj = pData->GetDataInfo();
	if (!pObj || (pObj->GetIcon()>2)) return FALSE;

	CModelPar *pParObj = DYNAMIC_DOWNCAST( CModelPar, pObj);

	////// Edit Value
	CEditValueDlg pDlg(pParent,pt);
	CString name;

	name = pData->GetDataName(pLU->m_bShowAbbrev);
	pDlg.m_strName = name;
	pDlg.m_strOldV.Format(_T("%.4g"),m_dValue);
	pDlg.m_strValue.Format(_T("%.4g"),m_dValue);
	if (pParObj)
	{
		pDlg.m_dMin = pParObj->GetMin();
		pDlg.m_dMax = pParObj->GetMax();
	}
	if (pDlg.DoModal()==IDOK)
	{
		CString mstr = pDlg.m_strValue;
		double nVal = atof(mstr.GetBuffer(mstr.GetLength()));
		oval = m_dValue;
		m_dValue = nval = nVal;
		nOutcome = m_iValueName;
		return TRUE;
	}
	return FALSE;
}

BOOL CValueObject::OnDoTranslation(CPoint pt,int& nOutcome)
{
	BOOL bRedraw = FALSE;

//	CxAnimObjectList pList;
//	CAnimObject* pObj = OnHitTest(pt,&pList);
//	if (pList.GetSize())
//	{
//		if (!m_bIsSelected)
//		{
			m_bIsSelected = TRUE;
			nOutcome = m_iValueName;
			bRedraw = TRUE;
//		}
//	}
/*	else
	{
		if (m_bIsSelected)
		{
			m_bIsSelected = FALSE;
			bRedraw = TRUE;
			//nOutcome = m_iValueName;
		}
	}*/
	return bRedraw;
}
