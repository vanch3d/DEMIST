// PreyPredModel.cpp: implementation of the CPreyPredModel class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "simul.h"

#include <MInstruction\LearningUnit.h>
#include "PreyPredModel.h"

#include <math.h>


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


CString GetObjInfo(CModelObj *pObj,BOOL bExp=FALSE)
{
	CString mstr;

	if (!bExp)
		mstr = pObj->GetName();
	else
		mstr = pObj->GetDef();
	return mstr;
}
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL( CPreyPredModel, CSimModel, VERSIONABLE_SCHEMA | 1)

void CPreyPredModel::Serialize( CArchive& ar )
{
	CSimModel::Serialize(ar);
	if( ar.IsStoring( ) )
	{
	}
	else
	{
	}
}

CPreyPredModel::CPreyPredModel()
{
	m_iMaxTime = 500;
	m_sName = "Prey & predator";
}

CPreyPredModel::~CPreyPredModel()
{
}

void CPreyPredModel::OnNewModel()
{
	CSimModel::OnNewModel();

	// The equations
//	m_cEquSet.Add(new CPP_SgUnlimited());
//	m_cEquSet.Add(new CPP_SgLimited());
//	m_cEquSet.Add(new CPP_TwCompetition());
//	m_cEquSet.Add(new CPP_TwPredationUnltd());
//	m_cEquSet.Add(new CPP_TwPredation());
//	m_cEquSet.Add(new CPP_TwLotkaII());

//	for (int i=0;i<m_cEquSet.GetSize();i++)
//		m_cEquSet.GetAt(i)->OnNewEquation(this);
}

