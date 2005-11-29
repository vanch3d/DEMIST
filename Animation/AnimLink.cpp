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

#include "AnimLinkPage.h"
#include "AnimPolygonPage.h"
#include "AnimShapePage.h"
#include "Tools\HelpPrSheet.h"


#include <MInteraction\Format.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// CLinkObject
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CLinkObject, CAnimObject, VERSIONABLE_SCHEMA | 0)

CLinkObject::CLinkObject():
	CAnimObject ()
{
//	m_ObjectDef.Format(_T("Value n.%d"),m_ObjectID);
	//SetObjectDef(_T(""));

	m_nIDInit = m_nIDEnd = -1;
	m_pInit = m_pEnd = NULL;
	m_pLinkPage = NULL;
	m_nArrowStyle = 0;
	m_bArrInit = m_bArrEnd = 0;
	m_ptInit = GetRect().TopLeft();
	m_ptEnd = GetRect().BottomRight();

	//LOGPEN pLogP;
	//m_pPen.GetLogPen(&pLogP);
	//
	//pLogP.lopnColor = RGB(0,64,64);
	//pLogP.lopnWidth.x = 3;
	//m_pPen.DeleteObject();
	//m_pPen.CreatePenIndirect(&pLogP);
	SetObjectColor(RGB(0,64,64),3);
	m_bAttEnd = m_bAttInit = 0;
}

CLinkObject::CLinkObject(int ID):	CAnimObject (ID,CRect(100,100,150,150))
{
//	m_ObjectDef.Format(_T("Value n.%d"),m_ObjectID);
//	SetObjectDef(_T(""));
	m_nIDInit = m_nIDEnd = -1;
	m_pInit = m_pEnd = NULL;
	m_pLinkPage = NULL;
	m_nArrowStyle = 0;
	m_bArrInit = m_bArrEnd = 0;
	m_ptInit = GetRect().TopLeft();
	m_ptEnd = GetRect().BottomRight();
	m_bAttEnd = m_bAttInit = 0;

//	LOGPEN pLogP;
//	m_pPen.GetLogPen(&pLogP);
//	
//	pLogP.lopnColor = RGB(0,64,64);
//	pLogP.lopnWidth.x = 3;
//	m_pPen.DeleteObject();
//	m_pPen.CreatePenIndirect(&pLogP);
	SetObjectColor(RGB(0,64,64),3);
}


CLinkObject::CLinkObject(CLinkObject *pObj) : CAnimObject(pObj)
{
	m_nIDInit = pObj->m_nIDInit;
	m_nIDEnd = pObj->m_nIDEnd;
	m_pInit = m_pEnd = NULL;
	m_pLinkPage = NULL;
	m_nArrowStyle = pObj->m_nArrowStyle;
	m_bArrInit = m_bArrEnd = 0;
	m_ptInit = pObj->m_ptInit;
	m_ptEnd = pObj->m_ptEnd;
	m_bAttEnd = pObj->m_bAttEnd;
	m_bAttInit = pObj->m_bAttInit;
}

int	 CLinkObject::GetObjectIcon()
{
	return CICON_LINK;
}


CAnimObject* CLinkObject::Clone()
{
	CLinkObject *pObj = new CLinkObject(this);
	return pObj;
}

CLinkObject::~CLinkObject()
{
	if (m_pLinkPage) delete m_pLinkPage;
	UnlinkScr();
	UnlinkDest();
}

CString CLinkObject::SetObjectType()
{
	CString mstr;
	mstr.Format(_T("Connector n.%d"),m_ObjectID);
	return mstr;
}

/*void CLinkObject::SetObjectDef(CString mstr)
{
	if (mstr.GetLength() == 0)
	{
		m_ObjectDef.Format(_T("Connector n.%d"),m_ObjectID);
	}
	else
		m_ObjectDef = mstr;
}*/

void CLinkObject::Serialize(CArchive& ar)
{
	CAnimObject::Serialize(ar);
	if( ar.IsStoring( ) )
	{
		//ar << m_iValueName;
		ar << m_nIDInit;
		ar << m_nIDEnd;
		ar << m_nArrowStyle;
		ar << m_bArrInit;
		ar << m_bArrEnd;
		ar << m_bAttInit;
		ar << m_bAttEnd;
	}
	else
	{
		//ar >> m_iValueName;
		ar >> m_nIDInit;
		ar >> m_nIDEnd;
		ar >> m_nArrowStyle;
		ar >> m_bArrInit;
		ar >> m_bArrEnd;
		ar >> m_bAttInit;
		ar >> m_bAttEnd;
	}
}

BOOL CLinkObject::LinkToObjects(CAnimObject *pObjSrc,CAnimObject *pObjDest)
{
	BOOL bDone = FALSE;
	if (m_pInit) UnlinkScr();
	if (m_pEnd) UnlinkDest();
	if (pObjSrc)
	{
		m_pInit = pObjSrc;
		m_nIDInit = pObjSrc->m_ObjectID;
		pObjSrc->m_cConnectors.Add(this);
		bDone = TRUE;
	}
	if (pObjDest)
	{
		m_pEnd = pObjDest;
		m_nIDEnd = pObjDest->m_ObjectID;
		pObjDest->m_cConnectors.Add(this);
		bDone |= TRUE;
	}
	return bDone;
}

