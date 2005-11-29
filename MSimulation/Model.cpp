// Model.cpp: implementation of the CModel class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "simul.h"

#include <MInstruction\LearningUnit.h>
#include "Model.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


#define MYMAX	1.7e+12

//////////////////////////////////////////////////////////////////////
// CMdData Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL( CMdData, CObject, VERSIONABLE_SCHEMA | 1)
	
CMdData::CMdData(CMdEquation *pEqu,int idx)
{
	m_pEquation = pEqu;
	m_dataRef=idx;
}

CMdData::CMdData(const CMdData *md)
{
	m_pEquation = md->m_pEquation;
	m_dataRef = md->m_dataRef;


	for (int i=0;i<md->m_dataMin.GetSize();i++)
	{
		double dd = md->m_dataMin.GetAt(i);
		m_dataMin.Add(dd);
	}
	for (i=0;i<md->m_dataMax.GetSize();i++)
	{
		m_dataMax.Add(md->m_dataMax.GetAt(i));
	}
	for (i=0;i<md->m_Values.GetSize();i++)
	{
		CxValueSet* pVSet = md->m_Values.GetAt(i);
		if (!pVSet) continue;

		CxValueSet* pNVSet= new CxValueSet();

		for (int k=0;k<pVSet->GetSize();k++)
		{
			double dd = pVSet->GetAt(i);
			pNVSet->Add(dd);
		}
		m_Values.Add(pNVSet);

	}
}


CMdData::~CMdData()
{
/*	int i = 0;
	while (i < m_Values.GetSize() )
	{
		delete m_Values.GetAt( i++ );
	}
	m_Values.RemoveAll();*/
	ClearAll();
}

CMdData* CMdData::Clone()
{
	CMdData*	pER = new CMdData(this);

	return pER;
}

void CMdData::Serialize( CArchive& ar )
{
	if( ar.IsStoring( ) )
	{
	}
	else
	{
	}
}

//////////////////////////////////////////////////////////////////////
/// Clear all the generated values from the data-points and the local minima/mixima.
//////////////////////////////////////////////////////////////////////
void CMdData::ClearAll() 
{
	int i = 0;
	while (i < m_Values.GetSize() )
	{
		delete m_Values.GetAt( i++ );
	}
	m_Values.RemoveAll();
	m_dataMin.RemoveAll();
	m_dataMax.RemoveAll();
}

//////////////////////////////////////////////////////////////////////
/// Return the number of Experimental Set in the data-points.
//////////////////////////////////////////////////////////////////////
int	CMdData::GetExpSetNb() 
{ 
	return m_Values.GetSize();
}


//////////////////////////////////////////////////////////////////////
/// Return the value generated at a given time-step and from a given Experimental Set.
/// \param idx Time index at which the data will be retrieved.
/// \param exps Zero-based index of the Experimental Set.
/// \return the value (double) stored at the specified location in the data-points list.
//////////////////////////////////////////////////////////////////////
double	CMdData::GetAt(int idx,int exps)
{
	if (exps >= m_Values.GetSize())
		return 0.0;
	CxValueSet *pVSet = m_Values.GetAt(exps);
	if (!pVSet || idx >= pVSet->GetSize())
		return 0.0;

	return pVSet->GetAt(idx);
}

//////////////////////////////////////////////////////////////////////
/// Add a value at a given time and in a given Experimental Set.
/// \param dt The value (double) to add at the specified location.
/// \param idx Time index at which the data will be added.
/// \param exps Zero-based index of the Experimental Set.
/// \return TRUE if the insertion is succesful, FALSE otherwise
///
/// The function adds the given value at the specified location in the CMdData::m_Values
/// attribute and updates the list of minimum and maximum values.
//////////////////////////////////////////////////////////////////////
int	CMdData::SetAt(double dt,int idx,int exps)
{
	CxValueSet *pVSet = 0;
	
	if (exps >= m_Values.GetSize())
	{
		pVSet= new CxValueSet();
		m_Values.Add(pVSet);
	} 

	pVSet = m_Values.GetAt(exps);
	ASSERT(pVSet);
	int nb = pVSet->GetUpperBound();
	if (idx > nb)
	{
		int nn = idx - nb;
		pVSet->InsertAt(nb+1,dt,nn);
	}
	else
		pVSet->SetAt(idx,dt);

	double lmin,lmax;
	if (exps >= m_dataMin.GetSize())
		lmin = MYMAX;
	else lmin = m_dataMin.GetAt(exps);

	if (exps >= m_dataMax.GetSize())
		lmax = -MYMAX;
	else lmax = m_dataMax.GetAt(exps);

	if (dt > lmax) m_dataMax.SetAtGrow(exps,dt);
	if (dt < lmin) m_dataMin.SetAtGrow(exps,dt);
	return true;
}

//////////////////////////////////////////////////////////////////////
/// Return the Mathematical Model associated with these data-points.
/// \return A pointer to the CMdEquation used to compute these data-points
//////////////////////////////////////////////////////////////////////
CMdEquation* CMdData::GetEquation() 
{ 
	return m_pEquation;
}

