// Format.cpp: implementation of the CFormat class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\simul.h"
#include "Format.h"

#include "simulDoc.h"

//#include "DlgFormatTable.h"
//#include "DlgActionTable.h"
//#include "DlgFormatGraph.h"
//#include "DlgActionGraph.h"
//#include "DlgFormatAnimation.h"
//#include "DlgFormatPhasePlot.h"
//#include "DlgTranslation.h"
//#include "DlgFormatParameter.h"
#include "MInteraction\DlgFormat.h"
#include "MInteraction\DlgTranslation.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL( CFormat, CObject, VERSIONABLE_SCHEMA | 2)

CFormat::CFormat()
{
	m_pPage = NULL;
	m_nMaxTime = 0;
	m_bready = FALSE;
}

CFormat::~CFormat()
{
	if (m_pPage) delete m_pPage;
}


CFormat::CFormat(const CFormat& mFor)
{
	m_pPage = NULL;
	m_nMaxTime = mFor.m_nMaxTime;
	m_bready = FALSE;
}

CFormat* CFormat::Clone()
{
	CFormat* pFor=new CFormat(*this);
	return pFor;
}

//////////////////////////////////////////////////////////////////////
/// Get the dialog interface associated with this format.
/// \return A pointer to an instance of CFormatPage.
//////////////////////////////////////////////////////////////////////
CPropertyPage* CFormat::GetPage()
{ 
	return NULL;
};

//////////////////////////////////////////////////////////////////////
/// Called by the framework to clean the page, when the configuration dialog is closed.
/// \param bOK	TRUE if the changes are validated, FALSE if they are cancelled.
///
/// When the configuration dialog (see CDefineERPropSheet), it calls this function to clean and delete 
/// the interface but also, if the changes were validated (by pressing OK), to upload them from the 
///	interface to this data structure.
//////////////////////////////////////////////////////////////////////
void CFormat::UpdateFormat(BOOL bOK/*=TRUE*/)
{
}

//////////////////////////////////////////////////////////////////////
/// Called by the framework to define the content of the format dialog.
/// \param m_otTree	A pointer to the option-tree control that will hold the format options.
///
//////////////////////////////////////////////////////////////////////
void CFormat::UpdatePageContent(COptionTree *m_otTree)
{
}