BOOL CLinkObject::UnlinkScr()
{
	BOOL bDone = FALSE;
	if (m_pInit)
	{
		for (int i=0;i<m_pInit->m_cConnectors.GetSize();i++)
		{
			CAnimObject *pObj = m_pInit->m_cConnectors.GetAt(i);
			if (pObj == this)
				m_pInit->m_cConnectors.RemoveAt(i);
		}
		m_pInit = NULL;
		m_nIDInit = -1;
		bDone = TRUE;
	}
	return bDone;
}

BOOL CLinkObject::UnlinkDest()
{
	BOOL bDone = FALSE;
	if (m_pEnd)
	{
		for (int i=0;i<m_pEnd->m_cConnectors.GetSize();i++)
		{
			CAnimObject *pObj = m_pEnd->m_cConnectors.GetAt(i);
			if (pObj == this)
				m_pEnd->m_cConnectors.RemoveAt(i);
		}
		m_pEnd = NULL;
		m_nIDEnd = -1;
		bDone = TRUE;
	}
	return bDone;
}

BOOL CLinkObject::UnlinkObject(CAnimObject *pObj)
{
	BOOL bDone = FALSE;
	if (pObj)
	{
		if (pObj == m_pInit) UnlinkScr();
		if (pObj == m_pEnd) UnlinkDest();
	}
	return bDone;
}


/*BOOL CLinkObject::UnlinkFromObjects(CAnimObject *pObjSrc,CAnimObject *pObjDest)
{
	BOOL bDone = FALSE;
	if (pObjSrc && (m_pInit == pObjSrc))
	{
		m_pInit = NULL;
		m_nIDInit = -1;
		for (int i=0;i<pObjSrc->m_cConnectors.GetSize();i++)
		{
			CAnimObject *pObj = pObjSrc->m_cConnectors.GetAt(i);
			if (pObj == this)
				pObjSrc->m_cConnectors.RemoveAt(i);
		}
		bDone = TRUE;
	}
	if (pObjDest && (m_pEnd == pObjDest))
	{
		m_pEnd = NULL;
		m_nIDEnd = -1;
		for (int i=0;i<pObjDest->m_cConnectors.GetSize();i++)
		{
			CAnimObject *pObj = pObjDest->m_cConnectors.GetAt(i);
			if (pObj == this)
				pObjDest->m_cConnectors.RemoveAt(i);
		}
		bDone |= TRUE;
	}
	return bDone;
}*/

CSectorObject* CLinkObject::GetTopSector(CAnimObject *pObj)
{
	CRect mrect(0,0,0,0);
	BOOL bTop = FALSE;
	CSectorObject* pSector = NULL;
	CAnimObject *pObj2 = pObj;
	CxAnimObjectList mList;
	if (!pObj) return pSector;
	while (!bTop)
	{
		pSector= pObj2->m_pSector;
		if (pSector)
		{
			mList.Add(pSector);
			pObj2=pSector;
		}
		else bTop=TRUE;
	}
	pSector = NULL;
	int nb = mList.GetSize();
	for (int i=nb-1;i>=0;i--)
	{
		CSectorObject *pObj3 = (CSectorObject *)mList.GetAt(i);
		if (!(pObj3->m_bIsOpen))
		{
			pSector = pObj3;
			break;
		}
	}
	return pSector;
}

void CLinkObject::OnInitialUpdate()
{
	CAnimObject::OnInitialUpdate();
}

void CLinkObject::OnClose()
{
	CAnimObject::OnClose();
}


