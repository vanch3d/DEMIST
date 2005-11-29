//////////////////////////////////////////////////////////////////////
/// @file UserOutput.h
/// @brief Interface for the CUserOutput class.
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_USEROUTPUT_H__D0ED59E0_A054_11D4_A2FA_00D0B71C8709__INCLUDED_)
#define AFX_USEROUTPUT_H__D0ED59E0_A054_11D4_A2FA_00D0B71C8709__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////////////////////
/// Stores information about a data-point of the simulation targetted by a user's intervention.
///
/// This class is used during an Action or a Prediction breakpoint in order to broacast this
/// user's intervention to all ERs and to the mathematical model.
/// Information stored include the nature of the intervention (#m_nType, see the #TSBreakPt enum type),
/// the translation level of the target ER (#m_nTLevel, see the #TSTranslt enum type), the time (#m_nTime)
/// at which the intervention took place, the outcome (#m_nOutcome) and the experimental set (#m_nExpSet)
/// corresponding to the data-point and the new and old values of the data-point (resp. #m_dValue and #m_dOldValue).
/// The #m_bUpdateNow data tells the framework if the modification has to be immediatly 
/// propagated to update the mathematical model (TRUE) or just kept at interface level (FALSE).
/// This allows to speed up the interface, i.e. just a visual feedback when dragging the data-point, 
/// complete model recalculation when releasing it.
//////////////////////////////////////////////////////////////////////
class CUserData : public CObject  
{
public:
	CUserData();
	virtual ~CUserData();

	void Serialize( CArchive& ar );
    void operator=(const CUserData& cell);

public:
	int		m_nType;		///< Type of the user's intervention (BREAKP_ACTION or BREAKP_HYPOT).
	int		m_nTLevel;		///< Translation level of the ER in which the user's intervention occurred (TRANS_INDEP, TRANS_MAPRELAT or TRANS_DYNAL).
	int		m_nTime;		///< Time-step at which the user's intervention occurred.
	int		m_nExpSet;		///< Zero-based index of the Experimental Set associated with the manipulated data-point.
	int		m_nOutcome;		///< Zero-based index of the aggregate associated with the manipulated data-point.
	double	m_dValue;		///< New value of the data-point after user's intervention.
	double	m_dOldValue;	///< Old value of the data-point before user's intervention.
	BOOL	m_bUpdateNow;	///< TRUE if the modification has to be notified to the ERs immediatly.

};

typedef CMap<unsigned long,unsigned long,CUserData,CUserData&> CxUserData;	///< Map of User's data modification


//////////////////////////////////////////////////////////////////////
/// Stores information about one user's interventions on the simulation.
///
/// During a user's intervention, more than one data-point could be modified at the same time (e.g. by
/// dragging a data-point in the phaseplot or several data-point in a table) so this class stores all the 
/// data-points involved in a single intervention at a given time (#m_nTime). 
/// This intervention wil be stored in the target ER, in order to keep a trace of
/// all the interventions during the exploration of the process (see CExternRepUnit::m_cUserInput).
/// The map used to store user's interventions will use a combination of the Extperimental Set and the 
/// Outcome of the data-point as a retrieval key (see the #GetAt() and #SetAt() functions).
//////////////////////////////////////////////////////////////////////
class CUserOutput : public CObject  
{
public:
	//typedef CMap<unsigned long,unsigned long,CUserData,CUserData&> CTUserData;

	int			m_nTime;			///< Time-step at which the user is currently acting on the data-points.
	CxUserData	m_lUserInput;		///< List of all the user's interventions at the given time-step. 

	CUserOutput(int nTime=0);
	virtual	~CUserOutput();

    void operator=(const CUserOutput& cell);

	BOOL GetAt(int nExpSet,int nData,CUserData& pData);
	BOOL SetAt(int nExpSet,int nData,CUserData& pData);
	void RemoveAll();

	void Serialize( CArchive& ar );
};

typedef CMap<int,int,CUserOutput,CUserOutput&> CxUserOutput;	///< Map of User's production during the simulation


#endif // !defined(AFX_USEROUTPUT_H__D0ED59E0_A054_11D4_A2FA_00D0B71C8709__INCLUDED_)
