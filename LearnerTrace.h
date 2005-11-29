// LearnerTrace.h: interface for the CLearnerTrace class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LEARNERTRACE_H__343FB8E0_5E46_11D4_A2FA_00D0B71C8709__INCLUDED_)
#define AFX_LEARNERTRACE_H__343FB8E0_5E46_11D4_A2FA_00D0B71C8709__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <MInstruction/LearningUnit.h>

class CSimulDoc;
class CUserData;
class CTranslationInfo;
class CTimerBreakPoint;
class CLearningUnit;
class CMdEquation;
class CLearningUnit;
class COutcomes;


//////////////////////////////////////////////////////////////////////
/// An Abstract class tracing and saving all the user's interactions with DEMIST.
///
/// 
//////////////////////////////////////////////////////////////////////
class CTrace  
{
private :
	virtual void dummy() = 0;

public:
	static BOOL			g_bTrace;
	static CSimulDoc*	g_pDoc;


	CTrace();
	virtual ~CTrace();

	static BOOL OpenLogFile();
	static BOOL CloseLogFile();
	static BOOL	WriteString(CString mstr,BOOL bAddTime=TRUE,BOOL bErase=FALSE);

	static BOOL WriteDocHeader(CSimulDoc *pDoc);
	static BOOL WriteDocEnd(CString strDocName);
	static BOOL WriteLUHeader(CLearningUnit* pLU);

	static BOOL T_SWITCH(CWnd* pActivateWnd, CWnd* pDeactivateWnd);
	static BOOL T_CLOSE(CWnd* pActivateWnd);
	static BOOL T_OPEN(CWnd* pActivateWnd);
	static BOOL T_RUN(int nCurr,int nrun,int nMax);
	static BOOL T_REVIEW(int nTime,int nMode,int nrun,int nMax);
	static BOOL T_STOPAT(int nTime,int nrun,int nMax,BOOL bBP);
	static BOOL T_INITAT(int nTime,int nrun,int nMax);
	static BOOL T_GOTO(int nTime,int nrun,int nMax);

	static BOOL T_BREAKPOINT(CTimerBreakPoint *pData);

	static BOOL T_HYPOTHESIS(CView* pSender,CUserData *pData);
	static BOOL T_HYPO_TEST(CView* pSender,CUserData *pData);
	static BOOL T_ACTION(CView* pSender,CUserData *pData);
	static BOOL T_MAPRELATION(CSimulDoc *pDoc,CTranslationInfo *mTInfo);

	static BOOL T_STARTUNIT(int nUnit,CString mstr);
	static BOOL T_CHANGEINIT(CLearningUnit* pLU,CMdEquation* pEqu);

	static BOOL T_CHANGEEXPSET(CView* pSender,int nExpSet);
	static BOOL T_CHANGEOUTCOME(CView* pSender,CxOutcomes* pSet);

	static BOOL	ReadLog();

protected:
	static CString		g_strFileName;	///< Path and name of the file used to store interactions of the current user.
};

#endif // !defined(AFX_LEARNERTRACE_H__343FB8E0_5E46_11D4_A2FA_00D0B71C8709__INCLUDED_)
