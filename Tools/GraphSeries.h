#if !defined(AFX_GRAPHSERIES_H__A059628E_F6C8_4DA0_9E2F_820131D257E7__INCLUDED_)
#define AFX_GRAPHSERIES_H__A059628E_F6C8_4DA0_9E2F_820131D257E7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GraphSeries.h : header file
//

#define GRAPH_UNDEF -1e-11

/////////////////////////////////////////////////////////////////////////////
// CGraphSeries window

class CGraphData : public CStatic
{
// Construction
public:
	CGraphData();

// Attributes
public:
	int m_nGroup;
};


class CGraphSeries : public CStatic
{
// Construction
public:
	CGraphSeries();

// Attributes
public:
	int		m_nGroup;		// Goup Data ExpSet reference
	BOOL	m_bPred;		// Is a prediction graph

protected:
	CString					seriesLabel;	///< Title of the serie (Experimental Set).
	CArray<double,double>	dataValue;		///< List of values for every Outcomes added in the serie.
	CArray<double,double>	predValue;		///< List of prediction's values (if any) made on each Outcome.
	CArray<int,int>			dataRef;		///< List of the zero-based index of the serie's Outcomes.
	CArray<BOOL,BOOL>		dataPred;		///< For each Outcomes of the serie, TRUE if a prediction has been made, FALSE otherwise.
	CArray<BOOL,BOOL>		dataSel;		///< For each Outcomes of the serie, TRUE if the outcome is selected in the graph, FALSE otherwise.

	//int		m_nSelected;
	//NVL int dataValue[10];	//allows 10 data values in a group

// Operations
public:
	CGraphSeries *Clone();

	void SetGroupIdx(int idx) {m_nGroup = idx;};
	int  GetGroupIdx() {return m_nGroup;};
	int  GetNumData() {return dataValue.GetSize();}; // NVL
	void SetLabel(CString label);
	void SetData(int group, int idx, double dValue);
	int  GetDataRef(int group);
	CString GetLabel();
	double GetData(int group);
	// NVL
	void SetSelected(int group=-1,BOOL bSel=FALSE);
	BOOL GetSelected(int group);

	void ResetDataPred();
	void SetDataPred(int group, BOOL bPred);
	BOOL GetDataPred(int group);

	void SetPrediction(int group, double dValue);
	double GetPrediction(int group);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGraphSeries)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CGraphSeries();

	// Generated message map functions
protected:
	//{{AFX_MSG(CGraphSeries)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRAPHSERIES_H__A059628E_F6C8_4DA0_9E2F_820131D257E7__INCLUDED_)
