// GraphSeries.cpp : implementation file
//

#include "stdafx.h"
//#include "testdraw.h"
#include "GraphSeries.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGraphSeries

CGraphSeries::CGraphSeries()
{
//	for(int i = 0; i < 10; i++)
//		dataValue[i] = 0;
	m_bPred = FALSE;
}

CGraphSeries::~CGraphSeries()
{
}


BEGIN_MESSAGE_MAP(CGraphSeries, CStatic)
	//{{AFX_MSG_MAP(CGraphSeries)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGraphSeries message handlers

CGraphSeries *CGraphSeries::Clone()
{
	CGraphSeries *pSerie = new CGraphSeries();
	pSerie->m_bPred = m_bPred;
	pSerie->m_nGroup = m_nGroup;
	//pSerie->m_nSelected = m_nSelected;
	pSerie->seriesLabel = seriesLabel;
	for (int i=0;i<dataValue.GetSize();i++)
		pSerie->dataValue.SetAtGrow(i,dataValue.GetAt(i));
	for (i=0;i<dataRef.GetSize();i++)
		pSerie->dataRef.SetAtGrow(i,dataRef.GetAt(i));

	return pSerie;
}

void CGraphSeries::SetLabel(CString label)
{
	seriesLabel = label;
}

void CGraphSeries::SetData(int group, int idx, double dValue)
{
	int nb = GetNumData();
	dataValue.SetAtGrow(group,dValue);
	dataRef.SetAtGrow(group,idx);
	//predValue.SetAtGrow(group,GRAPH_UNDEF);
	//dataPred.SetAtGrow(group,FALSE);
	//dataSel.SetAtGrow(group,FALSE);

	nb = GetNumData();
}

void CGraphSeries::SetPrediction(int group, double dValue)
{
	int nb = GetNumData();
	if (group >= nb) return;

	predValue.SetAtGrow(group,dValue);
	dataPred.SetAtGrow(group,TRUE);

	nb = GetNumData();
}

double CGraphSeries::GetPrediction(int group)
{
	if (group >= dataPred.GetSize())
		return GRAPH_UNDEF;
	if (!dataPred.GetAt(group))
		return GRAPH_UNDEF;
	else
		return predValue.GetAt(group);
}


CString CGraphSeries::GetLabel()
{
	return seriesLabel;
}

double CGraphSeries::GetData(int group)
{
//	return dataValue[group];
	int nb = GetNumData();
	return dataValue.GetAt(group);

}

int  CGraphSeries::GetDataRef(int group)
{
	int nb = GetNumData();
	return dataRef.GetAt(group);
}

void CGraphSeries::ResetDataPred()
{
	int nb = GetNumData();
	for (int i=0;i<nb;i++)
	{
		SetDataPred(i, FALSE);
	}
}

void CGraphSeries::SetSelected(int group/*=-1*/,BOOL bSel/*=FALSE*/)
{
	//m_nSelected = nSel;
	int nb = GetNumData();
	if (group==-1)
	{
		for (int i=0;i<nb;i++)
			dataSel.SetAtGrow(i,FALSE);
	}
	else if (group<nb)
	{
		dataSel.SetAtGrow(group,bSel);
	}
}

BOOL CGraphSeries::GetSelected(int group) 
{ 
	int nb = dataSel.GetSize();
	if (group>=nb) return FALSE;
	
	return dataSel.GetAt(group);
}


void CGraphSeries::SetDataPred(int group, BOOL bPred)
{
	dataPred.SetAtGrow(group,bPred);
}

BOOL CGraphSeries::GetDataPred(int group)
{
	int nb = dataPred.GetSize();
	if (group>=nb) return FALSE;
	
	return dataPred.GetAt(group);
}
