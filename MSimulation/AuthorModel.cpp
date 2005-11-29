// AuthorModel.cpp: implementation of the CAuthorModel class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "simul.h"
#include "AuthorModel.h"

#include <MInstruction\LearningUnit.h>
#include "simul.h"

//#include <math.h>

#include <MathParser\MathParser.h>


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL( CPP_SimModel, CMdEquation, VERSIONABLE_SCHEMA | 1)

void CPP_SimModel::Serialize( CArchive& ar )
{
	CMdEquation::Serialize(ar);
	if( ar.IsStoring( ) )
	{
		ar << m_strModel;
		ar << m_nEulerH;
		ar << m_nNumApprox;
	}
	else
	{
		ar >> m_strModel;
		ar >> m_nEulerH;
		ar >> m_nNumApprox;

		SetDependency();

	}
}

CPP_SimModel::CPP_SimModel() : CMdEquation()
{
	m_sName = "New Simulation Model";
	m_nEulerH = 0.01;
	m_nNumApprox = 0;
}

CPP_SimModel::CPP_SimModel(CMdEquation*pEq) : CMdEquation(pEq)
{
	m_sName = "New Simulation Model";
	m_strModel = ((CPP_SimModel*)pEq)->m_strModel;
	m_nEulerH = ((CPP_SimModel*)pEq)->m_nEulerH;
	m_nNumApprox = ((CPP_SimModel*)pEq)->m_nNumApprox;
}



CMdEquation *CPP_SimModel::Clone()
{
	CMdEquation *pEqu = new CPP_SimModel(this);
	return pEqu;
}



CPP_SimModel::~CPP_SimModel()
{
}