//////////////////////////////////////////////////////////////////////
/// Return the entity associated with these data-points.
/// \return A pointer to the CModelObj associated with these data-points.
///
/// This function is reimplemented in the two derived class, see  CMdParData::GetDataInfo()
/// and CMdVarData::GetDataInfo().
//////////////////////////////////////////////////////////////////////
CModelObj* CMdData::GetDataInfo()
{
	return NULL;
}

//////////////////////////////////////////////////////////////////////
/// Return the index of the entity associated with these data-points.
/// \return Zero-based index of the entity in the parameter or variable list.
//////////////////////////////////////////////////////////////////////
int	CMdData::GetDataRef()
{
	return m_dataRef;
}

//////////////////////////////////////////////////////////////////////
/// Return the name of the entity associated with these data-points.
/// \param bAbbrev TRUE to retrieve the abbreviation of the entity, FALSE to retrieve the long name.
///
/// This function uses the CMdData::GetDataInfo() function to retrieve a pointer to the 
/// entity corresponding to these data-points and then calls CModelObj::GetFormatName() to 
/// retrieve its name (or abbreviation).
//////////////////////////////////////////////////////////////////////
CString CMdData::GetDataName(BOOL bAbbrev)
{
	CString mstr(_T(""));

	CModelObj *pObj = GetDataInfo();
	if (pObj)
		mstr = pObj->GetFormatName(bAbbrev);
	return mstr;
}

//////////////////////////////////////////////////////////////////////
/// Reset and recalculate the minima/maxima of the data-points. 
/// \param exps Zero-based index of the Experimental Set to update.
///
///
//////////////////////////////////////////////////////////////////////
void CMdData::ResetMinMax(int exps)
{
	CxValueSet *pVSet = m_Values.GetAt(exps);
	if (!pVSet) return;
	double lmin,lmax;
	lmin = MYMAX;
	lmax = -MYMAX;

	double llmax = m_dataMax.GetAt(exps);
	double llmin = m_dataMin.GetAt(exps);
	for( int i = 0; i < pVSet->GetSize();i++ )
	{
		double dt = pVSet->GetAt(i);
		if (dt > lmax) 
		{
			m_dataMax.SetAtGrow(exps,dt);
			lmax = dt;
		}
		if (dt < lmin) 
		{
			m_dataMin.SetAtGrow(exps,dt);
			lmin = dt;
		}
	}

}

//////////////////////////////////////////////////////////////////////
/// Retrieve the minimum and maximum of the data-points from a given Experimental Set. 
/// \param pMin Pointer to the variable to update with the minimum value.
/// \param pMax Pointer to the variable to update with the maximum value.
/// \param exps Zero-based index of the Experimental Set.
///
//////////////////////////////////////////////////////////////////////
bool CMdData::GetMinMax(double *pMin, double *pMax,int exps)
{	
	int nb = m_dataMin.GetSize();
	*pMin = m_dataMin.GetAt(exps); 
	*pMax = m_dataMax.GetAt(exps);
	return true;
}

//////////////////////////////////////////////////////////////////////
/// Return the lists of the local minima/maxima of the data-points from a given Experimental Set.
/// \param minList List of minimum values to update.
/// \param maxList List of maximum values to update.
/// \param exps Zero-based index of the Experimental Set.
//////////////////////////////////////////////////////////////////////
void CMdData::GetLocalMinMax(CxIntList &minList,CxIntList &maxList,int exps) 
{
	CxValueSet *pVSet = 0;
	pVSet = m_Values.GetAt(exps);

	bool goUp = true;
	double start = pVSet->GetAt(0);

	for( int i = 1; i < pVSet->GetSize();i++ )
	{
	    double current = pVSet->GetAt(i);
		if (goUp)
		{
			if (current >= start)
			{
			}
			else
			{
				maxList.Add(i);
				goUp=!goUp;
			}
			start = current;
		}
		else
		{
			if (current <= start)
			{
			}
			else
			{
				minList.Add(i);
				goUp=!goUp;
			}
			start = current;
		}
	}
	
}

//////////////////////////////////////////////////////////////////////
// CMdVarData Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL( CMdVarData, CMdData, VERSIONABLE_SCHEMA | 1)
	
CMdVarData::CMdVarData(CMdEquation *pEqu,int idx) : CMdData(pEqu,idx)
{
}

CMdData* CMdVarData::Clone()
{
	CMdVarData*	pLU = new CMdVarData(this);

	return pLU;
}

//////////////////////////////////////////////////////////////////////
/// Return the entity associated with these data-points.
///
/// This function, a reimplementation of CMdData::GetDataInfo(), return the entity
/// associated with these data-points by looking in the list of Variables in the 
/// Mathematical Model (see CMdEquation::m_cVarSet).
//////////////////////////////////////////////////////////////////////
CModelObj*	CMdVarData::GetDataInfo()
{
	CModelVar* pVar = m_pEquation->m_cVarSet.GetAt(m_dataRef);
	return pVar;

}

//////////////////////////////////////////////////////////////////////
// CMdParData Construction/Destruction
//////////////////////////////////////////////////////////////////////

//IMPLEMENT_SERIAL( CMdParData, CMdData, VERSIONABLE_SCHEMA | 1)
IMPLEMENT_SERIAL( CMdParData, CMdData, VERSIONABLE_SCHEMA | 1)
	
