// Pref.cpp: implementation of the CPref class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "simul.h"
#include "Pref.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


const char* _strPrefSection= "Settings";
const char* _strUserSection= "Users";

//////////////////////////////////////////////////////////////////////
// Static initialization
//////////////////////////////////////////////////////////////////////


//	0 : Authoring Mode
//	1 : Learning Mode				
int					CPref::g_nMode=1;	

//	Default PassWord for Authoring Mode
CString				CPref::g_sPassWd = _T("NVL");

//	Default User
CString				CPref::g_sLastUser = _T("");

// List of previous registered users
CStringArray		CPref::g_cUserList;
CStringArray		CPref::g_cUnitList;

CStringArray		CPref::g_cCatList;

CString				CPref::g_sTraceRep = _T("");
CPref::CPrefTrace	CPref::CTrace = {TRUE,TRUE,TRUE,TRUE,TRUE};

COLORREF			CPref::g_crPast = RGB(0xFF, 0xFF, 0xE0);
COLORREF			CPref::g_crFuture = RGB(235,235,235);

int					CPref::g_nSpeedNorm=50;	
int					CPref::g_nSpeedAcc=150;	

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CPref::CPref()
{
}

CPref::~CPref()
{

}

BOOL CPref::AddUserInList(CString mstr)
{
	BOOL btest = TRUE;
	int nb = CPref::g_cUserList.GetSize();
	for (int i =0;i<nb && btest;i++)
	{
		if (mstr == CPref::g_cUserList.GetAt(i))
			btest = FALSE;
	}
	if (btest)
		CPref::g_cUserList.Add(mstr);
	return btest;
}



