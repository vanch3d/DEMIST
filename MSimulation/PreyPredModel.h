// PreyPredModel.h: interface for the CPreyPredModel class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PREYPREDMODEL_H__4FA56F56_0B9D_11D4_A2F6_00D0B71C8709__INCLUDED_)
#define AFX_PREYPREDMODEL_H__4FA56F56_0B9D_11D4_A2F6_00D0B71C8709__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <MSimulation\Model.h>

// Construction/Destruction
/////////////////////////////////////////////////////////////////////
class CPreyPredModel : public CSimModel  
{
public:
	DECLARE_SERIAL(CPreyPredModel)
	CPreyPredModel();
	virtual ~CPreyPredModel();

	virtual void OnNewModel();
	virtual void OnInitiaUpdate();
	void Serialize( CArchive& ar );
};

// Construction/Destruction
//////////////////////////////////////////////////////////////////////
class CPP_SgUnlimited : public CMdEquation  
{
public:
	DECLARE_SERIAL(CPP_SgUnlimited)
	CPP_SgUnlimited();
	CPP_SgUnlimited(CMdEquation*pEq);
	virtual ~CPP_SgUnlimited();
	virtual CMdEquation *Clone();

	double NFunc(double N,double param[]);

	virtual void	OnNewEquation(CSimModel *pModel);
	virtual void	OnUpdateFrom(CLearningUnit *m_pLU,int nExpSet,int nTime,BOOL bInit);

	virtual CString GetFormatedText(int nLevel=0);
	virtual CString GetFormatedEquation();

	virtual CRect	DrawFlowChart(CDC* pDC);

	void Serialize( CArchive& ar );
};

// Construction/Destruction
//////////////////////////////////////////////////////////////////////
class CPP_SgUnlimitedII : public CPP_SgUnlimited  
{
public:
	DECLARE_SERIAL(CPP_SgUnlimitedII)
	CPP_SgUnlimitedII();
	CPP_SgUnlimitedII(CMdEquation*pEq);
	virtual ~CPP_SgUnlimitedII();
	virtual CMdEquation *Clone();

	virtual void	OnNewEquation(CSimModel *pModel);
	virtual void	OnUpdateFrom(CLearningUnit *m_pLU,int nExpSet,int nTime,BOOL bInit);

	virtual CString GetFormatedText(int nLevel=0);
	virtual CRect	DrawFlowChart(CDC* pDC);

	void Serialize( CArchive& ar );
};

// Construction/Destruction
//////////////////////////////////////////////////////////////////////
class CPP_SgLimited : public CMdEquation  
{
public:
	DECLARE_SERIAL(CPP_SgLimited)
	CPP_SgLimited();
	CPP_SgLimited(CMdEquation*pEq);
	virtual ~CPP_SgLimited();
	virtual CMdEquation *Clone();

	double NFunc(double N,double param[]);

	virtual void	OnNewEquation(CSimModel *pModel);
	virtual void	OnUpdateFrom(CLearningUnit *m_pLU,int nExpSet,int nTime,BOOL bInit);

	virtual CRect	DrawFlowChart(CDC* pDC);

	virtual CString GetFormatedText(int nLevel=0);
	virtual CString GetFormatedEquation();
	void Serialize( CArchive& ar );

};

// Construction/Destruction
//////////////////////////////////////////////////////////////////////
class CPP_SgLimitedII : public CPP_SgLimited  
{
public:
	DECLARE_SERIAL(CPP_SgLimitedII)
	CPP_SgLimitedII();
	CPP_SgLimitedII(CMdEquation*pEq);
	virtual ~CPP_SgLimitedII();
	virtual CMdEquation *Clone();

	virtual void	OnNewEquation(CSimModel *pModel);
	virtual void	OnUpdateFrom(CLearningUnit *m_pLU,int nExpSet,int nTime,BOOL bInit);

	virtual CString GetFormatedText(int nLevel=0);
};