void CPP_SimModel::OnUpdateFrom(CLearningUnit *m_pLU,int nExpSet,int nTime,BOOL bInit)
{
	int m_nTimer = m_pLU->GetMaxTimer();

	int nbVar = m_cVarSet.GetSize();
	int nbPar = m_cParSet.GetSize();

	int nbOut = m_pLU->m_cDataPoints.GetSize();

	if (!nbOut) return;

	int nbExpSet = m_pLU->m_cExpSet.GetSize();

	int nbValue = m_pLU->m_cExpSet.GetAt(nExpSet)->GetSize();

	int nbVarS = 0;
	int nbVarE = 0;

	BOOL bVar = FALSE;

	CMathParser mp;
	CStrMap varmap;


	double *m_value = new double[nbOut];
	for (int h=0;h<nbOut;h++)
	{
		CString mstr;
		CMdData* pData= m_pLU->m_cDataPoints.GetAt(h);

		CModelObj * pObj = pData->GetDataInfo();

		mstr = pObj->GetAbbrev();

		varmap.AddString(mstr.GetBuffer(mstr.GetLength()),&(m_value[h]));
	}

	mp.Parameters=&varmap;


	// Get Variables' range in outcomes
	for (int k=0;k<nbVar;k++)
	{
		CModelVar *pVar = m_cVarSet.GetAt(k);
		if (!pVar) continue;

		if (pVar->GetInitValue()!=-1)
		{
			if (nbVarS==0) nbVarS = nbPar+k;
			nbVarE = nbPar+k;
			bVar = TRUE;
		}
	}

	if (nTime==0 && bInit)
	{
		// Set the value for the parameters
		for (int i=0;i<nbValue;i++)
		{
			CMdExpertSet *pESet = m_pLU->m_cExpSet.GetAt(nExpSet);
			CMdData		 *pPar  = m_pLU->m_cDataPoints.GetAt(i);
			if (!pESet) continue;
			
			double dd =pESet->GetValue(i);

			m_value[i] = dd;

			pPar->SetAt(dd,0,nExpSet);
		}
		// Set the value for the variables (i.e. initial value)
		if (bVar) for (i=nbVarS;i<=nbVarE;i++)
		{
			m_value[i] = m_value[i-nbVarS] ;
		}
	}
	else
	{
		// Set the value for the parameters
		for (int i=0;i<nbValue;i++)
		{
			CMdData		 *pPar  = m_pLU->m_cDataPoints.GetAt(i);

			double dd =pPar->GetAt(nTime,nExpSet);
			m_value[i] = dd;
		}
		if (nTime==0)
		{
			// Set the value for the variables (i.e. initial value)
			if (bVar) for (int i2=nbVarS;i2<=nbVarE;i2++)
			{
				m_value[i2] = m_value[i2-nbVarS] ;
			}
		}
		else
		{
			// Set the value for the variables (i.e. initial value)
			if (bVar) for (int i2=nbVarS;i2<=nbVarE;i2++)
			{
				CMdData		 *pPar  = m_pLU->m_cDataPoints.GetAt(i2);
				double dd = pPar->GetAt(nTime,nExpSet);
				m_value[i2] = dd ;
			}
			//s_theta = pTheta->GetAt(nTime,nExpSet);
			//s_omega = pOmeha->GetAt(nTime,nExpSet);
		}
	}

	for (int j=nTime;j<=m_nTimer;j++)
	{
		// TIME
		CMdData *pTime	= m_pLU->m_cDataPoints.GetAt(nbPar);
		m_value[nbPar] = j*m_nEulerH;
		pTime->SetAt(j*m_nEulerH,j,nExpSet);
		CModelObj * pObj = pTime->GetDataInfo();
		CString mstr = pObj->GetAbbrev();
		varmap.Replace(mstr.GetBuffer(mstr.GetLength()),&(m_value[nbPar]));

		// Set the value for the parameters
		for (int i=0;i<nbValue;i++)
		{
			CMdData *pTheta	= m_pLU->m_cDataPoints.GetAt(i);
			pTheta->SetAt(m_value[i],j,nExpSet);
			pObj = pTheta->GetDataInfo();
			mstr = pObj->GetAbbrev();
			varmap.Replace(mstr.GetBuffer(mstr.GetLength()),&(m_value[i]));

		}
		// Set the value for the variables
		if (bVar) for (i=nbVarS;i<=nbVarE;i++)
		{
			CMdData *pTheta	= m_pLU->m_cDataPoints.GetAt(i);
			pTheta->SetAt(m_value[i],j,nExpSet);
			pObj = pTheta->GetDataInfo();
			mstr = pObj->GetAbbrev();
			varmap.Replace(mstr.GetBuffer(mstr.GetLength()),&(m_value[i]));
		}
		// Set the value for the aggregates
		for (i=nbVarE+1;i<nbOut;i++)
		{
			//CMdData *pTheta	= m_pLU->m_cDataPoints.GetAt(i);
			//m_value[i] = 50.0;
			//pTheta->SetAt(m_value[i],j,nExpSet);
			//pObj = pTheta->GetDataInfo();
			//mstr = pObj->GetAbbrev();
			//varmap.Replace(mstr.GetBuffer(mstr.GetLength()),&(m_value[i]));

		
			CMdData *pData	= m_pLU->m_cDataPoints.GetAt(i);
			CModelObj * pObj = pData->GetDataInfo();
			mstr = pObj->GetAbbrev();
	
			CString mstr2;
			mstr2 = pObj->GetDef();
			int nb = mstr2.Find(_T("="));
			if (nb!=-1)
			{
				mstr2.Delete(0,nb+1);
			}

			double result;

			char *ErrMsg = mp.Parse( mstr2.GetBuffer(mstr2.GetLength()), &result );

			m_value[i] = result;
		
			pData->SetAt(m_value[i],j,nExpSet);
			mstr = pObj->GetAbbrev();
			varmap.Replace(mstr.GetBuffer(mstr.GetLength()),&(m_value[i]));
		
		}

		/*// Test the varmap content
		int nb2 = varmap.GetSize();
		for (int gj=0;gj<nb2;gj++)
		{
			int gg=0;
			double *value;
			char * ttt= varmap.GetString(gj,&gg,(void**) &value);
			char ttty[255];
			strncpy(ttty,ttt,gg);
			ttty[gg]='\0';
			CString ff = ttty;
		}*/

		// Calculate the new variables' value
		if (bVar) for (i=nbVarS;i<=nbVarE;i++)
		{
			CMdData *pData	= m_pLU->m_cDataPoints.GetAt(i);
			CModelObj * pObj = pData->GetDataInfo();
			mstr = pObj->GetAbbrev();
	
			//m_value[i] = 50.0;
			//pTheta->SetAt(m_value[i],j,nExpSet);

			CString mstr2;
			mstr2 = pObj->GetDef();
			int nb = mstr2.Find(_T("="));
			if (nb!=-1)
			{
				mstr2.Delete(0,nb+1);
			}

			double result;

			//if (i<=nbVarE)
			{
				double resO, resT=m_value[i];
				double h = m_nEulerH;
				//double param[]={s_theta,pg,pl};
				double k1 = 0;//h*dOmegaFunc(s_omega,param);
				double k2 = 0;//h*dOmegaFunc(s_omega+k1/2,param);
				double k3 = 0;//h*dOmegaFunc(s_omega+k2/2,param);
				double k4 = 0;//h*dOmegaFunc(s_omega+k3,param);
				resO = m_value[i] + (k1 + 2*k2 + 2*k3 + k4)/6;

				char *ErrMsg = mp.Parse( mstr2.GetBuffer(mstr2.GetLength()), &k1 );
				k1 = h*k1;
				m_value[i] = resT+k1/2;
				varmap.Replace(mstr.GetBuffer(mstr.GetLength()),&(m_value[i]));

				ErrMsg = mp.Parse( mstr2.GetBuffer(mstr2.GetLength()), &k2 );
				k2 = h*k2;
				m_value[i] = resT+k2/2;
				varmap.Replace(mstr.GetBuffer(mstr.GetLength()),&(m_value[i]));

				ErrMsg = mp.Parse( mstr2.GetBuffer(mstr2.GetLength()), &k3 );
				k3 = h*k3;
				m_value[i] = resT+k3;
				varmap.Replace(mstr.GetBuffer(mstr.GetLength()),&(m_value[i]));

				ErrMsg = mp.Parse( mstr2.GetBuffer(mstr2.GetLength()), &k4);
				k4 = h*k4;

				result = resT + (k1 + 2*k2 + 2*k3 + k4)/6;

			}
			/*else
			{
				char *ErrMsg = mp.Parse( mstr2.GetBuffer(mstr2.GetLength()), &result );
			}*/
			m_value[i] = result;


		}

	}

	delete[] m_value;
}


