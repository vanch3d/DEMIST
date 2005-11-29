// UserOutput.cpp: implementation of the CUserOutput class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "simul.h"
#include "UserOutput.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUserData::CUserData()
{
	m_nTLevel = -1;
	m_nTime = -1;
	m_nExpSet = -1;
	m_nOutcome= -1;
	m_dValue= -1.;
	m_dOldValue = -1.;
	m_nType = -1;
	m_bUpdateNow = TRUE;
}

CUserData::~CUserData()
{

}

void CUserData::operator=(const CUserData& cell)
{
	m_nTLevel = cell.m_nTLevel;
	m_nType = cell.m_nType;
	m_nTime = cell.m_nTime;
	m_nExpSet = cell.m_nExpSet;
	m_nOutcome= cell.m_nOutcome;
	m_dValue= cell.m_dValue;
	m_dOldValue= cell.m_dOldValue;
	m_bUpdateNow= cell.m_bUpdateNow;
}

void CUserData::Serialize( CArchive& ar )
{

}



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUserOutput::CUserOutput(int nTime/*=0*/)
{
	m_nTime = nTime;
	m_lUserInput.RemoveAll();

}

CUserOutput::~CUserOutput()
{
	m_lUserInput.RemoveAll();

}

void CUserOutput::Serialize( CArchive& ar )
{

}

void CUserOutput::operator=(const CUserOutput& cell)
{
	m_nTime = cell.m_nTime;
	m_lUserInput.RemoveAll();
	POSITION pos = cell.m_lUserInput.GetStartPosition();
	while (pos)
	{
		CUserData pData;
		unsigned long nkey; 
		cell.m_lUserInput.GetNextAssoc(pos,nkey,pData);
		m_lUserInput.SetAt(nkey,pData);
	}
}



//////////////////////////////////////////////////////////////////////
/// Get the information of user's intervention (CUserData) given an Experimental Set and an Aggregate.
/// \param nExpSet Zero-based index of the Experimental Set associated with the data-point
/// \param nData Zero-based index of the entity associated with the data-point
/// \param pData Fill this CUserData class with the information extracted from the list (#m_lUserInput)
/// \return TRUE if an element was found, FALSE otherwise. 
//////////////////////////////////////////////////////////////////////
BOOL CUserOutput::GetAt(int nExpSet,int nData,CUserData& pData)
{
	unsigned long dKey = nExpSet*10000+nData;
	BOOL bRes = m_lUserInput.Lookup(dKey,pData);
	return bRes;
}

//////////////////////////////////////////////////////////////////////
/// Add the information of user's intervention (a CUserData class) to the list, indexed according to an Experimental Set and an Aggregate.
/// \param nExpSet Zero-based index of the Experimental Set associated with the data-point
/// \param nData Zero-based index of the entity associated with the data-point
/// \param pData A CUserData class containing information about the user's intervention
/// \return TRUE if the element was inserted, FALSE otherwise. 
//////////////////////////////////////////////////////////////////////
BOOL CUserOutput::SetAt(int nExpSet,int nData,CUserData& pData)
{
	unsigned long dKey = nExpSet*10000+nData;
	m_lUserInput.SetAt(dKey,pData);
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
/// Clear all the user's information from the list.
//////////////////////////////////////////////////////////////////////
void CUserOutput::RemoveAll()
{
	m_lUserInput.RemoveAll();
}