void CLinkObject::OnUpdate()
{
	CAnimObject::OnUpdate();

	CRect mrect = GetRect();
	m_ptInit = m_nDDPos.m_ptDefPos;;
	m_ptEnd = mrect.BottomRight();

	//m_currTime = m_pDoc->m_currTimer;
	int nExpSet = m_currExpSet;

	//m_bDirEnd = m_bDirInit = TRUE;
	CPoint dist1[9],dist2[9];
	//double ddist[4];

	if (m_pInit)
	{
		//m_pInit->OnUpdate();
		CRect mrect2;
		CSectorObject* pS1 = GetTopSector(m_pInit);
		if (pS1)
			mrect2 = pS1->GetRect();
		else
			mrect2 = m_pInit->GetRect();

/*		if (m_pInit->m_pSector && !(m_pInit->m_pSector->m_bIsOpen))
		{
			mrect2 = m_pInit->m_pSector->GetRect();
		}
		else
			mrect2 = m_pInit->GetRect();*/
		//mrect.left = mrect2.right;
		//mrect.top = mrect2.CenterPoint().y;

		dist1[0]=CPoint(mrect2.CenterPoint().x,mrect2.top);
		dist1[1]=CPoint(mrect2.left,mrect2.CenterPoint().y);
		dist1[2]=CPoint(mrect2.CenterPoint().x,mrect2.bottom);
		dist1[3]=CPoint(mrect2.right,mrect2.CenterPoint().y);
		dist1[4]=mrect2.CenterPoint();
		dist1[5]=CPoint(mrect2.left,mrect2.top);
		dist1[6]=CPoint(mrect2.right,mrect2.top);
		dist1[7]=CPoint(mrect2.left,mrect2.bottom);
		dist1[8]=CPoint(mrect2.right,mrect2.bottom);
		//m_ptInit = mrect2.CenterPoint();
		m_ptInit = dist1[m_bAttInit];
	}
	if (m_pEnd)
	{
		//m_pEnd->OnUpdate();
		CRect mrect2;

		CSectorObject* pS2 = GetTopSector(m_pEnd);
		if (pS2)
			mrect2 = pS2->GetRect();
		else
			mrect2 = m_pEnd->GetRect();

/*		if (m_pEnd->m_pSector && !(m_pEnd->m_pSector->m_bIsOpen))
		{
			mrect2 = m_pEnd->m_pSector->GetRect();
		}
		else
			mrect2 = m_pEnd->GetRect();*/
		//mrect.right = mrect2.left;
		//mrect.bottom = mrect2.CenterPoint().y;
		dist2[0]=CPoint(mrect2.CenterPoint().x,mrect2.top);
		dist2[1]=CPoint(mrect2.left,mrect2.CenterPoint().y);
		dist2[2]=CPoint(mrect2.CenterPoint().x,mrect2.bottom);
		dist2[3]=CPoint(mrect2.right,mrect2.CenterPoint().y);
		dist2[4]=mrect2.CenterPoint();
		dist2[5]=CPoint(mrect2.left,mrect2.top);
		dist2[6]=CPoint(mrect2.right,mrect2.top);
		dist2[7]=CPoint(mrect2.left,mrect2.bottom);
		dist2[8]=CPoint(mrect2.right,mrect2.bottom);
		//m_ptEnd = mrect2.CenterPoint();
		m_ptEnd = dist2[m_bAttEnd];
	}

/*	int idx1,idx2;
	if (m_pInit)
	{
		for (int i=0;i<4;i++)
		{
			double dx = dist1[i].x - m_ptEnd.x;
			double dy = dist1[i].y - m_ptEnd.y;
			ddist[i] = sqrt(dx*dx+dy*dy);
		}
		idx1 = 0;
		double val = ddist[0];
		for (i=1;i<4;i++)
		{
			if (ddist[i] < val)
			{
				val = ddist[i];
				idx1 = i;
			}
		}
		m_ptInit = dist1[idx1];
	}
	if (m_pEnd)
	{
		for (int i=0;i<4;i++)
		{
			double dx = dist2[i].x - m_ptInit.x;
			double dy = dist2[i].y - m_ptInit.y;
			ddist[i] = sqrt(dx*dx+dy*dy);
		}
		idx2 = 0;
		double val = ddist[0];
		for (i=1;i<4;i++)
		{
			if (ddist[i] < val)
			{
				val = ddist[i];
				idx2 = i;
			}
		}
		m_ptEnd = dist2[idx2];
	}
	m_bDirInit = (idx1 % 2);
	m_bDirEnd = (idx2 % 2);*/

	mrect = CRect(m_ptInit,m_ptEnd);
	mrect.NormalizeRect();
	m_nDDPos.m_ptDefPos = mrect.TopLeft();
	SetRect(mrect);

	CLearningUnit *pLU= m_pDoc->GetCurrentLU();

	//CMdData *pData = pLU->GetDataAt(m_iValueName);
	//if (!pData) return;
	//m_dValue = pData->GetAt(m_currTime,nExpSet);
}

void CLinkObject::SetPropSheetPages(CPropertySheet *pSheet,CxAnimObjectList *pList)
{
	if (!m_pLinkPage)
		m_pLinkPage = new CAnimLinkPage();

	SetPropSheetShape(pSheet,pList);

	////// No background modification
	m_pShapPage->m_bAllowBk = FALSE;

	CLearningUnit *pLU= m_pDoc->GetCurrentLU();
	int nb = pLU->GetDataSize();
	for (int i=0;i<nb;i++)
	{
		CMdData *pData = pLU->GetDataAt(i);
		CString mstr = pData->GetDataName(pLU->m_bShowAbbrev);
		COLORREF mclr = pData->GetDataInfo()->GetColor();
		m_pShapPage->m_sVarList.Add(mstr);
		m_pShapPage->m_sVarColorList.Add(mclr);
		m_pShapPage->m_sOutcomeIcon.Add(pData->GetDataInfo()->GetIcon());
	}

	////// Setup Linkable Components
	m_pLinkPage->m_sCpntList.RemoveAll();
	if (pList)
	{
		BOOL bFound = FALSE;
		CxAnimObjectList mList2;
		CopyElt(pList,&mList2);
		int nbC = mList2.GetSize();
		for (int i=0;i<nbC && !bFound;i++)
		{
			CAnimObject *pObj = mList2.GetAt(i);
			if (!pObj) continue;
			if (pObj != this)
			{
				m_pLinkPage->m_sCpntList.Add(pObj->GetObjectDef());
			}
			else
				bFound = TRUE;
		}

/*		int nbC = pList->GetSize();
		for (int i=0;i<nbC && !bFound;i++)
		{
			CAnimObject *pObj = pList->GetAt(i);
			if (!pObj) continue;
			if (pObj != this)
			{
				CSectorObject *pSecObj = DYNAMIC_DOWNCAST(CSectorObject, pObj);
				m_pLinkPage->m_sCpntList.Add(pObj->GetObjectDef());
				if (pSecObj)
				{
					for (int j=0;j<pSecObj->GetSectorSize();j++)
					{
					m_pLinkPage->m_sCpntList.Add(pSecObj->GetFromSectorAt(j)->GetObjectDef());
					}
				}
			}
			else
				bFound = TRUE;
		}*/
	}
	m_pLinkPage->m_pObjSet = (CxAnimObjectList*)pList;
	m_pLinkPage->m_nStyleConnector = m_nArrowStyle;
	m_pLinkPage->m_bArrowEnd = m_bArrEnd;
	m_pLinkPage->m_bArrowStart = m_bArrInit;
	//m_pLinkPage->m_nIDStart = m_nIDInit+1;

	m_pLinkPage->m_nIDStart = (m_pInit) ? m_pInit->m_ObjectID : 0;
	m_pLinkPage->m_nIDEnd = (m_pEnd) ? m_pEnd->m_ObjectID : 0;

	m_pLinkPage->m_nAttachStart = m_bAttInit;
	m_pLinkPage->m_nAttachEnd = m_bAttEnd;


	////// Add Pages' Icon
	CImageList pImageList;
	pImageList.Create(IDB_ANIM_CPNTPAGE,16,0,RGB(192,192,192));

	m_pLinkPage->m_psp.dwFlags |= PSP_USEHICON ;
	m_pLinkPage->m_psp.hIcon = pImageList.ExtractIcon(6);

	m_pShapPage->m_psp.dwFlags |= PSP_USEHICON ;
	m_pShapPage->m_psp.hIcon = pImageList.ExtractIcon(2);


	////// Add All Pages
	pSheet->AddPage(m_pLinkPage);
	pSheet->AddPage(m_pShapPage);

//	CAnimObject::SetPropSheetPages(pSheet,pList);
//	pSheet->SetActivePage(m_pTextPage);
}

