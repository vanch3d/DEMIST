// Graph.cpp : implementation file
//

#include "stdafx.h"
//#include "testdraw.h"
#include "GraphSeries.h"
#include "GraphLegend.h"
#include "math.h"
#include "Graph.h"
#include "GridCtrl\MemDC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define GREEN		RGB(0,255,0)
#define BLUE		RGB(0,0,255)
#define RED			RGB(255,0,0)
#define YELLOW		RGB(255,255,0)
#define ORANGE		RGB(255,153,51)
#define HOT_PINK	RGB(255,51,153)
#define PURPLE		RGB(153,0,204)
#define CYAN		RGB(0,255,255)
#define BLACK		RGB(0,0,0)
#define WHITE		RGB(255,255,255)

/////////////////////////////////////////////////////////////////////////////
// CGraph

CGraph::CGraph()
{
	graphSeries = new CObList();
	tickRange = 100;
	seriesSize = 0;
	nTextSize=CSize(0,0);

//	nSelSerie = nSelData = -1;
	m_bIsSelected = FALSE;

	graphHasLegend = FALSE;
	legendWidth = 0;

	graphType = 0;

	dataColor[0] = GREEN;  //green
	dataColor[1] = BLUE;  //blue
	dataColor[2] = RED;  //red
	dataColor[3] = YELLOW;  //yellow
	dataColor[4] = ORANGE;  //orange
	dataColor[5] = HOT_PINK;  //hot pink
	dataColor[6] = PURPLE;  //purple
	dataColor[7] = CYAN;  //cyan
	dataColor[8] = BLACK;  //black
	dataColor[9] = WHITE;  //white

	xAxisAlign = 0;  //horizontal
	xAxisLabelLength = 0;

	xTickFontSize = 12;
	yTickFontSize = 12;
	legendFontSize = 12;

	m_bkColor = RGB(255,255,255);  //replace with desired background color
	m_insColor = RGB(255,255,255);  //replace with desired background color
}

CGraph::CGraph(int type)
{
	graphSeries = new CObList();
	tickRange = 100;
	seriesSize = 0;
	nTextSize=CSize(0,0);

	graphHasLegend = FALSE;
	legendWidth = 0;

//	nSelSerie = nSelData = -1;
	m_bIsSelected = FALSE;
	graphType = type;

	dataColor[0] = GREEN;  //green
	dataColor[1] = BLUE;  //blue
	dataColor[2] = RED;  //red
	dataColor[3] = YELLOW;  //yellow
	dataColor[4] = ORANGE;  //orange
	dataColor[5] = HOT_PINK;  //hot pink
	dataColor[6] = PURPLE;  //purple
	dataColor[7] = CYAN;  //cyan
	dataColor[8] = BLACK;  //black
	dataColor[9] = WHITE;  //white

	xAxisAlign = 0;  //horizontal
	xAxisLabelLength = 0;

	xTickFontSize = 12;
	yTickFontSize = 12;
	legendFontSize = 12;
}

CGraph::~CGraph()
{
	RemoveSeries();
	delete graphSeries;
}


BEGIN_MESSAGE_MAP(CGraph, CStatic)
	//{{AFX_MSG_MAP(CGraph)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGraph message handlers

void CGraph::SetTickSpace(double yDist)
{
	tickSpace = yDist;
}

void CGraph::SetTickRange(int minTick,int maxTick)
{
	tickRangeMax = maxTick;
	tickRangeMin = minTick;
	if (minTick<0)
		tickRange = maxTick-minTick;
	else
		tickRange = maxTick;
}

void CGraph::GetTickRanges(int &minTick,int &maxTick)
{
	minTick = tickRangeMin;
	maxTick = tickRangeMax;
}


void CGraph::SetGraphType(int gType)
{
	graphType = gType;
}

int CGraph::GetGraphType()
{
	return graphType;
}

void CGraph::SetXAxisAlignment(int alignValue)
{
	xAxisAlign = alignValue;
}

int CGraph::GetXAxisAlignment()
{
	return xAxisAlign;
}

void CGraph::DrawGraph(CDC* ptDC)
{
	CString tickLabel;
	CWnd* graphWnd = ptDC->GetWindow();
	CRect graphRect;
	graphWnd->GetClientRect(&graphRect);

	CMemDC MemDC(ptDC);

	//reset graph to be clear background
	COLORREF backColor;
	//backColor = RGB(255,255,255);  //replace with desired background color
	backColor = m_bkColor;  //replace with desired background color
	CBrush backBrush (backColor);
	CBrush* pOldBackBrush;
	pOldBackBrush = MemDC.SelectObject(&backBrush);
	MemDC.Rectangle(0, 0, graphRect.Width(), graphRect.Height());
	MemDC.SelectObject(pOldBackBrush);

	maxHeight = graphRect.Height() - 20;  //for frame window and status bar
	maxWidth = graphRect.Width() - 5;  //for frame window
	//We will leave 5 pixels blank on all sides of the graph.  So
	//top-left side of graph is at 5,5 and the bottom-right side of
	//graph is at ((maxHeight - 5), (maxWidth - 5))
	//these settings are altered by axis labels and legends.

	//draw graph title
	CFont titleFont;
	titleFont.CreateFont(28, 0, 0, 0, 700, FALSE, FALSE, 0,
		ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN,"Arial");
	CFont* pOldFont = (CFont*) MemDC.SelectObject(&titleFont);

//	MemDC.TextOut((maxWidth / 2) - ((graphTitle.GetLength() * 12) / 2),
//				10, graphTitle);
	MemDC.SelectObject(pOldFont);

	if(graphType == 2)  //pie
	{

		//since pie does not have axis lines, set to full size minus 5 on each side
		//these are needed for legend to plot itself
		xAxisWidth = maxWidth - 10;
		yAxisHeight = maxHeight - 20;  //10 buffer and 20 for title
		xApexPoint = 10;
		yApexPoint = maxHeight - 5;
	}
	else
	{
		//X-axis will be raised by 20 pixels for tick-labels
		//Y-axis will be shifted by (max(tick-label size) * 10) + label * 10
		
		tickLabel.Format("%d", tickRange);

		//determine axis specifications 
			//tick label offset is 4 (for the tick) + 6 (total 10) from axis line
		xApexPoint = 5 + (tickLabel.GetLength() * (yTickFontSize / 2)) 
						+ 10 + 20/*(axisYLabel.GetLength() * 8)*/ + 5; //allowing 8 pixels per char in tick label
			//y apex based on 5 + 15 (x label) + 4 (for the tick) + 4 (text below tick) + 12 (tick label) + 10
		if(!xAxisAlign)  //horizontal
			yApexPoint = (maxHeight - 5) - 15;		//apex points are the cross section of axis lines
		else
			yApexPoint = (maxHeight - 5) - (xAxisLabelLength * (xTickFontSize / 2)) - 10;
		yAxisHeight = yApexPoint - 15;//40;
		xAxisWidth = (maxWidth - 5) - xApexPoint;
	}

	//draw legend
	if(graphHasLegend)
		DrawLegend(&MemDC);

	if(graphType != 2)  //pie
	{
		//draw axis lines
		DrawAxis(&MemDC);
	}

	//draw series data and labels
//	MemDC.Rectangle(xApexPoint,yApexPoint+1,xApexPoint+xAxisWidth,yApexPoint-yAxisHeight);
	DrawSeries(&MemDC);

}