//CMdParData::CMdParData(CMdEquation *pEqu,int idx) : CMdData(pEqu,idx)
CMdParData::CMdParData(CMdEquation *pEqu,int idx) : CMdData(pEqu,idx)
{
}

CMdData* CMdParData::Clone()
{
	CMdParData*	pLU = new CMdParData(this);

	return pLU;
}

//////////////////////////////////////////////////////////////////////
/// Return the entity associated with these data-points.
///
/// This function, a reimplementation of CMdData::GetDataInfo(), return the entity
/// associated with these data-points by looking in the list of Parameters in the 
/// Mathematical Model (see CMdEquation::m_cParSet).
//////////////////////////////////////////////////////////////////////
CModelObj*	CMdParData::GetDataInfo()
{
	CModelPar* pVar = m_pEquation->m_cParSet.GetAt(m_dataRef);
	return pVar;

}

/*CMdVarData::CMdVarData(const CMdVarData *md) : CMdData(md)
{
	for (int i=0;i<md->m_Values.GetSize();i++)
	{
		CxValueSet* pVSet = md->m_Values.GetAt(i);
		if (!pVSet) continue;

		CxValueSet* pNVSet= new CxValueSet();

		for (int k=0;k<pVSet->GetSize();k++)
		{
			double dd = pVSet->GetAt(i);
			pNVSet->Add(dd);
		}
		m_Values.Add(pNVSet);

	}
}

CMdVarData::~CMdVarData()
{
	int i = 0;
	while (i < m_Values.GetSize() )
	{
		delete m_Values.GetAt( i++ );
	}
	m_Values.RemoveAll();
}*/
/*

void CMdVarData::Serialize( CArchive& ar )
{
	if( ar.IsStoring( ) )
	{
	}
	else
	{
	}
}

CString CMdVarData::GetDataName(BOOL bAbbrev)
{
	CModelVar* pVar = m_pEquation->m_cVarSet.GetAt(m_dataRef);
	CString mstr = pVar->GetFormatName(bAbbrev);
	return mstr;
}*/

/*
void CMdVarData::ClearAll()
{
	int i = 0;
	while (i < m_Values.GetSize() )
	{
		delete m_Values.GetAt( i++ );
	}
	m_Values.RemoveAll();
	m_dataMin.RemoveAll();
	m_dataMax.RemoveAll();
}

int	CMdVarData::GetExpSetNb()
{
	return m_Values.GetSize();
}

double	CMdVarData::GetAt(int idx,int exps)
{
	if (exps >= m_Values.GetSize())
		return 0.0;
	CxValueSet *pVSet = m_Values.GetAt(exps);
	if (!pVSet || idx >= pVSet->GetSize())
		return 0.0;

	return pVSet->GetAt(idx);
}

int	CMdVarData::SetAt(double dt,int idx,int exps)
{
	CxValueSet *pVSet = 0;
	
	if (exps >= m_Values.GetSize())
	{
		pVSet= new CxValueSet();
		m_Values.Add(pVSet);
	} 

	pVSet = m_Values.GetAt(exps);
	ASSERT(pVSet);
	int nb = pVSet->GetUpperBound();
	if (idx > nb)
	{
		int nn = idx - nb;
		pVSet->InsertAt(nb+1,dt,nn);
	}
	else
		pVSet->SetAt(idx,dt);

	double lmin,lmax;
	if (exps >= m_dataMin.GetSize())
		lmin = MYMAX;
	else lmin = m_dataMin.GetAt(exps);

	if (exps >= m_dataMax.GetSize())
		lmax = -MYMAX;
	else lmax = m_dataMax.GetAt(exps);

	if (dt > lmax) m_dataMax.SetAtGrow(exps,dt);
	if (dt < lmin) m_dataMin.SetAtGrow(exps,dt);
	return true;
}

void CMdVarData::ResetMinMax(int exps)
{
	CxValueSet *pVSet = m_Values.GetAt(exps);
	if (!pVSet) return;
	double lmin,lmax;
	lmin = MYMAX;
	lmax = -MYMAX;

	double llmax = m_dataMax.GetAt(exps);
	double llmin = m_dataMin.GetAt(exps);
	for( int i = 0; i < pVSet->GetSize();i++ )
	{
		double dt = pVSet->GetAt(i);
		if (dt > lmax) 
		{
			m_dataMax.SetAtGrow(exps,dt);
			lmax = dt;
		}
		if (dt < lmin) 
		{
			m_dataMin.SetAtGrow(exps,dt);
			lmin = dt;
		}
	}
}

void CMdVarData::GetLocalMinMax(CxIntList &minList,CxIntList &maxList,int exps)
{
	CxValueSet *pVSet = 0;
	pVSet = m_Values.GetAt(exps);

	bool goUp = true;
	double start = pVSet->GetAt(0);

	for( int i = 1; i < pVSet->GetSize();i++ )
	{
	    double current = pVSet->GetAt(i);
		if (goUp)
		{
			if (current >= start)
			{
			}
			else
			{
				maxList.Add(i);
				goUp=!goUp;
			}
			start = current;
		}
		else
		{
			if (current <= start)
			{
			}
			else
			{
				minList.Add(i);
				goUp=!goUp;
			}
			start = current;
		}
	}


}

*/

/*CMdParData::CMdParData(const CMdParData *md) : CMdVarData(md)
{

}


CMdParData::~CMdParData()
{
//	m_Values.RemoveAll();
}*/

