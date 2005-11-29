// XYGraphData.cpp: implementation of the CXYGraphData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Simul.h"

#include "XYGraphData.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// CPhasePlotData

CPhasePlotData::CPhasePlotData(int nb)
{
	m_bSelected = FALSE;
	m_bEditable = FALSE;
	m_bPred = FALSE;
	m_nData = nb;
	m_pXaxis = new double[nb];
	m_pYaxis = new double[nb];
}

CPhasePlotData::~CPhasePlotData()
{
	delete []m_pXaxis ;
	delete []m_pYaxis ;
}

/*int CPhasePlotData::HitTest(CLinear<double>* pGraph,CPoint mpt,CTPoint<double> &pt,CTPoint<double> &nDelta,int nLim)
{
	int nRet = -1;
	BOOL bFound = FALSE;
	int nCount = (nLim == -1) ? m_nData : nLim;
	for (int i=nCount;i>=0 && !bFound;i--)
	{
		double x1 = x[i]-nDelta.x;
		double x2 = x[i]+nDelta.x;
		double y1 = y[i]-nDelta.y;
		double y2 = y[i]+nDelta.y;
		bFound = (x1 <= pt.x) && (x2 >= pt.x) &&
				 (y1 <= pt.y) && (y2 >= pt.y);
		if (bFound) nRet = i;

		CTPoint<double> pt1;
		pt1.x = x[i];
		pt1.y = y[i];
		CPoint npt = pGraph->GetTPoint(pt1);
		CRect mrect(npt,npt);
		mrect.InflateRect(3,3);
		bFound = mrect.PtInRect(mpt);
		if (bFound) nRet = i;
	}
	return nRet;
}*/
