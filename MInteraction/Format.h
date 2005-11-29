//////////////////////////////////////////////////////////////////////
/// @file Format.h
/// @brief Interface for the CFormat and inherited classes
///
/// asasasas
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FORMAT_H__E3579F21_3594_11D5_A2FA_00D0B71C8709__INCLUDED_)
#define AFX_FORMAT_H__E3579F21_3594_11D5_A2FA_00D0B71C8709__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Animation\AnimObject.h"
#include "FormatPage.h"

class COptionTree;
class COptionTreeItemEdit;
class COptionTreeItemSpinner;
class COptionTreeItemColor;
class COptionTreeItemCheckBox;
class COptionTreeItemComboBox;
class COptionTreeItemRadio;

//////////////////////////////////////////////////////////////////////
/// Generic class containing the information related to the format of an External Representation.
///
/// Each inherited class represents the format for one of various types 
/// of External Representations supported by DEMIST (see CExternRepUnit):
///		- CFormatTimer for the Timer ER
///		- CFormatAnimation for the Animation ER
///		- CFormatChart for the Pie and Bar Chart ERs
///		- CFormatGraph for the X-Time Graph ER
///		- CFormatPhasePlot for the XY Graph ER
///		- CFormatText for the Text ER
///	
/// This class also act as a base for defining the Translation level associated
/// with the ER:
///		- CFormatTranslation
///
/// Basically, each of the inherited contains the data member used to 
/// store the parameters and a pointer to the interface class (CERFormatPage)
/// that will appear in the ER's configuration dialog box (CDefineERPropSheet)
//////////////////////////////////////////////////////////////////////
class CFormat : public CObject  
{
	DECLARE_SERIAL(CFormat)
protected:
	CFormatPage*	m_pPage;		///< Pointer to the dialog interface used to configurate this ER's format.
public:
	int				m_nMaxTime;		///< Number of data samples
	BOOL			m_bready;		///< TRUE if the format interface is ready to be used, FALSE otherwise.

	CFormat();
	CFormat(const CFormat& mFor);
	virtual ~CFormat();
	virtual CFormat* Clone();

	virtual CPropertyPage *GetPage();
	virtual	void UpdatePageContent(COptionTree *m_otTree);
	virtual void UpdateFormat(BOOL bOK=TRUE);

	void	Serialize( CArchive& ar );
};

typedef CTypedPtrArray<CObArray,CFormat*>		CxFormat;	///< A list of CFormat instances, used to configurate an External Representation.

//////////////////////////////////////////////////////////////////////
/// Format for the Timer ER.
//////////////////////////////////////////////////////////////////////
class CFormatTimer : public CFormat  
{
	DECLARE_SERIAL(CFormatTimer)
public:
	CFormatTimer();
	CFormatTimer(const CFormatTimer& mFor);
	virtual ~CFormatTimer();
	virtual CFormat* Clone();

	void	Serialize( CArchive& ar );
	virtual CPropertyPage *GetPage();
	virtual void UpdateFormat(BOOL bOK=TRUE);
};

//////////////////////////////////////////////////////////////////////
/// Format for the Table ER.
//////////////////////////////////////////////////////////////////////
class CFormatTable : public CFormat  
{
	DECLARE_SERIAL(CFormatTable)
public:
	int			m_nPrecision;		///< Number of decimal places for the values
	int			m_nTimeFrom;		///< Lower bound for diaplyed values
	int			m_nTimeTo;			///< Upper bound for displayed values
	int			m_nTimeStep;		///< increment step for displayed values
	BOOL		m_bNeg;				///< Change the text color for negative values
	BOOL		m_bMin;				///< Change the background color for local minima
	BOOL		m_bMax;				///< Change the background color for local maxima
	COLORREF	m_clrMax,			///< Color for local maxima
				m_clrMin,			///< Color for local minima
				m_clrNeg;			///< Color for negative value
private:
	/// @name Validation elements
	/// Pointer to interface elements (for validating changes)
	//@{
	COptionTreeItemEdit*		otiEditFrom;
	COptionTreeItemEdit*		otiEditTo;
	COptionTreeItemEdit*		otiEditStep;
	COptionTreeItemSpinner*		otiSpinPrec;
	COptionTreeItemColor*		otiClrMax;
	COptionTreeItemColor*		otiClrMin;
	COptionTreeItemColor*		otiClrNeg;
	COptionTreeItemCheckBox*	otiChkMax;
	COptionTreeItemCheckBox*	otiChkMin;
	COptionTreeItemCheckBox*	otiChkNeg;
	//@}

public:
	CFormatTable();
	CFormatTable(const CFormatTable& mFor);
	virtual ~CFormatTable();
	virtual CFormat* Clone();