void CFormat::Serialize(CArchive &ar)
{
	CObject::Serialize(ar);
	if( ar.IsStoring( ) )
	{
		ar << m_nMaxTime;
		//ar << m_bUseLabel;
	}
	else
	{
		ar >> m_nMaxTime;
		int nb= ar.GetObjectSchema();
		if (nb==3)
		{
			int m_bUseLabel;
			ar >> m_bUseLabel;
		}
	}
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL( CFormatTimer, CFormat, VERSIONABLE_SCHEMA | 2)

CFormatTimer::CFormatTimer() : CFormat()
{
}

CFormatTimer::~CFormatTimer()
{

}

CFormatTimer::CFormatTimer(const CFormatTimer& mFor): CFormat(mFor)
{
	m_pPage = NULL;
	m_nMaxTime = mFor.m_nMaxTime;
}

CFormat* CFormatTimer::Clone()
{
	CFormat* pFor=new CFormatTimer(*this);
	return pFor;
}

CPropertyPage* CFormatTimer::GetPage()
{
	if (!m_pPage)
	{
		CFormatPage *pPage = new CFormatPage(IDD_ER_FORMAT);
		//CERFormatPage *pPage = new CERFormatPage();
		m_pPage = pPage;
	}
	return m_pPage;
}

void CFormatTimer::UpdateFormat(BOOL bOK)
{
	if (!m_pPage) return;
	if (bOK)
	{
	}
	delete m_pPage;
	m_pPage = NULL;
}

void CFormatTimer::Serialize(CArchive &ar)
{
	CFormat::Serialize(ar);
	if( ar.IsStoring( ) )
	{
	}
	else
	{
	}
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL( CFormatTable, CFormat, VERSIONABLE_SCHEMA | 2)

CFormatTable::CFormatTable(): CFormat()
{
	m_nPrecision = 4;
	m_nTimeFrom = 1;
	m_nTimeTo = 1;
	m_nTimeStep = 1;
	m_bNeg = FALSE;
	m_bMin = FALSE;
	m_bMax = FALSE;
	m_clrMax = RGB(255,0,0);
	m_clrMin = RGB(0,255,0);
	m_clrNeg = CLR_DEFAULT;

	otiEditFrom = NULL;
	otiEditTo= NULL;
	otiEditStep= NULL;
	otiSpinPrec= NULL;
	otiClrMax= NULL;
	otiClrMin= NULL;
	otiClrNeg= NULL;
	otiChkMax= NULL;
	otiChkMin= NULL;
	otiChkNeg= NULL;
}

CFormatTable::~CFormatTable()
{

}

CFormatTable::CFormatTable(const CFormatTable& mFor): CFormat(mFor)
{
	m_nPrecision = mFor.m_nPrecision;
	m_nTimeFrom = mFor.m_nTimeFrom;
	m_nTimeTo = mFor.m_nTimeTo;
	m_nTimeStep = mFor.m_nTimeStep;
	m_bNeg = mFor.m_bNeg;
	m_bMin = mFor.m_bMin;
	m_bMax = mFor.m_bMax;
	m_clrMax = mFor.m_clrMax;
	m_clrMin = mFor.m_clrMin;
	m_clrNeg = mFor.m_clrNeg;
}

CFormat* CFormatTable::Clone()
{
	CFormat* pFor=new CFormatTable(*this);
	return pFor;
}

CString CFormatTable::GetFormatString()
{
	CString m_strprec;
	m_strprec.Format(_T("%%.%df"),m_nPrecision);
	return m_strprec;
}

CPropertyPage* CFormatTable::GetPage()
{
	if (!m_pPage)
	{
		/*CDlgFormatTable *pPage = new CDlgFormatTable(m_nMaxTime);

		if (m_nTimeTo==1)
			m_nTimeTo=m_nMaxTime;
		pPage->m_nPrecision = m_nPrecision;
		pPage->m_bNeg = m_bNeg;
		pPage->m_bMin=m_bMin;
		pPage->m_bMax=m_bMax;
		pPage->m_nTimeFrom=m_nTimeFrom;
		pPage->m_nTimeTo=m_nTimeTo;
		pPage->m_nTimeStep=m_nTimeStep;
		pPage->m_clrMax=m_clrMax;
		pPage->m_clrMin=m_clrMin;
		pPage->m_clrNeg=m_clrNeg;*/

		m_bready = FALSE;
		CERFormatPage *pPage = new CERFormatPage(this);

		m_pPage = pPage;
	}
	return m_pPage;
}

void CFormatTable::UpdatePageContent(COptionTree *m_otTree)
{
	m_bready = FALSE;
	if (!m_otTree) return;

	// Setup options tree
	COptionTreeItem *otiRoot = NULL;

	// -- CONTENT
	otiRoot = m_otTree->InsertItem(new COptionTreeItem());
	otiRoot->SetLabelText(_T("Content"));
	otiRoot->SetInfoText(_T("Define which of the components (e.g. legend, axes, etc.) are included in the ER."));
	
	// -- Local Maximum
	otiChkMax = (COptionTreeItemCheckBox*)m_otTree->InsertItem(new COptionTreeItemCheckBox(), otiRoot);
	otiChkMax->SetLabelText(_T("Local Maximum"));
	otiChkMax->SetInfoText(_T("Check the box to have the local maximum highlighted in the ER."));
	if (otiChkMax->CreateCheckBoxWindow(m_bMax, TRUE, TRUE) == TRUE)
	{
		otiChkMax->SetCheckText(_T("Displayed"), _T("Hidden"));
		//otiCheck->ReadOnly(TRUE);
	}
	// -- Local Minimum
	otiChkMin = (COptionTreeItemCheckBox*)m_otTree->InsertItem(new COptionTreeItemCheckBox(), otiRoot);
	otiChkMin->SetLabelText(_T("Local Minimum"));
	otiChkMin->SetInfoText(_T("Check the box to have the local minimum highlighted in the ER."));
	if (otiChkMin->CreateCheckBoxWindow(m_bMin, TRUE, TRUE) == TRUE)
	{
		otiChkMin->SetCheckText(_T("Displayed"), _T("Hidden"));
		//otiCheck->ReadOnly(TRUE);
	}
	// -- Negative Value
	otiChkNeg = (COptionTreeItemCheckBox*)m_otTree->InsertItem(new COptionTreeItemCheckBox(), otiRoot);
	otiChkNeg->SetLabelText(_T("Negative Values"));
	otiChkNeg->SetInfoText(_T("Check the box to have the negative values highlighted in the ER."));
	if (otiChkNeg->CreateCheckBoxWindow(m_bNeg, TRUE, TRUE) == TRUE)
	{
		otiChkNeg->SetCheckText(_T("Displayed"), _T("Hidden"));
		otiChkNeg->ReadOnly(TRUE);
	}

	// -- RANGE
	otiRoot = m_otTree->InsertItem(new COptionTreeItem());
	otiRoot->SetLabelText(_T("Range"));
	otiRoot->SetInfoText(_T("Define the range of data points for the table."));


	// -- From
	otiEditFrom = (COptionTreeItemEdit*) m_otTree->InsertItem(new COptionTreeItemEdit(), otiRoot);
	otiEditFrom->SetLabelText(_T("From"));
	otiEditFrom->SetInfoText(_T("Set the minimum data point to be displayed."));
	if (otiEditFrom->CreateEditWindow(FALSE, FALSE, TRUE, 0) == TRUE)
	{
		otiEditFrom->SetEditInt(m_nTimeFrom);
		otiEditFrom->ReadOnly(TRUE);
	}

	// -- To
	otiEditTo = (COptionTreeItemEdit*) m_otTree->InsertItem(new COptionTreeItemEdit(), otiRoot);
	otiEditTo->SetLabelText(_T("To"));
	otiEditTo->SetInfoText(_T("Set the maximum data point to be displayed."));
	if (otiEditTo->CreateEditWindow(FALSE, FALSE, TRUE, 0) == TRUE)
	{
		otiEditTo->SetEditInt(m_nTimeTo);
		otiEditTo->ReadOnly(TRUE);
	}

	// -- Step
	otiEditStep = (COptionTreeItemEdit*) m_otTree->InsertItem(new COptionTreeItemEdit(), otiRoot);
	otiEditStep->SetLabelText(_T("Steps"));
	otiEditStep->SetInfoText(_T("Set the frequency on which data points are displayed."));
	if (otiEditStep->CreateEditWindow(FALSE, FALSE, TRUE, 0) == TRUE)
	{
		otiEditStep->SetEditInt(m_nTimeStep);
	}

	// -- VALUES
	otiRoot = m_otTree->InsertItem(new COptionTreeItem());
	otiRoot->SetLabelText(_T("Values"));
	otiRoot->SetInfoText(_T("Define the format of the values displayed in the table."));

	// -- Precision
	otiSpinPrec = (COptionTreeItemSpinner*)m_otTree->InsertItem(new COptionTreeItemSpinner(), otiRoot);
	otiSpinPrec->SetLabelText(_T("Precision:"));
	otiSpinPrec->SetInfoText(_T("Set the number of decimal places for the value."));
	if (otiSpinPrec->CreateSpinnerWindow(TRUE, TRUE, m_nPrecision, 0, 8) == TRUE)
	{
	}

	// -- COLORS
	otiRoot = m_otTree->InsertItem(new COptionTreeItem());
	otiRoot->SetLabelText(_T("Colors"));
	otiRoot->SetInfoText(_T("Define the colors used to format the text and the background of the table."));

	// -- Local Maximum
	otiClrMax = (COptionTreeItemColor*)m_otTree->InsertItem(new COptionTreeItemColor(), otiRoot);
	otiClrMax->SetLabelText(_T("Local maximum"));
	otiClrMax->SetInfoText(_T("Set the background color of the cells containing the local maximum(s)."));
	if (otiClrMax->CreateColorWindow(m_clrMax, RGB(255, 0, 0), FALSE, TRUE) == TRUE)
	{
	}
	// -- Local Minimum
	otiClrMin = (COptionTreeItemColor*)m_otTree->InsertItem(new COptionTreeItemColor(), otiRoot);
	otiClrMin->SetLabelText(_T("Local minimum"));
	otiClrMin->SetInfoText(_T("Set the background color of the cells containing the local minimum(s)."));
	if (otiClrMin->CreateColorWindow(m_clrMin, RGB(0, 255, 0), FALSE, TRUE) == TRUE)
	{
	}
	// -- Negative values
	otiClrNeg = (COptionTreeItemColor*)m_otTree->InsertItem(new COptionTreeItemColor(), otiRoot);
	otiClrNeg->SetLabelText(_T("Negative values"));
	otiClrNeg->SetInfoText(_T("Set the text color of the cells containing negative value(s)."));
	if (otiClrNeg->CreateColorWindow(m_clrNeg, RGB(0, 0, 0), FALSE, TRUE) == TRUE)
	{
		otiClrNeg->ReadOnly(TRUE);
	}

	m_bready = TRUE;
}

void CFormatTable::UpdateFormat(BOOL bOK)
{
	if (!m_pPage) return;
	if (!m_bready) return;
	if (bOK)
	{
		m_bNeg = otiChkNeg->GetCheck();
		m_bMax = otiChkMax->GetCheck();
		m_bMin = otiChkMin->GetCheck();
		m_clrNeg = otiClrNeg->GetColor();
		m_clrMin = otiClrMin->GetColor();
		m_clrMax = otiClrMax->GetColor();

		int nb = m_nPrecision;
		if (otiSpinPrec->GetEditInt(nb))	m_nPrecision = nb;

		nb = m_nTimeFrom;
		if (otiEditFrom->GetEditInt(nb))	m_nTimeFrom = nb;
		nb = m_nTimeTo;
		if (otiEditTo->GetEditInt(nb))		m_nTimeTo = nb;
		nb = m_nTimeStep;
		if (otiEditStep->GetEditInt(nb))	m_nTimeStep = nb;

		/*CDlgFormatTable *pPage = (CDlgFormatTable *) m_pPage;
		m_nPrecision = pPage->m_nPrecision;
		m_bNeg = pPage->m_bNeg;
		m_bMin=pPage->m_bMin;
		m_bMax=pPage->m_bMax;
		m_nTimeFrom=pPage->m_nTimeFrom;
		m_nTimeTo=pPage->m_nTimeTo;
		m_nTimeStep=pPage->m_nTimeStep;
		m_clrMax=pPage->m_clrMax;
		m_clrMin=pPage->m_clrMin;
		m_clrNeg=pPage->m_clrNeg;*/
	}
	delete m_pPage;
	m_pPage = NULL;
}

void CFormatTable::Serialize(CArchive &ar)
{
	CFormat::Serialize(ar);
	if( ar.IsStoring( ) )
	{
		ar << m_nPrecision;
		ar << m_nTimeFrom;
		ar << m_nTimeTo;
		ar << m_nTimeStep;
		ar << m_bNeg;
		ar << m_bMin;
		ar << m_bMax;
		ar << m_clrMax;
		ar << m_clrMin;
		ar << m_clrNeg;
	}
	else
	{
		ar >> m_nPrecision;
		ar >> m_nTimeFrom;
		ar >> m_nTimeTo;
		ar >> m_nTimeStep;
		ar >> m_bNeg;
		ar >> m_bMin;
		ar >> m_bMax;
		ar >> m_clrMax;
		ar >> m_clrMin;
		ar >> m_clrNeg;
	}
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL( CFormatAnimation, CFormat, VERSIONABLE_SCHEMA | 2)

CFormatAnimation::CFormatAnimation()
{
	m_nAnimCat = 0;
	m_bFitToView = FALSE;
	otiRadio = NULL;
	otiChkFit = NULL;
}

CFormatAnimation::~CFormatAnimation()
{
	for(int i = 0; i < m_cObjSet.GetSize();i++ )
		delete m_cObjSet.GetAt(i);
	m_cObjSet.RemoveAll();

}

CFormatAnimation::CFormatAnimation(const CFormatAnimation& mFor): CFormat(mFor)
{
	m_nAnimCat = mFor.m_nAnimCat;
	m_bFitToView = mFor.m_bFitToView;

	for (int i=0;i<mFor.m_cObjSet.GetSize();i++)
	{
		CAnimObject *pObj = mFor.m_cObjSet.GetAt(i);
		if (!pObj) continue;

		CAnimObject *pNObj = pObj->Clone();
		if (pNObj) m_cObjSet.Add(pNObj);
	}

	CxAnimObjectList pList;
	CopyElt(&m_cObjSet,&pList);
	for (i=0;i<m_cObjSet.GetSize();i++)
	{
		CAnimObject *pObj = m_cObjSet.GetAt(i);
		//CAnimObject *pObj = pList.GetAt(i);
		if (pObj && (pObj->m_nRelatedID != -1))
		{
			//CAnimObject *pObj2 = pList.GetAt(pObj->m_nRelatedID);
			CAnimObject *pObj2 = m_cObjSet.GetAt(pObj->m_nRelatedID);
			pObj->m_pRelatedObj = pObj2;
		}
		CLinkObject *pLO = DYNAMIC_DOWNCAST(CLinkObject,pObj);
		if (pLO)
		{
			CAnimObject *p1=NULL;
			CAnimObject *p2=NULL;
			if (pLO->m_nIDInit != -1)
			{
				//CAnimObject *pObj2 = m_cObjSet.GetAt(pLO->m_nIDInit);
				p1 = pList.GetAt(pLO->m_nIDInit-1);
				//pLO->LinkToObjects(pObj2,NULL);
					//pLO->m_pInit = pObj2;
			}
			if (pLO->m_nIDEnd != -1)
			{
				//CAnimObject *pObj2 = m_cObjSet.GetAt(pLO->m_nIDEnd);
				p2 = pList.GetAt(pLO->m_nIDEnd-1);
				//pLO->m_pEnd = pObj2;
			}
			pLO->LinkToObjects(p1,p2);
		}
	}
}

CFormat* CFormatAnimation::Clone()
{
	CFormat* pFor=new CFormatAnimation(*this);
	return pFor;
}

CPropertyPage* CFormatAnimation::GetPage()
{
	if (!m_pPage)
	{
		/*CDlgFormatAnimation *pPage = new CDlgFormatAnimation();

		pPage->m_cObjSet = &m_cObjSet;			// List of objects
		//pPage->m_nAnimCat = m_nAnimCat;
		pPage->m_bFitToView = m_bFitToView;
		pPage->m_nDrawHyp = m_nAnimCat;*/

		m_bready = FALSE;
		CERFormatPage *pPage = new CERFormatPage(this);
		m_pPage = pPage;
	}
	return m_pPage;
}

void CFormatAnimation::UpdatePageContent(COptionTree *m_otTree)
{
	m_bready = FALSE;
	if (!m_otTree) return;

	int nb = m_cObjSet.GetSize();

	// Setup options tree
	COptionTreeItem *otiRoot = NULL;

	// -- CONTENT
	otiRoot = m_otTree->InsertItem(new COptionTreeItem());
	otiRoot->SetLabelText(_T("Content"));
	otiRoot->SetInfoText(_T("Define which of the components (e.g. legend, axes, etc.) are included in the ER."));
	
	// -- Radio Items -> Radio Items
	otiRadio = (COptionTreeItemRadio*)m_otTree->InsertItem(new COptionTreeItemRadio(), otiRoot);
	otiRadio->SetLabelText(_T("Hypothesis"));
	otiRadio->SetInfoText(_T("Define the way the hypothesis is displayed in the animation ER."));
	if (otiRadio->CreateRadioWindow() == TRUE)
	{
		otiRadio->InsertNewRadio(_T("On the right"), m_nAnimCat==0);
		otiRadio->InsertNewRadio(_T("On the bottom"), m_nAnimCat==1);
		otiRadio->InsertNewRadio(_T("As a new Exp. Set"), m_nAnimCat==2);
	}

	// -- Local Maximum
	otiChkFit = (COptionTreeItemCheckBox*)m_otTree->InsertItem(new COptionTreeItemCheckBox(), otiRoot);
	otiChkFit->SetLabelText(_T("Fit to view"));
	otiChkFit->SetInfoText(_T("Check the box to resize the component to the dimension of the ER."));
	if (otiChkFit->CreateCheckBoxWindow((nb==1) ? m_bFitToView : FALSE, TRUE, TRUE) == TRUE)
	{
		otiChkFit->SetCheckText(_T("Fit"), _T("Keep"));
		otiChkFit->ReadOnly(nb>1);
	}
	m_bready = TRUE;

}


void CFormatAnimation::UpdateFormat(BOOL bOK)
{
	if (!m_pPage) return;
	if (!m_bready) return;
	if (bOK)
	{
		/*CDlgFormatAnimation *pPage = (CDlgFormatAnimation *)m_pPage;
		//m_nAnimCat = pPage->m_nAnimCat;
		m_bFitToView = pPage->m_bFitToView;
		m_nAnimCat = pPage->m_nDrawHyp;*/

		m_nAnimCat = otiRadio->GetCheckedRadio();
		m_bFitToView = otiChkFit->GetCheck();
	}
	delete m_pPage;
	m_pPage = NULL;
}

void CFormatAnimation::Serialize(CArchive &ar)
{
	CFormat::Serialize(ar);
	if (ar.IsStoring())
	{	// storing code
		ar << m_nAnimCat;
		ar << m_bFitToView;
	}
	else
	{	// loading code
		int nb = ar.GetObjectSchema();
		ar >> m_nAnimCat;
		ar >> m_bFitToView;

		if (m_nAnimCat==-1) m_nAnimCat = 0;
	}

	m_cObjSet.Serialize(ar);
	if (ar.IsLoading())
	{
		CxAnimObjectList pList;
		CopyElt(&m_cObjSet,&pList);
		for (int i=0;i<m_cObjSet.GetSize();i++)
//		for (int i=0;i<pList.GetSize();i++)
		{
			CAnimObject *pObj = m_cObjSet.GetAt(i);
			//CAnimObject *pObj = pList.GetAt(i);
			if (pObj && (pObj->m_nRelatedID != -1))
			{
				//CAnimObject *pObj2 = pList.GetAt(pObj->m_nRelatedID);
				CAnimObject *pObj2 = m_cObjSet.GetAt(pObj->m_nRelatedID);
				pObj->m_pRelatedObj = pObj2;
			}
			CLinkObject *pLO = DYNAMIC_DOWNCAST(CLinkObject,pObj);
			if (pLO)
			{
				CAnimObject *p1=NULL;
				CAnimObject *p2=NULL;
				if (pLO->m_nIDInit != -1)
				{
					//CAnimObject *pObj2 = m_cObjSet.GetAt(pLO->m_nIDInit);
					p1 = pList.GetAt(pLO->m_nIDInit-1);
					//pLO->LinkToObjects(pObj2,NULL);

					//pLO->m_pInit = pObj2;
				}
				if (pLO->m_nIDEnd != -1)
				{
					//CAnimObject *pObj2 = m_cObjSet.GetAt(pLO->m_nIDEnd);
					p2 = pList.GetAt(pLO->m_nIDEnd-1);
					//pLO->m_pEnd = pObj2;
				}
				pLO->LinkToObjects(p1,p2);
			}

			CPolygonObject *pLOP = DYNAMIC_DOWNCAST(CPolygonObject,pObj);
			if (pLOP)
			{
				pLOP->LinkToObjects(&pList);
			}

		}
	}
}

int	CFormatAnimation::SetupVisualList(CxAnimObjectList& myList)
{
	return myList.GetSize();
}

int CFormatAnimation::AddObject(CAnimObject*)
{
	return m_cObjSet.GetSize();
}

int CFormatAnimation::RemoveObject(CAnimObject* pObj)
{
	return m_cObjSet.GetSize();
}

int	CFormatAnimation::CopyObjects(CxAnimObjectList& myList)
{
	return m_cObjSet.GetSize();
}

int CFormatAnimation::PasteObjects(CxAnimObjectList& myList)
{
	return m_cObjSet.GetSize();
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL( CFormatGraph, CFormat, VERSIONABLE_SCHEMA | 2)

CFormatGraph::CFormatGraph()
{
	m_bAutoScale = TRUE;
	m_bShowAxes = FALSE;
	m_bShowLegend = FALSE;
	m_nXmax = 0;
	m_nXmin = 0;
	m_nYmax = 0.0;
	m_nYmin = 0.0;

	otiEditXMin = NULL;
	otiEditXMax = NULL;
	otiEditYMin = NULL;
	otiEditYMax = NULL;
	otiChkAxis = NULL;
	otiChkLeg = NULL;
	otiChkScale = NULL;
	oitCbbType = NULL;

}

CFormatGraph::~CFormatGraph()
{

}

CFormatGraph::CFormatGraph(const CFormatGraph& mFor): CFormat(mFor)
{
	m_bAutoScale = mFor.m_bAutoScale;
	m_bShowAxes = mFor.m_bShowAxes;
	m_bShowLegend = mFor.m_bShowLegend;
	m_nXmax = mFor.m_nXmax;
	m_nXmin = mFor.m_nXmin;
	m_nYmax = mFor.m_nYmax;
	m_nYmin = mFor.m_nYmin;
}

CFormat* CFormatGraph::Clone()
{
	CFormat* pFor=new CFormatGraph(*this);
	return pFor;
}


CPropertyPage* CFormatGraph::GetPage()
{
	if (!m_pPage)
	{
		/*CDlgFormatGraph *pPage = new CDlgFormatGraph(m_nMaxTime);

		//pPage->m_bAutoScale = m_bAutoScale;
		pPage->m_bAutoScale = m_bAutoScale;
		pPage->m_bShowAxes=m_bShowAxes;
		pPage->m_bShowLegend=m_bShowLegend;
		pPage->m_nXmax=m_nXmax;
		pPage->m_nXmin=m_nXmin;
		pPage->m_nYmax=m_nYmax;
		pPage->m_nYmin=m_nYmin;*/

		m_bready = FALSE;
		CERFormatPage *pPage = new CERFormatPage(this);
		m_pPage = pPage;
	}
	return m_pPage;
}

void CFormatGraph::UpdatePageContent(COptionTree *m_otTree)
{
	m_bready = FALSE;
	if (!m_otTree) return;

	// Setup options tree
	COptionTreeItem *otiRoot = NULL;
	COptionTreeItem *otiRoot2 = NULL;

	// -- CONTENT
	otiRoot = m_otTree->InsertItem(new COptionTreeItem());
	otiRoot->SetLabelText(_T("Content"));
	otiRoot->SetInfoText(_T("Define which of the components (e.g. legend, axes, etc.) are included in the ER."));
	
	// -- Axes
	otiChkAxis = (COptionTreeItemCheckBox*)m_otTree->InsertItem(new COptionTreeItemCheckBox(), otiRoot);
	otiChkAxis->SetLabelText(_T("Axes"));
	otiChkAxis->SetInfoText(_T("Check the box to display the axes in the ER."));
	if (otiChkAxis->CreateCheckBoxWindow(m_bShowAxes, TRUE, TRUE) == TRUE)
	{
		otiChkAxis->SetCheckText(_T("Displayed"), _T("Hidden"));
	}
	// -- Legend
	otiChkLeg = (COptionTreeItemCheckBox*)m_otTree->InsertItem(new COptionTreeItemCheckBox(), otiRoot);
	otiChkLeg->SetLabelText(_T("Legend"));
	otiChkLeg->SetInfoText(_T("Check the box to display the legend in the ER."));
	if (otiChkLeg->CreateCheckBoxWindow(m_bShowLegend, TRUE, TRUE) == TRUE)
	{
		otiChkLeg->SetCheckText(_T("Displayed"), _T("Hidden"));
	}

	// -- SCALE
	otiRoot = m_otTree->InsertItem(new COptionTreeItem());
	otiRoot->SetLabelText(_T("Scale"));
	otiRoot->SetInfoText(_T("Define the scale of the graph."));

	otiRoot2 = otiRoot;

	// -- Scale Type
	oitCbbType = (COptionTreeItemComboBox*)m_otTree->InsertItem(new COptionTreeItemComboBox(), otiRoot);
	oitCbbType->SetLabelText(_T("Y-Axis Scale"));
	oitCbbType->SetInfoText(_T("Select the type of scale of the Y-axis."));
	if (oitCbbType->CreateComboWindow(0) == TRUE)
	{
		oitCbbType->AddString("Linear");
		oitCbbType->AddString("Logarithmic");
		//oitCbbType->SelectString(0, "Blue");
		oitCbbType->SetCurSel(m_bAutoScale);
	}

	// -- Autoscale
	otiChkScale = (COptionTreeItemCheckBox*)m_otTree->InsertItem(new COptionTreeItemCheckBox(), otiRoot);
	otiChkScale->SetLabelText(_T("Autoscale"));
	otiChkScale->SetInfoText(_T("Check the box to have the graph scaled automatically."));
	if (otiChkScale->CreateCheckBoxWindow(TRUE, TRUE, TRUE) == TRUE)
	{
		otiChkScale->SetCheckText(_T("Yes"), _T("No"));
		otiChkScale->ReadOnly(TRUE);
	}

	// -- SCALE : X-A
	otiRoot = m_otTree->InsertItem(new COptionTreeItem(),otiRoot2);
	otiRoot->SetLabelText(_T("X-Axis (Time)"));
	otiRoot->SetInfoText(_T("Define the scale of the Time axis (the X-axis)."));
	otiRoot->ReadOnly(TRUE);

	// -- From
	otiEditXMin = (COptionTreeItemEdit*) m_otTree->InsertItem(new COptionTreeItemEdit(), otiRoot);
	otiEditXMin->SetLabelText(_T("From"));
	otiEditXMin->SetInfoText(_T("Set the minimum data point to be displayed on the X-axis."));
	if (otiEditXMin->CreateEditWindow(FALSE, FALSE, TRUE, 0) == TRUE)
	{
		otiEditXMin->SetEditInt(m_nXmin);
		otiEditXMin->ReadOnly(TRUE);
	}
	// -- To
	otiEditXMax = (COptionTreeItemEdit*) m_otTree->InsertItem(new COptionTreeItemEdit(), otiRoot);
	otiEditXMax->SetLabelText(_T("To"));
	otiEditXMax->SetInfoText(_T("Set the maximum data point to be displayed on the X-axis."));
	if (otiEditXMax->CreateEditWindow(FALSE, FALSE, TRUE, 0) == TRUE)
	{
		otiEditXMax->SetEditInt(m_nXmax);
		otiEditXMax->ReadOnly(TRUE);
	}
	// -- SCALE : Y-Axis
	otiRoot = m_otTree->InsertItem(new COptionTreeItem(),otiRoot2);
	otiRoot->SetLabelText(_T("Y-Axis (Data)"));
	otiRoot->SetInfoText(_T("Define the scale of the data axis (the Y-axis)."));
	otiRoot->ReadOnly(TRUE);

	// -- From
	otiEditYMin = (COptionTreeItemEdit*) m_otTree->InsertItem(new COptionTreeItemEdit(), otiRoot);
	otiEditYMin->SetLabelText(_T("From"));
	otiEditYMin->SetInfoText(_T("Set the minimum data point to be displayed on the Y-axis."));
	if (otiEditYMin->CreateEditWindow(FALSE, FALSE, TRUE, 0) == TRUE)
	{
		otiEditYMin->SetEditDouble(m_nYmin);
		otiEditYMin->ReadOnly(TRUE);
	}
	// -- To
	otiEditYMax = (COptionTreeItemEdit*) m_otTree->InsertItem(new COptionTreeItemEdit(), otiRoot);
	otiEditYMax->SetLabelText(_T("To"));
	otiEditYMax->SetInfoText(_T("Set the maximum data point to be displayed on the Y-axis."));
	if (otiEditYMax->CreateEditWindow(FALSE, FALSE, TRUE, 0) == TRUE)
	{
		otiEditYMax->SetEditDouble(m_nYmax);
		otiEditYMax->ReadOnly(TRUE);
	}
	m_bready = TRUE;

}


void CFormatGraph::UpdateFormat(BOOL bOK)
{
	if (!m_pPage) return;
	if (!m_bready) return;
	if (bOK)
	{
		/*CDlgFormatGraph *pPage = (CDlgFormatGraph *)m_pPage;
		//m_bAutoScale = pPage->m_bAutoScale;
		m_bAutoScale = pPage->m_bAutoScale;
		m_bShowAxes=pPage->m_bShowAxes;
		m_bShowLegend=pPage->m_bShowLegend;
		m_nXmax=pPage->m_nXmax;
		m_nXmin=pPage->m_nXmin;
		m_nYmax=pPage->m_nYmax;
		m_nYmin=pPage->m_nYmin;*/
		m_bShowAxes = otiChkAxis->GetCheck();
		m_bShowLegend = otiChkLeg->GetCheck();
		m_bAutoScale = oitCbbType->GetCurSel();

		int nb = m_nXmin;
		if (otiEditXMin->GetEditInt(nb))	m_nXmin = nb;
		nb = m_nXmax;
		if (otiEditXMax->GetEditInt(nb))	m_nXmax = nb;
		double nnb = m_nYmin;
		if (otiEditXMin->GetEditDouble(nnb))	m_nYmin = nnb;
		nnb = m_nYmax;
		if (otiEditYMax->GetEditDouble(nnb))	m_nYmax = nnb;

	}
	delete m_pPage;
	m_pPage = NULL;
}



void CFormatGraph::Serialize(CArchive &ar)
{
	CFormat::Serialize(ar);
	if (ar.IsStoring())
	{
		//ar << m_TimerMax;
		ar << m_bAutoScale;
		ar << m_bShowAxes;
		ar << m_bShowLegend;
		ar << m_nXmax;
		ar << m_nXmin;
		ar << m_nYmax;
		ar << m_nYmin;
	}
	else
	{	// loading code
		//ar >> m_TimerMax;
		ar >> m_bAutoScale;
		ar >> m_bShowAxes;
		ar >> m_bShowLegend;
		ar >> m_nXmax;
		ar >> m_nXmin;
		ar >> m_nYmax;
		ar >> m_nYmin;
	}
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL( CFormatPhasePlot, CFormat, VERSIONABLE_SCHEMA | 2)

CFormatPhasePlot::CFormatPhasePlot()
{
	m_bShowAxe = TRUE;
	m_bShowCurrTime = TRUE;
	m_bShowIsocline = FALSE;
	m_bShowLegend = TRUE;
	m_bShowStab = TRUE;
	m_nScale = 0;

	m_nIsoX0 = -1;
	m_nIsoXN = -1;
	m_nIsoY0 = -1;
	m_nIsoYN = -1;

	otiEditXMin = NULL;
	otiEditXMax = NULL;
	otiEditYMin = NULL;
	otiEditYMax = NULL;
	otiChkAxis = NULL;
	otiChkLeg = NULL;
	otiChkScale = NULL;
	oitCbbType = NULL;
}

CFormatPhasePlot::~CFormatPhasePlot()
{

}

CFormatPhasePlot::CFormatPhasePlot(const CFormatPhasePlot& mFor): CFormat(mFor)
{
	m_bShowAxe = mFor.m_bShowAxe;
	m_bShowCurrTime = mFor.m_bShowCurrTime;
	m_bShowIsocline = mFor.m_bShowIsocline;
	m_bShowLegend = mFor.m_bShowLegend;
	m_bShowStab = mFor.m_bShowStab;
	m_nScale = mFor.m_nScale;
	m_nIsoX0 = mFor.m_nIsoX0;
	m_nIsoXN = mFor.m_nIsoXN;
	m_nIsoY0 = mFor.m_nIsoY0;
	m_nIsoYN = mFor.m_nIsoYN;
}

CFormat* CFormatPhasePlot::Clone()
{
	CFormat* pFor=new CFormatPhasePlot(*this);
	return pFor;
}

CPropertyPage* CFormatPhasePlot::GetPage()
{
	if (!m_pPage)
	{
		/*CDlgFormatPhasePlot *pPage = new CDlgFormatPhasePlot();

		pPage->m_bShowAxe = m_bShowAxe;
		pPage->m_bShowCurrTime=m_bShowCurrTime;
		pPage->m_bShowIsocline=m_bShowIsocline;
		pPage->m_bShowLegend=m_bShowLegend;
		pPage->m_bShowStab=m_bShowStab;
		pPage->m_nScale=m_nScale;*/

		m_bready = FALSE;
		CERFormatPage *pPage = new CERFormatPage(this);

		m_pPage = pPage;
	}
	return m_pPage;
}

void CFormatPhasePlot::UpdatePageContent(COptionTree *m_otTree)
{
	m_bready = FALSE;
	if (!m_otTree) return;

	double m_nXmin = 0;
	double m_nXmax = 0;
	double m_nYmin = 0;
	double m_nYmax = 0;

	// Setup options tree
	COptionTreeItem *otiRoot = NULL;
	COptionTreeItem *otiRoot2 = NULL;

	// -- CONTENT
	otiRoot = m_otTree->InsertItem(new COptionTreeItem());
	otiRoot->SetLabelText(_T("Content"));
	otiRoot->SetInfoText(_T("Define which of the components (e.g. legend, axes, etc.) are included in the ER."));
	
	// -- Axes
	otiChkAxis = (COptionTreeItemCheckBox*)m_otTree->InsertItem(new COptionTreeItemCheckBox(), otiRoot);
	otiChkAxis->SetLabelText(_T("Axes"));
	otiChkAxis->SetInfoText(_T("Check the box to display the axes in the ER."));
	if (otiChkAxis->CreateCheckBoxWindow(m_bShowAxe, TRUE, TRUE) == TRUE)
	{
		otiChkAxis->SetCheckText(_T("Displayed"), _T("Hidden"));
	}
	// -- Legend
	otiChkLeg = (COptionTreeItemCheckBox*)m_otTree->InsertItem(new COptionTreeItemCheckBox(), otiRoot);
	otiChkLeg->SetLabelText(_T("Legend"));
	otiChkLeg->SetInfoText(_T("Check the box to display the legend in the ER."));
	if (otiChkLeg->CreateCheckBoxWindow(m_bShowLegend, TRUE, TRUE) == TRUE)
	{
		otiChkLeg->SetCheckText(_T("Displayed"), _T("Hidden"));
	}

	// -- SCALE
	otiRoot = m_otTree->InsertItem(new COptionTreeItem());
	otiRoot->SetLabelText(_T("Scale"));
	otiRoot->SetInfoText(_T("Define the scale of the graph."));

	otiRoot2 = otiRoot;

	// -- Scale Type
	oitCbbType = (COptionTreeItemComboBox*)m_otTree->InsertItem(new COptionTreeItemComboBox(), otiRoot);
	oitCbbType->SetLabelText(_T("Axes Scale"));
	oitCbbType->SetInfoText(_T("Select the type of scale of the X-axis and the Y-axis."));
	if (oitCbbType->CreateComboWindow(0) == TRUE)
	{
		oitCbbType->AddString("XY - Linear");
		oitCbbType->AddString("X  - Logarithmic");
		oitCbbType->AddString("Y  - Logarithmic");
		oitCbbType->AddString("XY - Logarithmic");
		//oitCbbType->SelectString(0, "Blue");
		oitCbbType->SetCurSel(m_nScale);
	}

	// -- Autoscale
	otiChkScale = (COptionTreeItemCheckBox*)m_otTree->InsertItem(new COptionTreeItemCheckBox(), otiRoot);
	otiChkScale->SetLabelText(_T("Autoscale"));
	otiChkScale->SetInfoText(_T("Check the box to have the graph scaled automatically."));
	if (otiChkScale->CreateCheckBoxWindow(TRUE, TRUE, TRUE) == TRUE)
	{
		otiChkScale->SetCheckText(_T("Yes"), _T("No"));
		otiChkScale->ReadOnly(TRUE);
	}

	// -- SCALE : X-A
	otiRoot = m_otTree->InsertItem(new COptionTreeItem(),otiRoot2);
	otiRoot->SetLabelText(_T("X-Axis (Time)"));
	otiRoot->SetInfoText(_T("Define the scale of the Time axis (the X-axis)."));

	// -- From
	otiEditXMin = (COptionTreeItemEdit*) m_otTree->InsertItem(new COptionTreeItemEdit(), otiRoot);
	otiEditXMin->SetLabelText(_T("From"));
	otiEditXMin->SetInfoText(_T("Set the minimum data point to be displayed on the X-axis."));
	if (otiEditXMin->CreateEditWindow(FALSE, FALSE, TRUE, 0) == TRUE)
	{
		otiEditXMin->SetEditDouble(m_nXmin);
		otiEditXMin->ReadOnly(TRUE);
	}
	// -- To
	otiEditXMax = (COptionTreeItemEdit*) m_otTree->InsertItem(new COptionTreeItemEdit(), otiRoot);
	otiEditXMax->SetLabelText(_T("To"));
	otiEditXMax->SetInfoText(_T("Set the maximum data point to be displayed on the X-axis."));
	if (otiEditXMax->CreateEditWindow(FALSE, FALSE, TRUE, 0) == TRUE)
	{
		otiEditXMax->SetEditDouble(m_nXmax);
		otiEditXMax->ReadOnly(TRUE);
	}
	// -- SCALE : Y-Axis
	otiRoot = m_otTree->InsertItem(new COptionTreeItem(),otiRoot2);
	otiRoot->SetLabelText(_T("Y-Axis (Data)"));
	otiRoot->SetInfoText(_T("Define the scale of the data axis (the Y-axis)."));

	// -- From
	otiEditYMin = (COptionTreeItemEdit*) m_otTree->InsertItem(new COptionTreeItemEdit(), otiRoot);
	otiEditYMin->SetLabelText(_T("From"));
	otiEditYMin->SetInfoText(_T("Set the minimum data point to be displayed on the Y-axis."));
	if (otiEditYMin->CreateEditWindow(FALSE, FALSE, TRUE, 0) == TRUE)
	{
		otiEditYMin->SetEditDouble(m_nYmin);
		otiEditYMin->ReadOnly(TRUE);
	}
	// -- To
	otiEditYMax = (COptionTreeItemEdit*) m_otTree->InsertItem(new COptionTreeItemEdit(), otiRoot);
	otiEditYMax->SetLabelText(_T("To"));
	otiEditYMax->SetInfoText(_T("Set the maximum data point to be displayed on the Y-axis."));
	if (otiEditYMax->CreateEditWindow(FALSE, FALSE, TRUE, 0) == TRUE)
	{
		otiEditYMax->SetEditDouble(m_nYmax);
		otiEditYMax->ReadOnly(TRUE);
	}
	m_bready = TRUE;

}

void CFormatPhasePlot::UpdateFormat(BOOL bOK)
{
	if (!m_pPage) return;
	if (!m_bready) return;
	if (bOK)
	{
	/*	CDlgFormatPhasePlot *pPage = (CDlgFormatPhasePlot*)m_pPage;

		m_bShowAxe = pPage->m_bShowAxe;
		m_bShowCurrTime=pPage->m_bShowCurrTime;
		m_bShowIsocline=pPage->m_bShowIsocline;
		m_bShowLegend=pPage->m_bShowLegend;
		m_bShowStab=pPage->m_bShowStab;
		m_nScale=pPage->m_nScale;*/

		m_bShowAxe = otiChkAxis->GetCheck();
		m_bShowLegend = otiChkLeg->GetCheck();
		m_nScale = oitCbbType->GetCurSel();

	}
	delete m_pPage;
	m_pPage = NULL;
}



void CFormatPhasePlot::Serialize(CArchive &ar)
{
	CFormat::Serialize(ar);
	if( ar.IsStoring( ) )
	{
		ar << m_bShowAxe;
		ar << m_bShowCurrTime;
		ar << m_bShowIsocline;
		ar << m_bShowLegend;
		ar << m_bShowStab;
		ar << m_nScale ;
		ar << m_nIsoX0;
		ar << m_nIsoXN;
		ar << m_nIsoY0;
		ar << m_nIsoYN;
	}
	else
	{
		//int nb = ar.GetObjectSchema();
		ar >> m_bShowAxe;
		ar >> m_bShowCurrTime;
		ar >> m_bShowIsocline;
		ar >> m_bShowLegend;
		ar >> m_bShowStab;
		ar >> m_nScale ;
		//if (nb==2)
		{
			ar >> m_nIsoX0;
			ar >> m_nIsoXN;
			ar >> m_nIsoY0;
			ar >> m_nIsoYN;
		}
	}
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL( CFormatText, CFormat, VERSIONABLE_SCHEMA | 2)

CFormatText::CFormatText()
{
	m_nDisplayMode = 0;
	m_clrVar = RGB(0,0,255);
	m_clrParam = RGB(128,0,128);
	m_clrCst = RGB(0,128,0);

}

CFormatText::~CFormatText()
{

}

CFormatText::CFormatText(const CFormatText& mFor): CFormat(mFor)
{
	m_nDisplayMode = mFor.m_nDisplayMode;
}

CFormat* CFormatText::Clone()
{
	CFormat* pFor=new CFormatText(*this);
	return pFor;
}

CPropertyPage* CFormatText::GetPage()
{
	if (!m_pPage)
	{
		CDlgFormatText *pPage = new CDlgFormatText();

		pPage->m_nDisplayMode = m_nDisplayMode;
		pPage->m_clrVar = m_clrVar;
		pPage->m_clrParam = m_clrParam;
		pPage->m_clrCst = m_clrCst;

		m_pPage = pPage;
	}
	return m_pPage;
}

void CFormatText::UpdateFormat(BOOL bOK)
{
	if (!m_pPage) return;
	if (bOK)
	{
		CDlgFormatText *pPage = (CDlgFormatText*)m_pPage;

		m_nDisplayMode = pPage->m_nDisplayMode;
		m_clrVar = pPage->m_clrVar;
		m_clrParam = pPage->m_clrParam;
		m_clrCst = pPage->m_clrCst;
	}
	delete m_pPage;
	m_pPage = NULL;
}

void CFormatText::UpdatePageContent(COptionTree *m_otTree)
{
	CFormat::UpdatePageContent(m_otTree);
}



void CFormatText::Serialize(CArchive &ar)
{
	CFormat::Serialize(ar);
	if( ar.IsStoring( ) )
	{
		ar << m_nDisplayMode;
	}
	else
	{
		ar >> m_nDisplayMode;
	}
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL( CFormatChart, CFormat, VERSIONABLE_SCHEMA | 2)

CFormatChart::CFormatChart()
{
	m_nChartKind = 0;
	otiChkLeg = NULL;
}

CFormatChart::~CFormatChart()
{

}

CFormatChart::CFormatChart(const CFormatChart& mFor): CFormat(mFor)
{
	m_nChartKind = mFor.m_nChartKind;
}

CFormat* CFormatChart::Clone()
{
	CFormat* pFor=new CFormatChart(*this);
	return pFor;
}

CPropertyPage* CFormatChart::GetPage()
{
	if (!m_pPage)
	{
		//CDlgFormatChart *pPage = new CDlgFormatChart();
		//pPage->m_nChartKind = m_nChartKind;
		m_bready = FALSE;
		CERFormatPage *pPage = new CERFormatPage(this);

		m_pPage = pPage;
	}
	return m_pPage;
}

void CFormatChart::UpdatePageContent(COptionTree *m_otTree)
{
	m_bready = FALSE;
	if (!m_otTree) return;

	// Setup options tree
	COptionTreeItem *otiRoot = NULL;


	// -- CONTENT
	otiRoot = m_otTree->InsertItem(new COptionTreeItem());
	otiRoot->SetLabelText(_T("Content"));
	otiRoot->SetInfoText(_T("Define which of the components (e.g. legend, axes, etc.) are included in the ER."));
	
	// -- Legend
	otiChkLeg = (COptionTreeItemCheckBox*)m_otTree->InsertItem(new COptionTreeItemCheckBox(), otiRoot);
	otiChkLeg->SetLabelText(_T("Legend"));
	otiChkLeg->SetInfoText(_T("Check the box to have the legend displayed in the ER."));
	if (otiChkLeg->CreateCheckBoxWindow(TRUE, TRUE, TRUE) == TRUE)
	{
		otiChkLeg->SetCheckText(_T("Displayed"), _T("Hidden"));
		otiChkLeg->ReadOnly(TRUE);
	}
	m_bready = TRUE;
}


void CFormatChart::UpdateFormat(BOOL bOK)
{
	if (!m_pPage) return;
	if (!m_bready) return;
	if (bOK)
	{
		//CDlgFormatChart *pPage = (CDlgFormatChart *) m_pPage;
		//m_nChartKind = pPage->m_nChartKind;
	}
	delete m_pPage;
	m_pPage = NULL;
}

void CFormatChart::Serialize(CArchive &ar)
{
	CFormat::Serialize(ar);
	if( ar.IsStoring( ) )
	{
		ar << m_nChartKind;
	}
	else
	{
		ar >> m_nChartKind;
	}
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL( CFormatTranslation, CFormat, VERSIONABLE_SCHEMA | 2)

CFormatTranslation::CFormatTranslation()
{
	m_nTranslation = TRANS_INDEP;
}

CFormatTranslation::~CFormatTranslation()
{

}

CFormatTranslation::CFormatTranslation(const CFormatTranslation& mFor): CFormat(mFor)
{
	m_nTranslation = mFor.m_nTranslation;
}

CFormat* CFormatTranslation::Clone()
{
	CFormat* pFor=new CFormatTranslation(*this);
	return pFor;
}

CPropertyPage* CFormatTranslation::GetPage()
{
	if (!m_pPage)
	{
		CERTranslationPage *pPage = new CERTranslationPage();

		pPage->m_nTranslation = m_nTranslation;
		m_pPage = pPage;
	}
	return m_pPage;
}

void CFormatTranslation::UpdateFormat(BOOL bOK)
{
	if (!m_pPage) return;
	if (bOK)
	{
		CERTranslationPage *pPage = (CERTranslationPage*)m_pPage;

		m_nTranslation = pPage->m_nTranslation;
	}
	delete m_pPage;
	m_pPage = NULL;
}



void CFormatTranslation::Serialize(CArchive &ar)
{
	CFormat::Serialize(ar);
	if (ar.IsStoring())
	{	// storing code
		ar << m_nTranslation;
	}
	else
	{	// loading code
		ar >> m_nTranslation;
	}
}