void CLinkObject::ModifyObjectProperty()
{
	CAnimObject::ModifyObjectProperty();

	m_nArrowStyle = m_pLinkPage->m_nStyleConnector;
	m_bArrEnd = m_pLinkPage->m_bArrowEnd;
	m_bArrInit = m_pLinkPage->m_bArrowStart;
	m_nIDInit = m_pLinkPage->m_nIDStart-1;
	m_nIDEnd = m_pLinkPage->m_nIDEnd-1;

	m_bAttInit = m_pLinkPage->m_nAttachStart;
	m_bAttEnd = m_pLinkPage->m_nAttachEnd;
//OnUpdate();
}

void CLinkObject::DisposeProperty()
{
	CAnimObject::DisposeProperty();
	if (m_pLinkPage) 
	{
		delete m_pLinkPage;
		m_pLinkPage=NULL;
	}
}


void CLinkObject::Draw(CDC* pDC,BOOL bParent)
{
	CRect mrect = GetRect();
	//CPoint startPt = m_nDDPos.m_ptDefPos;
	CPoint startPt = mrect.TopLeft();
	CPoint endPt = mrect.BottomRight();
	startPt = m_ptInit;
	endPt = m_ptEnd;
	
	CBrush *oldB = NULL;
	CPen *oldP = NULL;

	CBrush pBrush(::GetSysColor(COLOR_WINDOW));
	CPen pPen(PS_NULL,0,CLR_DEFAULT);

	LOGBRUSH pLogB;

	if (!m_pBrush.GetSafeHandle() || !m_pPen.GetSafeHandle())
		OnInitialUpdate();

	m_pBrush.GetLogBrush(&pLogB);
	if (pLogB.lbColor != CLR_DEFAULT)
	{
		oldB = pDC->SelectObject(&m_pBrush);
//		pDC->FillRect(m_ObjRect,&m_pBrush);
	}
	else
		oldB = pDC->SelectObject(&pBrush);

	LOGPEN pLogP;
	m_pPen.GetLogPen(&pLogP);
	if (pLogP.lopnColor != CLR_DEFAULT)
	{
		oldP = pDC->SelectObject(&m_pPen);
	}
	else
		oldP = pDC->SelectObject(&pPen);
	
		//pDC->Rectangle(m_ObjRect);
	switch (m_nArrowStyle) {
	case 1 :		// ELBOW
		{
			CPoint pt;
			pt.x = (startPt.x + endPt.x)/2;
			pt.y = (startPt.y + endPt.y)/2;
			pDC->MoveTo(startPt);
			int m_bDirInit = (m_bAttInit % 2);
			int m_bDirEnd = (m_bAttEnd % 2);
			if (m_bDirEnd == m_bDirInit)
			{
				if (m_bDirEnd)
				{
					pDC->LineTo(pt.x,startPt.y);
					pDC->LineTo(pt.x,endPt.y);
				}
				else
				{
					pDC->LineTo(startPt.x,pt.y);
					pDC->LineTo(endPt.x,pt.y);
				}
			}
			else
			{
				if (m_bDirInit)
				{
					pDC->LineTo(endPt.x,startPt.y);
				}
				else
				{
					pDC->LineTo(startPt.x,endPt.y);
				}
			}
//			if (m_bDoubleElbow)
//			{
//				pDC->LineTo(pt.x,startPt.y);
//				pDC->LineTo(pt.x,endPt.y);
//			}
//			else
//			{
//				pDC->LineTo(pt.x,startPt.y);
//				pDC->LineTo(pt.x,endPt.y);
//			}
			pDC->LineTo(endPt);
		}
		break;
	case 3 :        // SPRING
		{
			CPoint mpt[20];
			CSize ms = endPt-startPt;

			double ddx = ms.cx / 19.;
			double ddy = ms.cy / 19.;

			CSize ms3 = ms;
			ms3.cx = -ms.cy;
			ms3.cy = ms.cx;
			double ddd= sqrtl(ms3.cx*ms3.cx+ms3.cy*ms3.cy);
			double a = 12*ms3.cx / ddd;
			double b = 12*ms3.cy / ddd;

			//BOOL bUp = TRUE;

			mpt[0]=startPt;
			mpt[1]=startPt;
			mpt[1].x += (long)ddx;
			mpt[1].y += (long)ddy;
			for (int i=2;i<18;i++)
			{
				//CSize mup(0,(bUp)?-10:10);
				//CSize dd;
				//dd.cx = ms.cx*i + mup.cx;
				//dd.cy = ms.cy*i + mup.cy;
				mpt[i]=startPt;
				mpt[i].x += (long)(ddx*i);
				mpt[i].y += (long)(ddy*i);//((bUp)?-10:10);
				//bUp = !bUp;
				if (i%2==0)
				{
					mpt[i].x += (int)a;
					mpt[i].y += (int)b;
				}
				else
				{
					mpt[i].x -= (int)a;
					mpt[i].y -= (int)b;
				}
			}
			mpt[19]=endPt;
			mpt[18]=endPt;
			mpt[18].x -= (long)ddx;
			mpt[18].y -= (long)ddy;
			pDC->Polyline(mpt,20);
		}
		break;
	case 2 :		// CURVED
		{
			CSize ms ,ms2 = endPt-startPt;

			CRect rect = m_pInit->GetRect();

			ms.cy = max(abs(ms2.cy),rect.Height());
			ms.cx = max(abs(ms2.cx),rect.Width());



			CPoint dist1[9],dist2[9];
			
			dist1[0]=CPoint(startPt.x,startPt.y-ms.cy);
			dist1[1]=CPoint(startPt.x-ms.cx,startPt.y);
			dist1[2]=CPoint(startPt.x,startPt.y+ms.cy);
			dist1[3]=CPoint(startPt.x+ms.cx,startPt.y);
			dist1[4]=CPoint(startPt.x,startPt.y);  //center
			dist1[5]=CPoint(startPt.x-ms.cx,startPt.y-ms.cy);
			dist1[6]=CPoint(startPt.x+ms.cx,startPt.y-ms.cy);
			dist1[7]=CPoint(startPt.x-ms.cx,startPt.y+ms.cy);
			dist1[8]=CPoint(startPt.x+ms.cy,startPt.y+ms.cy);

			rect = m_pEnd->GetRect();

			ms.cy = 2*rect.Height();
			ms.cx = 2*rect.Width();
			ms.cy = max(abs(ms2.cy),rect.Height());
			ms.cx = max(abs(ms2.cx),rect.Width());

			dist2[0]=CPoint(endPt.x,endPt.y-ms.cy);
			dist2[1]=CPoint(endPt.x-ms.cx,endPt.y);
			dist2[2]=CPoint(endPt.x,endPt.y+ms.cy);
			dist2[3]=CPoint(endPt.x+ms.cx,endPt.y);
			dist2[4]=CPoint(endPt.x,endPt.y);  //center
			dist2[5]=CPoint(endPt.x-ms.cx,endPt.y-ms.cy);
			dist2[6]=CPoint(endPt.x+ms.cx,endPt.y-ms.cy);
			dist2[7]=CPoint(endPt.x-ms.cx,endPt.y+ms.cy);
			dist2[8]=CPoint(endPt.x+ms.cy,endPt.y+ms.cy);

			CPoint pt[4] = {
				startPt,
				dist1[m_bAttInit],
				dist2[m_bAttEnd],
				endPt};
			/*pDC->MoveTo(pt[0]);
			pDC->LineTo(pt[1]);
			pDC->MoveTo(pt[2]);
			pDC->LineTo(pt[3]);*/

			pDC->PolyBezier(pt,4);

		}
		break;
	default:		// STRAIGHT
		pDC->MoveTo(startPt);
		pDC->LineTo(endPt);
	}

	if (m_bArrInit)
	{
		CRect endRect(startPt,startPt);
		endRect.InflateRect(1+pLogP.lopnWidth.x,1+pLogP.lopnWidth.x);
		pDC->Ellipse(endRect);
	}
	if (m_bArrEnd)
	{
		CRect endRect(endPt,endPt);
		endRect.InflateRect(1+pLogP.lopnWidth.x,1+pLogP.lopnWidth.x);
		pDC->Ellipse(endRect);
	}

	/*{
		CRect endRect(m_ptInit,m_ptInit);
		endRect.InflateRect(1+pLogP.lopnWidth.x,1+pLogP.lopnWidth.x);
		pDC->Ellipse(endRect);
		endRect = CRect(m_ptEnd,m_ptEnd);
		endRect.InflateRect(1+pLogP.lopnWidth.x,1+pLogP.lopnWidth.x);
		pDC->Ellipse(endRect);
	}*/

	if (oldP)
		pDC->SelectObject(oldP);
	if (oldB)
		pDC->SelectObject(oldB);
}