	CString GetFormatString();
	void	Serialize( CArchive& ar );
	virtual	void UpdatePageContent(COptionTree *m_otTree);
	virtual CPropertyPage *GetPage();
	virtual void UpdateFormat(BOOL bOK=TRUE);
};

//////////////////////////////////////////////////////////////////////
/// Format for the Animation ER.
//////////////////////////////////////////////////////////////////////
class CFormatAnimation : public CFormat  
{
	DECLARE_SERIAL(CFormatAnimation)
public:
	CxAnimObjectList	m_cObjSet;		///< List of Animation Components
	int					m_nAnimCat;		///< ????
	BOOL				m_bFitToView;	///< Resize the component to fit the whole view

private:
	/// @name Validation elements
	/// Pointer to interface elements (for validating changes)
	//@{
	COptionTreeItemRadio*		otiRadio;
	COptionTreeItemCheckBox*	otiChkFit;
	//@}

public:
	CFormatAnimation();
	CFormatAnimation(const CFormatAnimation& mFor);
	virtual ~CFormatAnimation();
	virtual CFormat* Clone();

	void	Serialize( CArchive& ar );
	virtual CPropertyPage *GetPage();
	virtual	void UpdatePageContent(COptionTree *m_otTree);
	virtual void UpdateFormat(BOOL bOK=TRUE);

	int		AddObject(CAnimObject* pObj);
	int		RemoveObject(CAnimObject* pObj);
	int		CopyObjects(CxAnimObjectList& myList);
	int		PasteObjects(CxAnimObjectList& myList);
	int		SetupVisualList(CxAnimObjectList& myList);
};

//////////////////////////////////////////////////////////////////////
/// Format for the X-Time Graph ER.
//////////////////////////////////////////////////////////////////////
class CFormatGraph : public CFormat  
{
	DECLARE_SERIAL(CFormatGraph)
public:
	BOOL	m_bAutoScale;		///< Automatic scaling
	BOOL	m_bShowAxes;		///< Show the axes
	BOOL	m_bShowLegend;		///< Show the legend
	int		m_nXmax;			///< Upper bound on the X-axe
	int 	m_nXmin;			///< Lower bound on the X-axe
	double	m_nYmax;			///< Upper bound on the Y-axe
	double	m_nYmin;			///< Lower bound on the Y-axe

private:
	/// @name Validation elements
	/// Pointer to interface elements (for validating changes)
	//@{
	COptionTreeItemEdit*		otiEditXMin;
	COptionTreeItemEdit*		otiEditXMax;
	COptionTreeItemEdit*		otiEditYMin;
	COptionTreeItemEdit*		otiEditYMax;
	COptionTreeItemCheckBox*	otiChkAxis;
	COptionTreeItemCheckBox*	otiChkLeg;
	COptionTreeItemCheckBox*	otiChkScale;
	COptionTreeItemComboBox*	oitCbbType;
	//@}
public:
	CFormatGraph();
	CFormatGraph(const CFormatGraph& mFor);
	virtual ~CFormatGraph();
	virtual CFormat* Clone();

	void	Serialize( CArchive& ar );
	virtual CPropertyPage *GetPage();
	virtual	void UpdatePageContent(COptionTree *m_otTree);
	virtual void UpdateFormat(BOOL bOK=TRUE);
};

//////////////////////////////////////////////////////////////////////
/// Format for the Phaseplot (XY-Graph) ER.
//////////////////////////////////////////////////////////////////////
class CFormatPhasePlot : public CFormat  
{
	DECLARE_SERIAL(CFormatPhasePlot)
public:
	BOOL	m_bShowAxe;			///< Show the axes in the graph
	BOOL	m_bShowCurrTime;	///< Show the current time in the graph
	BOOL	m_bShowIsocline;	///< Show the isoclines (if defined)
	BOOL	m_bShowLegend;		///< Show the legend
	BOOL	m_bShowStab;		///< Show the stability point (if defined)
	int		m_nScale;			///< Define the nature of the scale (linear or logarithm) on both axes

