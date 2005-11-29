// LearnerTrace.cpp: implementation of the CLearnerTrace class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "simul.h"

#include <Prefs\Pref.h>
#include "simulDoc.h"

#include <MSimulation\Model.h>

#include <MInstruction\LearningUnit.h>
#include <MInteraction\Format.h>
#include <Animation\AnimObject.h>


#include "LearnerTrace.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CString		CTrace::g_strFileName = _T("");
BOOL		CTrace::g_bTrace = TRUE;
CSimulDoc*	CTrace::g_pDoc = NULL;

CTrace::CTrace()
{
}

CTrace::~CTrace()
{

}

BOOL CTrace::ReadLog()
{
	if (CTrace::g_strFileName == _T("")) return FALSE;

	FILE *fp = fopen(CTrace::g_strFileName,"a+");

	CStdioFile f(fp);
	CString mstr;
	BOOL ret = TRUE;
	while (ret)
	{
		ret = f.ReadString(mstr);
	}
	f.Close();

	return TRUE;
}

BOOL CTrace::CloseLogFile()
{
	CTrace::g_strFileName = _T("");
	return TRUE;
}

BOOL CTrace::OpenLogFile()
{
	if (CTrace::g_strFileName == _T(""))
	{
		SYSTEMTIME lpTime;
		LONG lBias = 0L;
		TIME_ZONE_INFORMATION stTZI;
		if (GetTimeZoneInformation(&stTZI) != TIME_ZONE_ID_INVALID)
		{
			lBias = ((stTZI.Bias + stTZI.DaylightBias) / 60) * -1;
		}
		GetSystemTime(&lpTime);
		if (lBias > 0) 
			lpTime.wHour += (unsigned short)lBias;
	    else 
			lpTime.wHour -= (unsigned short)lBias;

		CString st_szGetStringTime;
		st_szGetStringTime.Format(_T("%02d/%02d/%04d"), 
				lpTime.wDay,lpTime.wMonth,lpTime.wYear);
		
		CTrace::g_strFileName = CPref::g_sTraceRep+_T("\\")+CPref::g_sLastUser+_T(".log");
		CString mstr;
		mstr.Format(_T("----\tNew Session : %s \t----"),st_szGetStringTime);
		WriteString(mstr,FALSE);
	}
	return TRUE;
}

BOOL CTrace::WriteString(CString mstr,BOOL bAddTime/*=TRUE*/,BOOL bErase/*=FALSE*/)
{
	if (!OpenLogFile()) return FALSE;

	CString strWrite;

	if (bAddTime)
	{
		SYSTEMTIME lpTime;
		LONG lBias = 0L;
		TIME_ZONE_INFORMATION stTZI;
		if (GetTimeZoneInformation(&stTZI) != TIME_ZONE_ID_INVALID)
		{
			lBias = ((stTZI.Bias + stTZI.DaylightBias) / 60) * -1;
		}
		GetSystemTime(&lpTime);
		if (lBias > 0) 
			lpTime.wHour += (unsigned short)lBias;
	    else 
			lpTime.wHour -= (unsigned short)lBias;

		CString st_szGetStringTime;
//		st_szGetStringTime.Format(_T("%02d/%02d/%04d %02d:%02d:%02d.%03d\t"), 
//				lpTime.wDay,lpTime.wMonth,lpTime.wYear,
//				lpTime.wHour,lpTime.wMinute,lpTime.wSecond,
//				lpTime.wMilliseconds);
		st_szGetStringTime.Format(_T("%02d:%02d:%02d.%03d\t"), 
				lpTime.wHour,lpTime.wMinute,lpTime.wSecond,
				lpTime.wMilliseconds);
		
		strWrite = st_szGetStringTime + mstr;
	}
	else
	{
		strWrite = _T("\t") + mstr;
	}

	/*	CStdioFile f(CTrace::m_strFileName, 
					CFile::modeCreate | CFile::modeNoTruncate | 
					CFile::modeWrite | CFile::shareExclusive | CFile::typeText);*/
	FILE *fp = NULL;
	if (bErase)
		fp = fopen(CTrace::g_strFileName,"w+");
	else
		fp = fopen(CTrace::g_strFileName,"a+");

	TRY
	{

		CStdioFile f(fp);
		f.WriteString(strWrite);
		f.WriteString(_T("\n"));
		f.Close();
	}
	CATCH( CFileException, e )
	{
#ifdef _DEBUG
		afxDump << "File could not be opened " << e->m_cause << "\n";
#endif
	}
	END_CATCH
	return TRUE;
}