void CopyElt(CxAnimObjectList *pList,CxAnimObjectList *pCopyList)
{
	for (int i=0;i<pList->GetSize();i++)
	{
		CAnimObject *pObj = pList->GetAt(i);
		pCopyList->Add(pObj);
		CSectorObject *pSecObj = DYNAMIC_DOWNCAST(CSectorObject, pObj);
		if (pSecObj)
			CopyElt(&(pSecObj->m_cSectorObjSet),pCopyList);
	}
}

/*CAnimObject *GetAt(CxAnimObjectList *pList,int idx,int &nID)
{
	int nb = pList->GetSize();
//	if (idx >= nb) return NULL;

	CAnimObject *pObj = pList->GetAt(idx);
	if (nID == 0) return pObj;

	nID--;

	CSectorObject *pSecObj = DYNAMIC_DOWNCAST(CSectorObject, pObj);
	if (pSecObj)
	{
		CAnimObject *pObj2 = GetAt(&(pSecObj->m_cSectorObjSet),0,nID);
		return pObj2;
	}
	else 
		return NULL;
	
/*	for (int i=0;i<nb && (nID>=0);i++)
	{
		CSectorObject *pSecObj = DYNAMIC_DOWNCAST(CSectorObject, pObj);
		if (pSecObj)
		{
			nID--;
			if (pObj2)
			{
				pObj = pObj2;
				nID--;
			}
			else
			{
//				nID--;
			}
		}
		else
			nID--;
	}
	if (i == nb) 
		return NULL;
	return pObj;
}*/

