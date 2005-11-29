// LearningUnit.cpp: implementation of the CLearningUnit class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include <math.h>
// Afx privae include file.
#include <AfxPriv.h>


#include "simul.h"
#include "SimulDoc.h"
#include "Tools\MvDocTemplate.h"

#include "ChildFrm.h"

#include <Prefs\Pref.h>

#include <MInteraction\Format.h>
#include "LearningUnit.h"
//#include "DlgTranslation.h"

#include <MInteraction\SimulAnimat.h>
#include <MInteraction\SimulGraph.h>
#include <MInteraction\SimulTable.h>
#include <MInteraction\SimulPie.h>

#include "Tools\ST_SplitterWnd.h"

#include "Tools\BCMenu.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CPoint StartPt(50,150);

void DrawArrow(POINT  p1, POINT p2, int arrowSize ,int arrowlength, int sae,CDC *pDC,CBrush* pbrush)
{
	POINT points[4];
    //CRgn rgn;

	int dx=0,dy=0,dx1,dy1;
	CRect rect(p1,p2);
		//rect.NormalizeRect();
	CRect saverect=rect;
	long r2=(rect.top-rect.bottom)*(rect.top-rect.bottom)+(rect.left-rect.right)*(rect.left-rect.right);
	int l=(int)sqrt((double)r2);
	if (l)
	{
		dy=(arrowlength*(rect.top-rect.bottom))/l;
		dx=(arrowlength*(rect.left-rect.right))/l;
		dy1=(arrowlength*(rect.top-rect.bottom))/l;
		dx1=(arrowlength*(rect.left-rect.right))/l;
	}
	if (rect.top > rect.bottom)
	{
		//rect.top -= m_logpen.lopnWidth.y / 2;
		rect.top -= abs(dy1);
		//rect.bottom += (m_logpen.lopnWidth.y + 1) / 2;
		rect.bottom += abs(dy);
	}
	else
	{
		//rect.top += (m_logpen.lopnWidth.y + 1) / 2;
		rect.top += abs(dy1);
		//rect.bottom -= m_logpen.lopnWidth.y / 2;
		rect.bottom -=abs(dy);
	}
	if (rect.left > rect.right)
	{
		//rect.left -= m_logpen.lopnWidth.x / 2;
		rect.left -=abs(dx1);
		//rect.right += (m_logpen.lopnWidth.x + 1) / 2;
		rect.right +=abs(dx);
	}
	else
	{
		//rect.left += (m_logpen.lopnWidth.x + 1) / 2;
		rect.left +=abs(dx1);
		//rect.right -= m_logpen.lopnWidth.x / 2;
		rect.right -=abs(dx);
	}

	//actually a triagle
	points[0]=saverect.BottomRight();
	points[1]=points[2]=rect.BottomRight();
	points[1].x+=arrowSize*dy/arrowlength;
	points[1].y-=arrowSize*dx/arrowlength;
	points[2].x-=arrowSize*dy/arrowlength;
	points[2].y+=arrowSize*dx/arrowlength;
	pDC->Polygon(points,3);

	if (sae)
	{	
		points[0]=saverect.TopLeft();
		points[1]=points[2]=rect.TopLeft();
		points[1].x+=arrowSize*dy1/arrowlength;
		points[1].y-=arrowSize*dx1/arrowlength;
		points[2].x-=arrowSize*dy1/arrowlength;
		points[2].y+=arrowSize*dx1/arrowlength;
		pDC->Polygon(points,3);
	}

	//rgn.CreatePolygonRgn(points, 3, ALTERNATE);
	//pDC->FillRgn(&rgn,pbrush);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL( CInstModel, CObject, VERSIONABLE_SCHEMA | 1)

CInstModel::CInstModel(CSimulDoc *pDoc)
{
	m_pDoc = pDoc;
	m_BndRect = CRect(0,0,800,600);
//	m_ImageList.Create(IDB_ER_ICONS,32,0,RGB(192,192,192));
}

CInstModel::~CInstModel()
{
	for(int i = 0; i < m_cLUSet.GetSize();i++ )
		delete m_cLUSet.GetAt(i);
	m_cLUSet.RemoveAll();
}

//////////////////////////////////////////////////////////////////////
/// Determines, if any, which LU or ER is at a specified position.
/// \param	ptHit	The point to be tested.
/// \param	mrect	Fill this CRect instance with the bounding box of the LU/ER.
/// \return	a bipoint (X,Y), where X identifies the target LU and Y identifies the target ER.
///
/// If the hit-point is outside of the LU's bounding box, the function return (0,0).
/// If the hit-point is inside the LU's bounding box but ouside any ERs, the function return (-1,0).
/// If the hit-point is in the bounding box of one of the ERs of thid LU, the function return (X,Y), 
/// where X is the zero-based index of the target LU (#m_nLUID) ans Y is the one-based index of the target ER.
//////////////////////////////////////////////////////////////////////
CPoint CInstModel::HitTest(CPoint Hitpt,CRect &mrect)
{
	//int repUnit = 0;
	CPoint repUnit(0,0);
//	int repER = 0;
	for(int i = 0; i < m_cLUSet.GetSize();i++ )
	{
		CPoint gg = m_cLUSet.GetAt(i)->HitTest(Hitpt,mrect);
		//repER = HIWORD(gg);
		//CString mstrrrr;
		//mstrrrr.Format(_T("Unit : %d , ER : %d , i : %d"),gg,repER,i+1);
		//::MessageBox(NULL,mstrrrr,_T("ggf"),MB_OK);
		//if (gg != 0)
		
		if (gg != CPoint(0,0))
		{
			//repUnit = MAKELONG(i+1,repER);
			repUnit = CPoint(i+1,gg.y);
			break;
		}
	}
	return repUnit;
}


void CInstModel::Serialize( CArchive& ar )
{
	CObject::Serialize(ar);
	if( ar.IsStoring( ) )
	{
		ar << m_sDesc;
		ar << m_sGoal;
		ar << m_BndRect;
	}
	else
	{
		ar >> m_sDesc;
		ar >> m_sGoal;
		ar >> m_BndRect;
	}
	m_cLUSet.Serialize(ar);
	for (int i=0;i<m_cLUSet.GetSize();i++)
	{
		m_cLUSet.GetAt(i)->m_pLUDoc = m_pDoc;
	}
}

//////////////////////////////////////////////////////////////////////
/// Called by the framework to draw the Instructional Design Interface.
/// \param pDC A pointer to the Device Context used for drawing the view.
/// \param pImageList A pointer to the image list containing the various LU's icons.
///
//////////////////////////////////////////////////////////////////////
void CInstModel::OnDraw(CDC* pDC,CImageList *pImageList)
{
	CPoint pt=StartPt;
	CRect mr(pt,pt);
	mr.InflateRect(5,5);
	pDC->Rectangle(mr);

	CPen mpen(PS_SOLID,2,RGB(0,0,0));
	CBrush mbrush(RGB(0,0,0));//::GetSysColor(COLOR_3DLIGHT));
	CPen *oldp=0;
	CBrush *oldb= 0;

	CPoint mpt = mr.CenterPoint();
	mpt.x = mr.right;

	for(int i = 0; i < m_cLUSet.GetSize();i++ )
	{
		CLearningUnit *plu = m_cLUSet.GetAt(i);
		plu->OnDraw(pDC,FALSE,i==m_pDoc->m_CurrInst,pImageList);
		CRect re = plu->GetLURect();
		CPoint npt = re.CenterPoint();
		npt.x=re.left;
		oldp=pDC->SelectObject(&mpen);
		oldb=pDC->SelectObject(&mbrush);
		pDC->MoveTo(mpt);
		pDC->LineTo(npt);
		DrawArrow(mpt,npt,4,8,0,pDC,0);
		pDC->SelectObject(oldb);
		pDC->SelectObject(oldp);
		npt.x=re.right;
		mpt=npt;
	}
}

//////////////////////////////////////////////////////////////////////
/// Called by the framework to update the position of each LU in the Context Model.
//////////////////////////////////////////////////////////////////////
void CInstModel::OnUpdatePosition()
{
	CPoint oldp=StartPt;

	for(int i = 0; i < m_cLUSet.GetSize();i++ )
	{
		CLearningUnit *pLU = m_cLUSet.GetAt(i);

		CRect mrect = pLU->GetLURect();

		int dx = mrect.left-oldp.x;
		int dy = mrect.CenterPoint().y - oldp.y; 
		int w = mrect.Width();
		pLU->TranslateBy(CSize(50-dx,-dy));
		mrect = pLU->GetLURect();
		oldp = mrect.CenterPoint();//pLU->GetLUCenter();
		oldp.x = mrect.right;
	}
}

//////////////////////////////////////////////////////////////////////
/// Called by the framework to add a new Learning Unit in the scenario.
/// \param mLU Pointer to a LU to add (if copy-and-paste), or NULL if a new LU is to be created.
/// \return A pointer to the newly added LU. 
///
/// This function is called either 
//////////////////////////////////////////////////////////////////////
CLearningUnit* CInstModel::AddLearningUnit(CLearningUnit* mLU/*=NULL*/)
{
//	double gg = m_pDoc->Cover;
//	CLearningUnit *pLU =NULL;
	CPoint oldp=StartPt;
	int nb = m_cLUSet.GetSize();

	if (nb != 0)
	{
		CLearningUnit *pLU = m_cLUSet.GetAt(nb-1);
		CRect mrect = pLU->GetLURect();
		oldp = mrect.CenterPoint();//pLU->GetLUCenter();
		oldp.x = mrect.right;
	}

	CRect mrect;
	CPoint pCenter;
	CLearningUnit *pLU =NULL;
	if (!mLU)
	{
		pLU = new CLearningUnit(m_pDoc,-1);
		pLU->m_nLUID = nb+1;
		pLU->m_sLUName.Format(_T("Unit %d"),nb+1);
		pLU->SetLUCenter(oldp);
		mrect = pLU->OnNewLearningUnit();
	}
	else 
	{
		pLU = mLU;
		mrect = pLU->GetLURect();
	}

	if (pLU)
	{
		m_cLUSet.Add(pLU);

		int dx = mrect.left-oldp.x;
		int dy = mrect.CenterPoint().y - oldp.y; 
		int w = mrect.Width();
		oldp.Offset(50-dx,dy);
		pLU->TranslateBy(CSize(50-dx,-dy));

		for (int i=0;i<m_cLUSet.GetSize();i++)
		{
			CLearningUnit *pLU2 = m_cLUSet.GetAt(i);
			if (pLU2) pLU2->m_nLUID = i;
		}

	}
	return pLU;
}


//////////////////////////////////////////////////////////////////////
/// Called by the framework when a new scenario is created (by opening a new document).
///
/// This function 
//////////////////////////////////////////////////////////////////////
void CInstModel::OnNewModel()
{
	CLearningUnit *pLU = AddLearningUnit();
	CSimModel *pSim = m_pDoc->m_pSimul;
	CMdEquation *pEqu = pSim->GetEquation(pLU->m_nSimulID);

	if (!pEqu) return;
	pEqu->OnUpdate(/*pSim,*/pLU,TRUE);
}

//////////////////////////////////////////////////////////////////////
/// Called by the framework when the LUs in the scenario are initialised (by opening a document).
///
/// This function 
//////////////////////////////////////////////////////////////////////
void CInstModel::OnInitiaUpdate()
{
//	m_pDoc->CarryingCap;
	for (int i=0;i<m_cLUSet.GetSize();i++)
	{
		CLearningUnit *pLU = m_cLUSet.GetAt(i);
		pLU->m_pLUDoc=m_pDoc;

		CSimModel *pSim = m_pDoc->m_pSimul;
		CMdEquation *pEqu = pSim->GetEquation(pLU->m_nSimulID);
		if (!pEqu) 
		{
			CString mstr;
			mstr.Format(_T("WARNING !\nThe equation of the Learning Unit n.%d (%s) is not defined.\nCheck its configuration"),
						i+1,pLU->m_sLUName);
			AfxMessageBox(mstr);
		}
		else
			pEqu->OnUpdate(/*pSim,*/pLU,TRUE);

	}

}



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL( CLearningUnit, CObject, VERSIONABLE_SCHEMA | 7)

#define DEFS 100
CSize defaultPos[]={
		CSize(-DEFS,0),CSize(0,-DEFS),
		CSize(DEFS,0),CSize(0,DEFS),
		CSize(-DEFS,-DEFS),CSize(DEFS,-DEFS),
		CSize(-DEFS,DEFS),CSize(DEFS,DEFS),
		CSize(-DEFS/2,-DEFS),CSize(DEFS/2,-DEFS),
		CSize(-DEFS,-DEFS/2),CSize(DEFS,-DEFS/2),
		CSize(-DEFS,DEFS/2),CSize(DEFS,DEFS/2),
		CSize(-DEFS/2,DEFS),CSize(DEFS/2,DEFS)
	};

CLearningUnit::CLearningUnit(CSimulDoc *pDoc,int SimulId)
{
	m_bShowAbbrev = FALSE;
	m_bCanStart = TRUE;
	m_pEquModel = NULL;
	m_pLUDoc = pDoc;
	m_nSimulID = SimulId;
	m_nbERMax = 0;
	m_nMaxTimer = 500;
	m_sLUHTML = _T("");
	m_bIsReady = TRUE;
	m_bExpSetLocked = TRUE;
	m_bFirstRun = FALSE;
	m_bCanCloseAll = FALSE;
	m_bActRequest = FALSE;
}


CLearningUnit::CLearningUnit(const CLearningUnit& mLU)
{
	m_pEquModel = mLU.m_pEquModel;
	m_pLUDoc = mLU.m_pLUDoc;
	m_bIsReady = FALSE;
	m_bCanStart = TRUE;
	m_bCanCloseAll = FALSE;
	m_bShowAbbrev = mLU.m_bShowAbbrev;
	m_bExpSetLocked = mLU.m_bExpSetLocked;
	m_bFirstRun = mLU.m_bFirstRun;

	m_sLUName = mLU.m_sLUName;
	//m_sName = _T("Copy of ") + m_sName;
	m_bActRequest = mLU.m_bActRequest;


	m_nSimulID = mLU.m_nSimulID;

	m_sLUDesc = mLU.m_sLUDesc;
	m_sLUGoal = mLU.m_sLUGoal;
	//m_sName = mLU.m_sName;

	m_nMaxTimer = mLU.m_nMaxTimer;
	m_nbERMax = mLU.m_nbERMax;

	m_sLUHTML = mLU.m_sLUHTML;

	m_locPt = mLU.m_locPt;
	m_locRect = mLU.m_locRect;
	m_inRect = mLU.m_inRect;

	for (int i=0;i<mLU.m_cMERSet.GetSize();i++)
	{
		CExternRepUnit* pER = mLU.m_cMERSet.GetAt(i);
		if (!pER) continue;
		CExternRepUnit* pcER = pER->Clone();
		m_cMERSet.Add(pcER);
	}
	for (i=0;i<mLU.m_cTBreakSet.GetSize();i++)
	{
		CTimerBreakPoint* pBP = mLU.m_cTBreakSet.GetAt(i);
		if (!pBP) continue;
		CTimerBreakPoint* pcBP = new CTimerBreakPoint();
		pcBP->m_nBPType = pBP->m_nBPType;
		pcBP->m_tBreakPt = pBP->m_tBreakPt;
		pcBP->m_sDesc = pBP->m_sDesc;
		pcBP->m_sHTML = pBP->m_sHTML;
		m_cTBreakSet.Add(pcBP);
	}
	for (i=0;i<mLU.m_cExpSet.GetSize();i++)
	{
		CMdExpertSet* pExp = mLU.m_cExpSet.GetAt(i);
		if (!pExp) continue;
		CMdExpertSet* pcExp = pExp->Clone();
		m_cExpSet.Add(pcExp);
	}
	POSITION pos = mLU.m_cDefaultPos.GetHeadPosition();
	while (pos)
	{
		CSize ms = mLU.m_cDefaultPos.GetNext(pos);
		m_cDefaultPos.AddTail(ms);
	}
}

CLearningUnit::~CLearningUnit()
{
	for(int i = 0; i < m_cMERSet.GetSize();i++ )
		delete m_cMERSet.GetAt(i);
	m_cMERSet.RemoveAll();
	for(i = 0; i < m_cTBreakSet.GetSize();i++ )
		delete m_cTBreakSet.GetAt(i);
	m_cTBreakSet.RemoveAll();
	ClearUnit();
}

CLearningUnit* CLearningUnit::Clone()
{
	CLearningUnit*	pLU = new CLearningUnit(*this);

	return pLU;
}


//////////////////////////////////////////////////////////////////////
/// Get the relevant data-points from the Learning Unit.
/// \param nIdx	Zero-based index of the data-point in the list (default=0).
/// \return		A pointer to the indexed instance of CMdData, NULL if none exists.
///
//////////////////////////////////////////////////////////////////////
CMdData* CLearningUnit::GetDataAt(int nIdx/*=0*/) 
{
	CMdData* pData = NULL;
	if (nIdx>=0 && nIdx < m_cDataPoints.GetSize())
		pData = m_cDataPoints.GetAt(nIdx);
	return pData;
}


/*BOOL CLearningUnit::CreateToolTip(CView *pView)
{
	return TRUE;
}

BOOL CLearningUnit::PreTranslateMessage(MSG* pMsg) 
{
	return TRUE;
}*/


void CLearningUnit::OnUpdateToolTip(CView *pView,CToolTipCtrl *pToolTip)
{
	if (!pToolTip || !pToolTip->m_hWnd) return;

//	int nb = pToolTip->GetToolCount();
//	pToolTip->DelTool(pView);
//	nb = pToolTip->GetToolCount();
	for(int i = 0; i < m_cMERSet.GetSize();i++ )
	{
		CExternRepUnit *obj = m_cMERSet.GetAt(i);
		if (!obj) continue;
		pToolTip->DelTool(pView,10*m_nLUID+i);
		pToolTip->AddTool(pView,
				obj->GetName(),
				obj->GetERRect(),
				10*(m_nLUID+1)+i);
	}
}


//////////////////////////////////////////////////////////////////////
/// Determines which item of the LU, if any, is at a specified position.
/// \param	ptHit	The point to be tested.
/// \param	mrect	Fill this CRect instance with the bounding box of the LU.
/// \return	a bipoint (X,Y), where X identifies the target LU and Y identifies the target ER.
///
/// If the hit-point is outside of the LU's bounding box, the function return (0,0).
/// If the hit-point is inside the LU's bounding box but ouside any ERs, the function return (-1,0).
/// If the hit-point is in the bounding box of one of the ERs of thid LU, the function return (X,Y), 
/// where X is the zero-based index of the target LU (#m_nLUID) ans Y is the one-based index of the target ER.
///
/// This function is called by the Context Design Interface (see CInstModel::CViewListModel()) to detect which LU and
/// which ER is clicked on by the user.
//////////////////////////////////////////////////////////////////////
CPoint CLearningUnit::HitTest(CPoint ptHit,CRect &mrect)
{ 
	//int rep = 0;
	CPoint rep(0,0);

	BOOL bIsInArea = m_locRect.PtInRect(ptHit);
	if (!bIsInArea) return rep;

	for(int i = 0; i < m_cMERSet.GetSize();i++ )
	{
		int gg = m_cMERSet.GetAt(i)->HitTest(ptHit,mrect);
		if (gg !=-1)
		{
			rep = CPoint(m_nLUID,i+1);
			mrect=m_locRect;
			return rep;
		}
	}
	rep = CPoint(-1,0);
	mrect=m_locRect;
	return rep;
}


/*UINT defaultName[]={
	129,
	130,
	IDR_SIMULTABLE,
	IDR_SIMULPIE,
	IDR_SIMULTEXT,
	IDR_SIMULGRAPH,
	IDR_SIMULANIM,
	IDR_SIMULPARAM_TMPL,
	IDR_SIMULTIMER,
	IDR_SIMULXYGRAPH
};*/

//////////////////////////////////////////////////////////////////////
/// Called by the framework when a new Learning Unit is added to the scenario.
/// \return The bouding rect enclosing the new LU.
///
//////////////////////////////////////////////////////////////////////
CRect CLearningUnit::OnNewLearningUnit()
{
	AddDefaultExpSet(m_nSimulID);

	int nb = sizeof(defaultPos) / sizeof(CSize);
	for (int i=0;i<nb;i++)
		m_cDefaultPos.AddTail(defaultPos[i]);
	
	CExternRepUnit *pERU = new CExternRepUnit(130);

	pERU->SetID(IDR_VIEWCONTROLLER);
	CString mstr;
	mstr.LoadString(IDR_VIEWCONTROLLER);
	// Find first CR
	int r = mstr.Find("\n");
	if (r != -1)
		mstr=mstr.Left(r);
	pERU->SetName(mstr);
	pERU->m_bOpenStart = TRUE;

	CPoint pt = GetLUCenter();
	CRect mrect = CRect(pt,pt);
	mrect.InflateRect(16,16);
	SetLURect(mrect);
	pERU->SetERRect(mrect);
	pERU->SetSelected(-1);
//	pERU->SetName("Model");
	m_cMERSet.Add(pERU);

	OnUpdateUnit();
	return GetLURect();
}

//////////////////////////////////////////////////////////////////////
/// Called by the framework when a Learning Unit is removed from the scenario.
//////////////////////////////////////////////////////////////////////
void CLearningUnit::ClearUnit()
{
	for(int i = 0; i < m_cDataPoints.GetSize();i++ )
		delete m_cDataPoints.GetAt(i);
	m_cDataPoints.RemoveAll();
	for(i = 0; i < m_cDefOutputSet.GetSize();i++ )
		delete m_cDefOutputSet.GetAt(i);
	m_cDefOutputSet.RemoveAll();
	for(i = 0; i < m_cExpSet.GetSize();i++ )
		delete m_cExpSet.GetAt(i);
	m_cExpSet.RemoveAll();
	for(i = 0; i < m_cDefExpSet.GetSize();i++ )
		delete m_cDefExpSet.GetAt(i);
	m_cDefExpSet.RemoveAll();

}

//////////////////////////////////////////////////////////////////////
/// Add a new Experimental Set to the Learning Unit.
/// \param nSimId	Zero-based index of the Mathematical Model to use to generate the Experimental Set.
/// \return			The actuel number of Experimental Sets in the Learning Unit.
///
/// Call CMdEquation::NewExpertSet() to create a new Experimental Set based on 
/// the initial conditions of the Simulation Model and add it in the list
/// (m_cExpSet).
//////////////////////////////////////////////////////////////////////
int CLearningUnit::AddDefaultExpSet(int nSimId)
{
	if (nSimId==-1) return 0;
	int nb =0;
	CSimModel *pSim = m_pLUDoc->m_pSimul;
	CMdEquation *pEqu = pSim->GetEquation(nSimId);

	if (!pEqu) return 0;
	
	CMdExpertSet* pSet = pEqu->NewExpertSet();

	nb = m_cExpSet.GetSize();
	CString mstr;
	mstr.Format(_T("Set %d"),nb);
	pSet->SetName(mstr);
	m_cExpSet.Add(pSet);
	return m_cExpSet.GetSize();
}

int CLearningUnit::RemoveExpSet(int n_expset)
{
	int nb =0;

	return nb;
}


//////////////////////////////////////////////////////////////////////
/// Add a new empty External Representation into the Learning Unit.
/// \return The number of External Representations in the Learning Unit.
///
/// Create a new instance of CExternRepUnit and add it in the list (#m_cMERSet). 
/// Place the ER in an available position in the interface (as stored in the #m_cDefaultPos
/// list) and update this list. 
//////////////////////////////////////////////////////////////////////
int CLearningUnit::AddExternalRep()
{
	//int nb = m_cMERSet.GetSize();
	//if (nb > 8) return FALSE;

	BOOL bIsEmpty = m_cDefaultPos.IsEmpty();
	if (bIsEmpty) return -1;

	CExternRepUnit *pERU = new CExternRepUnit();
	CPoint pt = GetLUCenter();
	CRect mrect(pt,pt);
	mrect.InflateRect(16,16);
	CSize mSize = m_cDefaultPos.RemoveHead();
	mrect.OffsetRect(mSize);
	pERU->SetERRect(mrect);
	//pERU->SetTranslation(false);
	m_cMERSet.Add(pERU);
	OnUpdateUnit();
	return m_cMERSet.GetSize();
}

//////////////////////////////////////////////////////////////////////
/// Remove the selected External Representation from the Learning Unit.
/// \param nbER Zero-based index of the ER to delete.
/// \return TRUE is the ER is deleted, FALSE otherwise.
///
/// Remove the corresponding ER from the list (m_cMERSet), delete the 
/// instance and update the list of available position for the interface.
//////////////////////////////////////////////////////////////////////
BOOL CLearningUnit::RemoveExternalRep(int nbER)
{
	int nSize = m_cMERSet.GetSize();
	if (nbER >= nSize) return FALSE;

	CExternRepUnit *pERU = m_cMERSet.GetAt(nbER);
	if (!pERU) return FALSE;

	CPoint pt1 = GetLUCenter();
	CRect mrect = pERU->GetERRect();
	CPoint pt2 = mrect.CenterPoint();
	CSize cs = pt2-pt1;
	m_cDefaultPos.AddHead(cs);
	m_cMERSet.RemoveAt(nbER);

	if (pERU->m_pExtView)
		pERU->m_pExtView->GetParentFrame()->SendMessage(WM_CLOSE);
	delete pERU;

	OnUpdateUnit();
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
/// Get the number of ERs actually open in the workspace.
/// \return		The number of ERs actually open.
///
//////////////////////////////////////////////////////////////////////
int	CLearningUnit::GetActiveERNumber()
{
	int nb = 0;
	int nbER = m_cMERSet.GetSize();
	for (int i=1;i<nbER;i++)
	{
		CExternRepUnit *pER=m_cMERSet.GetAt(i);
		if (pER->m_pExtView) nb++;

	}
	return nb;

}

void CLearningUnit::Serialize( CArchive& ar )
{
	CObject::Serialize(ar);
	if( ar.IsStoring( ) )
	{
		ar << m_sLUName;
		ar << m_nSimulID;
		ar << m_sLUDesc;
		ar << m_sLUGoal;
		ar << m_nMaxTimer;
		ar << m_nbERMax;
		ar << m_locPt;
		ar << m_locRect;
		ar << m_inRect;
		ar << m_sLUHTML;
		ar << m_bShowAbbrev;
		ar << m_bExpSetLocked;
		ar << m_bFirstRun;
		ar << m_bActRequest;

	}
	else
	{
		int nb = ar.GetObjectSchema();
		ar >> m_sLUName;
		ar >> m_nSimulID;
		ar >> m_sLUDesc;
		ar >> m_sLUGoal;
		ar >> m_nMaxTimer;
		ar >> m_nbERMax;
		ar >> m_locPt;
		ar >> m_locRect;
		ar >> m_inRect;
		if (nb>=3)
		{
			ar >> m_sLUHTML;
		}
		if (nb>=4)
		{
			ar >> m_bShowAbbrev;
		}
		m_bExpSetLocked = TRUE;
		m_bFirstRun = FALSE;
		if (nb>=5)
		{
			ar >> m_bExpSetLocked;
		}
		if (nb>=6)
		{
			ar >> m_bFirstRun;
		}
		if (nb>=7)
		{
			ar >> m_bActRequest;
		}
		m_bIsReady=TRUE;
		m_pEquModel = NULL;
		m_bCanStart = TRUE;
	}
	m_cMERSet.Serialize(ar);
	m_cTBreakSet.Serialize(ar);
	m_cExpSet.Serialize(ar);
	m_cDefaultPos.Serialize(ar);
//	m_cDataPoints.Serialize(ar);
//	m_cDefOutputSet.Serialize(ar);
	if (ar.IsLoading())
	{
		for(int i = 0; i < m_cTBreakSet.GetSize();i++ )
		{
			CTimerBreakPoint *pTB = m_cTBreakSet.GetAt(i);
//			m_cTTTT.SetAt(pTB->m_tBreakPt,*pTB);
		}

		for (i=0;i<m_cExpSet.GetSize();i++)
		{
			CMdExpertSet *pSet = m_cExpSet.GetAt(i);
			if (!pSet) continue;
			CMdExpertSet *pNewSet = pSet->Clone();
			m_cDefExpSet.Add(pNewSet);
		}

		int nbT = sizeof(defaultPos) / sizeof(CSize);
		int nbE = m_cDefaultPos.GetCount();
		int nbER = m_cMERSet.GetSize()-1;

		if (nbER+nbE != nbT)
		{
			m_cDefaultPos.RemoveAll();
			for (int i=0;i<nbT;i++)
				m_cDefaultPos.AddTail(defaultPos[i]);
			for (i=1;i<=nbER;i++)
			{
				CExternRepUnit *pERU = m_cMERSet.GetAt(i);
				CPoint pt = GetLUCenter();
				CPoint pt2 = pERU->GetERRect().CenterPoint();
				//CRect mrect(pt,pt);
				//mrect.InflateRect(16,16);
				CSize mSize = pt-pt2;//m_cDefaultPos.RemoveHead();
				mSize.cx = (mSize.cx / 80) * DEFS;
				mSize.cy = (mSize.cy / 80) * DEFS;
				POSITION pos = m_cDefaultPos.GetHeadPosition();
				while (pos)
				{
					CSize tt = m_cDefaultPos.GetAt(pos);
					if (tt==mSize)
					{
						m_cDefaultPos.RemoveAt(pos);
						break;
					}
					else m_cDefaultPos.GetNext(pos);
				}
				CRect mrect(pt,pt);
				mrect.InflateRect(16,16);
				mrect.OffsetRect(mSize);
				pERU->SetERRect(mrect);
			}
			OnUpdateUnit();
		}

	}
}


//////////////////////////////////////////////////////////////////////
/// Used by the Instructional Design Interface to update the interface of the LU.
///
/// 
//////////////////////////////////////////////////////////////////////
void CLearningUnit::OnUpdateUnit()
{
	int nb = m_cMERSet.GetSize();
	CRect tmprect(0,0,0,0);

	for (int i=0;i<nb;i++)
	{
		CRect ur=m_cMERSet.GetAt(i)->GetERRect();
		if (i)
			tmprect.UnionRect(tmprect,ur);
		else
			tmprect=ur;
	}
	m_inRect = tmprect;
	m_inRect.InflateRect(10,10);
	m_locRect = m_inRect;
}

//////////////////////////////////////////////////////////////////////
/// Used by the Instructional Design Interface to translate the interface of the LU.
/// \param cs Specify the dimension (width and height) to move the LU's interface.
///
/// 
//////////////////////////////////////////////////////////////////////
void CLearningUnit::TranslateBy(CSize cs)
{
	m_inRect.OffsetRect(cs);
	m_locRect.OffsetRect(cs);
	m_locPt.Offset(cs);
	int nb = m_cMERSet.GetSize();
	for (int i=0;i<nb;i++)
	{
		CRect ur=m_cMERSet.GetAt(i)->GetERRect();
		ur.OffsetRect(cs);
		m_cMERSet.GetAt(i)->SetERRect(ur);
	}
}

//////////////////////////////////////////////////////////////////////
/// Used by the Instructional Design Interface to draw the LU.
/// \param pDC A pointer to the Device Context used for drawing the view.
/// \param bUserMode TRUE if DEMIST is in Authoring Mode, FALSE otherwise.
/// \param bActive TRUE if the current LU is selected by the user.
/// \param pImageList A pointer to the image list containing the various LU's icons.
///
//////////////////////////////////////////////////////////////////////
void CLearningUnit::OnDraw(CDC* pDC,BOOL bUserMode,BOOL bActive,CImageList *pImageList)
{
	int S_arrow = 5,
		S_icon	= 16,
		S_margin = 3;
	int adjust = S_icon+S_margin;

	CSize ttsize = pDC->GetTextExtent(m_sLUName);
	CRect ttrect = GetLURect();
	ttrect.OffsetRect(0,-ttsize.cy-3);
	ttrect.bottom=ttrect.top+ttsize.cy+4;
	m_locRect = ttrect;
	m_locRect.bottom = m_inRect.bottom;
	CBrush bf(::GetSysColor(COLOR_3DLIGHT));
	CBrush bfA(::GetSysColor(COLOR_WINDOW));
	CBrush bfNA(::GetSysColor(COLOR_3DLIGHT));
	CBrush *oldb = pDC->SelectObject(&bf);

	int oldBkmod = pDC->SetBkMode(TRANSPARENT);

	pDC->Rectangle(ttrect);
	pDC->SelectObject(oldb);
	pDC->DrawText(m_sLUName,ttrect, DT_VCENTER | DT_CENTER | DT_NOPREFIX | DT_SINGLELINE  );

	if (bActive)
		oldb = pDC->SelectObject(&bfA);
	else
		oldb = pDC->SelectObject(&bfNA);
	pDC->Rectangle(m_inRect);
	pDC->SelectObject(oldb);

	int nb = m_cMERSet.GetSize();
	for (int i=0;i<nb;i++)
		m_cMERSet.GetAt(i)->OnDraw(pDC,bUserMode,pImageList);

	for (i=1;i<nb;i++)
	{
		int nbT = m_cMERSet.GetAt(i)->GetTranslationLevel();
		BOOL	bRetArrow = (nbT > 0);

		CPoint spt = m_cMERSet.GetAt(0)->GetERRect().CenterPoint();
		CPoint ept = m_cMERSet.GetAt(i)->GetERRect().CenterPoint();
		if (spt.x < ept.x)
		{
			spt.x+=adjust;
			ept.x-=adjust;

		}
		else if (spt.x > ept.x)
		{
			spt.x-=adjust;
			ept.x+=adjust;
		}
		if (spt.y > ept.y)
		{
			spt.y-=adjust;
			ept.y+=adjust;

		}
		else if (spt.y < ept.y)
		{
			spt.y+=adjust;
			ept.y-=adjust;
		}
		int arrowlength=2*S_arrow;  //arrowlength is the start arrow length

		CPen mpen(PS_SOLID,1,RGB(0,0,255));//::GetSysColor(COLOR_3DLIGHT));
		CBrush bf2(RGB(0,0,255));//::GetSysColor(COLOR_3DLIGHT));
		CPen *oldp=pDC->SelectObject(&mpen);
		pDC->MoveTo(spt);
		pDC->LineTo(ept);

		CRect arrowS(ept,ept),arrowE(spt,spt);
		arrowS.InflateRect(S_arrow,S_arrow);
		arrowE.InflateRect(S_arrow,S_arrow);

		oldb = pDC->SelectObject(&bf2);

		DrawArrow(spt,ept,S_arrow,arrowlength,bRetArrow,pDC,0);

		pDC->SelectObject(oldb);
		pDC->SelectObject(oldp);

		if (nbT != -1)
		{

			CPen mpen(PS_NULL,0,RGB(0,0,255));
			CBrush mbrush(RGB(255,255,255));
			CString mstr;
			mstr.Format(_T("%d"),nbT+1);
			
			CRect mrct(spt,ept);
			mrct.NormalizeRect();
			CPoint mpt1 = mrct.CenterPoint();
			mrct = CRect(mpt1,mpt1);

			COLORREF oldC = pDC->SetTextColor(RGB(0,0,255));
			pDC->DrawText(mstr,mrct,DT_CALCRECT);

			mrct.OffsetRect(1-mrct.Width()/2,-mrct.Height()/2);
			mrct.InflateRect(2,0,2,0);


			CBrush *oldB = NULL;//pDC->SelectObject(&mbrush);
			if (bActive)
				oldB = pDC->SelectObject(&bfA);
			else
				oldB = pDC->SelectObject(&bfNA);
			CPen *oldP = pDC->SelectObject(&mpen);
			pDC->Rectangle(mrct);
			pDC->SelectObject(oldP );
			pDC->SelectObject(oldB );

			pDC->DrawText(mstr,mrct,DT_VCENTER|DT_CENTER|DT_SINGLELINE);
			pDC->SetTextColor(oldC);
		}
	}
	pDC->SetBkMode(oldBkmod);
}

#define ID_FIRST	IDR_VIEWCONTROLLER
#define ID_LAST		ID_VIEW_TIMER

UINT	MyCmdList2[] ={
	ID_VIEW_CONTROLLER,
	ID_VIEW_CHART,
	ID_VIEW_MODEL,
	ID_VIEW_GRAPH,
	ID_VIEW_ANIM,
	ID_VIEW_TABLE,
	ID_VIEW_CHART,
	ID_VIEW_XYGRAPH,
	ID_VIEW_TIMER
};

#define NB_MENU_ER	7


//////////////////////////////////////////////////////////////////////
/// Create a menu containing all the ERs available in the Learning Unit.
/// \return		A pointer to the menu, NULL if it cannot be constructed.
///
/// This function creates a new menu that contains all the ERs (including the Controller) available
/// in the LU. Each item is defined by the name of the ER (see CExternRepUnit::GetName()), by its
/// icon (see CExternRepUnit::GetIcon()) and the command ID used to open the view (CExternRepUnit::m_CmDId).
//////////////////////////////////////////////////////////////////////
CMenu*	CLearningUnit::GetERsMenu()
{
	int nbER = m_cMERSet.GetSize();

	if (!nbER) return NULL;

	BCMenu *pMenu = new BCMenu();
	static UINT toolbars[]={ IDR_MAINFRAME  };
	pMenu->CreatePopupMenu();

	int nMenuNb = 1;
	for (int i=0;i<nbER;i++)
	{
		if (i==1)
			pMenu->AppendMenu(MF_SEPARATOR,0,NULL);

		CExternRepUnit *pER=m_cMERSet.GetAt(i);
		if (!pER) continue;

		UINT mID = pER->GetID();
		UINT nFlags = MF_STRING;
		if (mID == -1) nFlags |= MF_DISABLED;

		CString mstr;
		if (i) mstr.Format(_T("&%d "),nMenuNb++);
		else mstr = _T("&");
		mstr+= pER->GetName();

		CImageList pMenuIcon;
		pMenuIcon.Create(16,16,ILC_COLOR4|ILC_MASK,0,10);

		HICON hIcon = AfxGetApp()->LoadIcon(pER->GetIcon());
		pMenuIcon.Add(hIcon);
		
		UINT nFlag = MF_STRING;
		if (pER->m_pExtView)
			nFlag |= MF_CHECKED;
		pER->m_CmDId = ID_VIEW_EXTREP1 + i;// -1;
		if (mID < 132 || mID==-1)
			pMenu->AppendMenu(nFlag,pER->m_CmDId,mstr);
		else
			pMenu->AppendMenu(nFlag,pER->m_CmDId,mstr,&pMenuIcon,0);

	}
	return pMenu;
}

//////////////////////////////////////////////////////////////////////
/// Called by the framework to update the content of the View menu.
/// \param pView	A pointer to the view currently active in the workspace
/// 
//////////////////////////////////////////////////////////////////////
int CLearningUnit::UpdateERMenu(CView *pView)
{
	//// NVL GetMenu
	CWnd *pWnd = AfxGetMainWnd();
	if (!pWnd) return 0;

	CMenu *pTopMenu = pWnd->GetMenu();
	if (!pTopMenu) return 0;
	
	CMenu* pViewMenu = NULL;

	int iPos;
	for (iPos = pTopMenu->GetMenuItemCount()-1; iPos >= 0; iPos--)
	{
		CMenu* pMenu = pTopMenu->GetSubMenu(iPos);
		if (pMenu && pMenu->GetMenuItemID(0) == ID_VIEW_TOOLBAR)
		{
			pViewMenu = pMenu;
			break;
		}
	}
	if (!pViewMenu) return 0;

	int nStartMenu = NB_MENU_ER;
	if (CPref::g_nMode)
		nStartMenu-=2;

	//BOOL addSep = TRUE;
	int nbMenuItem = pViewMenu->GetMenuItemCount();
	if (nbMenuItem > nStartMenu) 
	{
		for (int i=nStartMenu;i<nbMenuItem;i++)
			pViewMenu->RemoveMenu(nStartMenu,MF_BYPOSITION);
	}
	int nbER = m_cMERSet.GetSize();

	pViewMenu->AppendMenu(MF_SEPARATOR);
	CString strAss;
	strAss.LoadString(IDS_VIEW_WELCOME);
	pViewMenu->AppendMenu(MF_STRING,ID_VIEW_WELCOME,strAss);

	if (!m_bCanStart) return 0;
	int nMenuNb = 1;
	for (int i=0;i<nbER;i++)
	{
		if (i==1)
		{
			pViewMenu->AppendMenu(MF_SEPARATOR);
		//	if (i) addSep = FALSE;
		}
		CExternRepUnit *pER=m_cMERSet.GetAt(i);
		if (!pER) continue;

		UINT mID = pER->GetID();
		UINT nFlags = MF_STRING;
		if (mID == -1) nFlags |= MF_DISABLED;

		CString mstr;
		if (i) mstr.Format(_T("&%d "),nMenuNb++);
		else mstr = _T("&");
		mstr+= pER->GetName();
		
		UINT nFlag = MF_STRING;
		if (pER->m_pExtView)
			nFlag |= MF_CHECKED;
		pER->m_CmDId = ID_VIEW_EXTREP1 + i;// -1;
		pViewMenu->AppendMenu(nFlag,
			pER->m_CmDId,//MyCmdList2[mID-ID_FIRST],
			mstr);
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////
/// Called by the framework to update each ER command in View menu.
/// \param pCmdUI	A pointer to the helper class of the command.
/// \return	TRUE if the command is updated, FALSE otherwise.
/// 
//////////////////////////////////////////////////////////////////////
BOOL CLearningUnit::UpdateLauchER(CCmdUI* pCmdUI)
{
	if (!m_bIsReady) return FALSE;

	BOOL bDone = FALSE;
	CExternRepUnit *pER=NULL;
	int nbER = m_cMERSet.GetSize();
	for (int i=0;i<nbER && !pER;i++)
	{
		pER=m_cMERSet.GetAt(i);
		if (pER->m_CmDId != pCmdUI->m_nID)
			pER = NULL;
	}
	if (pER && (pER->GetID() == 131 || pER->m_bIsReady))
	{
		pCmdUI->Enable(TRUE);
		BOOL bCheck = (BOOL)(pER->m_pExtView!=NULL);
		pCmdUI->SetCheck(bCheck);
		bDone = TRUE;	
	}

	return bDone;
}

//////////////////////////////////////////////////////////////////////
/// Called by the framework when a view has been closed, in order to update the ER.
/// \param pView	A pointer to the view beeing closed.
/// \return	TRUE if the view can be closed, FALSE otherwise.
/// 
//////////////////////////////////////////////////////////////////////
BOOL CLearningUnit::OnCloseER(CView *pView)
{
	BOOL bCanClose = TRUE;
	int nbER = m_cMERSet.GetSize();
	for (int i=0;i<nbER;i++)
	{
		CExternRepUnit *pER=m_cMERSet.GetAt(i);
		if (pER->m_pExtView == pView)
		{
			if (m_bCanCloseAll || CPref::g_nMode == 0)
			{
				pER->m_pExtView = NULL;
				bCanClose = TRUE;
			}
			else
			{
				if (!pER->m_bKeepOpen)
				{
					pER->m_pExtView = NULL;
					bCanClose = TRUE;
				}
				else
				{
					bCanClose = FALSE;
				}

			}
		}
	}
	return bCanClose;
}

//////////////////////////////////////////////////////////////////////
/// Called by the framework when ALL views have to be closed.
/// \return	TRUE if the views have been closed, FALSE otherwise.
/// 
//////////////////////////////////////////////////////////////////////
BOOL CLearningUnit::OnCloseAllER()
{
	m_bCanCloseAll = TRUE;

	int nb = m_cMERSet.GetSize();
	for (int i=0;i<nb;i++)
	{
		CExternRepUnit *pERU = m_cMERSet.GetAt(i);
		if (!pERU) return FALSE;
		if (pERU->m_pExtView)
			pERU->m_pExtView->GetParentFrame()->SendMessage(WM_CLOSE);
		pERU->m_pExtView = NULL;
	}
	m_bCanCloseAll = FALSE;
	return TRUE;
}



//////////////////////////////////////////////////////////////////////
/// Called by the framework to launch the designated External Representation.
/// \param	nID	The command ID of the ER to launch.
/// \return	TRUE if a relevant ER can be launched, FALSE otherwise.
///
/// This function is called by the framework when the command associated with an ER is activated 
/// and tries to open it. If the Er is already open, then it is activated in the foreground of the workspace.
/// If not, then the function checks if the number of simultaneous ERs is not reached and opens the ER.
/// Otherwise, an error meesage is displayed, asking the user to close one or more ERs in order to open
/// the new one.
//////////////////////////////////////////////////////////////////////
BOOL CLearningUnit::LauchER(UINT nID)
{
	if (!m_pLUDoc) return FALSE;

	if (!m_bIsReady) return FALSE;

	CExternRepUnit *pER=NULL;
	int nbER = m_cMERSet.GetSize();
	for (int i=0;i<nbER && !pER;i++)
	{
		pER=m_cMERSet.GetAt(i);
		if (pER->m_CmDId != nID)
			pER = NULL;
	}
	if (!pER) return FALSE;
	UINT nERID = pER->GetID();
	if (nERID == -1) return FALSE;
	nERID = MyCmdList2[nERID-ID_FIRST];

	if (pER->m_pExtView)
	{
		pER->m_pExtView->GetParentFrame()->ActivateFrame(SW_RESTORE);
		return TRUE;
	}

	if (nERID != ID_VIEW_CONTROLLER && m_nbERMax && (GetActiveERNumber() >= m_nbERMax))
	{
		CString str;
		str.LoadString(ERR_LUNIT_MAXERACTIVE);
		AfxGetMainWnd()->MessageBox(str,"Warning",MB_OK|MB_ICONWARNING);
		return FALSE;
	}

	CMvDocTemplate* pDocTmpl = (CMvDocTemplate*)m_pLUDoc->GetDocTemplate( ) ;
	if (pDocTmpl)
	{
		CDocFrameMgr *pDocFrMgr = pDocTmpl->GetAssociatedDocFrameMgr(m_pLUDoc);
		if (pDocFrMgr)
		{
			CFrame *pFrame = pDocFrMgr->FindFrame(nERID);
			if ( pFrame)
			{
				//CFrameWnd* pFrameWnd = pFrame->FindFrameWnd();

				// Frame not open.  Create it and validate it.
				// Create the window and validate it.
				CMDIChildWnd* pNewFrameWnd	= ( CMDIChildWnd* )( pFrame->CreateNewFrame( pFrame->GetDocument(), NULL ) );
				if ( !pNewFrameWnd)
				{
					// Must never happen.
					ASSERT( pNewFrameWnd );
					TRACE( "Unable to create frame wnd in CFrameTemplate::CreateFrame.\n", THIS_FILE, __LINE__ ); 
					return FALSE;
				}

				LONG dwStyle = ::GetWindowLong(pNewFrameWnd->GetSafeHwnd(),GWL_STYLE);
				if (pER->m_bKeepOpen && CPref::g_nMode)
				{
					dwStyle &= ~(WS_SYSMENU|WS_DLGFRAME|WS_BORDER);
					::SetWindowLong(pNewFrameWnd->GetSafeHwnd(),GWL_STYLE,dwStyle);
				}
				
				// Make sure that the frame is a mdi child window.
				ASSERT( pNewFrameWnd->IsKindOf( RUNTIME_CLASS( CMDIChildWnd ) ) );
							
				// Initialize the newly created frame.
				pFrame->InitialUpdateFrame( pNewFrameWnd, pFrame->GetDocument(), FALSE );

				CView *pView = pNewFrameWnd->GetActiveView(); //EnableWindow(false);
				if (pView)
				{
					pER->m_pExtView = pView;
					pView->SendMessage(DOC_UPDATE_VIEWCONTENT,(WPARAM)true,(LPARAM)pER);
				}

				pNewFrameWnd->SendMessageToDescendants( WM_INITIALUPDATE, 0, 0, TRUE, TRUE );

				// Retrieve the current window states.
				//CFrameWnd* pFrameWnd = pFrame->FindFrameWnd();
				int nStates = SW_RESTORE;
				if (pER->m_pExtViewPl.length != 0)
				{
					WINDOWPLACEMENT lpwndpl;
					pNewFrameWnd->GetWindowPlacement(&lpwndpl);
				/*	if (DYNAMIC_DOWNCAST(CChildFrameFix,pNewFrameWnd))
					{
						WINDOWPLACEMENT lpwndpl2 = pER->m_pExtViewPl;
						//lpwndpl.rcNormalPosition.
						CRect mrect = lpwndpl.rcNormalPosition;
						CRect mrect2 = lpwndpl2.rcNormalPosition;
						CPoint pt = mrect2.TopLeft();
						CRect mrect3(pt,pt);
						mrect3.InflateRect(0,0,mrect.Width(),mrect.Height());
						lpwndpl = lpwndpl2;
						lpwndpl.rcNormalPosition = mrect3;
					}
					else*/
						lpwndpl = pER->m_pExtViewPl;
					//pNewFrameWnd->SetWindowPlacement(&(pER->m_pExtViewPl));
					pNewFrameWnd->SetWindowPlacement(&lpwndpl);
					nStates = pER->m_pExtViewPl.showCmd;
				}
				pNewFrameWnd->SetWindowText(pER->GetName());

			/*	
				CWnd* pActWnd = ( ( CFrameWnd* ) AfxGetMainWnd() )->GetActiveFrame();
				if ( pActWnd ) 
				{
					WINDOWPLACEMENT wp;
					pActWnd->GetWindowPlacement( &wp );
					if ( wp.showCmd != SW_SHOWMINIMIZED )
					{
						nStates = wp.showCmd;
					}
				}*/

				// Activate the frame in the given states.
				pNewFrameWnd->ActivateFrame( nStates );
			}
			
		}
	}
	return TRUE;

}

CRuntimeClass *GetPaneRT(UINT nERID,CSimulDoc *m_pLUDoc)
{
	CRuntimeClass *pRT = NULL;
	CMvDocTemplate* pDocTmpl = (CMvDocTemplate*)m_pLUDoc->GetDocTemplate( ) ;
	if (!pDocTmpl) return NULL;

	CDocFrameMgr *pDocFrMgr = pDocTmpl->GetAssociatedDocFrameMgr(m_pLUDoc);
	if (!pDocFrMgr) return NULL;

	CFrame *pFrame = pDocFrMgr->FindFrame(nERID);
	if (!pFrame) return NULL;

	pRT = pFrame->GetViewRTC();

	return pRT;
}

BOOL CLearningUnit::OnCreateMultiPanes(CObject *pWndList,CCreateContext* pContext,CMDIChildWnd *pChildWnd)
{
	CPtrArray *m_pSplitterWnds = DYNAMIC_DOWNCAST( CPtrArray, pWndList);


	int nb = m_cMERSet.GetSize();

	//int nb2 = m_pSplitterWnds->GetSize();


/*	for (int i=0;i<nb2;i++)
	{
	   ST_SplitterWnd* m_pSplitterWnd2 = (ST_SplitterWnd*)m_pSplitterWnds->GetAt(i);

		int nCol,nRow;
		m_pSplitterWnd2->SideToRowCol(0, &nRow, &nCol);
		CWnd *pWnd = m_pSplitterWnd2->GetPane(nRow,nCol);
		m_pSplitterWnd2->SideToRowCol(1, &nRow, &nCol);
		CWnd *pWnd2 = m_pSplitterWnd2->GetPane(nRow,nCol);
	}*/

	// (((1V2)V3)H(4V5))


	ST_SplitterWnd* m_pSplitterWnd = new ST_SplitterWnd();
	
	m_pSplitterWnds->Add(m_pSplitterWnd);
	m_pSplitterWnd->Create(pChildWnd,NULL,NULL,pContext,false);

	int nbb = 1;
	ST_SplitterWnd* m_pSplitterWnd2  = NULL;
	ST_SplitterWnd* m_pSplitterWnd3  = NULL;
	ST_SplitterWnd* m_pSplitterWnd4  = NULL;
	ST_SplitterWnd* m_pSplitterWnd5  = NULL;

	{
		CExternRepUnit *pER=m_cMERSet.GetAt(nbb++);
		UINT nERID = pER->GetID();
		nERID = MyCmdList2[nERID-ID_FIRST];
		CRuntimeClass *pRT1 = GetPaneRT(nERID,m_pLUDoc);
	
		m_pSplitterWnd2 = m_pSplitterWnd->AddSubDivision(TOP_SIDE,pRT1,NULL,pContext,true);   
		{
			int nCol,nRow;
			m_pSplitterWnd2->SideToRowCol(TOP_SIDE, &nRow, &nCol);
			CWnd *pWnd = m_pSplitterWnd2->GetPane(nRow,nCol);
			pWnd->SendMessage(DOC_UPDATE_VIEWCONTENT,(WPARAM)true,(LPARAM)pER);
		}
	}

	{
		CExternRepUnit *pER=m_cMERSet.GetAt(nbb++);
		UINT nERID = pER->GetID();
		nERID = MyCmdList2[nERID-ID_FIRST];
		CRuntimeClass *pRT1 = GetPaneRT(nERID,m_pLUDoc);
	
		CExternRepUnit *pER1=m_cMERSet.GetAt(nbb++);
		nERID = pER1->GetID();
		nERID = MyCmdList2[nERID-ID_FIRST];
		CRuntimeClass *pRT2 = GetPaneRT(nERID,m_pLUDoc);

		m_pSplitterWnd3 = m_pSplitterWnd2->AddSubDivision(BOTTOM_SIDE,pRT1,pRT2,pContext,true);   
		{
			int nCol,nRow;
			m_pSplitterWnd3->SideToRowCol(TOP_SIDE, &nRow, &nCol);
			CWnd *pWnd = m_pSplitterWnd3->GetPane(nRow,nCol);
			pWnd->SendMessage(DOC_UPDATE_VIEWCONTENT,(WPARAM)true,(LPARAM)pER);
		}
		{
			int nCol,nRow;
			m_pSplitterWnd3->SideToRowCol(BOTTOM_SIDE, &nRow, &nCol);
			CWnd *pWnd = m_pSplitterWnd3->GetPane(nRow,nCol);
			pWnd->SendMessage(DOC_UPDATE_VIEWCONTENT,(WPARAM)true,(LPARAM)pER);
		}
	}

	{
		CExternRepUnit *pER=m_cMERSet.GetAt(nbb++);
		UINT nERID = pER->GetID();
		nERID = MyCmdList2[nERID-ID_FIRST];
		CRuntimeClass *pRT1 = GetPaneRT(nERID,m_pLUDoc);
	
		CExternRepUnit *pER2=m_cMERSet.GetAt(nbb++);
		nERID = pER2->GetID();
		nERID = MyCmdList2[nERID-ID_FIRST];
		CRuntimeClass *pRT2 = GetPaneRT(nERID,m_pLUDoc);

		m_pSplitterWnd4 = m_pSplitterWnd->AddSubDivision(BOTTOM_SIDE,pRT1,pRT2,pContext,true);   

		int nCC = m_pSplitterWnd4->GetColumnCount();
		int nRC = m_pSplitterWnd4->GetRowCount();
		{
			int nCol,nRow;
			m_pSplitterWnd4->SideToRowCol(LEFT_SIDE, &nRow, &nCol);
			CWnd *pWnd = m_pSplitterWnd4->GetPane(nRow,nCol);
			pWnd->SendMessage(DOC_UPDATE_VIEWCONTENT,(WPARAM)true,(LPARAM)pER);
		}

		{
			int nCol,nRow;
			m_pSplitterWnd4->SideToRowCol(RIGHT_SIDE, &nRow, &nCol);
			CWnd *pWnd = m_pSplitterWnd4->GetPane(nRow,nCol);
			pWnd->SendMessage(DOC_UPDATE_VIEWCONTENT,(WPARAM)true,(LPARAM)pER2);
		}
	}

	/*
//	CMvDocTemplate* pDocTmpl = (CMvDocTemplate*)m_pLUDoc->GetDocTemplate( ) ;
//	if (pDocTmpl)
//	{
//		CDocFrameMgr *pDocFrMgr = pDocTmpl->GetAssociatedDocFrameMgr(m_pLUDoc);
//		if (pDocFrMgr)
//		{
//			CFrame *pFrame = pDocFrMgr->FindFrame(nERID);
//			if ( pFrame)
//			{
//				CRuntimeClass *pRT = pFrame->GetViewRTC();
	
	CRuntimeClass *pRT = GetPaneRT(nERID,m_pLUDoc);
	m_pSplitterWnd2 = m_pSplitterWnd->AddSubDivision(LEFT_SIDE,pRT,NULL,pContext,false);   
	//*m_pSplitterWnd3= m_pSplitterWnd2->AddSubDivision(LEFT_SIDE,pRT,NULL,pContext,false);

	{
	int nCol,nRow;
	m_pSplitterWnd2->SideToRowCol(LEFT_SIDE, &nRow, &nCol);
	CWnd *pWnd = m_pSplitterWnd2->GetPane(nRow,nCol);
	pWnd->SendMessage(DOC_UPDATE_VIEWCONTENT,(WPARAM)true,(LPARAM)pER);
	}
			//}
		//}
	//}

	pER=m_cMERSet.GetAt(nbb++);
	nERID = pER->GetID();
	nERID = MyCmdList2[nERID-ID_FIRST];
//	if (pDocTmpl)
//	{
//		CDocFrameMgr *pDocFrMgr = pDocTmpl->GetAssociatedDocFrameMgr(m_pLUDoc);
//		if (pDocFrMgr)
//		{
//			CFrame *pFrame = pDocFrMgr->FindFrame(nERID);
//			if ( pFrame)
//			{
//				CRuntimeClass *pRT = pFrame->GetViewRTC();

	pRT = GetPaneRT(nERID,m_pLUDoc);
	m_pSplitterWnd3 = m_pSplitterWnd2->AddSubDivision(BOTTOM_SIDE,NULL,pRT,pContext,true);   

	//m_pSplitterWnd4= m_pSplitterWnd3->AddSubDivision(LEFT_SIDE,pRT2,pRT3,pContext,false);
	//m_pSplitterWnd5= m_pSplitterWnd2->AddSubDivision(BOTTOM_SIDE,pRT4,pRT5,pContext,false);
	m_pSplitterWnd2->LockBar(TRUE);

	{			int nCol,nRow;
				m_pSplitterWnd3->SideToRowCol(BOTTOM_SIDE, &nRow, &nCol);
				CWnd *pWnd = m_pSplitterWnd3->GetPane(nRow,nCol);
				pWnd->SendMessage(DOC_UPDATE_VIEWCONTENT,(WPARAM)true,(LPARAM)pER);
			}
	//	}
	//}

	pER=m_cMERSet.GetAt(nbb++);
	nERID = pER->GetID();
	nERID = MyCmdList2[nERID-ID_FIRST];

	CExternRepUnit *pER2=m_cMERSet.GetAt(nbb++);
	UINT nERID3 = pER2->GetID();
	nERID3 = MyCmdList2[nERID3-ID_FIRST];
//	if (pDocTmpl)
//	{
//		CDocFrameMgr *pDocFrMgr = pDocTmpl->GetAssociatedDocFrameMgr(m_pLUDoc);
//		if (pDocFrMgr)
//		{
//			CFrame *pFrame = pDocFrMgr->FindFrame(nERID);
//			CFrame *pFrame2 = pDocFrMgr->FindFrame(nERID3);
//			if ( pFrame && pFrame2)
//			{

	CRuntimeClass *pRT1 = GetPaneRT(nERID,m_pLUDoc);
	CRuntimeClass *pRT2 = GetPaneRT(nERID3,m_pLUDoc);
	m_pSplitterWnd4 = m_pSplitterWnd3->AddSubDivision(LEFT_SIDE,pRT1,pRT2,pContext,false);   

	{
				int nCol,nRow;
				m_pSplitterWnd4->SideToRowCol(LEFT_SIDE, &nRow, &nCol);
				CWnd *pWnd = m_pSplitterWnd4->GetPane(nRow,nCol);
				pWnd->SendMessage(DOC_UPDATE_VIEWCONTENT,(WPARAM)true,(LPARAM)pER);
				m_pSplitterWnd4->SideToRowCol(RIGHT_SIDE, &nRow, &nCol);
				pWnd = m_pSplitterWnd4->GetPane(nRow,nCol);
				pWnd->SendMessage(DOC_UPDATE_VIEWCONTENT,(WPARAM)true,(LPARAM)pER2);
			}
	//	}
	//}*/


   m_pSplitterWnds->Add(m_pSplitterWnd2);
   m_pSplitterWnds->Add(m_pSplitterWnd3);
   m_pSplitterWnds->Add(m_pSplitterWnd4);

   // Make the right pane switchable between two different views 
	//m_pSplitterWnd->AddView(RIGHT_SIDE,RUNTIME_CLASS(CViewAnimat),pContext);
	//m_pSplitterWnd->AddView(RIGHT_SIDE,RUNTIME_CLASS(CViewAnimat),pContext);


	m_pSplitterWnd->SetInitialStatus();

	return TRUE;
}

//////////////////////////////////////////////////////////////////////
/// If relevant, remove from the Learning Unit all the breakpoints defined by the learner.
///
/// This function is called when the simulation is reinitialised. It deletes from the Learning Unit 
/// (see #m_cTBreakSet) all the breakpoint that were created by the learner during the previous run
/// but leaves those created by the authors.
//////////////////////////////////////////////////////////////////////
void CLearningUnit::CleanTimerBP()
{
	CxTBreakSet temp;

	int nb = m_cTBreakSet.GetSize();
	for (int i=0;i<nb;i++)
	{
		CTimerBreakPoint* pTB = m_cTBreakSet.GetAt(i);
		if (!pTB) continue;

		if (pTB->m_bUserDefined)
			delete pTB;
		else
			temp.Add(pTB);
	}
	m_cTBreakSet.RemoveAll();
	m_cTBreakSet.Copy(temp);

	nb = m_cTBreakSet.GetSize();
}



//////////////////////////////////////////////////////////////////////
/// Get, if any, the breakpoint defined at the given time-step in the LU.
/// \param	nTime	Zero-based time-step that identifies the breakpoint to be looked up.
/// \param	pBP		A reference to a CTimerBreakPoint instance that will receive the breakpoint, if any.
/// \return	TRUE if a breakpoint can be found at the given time, FALSE otherwise.
///
//////////////////////////////////////////////////////////////////////
BOOL CLearningUnit::GetTimerBPAt(int nTime,CTimerBreakPoint& pBP)
{
	int nb = m_cTBreakSet.GetSize();
	BOOL bFound = FALSE;
	for (int i=0;i<nb;i++)
	{
		CTimerBreakPoint* pTB = m_cTBreakSet.GetAt(i);
		if (!pTB) continue;

		if (pTB->m_tBreakPt == nTime)
		{
			pBP = *pTB;
			bFound = TRUE;
			break;
		}

	}
	//BOOL bRes = m_cTTTT.Lookup(nTime,pBP);
	return bFound;
}



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL( CExternRepUnit, CObject, VERSIONABLE_SCHEMA | 5)

CExternRepUnit::CExternRepUnit(UINT id)
{
	m_IdMER = id;
	m_nIcon = id;
	m_CmDId = 0;
	m_bTrans = TRUE;
	m_nMode  = 0;
	m_bSelected = FALSE;
	m_bOpenStart = FALSE;
	m_bKeepOpen = FALSE;
	m_bIsReady = TRUE;
	m_sDesc = "??????";
	//m_dlgFormat = NULL;
	m_pExtView = NULL;
	m_pExtViewPl.length = 0;
//	m_pExtViewPl.flags = 0;
//	m_pExtViewPl.ptMaxPosition = 0;
//	m_pExtViewPl.ptMinPosition = 0;
//	m_pExtViewPl.rcNormalPosition= 0;
//	m_pExtViewPl.showCmd= 0;
	m_sCategory.Empty();
}

CExternRepUnit::~CExternRepUnit()
{
	for(int i = 0; i < m_cOutputSet.GetSize();i++ )
		delete m_cOutputSet.GetAt(i);
	m_cOutputSet.RemoveAll();
	for(i = 0; i < m_cFormatSet.GetSize();i++ )
		delete m_cFormatSet.GetAt(i);
	m_cOutputSet.RemoveAll();
//	if (m_dlgFormat) delete m_dlgFormat;
}

CExternRepUnit*	CExternRepUnit::Clone()
{
	CExternRepUnit*	pER = new CExternRepUnit();

	pER->m_sCategory = m_sCategory;

	pER->m_pExtView = NULL;
	pER->m_bSelected = FALSE;
	pER->m_CmDId = 0;
	pER->m_sDesc = m_sDesc;
	pER->m_IdMER = m_IdMER;
	pER->m_nIcon = m_nIcon;
	pER->m_bTrans = m_bTrans;
	pER->m_locRect = m_locRect;
	pER->m_pExtViewPl = m_pExtViewPl;
	pER->m_nMode  = m_nMode;
	pER->m_bOpenStart  = m_bOpenStart;
	//pER->m_pExtViewPl.length = 0;

	pER->m_bIsReady = FALSE;


	for(int i = 0; i < m_cOutputSet.GetSize();i++ )
	{
		COutcomes *pCO = m_cOutputSet.GetAt(i);
		COutcomes *pCO2 = NULL;
		if (pCO) 
			pCO2 = pCO->Clone();
		pER->m_cOutputSet.Add(pCO2);
	}
	for(i = 0; i < m_cFormatSet.GetSize();i++ )
	{
		CFormat *pFor = m_cFormatSet.GetAt(i);
		if (!pFor) continue;
		CFormat *pCFor = pFor->Clone();
		if (pCFor) pER->m_cFormatSet.Add(pCFor);
/*		COutcomes *pCO = m_cFormatSet.GetAt(i);
		COutcomes *pCO2 = NULL;
		if (pCO) 
			pCO2 = pCO->Clone();
		pER->m_cOutputSet.Add(pCO2);*/
	}
	return pER;
}


//////////////////////////////////////////////////////////////////////
/// Called by the framework to modify the ID and the name of the ER.
/// \param	mID	The new ID of the External Representation.
/// 
//////////////////////////////////////////////////////////////////////
void CExternRepUnit::SetID(UINT mId) 
{
	m_IdMER=mId;
	m_sDesc = "??????";
	if (m_IdMER == -1) return;

	CString mstring;
	BOOL res = mstring.LoadString(m_IdMER);
	if (res)
	{
		// Find first CR
		int r = mstring.Find("\n");
		if (r == -1)
			m_sDesc = mstring;
		else
			m_sDesc=mstring.Left(r);
	}
};

void CExternRepUnit::Serialize( CArchive& ar )
{
	CObject::Serialize(ar);

	if( ar.IsStoring( ) )
	{
		ar << m_sDesc;
		ar << m_IdMER;
		ar << m_bTrans;
		ar << m_bOpenStart;
		ar << m_bSelected;
		ar << m_locRect;
		ar << m_pExtViewPl.length;
		ar << m_pExtViewPl.flags;
		ar << m_pExtViewPl.ptMaxPosition;
		ar << m_pExtViewPl.ptMinPosition;
		ar << m_pExtViewPl.rcNormalPosition;
		ar << m_pExtViewPl.showCmd;
		ar << m_nMode;
		ar << m_sCategory;
		ar << m_nIcon;
		ar << m_bKeepOpen;

	}
	else
	{
		int nVersion = ar.GetObjectSchema();
		ar >> m_sDesc;
		ar >> m_IdMER;
		ar >> m_bTrans;
		ar >> m_bOpenStart;
		ar >> m_bSelected;
		if (m_bSelected != -1)
			m_bSelected = FALSE;
		ar >> m_locRect;
		if (nVersion >=2)
		{
			ar >> m_pExtViewPl.length;
			ar >> m_pExtViewPl.flags;
			ar >> m_pExtViewPl.ptMaxPosition;
			ar >> m_pExtViewPl.ptMinPosition;
			ar >> m_pExtViewPl.rcNormalPosition;
			ar >> m_pExtViewPl.showCmd;
			if (m_pExtViewPl.length)
				m_pExtViewPl.length = sizeof(WINDOWPLACEMENT);
		}
		if (nVersion >=3)
		{
			ar >> m_nMode;
			SetIcon(m_IdMER);
		}
		if (nVersion >=4)
		{
			ar >> m_sCategory;
			ar >> m_nIcon;
		}
		if (nVersion >=5)
		{
			ar >> m_bKeepOpen;
		}
		m_bIsReady = TRUE;
		//m_sCategory.Empty();
	}
	m_cOutputSet.Serialize(ar);
	m_cFormatSet.Serialize(ar);
}

//////////////////////////////////////////////////////////////////////
/// Used by the Instructional Design Interface to draw the ER in the LU.
/// \param pDC A pointer to the Device Context used for drawing the view.
/// \param bUserMode TRUE if DEMIST is in Authoring Mode, FALSE otherwise.
/// \param pImageList A pointer to the image list containing the various LU's icons.
///
//////////////////////////////////////////////////////////////////////
void CExternRepUnit::OnDraw(CDC* pDC,BOOL bUserMode/*=FALSE*/,CImageList* m_ImageList/*=NULL*/)
{
	CRect tmprect = m_locRect;
	tmprect.InflateRect(3,3);
	if (m_bSelected != -1)
	{
		COLORREF bcolor = ::GetSysColor(COLOR_3DFACE); 
		CBrush FixedBack(bcolor);

		BOOL bDrawSel = ((!bUserMode && m_bSelected) ||
						 (bUserMode && m_pExtView));

		//if (m_bSelected)
		if (bDrawSel)
		{
			//CBrush *oldb = pDC->SelectObject(&FixedBack);
			pDC->FillRect(tmprect,&FixedBack);
		}

		//pDC->DrawEdge(tmprect,(m_bSelected) ? EDGE_SUNKEN : EDGE_RAISED,BF_RECT );
		pDC->DrawEdge(tmprect,(bDrawSel) ? EDGE_SUNKEN : EDGE_RAISED,BF_RECT );
	}


	if (GetIcon() != -1)
	{
		CImageList m_IconList;
		m_IconList.Create(32,32,ILC_COLOR4|ILC_MASK,0,10);

		int nb = GetIcon();
		if (nb)
		{
			HICON hIcon = AfxGetApp()->LoadIcon(nb);
			m_IconList.Add(hIcon);
		}

		COLORREF oldc= m_IconList.SetBkColor(CLR_NONE);
		m_IconList.Draw(pDC,0,m_locRect.TopLeft(),ILD_NORMAL);
				//(m_CurrModel) ? ILD_NORMAL | ILD_SELECTED : ILD_NORMAL);
		m_IconList.SetBkColor(oldc);
	}


/*	if (m_IdMER != -1 && m_ImageList)
	{
		//CImageList m_ImageList;
		//m_ImageList.Create(IDB_ER_ICONS,32,0,RGB(192,192,192));
		COLORREF oldc= m_ImageList->SetBkColor(CLR_NONE);
		m_ImageList->Draw(pDC,m_IdMER - 129,m_locRect.TopLeft(),ILD_NORMAL);
				//(m_CurrModel) ? ILD_NORMAL | ILD_SELECTED : ILD_NORMAL);
		m_ImageList->SetBkColor(oldc);
	}*/
	else
	{
		pDC->MoveTo(m_locRect.left,m_locRect.top);
		pDC->LineTo(m_locRect.right,m_locRect.bottom);
		pDC->MoveTo(m_locRect.right,m_locRect.top);
		pDC->LineTo(m_locRect.left,m_locRect.bottom);
	}

}

//////////////////////////////////////////////////////////////////////
/// Determines if this ER is at a specified position.
/// \param	ptHit	The point to be tested.
/// \param	mrect	Fill this CRect instance with the bounding box of the ER.
/// \return	-1 if the ER is not at the position, the ID of the ER otherwise.
//////////////////////////////////////////////////////////////////////
int CExternRepUnit::HitTest(CPoint Hitpt,CRect &mrect)
{ 
	int rep = -1;
	if (m_locRect.PtInRect(Hitpt))
	{
		rep = m_IdMER+1;
		mrect = m_locRect;
	}
	return rep;
}


//////////////////////////////////////////////////////////////////////
/// Get the translation level associated with this ER.
/// \return	The translation level (see #TSTranslt)
//////////////////////////////////////////////////////////////////////
int	CExternRepUnit::GetTranslationLevel()
{
	int nTrans = -1;
	CFormatTranslation* pDlg = NULL;
	for (int i=0;i<m_cFormatSet.GetSize();i++)
	{
		CFormat *pPage = m_cFormatSet.GetAt(i);
		pDlg = DYNAMIC_DOWNCAST(CFormatTranslation,pPage);
		if (pDlg) break;
	}
	if (pDlg) nTrans = pDlg->m_nTranslation;
	return nTrans;
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL( CTimerBreakPoint, CObject, VERSIONABLE_SCHEMA | 2)

CTimerBreakPoint::CTimerBreakPoint()
{
	m_bUserDefined = FALSE;
	m_tBreakPt = -1;
	m_nBPType=BREAKP_INFO;			// Breakpoint nature
	m_sDesc=_T("");			// Breakpoint Description
	m_sHTML=_T("");			// Breakpoint Description
}


CTimerBreakPoint* CTimerBreakPoint::Clone()
{
	CTimerBreakPoint*	pLU = new CTimerBreakPoint();

	return pLU;
}

void CTimerBreakPoint::operator=(const CTimerBreakPoint& cTB)
{
	m_bUserDefined = FALSE;
	m_tBreakPt = cTB.m_tBreakPt;
	m_sDesc = cTB.m_sDesc;
	m_sHTML = cTB.m_sHTML;
	m_nBPType = cTB.m_nBPType;
}


void CTimerBreakPoint::Serialize(CArchive &ar)
{
	CObject::Serialize(ar);
	if( ar.IsStoring( ) )
	{
		ar << m_tBreakPt;
		ar << m_nBPType;
		ar << m_sDesc;
		ar << m_sHTML;
	}
	else
	{
		ar >> m_tBreakPt;
		ar >> m_nBPType;
		ar >> m_sDesc;
		UINT nAr = ar.GetObjectSchema();
		if (nAr==2)
		{
			ar >> m_sHTML;
		}
		m_bUserDefined = FALSE;
	}
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL( COutcomes, CObject, VERSIONABLE_SCHEMA | 1)
IMPLEMENT_SERIAL( COutputs, COutcomes, VERSIONABLE_SCHEMA | 1)

COutcomes::COutcomes()
{
	m_nExpSet=-1;
	m_nData=-1;
	m_nAllow= 0;
	m_nSelected= 0;
	//m_pFormat = NULL;

}

void COutcomes::Serialize(CArchive &ar)
{
	CObject::Serialize(ar);
	if( ar.IsStoring( ) )
	{
		ar << m_nExpSet;
		ar << m_nData;
		ar << m_nAllow;
		ar << m_nSelected;
	}
	else
	{
		ar >> m_nExpSet;
		ar >> m_nData;
		ar >> m_nAllow;
		ar >> m_nSelected;
	}
	//if (m_pFormat)
	//	m_pFormat->Serialize(ar);
}

COutcomes* COutcomes::Clone()
{
	COutcomes* pCOC = new COutcomes();
	pCOC->m_nAllow = m_nAllow;
	pCOC->m_nData = m_nData;
	pCOC->m_nExpSet = m_nExpSet;
	pCOC->m_nSelected = m_nSelected;
	return pCOC;
}
