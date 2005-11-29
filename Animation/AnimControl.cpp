// AnimControl.cpp: implementation of the CAnimControl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "simul.h"
#include "SimulDoc.h"
#include <MInstruction\LearningUnit.h>
#include <MSimulation\Model.h>

#include "AnimControl.h"

#include "AnimCtrl/CRadialCtrl.h"
#include "AnimCtrl/3DMeterCtrl.h"

#include "AnimCtrl/ComponentStatic.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAnimControl::CAnimControl()
{
	m_pWnd = NULL;
	//m_pControl = (CComponentStatic*)new CRadialCtrl();
	//((CRadialCtrl*)m_pControl)->DisplayAngle(TRUE);
	//((CRadialCtrl*)m_pControl)->SetActive(TRUE);
	m_pControl = new C3DMeterCtrl();
	m_bEnable = FALSE;
}

CAnimControl::~CAnimControl()
{
	delete m_pControl;
//	delete m_pWnd;
}

int	 CAnimControl::GetObjectIcon()
{
	return -1;
}


CWnd* CAnimControl::GetObjectView()
{
	return m_pWnd;
}

void CAnimControl::SetObjectView(CWnd *pWnd)
{
	if (!pWnd/* || m_pWnd*/) return;
	m_pWnd = pWnd;

	if (m_pControl && !(m_pControl->m_hWnd))
	{
		DWORD dwStyle=WS_CHILD | WS_VISIBLE ;//| SS_USERITEM ;
		CRect mrect = GetRect();
		//CRadialCtrl*pRad = (CRadialCtrl*)m_pControl;
		C3DMeterCtrl* pRad = (C3DMeterCtrl*)m_pControl;
		pRad->Create("SSD",dwStyle,mrect,m_pWnd);
		DWORD dwExStyle = m_pControl->GetExStyle();
		//dwExStyle |=WS_EX_STATICEDGE ;
		//m_pControl->ModifyStyleEx(0,WS_EX_STATICEDGE);
		//m_pControl->EnableWindow(m_bEnable);
	}
}

void CAnimControl::Draw (CDC* pDC,BOOL bParent)
{
	CAnimObject::Draw(pDC,bParent);
	if (m_pControl->m_hWnd)
	{
		CRect mrect = GetRect();
		m_pControl->Draw(pDC,mrect);
	}
}

void CAnimControl::ModifyObjectProperty()
{
	CAnimObject::ModifyObjectProperty();
}

void CAnimControl::OnUpdate()
{
	CAnimObject::OnUpdate();
//	m_sText="####";

	//m_currTime = m_pDoc->m_currTimer;
	int nExpSet = m_currExpSet;

	CLearningUnit *pLU= m_pDoc->GetCurrentLU();

	int m_iValueName = 4;
	CMdData *pData = pLU->GetDataAt(m_iValueName);
	CString mstr = pData->GetDataName(pLU->m_bShowAbbrev);
	if (!pData) return;

	CModelObj *pModel = pData->GetDataInfo();
	//pModel->Get
	double dmin,dmax;
	pData->GetMinMax(&dmin,&dmax,nExpSet);
	double m_dValue = pData->GetAt(m_currTime,nExpSet);
	//((CRadialCtrl*)m_pControl)->SetRange((int)dmin,(int)(dmax+1));
	//((CRadialCtrl*)m_pControl)->SetPos((int)m_dValue);
	((C3DMeterCtrl*)m_pControl)->SetRange(dmin,dmax);
	((C3DMeterCtrl*)m_pControl)->UpdateNeedle(m_dValue);
	((C3DMeterCtrl*)m_pControl)->SetUnits(mstr);
}