int CLinkObject::OnEditProperty(CView *pView,int nShowSheet)
{
	int res = -1;

	//CAnimObjPropDlg mSheet(_T("Object n. 54"),pView);
	CHelpPrSheet mSheet(_T("Object n. 54"),pView);
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

	SetPropSheetPages(&mSheet,myList);
	if ((res = mSheet.DoModal()) == IDOK)
	{
		ModifyObjectProperty();
		CxAnimObjectList pCopyList;

		CAnimObject *p1 = NULL;
		CAnimObject *p2 = NULL;

		CopyElt(myList,&pCopyList);
		if ((m_nIDInit >=0) && myList)
		{
			int nID = m_nIDInit;
			//m_pInit = GetAt(myList,0,nID);
			//m_pInit = pCopyList.GetAt(nID);
			p1 = pCopyList.GetAt(nID);
		}
		//else
		//	m_pInit = NULL;
		if ((m_nIDEnd >=0) && myList)
		{
			int nID = m_nIDEnd;
			//m_pEnd = GetAt(myList,0,nID);
			//m_pEnd = pCopyList.GetAt(nID);
			p2 = pCopyList.GetAt(nID);
		}
		//else
		//	m_pEnd = NULL;

		LinkToObjects(p1,p2);

//		OnUpdate();
//		if (pView)
//			pView->Invalidate();
	}
	DisposeProperty();
	return res;

}



//////////////////////////////////////////////////////////////////////
// CVectorObject
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CVectorObject, CAnimObject, VERSIONABLE_SCHEMA | 0)

CVectorObject::CVectorObject () : CAnimObject()
{
//	SetObjectDef(_T(""));

	m_nIDInit = -1;
	m_pInit = NULL;
	m_pLinkPage = NULL;
	m_nArrowStyle = 0;
	m_ptInit = GetRect().TopLeft();
	m_ptEnd = GetRect().BottomRight();

	SetObjectColor(RGB(0,64,64),2);
}

CVectorObject::CVectorObject (CVectorObject *pObj): CAnimObject(pObj)
{
	m_nIDInit = -1;
	m_pInit = NULL;
	m_pLinkPage = NULL;
	m_nArrowStyle = pObj->m_nArrowStyle;
	m_ptInit = pObj->m_ptInit;
	m_ptEnd = pObj->m_ptEnd;
}
 
CVectorObject::CVectorObject (int ID) : CAnimObject(ID,CRect(100,100,150,150))
{
//	SetObjectDef(_T(""));
	m_nIDInit = -1;
	m_pInit = NULL;
	m_pLinkPage = NULL;
	m_nArrowStyle = 0;
	m_ptInit = GetRect().TopLeft();
	m_ptEnd = GetRect().BottomRight();

	SetObjectColor(RGB(0,64,64),2);
}

CVectorObject::~CVectorObject()
{
	if (m_pLinkPage) delete m_pLinkPage;
}

int	 CVectorObject::GetObjectIcon()
{
	return -1;
}


CAnimObject* CVectorObject::Clone()
{
	CVectorObject *pObj = new CVectorObject(this);
	return pObj;
}

void CVectorObject::Serialize(CArchive& ar)
{
	CAnimObject::Serialize(ar);
	if( ar.IsStoring( ) )
	{
		//ar << m_iValueName;
		ar << m_nIDInit;
		ar << m_nArrowStyle;
	}
	else
	{
		//ar >> m_iValueName;
		ar >> m_nIDInit;
		ar >> m_nArrowStyle;
	}
}