void CGraph::DrawAxis(CDC* pDC)
{
	//to allow scalability (height may be less than tickRange)
	double tickScale = 0.00;
//	if(tickRange > yAxisHeight)
		tickScale = ((yAxisHeight * 1.00) / (tickRange * 1.00)) * tickSpace;
//	else tickScale = tickSpace;

	//draw y axis
	pDC->MoveTo(xApexPoint, yApexPoint);  
	pDC->LineTo(xApexPoint, yApexPoint - yAxisHeight);

	int nStart = 0;
	if (tickRangeMin<0) 
	{
		nStart = tickRangeMin/ tickSpace;
		yApexPoint += tickRangeMin/tickSpace*tickScale;
	}

	pDC->SetTextColor(RGB(0,0,0));


	//draw x axis
	pDC->MoveTo(xApexPoint, yApexPoint);  
	//if(graphHasLegend)
	//	pDC->LineTo(xApexPoint + (xAxisWidth - legendWidth - 10), yApexPoint);
	//else
		pDC->LineTo(xApexPoint + xAxisWidth, yApexPoint);

	//draw labels
	CFont sideFont;
	sideFont.CreateFont(16, 0, 900, 0, 700, FALSE, FALSE, 0,
		ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN,"Arial");
	CFont* pOldFont = (CFont*) pDC->SelectObject(&sideFont);

	int oldBkMode = pDC->SetBkMode(TRANSPARENT);
	pDC->TextOut(10, (maxHeight / 2) + ((axisYLabel.GetLength() / 2) * 4), axisYLabel);
	pDC->SelectObject(pOldFont);

	pDC->TextOut(xApexPoint + (xAxisWidth / 2) - ((axisXLabel.GetLength() / 2) * 19), maxHeight - 5 - 6, axisXLabel);
	pDC->SetBkMode(oldBkMode);

	//draw y axis ticks
	for(int y = nStart; y <= tickRangeMax / tickSpace; y++)  //no tick at 0
	{
		int tickYLocation = (int)(yApexPoint - (y * tickScale));

		pDC->MoveTo(xApexPoint - 4, tickYLocation);
		pDC->LineTo(xApexPoint + 4, tickYLocation);

		//draw tick label
		CString tickLabel;
		if (y * tickSpace < 1 && y && tickSpace < 1)
			tickLabel.Format("0.%d", (int) (10*y * tickSpace));
		else
			tickLabel.Format("%d", (int) (y * tickSpace));

		CFont yFont;
		yFont.CreateFont(yTickFontSize, 0, 0, 0, 700, FALSE, FALSE, 0,
			ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN,"Arial");
		pOldFont = (CFont*) pDC->SelectObject(&yFont);

		int oldBkMode = pDC->SetBkMode(TRANSPARENT);
		pDC->TextOut(xApexPoint - 10 - (tickLabel.GetLength() * (yTickFontSize / 2)), tickYLocation - 6, tickLabel);
		pDC->SetBkMode(oldBkMode);

		pDC->SelectObject(pOldFont);

	}

	//draw X axis tick marks
	POSITION pos;
	pos = graphSeries->GetHeadPosition();
	CGraphSeries* tmpSeries;
	for(int x = 1; x <= graphSeries->GetCount(); x++)
	{
		tmpSeries = (CGraphSeries*)graphSeries->GetNext(pos);

		int seriesSpace;
		int tickXLocation;
		//if(graphHasLegend)
		//	seriesSpace= (xAxisWidth - legendWidth - 10) / graphSeries->GetCount();
		//else
			seriesSpace= xAxisWidth / graphSeries->GetCount();
		tickXLocation = xApexPoint + ((x * seriesSpace) - (seriesSpace / 2));

		CRect mrect;
		mrect.left = tickXLocation - seriesSpace/2 +5;
		mrect.right = tickXLocation + seriesSpace/2-5;
		mrect.top = yApexPoint + 4;
		mrect.bottom = maxHeight+15;//mrect.top + 50;
		pDC->MoveTo(tickXLocation,yApexPoint - 4);
		pDC->LineTo(tickXLocation,yApexPoint + 4);

		//pDC->Rectangle(mrect);
		//draw tick label
		CString tickLabel;
		tickLabel = tmpSeries->GetLabel();
//		if(!xAxisAlign)  //horizontal
//			pDC->TextOut(tickXLocation - ((tickLabel.GetLength() * 8) / 2), yApexPoint + 8, tickLabel);
//		else
		{
			CFont sideFont2;
			sideFont2.CreateFont(xTickFontSize, 0, (xAxisAlign * 10), 0, 700, FALSE, FALSE, 0,
				ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
				DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN,"Arial");
			pOldFont = (CFont*) pDC->SelectObject(&sideFont2);
			int oldBkMode = pDC->SetBkMode(TRANSPARENT);
			if(!xAxisAlign)  //horizontal
				//pDC->TextOut(tickXLocation - ((tickLabel.GetLength() * 8) / 2), yApexPoint + 8, tickLabel);
				pDC->DrawText(tickLabel,mrect,DT_CENTER|DT_WORDBREAK|DT_END_ELLIPSIS );
				//pDC->DrawText(tickLabel,mrect,DT_CENTER|DT_WORDBREAK|DT_END_ELLIPSIS );
			else if(xAxisAlign < 180)
				pDC->TextOut(tickXLocation - 8, yApexPoint + 8 + (xAxisLabelLength * (xTickFontSize / 2)), tickLabel);
			else
				pDC->TextOut(tickXLocation + 2, yApexPoint + 8, tickLabel);
			pDC->SetBkMode(oldBkMode);

			pDC->SelectObject(pOldFont);
		}
	}


}

void CGraph::RemoveSeries()
{
	POSITION pos;
	CGraphSeries* pSeries;

	for( pos = graphSeries->GetHeadPosition(); pos != NULL; )
	{
		pSeries = (CGraphSeries*) graphSeries->GetNext( pos );
		delete pSeries;
	}

	graphSeries->RemoveAll();
}

void CGraph::RemoveSerie(int nIdx)
{
	int	nb = GetSeriesSize();
	if (nIdx>nb) return;

	int nPos = 0;
	CGraphSeries* pSeries = NULL;

	POSITION pos = graphSeries->GetHeadPosition();
	POSITION posd = pos;
	while (pos)
	{
		pSeries = (CGraphSeries*) graphSeries->GetNext( pos );
		if (nPos==nIdx) break;
		pSeries = NULL;
		nPos++;
		posd = pos;
	}
	if (pSeries)
	{
		graphSeries->RemoveAt(posd);
		delete pSeries;
	}
}


void CGraph::AddSeries(CGraphSeries* dataSet)
{
	//NVL int numData = 0;
	int numData = dataSet->GetNumData();	// NVL
	//NVL for(int i = 0; i < 10; i++)
	for(int i = 0; i < numData; i++)		// NVL
	{
		//NVL if(dataSet->GetData(i) > 0)
		//NVL 	numData++;
		if(dataSet->GetLabel().GetLength() > xAxisLabelLength)
			xAxisLabelLength = dataSet->GetLabel().GetLength();
	}
	if(numData > seriesSize)
		seriesSize = numData;

	graphSeries->AddTail(dataSet);
}

CGraphSeries* CGraph::GetSeries(int nb)
{
	int dd = graphSeries->GetCount();
	if (nb >= dd) return 0;

	CGraphSeries* tmpSeries=NULL;

	POSITION pos = graphSeries->GetHeadPosition();

	for(int x = 0; x <= nb; x++)
	{
		tmpSeries = (CGraphSeries*)graphSeries->GetNext(pos);
	}
	return tmpSeries;
}


void CGraph::SetXAxisLabel(CString label)
{
	axisXLabel = label;
}
void CGraph::SetYAxisLabel(CString label)
{
	axisYLabel = label;
}

void CGraph::SetColor(int group, COLORREF groupColor)
{
	dataColor[group] = groupColor;
}

COLORREF CGraph::GetColor(int group)
{
	return dataColor[group];
}