/*

void CMdParData::Serialize( CArchive& ar )
{
	if( ar.IsStoring( ) )
	{
	}
	else
	{
	}
}


CString CMdParData::GetDataName(BOOL bAbbrev)
{
	CModelPar* pPar = m_pEquation->m_cParSet.GetAt(m_dataRef);
	CString mstr = pPar->GetFormatName(bAbbrev);
	return mstr;
}
*/

/*void CMdParData::ClearAll()
{
	CMdVarData::ClearAll();
	//m_Values.RemoveAll();
}

int	CMdParData::GetExpSetNb()
{
	return m_Values.GetSize();
}*/

//double	CMdParData::GetAt(int idx,int exps)
//{
//	return CMdVarData::GetAt(idx,exps);
/*	if (exps >= m_Values.GetSize())
		return 0.0;
	else 
		return m_Values.GetAt(exps);*/
//}

//int	CMdParData::SetAt(double dt,int idx,int exps)
//{
//	return CMdVarData::SetAt(dt,idx,exps);
/*	int nb = m_Values.GetUpperBound();
	if (exps > nb)
	{
		int nn = exps - nb;
		m_Values.InsertAt(nb+1,dt,nn);
	}
	else
		m_Values.SetAt(exps,dt);


	double lmin,lmax;
	if (exps >= m_dataMin.GetSize())
		lmin = MYMAX;
	else lmin = m_dataMin.GetAt(exps);

	if (exps >= m_dataMax.GetSize())
		lmax = -MYMAX;
	else lmax = m_dataMax.GetAt(exps);

	if (dt > lmax) m_dataMax.SetAtGrow(exps,dt);
	if (dt < lmin) m_dataMin.SetAtGrow(exps,dt);

	return true;*/
//}

//void CMdParData::ResetMinMax(int exps)
//{
//	CMdVarData::ResetMinMax(exps);
//}


//void CMdParData::GetLocalMinMax(CIntList &minList,CIntList &maxList,int exps)
//{
	//CMdVarData::GetLocalMinMax(minList,maxList,exps);
/*	bool goUp = true;
	double start = m_Values.GetAt(exps);
	//maxList.Add(i);
	//minList.Add(i);*/
//}



//////////////////////////////////////////////////////////////////////
// CMVariable Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL( CModelObj, CObject, VERSIONABLE_SCHEMA | 1)
	
CModelObj::CModelObj()
{
	m_sName = "";
	m_sDesc = "";
	m_sUnit = "";
	m_crColor = RGB(0,0,255);
	m_nIcon = -1;		// No Icon in Table

}

CModelObj::CModelObj(LPCTSTR name,LPCTSTR desc,LPCTSTR unit)
{
	m_sName = name;
	m_sDesc = desc;
	m_sUnit = unit;
	m_crColor = RGB(0,0,255);
	m_nIcon = -1;		// No Icon in Table
}

CModelObj::CModelObj(const CModelObj* pObj)
{
	m_pProp = NULL;
	m_sName = pObj->m_sName;
	m_sAbbrev = pObj->m_sAbbrev;
	m_sDesc  = pObj->m_sDesc;
	m_sDef = pObj->m_sDef;
	m_sUnit = pObj->m_sUnit;
	m_crColor = pObj->m_crColor;
	m_nIcon = pObj->m_nIcon;

}


CModelObj* CModelObj::Clone()
{
	return new CModelObj(this);
}


CModelObj::~CModelObj()
{
}

//////////////////////////////////////////////////////////////////////
/// Return the formatted name of the entity: the abbreviation (m_sAbbrev) 
/// if bLabel is TRUE, its name (m_sName) otherwise.
//////////////////////////////////////////////////////////////////////
CString CModelObj::GetFormatName(BOOL bLabel)
{
	if (bLabel)
		return GetAbbrev();
	else
		return GetName();
}


void CModelObj::Serialize( CArchive& ar )
{
	CObject::Serialize(ar);
	if( ar.IsStoring( ) )
	{
		ar << m_sName;
		ar << m_sAbbrev;
		ar << m_sDesc;
		ar << m_sDef;
		ar << m_sUnit;
		ar << m_crColor;
		ar << m_nIcon;
	}
	else
	{
		ar >> m_sName;
		ar >> m_sAbbrev;
		ar >> m_sDesc;
		ar >> m_sDef;
		ar >> m_sUnit;
		ar >> m_crColor;
		ar >> m_nIcon;

	}
}


//////////////////////////////////////////////////////////////////////
// CMVariable Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL( CModelVar, CModelObj, VERSIONABLE_SCHEMA | 1)
	
CModelVar::CModelVar() : CModelObj()
{
	m_InitValue = -1;
	m_strEquation = _T("");
	m_nIcon = 3;		// 'A' Icon in Table
}

CModelVar::CModelVar(LPCTSTR  name,LPCTSTR  desc,LPCTSTR  unit) :
	CModelObj(name,desc,unit)
{
	m_InitValue = -1;
	m_strEquation = _T("");
	m_nIcon = 3;		// 'A' Icon in Table
}

CModelVar::CModelVar(const CModelVar* pObj) : CModelObj(pObj)
{
	m_InitValue = pObj->m_InitValue;
	m_strEquation = pObj->m_strEquation;
}


