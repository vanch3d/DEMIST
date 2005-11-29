//////////////////////////////////////////////////////////////////////
/// @file Model.h
/// @brief Interfaces for the Simulation Model classes.
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MODEL_H__C9C7A02A_0AD2_11D4_A2F6_00D0B71C8709__INCLUDED_)
#define AFX_MODEL_H__C9C7A02A_0AD2_11D4_A2F6_00D0B71C8709__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMdEquation;
class CModelObj;
class CMdEquation;
class CLearningUnit;
class CModelVar;
class CModelPar;
class CMdExpertSet;
class CMdData;

typedef CArray<int,int>							CxIntList;		///< Array of integer
typedef CArray<double,double>					CxValueSet;		///< Array of double
typedef CTypedPtrArray<CObArray,CxValueSet*>	CxValueArray;	///< 2D-Array of double
typedef CTypedPtrArray<CObArray,CMdEquation*>	CxModelEqSet;	///< Array of Simulation Model's equations
typedef CTypedPtrArray<CObArray,CModelVar*>		CxModelVarSet;	///< Array of Simulation Model's variables
typedef CTypedPtrArray<CObArray,CModelPar*>		CxModelParSet;	///< Array of Simulation Model's parameters
typedef CTypedPtrArray<CObArray,CMdExpertSet*>	CxExpSet;		///< Array of Simulation Model's parameters
typedef CTypedPtrArray<CObArray,CMdData*>		CxModelData;	///< Array of Simulation Model's parameters





//typedef CArray<double,double> CDoubleCont;				

#define AGG_SIZE 20
#define VAR_SIZE 30

//////////////////////////////////////////////////////////////////////
/// Generic class used to store the data-points that will be displayed in an External Representation.
///
/// Each instance of CMdData stores the data-points of one entity extracted from the
/// Simulation Model (i.e. variable, parameters, time, etc.) in the #m_Values
/// attribute (a 2-dimentional arrays of values, each "column" representing an Experimental Set).
/// These data-points are generated according to the model (referenced by #m_pEquation), 
/// the duration of the simulation and the initial conditions expressed by the Experimental Sets.
/// CMdData also stored information about the minima and maxima (resp. #m_dataMin 
/// and #m_dataMax) of each Experimental Set. 
///
/// This class has two derived classes, CMdVarData and CMdParData, whose difference are only 
/// in the CMdData::GetDataInfo() function, looking for the relevant entity repsectively in the Variables list
/// (CMdEquation::m_cVarSet) or in the Parameters list (CMdEquation::m_cParSet) of the Mathematical Model.
//////////////////////////////////////////////////////////////////////
class CMdData : public CObject  
{
public:
	DECLARE_SERIAL(CMdData)
	CMdData(CMdEquation *pEqu=NULL,int idx=0);
	CMdData(const CMdData* md);
	virtual	~CMdData();
	virtual CMdData* Clone();

	void			Serialize( CArchive& ar );

	virtual int		GetExpSetNb();
	virtual void	ClearAll();
	virtual double	GetAt(int idx,int exps=0);
	virtual int		SetAt(double dt,int idx,int exps=0);
	virtual CString	GetDataName(BOOL bAbbrev);
	
	CMdEquation*		GetEquation();
	virtual CModelObj*	GetDataInfo();
	int					GetDataRef();

	virtual void	ResetMinMax(int exps=0);
	bool	GetMinMax(double *mn, double *mx,int exps=0);
	virtual void GetLocalMinMax(CxIntList &minList,CxIntList &maxList,int exps=0);

protected:
	CMdEquation*	m_pEquation;	///< Pointer the the associated equation
	int				m_dataRef;		///< Index of associated entity (var. or param.)
protected:
	CxValueSet		m_dataMin,		///< List of the minimum values for each Experimental Set.
					m_dataMax;		///< List of the maximum values for each Experimental Set.
	CxValueArray	m_Values;		///< List of generated data-points for each Experimental Set.
};

//////////////////////////////////////////////////////////////////////
/// Class storing the data-points associated with a variable or an aggregate.
//////////////////////////////////////////////////////////////////////
class CMdVarData : public CMdData  
{
public:
	DECLARE_SERIAL(CMdVarData)
	CMdVarData(CMdEquation *pEqu=NULL,int idx=0);
	CMdVarData(const CMdVarData* md):  CMdData(md) {};

//	virtual ~CMdVarData();
	virtual CMdData*	Clone();

	//void			Serialize( CArchive& ar );