void CPreyPredModel::OnInitiaUpdate()
{
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL( CPP_SgUnlimited, CMdEquation, VERSIONABLE_SCHEMA | 1)

void CPP_SgUnlimited::Serialize( CArchive& ar )
{
	CMdEquation::Serialize(ar);
	if( ar.IsStoring( ) )
	{
	}
	else
	{
	}
}

CPP_SgUnlimited::CPP_SgUnlimited() : CMdEquation()
{
	m_sName = "Single-Species with unlimited growth";
}

CPP_SgUnlimited::CPP_SgUnlimited(CMdEquation*pEq) : CMdEquation(pEq)
{
	m_sName = "Single-Species with unlimited growth";
}



CMdEquation *CPP_SgUnlimited::Clone()
{
	CMdEquation *pEqu = new CPP_SgUnlimited(this);
	return pEqu;
}



CPP_SgUnlimited::~CPP_SgUnlimited()
{
}

double CPP_SgUnlimited::NFunc(double N,double param[])
{
	double	res = 0.0;
	double	b = param[0],
			d = param[1];
	res = (b-d) * N;
	return res;
}

void CPP_SgUnlimited::OnNewEquation(CSimModel *pModel)
{
	if (!pModel) return;

	m_nEquNb = 1;
	if (!m_sEqu.GetSize())
	{
	m_sEqu.Add(_T("dN / dt = (b - d) * N"));
	m_sEquDef.Add(_T("Species density calculation"));
	}
	if (!m_cVarSet.GetSize())
	{
	// The variables 
	CModelVar *pVar = NULL;

	//m_cVarSet.Add(pVar = new CModelVar("Time"));			
	m_cVarSet.Add(pVar = new CModelTime("Time"));			
	pVar->SetDef(_T("t"));
	pVar->SetAbbrev(_T("t"));
	//pVar->SetInitValue(-1);
	m_cVarSet.Add(pVar = new CModelVar("Pop. Density"));				
	pVar->SetInitValue(2);
	pVar->SetColor(RGB(255,0,0));
	pVar->SetDef(_T("N"));
	pVar->SetAbbrev(_T("N"));
	m_cVarSet.Add(pVar = new CModelVar("Increase Rate"));			
	pVar->SetDef(_T("BirthRate - DeathRate"));
	pVar->SetAbbrev(_T("r"));
	m_cVarSet.Add(pVar = new CModelVar("Pop. Change"));				
	pVar->SetDef(_T("IncRate * SpecPop"));
	pVar->SetAbbrev(_T("Change"));
	m_cVarSet.Add(pVar = new CModelVar("Doubling Time"));			
	pVar->SetDef(_T("ln(2) / PreyIncRate"));
	pVar->SetAbbrev(_T("TDouble"));
	}

		if (!m_cParSet.GetSize())
	{
	// The parametres
	CModelPar *pPar=NULL;
//	m_cParSet.Add(pPar= new CModelPar("Time",100));
//	pPar->SetInfos(100,100,200);
	m_cParSet.Add(pPar= new CModelPar("Birth Rate",0.4));
	pPar->SetDef(_T("b"));
	pPar->SetAbbrev(_T("b"));
	pPar->SetInfos(0.4,0.001,0.5);
	m_cParSet.Add(pPar= new CModelPar("Death Rate",0.1));
	pPar->SetDef(_T("d"));
	pPar->SetAbbrev(_T("d"));
	pPar->SetInfos(0.1,0.000,0.5);
	m_cParSet.Add(pPar= new CModelPar("Initial Pop.",20));		
	pPar->SetInfos(20,0,50);
	pPar->SetInitValue(1);
	pPar->SetAbbrev(_T("N(0)"));
	pPar->SetDef(_T("N(0)"));
	pPar->SetColor(RGB(255,0,0));
		}
}

void CPP_SgUnlimited::OnUpdateFrom(CLearningUnit *m_pLU,int nExpSet,int nTime,BOOL bInit)
{
	int m_nTimer = m_pLU->GetMaxTimer();

	int nb=0;
//	CMdData *pTime			= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *PreyFert		= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *PreyDeath		= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *PreyInit		= m_pLU->m_cDataPoints.GetAt(nb++);

	CMdData *pTime			= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *Prey			= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *PreyIncRate	= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *PreyChange		= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *Tdouble		= m_pLU->m_cDataPoints.GetAt(nb++);

	double  s_prey = m_pLU->m_cExpSet.GetAt(nExpSet)->GetValue(2);
	//PreyInit->SetAt(s_prey,0,nExpSet);
	//if (nTime == 0 && bInit)
	//	s_prey = s_prey;
	//else
	//	s_prey = Prey->GetAt(nTime,nExpSet);

	double pri,pf,pd;
	if (nTime==0 && bInit)
	{
		pf = m_pLU->m_cExpSet.GetAt(nExpSet)->GetValue(0);
		PreyFert->SetAt(pf,0,nExpSet);
		pd = m_pLU->m_cExpSet.GetAt(nExpSet)->GetValue(1);
		PreyDeath->SetAt(pd,0,nExpSet);
		pri = m_pLU->m_cExpSet.GetAt(nExpSet)->GetValue(2);
		PreyInit->SetAt(pri,0,nExpSet);

		s_prey = pri;
	}
	else
	{
		pri = PreyInit->GetAt(nTime,nExpSet);
		pf = PreyFert->GetAt(nTime,nExpSet);
		pd = PreyDeath->GetAt(nTime,nExpSet);

		if (nTime==0)
			s_prey = pri;
		else
			s_prey = Prey->GetAt(nTime,nExpSet);
	}

	for (int j=nTime;j<=m_nTimer;j++)
	{
		pTime->SetAt(j,j,nExpSet);
		Prey->SetAt(s_prey,j,nExpSet);
			
		PreyInit->SetAt(pri,j,nExpSet);
		PreyFert->SetAt(pf,j,nExpSet);
		PreyDeath->SetAt(pd,j,nExpSet);

		double nPreyIncRate,nPreyChange,nTdouble;
		double nnPreyChange =0;


		double h = 1.0;
		double param[]={pf,pd};
		double k1 = h*NFunc(s_prey,param);
		double k2 = h*NFunc(s_prey+k1/2,param);
		double k3 = h*NFunc(s_prey+k2/2,param);
		double k4 = h*NFunc(s_prey+k3,param);
		double res = s_prey + (k1 + 2*k2 + 2*k3 + k4)/6;

//		for (int k=0;k<10;k++)
//		{
			nPreyIncRate = pf - pd;
			nPreyChange = s_prey * nPreyIncRate;
			nTdouble = log(2) / nPreyIncRate;

//			nnPreyChange += 0.1 * nPreyChange;

//			double ss1 = s_prey + 0.1 * nPreyChange;
//			s_prey = ss1;
			s_prey = res;
//			if (!k)
//			{
				PreyIncRate->SetAt(nPreyIncRate,j,nExpSet);
				Tdouble->SetAt(nTdouble,j,nExpSet);
//			}
//		}
//		PreyChange->SetAt(nnPreyChange,j,nExpSet);
		PreyChange->SetAt(nPreyChange,j,nExpSet);
	}
}

CString CPP_SgUnlimited::GetFormatedText(int nLevel)
{
//	m_sText = GetFormatedEquation();
	BOOL bExp = (nLevel==2);
	int nb = 0;
	CModelObj *PTime			= m_cVarSet.GetAt(nb++);
	CModelObj *Prey				= m_cVarSet.GetAt(nb++);
	CModelObj *PreyIncRate		= m_cVarSet.GetAt(nb++);
	CModelObj *PreyChange		= m_cVarSet.GetAt(nb++);
	CModelObj *Tdouble			= m_cVarSet.GetAt(nb++);

	nb = 0;
	CModelObj *PreyFert		= m_cParSet.GetAt(nb++);
	CModelObj *PreyDeath	= m_cParSet.GetAt(nb++);
	CModelObj *PreyInit		= m_cParSet.GetAt(nb++);

	CString tstr;

	tstr.Format(_T("t"));
	PTime->SetDef(tstr);
	tstr.Format(_T("N"));
	Prey->SetDef(tstr);
	tstr.Format(_T("N[0]"));
	PreyInit->SetDef(tstr);
	tstr.Format(_T("b"));
	PreyFert->SetDef(tstr);
	tstr.Format(_T("d"));
	PreyDeath->SetDef(tstr);
	tstr.Format(_T("(%s - %s)"),GetObjInfo(PreyFert,bExp),GetObjInfo(PreyDeath,bExp));
	PreyIncRate->SetDef(tstr);
	tstr.Format(_T("%s * %s"),GetObjInfo(PreyIncRate,bExp),GetObjInfo(Prey,bExp));
	PreyChange->SetDef(tstr);
	tstr.Format(_T("ln(2) / %s"),GetObjInfo(PreyIncRate,bExp));
	Tdouble->SetDef(tstr);
	return m_sText;
}

CString CPP_SgUnlimited::GetFormatedEquation()
{
	CString mText;
	mText.Format(_T("    %s \t\t ; %s \r\n\r\n\r\n"),m_sEqu.GetAt(0),m_sEquDef.GetAt(0));

	return mText;
}

CRect CPP_SgUnlimited::DrawFlowChart(CDC* pDC)
{
	int nb = 0;
	CModelObj *PTime			= m_cVarSet.GetAt(nb++);
	CModelObj *Prey				= m_cVarSet.GetAt(nb++);
	CModelObj *PreyIncRate		= m_cVarSet.GetAt(nb++);
	CModelObj *PreyChange		= m_cVarSet.GetAt(nb++);
	CModelObj *Tdouble			= m_cVarSet.GetAt(nb++);

	nb = 0;
	CModelObj *PreyFert		= m_cParSet.GetAt(nb++);
	CModelObj *PreyDeath	= m_cParSet.GetAt(nb++);
	CModelObj *PreyInit		= m_cParSet.GetAt(nb++);


	// Draw Arrow
	CPen arrPen(PS_SOLID,3,RGB(0,0,255));
	CPen varPen(PS_SOLID,2,RGB(0,0,255));
	CPen arwPen(PS_SOLID,0,RGB(0,0,255));
	CBrush varBrush(RGB(0,0,255));

	CPen *oldP = pDC->SelectObject(&arrPen);
	pDC->MoveTo(20,40);
	pDC->LineTo(240,40);
	pDC->SelectObject(oldP);
	POINT polyPt[3] = {{240,40},{230,33},{230,47}};
	oldP = pDC->SelectObject(&arwPen);
	CBrush *oldB = pDC->SelectObject(&varBrush);
	pDC->Polygon(polyPt,3);
	pDC->SelectObject(oldP);
	pDC->SelectObject(oldB);

	CRect rRet(20,40,240,40);

	// Draw Prey
	rRet |= DrawNode(pDC,CPoint(240,25),Prey->GetName(),TT_TOP,TT_VAR);
	// Draw PreyChange
	rRet |= DrawNode(pDC,CPoint(110,30),PreyChange->GetName(),TT_TOP,TT_AGG);
	// Draw PreyIncRate
	rRet |= DrawNode(pDC,CPoint(110,110),PreyIncRate->GetName(),TT_BOTTOM,TT_AGG);
	// Draw TDouble
	rRet |= DrawNode(pDC,CPoint(110,190),Tdouble->GetName(),TT_BOTTOM,TT_AGG);
	// Draw PreyFert
	rRet |= DrawNode(pDC,CPoint(50,80),PreyFert->GetName(),TT_TOP,TT_PAR);
	// Draw PreyDeath
	rRet |= DrawNode(pDC,CPoint(50,140),PreyDeath->GetName(),TT_BOTTOM,TT_PAR);

	// Draw connectors
	// PretFert -> PreyIncRate
	CPoint mpt[2] = {CPoint(50+AGG_SIZE,80+AGG_SIZE/2),CPoint(110+2,110+AGG_SIZE/2-4)};
	rRet |= DrawConnector(pDC,2,mpt,TT_LINE);
	// PreyDeath -> PreyIncRate
	mpt[0] = CPoint(50+AGG_SIZE,140+AGG_SIZE/2);
	mpt[1] = CPoint(110+2,110+AGG_SIZE/2+4);
	rRet |= DrawConnector(pDC,2,mpt,TT_LINE);
	// PreyIncRate -> PreyChange
	mpt[0] = CPoint(110+AGG_SIZE/2,110);
	mpt[1] = CPoint(110+AGG_SIZE/2,30+AGG_SIZE);
	rRet |= DrawConnector(pDC,2,mpt,TT_LINE);
	// PreyIncRate -> TDouble
	mpt[0] = CPoint(110+AGG_SIZE/2,110+AGG_SIZE);
	mpt[1] = CPoint(110+AGG_SIZE/2,190);
	rRet |= DrawConnector(pDC,2,mpt,TT_LINE);
	// Prey -> PreyChange
//	CPoint polyPt3[4] = {CPoint(330,110),CPoint(270,150),CPoint(188,110),CPoint(188,94)};
	CPoint polyPt3[4] = {CPoint(240+VAR_SIZE/4,25+VAR_SIZE),
						 CPoint(240+VAR_SIZE/4-VAR_SIZE,25+2*VAR_SIZE),
						 CPoint(110+AGG_SIZE*3/2+7,30+2*AGG_SIZE-2),
						 CPoint(110+AGG_SIZE/2+7,30+AGG_SIZE-2)};

	rRet |= DrawConnector(pDC,4,polyPt3,TT_CURVE);

	return rRet;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL( CPP_SgUnlimitedII, CPP_SgUnlimited, VERSIONABLE_SCHEMA | 1)


CPP_SgUnlimitedII::CPP_SgUnlimitedII() : CPP_SgUnlimited()
{
	m_sName = "Single-Species with unlimited growth (and LinSol)";
}

CPP_SgUnlimitedII::CPP_SgUnlimitedII(CMdEquation*pEq) : CPP_SgUnlimited(pEq)
{
	m_sName = "Single-Species with unlimited growth (and LinSol)";
}

CPP_SgUnlimitedII::~CPP_SgUnlimitedII()
{
}

CMdEquation* CPP_SgUnlimitedII::Clone()
{
	CMdEquation *pEqu = new CPP_SgUnlimitedII(this);
	return pEqu;
}

void CPP_SgUnlimitedII::OnNewEquation(CSimModel *pModel)
{
	int nb = m_cVarSet.GetSize();
	CPP_SgUnlimited::OnNewEquation(pModel);

	if (!pModel) return;

	// The variables 
	CModelVar *pVar = NULL;

	if (!nb)
	{
		m_cVarSet.Add(pVar = new CModelVar("Linear Solution"));			
		pVar->SetDef(_T("N[0] * exp((b-d)*t)"));
		pVar->SetAbbrev(_T("LinSol"));
		m_cVarSet.Add(pVar = new CModelVar("Log(N)"));			
		pVar->SetDef(_T("log(N)"));
		pVar->SetAbbrev(_T("Log(N)"));
	}
}

void CPP_SgUnlimitedII::OnUpdateFrom(CLearningUnit *m_pLU,int nExpSet,int nTime,BOOL bInit)
{
	CPP_SgUnlimited::OnUpdateFrom(m_pLU,nExpSet,nTime,bInit);

	int m_nTimer = m_pLU->GetMaxTimer();

	//int nb=m_pLU->m_cDataPoints.GetSize();

	int nb=0;
	CMdData *PreyFert		= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *PreyDeath		= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *PreyInit		= m_pLU->m_cDataPoints.GetAt(nb++);

	CMdData *pTime			= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *Prey			= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *PreyIncRate	= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *PreyChange		= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *Tdouble		= m_pLU->m_cDataPoints.GetAt(nb++);

	CMdData *pLinSol = m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *pLogN = m_pLU->m_cDataPoints.GetAt(nb++);

	double pri,pf,pd;

	pri = PreyInit->GetAt(nTime,nExpSet);
	pf = PreyFert->GetAt(nTime,nExpSet);
	pd = PreyDeath->GetAt(nTime,nExpSet);

	for (int j=nTime;j<=m_nTimer;j++)
	{

		double nPreyIncRate = pf - pd;
		double nLinSol = pri * exp(nPreyIncRate * j);

		double nPrey = Prey->GetAt(j,nExpSet);
		double nLogN = log(nPrey);

		pLinSol->SetAt(nLinSol,j,nExpSet);
		pLogN->SetAt(nLogN,j,nExpSet);
	}

}

CString CPP_SgUnlimitedII::GetFormatedText(int nLevel)
{
	m_sText = CPP_SgUnlimited::GetFormatedText(nLevel);


	int nb = m_cVarSet.GetSize();
	CModelObj *PTime			= m_cVarSet.GetAt(0);
	CModelObj *Prey				= m_cVarSet.GetAt(1);

	CModelObj *pLinSol			= m_cVarSet.GetAt(nb-2);
	CModelObj *pLogN			= m_cVarSet.GetAt(nb-1);

	nb = 0;
	CModelObj *PreyFert		= m_cParSet.GetAt(nb++);
	CModelObj *PreyDeath	= m_cParSet.GetAt(nb++);
	CModelObj *PreyInit		= m_cParSet.GetAt(nb++);

	BOOL bExp = (nLevel==2);

	CString tstr;

	tstr.Format(_T("%s * exp((%s - %s) * %s)"),
						GetObjInfo(PreyInit,bExp),
						GetObjInfo(PreyFert,bExp),
						GetObjInfo(PreyDeath,bExp),
						GetObjInfo(PTime,bExp));
	pLinSol->SetDef(tstr);

	tstr.Format(_T("log(%s)"),
						GetObjInfo(Prey,bExp));
	pLinSol->SetDef(tstr);

	return m_sText;
}

CRect CPP_SgUnlimitedII::DrawFlowChart(CDC* pDC)
{
	return CPP_SgUnlimited::DrawFlowChart(pDC);
}

void CPP_SgUnlimitedII::Serialize( CArchive& ar )
{
	CPP_SgUnlimited::Serialize(ar);
	if( ar.IsStoring( ) )
	{
	}
	else
	{
	}
}



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL( CPP_SgLimited, CMdEquation, VERSIONABLE_SCHEMA | 1)

void CPP_SgLimited::Serialize( CArchive& ar )
{
	CMdEquation::Serialize(ar);
	if( ar.IsStoring( ) )
	{
	}
	else
	{
	}
}

CPP_SgLimited::CPP_SgLimited(CMdEquation*pEq) : CMdEquation(pEq)
{
	m_sName = "Single-Species with limited growth";
}

CPP_SgLimited::CPP_SgLimited() : CMdEquation()
{
	m_sName = "Single-Species with limited growth";
}

CMdEquation *CPP_SgLimited::Clone()
{
	CMdEquation *pEqu = new CPP_SgLimited(this);
	return pEqu;
}



CPP_SgLimited::~CPP_SgLimited()
{
}

double CPP_SgLimited::NFunc(double N,double param[])
{
	double res;
	double	b = param[0],
			d = param[1],
			K = param[2];
	res = (b-d) * N * (1 - N / K);
	return res;
}

void CPP_SgLimited::OnNewEquation(CSimModel *pModel)
{
	if (!pModel) return;

	m_nEquNb = 1;
		if (!m_sEqu.GetSize())
	{

	m_sEqu.Add(_T("dN / dt = (b - d) * N * ( 1 - N / K)"));
	m_sEquDef.Add(_T("Prey density evolution"));
		}
	if (!m_cVarSet.GetSize())
	{
	// The variables 
	CModelVar *pVar = NULL;
//	m_cVarSet.Add(pVar = new CModelVar("Time"));				
//	pVar->SetDef(_T("t"));
//	pVar->SetInitValue(-1);
	m_cVarSet.Add(pVar = new CModelTime("Time"));			
	pVar->SetDef(_T("t"));
	pVar->SetAbbrev(_T("t"));
	//pVar->SetInitValue(-1);

	m_cVarSet.Add(pVar = pVar = new CModelVar("Pop. Density"));				
	pVar->SetInitValue(3);
	pVar->SetDef(_T("N"));
	pVar->SetAbbrev(_T("N"));
	pVar->SetColor(RGB(255,0,0));
	m_cVarSet.Add(pVar = new CModelVar("Increase Rate"));			
	pVar->SetDef(_T("(b - d) * (1- N / K)"));
	pVar->SetAbbrev(_T("r"));
	m_cVarSet.Add(pVar = new CModelVar("Pop. Change"));				
	pVar->SetDef(_T("N * (b - d) * (1 - N / K)"));
	pVar->SetAbbrev(_T("Change"));
	m_cVarSet.Add(pVar = new CModelVar("Doubling Time"));			
	pVar->SetDef(_T("ln(2) / PreyIncRate"));
	pVar->SetAbbrev(_T("TDouble"));
	m_cVarSet.Add(pVar = new CModelVar("Env. Resistance"));			
	pVar->SetDef(_T("N / K"));
	pVar->SetAbbrev(_T("EnvRes"));
	}

		if (!m_cParSet.GetSize())
	{
	// The parametres
	CModelPar *pPar=NULL;
//	m_cParSet.Add(pPar = new CModelPar("Time",100));						
//	pPar->SetInfos(100,100,200);
	m_cParSet.Add(pPar= new CModelPar("Birth Rate",0.3));		// 1
	pPar->SetInfos(0.3,0.001,0.5);
	pPar->SetDef(_T("b"));
	pPar->SetAbbrev(_T("b"));
	m_cParSet.Add(pPar= new CModelPar("Death Rate",0.1));
	pPar->SetDef(_T("d"));
	pPar->SetAbbrev(_T("d"));
	pPar->SetInfos(0.1,0.000,0.5);
	m_cParSet.Add(pPar= new CModelPar("Carrying Cap.",150));		// 2
	pPar->SetInfos(150,50,450);
	pPar->SetDef(_T("K"));
	pPar->SetAbbrev(_T("K"));
	m_cParSet.Add(pPar= new CModelPar("Initial Pop.",20));		// 0
	pPar->SetInfos(20,0,50);
	pPar->SetInitValue(1);
	pPar->SetDef(_T("N(0)"));
	pPar->SetAbbrev(_T("N(0)"));
	pPar->SetColor(RGB(255,0,0));
	}
}

void CPP_SgLimited::OnUpdateFrom(CLearningUnit *m_pLU,int nExpSet,int nTime,BOOL bInit)
{
	int m_nTimer = m_pLU->GetMaxTimer();

	int nb=0;
//	CMdData *pTime			= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *PreyFert		= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *PreyDeath		= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *Carrying		= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *PreyInit		= m_pLU->m_cDataPoints.GetAt(nb++);

	CMdData *pTime			= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *Prey			= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *PreyIncRate	= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *PreyChange		= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *Tdouble		= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *TEnvRes		= m_pLU->m_cDataPoints.GetAt(nb++);

	double  s_prey = m_pLU->m_cExpSet.GetAt(nExpSet)->GetValue(3);
	//PreyInit->SetAt(s_prey,0,nExpSet);
	//if (nTime == 0)
	//	s_prey = s_prey;
	//else
	//	s_prey = Prey->GetAt(nTime,nExpSet);

	double pri,pf,pd,cc;
	if (nTime==0 && bInit)
	{
		pri = m_pLU->m_cExpSet.GetAt(nExpSet)->GetValue(3);
		PreyInit->SetAt(pri,0,nExpSet);
		pf = m_pLU->m_cExpSet.GetAt(nExpSet)->GetValue(0);
		PreyFert->SetAt(pf,0,nExpSet);
		pd = m_pLU->m_cExpSet.GetAt(nExpSet)->GetValue(1);
		PreyDeath->SetAt(pf,0,nExpSet);
		cc = m_pLU->m_cExpSet.GetAt(nExpSet)->GetValue(2);
		Carrying->SetAt(cc,0,nExpSet);

		s_prey = pri;
	}
	else
	{
		pri = PreyInit->GetAt(nTime,nExpSet);
		pf = PreyFert->GetAt(nTime,nExpSet);
		pd = PreyDeath->GetAt(nTime,nExpSet);
		cc = Carrying->GetAt(nTime,nExpSet);

		if (nTime==0)
			s_prey = pri;
		else
			s_prey = Prey->GetAt(nTime,nExpSet);
	}
	for (int j=nTime;j<=m_nTimer;j++)
	{
		pTime->SetAt(j,j,nExpSet);
		Prey->SetAt(s_prey,j,nExpSet);
			
		PreyInit->SetAt(pri,j,nExpSet);
		PreyFert->SetAt(pf,j,nExpSet);
		PreyDeath->SetAt(pd,j,nExpSet);
		Carrying->SetAt(cc,j,nExpSet);

		double nPreyIncRate,nPreyChange,nTdouble,nEnvRes;
		double nnPreyChange =0;

		double h = 1.0;
		double param[]={pf,pd,cc};
		double k1 = h*NFunc(s_prey,param);
		double k2 = h*NFunc(s_prey+k1/2,param);
		double k3 = h*NFunc(s_prey+k2/2,param);
		double k4 = h*NFunc(s_prey+k3,param);
		double res = s_prey + (k1 + 2*k2 + 2*k3 + k4)/6;

//		for (int k=0;k<10;k++)
//		{
			nPreyIncRate = (pf-pd) * ( 1 - s_prey / cc);
			nPreyChange = s_prey * nPreyIncRate;
			nTdouble = log(2) / nPreyIncRate;
			nEnvRes = s_prey / cc;

//			nnPreyChange += 0.1 * nPreyChange;

//			double ss1 = s_prey + 0.1 * nPreyChange;
//			s_prey = ss1;
			s_prey = res;
//			if (!k)
//			{
				PreyIncRate->SetAt(nPreyIncRate,j,nExpSet);
				Tdouble->SetAt(nTdouble,j,nExpSet);
				TEnvRes->SetAt(nEnvRes,j,nExpSet);
//			}
//		}

//		PreyChange->SetAt(nnPreyChange,j,nExpSet);
		PreyChange->SetAt(nPreyChange,j,nExpSet);
	}
}


CString CPP_SgLimited::GetFormatedText(int nLevel)
{
//	m_sText = GetFormatedEquation();
	BOOL bExp = (nLevel==2);
	int nb = 0;

	CModelObj *PTime			= m_cVarSet.GetAt(nb++);
	CModelObj *Prey				= m_cVarSet.GetAt(nb++);
	CModelObj *PreyIncRate		= m_cVarSet.GetAt(nb++);
	CModelObj *PreyChange		= m_cVarSet.GetAt(nb++);
	CModelObj *Tdouble			= m_cVarSet.GetAt(nb++);
	CModelObj *TEnvRes			= m_cVarSet.GetAt(nb++);

	nb = 0;
	CModelObj *PreyFert		= m_cParSet.GetAt(nb++);
	CModelObj *PreyDeath	= m_cParSet.GetAt(nb++);
	CModelObj *Carrying		= m_cParSet.GetAt(nb++);
	CModelObj *PreyInit		= m_cParSet.GetAt(nb++);

	CString tstr;

	tstr.Format(_T("(%s - %s) * (1 - %s / %s)"),GetObjInfo(PreyFert,bExp),GetObjInfo(PreyDeath,bExp),
					GetObjInfo(Prey,bExp),GetObjInfo(Carrying,bExp));
	PreyIncRate->SetDef(tstr);
	tstr.Format(_T("%s * %s"),GetObjInfo(Prey,bExp),GetObjInfo(PreyIncRate,bExp));
	PreyChange->SetDef(tstr);
	tstr.Format(_T("%s * %s"),GetObjInfo(Prey,bExp),GetObjInfo(PreyIncRate,bExp));
	PreyChange->SetDef(tstr);
	tstr.Format(_T("ln(2) / %s"),GetObjInfo(PreyIncRate,bExp));
	Tdouble->SetDef(tstr);
	tstr.Format(_T("%s / %s"),GetObjInfo(Prey,bExp),GetObjInfo(Carrying,bExp));
	TEnvRes->SetDef(tstr);

	tstr.Format(_T("t"));
	PTime->SetDef(tstr);
	tstr.Format(_T("N"));
	Prey->SetDef(tstr);
	tstr.Format(_T("N[0]"));
	PreyInit->SetDef(tstr);
	tstr.Format(_T("b"));
	PreyFert->SetDef(tstr);
	tstr.Format(_T("d"));
	PreyDeath->SetDef(tstr);
	tstr.Format(_T("K"));
	Carrying->SetDef(tstr);

	return m_sText;
}

CString CPP_SgLimited::GetFormatedEquation()
{
	CString mText;// =	"dN / dt = (f - g) * N * ( 1 - N / K) \r\n\r\n\r\n"; 
	mText.Format(_T("    %s \t\t ; %s \r\n\r\n\r\n"),m_sEqu.GetAt(0),m_sEquDef.GetAt(0));

	return mText;
}

CRect CPP_SgLimited::DrawFlowChart(CDC* pDC)
{
	int nb = 0;
	CModelObj *PTime			= m_cVarSet.GetAt(nb++);
	CModelObj *Prey				= m_cVarSet.GetAt(nb++);
	CModelObj *PreyIncRate		= m_cVarSet.GetAt(nb++);
	CModelObj *PreyChange		= m_cVarSet.GetAt(nb++);
	CModelObj *Tdouble			= m_cVarSet.GetAt(nb++);
	CModelObj *TEnvRes			= m_cVarSet.GetAt(nb++);

	nb = 0;
	CModelObj *PreyFert		= m_cParSet.GetAt(nb++);
	CModelObj *PreyDeath	= m_cParSet.GetAt(nb++);
	CModelObj *Carrying		= m_cParSet.GetAt(nb++);
	CModelObj *PreyInit		= m_cParSet.GetAt(nb++);

	// Draw Arrow
	CPen arrPen(PS_SOLID,3,RGB(0,0,255));
	CPen varPen(PS_SOLID,2,RGB(0,0,255));
	CPen arwPen(PS_SOLID,0,RGB(0,0,255));
	CBrush varBrush(RGB(0,0,255));

	CPen *oldP = pDC->SelectObject(&arrPen);
	pDC->MoveTo(20,40);
	pDC->LineTo(240,40);
	pDC->SelectObject(oldP);
	POINT polyPt[3] = {{240,40},{230,33},{230,47}};
	oldP = pDC->SelectObject(&arwPen);
	CBrush *oldB = pDC->SelectObject(&varBrush);
	pDC->Polygon(polyPt,3);
	pDC->SelectObject(oldP);
	pDC->SelectObject(oldB);

	CRect rRet(20,80,320,80);

	// Draw Prey
	rRet |= DrawNode(pDC,CPoint(240,25),Prey->GetName(),TT_TOP,TT_VAR);
	// Draw PreyChange
	rRet |= DrawNode(pDC,CPoint(110,30),PreyChange->GetName(),TT_TOP,TT_AGG);
	// Draw PreyIncRate
	rRet |= DrawNode(pDC,CPoint(110,110),PreyIncRate->GetName(),TT_BOTTOM,TT_AGG);
	// Draw TDouble
	rRet |= DrawNode(pDC,CPoint(110,190),Tdouble->GetName(),TT_BOTTOM,TT_AGG);
	// Draw PreyFert
	rRet |= DrawNode(pDC,CPoint(50,80),PreyFert->GetName(),TT_TOP,TT_PAR);
	// Draw PreyDeath
	rRet |= DrawNode(pDC,CPoint(50,140),PreyDeath->GetName(),TT_BOTTOM,TT_PAR);
	// Draw Carrying
	rRet |= DrawNode(pDC,CPoint(170,140),Carrying->GetName(),TT_BOTTOM,TT_PAR);
	// Draw TEnvRes
	rRet |= DrawNode(pDC,CPoint(240+VAR_SIZE/2-AGG_SIZE/2,140),TEnvRes->GetName(),TT_BOTTOM,TT_AGG);

	// Draw connectors
	// PretFert -> PreyIncRate
	CPoint mpt[2] = {CPoint(50+AGG_SIZE,80+AGG_SIZE/2),CPoint(110+2,110+AGG_SIZE/2-4)};
	rRet |= DrawConnector(pDC,2,mpt,TT_LINE);
	// Carrying -> PreyIncRate
	mpt[0] = CPoint(170,140+AGG_SIZE/2);
	mpt[1] = CPoint(110+AGG_SIZE,110+AGG_SIZE/2+4);
	rRet |= DrawConnector(pDC,2,mpt,TT_LINE);
	// PreyDeath -> PreyIncRate
	mpt[0] = CPoint(50+AGG_SIZE,140+AGG_SIZE/2);
	mpt[1] = CPoint(110+2,110+AGG_SIZE/2+4);
	rRet |= DrawConnector(pDC,2,mpt,TT_LINE);
	// PreyIncRate -> PreyChange
	mpt[0] = CPoint(110+AGG_SIZE/2,110);
	mpt[1] = CPoint(110+AGG_SIZE/2,30+AGG_SIZE);
	rRet |= DrawConnector(pDC,2,mpt,TT_LINE);
	// PreyIncRate -> TDouble
	mpt[0] = CPoint(110+AGG_SIZE/2,110+AGG_SIZE);
	mpt[1] = CPoint(110+AGG_SIZE/2,190);
	rRet |= DrawConnector(pDC,2,mpt,TT_LINE);
	// Prey -> TEnvRes
	mpt[0] = CPoint(240+VAR_SIZE/2,25+VAR_SIZE);
	mpt[1] = CPoint(240+VAR_SIZE/2,140);
	rRet |= DrawConnector(pDC,2,mpt,TT_LINE);
	// Carrying -> TEnvRes
	mpt[0] = CPoint(170+AGG_SIZE,140+AGG_SIZE/2);
	mpt[1] = CPoint(240+VAR_SIZE/2-AGG_SIZE/2,140+AGG_SIZE/2);
	rRet |= DrawConnector(pDC,2,mpt,TT_LINE);
	// Prey -> PreyChange
	CPoint polyPt3[4] = {CPoint(240+VAR_SIZE/8,25+VAR_SIZE),
						 CPoint(240+VAR_SIZE/8-VAR_SIZE,25+2*VAR_SIZE),
						 CPoint(110+AGG_SIZE*3/2+7,30+2*AGG_SIZE-2),
						 CPoint(110+AGG_SIZE/2+7,30+AGG_SIZE-2)};
	rRet |= DrawConnector(pDC,4,polyPt3,TT_CURVE);

	// Prey -> PreyIncRate
	CPoint polyPt2[4] = {CPoint(240+VAR_SIZE*3/8,25+VAR_SIZE),
						 CPoint(240+VAR_SIZE*3/8-VAR_SIZE,25+2*VAR_SIZE),
						 CPoint(110+AGG_SIZE+AGG_SIZE/2,110+AGG_SIZE/2-4),
						 CPoint(110+AGG_SIZE,110+AGG_SIZE/2-4)};
	rRet |= DrawConnector(pDC,4,polyPt2,TT_CURVE);

	return rRet;
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL( CPP_SgLimitedII, CPP_SgLimited, VERSIONABLE_SCHEMA | 1)


CPP_SgLimitedII::CPP_SgLimitedII() : CPP_SgLimited()
{
	m_sName = "Single-Species with limited growth (and r)";
}

CPP_SgLimitedII::CPP_SgLimitedII(CMdEquation*pEq) : CPP_SgLimited(pEq)
{
	m_sName = "Single-Species with limited growth (and r)";
}

CPP_SgLimitedII::~CPP_SgLimitedII()
{
}

CMdEquation* CPP_SgLimitedII::Clone()
{
	CMdEquation *pEqu = new CPP_SgLimitedII(this);
	return pEqu;
}

void CPP_SgLimitedII::OnNewEquation(CSimModel *pModel)
{
	int nb = m_cVarSet.GetSize();
	CPP_SgLimited::OnNewEquation(pModel);

	if (!pModel) return;

	// The variables 
	CModelVar *pVar = NULL;

	if (!nb)
	{
		m_cVarSet.Add(pVar = new CModelVar("Increase Rate"));			
		pVar->SetDef(_T("BirthRate - DeathRate"));
		pVar->SetAbbrev(_T("r"));
	}
}

void CPP_SgLimitedII::OnUpdateFrom(CLearningUnit *m_pLU,int nExpSet,int nTime,BOOL bInit)
{
	CPP_SgLimited::OnUpdateFrom(m_pLU,nExpSet,nTime,bInit);

	int m_nTimer = m_pLU->GetMaxTimer();

	//int nb=m_pLU->m_cDataPoints.GetSize();

	int nb=0;
//	CMdData *pTime			= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *PreyFert		= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *PreyDeath		= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *Carrying		= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *PreyInit		= m_pLU->m_cDataPoints.GetAt(nb++);

	CMdData *pTime			= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *Prey			= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *PreyIncRate	= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *PreyChange		= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *Tdouble		= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *TEnvRes		= m_pLU->m_cDataPoints.GetAt(nb++);

	CMdData *pR = m_pLU->m_cDataPoints.GetAt(nb++);

	for (int j=nTime;j<=m_nTimer;j++)
	{
		double nPreyDeath = PreyDeath->GetAt(j,nExpSet);
		double nPreyFert = PreyFert->GetAt(j,nExpSet);
		double nR= nPreyFert-nPreyDeath;

		pR->SetAt(nR,j,nExpSet);
	}

}

CString CPP_SgLimitedII::GetFormatedText(int nLevel)
{
	m_sText = CPP_SgLimited::GetFormatedText(nLevel);

	int nb = 0;

	CModelObj *PTime			= m_cVarSet.GetAt(nb++);
	CModelObj *Prey				= m_cVarSet.GetAt(nb++);
	CModelObj *PreyIncRate		= m_cVarSet.GetAt(nb++);
	CModelObj *PreyChange		= m_cVarSet.GetAt(nb++);
	CModelObj *Tdouble			= m_cVarSet.GetAt(nb++);
	CModelObj *TEnvRes			= m_cVarSet.GetAt(nb++);
	CModelObj *pR		= m_cVarSet.GetAt(nb++);

	nb = 0;
	CModelObj *PreyFert		= m_cParSet.GetAt(nb++);
	CModelObj *PreyDeath	= m_cParSet.GetAt(nb++);
	CModelObj *Carrying		= m_cParSet.GetAt(nb++);
	CModelObj *PreyInit		= m_cParSet.GetAt(nb++);
		
	BOOL bExp = (nLevel==2);

	CString tstr;

	tstr.Format(_T("%s - %s"),
						GetObjInfo(PreyFert,bExp),
						GetObjInfo(PreyDeath,bExp));
	pR->SetDef(tstr);
	return m_sText;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL( CPP_TwCompetition, CMdEquation, VERSIONABLE_SCHEMA | 1)

void CPP_TwCompetition::Serialize( CArchive& ar )
{
	CMdEquation::Serialize(ar);
	if( ar.IsStoring( ) )
	{
	}
	else
	{
	}
}

CPP_TwCompetition::CPP_TwCompetition() : CMdEquation()
{
	m_sName = "Two-Species Competition";
}

CPP_TwCompetition::CPP_TwCompetition(CMdEquation*pEq) : CMdEquation(pEq)
{
	m_sName = "Two-Species Competition";
}

CPP_TwCompetition::~CPP_TwCompetition()
{
}

CMdEquation *CPP_TwCompetition::Clone()
{
	CMdEquation *pEqu = new CPP_TwCompetition(this);
	return pEqu;
}

double CPP_TwCompetition::NFunc(double N,double param[])
{
	double res;
	double	r1 = param[0],
			a = param[1],
			N2 = param[2],
			K1 = param[3];
	res = r1 * N * (1 - (N + a * N2) / K1);
	return res;
}

double CPP_TwCompetition::PFunc(double P,double param[])
{
	double res;
	double	r2 = param[0],
			b = param[1],
			N1 = param[2],
			K2 = param[3];
	res = r2 * P * (1 - (P + b * N1) / K2);
	return res;
}


void CPP_TwCompetition::OnNewEquation(CSimModel *pModel)
{
	if (!pModel) return;

	m_nEquNb = 2;

	if (!m_sEqu.GetSize())
	{
	m_sEqu.Add(_T("dN1 / dt = r1 * N1 * (1 - (N1 + a * N2) / K1)"));
	m_sEquDef.Add(_T("Species 1 density calculation"));
	m_sEqu.Add(_T("dN2 / dt = r2 * N2 * (1 - (N2 + b * N1) / K2)"));
	m_sEquDef.Add(_T("Species 2 density calculation"));
	}

	if (!m_cVarSet.GetSize())
	{
		// The variables 
		CModelVar *pVar = NULL;
	//	m_cVarSet.Add(pVar = new CModelVar("Time"));			
	//	pVar->SetDef(_T("t"));
	//	pVar->SetInitValue(-1);
	m_cVarSet.Add(pVar = new CModelTime("Time"));			
	pVar->SetDef(_T("t"));
	pVar->SetAbbrev(_T("t"));

	m_cVarSet.Add(pVar = new CModelVar("Pop. Density (N)"));				
	pVar->SetInitValue(6);
	pVar->SetDef(_T("N"));
	pVar->SetAbbrev(_T("N"));
	pVar->SetColor(RGB(255,0,0));
	m_cVarSet.Add(pVar = new CModelVar("Pop. Density (M)"));				
	pVar->SetInitValue(7);
	pVar->SetDef(_T("M"));
	pVar->SetAbbrev(_T("M"));
	pVar->SetColor(RGB(0,128,0));
	m_cVarSet.Add(pVar = new CModelVar("IncRate(N)"));				
	pVar->SetDef(_T("r1 * (1 - (N1 + a * N2) / K1)"));
	pVar->SetAbbrev(_T("IncRate(N)"));
	m_cVarSet.Add(pVar = new CModelVar("IncRate(M)"));				
	pVar->SetDef(_T("r2 * (1 - (N2 + b * N1) / K2)"));
	pVar->SetAbbrev(_T("IncRate(M)"));
	m_cVarSet.Add(pVar = new CModelVar("Change(N)"));				
	pVar->SetDef(_T("r1 * N1 * (1 - (N1 + a * N2) / K1)"));
	pVar->SetAbbrev(_T("Change(N)"));
	m_cVarSet.Add(pVar = new CModelVar("Change(M)"));				
	pVar->SetDef(_T("r2 * N2 * (1 - (N2 + b * N1) / K2)"));
	pVar->SetAbbrev(_T("Change(M)"));
	m_cVarSet.Add(pVar = new CModelVar("TDouble(N)"));			
	pVar->SetDef(_T("ln(2) / S1_IncRate"));
	pVar->SetAbbrev(_T("Change(N)"));
	m_cVarSet.Add(pVar = new CModelVar("TDouble(M)"));			
	pVar->SetDef(_T("ln(2) / S2_IncRate"));
	pVar->SetAbbrev(_T("Change(M)"));
	m_cVarSet.Add(pVar = new CModelVar("K(N)/K(M)"));			
	pVar->SetDef(_T("K1/K2"));
	pVar->SetAbbrev(_T("K(N)/K(M)"));
	m_cVarSet.Add(pVar = new CModelVar("1/beta"));			
	pVar->SetDef(_T("1/b"));
	pVar->SetAbbrev(_T("1/beta"));

	m_cVarSet.Add(pVar = new CModelVar("Stab(N)"));			
	pVar->SetDef(_T("(K1 - a*K2)/(1 - a*b)"));
	pVar->SetAbbrev(_T("Stab(N)"));
	m_cVarSet.Add(pVar = new CModelVar("Stab(M)"));			
	pVar->SetDef(_T("(K2 - b*K1)/(1 - a*b)"));
	pVar->SetAbbrev(_T("Stab(M)"));

	m_cVarSet.Add(pVar = new CModelVar("dN/dt=0 (N)"));			
	pVar->SetDef(_T("K1/a"));
	pVar->SetAbbrev(_T("dN/dt=0 (N)"));
	pVar->SetColor(RGB(255,0,0));
	m_cVarSet.Add(pVar = new CModelVar("dN/dt=0 (M)"));			
	pVar->SetDef(_T("K1"));
	pVar->SetAbbrev(_T("dN/dt=0 (M)"));
	pVar->SetColor(RGB(255,0,0));
	m_cVarSet.Add(pVar = new CModelVar("dM/dt=0 (N)"));			
	pVar->SetDef(_T("K2"));
	pVar->SetAbbrev(_T("dM/dt=0 (N)"));
	pVar->SetColor(RGB(0,128,0));
	m_cVarSet.Add(pVar = new CModelVar("dM/dt=0 (M)"));			
	pVar->SetDef(_T("K2/b"));
	pVar->SetAbbrev(_T("dM/dt=0 (M)"));
	pVar->SetColor(RGB(0,128,0));
	}

	if (!m_cParSet.GetSize())
	{

	// The parametres
	CModelPar *pPar=NULL;
//	m_cParSet.Add(pPar= new CModelPar("Time",100));
//	pPar->SetInfos(100,100,200);
	m_cParSet.Add(pPar= new CModelPar("Intrinsic Rate of Increase(N)"));
	pPar->SetDef(_T("r(N)"));
	pPar->SetAbbrev(_T("r(N)"));
	pPar->SetInfos(0.4,0.001,0.9);
	m_cParSet.Add(pPar= new CModelPar("Intrinsic Rate of Increase(M)"));
	pPar->SetDef(_T("r(M)"));
	pPar->SetAbbrev(_T("r(M)"));
	pPar->SetInfos(0.2,0.001,0.9);
	m_cParSet.Add(pPar= new CModelPar("Carrying Cap.(N)"));
	pPar->SetDef(_T("K(N)"));
	pPar->SetAbbrev(_T("K(N)"));
	pPar->SetInfos(150,50,500);
	m_cParSet.Add(pPar= new CModelPar("Carrying Cap.(M)"));
	pPar->SetDef(_T("K(M)"));
	pPar->SetAbbrev(_T("K(M)"));
	pPar->SetInfos(150,50,500);
	m_cParSet.Add(pPar= new CModelPar("Competition Coef. (N)"));
	pPar->SetDesc(_T("Competition coefficient for the first species"));
	pPar->SetDef(_T("alpha"));
	pPar->SetAbbrev(_T("alpha"));
	pPar->SetInfos(0.9,0.000,1.0);
	m_cParSet.Add(pPar= new CModelPar("Competition Coef. (M)"));
	pPar->SetDesc(_T("Competition coefficient for the second species"));
	pPar->SetDef(_T("beta"));
	pPar->SetAbbrev(_T("beta"));
	pPar->SetInfos(0.9,0.000,1.0);
	m_cParSet.Add(pPar= new CModelPar("Initial Pop. (N)",20));		
	pPar->SetInfos(20,0,50);
	pPar->SetInitValue(1);
	pPar->SetAbbrev(_T("N(0)"));
	pPar->SetDef(_T("N(0)"));
	pPar->SetColor(RGB(255,0,0));
	m_cParSet.Add(pPar= new CModelPar("Initial Pop. (M)",20));		
	pPar->SetInfos(20,0,50);
	pPar->SetInitValue(2);
	pPar->SetDef(_T("M(0)"));
	pPar->SetAbbrev(_T("M(0)"));
	pPar->SetColor(RGB(0,128,0));
	}
}

void CPP_TwCompetition::OnUpdateFrom(CLearningUnit *m_pLU,int nExpSet,int nTime,BOOL bInit)
{
	int m_nTimer = m_pLU->GetMaxTimer();

	int nb=0;
	CMdData *S1Growth		= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *S2Growth		= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *S1CC			= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *S2CC			= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *S1alpha		= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *S2beta 		= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *S1init			= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *S2init			= m_pLU->m_cDataPoints.GetAt(nb++);

	CMdData *pTime			= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *S1				= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *S2				= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *S1IncRate		= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *S2IncRate		= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *S1Change		= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *S2Change		= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *S1Double		= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *S2Double		= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *K1dK2			= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *invS2beta		= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *S1Stab			= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *S2Stab			= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *dN1N1			= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *dN1N2			= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *dN2N1			= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *dN2N2			= m_pLU->m_cDataPoints.GetAt(nb++);

	double  s_p1 = m_pLU->m_cExpSet.GetAt(nExpSet)->GetValue(6);
	double  s_p2 = m_pLU->m_cExpSet.GetAt(nExpSet)->GetValue(7);
	//S1init->SetAt(s_p1,0,nExpSet);
	//S2init->SetAt(s_p2,0,nExpSet);
	//if (nTime == 0)
	//{
	//	s_p1 = s_p1;
	//	s_p2 = s_p2;
	//}
	//else
	//{
	//	s_p1 = S1->GetAt(nTime,nExpSet);
	//	s_p2 = S2->GetAt(nTime,nExpSet);
	//}

	double s1i,s2i,s1g,s2g,s1a,s2a,s1k,s2k;
	if (nTime==0 && bInit)
	{
		s1g = m_pLU->m_cExpSet.GetAt(nExpSet)->GetValue(0);
		s2g = m_pLU->m_cExpSet.GetAt(nExpSet)->GetValue(1);
		S1Growth->SetAt(s1g,0,nExpSet);
		S2Growth->SetAt(s2g,0,nExpSet);

		s1k = m_pLU->m_cExpSet.GetAt(nExpSet)->GetValue(2);
		s2k = m_pLU->m_cExpSet.GetAt(nExpSet)->GetValue(3);
		S1CC->SetAt(s1k,0,nExpSet);
		S2CC->SetAt(s2k,0,nExpSet);

		s1a = m_pLU->m_cExpSet.GetAt(nExpSet)->GetValue(4);
		s2a = m_pLU->m_cExpSet.GetAt(nExpSet)->GetValue(5);
		S1alpha->SetAt(s1a,0,nExpSet);
		S2beta->SetAt(s2a,0,nExpSet);


		s1i = m_pLU->m_cExpSet.GetAt(nExpSet)->GetValue(6);
		s2i = m_pLU->m_cExpSet.GetAt(nExpSet)->GetValue(7);
		S1init->SetAt(s1i,0,nExpSet);
		S2init->SetAt(s2i,0,nExpSet);

		s_p1 = s1i;
		s_p2 = s2i;
	}
	else
	{
		s1g = S1Growth->GetAt(nTime,nExpSet);
		s2g = S2Growth->GetAt(nTime,nExpSet);

		s1k = S1CC->GetAt(nTime,nExpSet);
		s2k = S2CC->GetAt(nTime,nExpSet);

		s1a = S1alpha->GetAt(nTime,nExpSet);
		s2a = S2beta->GetAt(nTime,nExpSet);

		s1i = S1init->GetAt(nTime,nExpSet);
		s2i = S2init->GetAt(nTime,nExpSet);

		if (nTime==0)
		{
			s_p1 = s1i;
			s_p2 = s2i;
		}
		else
		{
			s_p1 = S1->GetAt(nTime,nExpSet);
			s_p2 = S2->GetAt(nTime,nExpSet);
		}
	}
	for (int j=nTime;j<=m_nTimer;j++)
	{
		pTime->SetAt(j,j,nExpSet);
		S1->SetAt(s_p1,j,nExpSet);
		S2->SetAt(s_p2,j,nExpSet);
			
		S1Growth->SetAt(s1g,j,nExpSet);
		S2Growth->SetAt(s2g,j,nExpSet);

		S1CC->SetAt(s1k,j,nExpSet);
		S2CC->SetAt(s2k,j,nExpSet);

		S1alpha->SetAt(s1a,j,nExpSet);
		S2beta->SetAt(s2a,j,nExpSet);


		S1init->SetAt(s1i,j,nExpSet);
		S2init->SetAt(s2i,j,nExpSet);

		double nS1IncRate,nS1Change,nS1Tdouble;
		double nS2IncRate,nS2Change,nS2Tdouble;
		double nnS1PreyChange =0;
		double nnS2PreyChange =0;


		double h = 1.0;
		double param[]={s1g,s1a,s_p2,s1k};
		double k1 = h*NFunc(s_p1,param);
		double k2 = h*NFunc(s_p1+k1/2,param);
		double k3 = h*NFunc(s_p1+k2/2,param);
		double k4 = h*NFunc(s_p1+k3,param);
		double res1 = s_p1 + (k1 + 2*k2 + 2*k3 + k4)/6;


		double param2[]={s2g,s2a,s_p1,s2k};
		k1 = h*PFunc(s_p2,param2);
		k2 = h*PFunc(s_p2+k1/2,param2);
		k3 = h*PFunc(s_p2+k2/2,param2);
		k4 = h*PFunc(s_p2+k3,param2);
		double res2 = s_p2 + (k1 + 2*k2 + 2*k3 + k4)/6;

//		for (int k=0;k<10;k++)
//		{
//			if (s_p1 < -0.00001) s_p1 = 0.0;
//			if (s_p2 < -0.00001) s_p2 = 0.0;

			nS1IncRate = s1g * (1 - (s_p1 + s1a * s_p2)/s1k);
			nS2IncRate = s2g * (1 - (s_p2 + s2a * s_p1)/s2k);
			nS1Change = s_p1 * nS1IncRate;
			nS2Change = s_p2 * nS2IncRate;
			nS1Tdouble = log(2) / nS1IncRate;
			nS2Tdouble = log(2) / nS2IncRate;

//			nnS1PreyChange += 0.1 * nS1Change;
//			nnS2PreyChange += 0.1 * nS2Change;

//			double ss1 = s_p1 + 0.1 * nS1Change;
//			double ss2 = s_p2 + 0.1 * nS2Change;
//			s_p1 = ss1;
//			s_p2 = ss2;
			s_p1 = res1;
			s_p2 = res2;
//			if (!k)
//			{
				S1IncRate->SetAt(nS1IncRate,j,nExpSet);
				S2IncRate->SetAt(nS2IncRate,j,nExpSet);

				S1Double->SetAt(nS1Tdouble,j,nExpSet);
				S2Double->SetAt(nS2Tdouble,j,nExpSet);

				K1dK2->SetAt(s1k/s2k,j,nExpSet);
				invS2beta->SetAt(1/s2a,j,nExpSet);

				S1Stab->SetAt((s1k - s1a * s2k) / (1 - s1a*s2a),j,nExpSet);
				S2Stab->SetAt((s2k - s2a * s1k) / (1 - s1a*s2a),j,nExpSet);

				dN1N1->SetAt(s1k/s1a,j,nExpSet);
				dN1N2->SetAt(s1k,j,nExpSet);
				dN2N1->SetAt(s2k,j,nExpSet);
				dN2N2->SetAt(s2k/s2a,j,nExpSet);
//			}
//		}
//		S1Change->SetAt(nnS1PreyChange,j,nExpSet);
//		S2Change->SetAt(nnS2PreyChange,j,nExpSet);
		S1Change->SetAt(nS1Change,j,nExpSet);
		S2Change->SetAt(nS2Change,j,nExpSet);
	}
}

CString CPP_TwCompetition::GetFormatedText(int nLevel)
{
	BOOL bExp = (nLevel==2);
//	m_sText = GetFormatedEquation();

	int nb = 0;
	CModelObj *S1Growth		= m_cParSet.GetAt(nb++);
	CModelObj *S2Growth		= m_cParSet.GetAt(nb++);
	CModelObj *S1CC			= m_cParSet.GetAt(nb++);
	CModelObj *S2CC			= m_cParSet.GetAt(nb++);
	CModelObj *S1alpha		= m_cParSet.GetAt(nb++);
	CModelObj *S2beta 		= m_cParSet.GetAt(nb++);
	CModelObj *S1init		= m_cParSet.GetAt(nb++);
	CModelObj *S2init		= m_cParSet.GetAt(nb++);

	nb = 0;
	CModelObj *pTime		= m_cVarSet.GetAt(nb++);
	CModelObj *S1			= m_cVarSet.GetAt(nb++);
	CModelObj *S2			= m_cVarSet.GetAt(nb++);
	CModelObj *S1IncRate	= m_cVarSet.GetAt(nb++);
	CModelObj *S2IncRate	= m_cVarSet.GetAt(nb++);
	CModelObj *S1Change		= m_cVarSet.GetAt(nb++);
	CModelObj *S2Change		= m_cVarSet.GetAt(nb++);
	CModelObj *S1Double		= m_cVarSet.GetAt(nb++);
	CModelObj *S2Double		= m_cVarSet.GetAt(nb++);
	CModelObj *K1dK2		= m_cVarSet.GetAt(nb++);
	CModelObj *invBeta		= m_cVarSet.GetAt(nb++);
	CModelObj *S1Stab		= m_cVarSet.GetAt(nb++);
	CModelObj *S2Stab		= m_cVarSet.GetAt(nb++);
	CModelObj *dN1N1		= m_cVarSet.GetAt(nb++);
	CModelObj *dN1N2		= m_cVarSet.GetAt(nb++);
	CModelObj *dN2N1		= m_cVarSet.GetAt(nb++);
	CModelObj *dN2N2		= m_cVarSet.GetAt(nb++);


	CString tstr;

	tstr.Format(_T("t"));
	pTime->SetDef(tstr);

	tstr.Format(_T("N1"));
	S1->SetDef(tstr);

	tstr.Format(_T("N2"));
	S2->SetDef(tstr);

	tstr.Format(_T("N1[0]"));
	S1init->SetDef(tstr);
	tstr.Format(_T("N2[0]"));
	S2init->SetDef(tstr);

	tstr.Format(_T("r1"));
	S1Growth->SetDef(tstr);

	tstr.Format(_T("r2"));
	S2Growth->SetDef(tstr);

	tstr.Format(_T("K1"));
	S1CC->SetDef(tstr);

	tstr.Format(_T("K2"));
	S2CC->SetDef(tstr);

	tstr.Format(_T("a"));
	S1alpha->SetDef(tstr);

	tstr.Format(_T("b"));
	S2beta->SetDef(tstr);

	tstr.Format(_T("%s * (1 - (%s + %s * %s) / %s)"),GetObjInfo(S1Growth,bExp),GetObjInfo(S1,bExp),
					GetObjInfo(S1alpha,bExp),GetObjInfo(S2,bExp),GetObjInfo(S1CC,bExp));
	S1IncRate->SetDef(tstr);

	tstr.Format(_T("%s * (1 - (%s + %s * %s) / %s)"),GetObjInfo(S2Growth,bExp),GetObjInfo(S2,bExp),
					GetObjInfo(S2beta,bExp),GetObjInfo(S1,bExp),GetObjInfo(S2CC,bExp));
	S2IncRate->SetDef(tstr);

	tstr.Format(_T("%s * %s"),GetObjInfo(S1,bExp),GetObjInfo(S1IncRate,bExp));
	S1Change->SetDef(tstr);
	tstr.Format(_T("%s * %s"),GetObjInfo(S2,bExp),GetObjInfo(S2IncRate,bExp));
	S2Change->SetDef(tstr);

	tstr.Format(_T("ln(2) / %s"),GetObjInfo(S1IncRate,bExp));
	S1Double->SetDef(tstr);
	tstr.Format(_T("ln(2) / %s"),GetObjInfo(S2IncRate,bExp));
	S2Double->SetDef(tstr);

	tstr.Format(_T("%s / %s"),GetObjInfo(S1CC,bExp),GetObjInfo(S2CC,bExp));
	K1dK2->SetDef(tstr);
	tstr.Format(_T("1 / %s"),GetObjInfo(S2beta,bExp));
	invBeta->SetDef(tstr);

	tstr.Format(_T("(%s - %s*%s)/(1 - %s*%s)"),GetObjInfo(S1CC,bExp), GetObjInfo(S1alpha,bExp),
					GetObjInfo(S2CC,bExp),GetObjInfo(S1alpha,bExp),GetObjInfo(S2beta,bExp));
	S1Stab->SetDef(tstr);
	tstr.Format(_T("(%s - %s*%s)/(1 - %s*%s)"),GetObjInfo(S2CC,bExp),GetObjInfo(S2beta,bExp),
					GetObjInfo(S1CC,bExp),GetObjInfo(S1alpha,bExp),GetObjInfo(S2beta,bExp));
	S2Stab->SetDef(tstr);

	tstr.Format(_T("%s / %s"),GetObjInfo(S1CC,bExp),GetObjInfo(S1alpha,bExp));
	dN1N1->SetDef(tstr);
	tstr.Format(_T("%s"),GetObjInfo(S1CC,bExp));
	dN1N2->SetDef(tstr);
	tstr.Format(_T("%s"),GetObjInfo(S2CC,bExp));
	dN2N1->SetDef(tstr);
	tstr.Format(_T("%s / %s"),GetObjInfo(S2CC,bExp),GetObjInfo(S2beta,bExp));
	dN2N2->SetDef(tstr);

	return m_sText;
}

CString CPP_TwCompetition::GetFormatedEquation()
{
	CString mText;

	mText.Format(_T("    %s \t\t ; %s \r\n\r\n    %s \t\t ; %s \r\n\r\n\r\n"),
		m_sEqu.GetAt(0),
		m_sEquDef.GetAt(0),
		m_sEqu.GetAt(1),
		m_sEquDef.GetAt(1)
		);
	return mText;
}

CRect CPP_TwCompetition::DrawFlowChart(CDC* pDC)
{
	int nb = 0;
	CModelObj *S1Growth		= m_cParSet.GetAt(nb++);
	CModelObj *S2Growth		= m_cParSet.GetAt(nb++);
	CModelObj *S1CC			= m_cParSet.GetAt(nb++);
	CModelObj *S2CC			= m_cParSet.GetAt(nb++);
	CModelObj *S1alpha		= m_cParSet.GetAt(nb++);
	CModelObj *S2beta 		= m_cParSet.GetAt(nb++);
	CModelObj *S1init		= m_cParSet.GetAt(nb++);
	CModelObj *S2init		= m_cParSet.GetAt(nb++);

	nb = 0;
	CModelObj *pTime		= m_cVarSet.GetAt(nb++);
	CModelObj *S1			= m_cVarSet.GetAt(nb++);
	CModelObj *S2			= m_cVarSet.GetAt(nb++);
	CModelObj *S1IncRate	= m_cVarSet.GetAt(nb++);
	CModelObj *S2IncRate	= m_cVarSet.GetAt(nb++);
	CModelObj *S1Change		= m_cVarSet.GetAt(nb++);
	CModelObj *S2Change		= m_cVarSet.GetAt(nb++);
	CModelObj *S1Double		= m_cVarSet.GetAt(nb++);
	CModelObj *S2Double		= m_cVarSet.GetAt(nb++);
	CModelObj *K1dK2		= m_cVarSet.GetAt(nb++);
	CModelObj *invBeta		= m_cVarSet.GetAt(nb++);
	CModelObj *S1Stab		= m_cVarSet.GetAt(nb++);
	CModelObj *S2Stab		= m_cVarSet.GetAt(nb++);
	CModelObj *dN1N1		= m_cVarSet.GetAt(nb++);
	CModelObj *dN1N2		= m_cVarSet.GetAt(nb++);
	CModelObj *dN2N1		= m_cVarSet.GetAt(nb++);
	CModelObj *dN2N2		= m_cVarSet.GetAt(nb++);


	// Draw Arrow
	CPen arrPen(PS_SOLID,3,RGB(0,0,255));
	CPen varPen(PS_SOLID,2,RGB(0,0,255));
	CPen arwPen(PS_SOLID,0,RGB(0,0,255));
	CBrush varBrush(RGB(0,0,255));

	CPen *oldP = pDC->SelectObject(&arrPen);
	// Pop1
	pDC->MoveTo(20,40);
	pDC->LineTo(240,40);
	// Pop1
	pDC->MoveTo(240+VAR_SIZE,220);
	pDC->LineTo(460+VAR_SIZE,220);
	pDC->SelectObject(oldP);

	oldP = pDC->SelectObject(&arwPen);
	CBrush *oldB = pDC->SelectObject(&varBrush);
	POINT polyPt[3] = {{240,40},{230,33},{230,47}};
	pDC->Polygon(polyPt,3);
	POINT polyPt2[3] = {{240+VAR_SIZE,220},{250+VAR_SIZE,213},{250+VAR_SIZE,227}};
	pDC->Polygon(polyPt2,3);

	pDC->SelectObject(oldP);
	pDC->SelectObject(oldB);

	CRect rRet(20,40,460+VAR_SIZE,220);

	// Draw Pops
	rRet |= DrawNode(pDC,CPoint(240,25),S1->GetName(),TT_TOP,TT_VAR);
	rRet |= DrawNode(pDC,CPoint(240,205),S2->GetName(),TT_BOTTOM,TT_VAR);
	// Draw PopsChange
	rRet |= DrawNode(pDC,CPoint(110,30),S1Change->GetName(),TT_TOP,TT_AGG);
	rRet |= DrawNode(pDC,CPoint(370,210),S2Change->GetName(),TT_BOTTOM,TT_AGG);
	// Draw PopsIncRate
	rRet |= DrawNode(pDC,CPoint(110,110),S1IncRate->GetName(),TT_TOP,TT_AGG);
	rRet |= DrawNode(pDC,CPoint(370,110),S2IncRate->GetName(),TT_BOTTOM,TT_AGG);
	// Draw PopsAlp
	rRet |= DrawNode(pDC,CPoint(110,190),S1alpha->GetName(),TT_BOTTOM,TT_PAR);
	rRet |= DrawNode(pDC,CPoint(370,30),S2beta->GetName(),TT_TOP,TT_PAR);
	// Draw PopsGrowth
	rRet |= DrawNode(pDC,CPoint(50,80),S1Growth->GetName(),TT_TOP,TT_PAR);
	rRet |= DrawNode(pDC,CPoint(430,140),S2Growth->GetName(),TT_BOTTOM,TT_PAR);
	// Draw PopsCC
	rRet |= DrawNode(pDC,CPoint(50,140),S1CC->GetName(),TT_BOTTOM,TT_PAR);
	rRet |= DrawNode(pDC,CPoint(430,80),S2CC->GetName(),TT_TOP,TT_PAR);

	// Draw connectors
	// PopsGrowth -> PopsIncRate
	CPoint mpt[2] = {CPoint(50+AGG_SIZE,80+AGG_SIZE/2),CPoint(110+2,110+AGG_SIZE/2-4)};
	rRet |= DrawConnector(pDC,2,mpt,TT_LINE);
	mpt[0] = CPoint(430,140+AGG_SIZE/2);
	mpt[1] = CPoint(370+AGG_SIZE-1,110+AGG_SIZE/2+3);
	rRet |= DrawConnector(pDC,2,mpt,TT_LINE);
	// PopsCC -> PopsIncRate
	mpt[0] = CPoint(50+AGG_SIZE,140+AGG_SIZE/2);
	mpt[1] = CPoint(110+2,110+AGG_SIZE/2+4);
	rRet |= DrawConnector(pDC,2,mpt,TT_LINE);
	mpt[0] = CPoint(430,80+AGG_SIZE/2);
	mpt[1] = CPoint(370+AGG_SIZE-1,110+AGG_SIZE/2-3);
	rRet |= DrawConnector(pDC,2,mpt,TT_LINE);
	// PopsAlp -> PopsIncRate
	mpt[0] = CPoint(110+AGG_SIZE/2,190);
	mpt[1] = CPoint(110+AGG_SIZE/2,110+AGG_SIZE);
	rRet |= DrawConnector(pDC,2,mpt,TT_LINE);
	mpt[0] = CPoint(370+AGG_SIZE/2,30+AGG_SIZE);
	mpt[1] = CPoint(370+AGG_SIZE/2,110);
	rRet |= DrawConnector(pDC,2,mpt,TT_LINE);
	// PopsIncRate -> PopsChange
	mpt[0] = CPoint(110+AGG_SIZE/2,110);
	mpt[1] = CPoint(110+AGG_SIZE/2,30+AGG_SIZE);
	rRet |= DrawConnector(pDC,2,mpt,TT_LINE);
	mpt[0] = CPoint(370+AGG_SIZE/2,110+AGG_SIZE);
	mpt[1] = CPoint(370+AGG_SIZE/2,210);
	rRet |= DrawConnector(pDC,2,mpt,TT_LINE);
	// Prey -> PreyChange
//	CPoint polyPt3[4] = {CPoint(330,110),CPoint(270,150),CPoint(188,110),CPoint(188,94)};
	CPoint polyPt3[4] = {CPoint(240+VAR_SIZE/4,25+VAR_SIZE),
						 CPoint(240+VAR_SIZE/4-VAR_SIZE,25+2*VAR_SIZE),
						 CPoint(110+AGG_SIZE*3/2+7,30+2*AGG_SIZE-2),
						 CPoint(110+AGG_SIZE/2+7,30+AGG_SIZE-2)};
	rRet |= DrawConnector(pDC,4,polyPt3,TT_CURVE);

	polyPt3[0] = CPoint(240+VAR_SIZE*3/4,205);
	polyPt3[1] = CPoint(240+VAR_SIZE*3/4+AGG_SIZE,205-AGG_SIZE);
	polyPt3[2] = CPoint(370-AGG_SIZE,210-AGG_SIZE);
	polyPt3[3] = CPoint(370+2,210+AGG_SIZE/2-4);

	rRet |= DrawConnector(pDC,4,polyPt3,TT_CURVE);
	// Prey -> PreyIncRate
	CPoint polyPt4[4] = {CPoint(240+VAR_SIZE*3/8,25+VAR_SIZE),
						 CPoint(240+VAR_SIZE*3/8-VAR_SIZE,25+2*VAR_SIZE),
						 CPoint(110+AGG_SIZE+AGG_SIZE/2,110+AGG_SIZE/2-4),
						 CPoint(110+AGG_SIZE,110+AGG_SIZE/2-4)};
	rRet |= DrawConnector(pDC,4,polyPt4,TT_CURVE);

	polyPt4[0] = CPoint(240+VAR_SIZE/2,205);
	polyPt4[1] = CPoint(240+VAR_SIZE/2,205-2*AGG_SIZE);
	polyPt4[2] = CPoint(370+2-2*AGG_SIZE,110+AGG_SIZE/2+4);
	polyPt4[3] = CPoint(370+2,110+AGG_SIZE/2+4);
	rRet |= DrawConnector(pDC,4,polyPt4,TT_CURVE);


	// Prey1 -> PreyIncRate2
	polyPt4[0] = CPoint(240+VAR_SIZE*3/4,25+VAR_SIZE);
	polyPt4[1] = CPoint(240+VAR_SIZE*3/4,25+2*VAR_SIZE);
	polyPt4[2] = CPoint(370+2,110+AGG_SIZE/2-4-2*VAR_SIZE);
	polyPt4[3] = CPoint(370+2,110+AGG_SIZE/2-4);
	rRet |= DrawConnector(pDC,4,polyPt4,TT_CURVE);

	polyPt4[0] = CPoint(240+VAR_SIZE/4,205);
	polyPt4[1] = CPoint(240+VAR_SIZE/4,205-2*+VAR_SIZE);
	polyPt4[2] = CPoint(110+AGG_SIZE-1,110+AGG_SIZE/2+4+2*+VAR_SIZE);
	polyPt4[3] = CPoint(110+AGG_SIZE-1,110+AGG_SIZE/2+4);
	rRet |= DrawConnector(pDC,4,polyPt4,TT_CURVE);

	return rRet;
}



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL( CPP_TwPredation, CMdEquation, VERSIONABLE_SCHEMA | 1)

void CPP_TwPredation::Serialize( CArchive& ar )
{
	CMdEquation::Serialize(ar);
	if( ar.IsStoring( ) )
	{
	}
	else
	{
	}
}

CPP_TwPredation::CPP_TwPredation() : CMdEquation()
{
	m_sName = "Two-Species Predation with limited growth";
}

CPP_TwPredation::CPP_TwPredation(CMdEquation*pEq) : CMdEquation(pEq)
{
	m_sName = "Two-Species Predation with limited growth";
}

CPP_TwPredation::~CPP_TwPredation()
{
}

CMdEquation *CPP_TwPredation::Clone()
{
	CMdEquation *pEqu = new CPP_TwPredation(this);
	return pEqu;
}



double CPP_TwPredation::NFunc(double N,double param[])
{
	double res;
	double	r = param[0],
			alpha = param[1],
			P = param[2],
			K = param[3];
	
	res = r * N * (1 - N / K) - alpha * N * P;
	return res;
}

double CPP_TwPredation::PFunc(double P,double param[])
{
	double res;
	double	m = param[0],
			beta = param[1],
			N = param[2];
	res = beta * P * N - m * P;
	return res;
}


void CPP_TwPredation::OnNewEquation(CSimModel *pModel)
{
	CMdEquation::OnNewEquation(pModel);

	m_nEquNb = 2;
		if (!m_sEqu.GetSize())
	{

	m_sEqu.Add(_T("dN / dt = r * N * (1 - N / K) - alpha * N * P"));
	m_sEqu.Add(_T("dP / dt = beta * P * N - m * P"));
	m_sEquDef.Add(_T("Prey density evolution"));
	m_sEquDef.Add(_T("Predator density evolution"));
		}

	if (!m_cVarSet.GetSize())
	{
	// The variables 
	CModelVar *pVar = NULL;
//	m_cVarSet.Add(pVar = new CModelVar("Time"));
//	pVar->SetDef(_T("t"));
//	pVar->SetInitValue(-1);
	m_cVarSet.Add(pVar = new CModelTime("Time"));			
	pVar->SetDef(_T("t"));
	pVar->SetAbbrev(_T("t"));

	m_cVarSet.Add(pVar = new CModelVar("Prey"));
	pVar->SetInitValue(5);
	pVar->SetAbbrev(_T("N"));
	pVar->SetColor(RGB(0,128,0));
	//pVar->SetAbbrev(_T("Prey"));
	m_cVarSet.Add(pVar = new CModelVar("Predator"));
	pVar->SetInitValue(6);
	pVar->SetAbbrev(_T("P"));
	pVar->SetColor(RGB(255,0,0));


	m_cVarSet.Add(pVar = new CModelVar("Change(N)"));
	pVar->SetDef(_T("PreyBorn - PreyKilled"));
	pVar->SetAbbrev(_T("Change(N)"));

		m_cVarSet.Add(pVar = new CModelVar("Potential(N)"));
		pVar->SetDef(_T("N * PreyIncRate"));
		pVar->SetAbbrev(_T("Potential(N)"));

			m_cVarSet.Add(pVar = new CModelVar("IncRate(N)"));
			pVar->SetDef(_T("r * (1-N/K)"));
			pVar->SetAbbrev(_T("IncRate(N)"));

		m_cVarSet.Add(pVar = new CModelVar("Killed(N)"));
		pVar->SetDef(_T("P * CaptPerPred"));
		pVar->SetAbbrev(_T("Killed(N)"));
	
			m_cVarSet.Add(pVar = new CModelVar("CaptPerPred"));
			pVar->SetDef(_T("alpha * N"));
			pVar->SetAbbrev(_T("CaptPerPred"));

	m_cVarSet.Add(pVar = new CModelVar("Change(P)"));
	pVar->SetDef(_T("PredInc - PredKilled"));
	pVar->SetAbbrev(_T("Change(P)"));

		m_cVarSet.Add(pVar = new CModelVar("Potential(P)"));
		pVar->SetDef(_T("P * ConvPerPred"));
		pVar->SetAbbrev(_T("Potential(P)"));

			m_cVarSet.Add(pVar = new CModelVar("ConvPerPred"));
			pVar->SetDef(_T("beta * N"));
			pVar->SetAbbrev(_T("ConvPerPred"));
	
		m_cVarSet.Add(pVar = new CModelVar("Dead(P)"));
		pVar->SetDef(_T("P * m"));
		pVar->SetAbbrev(_T("Dead(P)"));

	m_cVarSet.Add(pVar = new CModelVar("Stab(N)"));			
	pVar->SetDef(_T("m / beta"));
	pVar->SetAbbrev(_T("Stab(N)"));
	m_cVarSet.Add(pVar = new CModelVar("Stab(P)"));			
	pVar->SetDef(_T("r / alpha - m / (alpha * beta * K)"));
	pVar->SetAbbrev(_T("Stab(P)"));

	m_cVarSet.Add(pVar = new CModelVar("dN/dt=0 (N)"));			
	pVar->SetDef(_T("r/alpha"));
	pVar->SetAbbrev(_T("dN/dt=0 (N)"));
	pVar->SetColor(RGB(0,128,0));
	m_cVarSet.Add(pVar = new CModelVar("dN/dt=0 (P)"));			
	pVar->SetAbbrev(_T("dN/dt=0 (P)"));
	pVar->SetDef(_T("K"));
	pVar->SetColor(RGB(0,128,0));
	m_cVarSet.Add(pVar = new CModelVar("dP/dt=0 (N)"));			
	pVar->SetAbbrev(_T("dP/dt=0 (N)"));
	pVar->SetDef(_T("m/beta"));
	pVar->SetColor(RGB(255,0,0));
	//m_cVarSet.Add(pVar = new CModelVar("dN1/dt=0 (N2)"));			
	//pVar->SetDef(_T("K2/b"));
	}

		if (!m_cParSet.GetSize())
	{
	// The parametres
	CModelPar *pPar=NULL;
//	m_cParSet.Add(pPar = new CModelPar("Time",100));						
//	pPar->SetInfos(100,100,200);
	m_cParSet.Add(pPar= new CModelPar("PreyFert",0.3));
	pPar->SetAbbrev(_T("dP/dt=0 (N)"));
	pPar->SetInfos(0.3,0.001,0.5);
	//pPar->SetAbbrev(_T("PredFert"));
	m_cParSet.Add(pPar= new CModelPar("CarCap",450));
	pPar->SetInfos(150,50,450);
	//pPar->SetAbbrev(_T("CarrCap"));
	m_cParSet.Add(pPar= new CModelPar("CaptEff"));
	pPar->SetInfos(0.03,0.0001,1.);
	pPar->SetDesc("Capture Efficiency");
	pPar->SetDef("alpha");
	//pPar->SetAbbrev(_T("Cov"));
	m_cParSet.Add(pPar= new CModelPar("PredDeath",0.05));
	pPar->SetInfos(0.05,0.0001,0.1);
	//pPar->SetAbbrev(_T("PredDR"));
	m_cParSet.Add(pPar= new CModelPar("ConvEff"));
	pPar->SetInfos(0.0027,0.0001,0.1);
	pPar->SetDesc("Conversion Efficiency");
	pPar->SetDef("beta");
	//pPar->SetAbbrev(_T("PredDR"));
	m_cParSet.Add(pPar= new CModelPar("Prey_init",20));
	pPar->SetInfos(20,0,50);
	pPar->SetInitValue(1);
	pPar->SetColor(RGB(0,128,0));
	//pPar->SetAbbrev(_T("PreyInit"));
	m_cParSet.Add(pPar= new CModelPar("Pred_init",5));
	pPar->SetInfos(5,0,50);
	pPar->SetInitValue(2);
	pPar->SetColor(RGB(255,0,0));
	//pPar->SetAbbrev(_T("PredInit"));
		}
}

void CPP_TwPredation::OnUpdateFrom(CLearningUnit *m_pLU,int nExpSet,int nTime,BOOL bInit)
{
	int m_nTimer = m_pLU->GetMaxTimer();
	int nb=0;

//	CMdData *pTime= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *PreyFertility	= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *CarryingCap	= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *CaptEff		= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *PredDeathRate	= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *ConvEff		= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *PreyInit		= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *PredInit		= m_pLU->m_cDataPoints.GetAt(nb++);

	CMdData *pTime= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *Prey= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *Predator= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *PreyChange= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *PreyBorn= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *PreyIncRate= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *PreyKilled= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *CaptPerPred= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *PredChange= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *PredInc= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *ConvPerPred= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *PredKilled= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *PreyStab= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *PredStab= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *dN1N1			= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *dN1N2			= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *dN2N1			= m_pLU->m_cDataPoints.GetAt(nb++);

	double  s_prey,s_pred;
	nb = 0;

	s_prey = m_pLU->m_cExpSet.GetAt(nExpSet)->GetValue(5);
	//PreyInit->SetAt(dd,0,nExpSet);
	//if (nTime == 0)
	//	s_prey = dd;
	//else
	//	s_prey = Prey->GetAt(nTime,nExpSet);
	s_pred = m_pLU->m_cExpSet.GetAt(nExpSet)->GetValue(6);
	//PredInit->SetAt(dd,0,nExpSet);
	//if (nTime == 0)
	//	s_pred = dd;
	//else
	//	s_pred = Predator->GetAt(nTime,nExpSet);

	double pri,pdi,pf,cc,pd,alp,bet;
	if (nTime==0 && bInit)
	{
		pf = m_pLU->m_cExpSet.GetAt(nExpSet)->GetValue(0);
		PreyFertility->SetAt(pf,0,nExpSet);
		cc = m_pLU->m_cExpSet.GetAt(nExpSet)->GetValue(1);
		CarryingCap->SetAt(cc,0,nExpSet);
		alp = m_pLU->m_cExpSet.GetAt(nExpSet)->GetValue(2);
		CaptEff->SetAt(alp,0,nExpSet);
		pd = m_pLU->m_cExpSet.GetAt(nExpSet)->GetValue(3);
		PredDeathRate->SetAt(pd,0,nExpSet);
		bet = m_pLU->m_cExpSet.GetAt(nExpSet)->GetValue(4);
		ConvEff->SetAt(bet,0,nExpSet);
		pri = m_pLU->m_cExpSet.GetAt(nExpSet)->GetValue(5);
		PreyInit->SetAt(pri,0,nExpSet);
		pdi = m_pLU->m_cExpSet.GetAt(nExpSet)->GetValue(6);
		PredInit->SetAt(pdi,0,nExpSet);
		
		s_prey = pri;
		s_pred = pdi;
	}
	else
	{
		pri = PreyInit->GetAt(nTime,nExpSet);
		pdi = PredInit->GetAt(nTime,nExpSet);
		pf = PreyFertility->GetAt(nTime,nExpSet);
		cc = CarryingCap->GetAt(nTime,nExpSet);
		pd = PredDeathRate->GetAt(nTime,nExpSet);
		alp = CaptEff->GetAt(nTime,nExpSet);
		bet = ConvEff->GetAt(nTime,nExpSet);

		if (nTime==0)
		{
			s_prey = pri;
			s_pred = pdi;
		}
		else
		{
			s_prey = Prey->GetAt(nTime,nExpSet);
			s_pred = Predator->GetAt(nTime,nExpSet);
		}
	}

	for (int j=nTime;j<=m_nTimer;j++)
	{
		pTime->SetAt(j,j,nExpSet);
		Prey->SetAt(s_prey,j,nExpSet);
		Predator->SetAt(s_pred,j,nExpSet);

		PreyInit->SetAt(pri,j,nExpSet);
		PredInit->SetAt(pdi,j,nExpSet);
		PreyFertility->SetAt(pf,j,nExpSet);
		CarryingCap->SetAt(cc,j,nExpSet);
		PredDeathRate->SetAt(pd,j,nExpSet);
		CaptEff->SetAt(alp,j,nExpSet);
		ConvEff->SetAt(bet,j,nExpSet);

		double nPredKilled,nConvPerPred,nPredInc,
				nPredChange,nCaptPerPred,nPreyKilled ,
				nPreyIncRate,nPreyBorn,nPreyChange ;
		double  nnPreyChange = 0,
				nnPredatorChange = 0;
		
		double h = 1.0;
		double param[]={pf,alp,s_pred,cc};
		double k1 = h*NFunc(s_prey,param);
		double k2 = h*NFunc(s_prey+k1/2,param);
		double k3 = h*NFunc(s_prey+k2/2,param);
		double k4 = h*NFunc(s_prey+k3,param);
		double res1 = s_prey + (k1 + 2*k2 + 2*k3 + k4)/6;


		double param2[]={pd,bet,s_prey};
		k1 = h*PFunc(s_pred,param2);
		k2 = h*PFunc(s_pred+k1/2,param2);
		k3 = h*PFunc(s_pred+k2/2,param2);
		k4 = h*PFunc(s_pred+k3,param2);
		double res2 = s_pred + (k1 + 2*k2 + 2*k3 + k4)/6;

//		for (int k=0;k<10;k++)
//		{

			nPredKilled = pd * s_pred;
			nConvPerPred = bet * s_prey;
			nPredInc = s_pred * nConvPerPred;
			nPredChange = nPredInc - nPredKilled;
			
nCaptPerPred = alp * s_prey;
			nPreyKilled = s_pred * nCaptPerPred;
			nPreyIncRate = pf * (1-s_prey/cc);
			nPreyBorn = s_prey * nPreyIncRate;
			nPreyChange = nPreyBorn - nPreyKilled;

//			nnPreyChange += 0.1 * nPreyChange;
//			nnPredatorChange += 0.1 * nPredChange ;

//			double ss1 = s_prey + 0.1 * nPreyChange;
//			double ss2 = s_pred + 0.1 * nPredChange;
//			s_prey = ss1;
//			s_pred = ss2;
			s_prey = res1;
			s_pred = res2;
//			if (!k)
//			{
				PredKilled->SetAt(nPredKilled,j,nExpSet);
				ConvPerPred->SetAt(nConvPerPred,j,nExpSet);
				PredInc->SetAt(nPredInc,j,nExpSet);
				//PredChange->SetAt(nPredChange,j,nExpSet);
				CaptPerPred->SetAt(nCaptPerPred,j,nExpSet);
				PreyKilled->SetAt(nPreyKilled,j,nExpSet);
				PreyIncRate->SetAt(nPreyIncRate,j,nExpSet);
				PreyBorn->SetAt(nPreyBorn,j,nExpSet);
				//PreyChange->SetAt(nPreyChange,j,nExpSet);

				PreyStab->SetAt(pd / bet,j,nExpSet);
				double abcdef = (pd) / (bet);
			
				//PreyLimit->SetAt(abcdef ,j,nExpSet);
				double 
				r1 = pf*(1/cc)*pd,	
				r2 = pf*bet;
				double fgfgfg = - (r1-r2) / (bet * alp);
				//edStab->SetAt(pf / alp - pd / (alp*bet*cc),j,nExpSet);
				PredStab->SetAt(fgfgfg,j,nExpSet);

				dN1N1->SetAt(pf/alp,j,nExpSet);
				dN1N2->SetAt(cc,j,nExpSet);
				dN2N1->SetAt(pd/bet,j,nExpSet);
//			}
//		}
			
//		PreyChange->SetAt(nnPreyChange,j,nExpSet);
//		PredChange->SetAt(nnPredatorChange,j,nExpSet);
		PreyChange->SetAt(nPreyChange,j,nExpSet);
		PredChange->SetAt(nPredChange,j,nExpSet);
	}
}

CString CPP_TwPredation::GetFormatedText(int nLevel)
{
	BOOL bExp = (nLevel==2);
//	m_sText = GetFormatedEquation();

	int nb = 0;
	CModelObj *PreyFertility= m_cParSet.GetAt(nb++);
	CModelObj *CarryingCap= m_cParSet.GetAt(nb++);
	CModelObj *CaptEff= m_cParSet.GetAt(nb++);
	CModelObj *PredDeathRate= m_cParSet.GetAt(nb++);
	CModelObj *ConvEff= m_cParSet.GetAt(nb++);
	CModelObj *PreyInit= m_cParSet.GetAt(nb++);
	CModelObj *PredInit= m_cParSet.GetAt(nb++);

	nb = 0;
	CModelObj *pTime= m_cVarSet.GetAt(nb++);
	CModelObj *Prey= m_cVarSet.GetAt(nb++);
	CModelObj *Predator= m_cVarSet.GetAt(nb++);
	CModelObj *PreyChange= m_cVarSet.GetAt(nb++);
	CModelObj *PreyBorn= m_cVarSet.GetAt(nb++);
	CModelObj *PreyIncRate= m_cVarSet.GetAt(nb++);
	CModelObj *PreyKilled= m_cVarSet.GetAt(nb++);
	CModelObj *CaptPerPred= m_cVarSet.GetAt(nb++);
	CModelObj *PredChange= m_cVarSet.GetAt(nb++);
	CModelObj *PredInc= m_cVarSet.GetAt(nb++);
	CModelObj *ConvPerPred= m_cVarSet.GetAt(nb++);
	CModelObj *PredKilled= m_cVarSet.GetAt(nb++);
	CModelObj *PreyStab= m_cVarSet.GetAt(nb++);
	CModelObj *PredStab= m_cVarSet.GetAt(nb++);
	CModelObj *dN1N1= m_cVarSet.GetAt(nb++);
	CModelObj *dN1N2= m_cVarSet.GetAt(nb++);
	CModelObj *dN2N1= m_cVarSet.GetAt(nb++);

//	m_sEqu.Add(_T("dN / dt = r * N * (1 - N / K) - alpha * N * P"));
//	m_sEqu.Add(_T("dP / dt = beta * P * N - m * P"));

	CString tstr;

	tstr.Format(_T("t"));
	pTime->SetDef(tstr);
	tstr.Format(_T("N"));
	Prey->SetDef(tstr);
	tstr.Format(_T("P"));
	Predator->SetDef(tstr);
	tstr.Format(_T("N[0]"));
	PreyInit->SetDef(tstr);
	tstr.Format(_T("P[0]"));
	PredInit->SetDef(tstr);

	tstr.Format(_T("r"));
	PreyFertility->SetDef(tstr);
	tstr.Format(_T("K"));
	CarryingCap->SetDef(tstr);
	tstr.Format(_T("alpha"));
	CaptEff->SetDef(tstr);
	tstr.Format(_T("m"));
	PredDeathRate->SetDef(tstr);
	tstr.Format(_T("beta"));
	ConvEff->SetDef(tstr);

	tstr.Format(_T("%s * %s"),GetObjInfo(CaptEff,bExp),GetObjInfo(Prey,bExp));
	CaptPerPred->SetDef(tstr);

	tstr.Format(_T("%s * %s"),GetObjInfo(Predator,bExp),GetObjInfo(CaptPerPred,bExp));
	PreyKilled->SetDef(tstr);

	tstr.Format(_T("%s * (1 - %s / %s)"),GetObjInfo(PreyFertility,bExp),GetObjInfo(Prey,bExp),
					GetObjInfo(CarryingCap,bExp));
	PreyIncRate->SetDef(tstr);

	tstr.Format(_T("%s * %s"),GetObjInfo(Prey,bExp),GetObjInfo(PreyIncRate,bExp));
	PreyBorn->SetDef(tstr);

	tstr.Format(_T("%s - %s"),GetObjInfo(PreyBorn,bExp),GetObjInfo(PreyKilled,bExp));
	PreyChange->SetDef(tstr);

	tstr.Format(_T("%s * %s"),GetObjInfo(Predator,bExp),GetObjInfo(PredDeathRate,bExp));
	PredKilled->SetDef(tstr);

	tstr.Format(_T("%s * %s"),GetObjInfo(ConvEff,bExp),GetObjInfo(Prey,bExp));
	ConvPerPred->SetDef(tstr);

	tstr.Format(_T("%s * %s"),GetObjInfo(Predator,bExp),GetObjInfo(ConvPerPred,bExp));
	PredInc->SetDef(tstr);

	tstr.Format(_T("%s - %s"),GetObjInfo(PredInc,bExp),GetObjInfo(PredKilled,bExp));
	PredChange->SetDef(tstr);

	tstr.Format(_T("%s / %s"),GetObjInfo(PredDeathRate,bExp),GetObjInfo(ConvEff,bExp));
	PreyStab->SetDef(tstr);
	
	tstr.Format(_T("%s / %s - %s / (%s * %s * %s)"),GetObjInfo(PreyFertility,bExp),GetObjInfo(CaptEff,bExp)
					,GetObjInfo(PredDeathRate,bExp),GetObjInfo(CaptEff,bExp)
					,GetObjInfo(ConvEff,bExp),GetObjInfo(CarryingCap,bExp));
	PredStab->SetDef(tstr);

	tstr.Format(_T("%s / %s"),GetObjInfo(PreyFertility,bExp),GetObjInfo(CaptEff,bExp));
	dN1N1->SetDef(tstr);

	tstr.Format(_T("%s"),GetObjInfo(CarryingCap,bExp));
	dN1N2->SetDef(tstr);

	tstr.Format(_T("%s / %s"),GetObjInfo(PredDeathRate,bExp),GetObjInfo(ConvEff,bExp));
	dN2N1->SetDef(tstr);

	
	return m_sText;

}

CString CPP_TwPredation::GetFormatedEquation()
{
	CString mText;
	mText.Format(_T("    %s \t\t ; %s \r\n\r\n"
				    "    %s \t\t ; %s \r\n\r\n"),
			m_sEqu.GetAt(0),
			m_sEquDef.GetAt(0),
			m_sEqu.GetAt(1),
			m_sEquDef.GetAt(1));

	return mText;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL( CPP_TwPredationUnltd, CMdEquation, VERSIONABLE_SCHEMA | 1)

void CPP_TwPredationUnltd::Serialize( CArchive& ar )
{
	CMdEquation::Serialize(ar);
	if( ar.IsStoring( ) )
	{
	}
	else
	{
	}
}

CPP_TwPredationUnltd::CPP_TwPredationUnltd() : CMdEquation()
{
	m_sName = "Two-Species Predation with unlimited growth";
}

CPP_TwPredationUnltd::CPP_TwPredationUnltd(CMdEquation*pEq) : CMdEquation(pEq)
{
	m_sName = "Two-Species Predation with unlimited growth";
}


CMdEquation *CPP_TwPredationUnltd::Clone()
{
	CMdEquation *pEqu = new CPP_TwPredationUnltd(this);
	return pEqu;
}


CPP_TwPredationUnltd::~CPP_TwPredationUnltd()
{
}

double CPP_TwPredationUnltd::NFunc(double N,double param[])
{
	double res;
	double	r = param[0],
			alpha = param[1],
			P = param[2];
	
	res = r * N - alpha * N * P;
	return res;
}

double CPP_TwPredationUnltd::PFunc(double P,double param[])
{
	double res;
	double	m = param[0],
			beta = param[1],
			N = param[2];
	res = beta * P * N - m * P;
	return res;
}


void CPP_TwPredationUnltd::OnNewEquation(CSimModel *pModel)
{
	CMdEquation::OnNewEquation(pModel);

	m_nEquNb = 2;
	if (!m_sEqu.GetSize())
	{
	m_sEqu.Add(_T("dN / dt = r * N - alpha * N * P"));
	m_sEqu.Add(_T("dP / dt = beta * P * N - m * P"));
	m_sEquDef.Add(_T("Prey density evolution"));
	m_sEquDef.Add(_T("Predator density evolution"));
	}
	if (!m_cVarSet.GetSize())
	{
	// The variables 
	CModelVar *pVar = NULL;
//	m_cVarSet.Add(pVar = new CModelVar("Time"));
//	pVar->SetDef(_T("t"));
//	pVar->SetInitValue(-1);
	m_cVarSet.Add(pVar = new CModelTime("Time"));			
	pVar->SetDef(_T("t"));

	m_cVarSet.Add(pVar = new CModelVar("Prey"));
	pVar->SetInitValue(4);
	pVar->SetColor(RGB(0,128,0));
	//pVar->SetAbbrev(_T("Prey"));
	m_cVarSet.Add(pVar = new CModelVar("Predator"));
	pVar->SetInitValue(5);
	pVar->SetColor(RGB(255,0,0));


	m_cVarSet.Add(pVar = new CModelVar("PreyChange"));
	pVar->SetDef(_T("PreyBorn - PreyKilled"));

		m_cVarSet.Add(pVar = new CModelVar("PreyBorn"));
		pVar->SetDef(_T("N * PreyIncRate"));

			m_cVarSet.Add(pVar = new CModelVar("PreyIncRate"));
			pVar->SetDef(_T("r"));

		m_cVarSet.Add(pVar = new CModelVar("PreyKilled"));
		pVar->SetDef(_T("P * CaptPerPred"));
	
			m_cVarSet.Add(pVar = new CModelVar("CaptPerPred"));
			pVar->SetDef(_T("alpha * N"));

	m_cVarSet.Add(pVar = new CModelVar("PredChange"));
	pVar->SetDef(_T("PredInc - PredKilled"));

		m_cVarSet.Add(pVar = new CModelVar("PredInc"));
		pVar->SetDef(_T("P * ConvPerPred"));

			m_cVarSet.Add(pVar = new CModelVar("ConvPerPred"));
			pVar->SetDef(_T("beta * N"));

		m_cVarSet.Add(pVar = new CModelVar("PredKilled"));
		pVar->SetDef(_T("P * m"));

	m_cVarSet.Add(pVar = new CModelVar("Prey_Stab"));			
	pVar->SetDef(_T("m / beta"));
	m_cVarSet.Add(pVar = new CModelVar("Pred_Stab"));			
	pVar->SetDef(_T("r / alpha"));

	//m_cVarSet.Add(pVar = new CModelVar("dN1/dt=0 (N1)"));			
	//pVar->SetDef(_T("K1/a"));
	//m_cVarSet.Add(pVar = new CModelVar("dN1/dt=0 (N2)"));			
	//pVar->SetDef(_T("K1"));
	//m_cVarSet.Add(pVar = new CModelVar("dN2/dt=0 (N1)"));			
	//pVar->SetDef(_T("K2"));
	//m_cVarSet.Add(pVar = new CModelVar("dN1/dt=0 (N2)"));			
	//pVar->SetDef(_T("K2/b"));
	}

	if (!m_cParSet.GetSize())
	{
	// The parametres
	CModelPar *pPar=NULL;
//	m_cParSet.Add(pPar = new CModelPar("Time",100));						
//	pPar->SetInfos(100,100,200);
	m_cParSet.Add(pPar= new CModelPar("PreyFert",0.3));
	pPar->SetInfos(0.3,0.001,0.5);
	//pPar->SetAbbrev(_T("PredFert"));
	m_cParSet.Add(pPar= new CModelPar("CaptEff"));
	pPar->SetInfos(0.03,0.0001,1.);
	pPar->SetDesc("Capture Efficiency");
	pPar->SetDef("alpha");
	//pPar->SetAbbrev(_T("Cov"));
	m_cParSet.Add(pPar= new CModelPar("PredDeath",0.05));
	pPar->SetInfos(0.05,0.0001,0.1);
	//pPar->SetAbbrev(_T("PredDR"));
	m_cParSet.Add(pPar= new CModelPar("ConvEff"));
	pPar->SetInfos(0.0027,0.0001,0.1);
	pPar->SetDesc("Conversion Efficiency");
	pPar->SetDef("beta");
	//pPar->SetAbbrev(_T("PredDR"));
	m_cParSet.Add(pPar= new CModelPar("Prey_init",20));
	pPar->SetInfos(20,0,50);
	pPar->SetInitValue(1);
	pPar->SetColor(RGB(0,128,0));
	//pPar->SetAbbrev(_T("PreyInit"));
	m_cParSet.Add(pPar= new CModelPar("Pred_init",5));
	pPar->SetInfos(5,0,50);
	pPar->SetInitValue(2);
	pPar->SetColor(RGB(255,0,0));
	//pPar->SetAbbrev(_T("PredInit"));
	}
}

void CPP_TwPredationUnltd::OnUpdateFrom(CLearningUnit *m_pLU,int nExpSet,int nTime,BOOL bInit)
{
	int m_nTimer = m_pLU->GetMaxTimer();
	int nb=0;

//	CMdData *pTime= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *PreyFertility	= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *CaptEff		= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *PredDeathRate	= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *ConvEff		= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *PreyInit		= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *PredInit		= m_pLU->m_cDataPoints.GetAt(nb++);

	CMdData *pTime= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *Prey= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *Predator= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *PreyChange= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *PreyBorn= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *PreyIncRate= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *PreyKilled= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *CaptPerPred= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *PredChange= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *PredInc= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *ConvPerPred= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *PredKilled= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *PreyStab= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *PredStab= m_pLU->m_cDataPoints.GetAt(nb++);

	double  s_prey,s_pred;
	nb = 0;

	s_prey = m_pLU->m_cExpSet.GetAt(nExpSet)->GetValue(4);
	//PreyInit->SetAt(dd,0,nExpSet);
	//if (nTime == 0)
	//	s_prey = dd;
	//else
	//	s_prey = Prey->GetAt(nTime,nExpSet);
	s_pred = m_pLU->m_cExpSet.GetAt(nExpSet)->GetValue(5);
	//PredInit->SetAt(dd,0,nExpSet);
	//if (nTime == 0)
	//	s_pred = dd;
	//else
	//	s_pred = Predator->GetAt(nTime,nExpSet);

	double pri,pdi,pf,pd,alp,bet;
	if (nTime==0 && bInit)
	{
		pf = m_pLU->m_cExpSet.GetAt(nExpSet)->GetValue(0);
		PreyFertility->SetAt(pf,0,nExpSet);
		alp = m_pLU->m_cExpSet.GetAt(nExpSet)->GetValue(1);
		CaptEff->SetAt(alp,0,nExpSet);
		pd = m_pLU->m_cExpSet.GetAt(nExpSet)->GetValue(2);
		PredDeathRate->SetAt(pd,0,nExpSet);
		bet = m_pLU->m_cExpSet.GetAt(nExpSet)->GetValue(3);
		ConvEff->SetAt(bet,0,nExpSet);
		pri = m_pLU->m_cExpSet.GetAt(nExpSet)->GetValue(4);
		PreyInit->SetAt(pri,0,nExpSet);
		pdi = m_pLU->m_cExpSet.GetAt(nExpSet)->GetValue(5);
		PredInit->SetAt(pdi,0,nExpSet);

		s_prey = pri;
		s_pred = pdi;
	}
	else
	{
		pri = PreyInit->GetAt(nTime,nExpSet);
		pdi = PredInit->GetAt(nTime,nExpSet);
		pf = PreyFertility->GetAt(nTime,nExpSet);
		pd = PredDeathRate->GetAt(nTime,nExpSet);
		alp = CaptEff->GetAt(nTime,nExpSet);
		bet = ConvEff->GetAt(nTime,nExpSet);

		if (nTime==0)
		{
			s_prey = pri;
			s_pred = pdi;
		}
		else
		{
			s_prey = Prey->GetAt(nTime,nExpSet);
			s_pred = Predator->GetAt(nTime,nExpSet);
		}
	}
	for (int j=nTime;j<=m_nTimer;j++)
	{
		pTime->SetAt(j,j,nExpSet);
		Prey->SetAt(s_prey,j,nExpSet);
		Predator->SetAt(s_pred,j,nExpSet);

		PreyInit->SetAt(pri,j,nExpSet);
		PredInit->SetAt(pdi,j,nExpSet);
		PreyFertility->SetAt(pf,j,nExpSet);
		PredDeathRate->SetAt(pd,j,nExpSet);
		CaptEff->SetAt(alp,j,nExpSet);
		ConvEff->SetAt(bet,j,nExpSet);

		double nPredKilled,nConvPerPred,nPredInc,
				nPredChange,nCaptPerPred,nPreyKilled ,
				nPreyIncRate,nPreyBorn,nPreyChange ;
		double  nnPreyChange = 0,
				nnPredatorChange = 0;
		

		double res1,res2;
		for (int k=0;k<10;k++)
		{
			double h = 0.1;
			double param[]={pf,alp,s_pred};
			double k1 = h*NFunc(s_prey,param);
			double k2 = h*NFunc(s_prey+k1/2,param);
			double k3 = h*NFunc(s_prey+k2/2,param);
			double k4 = h*NFunc(s_prey+k3,param);
			res1 = s_prey + (k1 + 2*k2 + 2*k3 + k4)/6;


			double param2[]={pd,bet,s_prey};
			k1 = h*PFunc(s_pred,param2);
			k2 = h*PFunc(s_pred+k1/2,param2);
			k3 = h*PFunc(s_pred+k2/2,param2);
			k4 = h*PFunc(s_pred+k3,param2);
			res2 = s_pred + (k1 + 2*k2 + 2*k3 + k4)/6;
			s_prey = res1;
			s_pred = res2;
		}

//		for (int k=0;k<10;k++)
//		{

			nPredKilled = pd * s_pred;
			nConvPerPred = bet * s_prey;
			nPredInc = s_pred * nConvPerPred;
			nPredChange = nPredInc - nPredKilled;
			
			nCaptPerPred = alp * s_prey;
			nPreyKilled = s_pred * nCaptPerPred;
			nPreyIncRate = pf;
			nPreyBorn = s_prey * nPreyIncRate;
			nPreyChange = nPreyBorn - nPreyKilled;

			nnPreyChange += 0.1 * nPreyChange;
			nnPredatorChange += 0.1 * nPredChange ;

//			double aaa = pf * s_prey - alp * s_pred * s_prey;
//			double bbb = bet * s_pred * s_prey - pd * s_pred;

//			double ss1 = s_prey + 0.1 * nPreyChange;
//			double ss2 = s_pred + 0.1 * nPredChange;
//			s_prey = ss1;
//			s_pred = ss2;
			s_prey = res1;
			s_pred = res2;
//			if (!k)
//			{
				PredKilled->SetAt(nPredKilled,j,nExpSet);
				ConvPerPred->SetAt(nConvPerPred,j,nExpSet);
				PredInc->SetAt(nPredInc,j,nExpSet);
				//PredChange->SetAt(nPredChange,j,nExpSet);
				CaptPerPred->SetAt(nCaptPerPred,j,nExpSet);
				PreyKilled->SetAt(nPreyKilled,j,nExpSet);
				PreyIncRate->SetAt(nPreyIncRate,j,nExpSet);
				PreyBorn->SetAt(nPreyBorn,j,nExpSet);
				//PreyChange->SetAt(nPreyChange,j,nExpSet);

				PreyStab->SetAt(pd / bet,j,nExpSet);
				PredStab->SetAt(pf / alp,j,nExpSet);
//			}
//		}
			
//		PreyChange->SetAt(nnPreyChange,j,nExpSet);
//		PredChange->SetAt(nnPredatorChange,j,nExpSet);
		PreyChange->SetAt(nPreyChange,j,nExpSet);
		PredChange->SetAt(nPredChange,j,nExpSet);
	}
}

CString CPP_TwPredationUnltd::GetFormatedText(int nLevel)
{
	BOOL bExp = (nLevel==2);
	//m_sText = GetFormatedEquation();

	int nb = 0;
	CModelObj *PreyFertility= m_cParSet.GetAt(nb++);
	CModelObj *CaptEff= m_cParSet.GetAt(nb++);
	CModelObj *PredDeathRate= m_cParSet.GetAt(nb++);
	CModelObj *ConvEff= m_cParSet.GetAt(nb++);
	CModelObj *PreyInit= m_cParSet.GetAt(nb++);
	CModelObj *PredInit= m_cParSet.GetAt(nb++);

	nb = 0;
	CModelObj *pTime= m_cVarSet.GetAt(nb++);
	CModelObj *Prey= m_cVarSet.GetAt(nb++);
	CModelObj *Predator= m_cVarSet.GetAt(nb++);
	CModelObj *PreyChange= m_cVarSet.GetAt(nb++);
	CModelObj *PreyBorn= m_cVarSet.GetAt(nb++);
	CModelObj *PreyIncRate= m_cVarSet.GetAt(nb++);
	CModelObj *PreyKilled= m_cVarSet.GetAt(nb++);
	CModelObj *CaptPerPred= m_cVarSet.GetAt(nb++);
	CModelObj *PredChange= m_cVarSet.GetAt(nb++);
	CModelObj *PredInc= m_cVarSet.GetAt(nb++);
	CModelObj *ConvPerPred= m_cVarSet.GetAt(nb++);
	CModelObj *PredKilled= m_cVarSet.GetAt(nb++);
	CModelObj *PreyStab= m_cVarSet.GetAt(nb++);
	CModelObj *PredStab= m_cVarSet.GetAt(nb++);
//	CModelObj *dN1N1= m_cVarSet.GetAt(nb++);
//	CModelObj *dN1N2= m_cVarSet.GetAt(nb++);
//	CModelObj *dN2N1= m_cVarSet.GetAt(nb++);


	CString tstr;

	tstr.Format(_T("t"));
	pTime->SetDef(tstr);
	tstr.Format(_T("N"));
	Prey->SetDef(tstr);
	tstr.Format(_T("P"));
	Predator->SetDef(tstr);
	tstr.Format(_T("N[0]"));
	PreyInit->SetDef(tstr);
	tstr.Format(_T("P[0]"));
	PredInit->SetDef(tstr);

	tstr.Format(_T("r"));
	PreyFertility->SetDef(tstr);
	tstr.Format(_T("alpha"));
	CaptEff->SetDef(tstr);
	tstr.Format(_T("m"));
	PredDeathRate->SetDef(tstr);
	tstr.Format(_T("beta"));
	ConvEff->SetDef(tstr);

	tstr.Format(_T("%s * %s"),GetObjInfo(CaptEff,bExp),GetObjInfo(Prey,bExp));
	CaptPerPred->SetDef(tstr);

	tstr.Format(_T("%s * %s"),GetObjInfo(Predator,bExp),GetObjInfo(CaptPerPred,bExp));
	PreyKilled->SetDef(tstr);

	tstr.Format(_T("%s"),GetObjInfo(PreyFertility,bExp));
	PreyIncRate->SetDef(tstr);

	tstr.Format(_T("%s * %s"),GetObjInfo(Prey,bExp),GetObjInfo(PreyIncRate,bExp));
	PreyBorn->SetDef(tstr);

	tstr.Format(_T("%s - %s"),GetObjInfo(PreyBorn,bExp),GetObjInfo(PreyKilled,bExp));
	PreyChange->SetDef(tstr);

	tstr.Format(_T("%s * %s"),GetObjInfo(Predator,bExp),GetObjInfo(PredDeathRate,bExp));
	PredKilled->SetDef(tstr);

	tstr.Format(_T("%s * %s"),GetObjInfo(ConvEff,bExp),GetObjInfo(Prey,bExp));
	ConvPerPred->SetDef(tstr);

	tstr.Format(_T("%s * %s"),GetObjInfo(Predator,bExp),GetObjInfo(ConvPerPred,bExp));
	PredInc->SetDef(tstr);

	tstr.Format(_T("%s - %s"),GetObjInfo(PredInc,bExp),GetObjInfo(PredKilled,bExp));
	PredChange->SetDef(tstr);

	tstr.Format(_T("%s / %s"),GetObjInfo(PredDeathRate,bExp),GetObjInfo(ConvEff,bExp));
	PreyStab->SetDef(tstr);
	
	tstr.Format(_T("%s / %s"),GetObjInfo(PreyFertility,bExp),GetObjInfo(CaptEff,bExp));
	PredStab->SetDef(tstr);

//	tstr.Format(_T("%s / %s"),GetObjInfo(PreyFertility,bExp),GetObjInfo(CaptEff,bExp));
//	dN1N1->SetDef(tstr);

//	tstr.Format(_T("%s"),GetObjInfo(CarryingCap,bExp));
//	dN1N2->SetDef(tstr);

//	tstr.Format(_T("%s / %s"),GetObjInfo(PredDeathRate,bExp),GetObjInfo(ConvEff,bExp));
//	dN2N1->SetDef(tstr);

	return m_sText;
}

CString CPP_TwPredationUnltd::GetFormatedEquation()
{
	CString mText;
	mText.Format(_T("    %s \t\t ; %s \r\n\r\n"
				    "    %s \t\t ; %s \r\n\r\n"),
			m_sEqu.GetAt(0),
			m_sEquDef.GetAt(0),
			m_sEqu.GetAt(1),
			m_sEquDef.GetAt(1));

	return mText;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL( CPP_TwLotkaII, CMdEquation, VERSIONABLE_SCHEMA | 1)

void CPP_TwLotkaII::Serialize( CArchive& ar )
{
	CMdEquation::Serialize(ar);
	if( ar.IsStoring( ) )
	{
	}
	else
	{
	}
}

CPP_TwLotkaII::CPP_TwLotkaII() : CMdEquation()
{
	m_sName = "Two-Species Predation (LV type II)";
}

CPP_TwLotkaII::CPP_TwLotkaII(CMdEquation*pEq) : CMdEquation(pEq)
{
	m_sName = "Two-Species Predation (LV type II)";
}


CPP_TwLotkaII::~CPP_TwLotkaII()
{
}

CMdEquation *CPP_TwLotkaII::Clone()
{
	CMdEquation *pEqu = new CPP_TwLotkaII(this);
	return pEqu;
}



void CPP_TwLotkaII::OnNewEquation(CSimModel *pModel)
{
	CMdEquation::OnNewEquation(pModel);

	m_nEquNb = 2;
	if (!m_sEqu.GetSize())
	{
	m_sEqu.Add(_T("dN / dt = r * N - (k * N / (N + d)) * P"));
	m_sEqu.Add(_T("dP / dt = beta * P * N - m * P"));
	m_sEquDef.Add(_T("Prey density evolution"));
	m_sEquDef.Add(_T("Predator density evolution"));
	}
	if (!m_cVarSet.GetSize())
	{
	// The variables 
	CModelVar *pVar = NULL;
//	m_cVarSet.Add(pVar = new CModelVar("Time"));
//	pVar->SetDef(_T("t"));
//	pVar->SetInitValue(-1);
	m_cVarSet.Add(pVar = new CModelTime("Time"));			
	pVar->SetDef(_T("t"));

	m_cVarSet.Add(pVar = new CModelVar("Prey"));
	pVar->SetInitValue(5);
	pVar->SetColor(RGB(0,128,0));
	//pVar->SetAbbrev(_T("Prey"));
	m_cVarSet.Add(pVar = new CModelVar("Predator"));
	pVar->SetInitValue(6);
	pVar->SetColor(RGB(255,0,0));


	m_cVarSet.Add(pVar = new CModelVar("PreyChange"));
	pVar->SetDef(_T("PreyBorn - PreyKilled"));

		m_cVarSet.Add(pVar = new CModelVar("PreyBorn"));
		pVar->SetDef(_T("N * PreyIncRate"));

			m_cVarSet.Add(pVar = new CModelVar("PreyIncRate"));
			pVar->SetDef(_T("r"));

		m_cVarSet.Add(pVar = new CModelVar("PreyKilled"));
		pVar->SetDef(_T("P * CaptPerPred"));
	
			m_cVarSet.Add(pVar = new CModelVar("CaptPerPred"));
			pVar->SetDef(_T("k * N / (N + d)"));

	m_cVarSet.Add(pVar = new CModelVar("PredChange"));
	pVar->SetDef(_T("PredInc - PredKilled"));

		m_cVarSet.Add(pVar = new CModelVar("PredInc"));
		pVar->SetDef(_T("P * ConvPerPred"));

			m_cVarSet.Add(pVar = new CModelVar("ConvPerPred"));
			pVar->SetDef(_T("beta * N"));

		m_cVarSet.Add(pVar = new CModelVar("PredKilled"));
		pVar->SetDef(_T("P * m"));

//	m_cVarSet.Add(pVar = new CModelVar("Prey_Stab"));			
//	pVar->SetDef(_T("m / beta"));
//	m_cVarSet.Add(pVar = new CModelVar("Pred_Stab"));			
//	pVar->SetDef(_T("r / alpha - m / (alpha * beta * K)"));

	//m_cVarSet.Add(pVar = new CModelVar("dN1/dt=0 (N1)"));			
	//pVar->SetDef(_T("K1/a"));
	//m_cVarSet.Add(pVar = new CModelVar("dN1/dt=0 (N2)"));			
	//pVar->SetDef(_T("K1"));
	//m_cVarSet.Add(pVar = new CModelVar("dN2/dt=0 (N1)"));			
	//pVar->SetDef(_T("K2"));
	//m_cVarSet.Add(pVar = new CModelVar("dN1/dt=0 (N2)"));			
	//pVar->SetDef(_T("K2/b"));
	}

		if (!m_cParSet.GetSize())
	{
// The parametres
	CModelPar *pPar=NULL;
//	m_cParSet.Add(pPar = new CModelPar("Time",100));						
//	pPar->SetInfos(100,100,200);
	m_cParSet.Add(pPar= new CModelPar("PreyFertility",0.3));
	pPar->SetInfos(0.3,0.001,0.5);
	//pPar->SetAbbrev(_T("PredFert"));
	m_cParSet.Add(pPar= new CModelPar("MaxFeedRate"));
	pPar->SetInfos(0.03,0.0001,1.);
	pPar->SetDesc("Maximum Feeding Rate");
	pPar->SetDef("k");
	m_cParSet.Add(pPar= new CModelPar("HalfSat"));
	pPar->SetInfos(0.03,0.0001,1.);
	pPar->SetDesc("Half-Saturation Constant");
	pPar->SetDef("d");
	//pPar->SetAbbrev(_T("Cov"));
	m_cParSet.Add(pPar= new CModelPar("PredDeathRate",0.05));
	pPar->SetInfos(0.05,0.0001,0.1);
	//pPar->SetAbbrev(_T("PredDR"));
	m_cParSet.Add(pPar= new CModelPar("ConvEff"));
	pPar->SetInfos(0.0027,0.0001,0.1);
	pPar->SetDesc("Conversion Efficiency");
	pPar->SetDef("beta");
	//pPar->SetAbbrev(_T("PredDR"));
	m_cParSet.Add(pPar= new CModelPar("Prey_init",20));
	pPar->SetInfos(20,0,50);
	pPar->SetInitValue(1);
	pPar->SetColor(RGB(0,128,0));
	//pPar->SetAbbrev(_T("PreyInit"));
	m_cParSet.Add(pPar= new CModelPar("Predator_init",5));
	pPar->SetInfos(5,0,50);
	pPar->SetInitValue(2);
	pPar->SetColor(RGB(255,0,0));
	//pPar->SetAbbrev(_T("PredInit"));
		}
}

void CPP_TwLotkaII::OnUpdateFrom(CLearningUnit *m_pLU,int nExpSet,int nTime,BOOL bInit)
{
	int m_nTimer = m_pLU->GetMaxTimer();
	int nb=0;

//	CMdData *pTime= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *PreyFertility	= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *MaxFeedRate	= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *HalfSat		= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *PredDeathRate	= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *ConvEff		= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *PreyInit		= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *PredInit		= m_pLU->m_cDataPoints.GetAt(nb++);

	CMdData *pTime= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *Prey= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *Predator= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *PreyChange= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *PreyBorn= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *PreyIncRate= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *PreyKilled= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *CaptPerPred= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *PredChange= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *PredInc= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *ConvPerPred= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *PredKilled= m_pLU->m_cDataPoints.GetAt(nb++);
//	CMdData *PreyStab= m_pLU->m_cDataPoints.GetAt(nb++);
//	CMdData *PredStab= m_pLU->m_cDataPoints.GetAt(nb++);

	double  s_prey,s_pred;
	nb = 0;

	s_prey = m_pLU->m_cExpSet.GetAt(nExpSet)->GetValue(5);
	//PreyInit->SetAt(ppp,0,nExpSet);
	//if (nTime == 0)
	//	s_prey = ppp;
	//else
	//	s_prey = Prey->GetAt(nTime,nExpSet);
	s_pred = m_pLU->m_cExpSet.GetAt(nExpSet)->GetValue(6);
	//PredInit->SetAt(ppp,0,nExpSet);
	//if (nTime == 0)
	//	s_pred = ppp;
	//else
	//	s_pred = Predator->GetAt(nTime,nExpSet);

	double pri,pdi,pf,pd,kk,dd,bet;
	if (nTime==0 && bInit)
	{
		pf = m_pLU->m_cExpSet.GetAt(nExpSet)->GetValue(0);
		PreyFertility->SetAt(pf,0,nExpSet);
		kk = m_pLU->m_cExpSet.GetAt(nExpSet)->GetValue(1);
		MaxFeedRate->SetAt(kk,0,nExpSet);
		dd = m_pLU->m_cExpSet.GetAt(nExpSet)->GetValue(2);
		HalfSat->SetAt(dd,0,nExpSet);
		pd = m_pLU->m_cExpSet.GetAt(nExpSet)->GetValue(3);
		PredDeathRate->SetAt(pd,0,nExpSet);
		bet = m_pLU->m_cExpSet.GetAt(nExpSet)->GetValue(4);
		ConvEff->SetAt(bet,0,nExpSet);
		pri = m_pLU->m_cExpSet.GetAt(nExpSet)->GetValue(5);
		PreyInit->SetAt(pri,0,nExpSet);
		pdi = m_pLU->m_cExpSet.GetAt(nExpSet)->GetValue(6);
		PredInit->SetAt(pdi,0,nExpSet);

		s_prey = pri;
		s_pred = pdi;
	}
	else
	{
		pri = PreyInit->GetAt(nTime,nExpSet);
		pdi = PredInit->GetAt(nTime,nExpSet);
		pf = PreyFertility->GetAt(nTime,nExpSet);
		pd = PredDeathRate->GetAt(nTime,nExpSet);
		kk = MaxFeedRate->GetAt(nTime,nExpSet);
		dd = HalfSat->GetAt(nTime,nExpSet);
		bet = ConvEff->GetAt(nTime,nExpSet);


		if (nTime==0)
		{
			s_prey = pri;
			s_pred = pdi;
		}
		else
		{
			s_prey = Prey->GetAt(nTime,nExpSet);
			s_pred = Predator->GetAt(nTime,nExpSet);
		}
	}
	for (int j=nTime;j<=m_nTimer;j++)
	{
		pTime->SetAt(j,j,nExpSet);
		Prey->SetAt(s_prey,j,nExpSet);
		Predator->SetAt(s_pred,j,nExpSet);

		PreyInit->SetAt(pri,j,nExpSet);
		PredInit->SetAt(pdi,j,nExpSet);
		PreyFertility->SetAt(pf,j,nExpSet);
		PredDeathRate->SetAt(pd,j,nExpSet);
		ConvEff->SetAt(bet,j,nExpSet);
		MaxFeedRate->SetAt(kk,j,nExpSet);
		HalfSat->SetAt(dd,j,nExpSet);

		double nPredKilled,nConvPerPred,nPredInc,
				nPredChange,nCaptPerPred,nPreyKilled ,
				nPreyIncRate,nPreyBorn,nPreyChange ;
		double  nnPreyChange = 0,
				nnPredatorChange = 0;
		
		for (int k=0;k<10;k++)
		{

			nPredKilled = pd * s_pred;
			nConvPerPred = bet * s_prey;
			nPredInc = s_pred * nConvPerPred;
			nPredChange = nPredInc - nPredKilled;
			
			nCaptPerPred = kk * s_prey / (s_prey + dd);
			nPreyKilled = s_pred * nCaptPerPred;
			nPreyIncRate = pf;
			nPreyBorn = s_prey * nPreyIncRate;
			nPreyChange = nPreyBorn - nPreyKilled;

			nnPreyChange += 0.1 * nPreyChange;
			nnPredatorChange += 0.1 * nPredChange ;

			double ss1 = s_prey + 0.1 * nPreyChange;
			double ss2 = s_pred + 0.1 * nPredChange;
			s_prey = ss1;
			s_pred = ss2;
			if (!k)
			{
				PredKilled->SetAt(nPredKilled,j,nExpSet);
				ConvPerPred->SetAt(nConvPerPred,j,nExpSet);
				PredInc->SetAt(nPredInc,j,nExpSet);
				//PredChange->SetAt(nPredChange,j,nExpSet);
				CaptPerPred->SetAt(nCaptPerPred,j,nExpSet);
				PreyKilled->SetAt(nPreyKilled,j,nExpSet);
				PreyIncRate->SetAt(nPreyIncRate,j,nExpSet);
				PreyBorn->SetAt(nPreyBorn,j,nExpSet);
				//PreyChange->SetAt(nPreyChange,j,nExpSet);

//				PreyStab->SetAt(0,j,nExpSet);
//				PredStab->SetAt(0,j,nExpSet);
			}
		}
			
		PreyChange->SetAt(nnPreyChange,j,nExpSet);
		PredChange->SetAt(nnPredatorChange,j,nExpSet);
	}
}

CString CPP_TwLotkaII::GetFormatedText(int nLevel)
{
	BOOL bExp = (nLevel==2);
	//m_sText = GetFormatedEquation();

	int nb = 0;
	CModelObj *PreyFertility= m_cParSet.GetAt(nb++);
	CModelObj *MaxFeedRate= m_cParSet.GetAt(nb++);
	CModelObj *HalfSat= m_cParSet.GetAt(nb++);
	CModelObj *PredDeathRate= m_cParSet.GetAt(nb++);
	CModelObj *ConvEff= m_cParSet.GetAt(nb++);
	CModelObj *PreyInit= m_cParSet.GetAt(nb++);
	CModelObj *PredInit= m_cParSet.GetAt(nb++);

	nb = 0;
	CModelObj *pTime= m_cVarSet.GetAt(nb++);
	CModelObj *Prey= m_cVarSet.GetAt(nb++);
	CModelObj *Predator= m_cVarSet.GetAt(nb++);
	CModelObj *PreyChange= m_cVarSet.GetAt(nb++);
	CModelObj *PreyBorn= m_cVarSet.GetAt(nb++);
	CModelObj *PreyIncRate= m_cVarSet.GetAt(nb++);
	CModelObj *PreyKilled= m_cVarSet.GetAt(nb++);
	CModelObj *CaptPerPred= m_cVarSet.GetAt(nb++);
	CModelObj *PredChange= m_cVarSet.GetAt(nb++);
	CModelObj *PredInc= m_cVarSet.GetAt(nb++);
	CModelObj *ConvPerPred= m_cVarSet.GetAt(nb++);
	CModelObj *PredKilled= m_cVarSet.GetAt(nb++);
//	CModelObj *PreyStab= m_cVarSet.GetAt(nb++);
//	CModelObj *PredStab= m_cVarSet.GetAt(nb++);
//	CModelObj *dN1N1= m_cVarSet.GetAt(nb++);
//	CModelObj *dN1N2= m_cVarSet.GetAt(nb++);
//	CModelObj *dN2N1= m_cVarSet.GetAt(nb++);

	CString tstr;

	tstr.Format(_T("t"));
	pTime->SetDef(tstr);
	tstr.Format(_T("N"));
	Prey->SetDef(tstr);
	tstr.Format(_T("P"));
	Predator->SetDef(tstr);
	tstr.Format(_T("N[0]"));
	PreyInit->SetDef(tstr);
	tstr.Format(_T("P[0]"));
	PredInit->SetDef(tstr);

	tstr.Format(_T("r"));
	PreyFertility->SetDef(tstr);
	tstr.Format(_T("k"));
	MaxFeedRate->SetDef(tstr);
	tstr.Format(_T("d"));
	HalfSat->SetDef(tstr);
	tstr.Format(_T("m"));
	PredDeathRate->SetDef(tstr);
	tstr.Format(_T("beta"));
	ConvEff->SetDef(tstr);

	tstr.Format(_T("%s * %s / (%s + %s)"),GetObjInfo(MaxFeedRate,bExp),GetObjInfo(Prey,bExp)
					,GetObjInfo(Prey,bExp),GetObjInfo(HalfSat,bExp));
	CaptPerPred->SetDef(tstr);

	tstr.Format(_T("%s * %s"),GetObjInfo(Predator,bExp),GetObjInfo(CaptPerPred,bExp));
	PreyKilled->SetDef(tstr);

	tstr.Format(_T("%s"),GetObjInfo(PreyFertility,bExp));
	PreyIncRate->SetDef(tstr);

	tstr.Format(_T("%s * %s"),GetObjInfo(Prey,bExp),GetObjInfo(PreyIncRate,bExp));
	PreyBorn->SetDef(tstr);

	tstr.Format(_T("%s - %s"),GetObjInfo(PreyBorn,bExp),GetObjInfo(PreyKilled,bExp));
	PreyChange->SetDef(tstr);

	tstr.Format(_T("%s * %s"),GetObjInfo(Predator,bExp),GetObjInfo(PredDeathRate,bExp));
	PredKilled->SetDef(tstr);

	tstr.Format(_T("%s * %s"),GetObjInfo(ConvEff,bExp),GetObjInfo(Prey,bExp));
	ConvPerPred->SetDef(tstr);

	tstr.Format(_T("%s * %s"),GetObjInfo(Predator,bExp),GetObjInfo(ConvPerPred,bExp));
	PredInc->SetDef(tstr);

	tstr.Format(_T("%s - %s"),GetObjInfo(PredInc,bExp),GetObjInfo(PredKilled,bExp));
	PredChange->SetDef(tstr);

//	tstr.Format(_T("%s / %s"),GetObjInfo(PredDeathRate,bExp),GetObjInfo(ConvEff,bExp));
//	PreyStab->SetDef(tstr);
	
//	tstr.Format(_T("%s / %s"),GetObjInfo(PreyFertility,bExp),GetObjInfo(CaptEff,bExp));
//	PredStab->SetDef(tstr);

//	tstr.Format(_T("%s / %s"),GetObjInfo(PreyFertility,bExp),GetObjInfo(CaptEff,bExp));
//	dN1N1->SetDef(tstr);

//	tstr.Format(_T("%s"),GetObjInfo(CarryingCap,bExp));
//	dN1N2->SetDef(tstr);

//	tstr.Format(_T("%s / %s"),GetObjInfo(PredDeathRate,bExp),GetObjInfo(ConvEff,bExp));
//	dN2N1->SetDef(tstr);

	return m_sText;
}

CString CPP_TwLotkaII::GetFormatedEquation()
{
	CString mText;
	mText.Format(_T("    %s \t\t ; %s \r\n\r\n"
				    "    %s \t\t ; %s \r\n\r\n"),
			m_sEqu.GetAt(0),
			m_sEquDef.GetAt(0),
			m_sEqu.GetAt(1),
			m_sEquDef.GetAt(1));

	return mText;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL( CPP_Pendulum, CMdEquation, VERSIONABLE_SCHEMA | 1)

void CPP_Pendulum::Serialize( CArchive& ar )
{
	CMdEquation::Serialize(ar);
	if( ar.IsStoring( ) )
	{
	}
	else
	{
	}
}

CPP_Pendulum::CPP_Pendulum(CMdEquation*pEq) : CMdEquation(pEq)
{
	m_sName = "Simple pendulum";
}

CPP_Pendulum::CPP_Pendulum() : CMdEquation()
{
	m_sName = "Simple pendulum";
}

CMdEquation *CPP_Pendulum::Clone()
{
	CMdEquation *pEqu = new CPP_Pendulum(this);
	return pEqu;
}



CPP_Pendulum::~CPP_Pendulum()
{
}

double CPP_Pendulum::dThetaFunc(double theta,double param[])
{
	double res;
	double	omega = param[0];
	res = omega;
	return res;
}

double CPP_Pendulum::dOmegaFunc(double omega,double param[])
{
	double res;
	double	theta = param[0],
			g = param[1],
			l = param[2];
	double ss = sin(theta);
	res = -(g / l) * ss;
	return res;
}

void CPP_Pendulum::OnNewEquation(CSimModel *pModel)
{
	if (!pModel) return;

	m_nEquNb = 2;
	if (!m_sEqu.GetSize())
	{
	m_sEqu.Add(_T("domega / dt = -g/l * sin(theta)"));
	m_sEqu.Add(_T("dtheta / dt = omega"));
	m_sEquDef.Add(_T("first-order derivation"));
	m_sEquDef.Add(_T("Second-order derivation"));
	}
	if (!m_cVarSet.GetSize())
	{
	// The variables 
	CModelVar *pVar = NULL;
	m_cVarSet.Add(pVar = new CModelTime("Time"));			
	pVar->SetDef(_T("t"));
	//pVar->SetInitValue(-1);

	m_cVarSet.Add(pVar = new CModelVar("Theta"));				
	pVar->SetInitValue(3);
	pVar->SetDef(_T("Theta"));
	pVar->SetColor(RGB(255,0,0));

	m_cVarSet.Add(pVar = new CModelVar("Omega"));				
	pVar->SetInitValue(4);
	pVar->SetDef(_T("Omega"));
	pVar->SetColor(RGB(255,0,0));

	m_cVarSet.Add(pVar = new CModelVar("X"));				
	//pVar->SetInitValue(4);
	pVar->SetDef(_T("X"));
	///pVar->SetColor(RGB(255,0,0));

	m_cVarSet.Add(pVar = new CModelVar("Y"));				
	//pVar->SetInitValue(4);
	pVar->SetDef(_T("Y"));
	///pVar->SetColor(RGB(255,0,0));

	m_cVarSet.Add(pVar = new CModelVar("V"));				
	pVar->SetDef(_T("V"));
	m_cVarSet.Add(pVar = new CModelVar("Vx"));				
	pVar->SetDef(_T("Vx"));
	m_cVarSet.Add(pVar = new CModelVar("Vy"));				
	pVar->SetDef(_T("Vy"));

	m_cVarSet.Add(pVar = new CModelVar("An"));				
	pVar->SetDef(_T("An"));
	m_cVarSet.Add(pVar = new CModelVar("At"));				
	pVar->SetDef(_T("At"));
	m_cVarSet.Add(pVar = new CModelVar("A"));				
	pVar->SetDef(_T("A"));

	m_cVarSet.Add(pVar = new CModelVar("T"));				
	pVar->SetDef(_T("T"));
	m_cVarSet.Add(pVar = new CModelVar("Tx"));				
	pVar->SetDef(_T("Tx"));
	m_cVarSet.Add(pVar = new CModelVar("Ty"));				
	pVar->SetDef(_T("Ty"));

	m_cVarSet.Add(pVar = new CModelVar("Fg"));				
	pVar->SetDef(_T("Fg"));
	m_cVarSet.Add(pVar = new CModelVar("Fgy"));				
	pVar->SetDef(_T("Fgy"));

	m_cVarSet.Add(pVar = new CModelVar("Fres"));				
	pVar->SetDef(_T("Fres"));
	m_cVarSet.Add(pVar = new CModelVar("Fresx"));				
	pVar->SetDef(_T("Fresx"));
	m_cVarSet.Add(pVar = new CModelVar("Fresy"));				
	pVar->SetDef(_T("Fresy"));

	m_cVarSet.Add(pVar = new CModelVar("Ax"));				
	pVar->SetDef(_T("Ax"));
	m_cVarSet.Add(pVar = new CModelVar("Ay"));				
	pVar->SetDef(_T("Ay"));
	m_cVarSet.Add(pVar = new CModelVar("Anx"));				
	pVar->SetDef(_T("Anx"));
	m_cVarSet.Add(pVar = new CModelVar("Any"));				
	pVar->SetDef(_T("Any"));
	m_cVarSet.Add(pVar = new CModelVar("Atx"));				
	pVar->SetDef(_T("Atx"));
	m_cVarSet.Add(pVar = new CModelVar("Aty"));				
	pVar->SetDef(_T("Aty"));
	}

	if (!m_cParSet.GetSize())
	{
	// The parametres
	CModelPar *pPar=NULL;
	m_cParSet.Add(pPar= new CModelPar("g",10));		// 1
	pPar->SetInfos(10,1,15);
	pPar->SetDef(_T("Gravitation Constant"));

	m_cParSet.Add(pPar= new CModelPar("l",1));
	pPar->SetDef(_T("Line Length"));
	pPar->SetInfos(1,0.1,10);

	m_cParSet.Add(pPar= new CModelPar("m",0.1));		// 2
	pPar->SetInfos(0.1,0.1,1);
	pPar->SetDef(_T("Pendulum Mass"));

	m_cParSet.Add(pPar= new CModelPar("Theta_init",0.65));		// 0
	pPar->SetInfos(0.65,0,1);
	pPar->SetInitValue(1);
	pPar->SetDef(_T("Theta(0)"));
	pPar->SetColor(RGB(255,0,0));

	m_cParSet.Add(pPar= new CModelPar("Omega_init",0));		// 0
	pPar->SetInfos(0,0,1);
	pPar->SetInitValue(2);
	pPar->SetDef(_T("Omega(0)"));
	pPar->SetColor(RGB(255,0,0));
	}
}

void CPP_Pendulum::OnUpdateFrom(CLearningUnit *m_pLU,int nExpSet,int nTime,BOOL bInit)
{
	int m_nTimer = m_pLU->GetMaxTimer();

	int nb=0;
//	CMdData *pTime			= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *PendG			= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *PendL			= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *PendM			= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *Theta_init		= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *Omega_init		= m_pLU->m_cDataPoints.GetAt(nb++);

	CMdData *pTime			= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *pTheta			= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *pOmeha			= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *pX				= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *pY				= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *pV				= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *pVx			= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *pVy			= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *pAn			= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *pAt			= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *pA				= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *pT				= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *pTx			= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *pTy			= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *pFgy			= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *pFg			= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *pFres			= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *pFresx			= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *pFresy			= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *pAx			= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *pAy			= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *pAnx			= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *pAny			= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *pAtx			= m_pLU->m_cDataPoints.GetAt(nb++);
	CMdData *pAty			= m_pLU->m_cDataPoints.GetAt(nb++);

	double  s_theta = m_pLU->m_cExpSet.GetAt(nExpSet)->GetValue(3);
	double  s_omega = m_pLU->m_cExpSet.GetAt(nExpSet)->GetValue(4);
	//Theta_init->SetAt(s_theta,0,nExpSet);
	//Omega_init->SetAt(s_omega,0,nExpSet);
	//if (nTime == 0)
	//{
	//	s_theta = s_theta;
	//	s_omega = s_omega;
	//}
	//else
	//{
	//	s_theta = pTheta->GetAt(nTime,nExpSet);
	//	s_omega = pOmeha->GetAt(nTime,nExpSet);
	//}

	double pti,poi,pg,pl,pm;
	if (nTime==0 && bInit)
	{
		pti = m_pLU->m_cExpSet.GetAt(nExpSet)->GetValue(3);
		Theta_init->SetAt(pti,0,nExpSet);
		poi = m_pLU->m_cExpSet.GetAt(nExpSet)->GetValue(4);
		Omega_init->SetAt(poi,0,nExpSet);
		pg = m_pLU->m_cExpSet.GetAt(nExpSet)->GetValue(0);
		PendG->SetAt(pg,0,nExpSet);
		pl = m_pLU->m_cExpSet.GetAt(nExpSet)->GetValue(1);
		PendL->SetAt(pl,0,nExpSet);
		pm = m_pLU->m_cExpSet.GetAt(nExpSet)->GetValue(2);
		PendM->SetAt(pm,0,nExpSet);

		s_theta = pti;
		s_omega = poi;
	}
	else
	{
		pti = Theta_init->GetAt(nTime,nExpSet);
		poi = Omega_init->GetAt(nTime,nExpSet);
		pg = PendG->GetAt(nTime,nExpSet);
		pl = PendL->GetAt(nTime,nExpSet);
		pm = PendM->GetAt(nTime,nExpSet);
	
		if (nTime==0)
		{
			s_theta = pti;
			s_omega = poi;
		}
		else
		{
			s_theta = pTheta->GetAt(nTime,nExpSet);
			s_omega = pOmeha->GetAt(nTime,nExpSet);
		}

	}
	for (int j=nTime;j<=m_nTimer;j++)
	{
		pTime->SetAt(j,j,nExpSet);
		pTheta->SetAt(s_theta,j,nExpSet);
		pOmeha->SetAt(s_omega,j,nExpSet);
			
		Omega_init->SetAt(poi,j,nExpSet);
		Theta_init->SetAt(pti,j,nExpSet);
		PendG->SetAt(pg,j,nExpSet);
		PendL->SetAt(pl,j,nExpSet);
		PendM->SetAt(pm,j,nExpSet);

///		double nPreyIncRate,nPreyChange,nTdouble,nEnvRes;
///		double nnPreyChange =0;

		double pi = 3.1415926535;

   		double x = pl * cos(s_theta + 3*pi/2);
		double y = pl * sin(s_theta + 3*pi/2);

		pX->SetAt(x,j,nExpSet);
		pY->SetAt(y,j,nExpSet);

		double v = fabs(s_omega*pl);

		pV->SetAt(v,j,nExpSet);
		pVx->SetAt(s_omega*pl*cos(s_theta),j,nExpSet);
		pVy->SetAt(s_omega*pl*sin(s_theta),j,nExpSet);

		double an = v*v/pl;
		double at = -pg/pl*sin(s_theta)*pl; 
		pAn->SetAt(an,j,nExpSet);
		pAt->SetAt(at,j,nExpSet);
		pA->SetAt(sqrtl(an*an+at*at),j,nExpSet);

		double T = pm*pg*cos(s_theta) + pm*v*v/pl;
		pT->SetAt(T,j,nExpSet);
		double Tx = -T*cos(s_theta+3*pi/2);
		double Ty = -T*sin(s_theta+3*pi/2);
		pTx->SetAt(Tx,j,nExpSet);
		pTy->SetAt(Ty,j,nExpSet);

		pFgy->SetAt(-pm*pg,j,nExpSet);
		pFg->SetAt(pm*pg,j,nExpSet);

		double fresx = pTx->GetAt(j,nExpSet);
		double fresy = pTy->GetAt(j,nExpSet) + pFgy->GetAt(j,nExpSet);
		pFres->SetAt(sqrtl(fresx*fresx+fresy*fresy),j,nExpSet);
		pFresx->SetAt(fresx,j,nExpSet);
		pFresy->SetAt(fresy,j,nExpSet);

		pAx->SetAt(fresx/pm,j,nExpSet);
		pAy->SetAt(fresy/pm,j,nExpSet);
		pAnx->SetAt(an*Tx/T,j,nExpSet);
		pAny->SetAt(an*Ty/T,j,nExpSet);
		pAtx->SetAt(at*cos(s_theta),j,nExpSet);
		pAty->SetAt(at*sin(s_theta),j,nExpSet);

		double resO,resT;
		{
			double h = 0.01;
			double param[]={s_theta,pg,pl};
			double k1 = h*dOmegaFunc(s_omega,param);
			double k2 = h*dOmegaFunc(s_omega+k1/2,param);
			double k3 = h*dOmegaFunc(s_omega+k2/2,param);
			double k4 = h*dOmegaFunc(s_omega+k3,param);
			resO = s_omega + (k1 + 2*k2 + 2*k3 + k4)/6;
		}
		s_omega = resO;
		{
			double h = 0.01;
			double param[]={s_omega};
			double k1 = h*dThetaFunc(s_theta,param);
			double k2 = h*dThetaFunc(s_theta+k1/2,param);
			double k3 = h*dThetaFunc(s_theta+k2/2,param);
			double k4 = h*dThetaFunc(s_theta+k3,param);
			resT = s_theta + (k1 + 2*k2 + 2*k3 + k4)/6;
		/*	if (resT < -pi)
			{
				while (resT < -pi)
				{
					resT += pi;
				}
			}
			else if (resT > pi)
			{
				while (resT >pi)
					resT-= pi;
			}*/
		}
		s_theta = resT;
	}
}


CString CPP_Pendulum::GetFormatedText(int nLevel)
{
//	m_sText = GetFormatedEquation();
/*	BOOL bExp = (nLevel==2);
	int nb = 0;

	CModelObj *PTime			= m_cVarSet.GetAt(nb++);
	CModelObj *Prey				= m_cVarSet.GetAt(nb++);
	CModelObj *PreyIncRate		= m_cVarSet.GetAt(nb++);
	CModelObj *PreyChange		= m_cVarSet.GetAt(nb++);
	CModelObj *Tdouble			= m_cVarSet.GetAt(nb++);
	CModelObj *TEnvRes			= m_cVarSet.GetAt(nb++);

	nb = 0;
	CModelObj *PreyFert		= m_cParSet.GetAt(nb++);
	CModelObj *PreyDeath	= m_cParSet.GetAt(nb++);
	CModelObj *Carrying		= m_cParSet.GetAt(nb++);
	CModelObj *PreyInit		= m_cParSet.GetAt(nb++);

	CString tstr;

	tstr.Format(_T("(%s - %s) * (1 - %s / %s)"),GetObjInfo(PreyFert,bExp),GetObjInfo(PreyDeath,bExp),
					GetObjInfo(Prey,bExp),GetObjInfo(Carrying,bExp));
	PreyIncRate->SetDef(tstr);
	tstr.Format(_T("%s * %s"),GetObjInfo(Prey,bExp),GetObjInfo(PreyIncRate,bExp));
	PreyChange->SetDef(tstr);
	tstr.Format(_T("%s * %s"),GetObjInfo(Prey,bExp),GetObjInfo(PreyIncRate,bExp));
	PreyChange->SetDef(tstr);
	tstr.Format(_T("ln(2) / %s"),GetObjInfo(PreyIncRate,bExp));
	Tdouble->SetDef(tstr);
	tstr.Format(_T("%s / %s"),GetObjInfo(Prey,bExp),GetObjInfo(Carrying,bExp));
	TEnvRes->SetDef(tstr);

	tstr.Format(_T("t"));
	PTime->SetDef(tstr);
	tstr.Format(_T("N"));
	Prey->SetDef(tstr);
	tstr.Format(_T("N[0]"));
	PreyInit->SetDef(tstr);
	tstr.Format(_T("b"));
	PreyFert->SetDef(tstr);
	tstr.Format(_T("d"));
	PreyDeath->SetDef(tstr);
	tstr.Format(_T("K"));
	Carrying->SetDef(tstr);
*/
	return m_sText;
}

CString CPP_Pendulum::GetFormatedEquation()
{
	CString mText;// =	"dN / dt = (f - g) * N * ( 1 - N / K) \r\n\r\n\r\n"; 
	mText.Format(_T("    %s \t\t ; %s \r\n"
					"    %s \t\t ; %s \r\n\r\n\r\n"),
				m_sEqu.GetAt(0),m_sEquDef.GetAt(0),
				m_sEqu.GetAt(1),m_sEquDef.GetAt(1));

	return mText;
}

/*
	dtheta/dt=omega
	domega/dt=alpha
	alpha=-g/l*sin(theta)
	x=l*cos(theta+3/2*$)
	y=l*sin(theta+3/2*$)
	vt=omega*l
	v=abs(vt)
	vx=vt*cos(theta)
	vy=vt*sin(theta)

	an=v^2/l
	at=alpha*l
	a=#(an^2+at^2)

	T=m*g*cos(theta)+m*v^2/l
	Tx=-T*cos(theta+3/2*$)
	Ty=-T*sin(theta+3/2*$)

	Fgy=-m*g
	Fg=m*g

	Fresx=Tx
	Fresy=Ty+Fgy
	Fres=#(Fresx^2+Fresy^2)

	ax=Fresx/m
	ay=Fresy/m
	anx=an*Tx/T
	any=an*Ty/T
	atx=at*cos(theta)
	aty=at*sin(theta)
*/