CModelObj* CModelVar::Clone()
{
	return new CModelVar(this);
}



CModelVar::~CModelVar()
{
}

void CModelVar::SetInitValue(int iv)
{
	m_InitValue = iv;
	m_nIcon = 0;		// 'V' Icon in Table
}

void CModelVar::Serialize( CArchive& ar )
{
	CModelObj::Serialize( ar );
	if( ar.IsStoring( ) )
	{
		ar << m_InitValue;
	}
	else
	{
		ar >> m_InitValue;
	}
}

//////////////////////////////////////////////////////////////////////
// CMVariable Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL( CModelTime, CModelVar, VERSIONABLE_SCHEMA | 1)
	
CModelTime::CModelTime() : CModelVar()
{
	m_nIcon = 4;		// 'P' Icon in Table
}

CModelTime::CModelTime(LPCTSTR  name,LPCTSTR  desc,LPCTSTR  unit) :
	CModelVar(name,desc,unit)
{
	m_nIcon = 4;		// 'P' Icon in Table
}

CModelTime::~CModelTime()
{
}

void CModelTime::Serialize( CArchive& ar )
{
	CModelVar::Serialize( ar );
	if( ar.IsStoring( ) )
	{
		//ar << m_InitValue;
	}
	else
	{
		//ar >> m_InitValue;
	}
}

//////////////////////////////////////////////////////////////////////
// CMParameters Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL( CModelPar, CModelObj, VERSIONABLE_SCHEMA | 1)
	
CModelPar::CModelPar() : CModelObj()
{
	m_Value = -1;
	m_lMin = -1;
	m_lMax = -1;
	m_InitValue = -1;
	m_nIcon = 1;		// 'P' Icon in Table
}

CModelPar::CModelPar(LPCTSTR name,double mv,LPCTSTR desc,LPCTSTR unit) :
	CModelObj(name,desc,unit)
{
	m_Value = mv;
	m_lMin = -1;
	m_lMax = -1;
	m_InitValue = -1;
	m_nIcon = 1;		// 'P' Icon in Table
}

CModelPar::CModelPar(const CModelPar* pObj) : CModelObj(pObj)
{
	m_InitValue = pObj->m_InitValue;
	m_lMin = pObj->m_lMin;
	m_lMax = pObj->m_lMax;
	m_Value = pObj->m_Value;
}


CModelObj* CModelPar::Clone()
{
	return new CModelPar(this);
}


CModelPar::~CModelPar()
{

}

void CModelPar::SetInitValue(int iv)
{
	m_InitValue = iv;
	m_nIcon = 2;		// 'I' Icon in Table
}

void CModelPar::SetInfos(double v,double mn, double mx)
{
	m_Value = v;
	m_lMin = mn;
	m_lMax = mx;
};

void CModelPar::Serialize( CArchive& ar )
{
	CModelObj::Serialize( ar );
	if( ar.IsStoring( ) )
	{
		ar << m_Value;
		ar << m_lMin;
		ar << m_lMax;
		ar << m_InitValue;
	}
	else
	{
		ar >> m_Value;
		ar >> m_lMin;
		ar >> m_lMax;
		ar >> m_InitValue;
	}
}

//////////////////////////////////////////////////////////////////////
// CMVariable Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL( CMdEquation, CObject, VERSIONABLE_SCHEMA | 1)
	
CMdEquation::CMdEquation()
{
//	m_sEquation = _T("");
	m_nEquNb = 0;
}

CMdEquation::~CMdEquation()
{
	for(int i = 0; i < m_cVarSet.GetSize();i++ )
		delete m_cVarSet.GetAt(i);
	m_cVarSet.RemoveAll();
	for (i = 0; i < m_cParSet.GetSize();i++ )
		delete m_cParSet.GetAt(i);
	m_cParSet.RemoveAll();
}

CMdEquation::CMdEquation(CMdEquation *pEqu)
{
	m_nEquNb = pEqu->m_nEquNb ;
	m_sName = pEqu->m_sName;
	m_sText = pEqu->m_sText;
//	m_sEquation = pEqu->m_sEquation;

	m_cVarSet.RemoveAll();
	for(int i = 0; i < pEqu->m_cVarSet.GetSize();i++ )
	{
		CModelVar *pVar = pEqu->m_cVarSet.GetAt(i);
		if (!pVar) continue;
		m_cVarSet.Add((CModelVar*)pVar->Clone());
	}
	m_cParSet.RemoveAll();
	for (i = 0; i < pEqu->m_cParSet.GetSize();i++ )
	{
		CModelPar *pPar = pEqu->m_cParSet.GetAt(i);
		if (!pPar) continue;

		CModelObj *pObj = pPar->Clone();
		m_cParSet.Add((CModelPar*)pObj);
	}
	for (i=0;i<pEqu->m_sEqu.GetSize();i++)
	{
		m_sEqu.Add(pEqu->m_sEqu.GetAt(i));
	}
	for (i=0;i<pEqu->m_sEquDef.GetSize();i++)
	{
		m_sEquDef.Add(pEqu->m_sEquDef.GetAt(i));
	}
}