void CGraph::DrawSeries(CDC* pDC)
{
	int barWidth;
	int dataPlotSize;   //used to plot rects of data
	int barL, barT, barR, barB;
	int tickXLocation;
	int seriesSpace;
	double barHeight;  //for scalability
	POSITION pos;
	CGraphSeries* tmpSeries;

	if (!seriesSize) return;	// NVL

	if(graphType == 0) //bar
	{
		//determine width of barchart data blocks
		seriesSpace = xAxisWidth / graphSeries->GetCount();
		barWidth = (int)((seriesSpace * .6) / seriesSize);
		dataPlotSize = seriesSize * barWidth;

		pos = graphSeries->GetHeadPosition();

		for(int x = 1; x <= graphSeries->GetCount(); x++)
		{
			tmpSeries = (CGraphSeries*)graphSeries->GetNext(pos);
			tickXLocation = xApexPoint + ((x * seriesSpace) - (seriesSpace / 2));

			/// Draw outcomes' bars
			for(int s = 0; s < seriesSize; s++)
			{
				if (tmpSeries->m_bPred && !tmpSeries->GetDataPred(s)) continue;
				barHeight = 0.00;
				//to allow scalability (height may be less than tickRange)
				double dataScale = 0.00;
				//if(tickRange > yAxisHeight)
					dataScale = (yAxisHeight * 1.00) / (tickRange * 1.00);
				//else dataScale = 1;
				if(tickRange > tmpSeries->GetData(s))
					barHeight = (tmpSeries->GetData(s) * 1.00) * dataScale;
				else barHeight = tickRange * dataScale;
				barL = tickXLocation - (dataPlotSize / 2) + (s * barWidth);
				barT = (int)(yApexPoint - barHeight);
				barR = barL + barWidth;
				barB = yApexPoint;
				
				COLORREF barColor;
				barColor = GetColor(s);

				BOOL bIsSelected = tmpSeries->GetSelected(s);

				CBrush brush;
				if (!m_bIsSelected)
					brush.CreateSolidBrush(barColor);
				else 
				{
					if (!bIsSelected/* || nSelSerie==-1*/)
						brush.CreateSolidBrush(barColor);
					else
						brush.CreateHatchBrush(HS_DIAGCROSS,barColor);
				}

				CBrush* pOldBrush;
				pOldBrush = pDC->SelectObject(&brush);

				pDC->Rectangle(barL, barT, barR, barB);

				pDC->SelectObject(pOldBrush);
			}

			/// Draw outcomes' prediction (if any)
			for(s = 0; s < seriesSize; s++)
			{
				BOOL bIsSelected = tmpSeries->GetSelected(s);
				BOOL bIsPred = tmpSeries->GetDataPred(s);
				if (!bIsPred) continue;

				double datapred = tmpSeries->GetPrediction(s);

				barHeight = 0.00;
				//to allow scalability (height may be less than tickRange)
				double dataScale = 0.00;
				//if(tickRange > yAxisHeight)
					dataScale = (yAxisHeight * 1.00) / (tickRange * 1.00);
				//else dataScale = 1;
				if(tickRange > datapred)
					barHeight = (datapred * 1.00) * dataScale;
				else barHeight = tickRange * dataScale;
				barL = tickXLocation - (dataPlotSize / 2) + (s * barWidth);
				barT = (int)(yApexPoint - barHeight);
				barR = barL + barWidth;
				barB = yApexPoint;
				
				COLORREF barColor;
				barColor = GetColor(s);

				CBrush brush;
				brush.CreateSolidBrush(barColor);

				CBrush* pOldBrush;
				pOldBrush = pDC->SelectObject(&brush);
				pDC->Rectangle(barL-7, barT-2, barR+7, barT+2);
				pDC->SelectObject(pOldBrush);
			}
		}
	}

	if(graphType == 1)  //line
	{
		int lastXLoc, lastYLoc;
		for(int s = 0; s < seriesSize; s++)
		{
			//determine width of barchart data blocks
			//if(graphHasLegend)
			//	seriesSpace = (xAxisWidth - legendWidth - 10) / graphSeries->GetCount();
			//else
				seriesSpace = xAxisWidth / graphSeries->GetCount();
			barWidth = (int)((seriesSpace * .6) / seriesSize);
			dataPlotSize = seriesSize * barWidth;

			pos = graphSeries->GetHeadPosition();

			for(int x = 1; x <= graphSeries->GetCount(); x++)
			{
				tmpSeries = (CGraphSeries*)graphSeries->GetNext(pos);

				tickXLocation = xApexPoint + ((x * seriesSpace) - (seriesSpace / 2));
				
				barHeight = 0.00;
				//to allow scalability (height may be less than tickRange)
				double dataScale = 0.00;
				if(tickRange > yAxisHeight)
					dataScale = (yAxisHeight * 1.00) / (tickRange * 1.00);
				else dataScale = 1;
				barHeight = (tmpSeries->GetData(s) * 1.00) * dataScale;

				int yDataLocation = (int)(yApexPoint - barHeight);
				
				//now have x and y location of center of ellipse
				COLORREF barColor;
				barColor = GetColor(s);
				CBrush brush (barColor);
				CBrush* pOldBrush;
				pOldBrush = pDC->SelectObject(&brush);
				//draw line back to last data member
				if(x > 1)
				{
					CPen* pOldPen;
					CPen linePen (PS_SOLID, 1, barColor);
					pOldPen = pDC->SelectObject(&linePen);
					pDC->MoveTo(lastXLoc + 2, lastYLoc - 1);
					pDC->LineTo(tickXLocation - 3, yDataLocation - 1);
					pDC->SelectObject(pOldPen);
				}
				//now draw ellipse...
				pDC->Ellipse(tickXLocation - 3, yDataLocation - 3,
					tickXLocation + 3, yDataLocation + 3);
				lastXLoc = tickXLocation;
				lastYLoc = yDataLocation;
				pDC->SelectObject(pOldBrush);
			}
		}
	}

	if(graphType == 2)  //pie
	{
		double dataSum = 0.00;  //for storing cumulative sum
		int lastXLocation, lastYLocation;
		int newXLocation, newYLocation;
		double percent = 0.00;
		int degrees;
		double totalSum = 0.00;
		int deltaXY;
		int radius;
			
		lastXLocation = 0;
		lastYLocation = 0;

		//determine width of pie display area
//		if(graphHasLegend)
//			if(((xAxisWidth - legendWidth/* - 10*/) / graphSeries->GetCount())
//					> (yAxisHeight - 10))
//				seriesSpace = (yAxisHeight - 10)/* / graphSeries->GetCount()*/;
//			else
//				seriesSpace = (xAxisWidth - legendWidth/* - 10*/) / graphSeries->GetCount();
//		else
		if (graphSeries->GetCount() == 1)
		{
			if(xAxisWidth > yAxisHeight)
				//seriesSpace = yAxisHeight / graphSeries->GetCount();
				seriesSpace = yAxisHeight / graphSeries->GetCount();
			else
				seriesSpace = xAxisWidth / graphSeries->GetCount();
		}
		else
//				seriesSpace = xAxisWidth / graphSeries->GetCount();
		{
			if(xAxisWidth/graphSeries->GetCount() < yAxisHeight)
				//seriesSpace = yAxisHeight / graphSeries->GetCount();
				seriesSpace = xAxisWidth / graphSeries->GetCount();
			else
				seriesSpace = yAxisHeight;
		}

		//double tmpD = (seriesSpace - /*60*/20)* .8;  //max width of any pie
		nTextSize = pDC->GetTextExtent("MMMMMM");
		double tmpD = seriesSpace - 20 - nTextSize.cy/2;  //max width of any pie
		radius = (int)(tmpD / 2);  

		int deltaW = xAxisWidth-seriesSpace*graphSeries->GetCount();
		int centerYPie = (yAxisHeight + 60) / 2;

		pos = graphSeries->GetHeadPosition();
		for(int x = 1; x <= graphSeries->GetCount(); x++)
		{
			tmpSeries = (CGraphSeries*)graphSeries->GetNext(pos);

			if (tmpSeries->m_bPred)
			{
				CRect barCharRect;
				barCharRect.left = xApexPoint + ((x-1) * seriesSpace);
				barCharRect.top = yApexPoint-yAxisHeight+1;
				barCharRect.right = xApexPoint + ((x == graphSeries->GetCount()) ? xAxisWidth-1 : 
										x * seriesSpace);
				barCharRect.bottom = yApexPoint;
				//rCharRect.InflateRect(-1,-1,-1,0);

				//pDC->Rectangle(barCharRect);
				CBrush mBrush(m_insColor);
				pDC->FillRect(barCharRect,&mBrush);
			}

			totalSum = 0;
			//NVL for(int s = 0; s < 10; s++)
			for(int s = 0; s < tmpSeries->GetNumData(); s++)
				totalSum += tmpSeries->GetData(s);
			int pieLeft, pieRight;
			if(graphSeries->GetCount() == 1)
			{
				//pieLeft = ((xAxisWidth  - 10) / 2) - radius/* - 25*/;
				pieLeft = xApexPoint + (xAxisWidth  - 2*radius)/2;
			}
			else
			{
				//pieLeft = xApexPoint + 15 + (x * /*50*/20) + ((x - 1) * (2 * radius));
				int ddd = deltaW/(graphSeries->GetCount())/2;
				pieLeft = (int)(xApexPoint + (-tmpD  + seriesSpace)/2 +(x-1)*seriesSpace +
					ddd*(x)+ddd*(x-1));

			}
			pieRight = pieLeft + (2 * radius);
			CRect pieRect (pieLeft, 
					centerYPie - radius,
					pieRight, 
					centerYPie + radius);
			int centerXPie = pieLeft + radius;

			//plot series label
			CRect pieLabelRect = pieRect;

			pieLabelRect.top = pieLabelRect.bottom +7;
			pieLabelRect.bottom = pieLabelRect.bottom +37;
			//pDC->Rectangle(pieLabelRect);
			CFont sideFont2;
			sideFont2.CreateFont(xTickFontSize, 0, (xAxisAlign * 10), 0, 700, FALSE, FALSE, 0,
				ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
				DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN,"Arial");
			CFont* pOldFont = (CFont*) pDC->SelectObject(&sideFont2);

			//pDC->TextOut(centerXPie - ((tmpSeries->GetLabel().GetLength() * 8) / 2),
			//			centerYPie + radius + 5, tmpSeries->GetLabel());

			int oldBkMode = pDC->SetBkMode(TRANSPARENT);
			pDC->DrawText(tmpSeries->GetLabel(),pieLabelRect,DT_CENTER|DT_WORDBREAK|DT_END_ELLIPSIS );
			pDC->SetBkMode(oldBkMode);

			pDC->SelectObject(pOldFont);

			lastXLocation = pieLeft;
			lastYLocation = centerYPie;
			
			dataSum = 0;
			int locDeg=0;
			 
			//NVL for(s = 0; s < 10; s++)
			for(s = 0; s < tmpSeries->GetNumData(); s++)
			{
				if(tmpSeries->GetData(s) > 0)
				{
					dataSum += tmpSeries->GetData(s);
					percent = (dataSum * 100) / totalSum;
					degrees = (int)((360 * percent) / 100);
					
						//degress / 90 will never exceed 4.
						//this can tell us the quadrant of destination
					deltaXY = (degrees * radius) / 90;
						//deltaXY is change from start point of pie 0
						//this value is total change.  so if radius is 300
						//and degrees is 270, delta is 300.  The change 
						//would move both x and y 300 pixels.  For x, 100
						//to right is center, another 100 to right edge,
						//100 back to center.  For y, 100 to bottom, 100
						//back to center, 100 to top. thus gives 270 degree
						//rotation.

					//determine destination quadrant...
						//and set newXLocation and newYLocation values...
					int quadrant = degrees / 90;  //truncates decimal
					switch(quadrant)
					{
						//in the computations below, the remarked line is
						//the original computation.  The line above it, for
						//actual use, is the simplified line, and gives the
						//exact same result
						case 0 : newXLocation = pieLeft + deltaXY;
								 newYLocation = centerYPie + deltaXY;
								 break;
						case 1 : newXLocation = pieLeft + deltaXY;
								 newYLocation = centerYPie + (2 * radius) - deltaXY;
//								 newYLocation = centerYPie + radius - (deltaXY - radius);
								 break;
						case 2 : newXLocation = pieLeft + (4 * radius) - deltaXY;
//								 newXLocation = pieLeft + (2 * radius) - (deltaXY - (2 * radius));
								 newYLocation = centerYPie + (2 * radius) - deltaXY;
//								 newYLocation = centerYPie - (deltaXY - (2 * radius));
								 break;
						case 3 : newXLocation = pieLeft + (4 * radius) - deltaXY;
//								 newXLocation = pieLeft + radius - (deltaXY - (3 * radius));
								 newYLocation = centerYPie - (4 * radius) + deltaXY;
//								 newYLocation = centerYPie - radius + (deltaXY - (3 * radius));
								 break;
						case 4 : newXLocation = pieLeft;
								 newYLocation = centerYPie - 1;
								 break;
					}
					double ll = tmpSeries->GetData(s);
					double  rpercent = (ll * 100) / totalSum;
					if (percent==100 && radius/rpercent>66)
					{
						newXLocation = pieLeft;
						newYLocation = centerYPie-1;
					}
					
					if(s == 0)
						lastYLocation -= 1;

					if (!(tmpSeries->m_bPred && !tmpSeries->GetDataPred(s)))
					{

					CPoint p1 (lastXLocation, lastYLocation);
					CPoint p2 (newXLocation, newYLocation);
					COLORREF barColor;
					barColor = GetColor(s);

					//int nSelSerie = -1;//x-1;
					//int nSelData = tmpSeries->GetSelected();
					//if (nSelData!=-1)
					//	nSelSerie = x-1;

					//BOOL bIsSelected = (nSelSerie == x-1 && nSelData == s);
					BOOL bIsSelected = tmpSeries->GetSelected(s);

					CBrush brush;
					if (!m_bIsSelected)
						brush.CreateSolidBrush(barColor);
					else 
					{
						if (!bIsSelected/* || nSelSerie==-1*/)
							brush.CreateSolidBrush(barColor);
						else
							brush.CreateHatchBrush(HS_DIAGCROSS,barColor);
					}
					CBrush* pOldBrush;
					pOldBrush = pDC->SelectObject(&brush);
					CPen mPen(PS_NULL,1,barColor);
					CPen *oldp = pDC->SelectObject(&mPen);
					CRect sliceRect = pieRect;

					sliceRect.NormalizeRect();
					pDC->Pie(sliceRect, p1, p2); 

					//pDC->Chord(pieRect, p1, p2); 
					pDC->SelectObject(oldp);
					pDC->SelectObject(pOldBrush);
					}
					lastXLocation = newXLocation;
					lastYLocation = newYLocation;
				}
				locDeg = degrees;

			}
			
		}
	}

}

