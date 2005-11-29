// AuthorModel.h: interface for the CAuthorModel class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AUTHORMODEL_H__7F709841_7FB2_11D6_A2F8_A8561AA23E28__INCLUDED_)
#define AFX_AUTHORMODEL_H__7F709841_7FB2_11D6_A2F8_A8561AA23E28__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include <MSimulation\Model.h>

//////////////////////////////////////////////////////////////////////
/// Class implementing an editable Mathemetical Model..
///
/// This class implements 
/// The textual description of the model (including equations and comments) is stored in the #m_strModel
/// attributes; it is displayed in the SDI's edition panes (see CViewSDIbuild) and can be edited by the user
/// to modify the mathematical definition of the model.
/// Actually, DEMIST only supports the 4th Order Runge-Kutta method (see the CLUModelPage interface) but future
/// upgrades may introduce other methods. 
//////////////////////////////////////////////////////////////////////
class CPP_SimModel : public CMdEquation  
{
public:
	CString		m_strModel;			///< Contains the complete textual description of the Mathematical model.
	int			m_nNumApprox;		///< Zero-based index of the methods used for numerical approximation.
	double		m_nEulerH;			///< Step used

	DECLARE_SERIAL(CPP_SimModel)
	CPP_SimModel();
	CPP_SimModel(CMdEquation*pEq);
	virtual ~CPP_SimModel();
	virtual CMdEquation *Clone();

	//virtual void	OnNewEquation(CSimModel *pModel);
	virtual void	OnUpdateFrom(CLearningUnit *m_pLU,int nExpSet,int nTime,BOOL bInit);
	virtual CRect	DrawFlowChart(CDC* pDC);

	void Serialize( CArchive& ar );

	BOOL CheckValidity();
	void SetDependency();
};

#endif // !defined(AFX_AUTHORMODEL_H__7F709841_7FB2_11D6_A2F8_A8561AA23E28__INCLUDED_)