// Construction/Destruction
//////////////////////////////////////////////////////////////////////
class CPP_TwCompetition : public CMdEquation  
{
public:
	DECLARE_SERIAL(CPP_TwCompetition)
	CPP_TwCompetition();
	CPP_TwCompetition(CMdEquation*pEq);
	virtual ~CPP_TwCompetition();
	virtual CMdEquation *Clone();

	double NFunc(double N,double param[]);
	double PFunc(double N,double param[]);

	virtual void	OnNewEquation(CSimModel *pModel);
	virtual void	OnUpdateFrom(CLearningUnit *m_pLU,int nExpSet,int nTime,BOOL bInit);

	virtual CRect	DrawFlowChart(CDC* pDC);

	virtual CString GetFormatedText(int nLevel=0);
	virtual CString GetFormatedEquation();

	void Serialize( CArchive& ar );
};

// Construction/Destruction
//////////////////////////////////////////////////////////////////////
class CPP_TwPredationUnltd : public CMdEquation  
{
public:
	DECLARE_SERIAL(CPP_TwPredationUnltd)
	CPP_TwPredationUnltd();
	CPP_TwPredationUnltd(CMdEquation*pEq);
	virtual ~CPP_TwPredationUnltd();
	virtual CMdEquation *Clone();

	double NFunc(double N,double param[]);
	double PFunc(double N,double param[]);

	virtual void	OnNewEquation(CSimModel *pModel);
	virtual void	OnUpdateFrom(CLearningUnit *m_pLU,int nExpSet,int nTime,BOOL bInit);

	virtual CString GetFormatedText(int nLevel=0);
	virtual CString GetFormatedEquation();

	void Serialize( CArchive& ar );
};

// Construction/Destruction
//////////////////////////////////////////////////////////////////////
class CPP_TwPredation : public CMdEquation  
{
public:
	DECLARE_SERIAL(CPP_TwPredation)
	CPP_TwPredation();
	CPP_TwPredation(CMdEquation*pEq);
	virtual ~CPP_TwPredation();
	virtual CMdEquation *Clone();

	double NFunc(double N,double param[]);
	double PFunc(double P,double param[]);

	virtual void	OnNewEquation(CSimModel *pModel);
	virtual void	OnUpdateFrom(CLearningUnit *m_pLU,int nExpSet,int nTime,BOOL bInit);

	virtual CString GetFormatedText(int nLevel=0);
	virtual CString GetFormatedEquation();

	void Serialize( CArchive& ar );
};


// Construction/Destruction
//////////////////////////////////////////////////////////////////////
class CPP_TwLotkaII : public CMdEquation  
{
public:
	DECLARE_SERIAL(CPP_TwLotkaII)
	CPP_TwLotkaII();
	CPP_TwLotkaII(CMdEquation*pEq);
	virtual ~CPP_TwLotkaII();
	virtual CMdEquation *Clone();

	virtual void	OnNewEquation(CSimModel *pModel);
	virtual void	OnUpdateFrom(CLearningUnit *m_pLU,int nExpSet,int nTime,BOOL bInit);

	virtual CString GetFormatedText(int nLevel=0);
	virtual CString GetFormatedEquation();

	void Serialize( CArchive& ar );
};

// Construction/Destruction
//////////////////////////////////////////////////////////////////////
class CPP_Pendulum : public CMdEquation  
{
public:
	DECLARE_SERIAL(CPP_Pendulum)
	CPP_Pendulum();
	CPP_Pendulum(CMdEquation*pEq);
	virtual ~CPP_Pendulum();
	virtual CMdEquation *Clone();

	double dThetaFunc(double theta,double param[]);
	double dOmegaFunc(double omega,double param[]);

	virtual void	OnNewEquation(CSimModel *pModel);
	virtual void	OnUpdateFrom(CLearningUnit *m_pLU,int nExpSet,int nTime,BOOL bInit);

	virtual CString GetFormatedText(int nLevel=0);
	virtual CString GetFormatedEquation();
	void Serialize( CArchive& ar );

};

#endif // !defined(AFX_PREYPREDMODEL_H__4FA56F56_0B9D_11D4_A2F6_00D0B71C8709__INCLUDED_)