//////////////////////////////////////////////////////////////////////
/// Create a carbon-copy of the class.
//////////////////////////////////////////////////////////////////////
CMdEquation *CMdEquation::Clone()
{
	CMdEquation *pEqu = new CMdEquation(this);
	return pEqu;
}


void CMdEquation::Serialize( CArchive& ar )
{
	CObject::Serialize(ar);
	if( ar.IsStoring( ) )
	{
		ar << m_sName;
		ar << m_sText;
		ar << m_nEquNb;
	}
	else
	{
		ar >> m_sName;
		ar >> m_sText;
		ar >> m_nEquNb;
	}
	m_sEqu.Serialize(ar);
	m_sEquDef.Serialize(ar);
	m_cVarSet.Serialize(ar);
	m_cParSet.Serialize(ar);
}


//////////////////////////////////////////////////////////////////////
/// Return a text-version of the equations used to define the simulation model.
//////////////////////////////////////////////////////////////////////
CString CMdEquation::GetFormatedText(int nLevel)
{
	return m_sText;
}

//////////////////////////////////////////////////////////////////////
/// Return a text-version of the equations used to define the simulation model.
//////////////////////////////////////////////////////////////////////
CString CMdEquation::GetFormatedEquation()
{
	return m_sText;
}


//////////////////////////////////////////////////////////////////////
/// Called when this equation is added to a Simulation Model.
//////////////////////////////////////////////////////////////////////
void CMdEquation::OnNewEquation(CSimModel *pModel)
{
	//HGDIOBJ pObj = m_bFunctDiag.GetSafeHandle();
}


//////////////////////////////////////////////////////////////////////
/// Called.by the framework after the model has been modified.
///
/// \param m_pLU a pointer to the Learning Unit using this model.
///	\param bInit TRUE if this function is called for an initial update, FALSE otherwise.
///	\see OnUpdate(CLearningUnit*,int,BOOL)
///	\see OnUpdateFrom(CLearningUnit*,int,BOOL)
///	\see OnUpdateFrom(CLearningUnit*,int,int,BOOL)
///
/// Call the OnUpdate(CLearningUnit*,int,BOOL) function in order to recalculate
/// all the experimental sets.
//////////////////////////////////////////////////////////////////////
void CMdEquation::OnUpdate(CLearningUnit *m_pLU,BOOL bInit/*=FALSE*/)
{
	if (!m_pLU) return;

	int nbexpset = m_pLU->m_cExpSet.GetSize(),
		nbdata = m_pLU->m_cDataPoints.GetSize(); //m_cValues.GetSize();

	int nbPar = m_cParSet.GetSize();
	int nbVar = m_cVarSet.GetSize();

	if (!nbdata)
	{
		// Add parameters
		for (int i=0;i<m_cParSet.GetSize();i++)
			m_pLU->m_cDataPoints.Add(new CMdParData(this,i));
			//m_pLU->m_cDataPoints.Add(new CMdVarData(this,i));
		for (i=0;i<m_cVarSet.GetSize();i++)
			m_pLU->m_cDataPoints.Add(new CMdVarData(this,i));
		nbdata = m_pLU->m_cDataPoints.GetSize();
	}
	for (int i=0;i<nbdata;i++)
	{
		CMdData *pData = m_pLU->m_cDataPoints.GetAt(i);
		CString mstr = pData->GetDataName(m_pLU->m_bShowAbbrev);
		pData->ClearAll();
	}
	for (i=0;i<nbexpset;i++)
	{
		OnUpdate(m_pLU,i,bInit);
	}
}


//////////////////////////////////////////////////////////////////////
/// Called.by the framework after the model has been modified.
///
/// \param m_pLU a pointer to the Learning Unit using this model.
/// \param nExpSet the index of the Experimental Set to update.
///	\param bInit TRUE if this function is called for an initial update, FALSE otherwise.
///	\see OnUpdate(CLearningUnit*,BOOL)
///	\see OnUpdateFrom(CLearningUnit*,int,BOOL)
///	\see OnUpdateFrom(CLearningUnit*,int,int,BOOL)
///
/// Call the OnUpdateFrom(CLearningUnit*,int,int,BOOL) function in order to recalculate
/// all the data of the given experimental set, .
//////////////////////////////////////////////////////////////////////
void CMdEquation::OnUpdate(CLearningUnit *m_pLU,int nExpSet,BOOL bInit/*=FALSE*/)
{
	OnUpdateFrom(m_pLU,nExpSet,0,bInit);
}