void CGraph::SetLegend(int datagroup, CString label)
{
	graphLegend.SetLegendText(datagroup, label);
	graphHasLegend = TRUE;
}

void CGraph::DrawLegend(CDC* pDC)
{
	//determine size of legend
	CFont legendFont;
	legendFont.CreateFont(legendFontSize, 0, 0, 0, 700, FALSE, FALSE, 0,
		ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN,"Arial");
	CFont* pOldFont = (CFont*) pDC->SelectObject(&legendFont);

	int legSize = graphLegend.GetLegendLength();
	CString mstr(_T(""));
	for (int hh=0;hh<legSize;hh++)
	{
		mstr+=_T("M");
	}

	legSize = pDC->GetTextExtent(mstr).cx/2 + 20;
	
	//legSize = ((legSize * (legendFontSize * .66)) + 15)+15;
	//legSize *= seriesSize;
	int legRow = 0;
	if (xAxisWidth >= (legSize*seriesSize))
	{
		legRow = 1;
	}
	else if (xAxisWidth <legSize)
	{
		legRow = seriesSize;
	}
	else
	{
		legRow = xAxisWidth/legSize;
		legRow = 1+(seriesSize-1)/legRow;
		legRow = min(seriesSize,legRow );
	}
	int legItem = 1+(seriesSize-1)/legRow;

	int dH = (legRow) * 18 + (legRow-1) * 3 +6;
	yAxisHeight -= dH;

//	pDC->Rectangle(xApexPoint,yApexPoint-yAxisHeight-5,
//			xApexPoint+xAxisWidth,yApexPoint-yAxisHeight-5-dH);

//	for (int i=0;i<legRow;i++)
//	{
//		pDC->Rectangle(xApexPoint+5,yApexPoint-yAxisHeight-5-18*i-3*(i+1),
//			xApexPoint+xAxisWidth-5,yApexPoint-yAxisHeight-5-(18+3)*(i+1));
//	}

	//pDC->Rectangle(xApexPoint,yApexPoint,xApexPoint+xAxisWidth,yApexPoint-yAxisHeight);


	//12 chars per seriesSize + 6 for spacing (3 top and bottom) 
	//+ 1 set for label title(3+12+6) + rectangle (2 chars) + 3 for final bottom buffer
	int legendHeight = 23 + (seriesSize * 18) + 3;
	int legendL, legendT, legendR, legendB;
	int barL, barT, barR, barB;

	legendT = (maxHeight / 2) - (legendHeight / 2);
	legendB = legendT + legendHeight;
	legendR = maxWidth - 5;
	legendL = (int)(legendR - ((graphLegend.GetLegendLength() * (legendFontSize * .66)) + 15));
		//allows 50 pixels for display of legend bar 45 + 5 space.
	legendWidth = legendR - legendL;

//	pDC->Rectangle(legendL, legendT, legendR, legendB);
//	pDC->TextOut(legendL + (legendWidth / 2) - 24, 
//					legendT + 3, "Legend");

	int nIdx = -1;
	CPoint StartPoint(xApexPoint+5,yApexPoint-yAxisHeight-5-dH);
	CRect mPRect(StartPoint,StartPoint);
	mPRect.InflateRect(2,2);
//	pDC->Rectangle(mPRect);
//	legSize =  1+ seriesSize / legSize;

	for(int i = 0; i < seriesSize; i++)
	{
		//top "Legend" text will use 12 + 3 top + 6 bottom (21 total)
		//each legend label will need 3 chars on top, so the 24 in the offset
		//each label than uses 12 + 3 below + 3 above next label, so 18
		// in the i * offset. 
		int col = i % legItem;
		int lin = i / legItem;
	//	if (!r) nIdx++;

		int dx = -2+(xAxisWidth-(legItem*legSize))/legItem;

		CRect locRect;
		locRect.left = StartPoint.x + col*(legSize+dx);
		locRect.right = StartPoint.x + (col+1)*(legSize+dx);
		locRect.top = StartPoint.y + (lin+1)*3 + lin*18;
		locRect.bottom = StartPoint.y + (lin+1)*3 + (lin+1)*18;

//		pDC->Rectangle(locRect);
		//pDC->TextOut(StartPoint.x,StartPoint.y,_T("fdfdfdfd"));
//		CFont legendFont;
//		legendFont.CreateFont(legendFontSize, 0, 0, 0, 700, FALSE, FALSE, 0,
//			ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
//			DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN,"Arial");
//		CFont* pOldFont = (CFont*) pDC->SelectObject(&legendFont);

//		pDC->TextOut(legendL + 5, legendT + 24 + (i * 18), graphLegend.GetLegendText(i));
		int oldBkMode = pDC->SetBkMode(TRANSPARENT);
		pDC->TextOut(locRect.left,locRect.top+2, graphLegend.GetLegendText(i));
		pDC->SetBkMode(oldBkMode);

		CSize msize = pDC->GetTextExtent(graphLegend.GetLegendText(i));
		CRect tmpRect = locRect;
		locRect.DeflateRect(msize.cx+5,3,20,3);
		locRect.right = locRect.left+20;

		//draw bar
		COLORREF barColor;
		barColor = GetColor(i);
		CBrush brush (barColor);
		CBrush* pOldBrush;
		pOldBrush = pDC->SelectObject(&brush);

		barL = legendL + 5 + (graphLegend.GetLegendText(i).GetLength() * 6) + 5;
		barT = legendT + 24 + (i * 18) + 1, graphLegend.GetLegendText(i);
		barR = legendR - 5;
		barB = barT + 12;
//		pDC->Rectangle(barL, barT, barR, barB);
		pDC->Rectangle(locRect);


/*		POSITION pos;
		pos = graphSeries->GetHeadPosition();
		pos = graphSeries->GetHeadPosition();
		CGraphSeries* tmpSeries = (CGraphSeries*)graphSeries->GetNext(pos);

		//int nSelSerie = i;
		BOOL nSelData = tmpSeries->GetSelected(i);

		if (nSelData)
		{
			//tmpRect.DeflateRect(2,0,0,0);
			tmpRect.left -=2;
			tmpRect.right = locRect.right+2;
			pDC->DrawFocusRect(tmpRect);
		}*/

		BOOL bSel=FALSE;
		POSITION pos = graphSeries->GetHeadPosition();
		int ttt = graphSeries->GetCount();

		for(int x = 1; x <= ttt; x++)
		{
			CGraphSeries* tmpSeries = (CGraphSeries*)graphSeries->GetNext(pos);
			bSel |= tmpSeries->GetSelected(i);
		}

		pDC->SelectObject(pOldBrush);
		if (bSel)
		{
			//tmpRect.DeflateRect(2,0,0,0);
			tmpRect.left -=2;
			tmpRect.right = locRect.right+2;
			pDC->DrawFocusRect(tmpRect);
		}
	
	}
		pDC->SelectObject(pOldFont);

}