//////////////////////////////////////////////////////////////////////
/// Read the Profile from the Registry.
///
/// 
//////////////////////////////////////////////////////////////////////
BOOL CPref::ReadPref(const char *fn/*=0*/)
{
	CPref::g_nMode = AfxGetApp()->GetProfileInt(_strPrefSection,"Mode",0);
	CPref::g_sLastUser = AfxGetApp()->GetProfileString(_strUserSection,"LastUser",_T(""));
	CPref::g_sTraceRep = AfxGetApp()->GetProfileString(_strUserSection,"TraceLog",CPref::g_sTraceRep);

	CPref::CTrace.g_bCloseER = AfxGetApp()->GetProfileInt(_strPrefSection,"bCloseER",CPref::CTrace.g_bCloseER);
	CPref::CTrace.g_bOpenER = AfxGetApp()->GetProfileInt(_strPrefSection,"bOpenER",CPref::CTrace.g_bOpenER);
	CPref::CTrace.g_bSwitchER = AfxGetApp()->GetProfileInt(_strPrefSection,"bSwitchER",CPref::CTrace.g_bSwitchER);
	CPref::CTrace.g_bPauseSimul = AfxGetApp()->GetProfileInt(_strPrefSection,"bPauseSimul",CPref::CTrace.g_bPauseSimul);
	CPref::CTrace.g_bReviewSimul = AfxGetApp()->GetProfileInt(_strPrefSection,"bReviewSimul",CPref::CTrace.g_bReviewSimul);
	CPref::CTrace.g_bRunSimul = AfxGetApp()->GetProfileInt(_strPrefSection,"bRunSimul",CPref::CTrace.g_bRunSimul);

	CPref::g_nSpeedNorm = AfxGetApp()->GetProfileInt(_strPrefSection,"nSpeedNorm",CPref::g_nSpeedNorm);
	CPref::g_nSpeedAcc = AfxGetApp()->GetProfileInt(_strPrefSection,"nSpeedAcc",CPref::g_nSpeedAcc);

	CPref::g_crPast = AfxGetApp()->GetProfileInt(_strPrefSection,"crPast",CPref::g_crPast);
	CPref::g_crFuture = AfxGetApp()->GetProfileInt(_strPrefSection,"crFuture",CPref::g_crFuture);

	CPref::g_cUserList.RemoveAll();
	BOOL bTest = TRUE;
	int nb = 0;
	while (bTest)
	{
		CString strEntry;
		strEntry.Format(_T("User%d"),nb);
		CString mstr = AfxGetApp()->GetProfileString(_strUserSection,strEntry,_T(""));
		bTest = (BOOL)(mstr!=_T(""));
		if (bTest)
		{
			CPref::g_cUserList.Add(mstr);
			nb++;
		}
	}
	bTest = TRUE;
	nb = 0;
	while (bTest)
	{
		CString strEntry;
		strEntry.Format(_T("Category%d"),nb);
		CString mstr = AfxGetApp()->GetProfileString(_strUserSection,strEntry,_T(""));
		bTest = (BOOL)(mstr!=_T(""));
		if (bTest)
		{
			CPref::g_cCatList.Add(mstr);
			nb++;
		}
	}
	if (!nb)
	{
		CPref::g_cCatList.Add(_T("X v Time Graph"));
		CPref::g_cCatList.Add(_T("X v Time Graph (log)"));
		CPref::g_cCatList.Add(_T("XY Graph"));
		CPref::g_cCatList.Add(_T("Chart"));
		CPref::g_cCatList.Add(_T("Pie Chart"));
		CPref::g_cCatList.Add(_T("Concrete Animation"));
		CPref::g_cCatList.Add(_T("Table"));
		CPref::g_cCatList.Add(_T("Dynamic Equation"));
		CPref::g_cCatList.Add(_T("Terms"));
		CPref::g_cCatList.Add(_T("Value"));
	}
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
/// Write the Profile into the Registry.
///
/// 
//////////////////////////////////////////////////////////////////////
BOOL CPref::WritePref(const char *fn/*=0*/)
{
	AfxGetApp()->WriteProfileInt(_strPrefSection,"Mode",CPref::g_nMode);
	AfxGetApp()->WriteProfileString(_strUserSection,"LastUser",CPref::g_sLastUser);
	AfxGetApp()->WriteProfileString(_strUserSection,"TraceLog",CPref::g_sTraceRep);

	AfxGetApp()->WriteProfileInt(_strPrefSection,"bCloseER",CPref::CTrace.g_bCloseER);
	AfxGetApp()->WriteProfileInt(_strPrefSection,"bOpenER",CPref::CTrace.g_bOpenER);
	AfxGetApp()->WriteProfileInt(_strPrefSection,"bSwitchER",CPref::CTrace.g_bSwitchER);
	AfxGetApp()->WriteProfileInt(_strPrefSection,"bPauseSimul",CPref::CTrace.g_bPauseSimul);
	AfxGetApp()->WriteProfileInt(_strPrefSection,"bReviewSimul",CPref::CTrace.g_bReviewSimul);
	AfxGetApp()->WriteProfileInt(_strPrefSection,"bRunSimul",CPref::CTrace.g_bRunSimul);

	AfxGetApp()->WriteProfileInt(_strPrefSection,"nSpeedNorm",CPref::g_nSpeedNorm);
	AfxGetApp()->WriteProfileInt(_strPrefSection,"nSpeedAcc",CPref::g_nSpeedAcc);

	AfxGetApp()->WriteProfileInt(_strPrefSection,"crPast",CPref::g_crPast);
	AfxGetApp()->WriteProfileInt(_strPrefSection,"crFuture",CPref::g_crFuture);

	int nb = CPref::g_cUserList.GetSize();
	for (int i=0;i<nb;i++)
	{
		CString mstr = CPref::g_cUserList.GetAt(i);
		CString strEntry;
		strEntry.Format(_T("User%d"),i);
		AfxGetApp()->WriteProfileString(_strUserSection,strEntry,mstr);
	}

	nb = CPref::g_cCatList.GetSize();
	for (i=0;i<nb;i++)
	{
		CString mstr = CPref::g_cCatList.GetAt(i);
		CString strEntry;
		strEntry.Format(_T("Category%d"),i);
		AfxGetApp()->WriteProfileString(_strUserSection,strEntry,mstr);
	}

	return TRUE;
}