//////////////////////////////////////////////////////////////////////
/// Called.by the framework after the model has been modified.
///
/// \param m_pLU a pointer to the Learning Unit using this model.
/// \param nTime the time (i.e. data-point) from which to update the data of every experimental sets.
///	\param bInit TRUE if this function is called for an initial update, FALSE otherwise.
///	\see OnUpdate(CLearningUnit*,BOOL)
///	\see OnUpdate(CLearningUnit*,int,BOOL)
///	\see OnUpdateFrom(CLearningUnit*,int,int,BOOL)
///
/// Call the OnUpdateFrom(CLearningUnit*,int,int,BOOL) function in order to recalculate
/// all the experimental sets from the given data-point onward.
//////////////////////////////////////////////////////////////////////
void CMdEquation::OnUpdateFrom(CLearningUnit *m_pLU,int nTime,BOOL bInit/*=FALSE*/)
{
	if (!m_pLU) return;

	int nbexpset = m_pLU->m_cExpSet.GetSize(),
		nbdata = m_pLU->m_cDataPoints.GetSize(); //m_cValues.GetSize();

	if (!nbdata)
	{
		// Add parameters
		for (int i=0;i<m_cParSet.GetSize();i++)
			m_pLU->m_cDataPoints.Add(new CMdParData(this,i));
			//m_pLU->m_cDataPoints.Add(new CMdVarData(this,i));
		for (i=0;i<m_cVarSet.GetSize();i++)
			m_pLU->m_cDataPoints.Add(new CMdVarData(this,i));
		nbdata = m_pLU->m_cDataPoints.GetSize();
	}
	for (int i=0;i<nbdata;i++)
	{
		CMdData *pData = m_pLU->m_cDataPoints.GetAt(i);
		CString mstr = pData->GetDataName(m_pLU->m_bShowAbbrev);
		pData->ClearAll();
	}
	for (i=0;i<nbexpset;i++)
	{
		OnUpdateFrom(m_pLU,i,nTime,bInit);
	}
}

//////////////////////////////////////////////////////////////////////
/// Called.by the framework after the model has been modified.
///
/// \param m_pLU a pointer to the Learning Unit using this model.
/// \param nExpSet the index of the Experimental Set to update.
/// \param nTime the time (i.e. data-point) from which to update the data.
///	\param bInit TRUE if this function is called for an initial update, FALSE otherwise.
///	\see OnUpdate(CLearningUnit*,BOOL)
///	\see OnUpdate(CLearningUnit*,int,BOOL)
///	\see OnUpdateFrom(CLearningUnit*,int,BOOL)
///
/// This function recalculates the data only of the given experimental set and from 
/// the given data-point onward. Basically, it uses the mathematical model (given 
/// by m_sText) to generate all the data, based on the entities (CMdEquation::m_cParSet 
/// and CMdEquation::m_cVarSet) and the experimental sets (CLearningUnit::m_cExpSet). 
/// These values are stored in the Learning Unit (see CLearningUnit::m_cDataPoints).
//////////////////////////////////////////////////////////////////////
void CMdEquation::OnUpdateFrom(CLearningUnit *m_pLU,int nExpSet,int nTime,BOOL bInit/*=FALSE*/)
{
}

//////////////////////////////////////////////////////////////////////
/// Create a new Experimental Set, based on the initial conditions of the mathematical model.
///	\return	A pointer to a new instance of CMdExpertSet.
///
/// This function uses the initial conditions of the mathematical model, i.e. the default values
/// of the parameters and initial values (see CModelPar::GetValue()) to instanciate a new CMdExpertSet
/// instance.
//////////////////////////////////////////////////////////////////////
CMdExpertSet* CMdEquation::NewExpertSet()
{
	CMdExpertSet *pSet = new CMdExpertSet();

	for (int i=0;i<m_cParSet.GetSize();i++)
	{
		CModelPar *pPar= m_cParSet.GetAt(i);
		double val = pPar->GetValue();
		pSet->AddValue(val);
	}
	return pSet;
}

//////////////////////////////////////////////////////////////////////
/// Draw the flow chart corresponding to the simulation model.
//////////////////////////////////////////////////////////////////////
CRect CMdEquation::DrawFlowChart(CDC* pDC)
{
	return CRect(0,0,0,0);
}

//////////////////////////////////////////////////////////////////////
/// Draw the connector between two boxes in the flow chart.
//////////////////////////////////////////////////////////////////////
CRect CMdEquation::DrawConnector(CDC* pDC,int nb,CPoint ptStart[],int nType)
{
	CBrush conBrush(RGB(0,0,0));
	CRect rRet;

	if (nType==TT_LINE)
	{
		ASSERT(nb==2);
		pDC->MoveTo(ptStart[0]);
		pDC->LineTo(ptStart[1]);
		CRect mrect(ptStart[1],ptStart[1]);
		mrect.InflateRect(4,4,3,3);
		CBrush *oldB = pDC->SelectObject(&conBrush);
		pDC->Ellipse(mrect);
		pDC->SelectObject(oldB);
		rRet = CRect(ptStart[0],ptStart[1]);
		rRet.NormalizeRect();
	}
	else if (nType==TT_CURVE)
	{
		ASSERT(nb==4);
		//POINT polyPt2[4] = {{340,110},{340,140},{215,197},{195,197}};
		pDC->PolyBezier(ptStart,nb);
		//pDC->MoveTo(ptStart[3]);
		CRect mrect(ptStart[3],ptStart[3]);
		mrect.InflateRect(4,4,3,3);
		CBrush *oldB = pDC->SelectObject(&conBrush);
		pDC->Ellipse(mrect);
		pDC->SelectObject(oldB);
		rRet = CRect(ptStart[0],ptStart[3]);
		rRet.NormalizeRect();
	}
	return rRet;
}