void CGraph::SetGraphTitle(CString title)
{
	graphTitle = title;
}

void CGraph::SetXTickFontSize(int size)
{
	xTickFontSize = size;
}

void CGraph::SetYTickFontSize(int size)
{
	yTickFontSize = size;
}

void CGraph::SetLegendFontSize(int size)
{
	legendFontSize = size;
}


int CGraph::PrintGraph(CDC* pDC, CPrintInfo* pInfo)
{
	CString		str;
	//char		tmpStr[10];
	CFont		graphTitleFont, textFont;
	TEXTMETRIC	tm;
	CString tickLabel;

    //titleFont is 24 point, bold
	graphTitleFont.CreateFont(-480, 0, 0, 0, 700, FALSE, FALSE, 0, ANSI_CHARSET,
			OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY, DEFAULT_PITCH || FF_ROMAN,
			"Times New Roman");

	//textFont is 12 point
	textFont.CreateFont(-240, 0, 0, 0, 400, FALSE, FALSE, 0, ANSI_CHARSET,
			OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY, DEFAULT_PITCH || FF_ROMAN,
			"Times New Roman");

	pDC->SetMapMode(MM_TWIPS);
	if(pInfo->m_nCurPage == 1)
	{
		CFont* pOldFont = (CFont*) pDC->SelectObject(&graphTitleFont);
		pDC->GetTextMetrics(&tm);

		int charWidth = tm.tmMaxCharWidth;
		//next line is centered....trust me !!
		int oldBkMode = pDC->SetBkMode(TRANSPARENT);
		pDC->TextOut(((graphTitle.GetLength() / 2) * charWidth) - (leftMargin / 2), pGraphT, graphTitle);
		pDC->SetBkMode(oldBkMode);

		pDC->SelectObject(pOldFont);
		
		//draw legend
		if(graphHasLegend)
			PrintLegend(pDC);

		maxHeight = pGraphT - pGraphB; 
		maxWidth = pGraphR - pGraphL - 2200; 
		if(graphType == 2)  //pie
		{

			//since pie does not have axis lines, set to full size minus 5 on each side
			//these are needed for legend to plot itself
			xAxisWidth = pGraphR - pGraphL - legendWidth - 200;
			yAxisHeight = maxHeight - 200;  //100 buffer and 200 for title
			xApexPoint = pGraphL + 1500;
			yApexPoint = pGraphB + maxHeight + 500;
		}
		else
		{
			
			tickLabel.Format("%d", tickRange);

			//determine axis specifications 
			xApexPoint = pGraphL + 350 + (tickLabel.GetLength() * 240); 

			yApexPoint = pGraphB + 500;		
			yAxisHeight = maxHeight + (pGraphB - yApexPoint) - 1000;
			xAxisWidth = pGraphR - legendWidth - xApexPoint;
		}

		if(graphType != 2)  //pie
		{
			//draw axis lines
			PrintAxis(pDC);
		}

		//draw series data and labels
		PrintSeries(pDC);

	}  //end printing for Page 1    
	
	//update pGraphB value to pad some space below the graph
	pGraphB -= 90;
	return pGraphB;
}


void CGraph::PrintLegend(CDC *pDC)
{
	CFont		legendLabelFont, legendTitleFont;
	TEXTMETRIC	tm;

	int legendL, legendT, legendR, legendB;
	int barL, barT, barR, barB;

	//legendLabelFont is 10 point
	legendLabelFont.CreateFont(-200, 0, 0, 0, 400, FALSE, FALSE, 0, ANSI_CHARSET,
			OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY, DEFAULT_PITCH || FF_ROMAN,
			"Times New Roman");

	//legendTitleFont is 12 point
	legendTitleFont.CreateFont(-240, 0, 0, 0, 400, FALSE, FALSE, 0, ANSI_CHARSET,
			OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY, DEFAULT_PITCH || FF_ROMAN,
			"Times New Roman");

//	pDC->Rectangle(pGraphL, pGraphT, pGraphR, pGraphB);
	legendT = (((pGraphB - pGraphT) / 2) + pGraphT) + (((seriesSize + 1) / 2) * 240 + 300);
	legendB = (((pGraphB - pGraphT) / 2) + pGraphT) - (((seriesSize + 1) / 2) * 240 + 300);
	legendR = pGraphR - 50;
	legendL = (int)(legendR - ((legendR - pGraphL) * .15));
	legendWidth = legendR - legendL;

	pDC->Rectangle(legendL, legendT, legendR, legendB);
	CFont* pOldFont = (CFont*) pDC->SelectObject(&legendTitleFont);
	pDC->GetTextMetrics(&tm);
	int oldBkMode = pDC->SetBkMode(TRANSPARENT);
	pDC->TextOut(legendL + (legendWidth / 2) - 360, 
					legendT - 160, "Legend");
	pDC->SetBkMode(oldBkMode);
	pDC->SelectObject(pOldFont);
	for(int i = 0; i < seriesSize; i++)
	{
		pOldFont = (CFont*) pDC->SelectObject(&legendLabelFont);
		pDC->GetTextMetrics(&tm);

		int charWidth = tm.tmMaxCharWidth;
		int charHeight = tm.tmHeight;

		CString valuStr;
		valuStr.Format("value %d", i + 1);
		int oldBkMode = pDC->SetBkMode(TRANSPARENT);
		pDC->TextOut(legendL + 50, legendT - (2 * charHeight) - 120 - (i * charHeight), valuStr);
		pDC->SetBkMode(oldBkMode);
		pDC->SelectObject(pOldFont);

		//draw bar
		COLORREF barColor;
		barColor = GetColor(i);
		CBrush brush (barColor);
		CBrush* pOldBrush;
		pOldBrush = pDC->SelectObject(&brush);

		barL = legendL + (9 * 120);
		barT = legendT - (2 * charHeight) - 120 - (i * charHeight) - 60;
		barR = legendR - 50;
		barB = barT - 120;
		pDC->Rectangle(barL, barT, barR, barB);

		pDC->SelectObject(pOldBrush);
	
	}

}