	int		m_nIsoX0;			///< Index of the aggregate used for the first isocline (on the X-axis)
	int		m_nIsoXN;			///< Index of the aggregate used for the second isocline (on the X-axis)
	int		m_nIsoY0;			///< Index of the aggregate used for the first isocline (on the Y-axis)
	int		m_nIsoYN;			///< Index of the aggregate used for the second isocline (on the Y-axis)
private:
	/// @name Validation elements
	/// Pointer to interface elements (for validating changes)
	//@{
	COptionTreeItemEdit*		otiEditXMin;
	COptionTreeItemEdit*		otiEditXMax;
	COptionTreeItemEdit*		otiEditYMin;
	COptionTreeItemEdit*		otiEditYMax;
	COptionTreeItemCheckBox*	otiChkAxis;
	COptionTreeItemCheckBox*	otiChkLeg;
	COptionTreeItemCheckBox*	otiChkScale;
	COptionTreeItemComboBox*	oitCbbType;
	//@}
public:

	CFormatPhasePlot();
	CFormatPhasePlot(const CFormatPhasePlot& mFor);
	virtual ~CFormatPhasePlot();
	virtual CFormat* Clone();

	void	Serialize( CArchive& ar );
	virtual CPropertyPage *GetPage();
	virtual	void UpdatePageContent(COptionTree *m_otTree);
	virtual void UpdateFormat(BOOL bOK=TRUE);
};

//////////////////////////////////////////////////////////////////////
/// Format for the Static Text ER.
//////////////////////////////////////////////////////////////////////
class CFormatText : public CFormat  
{
	DECLARE_SERIAL(CFormatText)
public:
	int			m_nDisplayMode;	///< Display mode for the equations
	COLORREF	m_clrParam,		///< Color used for the parameters
				m_clrVar,		///< Color used for the variables
				m_clrCst;		///< Color used for the numerical constants

private:

public:
	CFormatText();
	CFormatText(const CFormatText& mFor);
	virtual ~CFormatText();
	virtual CFormat* Clone();

	void	Serialize( CArchive& ar );
	virtual CPropertyPage *GetPage();
	virtual	void UpdatePageContent(COptionTree *m_otTree);
	virtual void UpdateFormat(BOOL bOK=TRUE);
};

//////////////////////////////////////////////////////////////////////
/// Format for the Pie Chart / Bar Chart ER.
//////////////////////////////////////////////////////////////////////
class CFormatChart : public CFormat  
{
	DECLARE_SERIAL(CFormatChart)
public:
	enum {CHART_BAR,CHART_PIE};
	int		m_nChartKind;		///< Type of chart (0 for a Pie Chart,
								///< 1 for a Bar Chart)

private:
	/// @name Validation elements
	/// Pointer to interface elements (for validating changes)
	//@{
	COptionTreeItemCheckBox*	otiChkLeg;
	//@}
public:
	CFormatChart();
	CFormatChart(const CFormatChart& mFor);
	virtual ~CFormatChart();
	virtual CFormat* Clone();

	void	Serialize( CArchive& ar );
	virtual CPropertyPage *GetPage();
	virtual	void UpdatePageContent(COptionTree *m_otTree);
	virtual void UpdateFormat(BOOL bOK=TRUE);
};


//////////////////////////////////////////////////////////////////////
/// Translation level for the External Representation.
//////////////////////////////////////////////////////////////////////
class CFormatTranslation : public CFormat  
{
	DECLARE_SERIAL(CFormatTranslation)
public:
	int		m_nTranslation;		///< Level of Translation for this ER (between 0 and 4)

	CFormatTranslation();
	CFormatTranslation(const CFormatTranslation& mFor);
	virtual ~CFormatTranslation();
	virtual CFormat* Clone();

	void	Serialize( CArchive& ar );
	virtual CPropertyPage *GetPage();
	virtual void UpdateFormat(BOOL bOK=TRUE);
};

#endif // !defined(AFX_FORMAT_H__E3579F21_3594_11D5_A2FA_00D0B71C8709__INCLUDED_)