BOOL CTrace::WriteDocEnd(CString strDocName)
{
	//if (!pDoc) return FALSE;
	if (CPref::g_nMode == 0) return FALSE;

	if (strDocName.IsEmpty()) strDocName = _T("< Untitled >");

	CString mstr;
	mstr.Format(_T("CLOSE FILE \t %s"),strDocName);
	//WriteString(_T("\n"),FALSE);
	WriteString(mstr);
	return TRUE;
}

BOOL CTrace::WriteDocHeader(CSimulDoc *pDoc)
{
	if (!pDoc) return FALSE;
	if (CPref::g_nMode == 0) return FALSE;

	g_pDoc = pDoc;

	CString strDocName = pDoc->GetPathName();
	if (strDocName.IsEmpty()) strDocName = _T("< Untitled >");

	WriteString(_T("\n"),FALSE);
	CString mstr;
	mstr.Format(_T("OPEN FILE \t %s"),strDocName);
	WriteString(mstr);


	// Write File Header in a separate file
	CString oldName = CTrace::g_strFileName ;
	TCHAR name[_MAX_FNAME];
	_tsplitpath (strDocName, NULL, NULL, name, NULL);
	CTrace::g_strFileName = CPref::g_sTraceRep+_T("\\")+name+_T(".log");

	mstr.Format(_T("OPEN FILE \t %s"),strDocName);
	WriteString(mstr,FALSE,TRUE);

	for (int i=0;i<pDoc->m_pSimul->m_cEquSet.GetSize();i++)
	{
		CMdEquation* pEqu = pDoc->m_pSimul->GetEquation(i);
		if (!pEqu) continue;

		CString mstr2;
		mstr2.Format(_T("(SM=%d) - %s"),i,pEqu->GetSimulationName());
		WriteString(mstr2,FALSE);

		for (int j=0;j<pEqu->m_cParSet.GetSize();j++)
		{
			CModelPar* pPar = pEqu->m_cParSet.GetAt(j);
			if (!pPar) continue;

			CString mstr2;
			mstr2.Format(_T("\t%d - %s\t%s"),j,pPar->GetName(),pPar->GetAbbrev());
			WriteString(mstr2,FALSE);
		}
		for (int k=0;k<pEqu->m_cVarSet.GetSize();k++)
		{
			CModelVar* pPar = pEqu->m_cVarSet.GetAt(k);
			if (!pPar) continue;

			CString mstr2;
			mstr2.Format(_T("\t%d - %s\t%s"),j+k,pPar->GetName(),pPar->GetAbbrev());
			WriteString(mstr2,FALSE);
		}

		pEqu->m_cVarSet.GetSize();
	}
	WriteString(_T("\n"),FALSE);

	for (i=0;i<pDoc->m_pInstruct->m_cLUSet.GetSize();i++)
	{
		CLearningUnit* pLU = pDoc->m_pInstruct->m_cLUSet.GetAt(i);
		CString mstr2;

		mstr2.Format(_T("(LU=%d) - %s\tSM=%d\tTIME=%d\tMAX=%d"),i,pLU->m_sLUName,pLU->m_nSimulID,
							pLU->GetMaxTimer(),pLU->m_nbERMax);
		WriteString(mstr2,FALSE);
		WriteLUHeader(pLU);
	}

	WriteString(_T("\n"),FALSE);

	// Reset the User's Log File
	CTrace::g_strFileName = oldName ;

	return TRUE;
}