void CGraph::PrintAxis(CDC *pDC)
{
	pDC->SetTextColor(RGB(0,0,0));

	//draw y axis
	pDC->MoveTo(xApexPoint, yApexPoint);  
	pDC->LineTo(xApexPoint, yApexPoint + yAxisHeight - 250);

	//draw x axis
	pDC->MoveTo(xApexPoint, yApexPoint);  
//	if(graphHasLegend)
//		pDC->LineTo(xApexPoint + (xAxisWidth - 500), yApexPoint);
//	else
		pDC->LineTo(xApexPoint + xAxisWidth - 500, yApexPoint);

	//draw labels
	CFont sideFont;
	sideFont.CreateFont(320, 0, 2700, 0, 700, FALSE, FALSE, 0,
		ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN,"Arial");
	CFont* pOldFont = (CFont*) pDC->SelectObject(&sideFont);

	int oldBkMode = pDC->SetBkMode(TRANSPARENT);
	pDC->TextOut(pGraphL + 10, pGraphT - (maxHeight / 2) - ((axisYLabel.GetLength() / 2) * 160), axisYLabel);
	pDC->SelectObject(pOldFont);

	pDC->TextOut(xApexPoint + (xAxisWidth / 2) - ((axisXLabel.GetLength() / 2) * 19), maxHeight - 5 - 6, axisXLabel);
	 pDC->SetBkMode(oldBkMode);

	//to allow scalability (height may be less than tickRange)
	double tickScale = 0.00;
	if(tickRange > yAxisHeight)
		tickScale = ((yAxisHeight * 1.00) / (tickRange * 1.00)) * (tickSpace * 20);
	else tickScale = tickSpace * 15;

	//draw y axis ticks
	for(int y = 1; y <= tickRange / tickSpace; y++)  //no tick at 0
	{
		int tickYLocation = (int)(yApexPoint + (y * tickScale));

		pDC->MoveTo(xApexPoint - 75, tickYLocation);
		pDC->LineTo(xApexPoint + 75, tickYLocation);

		//draw tick label
		CString tickLabel;
		tickLabel.Format("%d", y * tickSpace);

		CFont yFont;
		yFont.CreateFont(yTickFontSize * 20, 0, 0, 0, 700, FALSE, FALSE, 0,
			ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN,"Arial");
		pOldFont = (CFont*) pDC->SelectObject(&yFont);

	int oldBkMode = pDC->SetBkMode(TRANSPARENT);
		pDC->TextOut(xApexPoint - 150 - (tickLabel.GetLength() * (yTickFontSize / 2) * 20), tickYLocation + 120, tickLabel);
		pDC->SetBkMode(oldBkMode);

		pDC->SelectObject(pOldFont);

	}

	//draw X axis tick marks
	POSITION pos;
	pos = graphSeries->GetHeadPosition();
	CGraphSeries* tmpSeries;
	for(int x = 1; x <= graphSeries->GetCount(); x++)
	{
		tmpSeries = (CGraphSeries*)graphSeries->GetNext(pos);

		int seriesSpace;
		int tickXLocation;
		//if(graphHasLegend)
		//	seriesSpace= (xAxisWidth - legendWidth - 10) / graphSeries->GetCount();
		//else
			seriesSpace= xAxisWidth / graphSeries->GetCount();
		tickXLocation = xApexPoint + ((x * seriesSpace) - (seriesSpace / 2));

		pDC->MoveTo(tickXLocation,yApexPoint - 75);
		pDC->LineTo(tickXLocation,yApexPoint + 75);

		//draw tick label
		CString tickLabel;
		tickLabel = tmpSeries->GetLabel();
		CFont sideFont2;
		sideFont2.CreateFont(xTickFontSize * 20, 0, ((360 - xAxisAlign) * 10), 0, 700, FALSE, FALSE, 0,
			ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN,"Arial");
		pOldFont = (CFont*) pDC->SelectObject(&sideFont2);
		int oldBkMode = pDC->SetBkMode(TRANSPARENT);

		if(!xAxisAlign)  //horizontal
			pDC->TextOut(tickXLocation - ((tickLabel.GetLength() / 2) * (xTickFontSize * 10)), yApexPoint - 160, tickLabel);
		else
		{
			if(xAxisAlign < 180)
				pDC->TextOut(tickXLocation - ((xTickFontSize * 20) / 2), yApexPoint - (xTickFontSize * 20) - (xAxisLabelLength * ((xTickFontSize * 20) / 2)), tickLabel);
			else
				pDC->TextOut(tickXLocation + ((xTickFontSize * 20) / 2), yApexPoint - 160, tickLabel);

		}
		pDC->SetBkMode(oldBkMode);
		pDC->SelectObject(pOldFont);
	}

}