//////////////////////////////////////////////////////////////////////
/// Verify if the mathematical model stored in #m_strModel is correct.
/// \return TRUE if the model is mathematically correct, FALSE otherwise.
///
/// NOT YET IMPLEMENTED
//////////////////////////////////////////////////////////////////////
BOOL CPP_SimModel::CheckValidity()
{
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
/// Determine the dependency between the entities extracted from the model.
///
/// NOT YET IMPLEMENTED
//////////////////////////////////////////////////////////////////////
void CPP_SimModel::SetDependency()
{
	int nbVar = m_cVarSet.GetSize();
	int nbPar = m_cParSet.GetSize();

	for (int i=0;i<nbVar;i++)
	{
		CModelVar * pVar = m_cVarSet.GetAt(i);
		if (!pVar) continue;

		pVar->m_cDepObj.RemoveAll();

		if (i==9 || i==10)
		{
			CString strDD = pVar->GetDef();

			int nf = strDD.Find(_T("-T"));
			if (nf!=-1)
			{
				CString st1 = strDD.Left(nf);
				CString st2 = strDD.Right(strDD.GetLength()-nf-2);
				CString st3 = st1 + _T("TN") + st2;
				//pVar->SetDef(st3);
			}

		}

		CString strEq = pVar->GetDef();
		int nbf = strEq.Find(_T("="));
		if (nbf==-1) continue;
		strEq.Delete(0,nbf+1);

		CMathParser mp;
		CStrMap varmap;
		double res;
		double varvalues[] = { 5, 6.1,0.1,5,2,3,4,8,.3,.5 };

		mp.Parameters=&varmap;

		int nbV3 = 0;
		char *ErrMsg = mp.Parse( strEq.GetBuffer(strEq.GetLength()), &res );
		while (ErrMsg)
		{
			CString strT1 = ErrMsg;
			CString strT(_T(" - Unknown function/variable!"));
			if (strT1.Find(strT) != -1)
			{
				strT.MakeReverse();
				strT1 = ErrMsg;
				strT1.MakeReverse();
				int mm = strT1.Replace(strT,_T(""));
				strT1.MakeReverse();
				strT1.Delete(0,2);

				char tt[255];
				strcpy(tt,strT1.GetBuffer(strT1.GetLength()));
				varmap.AddString(tt,varvalues+(nbV3++));

				BOOL bFound = FALSE;

				for (int j=0;j<nbPar && !bFound;j++)
				{
					CModelPar * pPar = m_cParSet.GetAt(j);
					if (!pPar) continue;

					CString strPar = pPar->GetAbbrev();
					if (strPar == strT1)
					{
						pVar->m_cDepObj.Add(j);
						bFound = TRUE;
					}
				}
				for (j=0;j<nbVar && !bFound;j++)
				{
					CModelVar * pPar = m_cVarSet.GetAt(j);
					if (!pPar) continue;

					CString strPar = pPar->GetAbbrev();
					if (strPar == strT1)
					{
						pVar->m_cDepObj.Add(nbPar+j);
						bFound = TRUE;
					}
				}


			}
			else continue;
			ErrMsg = mp.Parse( strEq.GetBuffer(strEq.GetLength()), &res );
		}
		for (int j=0;j<pVar->m_cDepObj.GetSize();j++)
		{
			int dwRes= pVar->m_cDepObj.GetAt(j);
			int nRPar=0,nRVar=0;

			CString strRR;

			if (dwRes<nbPar)
				strRR = m_cParSet.GetAt(dwRes)->GetAbbrev();
			else
				strRR = m_cVarSet.GetAt(dwRes-nbPar)->GetAbbrev();


		}
	}


}


CRect CPP_SimModel::DrawFlowChart(CDC* pDC)
{
	int nbVar = m_cVarSet.GetSize();
	int nbPar = m_cParSet.GetSize();

	if (!nbVar && !nbPar) return CRect(0,0,0,0);

	CPen arrPen(PS_SOLID,3,RGB(0,0,255));
	CPen varPen(PS_SOLID,2,RGB(0,0,255));
	CPen arwPen(PS_SOLID,0,RGB(0,0,255));
	CBrush varBrush(RGB(0,0,255));

	CRect rRet(20,80,320,80);

	int nInit=0;

	for (int i=1;i<nbVar;i++)
	{
		CModelVar* pVar = m_cVarSet.GetAt(i);
		//CModelTime *pT= DYNAMIC_DOWNCAST( CModelTime, pVar);
		//if (pT) continue;
		if (pVar->GetInitValue()==-1) break;

		nInit++;

		int nDY = (i-1)*220;

		// Draw Arrow
		CPen *oldP = pDC->SelectObject(&arrPen);
		pDC->MoveTo(20,40+nDY);
		pDC->LineTo(240,40+nDY);
		pDC->SelectObject(oldP);
		POINT polyPt[3] = {{240,40+nDY},{230,33+nDY},{230,47+nDY}};
		oldP = pDC->SelectObject(&arwPen);
		CBrush *oldB = pDC->SelectObject(&varBrush);
		pDC->Polygon(polyPt,3);
		pDC->SelectObject(oldP);
		pDC->SelectObject(oldB);

		// Draw Variable
		CString mstr;
		if (pVar) mstr = pVar->GetName();
		else mstr.Empty();

		rRet |= DrawNode(pDC,CPoint(240,25+nDY),mstr,(i==1)?TT_TOP:TT_BOTTOM,TT_VAR);
		// Draw VarChange
		rRet |= DrawNode(pDC,CPoint(110,30+nDY),_T(""),(i==1)?TT_TOP:TT_BOTTOM,TT_CHANGE);

	}

	int nbParDraw = 0;
	for (i=0;i<nbPar;i++)
	{
		CModelPar* pPar = m_cParSet.GetAt(i);
		CString strPar = pPar->GetName();
		if (pPar->GetInitValue()!=-1) continue;

		int nDY = (nbParDraw++)*60;
		// Draw PreyChange
		rRet |= DrawNode(pDC,CPoint(50,80+nDY),strPar,TT_TOP,TT_PAR);
	}

	// Draw connectors
	for (i=1;i<nbVar;i++)
	{
		CModelVar* pVar = m_cVarSet.GetAt(i);
		//CModelTime *pT= DYNAMIC_DOWNCAST( CModelTime, pVar);
		//if (pT) continue;
		if (pVar->GetInitValue()==-1) break;

		int nbDep = pVar->m_cDepObj.GetSize();
		for (int j=0;j<nbDep;j++)
		{
			int nRes= pVar->m_cDepObj.GetAt(j);

			if (nRes<nbPar)
			{
				// to a parameter
				CModelPar* pPar = m_cParSet.GetAt(nRes);
				if (pPar->GetInitValue()!=-1) continue;

				int nDPY = (nRes-nInit)*60;
				int nDVY = (i-1)*220;

			/*	CPoint mpt[2] = {
						CPoint(50+AGG_SIZE,80+nDPY+AGG_SIZE/2),
						CPoint(110+AGG_SIZE/2,(i==1)?30+nDVY+AGG_SIZE:30+nDVY)};
				rRet |= DrawConnector(pDC,2,mpt,TT_LINE);*/


				CPoint polyPt3[4] = {
						CPoint(50+AGG_SIZE,80+nDPY+AGG_SIZE/2),
						CPoint(50+2*AGG_SIZE,80+nDPY+AGG_SIZE/2),
						CPoint(110+AGG_SIZE/2,(i==1)?30+nDVY+2*AGG_SIZE:30+nDVY-AGG_SIZE),
						CPoint(110+AGG_SIZE/2,(i==1)?30+nDVY+AGG_SIZE:30+nDVY)};
					rRet |= DrawConnector(pDC,4,polyPt3,TT_CURVE);

			}
			else
			{
				// to a variable
				CModelVar* pPar = m_cVarSet.GetAt(nRes-nbPar);
				if (pPar->GetInitValue()==-1) continue;
				int nDV1 = (i-1)*220;
				int nDV2 = (nRes-nbPar-1)*220;

				if (i!=(nRes-nbPar))
				{
					/*CPoint mpt[2] = {
						CPoint(240+AGG_SIZE/2,(i==1)?25+nDV1+VAR_SIZE:25+nDV1),
						CPoint(110+AGG_SIZE/2,(nRes-nbPar==1)?30+nDV2+AGG_SIZE:30+nDV2)};
					rRet |= DrawConnector(pDC,2,mpt,TT_LINE);*/

					CPoint polyPt3[4] = {
						CPoint(240+AGG_SIZE/2,(i==1)?25+nDV1+VAR_SIZE:25+nDV1),
						CPoint(240+AGG_SIZE/2,(i==1)?25+nDV1+3*VAR_SIZE:25+nDV1-3*AGG_SIZE),
						CPoint(110+AGG_SIZE/2,(nRes-nbPar==1)?30+nDV2+3*AGG_SIZE:30+nDV2-3*AGG_SIZE),
						CPoint(110+AGG_SIZE/2,(nRes-nbPar==1)?30+nDV2+AGG_SIZE:30+nDV2)};
					rRet |= DrawConnector(pDC,4,polyPt3,TT_CURVE);
				}
				else
				{
					// Prey -> PreyChange
					CPoint polyPt3[4] = {
						CPoint(240+AGG_SIZE/2,(i==1)?25+nDV1+VAR_SIZE:25+nDV1),
						CPoint(240+AGG_SIZE/2-VAR_SIZE,(i==1)?25+nDV1+2*VAR_SIZE:25+nDV1-VAR_SIZE),
						CPoint(110+AGG_SIZE/2+AGG_SIZE,(nRes-nbPar==1)?30+nDV2+2*AGG_SIZE:30+nDV2-AGG_SIZE),
						CPoint(110+AGG_SIZE/2,(nRes-nbPar==1)?30+nDV2+AGG_SIZE:30+nDV2)};
					rRet |= DrawConnector(pDC,4,polyPt3,TT_CURVE);
				}

			}
		}
	}

	return rRet;
}