BOOL CTrace::WriteLUHeader(CLearningUnit *pLU)
{
	if (!pLU) return FALSE;
	if (CPref::g_nMode == 0) return FALSE;

	for (int i=0;i<pLU->m_cMERSet.GetSize();i++)
	{
		CExternRepUnit* pER = pLU->m_cMERSet.GetAt(i);
		if (!pER) continue;

		CString strER;
		strER.LoadString(pER->GetID());
		int nLCR = strER.Find('\n');
		strER = strER.Left(nLCR);

		if (strER != _T("Controller"))
			strER = pER->m_sCategory;

		CString mstr3;
		//mstr3.Format(_T("\t%s\t%s\tid=%d\t%d"),pER->GetName(),strER,pER->GetID(),pER->m_bOpenStart);
		mstr3.Format(_T("\t%s\t%s\t%d"),pER->GetName(),strER,pER->m_bOpenStart);

		WriteString(mstr3,FALSE);
		if (pER->GetID()==IDR_SIMULANIM)
		{
			CFormatAnimation* pDlg = NULL;
			int nbF = pER->m_cFormatSet.GetSize();
			if (nbF)
			{
				CFormat* pPage = pER->m_cFormatSet.GetAt(0);
				pDlg = DYNAMIC_DOWNCAST(CFormatAnimation,pPage);
			}
			if (pDlg)
			{
				int nb = pDlg->m_cObjSet.GetSize();
				for (int j=0;j<nb;j++)
				{
					CAnimObject* pObj = pDlg->m_cObjSet.GetAt(j);
					CValueObject *pVal = DYNAMIC_DOWNCAST(CValueObject,pObj);
					CPictDensityObject *pCater = DYNAMIC_DOWNCAST(CPictDensityObject,pObj);
					CDensityObject *pPopD = DYNAMIC_DOWNCAST(CDensityObject,pObj);
					if (pVal)
					{
						CString mstr4;
						mstr4.Format(_T("\t\t%d--%d\tDISPLAYED\tAVAILABLE"),pVal->m_iValueName,0);
						WriteString(mstr4,FALSE);
					}
					else if (pCater)
					{
						CString mstr4;
						mstr4.Format(_T("\t\t%d--%d\tDISPLAYED\tAVAILABLE"),pCater->m_iValueName,0);
						WriteString(mstr4,FALSE);
					}
					else if (pPopD)
					{
						POSITION pos = pPopD->m_lData.GetHeadPosition();
						while (pos)
						{
							CDensityData *DensO = (CDensityData *)pPopD->m_lData.GetNext(pos);
							CString mstr4;
							mstr4.Format(_T("\t\t%d--%d\tDISPLAYED\tAVAILABLE"),DensO->m_nOutcome,0);
							WriteString(mstr4,FALSE);
						}
					}

				}
			}
		}
		else for (int k=0;k<pER->m_cOutputSet.GetSize();k++)
		{
			COutcomes* pOut = pER->m_cOutputSet.GetAt(k);
			if (!pOut) continue;

			CString mstr4;
			CString mstrSel = (pOut->m_nSelected) ? _T("DISPLAYED"): _T("UNDISPLAYED") ;
			CString mstrAll = (pOut->m_nAllow) ? _T("AVAILABLE"): _T("UNAVAILABLE") ;
			mstr4.Format(_T("\t\t%d--%d\t%s\t%s"),pOut->m_nData,pOut->m_nExpSet,mstrSel,mstrAll);
			WriteString(mstr4,FALSE);
		}
	}
	for (i=0;i<pLU->m_cTBreakSet.GetSize();i++)
	{
		CTimerBreakPoint* pBP = pLU->m_cTBreakSet.GetAt(i);
		if (!pBP) continue;

		CString mstr3;
	}
	for (i=0;i<pLU->m_cExpSet.GetSize();i++)
	{
		CMdExpertSet* pExpSet = pLU->m_cExpSet.GetAt(i);
		if (!pExpSet) continue;

		CString mstr3;
	}



	return TRUE;
}