void CGraph::PrintSeries(CDC *pDC)
{
	int barWidth;
	int dataPlotSize;   //used to plot rects of data
	int barL, barT, barR, barB;
	int tickXLocation;
	int seriesSpace;
	double barHeight;  //for scalability
	POSITION pos;
	CGraphSeries* tmpSeries;

	if(graphType == 0) //bar
	{
		//determine width of barchart data blocks
		//if(graphHasLegend)
		//	seriesSpace = (xAxisWidth - legendWidth - 10) / graphSeries->GetCount();
		//else
			seriesSpace = xAxisWidth / graphSeries->GetCount();
		barWidth = (int)((seriesSpace * .6) / seriesSize);
		dataPlotSize = seriesSize * barWidth;

		pos = graphSeries->GetHeadPosition();

		for(int x = 1; x <= graphSeries->GetCount(); x++)
		{
			tmpSeries = (CGraphSeries*)graphSeries->GetNext(pos);

			double tickScale = 0.00;
			if(tickRange > yAxisHeight)
				tickScale = ((yAxisHeight * 1.00) / (tickRange * 1.00)) * (tickSpace * 20);
			else tickScale = tickSpace * 15;
			tickXLocation = xApexPoint + ((x * seriesSpace) - (seriesSpace / 2));

			for(int s = 0; s < seriesSize; s++)
			{
				barHeight = 0.00;

				int axisHeight = (int)(tickScale * (tickRange / tickSpace));
				double barPercent = 0.00;
				barPercent = (tmpSeries->GetData(s) * 1.00) / (tickRange * 1.00);
				barHeight = barPercent * axisHeight;

				barL = tickXLocation - (dataPlotSize / 2) + (s * barWidth);
				barT = (int)(yApexPoint + barHeight);
				barR = barL + barWidth;
				barB = yApexPoint;
				
				COLORREF barColor;
				barColor = GetColor(s);
				CBrush brush (barColor);
				CBrush* pOldBrush;
				pOldBrush = pDC->SelectObject(&brush);
				pDC->Rectangle(barL, barT, barR, barB);
				pDC->SelectObject(pOldBrush);
			}
		}
	}

	if(graphType == 1)  //line
	{
		int lastXLoc, lastYLoc;
		for(int s = 0; s < seriesSize; s++)
		{
			//determine width of barchart data blocks
			//if(graphHasLegend)
			//	seriesSpace = (xAxisWidth - legendWidth - 10) / graphSeries->GetCount();
			//else
				seriesSpace = xAxisWidth / graphSeries->GetCount();
			barWidth = (int)((seriesSpace * .6) / seriesSize);
			dataPlotSize = seriesSize * barWidth;

			pos = graphSeries->GetHeadPosition();

			for(int x = 1; x <= graphSeries->GetCount(); x++)
			{
				tmpSeries = (CGraphSeries*)graphSeries->GetNext(pos);

				tickXLocation = xApexPoint + ((x * seriesSpace) - (seriesSpace / 2));
				
				barHeight = 0.00;
				//to allow scalability (height may be less than tickRange)
				double dataScale = 0.00;
				if(tickRange > yAxisHeight)
					dataScale = ((yAxisHeight * 1.00) / (tickRange * 1.00)) * (tickSpace * 20);
				else dataScale = tickSpace * 15.00;

				int axisHeight = (int)(dataScale * (tickRange / tickSpace));
				double barPercent = 0.00;
				barPercent = (tmpSeries->GetData(s) * 1.00) / (tickRange * 1.00);
				barHeight = barPercent * axisHeight;

				int yDataLocation = (int)(yApexPoint + barHeight);
				
				//now have x and y location of center of ellipse
				COLORREF barColor;
				barColor = GetColor(s);
				CBrush brush (barColor);
				CBrush* pOldBrush;
				pOldBrush = pDC->SelectObject(&brush);
				//draw line back to last data member
				if(x > 1)
				{
					CPen* pOldPen;
					CPen linePen (PS_SOLID, 1, barColor);
					pOldPen = pDC->SelectObject(&linePen);
					pDC->MoveTo(lastXLoc/* - 40*/, lastYLoc/* + 20*/);
					pDC->LineTo(tickXLocation/* + 60*/, yDataLocation/* + 20*/);
					pDC->SelectObject(pOldPen);
				}
				//now draw ellipse...
				pDC->Ellipse(tickXLocation + 60, yDataLocation + 60,
					tickXLocation - 60, yDataLocation - 60);
				lastXLoc = tickXLocation;
				lastYLoc = yDataLocation;
				pDC->SelectObject(pOldBrush);
			}
		}
	}

	if(graphType == 2)  //pie
	{
		double dataSum = 0.00;  //for storing cumulative sum
		int lastXLocation, lastYLocation;
		int newXLocation, newYLocation;
		double percent = 0.00;
		int degrees;
		double totalSum = 0.00;
		int deltaXY;
		int radius;
			
		lastXLocation = 0;
		lastYLocation = 0;

		//determine width of pie display area
		//if(graphHasLegend)
		//	if((xAxisWidth / graphSeries->GetCount())
		//			> (yAxisHeight - 200))
		//		seriesSpace = (yAxisHeight - 200)/* / graphSeries->GetCount()*/;
		//	else
		//		seriesSpace = xAxisWidth / graphSeries->GetCount();
		//else
		if(xAxisWidth > yAxisHeight)
			//seriesSpace = yAxisHeight / graphSeries->GetCount();
			seriesSpace = xAxisWidth / graphSeries->GetCount();
		else
			seriesSpace = xAxisWidth / graphSeries->GetCount();
		double tmpD = (seriesSpace - 100) * .9;  //max width of any pie
		radius = (int)(tmpD / 2);  

		int centerYPie = pGraphT - (yAxisHeight / 2) - 300;
		
		pos = graphSeries->GetHeadPosition();
		for(int x = 1; x <= graphSeries->GetCount(); x++)
		{
			tmpSeries = (CGraphSeries*)graphSeries->GetNext(pos);
			totalSum = 0;
			for(int s = 0; s < 10; s++)
				totalSum += tmpSeries->GetData(s);
			int pieLeft, pieRight;
			if(graphSeries->GetCount() == 1)
			{
				pieLeft = ((xAxisWidth - 200) / 2) - radius;
			}
			else
			{
				pieLeft = pGraphL + 15 + (x * 200) + ((x - 1) * (2 * radius));
			}
			pieRight = pieLeft + (2 * radius);
			CRect pieRect (pieLeft, 
					centerYPie + radius,
					pieRight, 
					centerYPie - radius);
			int centerXPie = pieLeft + radius;

			CFont pieFont;
			pieFont.CreateFont(xTickFontSize * 20, 0, 0, 0, 700, FALSE, FALSE, 0,
				ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
				DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN,"Arial");
			CFont* pOldFont = (CFont*) pDC->SelectObject(&pieFont);

			//plot series label
			int oldBkMode = pDC->SetBkMode(TRANSPARENT);
			pDC->TextOut(centerXPie - ((tmpSeries->GetLabel().GetLength() / 2) * ((xTickFontSize / 2) * 20)),
						centerYPie - radius - 300, tmpSeries->GetLabel());
			pDC->SelectObject(pOldFont);
			pDC->SetBkMode(oldBkMode);

			lastXLocation = pieLeft;
			lastYLocation = centerYPie;
			
			dataSum = 0;
			for(s = 0; s < seriesSize; s++)
			{
				if(tmpSeries->GetData(s) > 0)
				{
					dataSum += tmpSeries->GetData(s);
					percent = (dataSum * 100) / totalSum;
					degrees = (int)((360 * percent) / 100);

						//degress / 90 will never exceed 4.
						//this can tell us the quadrant of destination
					deltaXY = (degrees * radius) / 90;
						//deltaXY is change from start point of pie 0
						//this value is total change.  so if radius is 300
						//and degrees is 270, delta is 300.  The change 
						//would move both x and y 300 pixels.  For x, 100
						//to right is center, another 100 to right edge,
						//100 back to center.  For y, 100 to bottom, 100
						//back to center, 100 to top. thus gives 270 degree
						//rotation.

					//determine destination quadrant...
						//and set newXLocation and newYLocation values...
					int quadrant = degrees / 90;  //truncates decimal
					switch(quadrant)
					{
						//in the computations below, the remarked line is
						//the original computation.  The line above it, for
						//actual use, is the simplified line, and gives the
						//exact same result
						case 0 : newXLocation = pieLeft + deltaXY;
								 newYLocation = centerYPie - deltaXY;
								 break;
						case 1 : newXLocation = pieLeft + deltaXY;
								 newYLocation = centerYPie - (2 * radius) + deltaXY;
								 break;
						case 2 : newXLocation = pieLeft + (4 * radius) - deltaXY;
								 newYLocation = centerYPie - (2 * radius) + deltaXY;
								 break;
						case 3 : newXLocation = pieLeft + (4 * radius) - deltaXY;
								 newYLocation = centerYPie + (4 * radius) - deltaXY;
								 break;
						case 4 : newXLocation = pieLeft;
								 newYLocation = centerYPie + 1;
								 break;
					}
					
					if(s == 0)
						lastYLocation -= 1;

					CPoint p1 (lastXLocation, lastYLocation);
					CPoint p2 (newXLocation, newYLocation);
					COLORREF barColor;
					barColor = GetColor(s);
					CBrush brush (barColor);
					CBrush* pOldBrush;
					pOldBrush = pDC->SelectObject(&brush);
					pDC->Pie(pieRect, p1, p2); 
					pDC->SelectObject(pOldBrush);

					lastXLocation = newXLocation;
					lastYLocation = newYLocation;
				}
			}
			
		}
	}
}

void CGraph::SetMargins(int top, int bottom, int left, int right, int graphTop)
{
	//page margins for portrait or landscape distinction
	topMargin = top;
	bottomMargin = bottom;
	leftMargin = left;
	rightMargin = right;

	//set up rectangle area for showing the graph
	pGraphL = leftMargin;
	pGraphT = graphTop;
	pGraphB = pGraphT - 7200;
	pGraphR = rightMargin;

	if(pGraphB < bottomMargin)
		pGraphB = bottomMargin; 

}

