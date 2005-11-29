// Pref.h: interface for the CPref class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PREF_H__211CAA47_5C9B_11D4_A2FA_00D0B71C8709__INCLUDED_)
#define AFX_PREF_H__211CAA47_5C9B_11D4_A2FA_00D0B71C8709__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////////////////////
/// Abstract class acting as a repository for all the DEMIST's preferences.
///
/// 
//////////////////////////////////////////////////////////////////////
class CPref  
{
private :
	virtual void dummy() = 0;

	//////////////////////////////////////////////////////////////////////
	/// Part of the Preferences of DEMIST, dealing with the learner's traces.
	//////////////////////////////////////////////////////////////////////
	struct CPrefTrace {
		BOOL	g_bSwitchER;		///< ????????
		BOOL	g_bOpenER;			///< ????????
		BOOL	g_bCloseER;			///< ????????
		BOOL	g_bRunSimul;		///< ????????
		BOOL	g_bPauseSimul;		///< ????????
		BOOL	g_bReviewSimul;		///< ????????
	};

public:
	CPref();
	virtual ~CPref();

public:
	static  CPrefTrace		CTrace;			///< ????????

	static	int				g_nMode;			///< Switch between Learning Mode (1) and Authoring Mode (0)
	static	int				g_nSpeedNorm;		///< Speed used in Learning Mode
	static	int				g_nSpeedAcc;		///< Speed used in Authoring Mode

	static  CString			g_sPassWd;		///< Password used to switch to Authoring Mode
	static	CString			g_sLastUser;		///< Name of the last user (to use in the ID dialog)
	static	CString			g_sTraceRep;		///< Directory used to save the user's traces

	static	CStringArray	g_cUserList;		///< List of all the previous users
	static	CStringArray	g_cCatList;		///< List of all ER's category (used in specifying the ER)
	static	CStringArray	g_cUnitList;		//


	static	COLORREF		g_crPast;			///< Background color used to represent the past in the relevant ERs
	static	COLORREF		g_crFuture;		///< Background color used to represent the past in the relevant ERs


public :
	static BOOL ReadPref(const char *fn=0);
	static BOOL WritePref(const char *fn=0);
	static BOOL AddUserInList(CString mstr);
};

#endif // !defined(AFX_PREF_H__211CAA47_5C9B_11D4_A2FA_00D0B71C8709__INCLUDED_)