void CVectorObject::Draw (CDC* pDC,BOOL bParent/*=TRUE*/)
{
	CAnimObject::Draw (pDC);
}

/*int	 CVectorObject::OnEditProperty(CView *pView,int nShowSheet = 0)
{
}*/

void CVectorObject::DisposeProperty()
{
	CAnimObject::DisposeProperty();
	if (m_pLinkPage) 
	{
		delete m_pLinkPage;
		m_pLinkPage=NULL;
	}
}

void CVectorObject::ModifyObjectProperty()
{
	CAnimObject::ModifyObjectProperty();
}

void CVectorObject::SetPropSheetPages(CPropertySheet *pSheet,CxAnimObjectList *pList/*=NULL*/)
{
	//if (!m_pLinkPage)
	//	m_pLinkPage = new CAnimLinkPage();

	//pSheet->AddPage(m_pTextPage);
	CAnimObject::SetPropSheetPages(pSheet,pList);
	//pSheet->SetActivePage(m_pTextPage);
}

CString CVectorObject::SetObjectType()
{
	CString mstr;
	mstr.Format(_T("Vector n.%d"),m_ObjectID);
	return mstr;
}


/*void CVectorObject::SetObjectDef(CString mstr)
{
	if (mstr.GetLength() == 0)
	{
		m_ObjectDef.Format(_T("Vector n.%d"),m_ObjectID);
	}
	else
		m_ObjectDef = mstr;
}*/

void CVectorObject::OnUpdate()
{
	CAnimObject::OnUpdate();
}


//////////////////////////////////////////////////////////////////////
// CLinkObject
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CPolygonObject, CAnimObject, VERSIONABLE_SCHEMA | 0)

CPolygonObject::CPolygonObject(): CAnimObject ()
{
//	SetObjectDef(_T(""));
	SetObjectColor(RGB(0,64,64),3);
	m_pPolyPage = NULL;
}

CPolygonObject::CPolygonObject(int ID): CAnimObject (ID,CRect(100,100,150,150))
{
//	SetObjectDef(_T(""));
	SetObjectColor(RGB(0,64,64),3);
	m_pPolyPage = NULL;
}


CPolygonObject::CPolygonObject(CPolygonObject *pObj) : CAnimObject(pObj)
{
	m_pPolyPage = NULL;
}

int	 CPolygonObject::GetObjectIcon()
{
	return CICON_POLY;
}

CString CPolygonObject::SetObjectType()
{
	CString mstr;
	mstr.Format(_T("Polygon n.%d"),m_ObjectID);
	return mstr;
}

void CPolygonObject::Serialize(CArchive& ar)
{
	CAnimObject::Serialize(ar);

	if( ar.IsStoring( ) )
	{
		//ar << m_iValueName;
	}
	else
	{
		//ar >> m_iValueName;
	}
	m_cPolyPt.Serialize(ar);
}


BOOL CPolygonObject::LinkToObjects(CxAnimObjectList *pObjList)
{
	if (!pObjList) return FALSE;


	int nb = m_cPolyPt.GetSize();

	m_cPolyObj.RemoveAll();

	for (int i=0;i<nb;i++)
	{
		CAnimObject *p1=NULL;
		CPoint pt = m_cPolyPt.GetAt(i);

		if (pt.x != 0)
		{
			p1 = pObjList->GetAt(pt.x-1);
		}
		if (p1) m_cPolyObj.Add(p1);

	}


	return TRUE;
}



CAnimObject* CPolygonObject::Clone()
{
	CPolygonObject *pObj = new CPolygonObject(this);
	return pObj;
}

CPolygonObject::~CPolygonObject()
{
	if (m_pPolyPage) delete m_pPolyPage;
}

void CPolygonObject::SetPropSheetPages(CPropertySheet *pSheet,CxAnimObjectList *pList)
{
	if (!m_pPolyPage)
		m_pPolyPage = new CAnimPolygonPage();

	SetPropSheetShape(pSheet,pList);

	////// No background modification
	m_pShapPage->m_bAllowBk = TRUE;

	CLearningUnit *pLU= m_pDoc->GetCurrentLU();
	int nb = pLU->GetDataSize();
	for (int i=0;i<nb;i++)
	{
		CMdData *pData = pLU->GetDataAt(i);
		CString mstr = pData->GetDataName(pLU->m_bShowAbbrev);
		COLORREF mclr = pData->GetDataInfo()->GetColor();
		m_pShapPage->m_sVarList.Add(mstr);
		m_pShapPage->m_sVarColorList.Add(mclr);
		m_pShapPage->m_sOutcomeIcon.Add(pData->GetDataInfo()->GetIcon());
	}

	////// Setup Polygon Components
	m_pPolyPage->m_cItems.Copy(m_cPolyPt);

	m_pPolyPage->m_sCpntList.RemoveAll();
	if (pList)
	{
		BOOL bFound = FALSE;
		CxAnimObjectList mList2;
		CopyElt(pList,&mList2);
		int nbC = mList2.GetSize();
		for (int i=0;i<nbC && !bFound;i++)
		{
			CAnimObject *pObj = mList2.GetAt(i);
			if (!pObj) continue;
			if (pObj != this)
			{
				m_pPolyPage->m_sCpntList.Add(pObj->GetObjectDef());
			}
			else
				bFound = TRUE;
		}
	}


	////// Add Pages' Icon
	CImageList pImageList;
	pImageList.Create(IDB_ANIM_CPNTPAGE,16,0,RGB(192,192,192));

	m_pPolyPage->m_psp.dwFlags |= PSP_USEHICON ;
	m_pPolyPage->m_psp.hIcon = pImageList.ExtractIcon(9);

	m_pShapPage->m_psp.dwFlags |= PSP_USEHICON ;
	m_pShapPage->m_psp.hIcon = pImageList.ExtractIcon(2);


	////// Add All Pages
	pSheet->AddPage(m_pPolyPage);
	pSheet->AddPage(m_pShapPage);
}