	//virtual int		GetExpSetNb();
	//virtual void	ClearAll();
	//virtual double	GetAt(int idx,int exps=0);
	//virtual int		SetAt(double dt,int idx,int exps=0);
	//virtual CString	GetDataName(BOOL bAbbrev);
	virtual CModelObj*	GetDataInfo();
	//virtual void	ResetMinMax(int exps=0);
	//virtual void GetLocalMinMax(CxIntList &minlist,CxIntList &maxlist,int exps=0);

//	virtual CModelObj*	GetDataInfo();
protected:
	//CxValueArray		m_Values;		///< List of displayed data
};

//////////////////////////////////////////////////////////////////////
/// Class storing the data-points associated with a parameter or an initial value.
//////////////////////////////////////////////////////////////////////
class CMdParData : public CMdData  
{
public:
	DECLARE_SERIAL(CMdParData)
	CMdParData(CMdEquation *pEqu=NULL,int idx=0);
//	CMdParData(const CMdParData* md);
	CMdParData(const CMdParData* md):  CMdData(md) {};

//	virtual ~CMdParData();
	virtual CMdData*	Clone();

//	virtual int		GetExpSetNb();
//	void			Serialize( CArchive& ar );

//	virtual void			ClearAll();
//	virtual double	GetAt(int idx,int exps=0);
//	virtual int		SetAt(double dt,int idx,int exps=0);
//	virtual CString	GetDataName(BOOL bAbbrev);
	virtual CModelObj*	GetDataInfo();
//	virtual void	ResetMinMax(int exps=0);
//	virtual void GetLocalMinMax(CIntList &minlist,CIntList &maxlist,int exps=0);

//	virtual CModelObj*	GetDataInfo();
protected:
//	CValueSet		m_Values;		// List of displayed data
};

//////////////////////////////////////////////////////////////////////
/// A generic class implementing an entity extracted from the Mathematical Model.
///
/// This class contains the information used to configure these entities for
/// an usage in the External Representation (i.e. name, short description, default
/// color, icon, etc.) as well as its mathematical definition.
/// The derived classes represent the 5 types of entity identified by DEMIST:
///		- CModelPar for a parameter or an intial value
///		- CModelVar for a variable or an aggregate
///		- CModeTime for the time
///
//////////////////////////////////////////////////////////////////////
class CModelObj : public CObject  
{
public:
	DECLARE_SERIAL(CModelObj)
	CModelObj();
	CModelObj(LPCTSTR name,LPCTSTR desc="",LPCTSTR unit="");
	CModelObj(const CModelObj* pObj);
	virtual ~CModelObj();

	virtual CModelObj* Clone();

	void			Serialize( CArchive& ar );

	void			SetName(CString name)	{ m_sName = name;};
	CString			GetName() { return m_sName;};
	void			SetUnit(CString unit)	{ m_sUnit = unit;};
	CString			GetUnit() { return m_sUnit;};
	void			SetDesc(CString desc)	{ m_sDesc = desc;}
	CString			GetDesc() { return m_sDesc;};
	void			SetDef(CString desc)	{ m_sDef = desc;}
	CString			GetDef() { return m_sDef;};
	void			SetAbbrev(CString desc)	{ m_sAbbrev = desc;}
	CString			GetAbbrev() { return m_sAbbrev;};
	void			SetColor(COLORREF cr) { m_crColor = cr;};
	COLORREF		GetColor() { return m_crColor;};
	int				GetIcon() { return m_nIcon;};

	CString			GetFormatName(BOOL bLabel);
protected:
	int				m_nIcon;		///< Index of the icon associated with the aggregate

	CString			m_sDef;			///< Mathematical definition of the aggregate
	CString			m_sName;		///< Name of the aggregate
	CString			m_sDesc;		///< Description of the aggregate
	CString			m_sAbbrev;		///< Abbreviation of the aggregate
	CString			m_sUnit;		///< Unit of Measure associated with the aggregate
	COLORREF		m_crColor;		///< Default color of the aggregate

	CPropertyPage	*m_pProp;		// OBSOLETE: Properties of Object
public:
	CWordArray		m_cDepObj;		///< Dependency graoh of the aggregate (for calculation)
};

//////////////////////////////////////////////////////////////////////
/// Class implementing a parameter or an intial value extracted 
/// from a Mathematical Model.
///
//////////////////////////////////////////////////////////////////////
class CModelPar : public CModelObj  
{
public:
	DECLARE_SERIAL(CModelPar)
	CModelPar();
	CModelPar(LPCTSTR name,double mv=0,LPCTSTR desc="",LPCTSTR unit="");
	virtual ~CModelPar();
	CModelPar(const CModelPar* pObj);
	virtual CModelObj* Clone();