//////////////////////////////////////////////////////////////////////
/// Draw a node (corresponding to one of the aggregate of the model) in the flow chart.
//////////////////////////////////////////////////////////////////////
CRect CMdEquation::DrawNode(CDC* pDC,CPoint mpt,CString nName,int bTop,int nType)
{
	CPen varPen(PS_SOLID,2,RGB(0,0,255));
	CPen aggPen(PS_SOLID,1,RGB(255,0,0));
	CPen parPen(PS_SOLID,1,RGB(128,0,128));

	CBrush varBrush(RGB(0,0,255));
	CBrush aggBrush(RGB(255,0,0));
	CBrush parBrush(RGB(128,0,128));

	CRect rText;
	CString mstr;
	CRect rRet;
	CRect rloc(mpt,mpt);

	CFont m_font;
	int nbf = m_font.CreatePointFont (80, _T ("MS Sans Serif"));
	CFont *oldF = pDC->SelectObject(&m_font);

	int oldM = pDC->SetBkMode(TRANSPARENT);

	CPen *oldP = NULL;
	CBrush *oldB = NULL;

	if (nType==TT_VAR)
	{
		oldP = pDC->SelectObject(&varPen);
	}
	else if (nType==TT_PAR)
	{
		oldP = pDC->SelectObject(&parPen);
		oldB = pDC->SelectObject(&parBrush);
	}
	else if (nType==TT_AGG)
	{
		oldP = pDC->SelectObject(&aggPen);
		oldB = pDC->SelectObject(&aggBrush);
	}
	else if (nType==TT_CHANGE)
	{
		oldP = pDC->SelectObject(&varPen);
	}

	if (nType==TT_VAR)
	{
		rloc.InflateRect(0,0,VAR_SIZE,VAR_SIZE);
		pDC->Rectangle(rloc);
	}
	else
	{
		rloc.InflateRect(0,0,AGG_SIZE,AGG_SIZE);
		pDC->Ellipse(rloc);
	}

	if (oldP) pDC->SelectObject(oldP);
	if (oldB) pDC->SelectObject(oldB);

	if (bTop==TT_TOP)
	{
		rText = CRect(rloc.left,rloc.top-15,rloc.right,rloc.top-5);
	}
	else
	{
		rText = CRect(rloc.left,rloc.bottom+5,rloc.right,rloc.bottom+15);
	}

	pDC->DrawText(nName,rText,DT_CENTER|DT_SINGLELINE|DT_NOCLIP);

	pDC->SetBkMode(oldM);
	pDC->SelectObject(oldF);

	rRet = rloc | rText;

	return rRet;
}



//////////////////////////////////////////////////////////////////////
// CMVariable Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL( CMdExpertSet, CObject, VERSIONABLE_SCHEMA | 1)
	
CMdExpertSet::CMdExpertSet()
{
}

CMdExpertSet* CMdExpertSet::Clone()
{
	CMdExpertSet*	pLU = new CMdExpertSet();

	pLU->m_sName = m_sName;
	for (int i=0;i<m_cParamSet.GetSize();i++)
	{
		double bb = m_cParamSet.GetAt(i);
		pLU->m_cParamSet.Add(bb);
	}
	return pLU;
}


int CMdExpertSet::AddValue(double val)
{
	m_cParamSet.Add(val);
	return 1;
}

double CMdExpertSet::GetValue(int idx)
{
	return m_cParamSet.GetAt(idx);
}

void CMdExpertSet::SetValue(int idx,double val)
{
	m_cParamSet.SetAt(idx,val);
}


CMdExpertSet::~CMdExpertSet()
{
}

void CMdExpertSet::Serialize( CArchive& ar )
{
	CObject::Serialize(ar);
	if( ar.IsStoring( ) )
	{
		ar << m_sName;
	}
	else
	{
		ar >> m_sName;
	}
	m_cParamSet.Serialize(ar);
}


//////////////////////////////////////////////////////////////////////
// CSimModel Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL( CSimModel, CObject, VERSIONABLE_SCHEMA | 1)
	
CSimModel::CSimModel()
{
	m_iMaxTime =0;
}

CSimModel::~CSimModel()
{
	int i;
	for (i = 0; i < m_cEquSet.GetSize();i++ )
	{
		CMdEquation *eq = m_cEquSet.GetAt(i);
		if (eq) delete eq;
	}
	m_cEquSet.RemoveAll();
}

void CSimModel::Serialize( CArchive& ar )
{
	CObject::Serialize(ar);
	if( ar.IsStoring( ) )
	{
		ar << m_iMaxTime;
	}
	else
	{
		ar >> m_iMaxTime;
	}
	m_cEquSet.Serialize(ar);
}

//////////////////////////////////////////////////////////////////////
/// Return the Mathematical Model stored at the idx position in the list. 
//////////////////////////////////////////////////////////////////////
CMdEquation* CSimModel::GetEquation(int idx)
{
	CMdEquation* pEqu=NULL;

	int nb = m_cEquSet.GetSize();
	if (idx < nb && idx !=-1)
	{
		pEqu = m_cEquSet.GetAt(idx);
	}
	return pEqu;
}

//////////////////////////////////////////////////////////////////////
/// Return the number of Mathematical Models in the simulation.
//////////////////////////////////////////////////////////////////////
int	CSimModel::GetEquationNb()			
{ 
	return m_cEquSet.GetSize();
}

//////////////////////////////////////////////////////////////////////
/// Return the name of the simulation.
//////////////////////////////////////////////////////////////////////
CString	CSimModel::GetSimulationName()
{ 
	return m_sName;
}