void CPolygonObject::ModifyObjectProperty()
{
	CAnimObject::ModifyObjectProperty();

	int nb = m_pPolyPage->m_cItems.GetSize();

	m_cPolyPt.Copy(m_pPolyPage->m_cItems);

}

void CPolygonObject::DisposeProperty()
{
	CAnimObject::DisposeProperty();
	if (m_pPolyPage) 
	{
		delete m_pPolyPage;
		m_pPolyPage=NULL;
	}
}

void CPolygonObject::OnInitialUpdate()
{
	CAnimObject::OnInitialUpdate();
}

void CPolygonObject::OnClose()
{
	CAnimObject::OnClose();
}


void CPolygonObject::OnUpdate()
{
	CAnimObject::OnUpdate();

	CRect mrect = GetRect();
	mrect.SetRectEmpty();

	//m_currTime = m_pDoc->m_currTimer;
	int nExpSet = m_currExpSet;

	//m_bDirEnd = m_bDirInit = TRUE;
	CPoint dist1[9];

	int nb = m_cPolyPt.GetSize();

	m_cPolyLine.RemoveAll();

	for (int i=0;i<nb;i++)
	{
		CPoint pt = m_cPolyPt.GetAt(i);

		CAnimObject *pObj = m_cPolyObj.GetAt(i);
		if (!pObj) continue;

		CRect mrect2;

		mrect2 = pObj->GetRect();

		dist1[0]=CPoint(mrect2.CenterPoint().x,mrect2.top);
		dist1[1]=CPoint(mrect2.left,mrect2.CenterPoint().y);
		dist1[2]=CPoint(mrect2.CenterPoint().x,mrect2.bottom);
		dist1[3]=CPoint(mrect2.right,mrect2.CenterPoint().y);
		dist1[4]=mrect2.CenterPoint();
		dist1[5]=CPoint(mrect2.left,mrect2.top);
		dist1[6]=CPoint(mrect2.right,mrect2.top);
		dist1[7]=CPoint(mrect2.left,mrect2.bottom);
		dist1[8]=CPoint(mrect2.right,mrect2.bottom);

		CPoint m_ptInit = dist1[pt.y];

		CRect nrect(m_ptInit,m_ptInit);
		nrect.InflateRect(1,1);
		if (mrect == CRect(0,0,0,0))
			mrect = nrect;
		else
			mrect |= nrect;

		m_cPolyLine.Add(m_ptInit);
	}

	SetRect(mrect);

}

int CPolygonObject::OnEditProperty(CView *pView,int nShowSheet)
{
	int res = -1;

	CHelpPrSheet mSheet(_T("Object n. 54"),pView);
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

	SetPropSheetPages(&mSheet,myList);
	if ((res = mSheet.DoModal()) == IDOK)
	{
		ModifyObjectProperty();
		LinkToObjects(myList);
	}
	DisposeProperty();
	return res;

}

void CPolygonObject::Draw(CDC* pDC,BOOL bParent)
{
	CRect mrect = GetRect();

	int nb = m_cPolyLine.GetSize();

	if (!nb) pDC->Rectangle(mrect);

	CPoint *pt = new CPoint[nb+1];

	for (int i=0;i<nb;i++)
	{
		pt[i] = m_cPolyLine.GetAt(i);
	}
	if (nb) pt[nb] = m_cPolyLine.GetAt(0);

	CBrush *oldB = NULL;
	CPen *oldP = NULL;

	CBrush pBrush(::GetSysColor(COLOR_WINDOW));
	CPen pPen(PS_NULL,0,CLR_DEFAULT);

	LOGBRUSH pLogB;

	if (!m_pBrush.GetSafeHandle() || !m_pPen.GetSafeHandle())
		OnInitialUpdate();

	LOGPEN pLogP;
	m_pPen.GetLogPen(&pLogP);
	if (pLogP.lopnColor != CLR_DEFAULT)
	{
		oldP = pDC->SelectObject(&m_pPen);
	}
	else
		oldP = pDC->SelectObject(&pPen);
	
	m_pBrush.GetLogBrush(&pLogB);
	if (pLogB.lbColor != CLR_DEFAULT)
	{
		oldB = pDC->SelectObject(&m_pBrush);
//		pDC->FillRect(m_ObjRect,&m_pBrush);
		pDC->Polygon(pt,nb);
	}
	else
	{
		oldB = pDC->SelectObject(&pBrush);
		pDC->Polyline(pt,nb+1);
	}


	if (oldP)
		pDC->SelectObject(oldP);
	if (oldB)
		pDC->SelectObject(oldB);

	delete[] pt;
}