	void			Serialize( CArchive& ar );

	void			SetInfos(double v,double mn, double mx);
	void			SetInitValue(int iv);
	int				GetInitValue()	{return m_InitValue;};
	double			GetValue() {return m_Value;};
	void			SetValue(double val) {m_Value=val;};
	double			GetMin() {return m_lMin;};
	void			SetMin(double min) {m_lMin = min;};
	double			GetMax() {return m_lMax;};
	void			SetMax(double max) {m_lMax = max;};

	int				m_InitValue;	///< Is an initial value (of a variable)
protected:
	double			m_Value;		///< Default value of the parameter
	double			m_lMin,			///< Minimum possible value for the parameter
					m_lMax;			///< Maximum possible value for the parameter
};

//////////////////////////////////////////////////////////////////////
/// Class implementing a variable or an aggregate extracted from 
/// a Mathematical Model.
///
//////////////////////////////////////////////////////////////////////
class CModelVar : public CModelObj  
{
public:
	DECLARE_SERIAL(CModelVar)
	CModelVar();
	CModelVar(LPCTSTR name,LPCTSTR desc="",LPCTSTR unit="");
	virtual ~CModelVar();
	CModelVar(const CModelVar* pObj);
	virtual CModelObj* Clone();

	void			Serialize( CArchive& ar );

	void			SetInitValue(int iv);
	int				GetInitValue()	{return m_InitValue;};
	CString			GetEquation() { return 	m_strEquation;};
	void			SetEquation(CString mstr) {m_strEquation = mstr;};
protected:
	int				m_InitValue;	///< Has initial value (as a parameter)
	CString			m_strEquation;	///< Equation of the var./aggr.
};


//////////////////////////////////////////////////////////////////////
/// Class implementing the special variable associated with time.
///
//////////////////////////////////////////////////////////////////////
class CModelTime : public CModelVar  
{
public:
	DECLARE_SERIAL(CModelTime)
	CModelTime();
	CModelTime(LPCTSTR name,LPCTSTR desc="",LPCTSTR unit="");
	virtual ~CModelTime();
	void			Serialize( CArchive& ar );
};


//////////////////////////////////////////////////////////////////////
/// Class implementing an Experimental Set, a set of values defining one initial condition 
/// for the simulation.
///
/// An experimental set assigns different values to the initial conditions 
/// (i.e. parameters and initial values) of a Mathematical Model in order to 
/// run simultaneously different simulations of the same process.
/// This different Experimental Sets are stored in the Learning Unit (see
/// CLearningUnit::m_cExpSet) and used by the Mathematical Model (CMdEquation) to generate
/// the various data-points.
//////////////////////////////////////////////////////////////////////
class CMdExpertSet : public CObject  
{
public:
	DECLARE_SERIAL(CMdExpertSet)
	CMdExpertSet();
	virtual ~CMdExpertSet();
	virtual CMdExpertSet* Clone();

	void			Serialize( CArchive& ar );

	int				AddValue(double val);
	double			GetValue(int idx);
	void			SetValue(int idx,double val);
	CString			GetName() {return m_sName;};
	void			SetName(CString tt) {m_sName=tt;};
	int				GetSize() { return m_cParamSet.GetSize();};

protected:
	CString			m_sName;			///< Name of the Experimental Set
	CxValueSet		m_cParamSet;		///< Values of the initial conditions (Initial Value & Parameters)
	CxValueSet		m_cParamDefaultSet;	///< Default Values of initial conditions
};


//////////////////////////////////////////////////////////////////////
/// Provides the functionality of the Simulation Model.
///
/// CSimModel contains a list of all the Mathematical Models (CMdEquation) defined by the 
/// user in the current document. The order in which these Mathematical Models appear in the 
/// list is important, as any reference made to them (e.g. in the Learning units, see 
/// CLearningUnit) is in term of their position in the list.
/// An instance of this class is stored as an attribute of the document (see CSimDoc::m_pSimul). 
//////////////////////////////////////////////////////////////////////
class CSimModel : public CObject  
{
public:
	DECLARE_SERIAL(CSimModel)
	CSimModel();
	virtual ~CSimModel();

	void Serialize( CArchive& ar );

	CMdEquation*	GetEquation(int idx=0);
	int				GetEquationNb();
	CString			GetSimulationName();