//////////////////////////////////////////////////////////////////////
/// Trace 
/// \param pActivateWnd		????
/// \param pDeactivateWnd	????
/// 
//////////////////////////////////////////////////////////////////////
BOOL CTrace::T_SWITCH(CWnd* pActivateWnd, CWnd* pDeactivateWnd)
{
	if (!CTrace::g_bTrace) return FALSE;
	if (CPref::g_nMode == 0) return FALSE;
	if (!CPref::CTrace.g_bSwitchER) return FALSE;
	CFrameWnd *pActFrm = DYNAMIC_DOWNCAST( CFrameWnd, pActivateWnd->GetParentFrame());
	CFrameWnd *pDesActFrm = DYNAMIC_DOWNCAST( CFrameWnd, pDeactivateWnd);
	if (pActFrm && pDesActFrm)
	{
		CString mstr;
		CView *pView = pActFrm->GetActiveView();
		//pView->GetWindowText(mstr);
		pActFrm->GetWindowText(mstr);

		CString mstr2,mstr3;
		pDesActFrm->GetWindowText(mstr2);
		mstr3.Format(_T("\t(%s)"),mstr2);

		mstr = _T("ACTIV\t") + mstr + mstr3;
		WriteString(mstr);
	}
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
/// Trace 
/// \param pActivateWnd	????
/// 
//////////////////////////////////////////////////////////////////////
BOOL CTrace::T_CLOSE(CWnd* pActivateWnd)
{
	if (!CTrace::g_bTrace) return FALSE;
	if (CPref::g_nMode == 0) return FALSE;
	if (!CPref::CTrace.g_bCloseER) return FALSE;
	CFrameWnd *pActFrm = DYNAMIC_DOWNCAST( CFrameWnd, pActivateWnd);
	if (pActFrm)
	{
		CString mstr;
		CView *pView = pActFrm->GetActiveView();
		//pView->GetWindowText(mstr);
		pActFrm->GetWindowText(mstr);
		mstr = _T("CLOSE\t") + mstr;
		WriteString(mstr);
	}
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
/// Trace 
/// \param pActivateWnd	????
/// 
//////////////////////////////////////////////////////////////////////
BOOL CTrace::T_OPEN(CWnd* pActivateWnd)
{
	if (!CTrace::g_bTrace) return FALSE;
	if (CPref::g_nMode == 0) return FALSE;
	if (!CPref::CTrace.g_bOpenER) return FALSE;
	CFrameWnd *pActFrm = DYNAMIC_DOWNCAST( CFrameWnd, pActivateWnd);
	if (pActFrm)
	{
		CString mstr;
		CView *pView = pActFrm->GetActiveView();
		//pView->GetWindowText(mstr);
		pActFrm->GetWindowText(mstr);
		mstr = _T("OPEN\t") + mstr;
		WriteString(mstr);
	}
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
/// Trace 
/// \param nCurr	????
/// \param nRun		????
/// \param nMax		????
/// 
//////////////////////////////////////////////////////////////////////
BOOL CTrace::T_RUN(int nCurr,int nRun,int nMax)
{
	if (!CTrace::g_bTrace) return FALSE;
	if (CPref::g_nMode == 0) return FALSE;
	CString mstr;
	mstr.Format(_T("RUN\t%d\t%d\t%d"),nCurr,nRun,nMax);
	WriteString(mstr);
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
/// Trace 
/// \param nTime	????
/// \param nMode		????
/// \param nRun		????
/// \param nMax		????
/// 
//////////////////////////////////////////////////////////////////////
BOOL CTrace::T_REVIEW(int nTime,int nMode,int nRun,int nMax)
{
	if (!CTrace::g_bTrace) return FALSE;
	if (CPref::g_nMode == 0) return FALSE;
	CString mstr,rstr;
	switch (nMode){
		case RUN_NONE: rstr = _T("None");break;
		case RUN_RUN: rstr = _T("Run mode");break;
		case RUN_BACKWARD: rstr = _T("backward");break;
		case RUN_FORWARD: rstr = _T("forward");break;
		case RUN_PREVSTEP: rstr = _T("previous step");break;
		case RUN_NEXTSTEP: rstr = _T("next step");break;
		case RUN_GOBEGIN: rstr = _T("goto begin");break;
		case RUN_GOEND: rstr = _T("goto end");break;
	}
	mstr.Format(_T("REVIEW\t%d\t%d\t%d\t(%s)"),nTime,nRun,nMax,rstr);
	WriteString(mstr);
	return TRUE;
}


//////////////////////////////////////////////////////////////////////
/// Trace 
/// \param nCurr	????
/// \param nRun		????
/// \param nMax		????
/// 
//////////////////////////////////////////////////////////////////////
BOOL CTrace::T_GOTO(int nCurr,int nRun,int nMax)
{
	if (!CTrace::g_bTrace) return FALSE;
	if (CPref::g_nMode == 0) return FALSE;
	CString mstr;
	mstr.Format(_T("GOTO\t%d\t%d\t%d"),nCurr,nRun,nMax);
	WriteString(mstr);
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
/// Trace 
/// \param nCurr	????
/// \param nRun		????
/// \param nMax		????
/// \param bBP		????
/// 
//////////////////////////////////////////////////////////////////////
BOOL CTrace::T_STOPAT(int nCurr,int nRun,int nMax,BOOL bBP)
{
	if (!CTrace::g_bTrace) return FALSE;
	if (CPref::g_nMode == 0) return FALSE;
	CString mstr;
	mstr.Format(_T("STOPAT\t%d\t%d\t%d"),nCurr,nRun,nMax);
	if (bBP)
		mstr += _T("\tBreakPoint");
	WriteString(mstr);
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
/// Trace 
/// \param nCurr	????
/// \param nRun		????
/// \param nMax		????
/// 
//////////////////////////////////////////////////////////////////////
BOOL CTrace::T_INITAT(int nCurr,int nRun,int nMax)
{
	if (!CTrace::g_bTrace) return FALSE;
	if (CPref::g_nMode == 0) return FALSE;
	CString mstr;
	mstr.Format(_T("INITAT\t%d\t%d\t%d"),nCurr,nRun,nMax);
	WriteString(mstr);
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
/// Trace 
/// \param nUnit	????
/// \param strUnit	????
/// 
//////////////////////////////////////////////////////////////////////
BOOL CTrace::T_STARTUNIT(int nUnit,CString strUnit)
{
	if (!CTrace::g_bTrace) return FALSE;
	if (CPref::g_nMode == 0) return FALSE;
	CString mstr;
	mstr.Format(_T("LAUNCH UNIT\t%s (%d)"),strUnit,nUnit);
	WriteString(mstr);
	return TRUE;
}


//////////////////////////////////////////////////////////////////////
/// Trace 
/// \param pDoc		????
/// \param mTInfo	????
/// 
//////////////////////////////////////////////////////////////////////
BOOL CTrace::T_MAPRELATION(CSimulDoc *pDoc,CTranslationInfo *mTInfo)
{
	if (!CTrace::g_bTrace) return FALSE;
	if (CPref::g_nMode == 0) return FALSE;
	if (!mTInfo) return FALSE;
	CString mstr = _T("");

	CLearningUnit*	pLU = pDoc->GetCurrentLU();
	if (!pLU) return FALSE;

	int nb = mTInfo->m_nDatas.GetSize();

	CString strWnd;
	if (mTInfo->m_pWndTarget)
	{
		CFrameWnd *pActFrm = DYNAMIC_DOWNCAST( CFrameWnd, mTInfo->m_pWndTarget->GetParentFrame());
		pActFrm->GetWindowText(strWnd);
	}

	if (!nb)
	{
		CString toto;
		toto.Format(_T("MAP RELATION\t%s\t%d"),strWnd,mTInfo->m_nTime);
		//if (i!=(nb-1)) toto+=_T("\n");
		mstr = toto;
		WriteString(mstr);
	}
	else 
	{
		CString strData;
		int nTime;
		for (int i=0;i<nb;i++)
		{
			CTranslationInfo::CTInfo tti = mTInfo->m_nDatas.GetAt(i);

			nTime = tti.nTime;
			//CString strExpSet =  pLU->m_cExpSet.GetAt(tti.nExpSet)->GetName();
			//CString strData   =  pLU->GetDataAt(tti.nData)->GetDataName();
			CString toto;
			toto.Format(_T("%d--%d"),tti.nData,tti.nExpSet);
			if (i)	strData +=_T("\t");
			strData +=toto;
		}

		CString toto;
		//toto.Format(_T("MAP RELATION\t%s\t%d\t%s (%s)\t%d\t%d"),strWnd,tti.nTime,strData,strExpSet,tti.nData,tti.nExpSet);
		toto.Format(_T("MAP RELATION\t%s\t%d\t%s"),strWnd,nTime,strData);
		//if (i!=(nb-1)) toto+=_T("\n");
		mstr = toto;
		WriteString(mstr);

	}
	//mstr.Format(_T("MAP RELATION\t%s (%d)"),strUnit,nUnit);
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
/// Trace 
/// \param pSender	????
/// \param pData	????
/// 
//////////////////////////////////////////////////////////////////////
BOOL CTrace::T_HYPOTHESIS(CView* pSender,CUserData *pData)
{
	if (!CTrace::g_bTrace) return FALSE;
	if (CPref::g_nMode == 0) return FALSE;
	if (!pSender || !pData) return FALSE;
	
	CFrameWnd *pActFrm = DYNAMIC_DOWNCAST( CFrameWnd, pSender->GetParentFrame());
	if (!pActFrm) return FALSE;

	CString strView;
	CView *pView = pActFrm->GetActiveView();
	pActFrm->GetWindowText(strView);

	CString mstr;
	mstr.Format(_T("HYPOTHESIS\t%s\t%d\t%d--%d\t%.4f"),strView,pData->m_nTime,pData->m_nOutcome,
			pData->m_nExpSet,pData->m_dValue);
	WriteString(mstr);
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
/// Trace 
/// \param pSender	????
/// \param pData	????
/// 
//////////////////////////////////////////////////////////////////////
BOOL CTrace::T_HYPO_TEST(CView* pSender,CUserData *pData)
{
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
/// Trace 
/// \param pSender	????
/// \param pData	????
/// 
//////////////////////////////////////////////////////////////////////
BOOL CTrace::T_ACTION(CView* pSender,CUserData *pData)
{
	if (!CTrace::g_bTrace) return FALSE;
	if (CPref::g_nMode == 0) return FALSE;
	if (!pSender || !pData) return FALSE;
	
	CFrameWnd *pActFrm = DYNAMIC_DOWNCAST( CFrameWnd, pSender->GetParentFrame());
	if (!pActFrm) return FALSE;

	CString strView;
	CView *pView = pActFrm->GetActiveView();
	pActFrm->GetWindowText(strView);

	CString mstr;
	mstr.Format(_T("ACTION\t%s\t%d\t%d--%d\t%.4f\t%.4f"),strView,pData->m_nTime,pData->m_nOutcome,
			pData->m_nExpSet,pData->m_dValue,pData->m_dOldValue);
	WriteString(mstr);
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
/// Trace 
/// \param pData	????
/// 
//////////////////////////////////////////////////////////////////////
BOOL CTrace::T_BREAKPOINT(CTimerBreakPoint *pData)
{
	if (!CTrace::g_bTrace) return FALSE;
	if (CPref::g_nMode == 0) return FALSE;
	if (!pData) return FALSE;

	CString strMode;
	switch (pData->m_nBPType){
		case BREAKP_INFO: strMode = _T("Information");break;
		case BREAKP_ACTION: strMode = _T("Action");break;
		case BREAKP_HYPOT: strMode = _T("Hypothesis");break;
		case BREAKP_TEST: strMode = _T("Test");break;
	}

	CString mstr;
	mstr.Format(_T("BREAKPOINT\t%s\t%d"),strMode,pData->m_tBreakPt);
	WriteString(mstr);

	return TRUE;
}

//////////////////////////////////////////////////////////////////////
/// Trace 
/// \param pSender	????
/// \param nExpSet	????
/// 
//////////////////////////////////////////////////////////////////////
BOOL CTrace::T_CHANGEEXPSET(CView* pSender,int nExpSet)
{
	if (!CTrace::g_bTrace) return FALSE;
	if (CPref::g_nMode == 0) return FALSE;
	if (!pSender) return FALSE;

	CFrameWnd *pActFrm = DYNAMIC_DOWNCAST( CFrameWnd, pSender->GetParentFrame());
	if (!pActFrm) return FALSE;

	CString strView;
	CView *pView = pActFrm->GetActiveView();
	pActFrm->GetWindowText(strView);

	CString mstr;
	mstr.Format(_T("EXP-SET\t%s\t%d"),strView,nExpSet);
	WriteString(mstr);
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
/// Trace 
/// \param pSender	????
/// \param pSet	????
/// 
//////////////////////////////////////////////////////////////////////
BOOL CTrace::T_CHANGEOUTCOME(CView* pSender,CxOutcomes* pSet)
{
	if (!CTrace::g_bTrace) return FALSE;
	if (CPref::g_nMode == 0) return FALSE;
	if (!pSender || !pSet) return FALSE;

	CFrameWnd *pActFrm = DYNAMIC_DOWNCAST( CFrameWnd, pSender->GetParentFrame());
	if (!pActFrm) return FALSE;

	CString strView;
	CView *pView = pActFrm->GetActiveView();
	pActFrm->GetWindowText(strView);

	CString strOutcome;

	for (int i=0;i<pSet->GetSize();i++)
	{
		COutcomes *po = pSet->GetAt(i);
		if (!po) continue;

		if (po->m_nSelected)
		{
			CString strTemp;
			strTemp.Format(_T("%d--%d "),po->m_nData,po->m_nExpSet);
			strOutcome += strTemp;
		}
	}

	CString mstr;
	mstr.Format(_T("EXP-SET\t%s\t%s"),strView,strOutcome);
	WriteString(mstr);
	return TRUE;
}



//////////////////////////////////////////////////////////////////////
/// Trace 
/// \param pLU	????
/// \param pEqu	????
/// 
//////////////////////////////////////////////////////////////////////
BOOL CTrace::T_CHANGEINIT(CLearningUnit* pLU,CMdEquation* pEqu)
{
	if (!CTrace::g_bTrace) return FALSE;
	if (CPref::g_nMode == 0) return FALSE;
	if (!pLU || !pEqu) return FALSE;

	CString strInfo;

	int nb = pEqu->m_cParSet.GetSize();
	for (int j=0;j<nb;j++)
	{
		CModelPar *pPar= pEqu->m_cParSet.GetAt(j);
		if (!pPar) continue;

		CString strExpSet = _T("(");

		int nbExpSet =  pLU->m_cExpSet.GetSize();
		for (int i=0;i<nbExpSet;i++)
		{
			CMdExpertSet* pExpSet = pLU->m_cExpSet.GetAt(i);
			if (!pExpSet) continue;

			CString name = pPar->GetName();
			double	val = pExpSet->GetValue(j);
			CString mstr;
			if (!i) mstr += _T(",");
			mstr.Format(_T("%.4f"),val);
			if (i!=(nbExpSet-1)) mstr += _T(",");
			strExpSet += mstr;
		}

		strExpSet += _T(")");

		CString mstr;
		mstr.Format(_T("%d--%s"),j,strExpSet);
		if (j) strInfo+=_T("\t");
		strInfo += mstr;
	}

	CString mstr;
	mstr.Format(_T("INIT_COND\t%s"),strInfo);
	WriteString(mstr);
	return TRUE;
}