BOOL CGraph::OnHitTest(CPoint pt,int& nSerie,int & nData)
{
	int barWidth;
	int dataPlotSize;   //used to plot rects of data
	int barL, barT, barR, barB;
	int tickXLocation;
	int seriesSpace;
	double barHeight;  //for scalability
	POSITION pos;
	CGraphSeries* tmpSeries;

	BOOL bRedraw = FALSE;


//	if (nSelSerie != -1 || nSelData !=-1) 
//		bRedraw=TRUE;

	int nSelSerie = nSerie = -1;
	int nSelData = nData = -1;
	if (!seriesSize) return bRedraw;

	if(graphType == 0) //bar
	{
		//determine width of barchart data blocks
		//if(graphHasLegend)
		//	seriesSpace = (xAxisWidth - legendWidth - 10) / graphSeries->GetCount();
		//else
			seriesSpace = xAxisWidth / graphSeries->GetCount();
		barWidth = (int)((seriesSpace * .6) / seriesSize);
		dataPlotSize = seriesSize * barWidth;

		pos = graphSeries->GetHeadPosition();

		for(int x = 1; x <= graphSeries->GetCount(); x++)
		{
			tmpSeries = (CGraphSeries*)graphSeries->GetNext(pos);

			tickXLocation = xApexPoint + ((x * seriesSpace) - (seriesSpace / 2));

			for(int s = 0; s < seriesSize; s++)
			{
				barHeight = 0.00;
				//to allow scalability (height may be less than tickRange)
				double dataScale = 0.00;
				//if(tickRange > yAxisHeight)
					dataScale = (yAxisHeight * 1.00) / (tickRange * 1.00);
				//else dataScale = 1;
				if(tickRange > tmpSeries->GetData(s))
					barHeight = (tmpSeries->GetData(s) * 1.00) * dataScale;
				else barHeight = tickRange * dataScale;
				barL = tickXLocation - (dataPlotSize / 2) + (s * barWidth);
				barT = (int)(yApexPoint - barHeight);
				barR = barL + barWidth;
				barB = yApexPoint;
				
				CRect mrect(barL, barT, barR, barB);
				mrect.NormalizeRect();
				BOOL bTmp = mrect.PtInRect(pt);
				if (bTmp)
				{
					int nSelSerie = nSerie = x-1;
					int nSelData = nData = s;
					int nn = tmpSeries->GetDataRef(s);
					return TRUE;
				}
			}
		}
	}

	if(graphType == 1)  //line
	{
//		int lastXLoc, lastYLoc;
		for(int s = 0; s < seriesSize; s++)
		{
			//determine width of barchart data blocks
			//if(graphHasLegend)
			//	seriesSpace = (xAxisWidth - legendWidth - 10) / graphSeries->GetCount();
			//else
				seriesSpace = xAxisWidth / graphSeries->GetCount();
			barWidth = (int)((seriesSpace * .6) / seriesSize);
			dataPlotSize = seriesSize * barWidth;

			pos = graphSeries->GetHeadPosition();

			for(int x = 1; x <= graphSeries->GetCount(); x++)
			{
				tmpSeries = (CGraphSeries*)graphSeries->GetNext(pos);

				tickXLocation = xApexPoint + ((x * seriesSpace) - (seriesSpace / 2));
				
				barHeight = 0.00;
				//to allow scalability (height may be less than tickRange)
				double dataScale = 0.00;
				if(tickRange > yAxisHeight)
					dataScale = (yAxisHeight * 1.00) / (tickRange * 1.00);
				else dataScale = 1;
				barHeight = (tmpSeries->GetData(s) * 1.00) * dataScale;

				int yDataLocation = (int)(yApexPoint - barHeight);
				
				//now draw ellipse...
				CRect mrect(tickXLocation - 3, yDataLocation - 3,
								tickXLocation + 3, yDataLocation + 3);
				mrect.NormalizeRect();
				BOOL bTmp = mrect.PtInRect(pt);
				if (bTmp)
				{
					nSelSerie = nSerie = x-1;
					nSelData = nData = s;
					return TRUE;
				}
			}
		}
	}

	if(graphType == 2)  //pie
	{
		double dataSum = 0.00;  //for storing cumulative sum
//		int lastXLocation, lastYLocation;
//		int newXLocation, newYLocation;
		double percent = 0.00;
		int degrees;
		double totalSum = 0.00;
//		int deltaXY;
		int radius;
			
//		lastXLocation = 0;
//		lastYLocation = 0;

		//determine width of pie display area
//		if(graphHasLegend)
//			if(((xAxisWidth - legendWidth/* - 10*/) / graphSeries->GetCount())
//					> (yAxisHeight - 10))
//				seriesSpace = (yAxisHeight - 10)/* / graphSeries->GetCount()*/;
//			else
//				seriesSpace = (xAxisWidth - legendWidth/* - 10*/) / graphSeries->GetCount();
//		else
		if (graphSeries->GetCount() == 1)
		{
			if(xAxisWidth > yAxisHeight)
				//seriesSpace = yAxisHeight / graphSeries->GetCount();
				seriesSpace = yAxisHeight / graphSeries->GetCount();
			else
				seriesSpace = xAxisWidth / graphSeries->GetCount();
		}
		else
//				seriesSpace = xAxisWidth / graphSeries->GetCount();
		{
			if(xAxisWidth/graphSeries->GetCount() < yAxisHeight)
				//seriesSpace = yAxisHeight / graphSeries->GetCount();
				seriesSpace = xAxisWidth / graphSeries->GetCount();
			else
				seriesSpace = yAxisHeight;
		}
//		CWnd *pWnd = GetParent();
//		CPaintDC pDC(pWnd);
//		CSize nTextSize = pDC.GetTextExtent("MMMMMM");
		double tmpD = seriesSpace - 20 - nTextSize.cy/2;  //max width of any pie
		radius = (int)(tmpD / 2);  

		int deltaW = xAxisWidth-seriesSpace*graphSeries->GetCount();

		int centerYPie = (yAxisHeight + 60) / 2;
		
		pos = graphSeries->GetHeadPosition();
		for(int x = 1; x <= graphSeries->GetCount(); x++)
		{
			tmpSeries = (CGraphSeries*)graphSeries->GetNext(pos);
			totalSum = 0;
			//NVL for(int s = 0; s < 10; s++)
			for(int s = 0; s < tmpSeries->GetNumData(); s++)
				totalSum += tmpSeries->GetData(s);
			int pieLeft, pieRight;
			if(graphSeries->GetCount() == 1)
			{
				pieLeft = xApexPoint + (xAxisWidth  - 2*radius)/2;
			}
			else
			{
				int ddd = deltaW/(graphSeries->GetCount())/2;
				pieLeft = (int)(xApexPoint + (-tmpD  + seriesSpace)/2 +(x-1)*seriesSpace +
					ddd*(x)+ddd*(x-1));
			}
			pieRight = pieLeft + (2 * radius);
			CRect pieRect (pieLeft, 
					centerYPie - radius,
					pieRight, 
					centerYPie + radius);
			int centerXPie = pieLeft + radius;

			//plot series label
//			pDC->TextOut(centerXPie - ((tmpSeries->GetLabel().GetLength() * 8) / 2),
//						centerYPie + radius + 15, tmpSeries->GetLabel());
//			lastXLocation = pieLeft;
//			lastYLocation = centerYPie;
			
			dataSum = 0;
			 
			CPoint pCenter = pieRect.CenterPoint();
			//CPoint vCP1 = p1-pCenter;
			//CPoint vCP2 = p2-pCenter;
			CPoint vCX = pt-pCenter;

			CPoint vCH = CPoint(pieRect.left,pieRect.CenterPoint().y)-pCenter;
			CPoint vCV = CPoint(pieRect.CenterPoint().x,pieRect.bottom) -pCenter;

			//double nvCP1 = sqrt(vCP1.x*vCP1.x + vCP1.y*vCP1.y);
			//double nvCP2 = sqrt(vCP2.x*vCP2.x + vCP2.y*vCP2.y);
			double nvCX = sqrt(vCX.x*vCX.x + vCX.y*vCX.y);

			double nvCH = sqrt(vCH.x*vCH.x + vCH.y*vCH.y);
			double nvCV = sqrt(vCV.x*vCV.x + vCV.y*vCV.y);

			double dotH= vCH.x*vCX.x + vCH.y*vCX.y;
			dotH /= nvCH*nvCX;
			double dotV= vCX.x*vCV.x + vCX.y*vCV.y;
			dotV /= nvCX*nvCV;

			double cH = acos(dotH)*360/(2*3.1415926535897932);
			double cV = acos(dotV)*360/(2*3.1415926535897932);
			if (dotV < 0) cH = 360-cH;

			for(s = 0; s < tmpSeries->GetNumData(); s++)
			{
				if(tmpSeries->GetData(s) > 0)
				{
					dataSum += tmpSeries->GetData(s);
					percent = (dataSum * 100) / totalSum;
					degrees = (int)((360 * percent) / 100);

					CRect mrect = pieRect;
					CRgn mPieRgn;
					mrect.NormalizeRect();
					mPieRgn.CreateEllipticRgnIndirect(mrect);
					//BOOL bTmp = mrect.PtInRect(pt);
					BOOL bTmp = mPieRgn.PtInRegion(pt);
					if (bTmp && cH<=degrees)
					{
						nSelSerie = nSerie = x-1;
						nSelData = nData = s;
						return TRUE;
					}
	
//					lastXLocation = newXLocation;
//					lastYLocation = newYLocation;
				}
			}
			
		}
	}

	return bRedraw;
}

void CGraph::SetPrediction(int nSerie,int nData,double pred)
{
	POSITION pos = graphSeries->GetHeadPosition();

	for(int x = 0; x < graphSeries->GetCount(); x++)
	{
		CGraphSeries *tmpSeries = (CGraphSeries*)graphSeries->GetNext(pos);
		if (tmpSeries)
		{
			if (nSerie==x)
				tmpSeries->SetPrediction(nData,pred);
		}
	}
}


double CGraph::GetPrediction(int nSerie,int nData)
{
	POSITION pos = graphSeries->GetHeadPosition();

	for(int x = 0; x < graphSeries->GetCount(); x++)
	{
		CGraphSeries *tmpSeries = (CGraphSeries*)graphSeries->GetNext(pos);
		if (tmpSeries)
		{
			if (nSerie==x)
				return tmpSeries->GetPrediction(nData);
		}
	}
	return -1e-122;
}

BOOL CGraph::ResetSelection(int nSerie/*=-1*/,int nData/*=-1*/,BOOL bReset/*=TRUE*/)
{ 
	//BOOL bRes= (nSelSerie!=-1 || nSelData!=-1);
	BOOL bRes= (nSerie!=-1 || nData!=-1);
	m_bIsSelected = (nSerie!=-1 && nData!=-1);


	POSITION pos = graphSeries->GetHeadPosition();

	for(int x = 0; x < graphSeries->GetCount(); x++)
	{
		CGraphSeries *tmpSeries = (CGraphSeries*)graphSeries->GetNext(pos);
		if (tmpSeries)
		{
			if (nSerie==x)
				tmpSeries->SetSelected(nData,TRUE);
			else if (bReset)
				tmpSeries->SetSelected();
		}
	}
	int nSelSerie = nSerie;
	int nSelData = nData;
	return bRes;
};