	virtual void	OnNewModel() {};
	virtual void	OnInitiaUpdate() {};
//	virtual bool	OnUpdateAllModel();
//	virtual bool	OnUpdateModel(int nExpS=0);

	int				m_iMaxTime;			///< OBSOLETE: Number of time-samples
	CString			m_sName;			///< Name of the Simulation Model
	CxModelEqSet	m_cEquSet;			///< List of Mathematical Models

	//CTypedPtrArray<CObArray,CMdEquation*>	m_cEquSet;	///< List of equations
};

//////////////////////////////////////////////////////////////////////
/// Class implementing a Mathematical Model, a set of mathematical equations used to define
/// the simulation. 
///
/// This class is the central part of DEMIST's simulation engine. It contains a set
/// of mathematical equations (discrete or differential equations) that are used to
/// identify the entities presented to the user (i.e. Parameters, Initial Values, 
/// Variables and Aggregates) and to calculate the value of these entities during the
/// simulation. 
/// The entities are stored in two different lists: #m_cVarSet for the variables and 
/// aggregates, #m_cParSet for the parameters and initial values).
/// Calculation of the values of each entity across the simulation is ensured by a 
/// couple of functions (see #OnUpdate() and #OnUpdateFrom()).
//////////////////////////////////////////////////////////////////////
class CMdEquation : public CObject  
{
	DECLARE_SERIAL(CMdEquation)
public:
	/// Position of the label wrt the node (in the flowchart)
	enum TNodeAlign	{
			TT_TOP,		///< The label is positioned above the node
			TT_BOTTOM	///< The label is positioned below the node
		};
	/// Types of node (in the flowchart)
	enum TNodeType	{
			TT_VAR,		///< The node is a variable
			TT_PAR,		///< The node is a parameter
			TT_AGG,		///< The node is an aggregate
			TT_CHANGE	///< The node is a reservoir
		};
	/// Types of link (in the flowchart)
	enum TNodeLink	{
			TT_LINE,	///< The link is a straight line
			TT_CURVE	///< The link is a curved line
		};

	CMdEquation();
	CMdEquation(CMdEquation*pEq);

	virtual ~CMdEquation();

	virtual CMdEquation *Clone();

	void Serialize( CArchive& ar );

	//////////////////////////////////////////////////////////////////////
	/// Return the name of the Mathematical Model.
	//////////////////////////////////////////////////////////////////////
	CString			GetSimulationName() { return m_sName;};
	//////////////////////////////////////////////////////////////////////
	/// Change the name of the Mathematical Model.
	//////////////////////////////////////////////////////////////////////
	void			SetSimulationName(CString mstr) {m_sName = mstr;};

	CMdExpertSet*	NewExpertSet();

	virtual void	OnNewEquation(CSimModel *pModel);
	virtual void	OnUpdate(CLearningUnit *m_pLU,BOOL bInit);
	virtual void	OnUpdate(CLearningUnit *m_pLU,int nExpSet,BOOL bInit);
	virtual void	OnUpdateFrom(CLearningUnit *m_pLU,int nTime,BOOL bInit);
	virtual void	OnUpdateFrom(CLearningUnit *m_pLU,int nExpSet,int nTime,BOOL bInit);

	virtual	CRect   DrawConnector(CDC* pDC,int nb,CPoint ptStart[],int nType);
	virtual	CRect   DrawNode(CDC* pDC,CPoint mpt,CString nName,int bTop,int nType);
	virtual CRect   DrawFlowChart(CDC* pDC);

	virtual CString GetFormatedText(int nLevel=0);
	virtual CString GetFormatedEquation();

protected:
	CString			m_sName;			///< Name of the model
	CString			m_sText;			///< Textual representation of equation
	CStringArray	m_sEqu;				///< List of equations
	CStringArray	m_sEquDef;			///< List of equations' definition
public:
	int				m_nEquNb;			///< Number of equations in the model
	CxModelVarSet	m_cVarSet;			///< List of Variables of the model
	CxModelParSet	m_cParSet;			///< List of Parameters of the model

	//CTypedPtrArray<CObArray,CModelVar*>	m_cVarSet;	///< List of Variables of the model
	//CTypedPtrArray<CObArray,CModelPar*>	m_cParSet;	///< List of Parameters of the model
	//CString		m_sEquation;		// Mathematical representation of equation
	//CBitmap		m_bFunctDiag;		// Functional Diagram
	//int			m_nBitmapID;
};

#endif // !defined(AFX_MODEL_H__C9C7A02A_0AD2_11D4_A2F6_00D0B71C8709__INCLUDED_)
