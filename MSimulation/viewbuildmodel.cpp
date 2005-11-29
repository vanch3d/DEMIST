// ViewBuildModel.cpp : implementation file
//

#include "stdafx.h"
#include "simul.h"
#include "SimulDoc.h"
#include "ViewBuildModel.h"


#include "BuildModelDlg.h"

#include <MathParser\MathParser.h>

#include <MSimulation\PreyPredModel.h>
#include <MSimulation\AuthorModel.h>

#include <MInstruction\LearningUnit.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CViewSDIbuild

IMPLEMENT_DYNCREATE(CViewSDIbuild, CFormView)

CViewSDIbuild::CViewSDIbuild()
	: CFormView(CViewSDIbuild::IDD)
{
	//{{AFX_DATA_INIT(CViewSDIbuild)
	//}}AFX_DATA_INIT

	m_nPrevMod = -1;
	m_bCanProceed = FALSE;
	m_ImageIcon.Create(IDB_MODEL_DESIGN,16,0,RGB(192,192,192));

}

CViewSDIbuild::~CViewSDIbuild()
{
}

void CViewSDIbuild::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CViewSDIbuild)
	DDX_Control(pDX, IDC_BMODEL_PROP, m_cProp);
	DDX_Control(pDX, IDC_BMODEL_EQS, m_wndEditEqn);
	DDX_Control(pDX, IDC_BMODEL_VAL, m_cCheck);
	//}}AFX_DATA_MAP
	if (pDX->m_bSaveAndValidate)
	{
		CString mstr;
		m_wndEditEqn.GetWindowText(mstr);
		int nCurrModel = GetDocument()->m_nModSelItem;
		CMdEquation* pEq = GetDocument()->m_pSimul->GetEquation(nCurrModel);
		if (!pEq) return;
		CPP_SimModel *pSim = DYNAMIC_DOWNCAST(CPP_SimModel,pEq);
		if (!pSim) return;

		pSim->m_strModel = mstr;
	}
	else
	{
	}
}


BEGIN_MESSAGE_MAP(CViewSDIbuild, CFormView)
	//{{AFX_MSG_MAP(CViewSDIbuild)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BMODEL_VAL, OnCheckEquation)
	ON_BN_CLICKED(IDC_BMODEL_PROP, OnSetSimulationProp)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEdit)
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
	ON_WM_CONTEXTMENU()
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEdit)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, OnUpdateEdit)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, OnUpdateEdit)
	ON_COMMAND(ID_EDIT_SELECT_ALL, OnEditSelectAll)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewSDIbuild diagnostics

#ifdef _DEBUG
void CViewSDIbuild::AssertValid() const
{
	CFormView::AssertValid();
}

void CViewSDIbuild::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CSimulDoc* CViewSDIbuild::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSimulDoc)));
	return (CSimulDoc*)m_pDocument;
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CViewSDIbuild message handlers
void CViewSDIbuild::EnableEdition(BOOL bEnab)
{
	m_wndEditEqn.SetAllowChange(bEnab);
	m_wndEditEqn.SetBackgroundColor(FALSE,(bEnab) ? ::GetSysColor(COLOR_WINDOW) : ::GetSysColor(COLOR_BTNFACE));
	m_cCheck.EnableWindow(bEnab);
	m_cProp.EnableWindow(bEnab);
}

void CViewSDIbuild::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();

	m_cCheck.SetIcon(m_ImageIcon.ExtractIcon(2));
	//m_cCheck.SetTooltipText(ID_CONTR_TASK);
	m_cCheck.SetShowText(TRUE);

	m_cProp.SetIcon(m_ImageIcon.ExtractIcon(3));
	//m_cProp.SetTooltipText(ID_CONTR_TASK);
	m_cProp.SetShowText(TRUE);


	COLORREF	m_clrVar = RGB(0,0,255),
				m_clrParam = RGB(0,0,0),
				m_clrCst = RGB(0,0,0);

	CHARFORMAT cfDefault;
	m_wndEditEqn.GetDefaultCharFormat(cfDefault);
	cfDefault.cbSize = sizeof(cfDefault);
	cfDefault.dwEffects = CFE_PROTECTED; 
	cfDefault.dwMask = CFM_BOLD | CFM_FACE | CFM_SIZE | CFM_CHARSET | CFM_PROTECTED;
	cfDefault.yHeight = 180;
	cfDefault.bCharSet = 0xEE; 
	strcpy(cfDefault.szFaceName, _T("Arial")); 

	// TODO: Add your specialized code here and/or call the base class
	m_wndEditEqn.Initialize();


	m_wndEditEqn.SetDefaultCharFormat(cfDefault);
	

	m_wndEditEqn.SetCaseSensitive(FALSE);
	m_wndEditEqn.SetStringQuotes(_T("\""));
	m_wndEditEqn.SetSLComment(_T(';'));

	m_wndEditEqn.SetConstantColor(m_clrVar, TRUE);
	m_wndEditEqn.SetKeywordColor(m_clrParam, TRUE);
	m_wndEditEqn.SetNumberColor(m_clrCst, TRUE);
	//m_wndEditEqn.SetCommentColor(m_clrCst, TRUE);
	

	CString sKeyword(_T(" "));
	sKeyword += _T("Pi cos sin tan atan asin acos exp ln lg sqrt abs fraq trunc floor ceil round sgn neg ");
	m_wndEditEqn.AddKeywords(sKeyword);

		PARAFORMAT pf;
	pf.cbSize = sizeof(PARAFORMAT);
	pf.dwMask = PFM_TABSTOPS ;
	pf.cTabCount = 2;
	pf.rgxTabs[0] = 5 * 1440/5 ;
	pf.rgxTabs[1] = 10 * 1440/5 ;

	m_wndEditEqn.SetParaFormat( pf );

	m_bCanProceed= TRUE;

	CRect mrect;
	GetClientRect(mrect);
	m_wndEditEqn.SetWindowPos(NULL,0,0,mrect.Width()-12,mrect.Height()-44,SWP_NOMOVE|SWP_NOACTIVATE);

	m_wndEditEqn.SetOptions(ECOOP_OR  ,ECO_NOHIDESEL   );
	long lMask = m_wndEditEqn.GetEventMask();
	lMask |= ENM_CHANGE;
	m_wndEditEqn.SetEventMask(lMask);

	int nCurrModel = GetDocument()->m_nModSelItem;

	if (m_nPrevMod == nCurrModel) return;
	CMdEquation* pEq = GetDocument()->m_pSimul->GetEquation(nCurrModel);
	if (!pEq) 
	{
		EnableEdition(FALSE);
		return;
	}

	CPP_SimModel *pSim = DYNAMIC_DOWNCAST(CPP_SimModel,pEq);
	m_nPrevMod = nCurrModel;

	EnableEdition((BOOL)pSim);
}

/////////////////////////////////////////////////////////////////////////////
/// The framework calls this function after receiving a notification message.
///
/// \param pSender Points to the view that modified the document, or NULL if all views are to be updated.
/// \param lHint Contains information about the modifications.
/// \param pHint Points to an object storing information about the modifications.
///
/// This member function proceeds and/or dispatches all the messages sent by the framework
/// to the relevant function.
/////////////////////////////////////////////////////////////////////////////
void CViewSDIbuild::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	// TODO: Add your specialized code here and/or call the base class
	// TODO: Add your specialized code here and/or call the base class
	int nCurrModel = GetDocument()->m_nModSelItem;

	if (m_nPrevMod == nCurrModel) return;
	CMdEquation* pEq = GetDocument()->m_pSimul->GetEquation(nCurrModel);
	if (!pEq) return;

	CPP_SimModel *pSim = DYNAMIC_DOWNCAST(CPP_SimModel,pEq);

	EnableEdition((BOOL)pSim);
	if (pSim)
	{
		m_wndEditEqn.ClearConstants();

		CString strCst(_T(" "));
		int nvar = pSim->m_cVarSet.GetSize();
		int npar = pSim->m_cParSet.GetSize();
		for(int i = 0; i < nvar;i++ )
		{
			CModelVar *pVar = pSim->m_cVarSet.GetAt(i);
			if (!pVar) continue;

			strCst +=pVar->GetAbbrev() + _T(" ");

			if (pVar->GetInitValue() !=-1)
			{
				strCst += _T("_") + pVar->GetAbbrev() + _T(" ");
				strCst += pVar->GetAbbrev() + _T("_INIT ");
			}

		}
		for (i = 0; i < npar;i++ )
		{
			CModelPar *pVar = pSim->m_cParSet.GetAt(i);
			if (!pVar) continue;

			strCst +=pVar->GetAbbrev() + _T(" ");

		}
		m_wndEditEqn.AddConstants(strCst);
		m_wndEditEqn.SetWindowText(pSim->m_strModel);
		m_wndEditEqn.FormatAll();
	}
	else
		m_wndEditEqn.SetWindowText(_T(""));


	m_nPrevMod = nCurrModel;
	//Invalidate();
}


void CViewSDIbuild::OnSize(UINT nType, int cx, int cy) 
{
	CFormView::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if (!m_bCanProceed) return;
	//if (m_wndEditEqn.GetSafeHwnd())
	{
		CRect mrect;
		m_wndEditEqn.GetClientRect(mrect);
	//if (::IsWindow(m_wndEditEqn.m_hWnd))
		m_wndEditEqn.SetWindowPos(NULL,0,0,cx-12,cy-44,SWP_NOMOVE|SWP_NOACTIVATE|SWP_NOZORDER);
	}

	
}


void CViewSDIbuild::OnSetSimulationProp() 
{

	// TODO: Add your control notification handler code here
	int nCurrModel = GetDocument()->m_nModSelItem;

	//if (m_nPrevMod == nCurrModel) return;
	CMdEquation* pEq = GetDocument()->m_pSimul->GetEquation(nCurrModel);
	if (!pEq) return;

	CPP_SimModel *pSim = DYNAMIC_DOWNCAST(CPP_SimModel,pEq);
	if (!pSim) return;


	CBuildModelDlg dlg;

	dlg.m_cNumMethod = pSim->m_nNumApprox;
	dlg.m_nCoeff = pSim->m_nEulerH;

	if (dlg.DoModal()==IDOK)
	{
		pSim->m_nEulerH = dlg.m_nCoeff;
		pSim->m_nNumApprox = dlg.m_cNumMethod;
	}

	
}


void CViewSDIbuild::OnCheckEquation() 
{
	int nCurrModel = GetDocument()->m_nModSelItem;

	//if (m_nPrevMod == nCurrModel) return;
	CMdEquation* pEq = GetDocument()->m_pSimul->GetEquation(nCurrModel);
	if (!pEq) return;

	CPP_SimModel *pSim = DYNAMIC_DOWNCAST(CPP_SimModel,pEq);
	if (!pSim) return;

	//BOOL bRet = pSim->CheckValidity(strErr);

	CString strEq;
	m_wndEditEqn.GetWindowText(strEq);

	CStringArray strArr;

	char seps[]   = "\r\n";


	LPTSTR p = strEq.GetBuffer( strEq.GetLength() );

	char *token;
	token = strtok( p, seps );
	
	while( token != NULL )
	{
		CString strItem = token;

		int nb = strItem.Find(_T("="));

		if (nb == -1 && strItem.GetAt(0)!= ';')
		{
			FINDTEXTEX ft;

			ft.chrg.cpMin = 0;
			ft.chrg.cpMax = -1;
			ft.chrgText.cpMin = 0;
			ft.chrgText.cpMax = 0;
			ft.lpstrText = 	strItem.GetBuffer(strItem.GetLength());

			m_wndEditEqn.FindText(FR_MATCHCASE|FR_WHOLEWORD,&ft);
			m_wndEditEqn.SetSel(ft.chrgText);
			CString strNew = _T("; ") + strItem;
			m_wndEditEqn.ReplaceSel(strNew);

			m_wndEditEqn.Invalidate();
			m_wndEditEqn.UpdateWindow();

		}
		else if (!strItem.IsEmpty() && strItem.GetAt(0)!= ';')
		{
			nb = strItem.Find(_T(";"));
			if (nb != -1)
			{
				CString strItem2 = strItem.Left(nb);
				strItem.Delete(0,nb);
				CString strItem3 = strItem;
				strItem2.TrimRight();
				strItem = strItem2;
			}
			strItem.Replace(_T(" "),_T(""));
			strArr.Add(strItem);
		}

		token = strtok( NULL, seps );
	}

	int nStr = strArr.GetSize();
	if (!nStr) 
	{
		UpdateData();
		return;
	}

	CMathParser mp;
	double result;
	CStrMap varmap;

	double varvalues[] = { 5, 6.1,0.1,5,2,3,4,8,.3,.5 };

	mp.Parameters=&varmap;

	for (int i=0;i<nStr;i++)
	{
		CString m_strEqu = strArr.GetAt(i);

		CString sT = m_strEqu;
		CString sT2(_T("/dt"));

		if (sT.Find(sT2))
		{
			m_strEqu.Replace(sT2,_T(""));

		}

		char *ErrMsg = mp.Parse( m_strEqu.GetBuffer(m_strEqu.GetLength()), &result );

		int nbV3 = 0;

		while (ErrMsg)
		{
			CString strT1 = ErrMsg;
			CString strT(_T(" - Unknown function/variable!"));
			if (strT1.Find(strT) != -1)
			{
				strT.MakeReverse();
				strT1 = ErrMsg;
				strT1.MakeReverse();
				int mm = strT1.Replace(strT,_T(""));
				strT1.MakeReverse();
				strT1.Delete(0,2);

				char tt[255];
				strcpy(tt,strT1.GetBuffer(strT1.GetLength()));
				varmap.AddString(tt,varvalues+(nbV3++));
			}
			else 
			{
				MessageBox(ErrMsg);
				FINDTEXTEX ft;

				ft.chrg.cpMin = 0;
				ft.chrg.cpMax = -1;
				ft.chrgText.cpMin = 0;
				ft.chrgText.cpMax = 0;
				ft.lpstrText = 	m_strEqu.GetBuffer(m_strEqu.GetLength());

				m_wndEditEqn.FindText(FR_MATCHCASE,&ft);
				m_wndEditEqn.SetSel(ft.chrgText);
				return;
			}

			ErrMsg = mp.Parse( m_strEqu.GetBuffer(m_strEqu.GetLength()), &result );
		}
	}


	CStringArray	strAVar;
	CStringArray	strAHVar;
	CStringArray	strATVar;
	CStringArray	strAVVar;
	CString strCst(_T(" "));
	int nb = varmap.GetSize();
	for (i=0;i<nb;i++)
	{
		int gg=0;
		double *value;
		char * ttt= varmap.GetString(i,&gg,(void**) &value);
		char ttty[255];
		strncpy(ttty,ttt,gg);
		ttty[gg]='\0';
		CString ff = ttty;
		strCst += ff + _T(" ");
		strAVar.Add(ff);
		//MessageBox(ff);
	}

	strATVar.Copy(strAVar);

	for (i=0;i<nStr;i++)
	{
		CString m_strEqu = strArr.GetAt(i);
		int nb = m_strEqu.Find(_T("="));

		CString strHead = m_strEqu.Left(nb);
		CString strTail = m_strEqu;
		strTail.Delete(0,nb+1);

		CString strTestH = strHead;
		strTestH.MakeLower();
		for (int j=0;j<strAVar.GetSize();j++)
		{
			CString v1 = strAVar.GetAt(j);
			v1.MakeLower();
			int gg = strTestH.Find(v1);
			if (gg !=-1)
			{
				strTestH.TrimLeft();
				strTestH.TrimLeft(v1);
				strTestH.TrimLeft();
			}
		}
	/*	if (!strTestH.IsEmpty())
		{
			MessageBox(_T("#Assignment expected!"));
			FINDTEXTEX ft;

			ft.chrg.cpMin = 0;
			ft.chrg.cpMax = -1;
			ft.chrgText.cpMin = 0;
			ft.chrgText.cpMax = 0;
			ft.lpstrText = 	m_strEqu.GetBuffer(m_strEqu.GetLength());

			m_wndEditEqn.FindText(FR_MATCHCASE|FR_WHOLEWORD,&ft);
			m_wndEditEqn.SetSel(ft.chrgText);
			return;
		}
		else*/
		{
			if (strHead.GetAt(0) == '_')
			{
				strAVVar.Add(strHead);

				CString strInit = strHead;
				strInit.Delete(0,1);
				strInit = strInit + _T("_INIT");
				strATVar.Add(strInit);
			}
			else
				strAHVar.Add(strHead);
			for (int g=0;g<strATVar.GetSize();g++)
			{
				CString tTail = strATVar.GetAt(g);
				CString tHead = strHead;
				if ((tTail == tHead) ||
					(tTail == (_T("_") + strHead)) ||
					((_T("_") + tTail == strHead)))
				{
					strATVar.RemoveAt(g);
					g--;
					//break;
				}
			}
		}
	
	}

	CString mstr;

	mstr += _T("Variables : \n");
	for (i=0;i<strAVVar.GetSize();i++)
	{
		mstr += _T("\t") + strAVVar.GetAt(i) + _T("\n");
	}
	mstr += _T("Parameters : \n");
	for (i=0;i<strATVar.GetSize();i++)
	{
		mstr += _T("\t") + strATVar.GetAt(i) + _T("\n");
	}
	mstr += _T("Aggregates : \n");
	for (i=0;i<strAHVar.GetSize();i++)
	{
		mstr += _T("\t") + strAHVar.GetAt(i) + _T("\n");
	}

	MessageBox(mstr);

	m_wndEditEqn.ClearConstants();
	if (!strCst.IsEmpty())
	{
		m_wndEditEqn.AddConstants(strCst);
		m_wndEditEqn.FormatAll();
		m_wndEditEqn.Invalidate();
		m_wndEditEqn.UpdateWindow();

	}

	CTypedPtrArray<CObArray,CModelVar*>	m_cLVarSet;	// List of Variables
	CTypedPtrArray<CObArray,CModelPar*>	m_cLParSet;	// List of Parameters

//	m_cLVarSet.Copy(pSim->m_cVarSet);
//	m_cLParSet.Copy(pSim->m_cParSet);

	int nvar = pSim->m_cVarSet.GetSize();
	int npar = pSim->m_cParSet.GetSize();
	for(i = 0; i < pSim->m_cVarSet.GetSize();i++ )
	{
		m_cLVarSet.Add(pSim->m_cVarSet.GetAt(i));
		//delete pSim->m_cVarSet.GetAt(i);
	}
	pSim->m_cVarSet.RemoveAll();
	for (i = 0; i < pSim->m_cParSet.GetSize();i++ )
	{
		//delete pSim->m_cParSet.GetAt(i);
		m_cLParSet.Add(pSim->m_cParSet.GetAt(i));
	}
	pSim->m_cParSet.RemoveAll();

	// The variables 
	CModelVar *pVar = NULL;
	CModelPar *pPar=NULL;

	pSim->m_cVarSet.Add(pVar = new CModelTime("Time"));			
	pVar->SetDef(_T("T"));
	pVar->SetAbbrev(_T("T"));
	//pVar->SetInitValue(-1);

	for (i=0;i<strAVVar.GetSize();i++)
	{
		CString mstr = strAVVar.GetAt(i);
		mstr.Delete(0);
		pSim->m_cVarSet.Add(pVar = new CModelVar(mstr));				
		pVar->SetInitValue(i);

		for (int h=0;h<nStr;h++)
		{
			CString sstr = strArr.GetAt(h);
			if (sstr.Find(mstr + _T("=")) != -1)
			{
				pVar->SetDef(sstr);
				break;
			}
		}
		pVar->SetAbbrev(mstr);
		pVar->SetColor(RGB(255,0,0));

		mstr = mstr + _T("_INIT");

		pSim->m_cParSet.Add(pPar= new CModelPar(mstr,0));		// 0
		pPar->SetInfos(0,0,1);
		pPar->SetInitValue(i+1);
		pPar->SetDef(mstr);
		pPar->SetAbbrev(mstr);
		pPar->SetColor(RGB(255,0,0));

		for (int k=0;k<m_cLVarSet.GetSize();k++)
		{
			CModelVar *pOld = m_cLVarSet.GetAt(k);
			if (!pOld) continue;

			CString str1=pVar->GetAbbrev();
			CString str2=pOld->GetAbbrev();

			if (str1==str2)
			{
				int r=0;
				r++;
				pVar->SetName(pOld->GetName());
				pVar->SetUnit(pOld->GetUnit());
				pVar->SetDesc(pOld->GetDesc());
				pVar->SetColor(pOld->GetColor());
			}
		}
		for (k=0;k<m_cLParSet.GetSize();k++)
		{
			CModelPar *pOld = m_cLParSet.GetAt(k);
			if (!pOld) continue;

			CString str1=pPar->GetAbbrev();
			CString str2=pOld->GetAbbrev();

			if (str1==str2)
			{
				int r=0;
				r++;
				pPar->SetName(pOld->GetName());
				pPar->SetUnit(pOld->GetUnit());
				pPar->SetDesc(pOld->GetDesc());
				pPar->SetColor(pOld->GetColor());
				pPar->SetInfos(pOld->GetValue(),pOld->GetMin(),pOld->GetMax());
			}
		}
	}

	for (i=0;i<strATVar.GetSize();i++)
	{
		CString mstr = strATVar.GetAt(i);

		if (mstr.Find(_T("_INIT")) != -1) continue;
		if (mstr==_T("T")) continue;

		pSim->m_cParSet.Add(pPar= new CModelPar(mstr,10));
		pPar->SetInfos(10,1,15);
		pPar->SetDef(mstr);
		pPar->SetAbbrev(mstr);

		for (int k=0;k<m_cLParSet.GetSize();k++)
		{
			CModelPar *pOld = m_cLParSet.GetAt(k);
			if (!pOld) continue;

			CString str1=pPar->GetAbbrev();
			CString str2=pOld->GetAbbrev();

			if (str1==str2)
			{
				int r=0;
				r++;
				pPar->SetName(pOld->GetName());
				pPar->SetUnit(pOld->GetUnit());
				pPar->SetDesc(pOld->GetDesc());
				pPar->SetColor(pOld->GetColor());
				pPar->SetInfos(pOld->GetValue(),pOld->GetMin(),pOld->GetMax());
			}
		}
	}

	for (i=0;i<strAHVar.GetSize();i++)
	{
		CString mstr = strAHVar.GetAt(i);

		pSim->m_cVarSet.Add(pVar = new CModelVar(mstr));				
		for (int h=0;h<nStr;h++)
		{
			CString sstr = strArr.GetAt(h);
			if (sstr.Find(mstr + _T("=")) != -1)
			{
				pVar->SetDef(sstr);
				break;
			}
		}
		//pVar->SetDef(mstr);
		pVar->SetAbbrev(mstr);
		//pVar->SetColor(RGB(0,0,255));
		for (int k=0;k<m_cLVarSet.GetSize();k++)
		{
			CModelVar *pOld = m_cLVarSet.GetAt(k);
			if (!pOld) continue;

			CString str1=pVar->GetAbbrev();
			CString str2=pOld->GetAbbrev();

			if (str1==str2)
			{
				int r=0;
				r++;
				pVar->SetName(pOld->GetName());
				pVar->SetUnit(pOld->GetUnit());
				pVar->SetDesc(pOld->GetDesc());
				pVar->SetColor(pOld->GetColor());
			}
		}

	}

	pSim->SetDependency();

	int nb2 = GetDocument()->m_pInstruct->m_cLUSet.GetSize();
	for (int k=0;k<nb2;k++)
	{
		CLearningUnit *pLU= GetDocument()->m_pInstruct->m_cLUSet.GetAt(k);
		if (!pLU) continue;

		if (pLU->m_nSimulID==nCurrModel)
		{
			int r;
			r++;

			CTypedPtrArray<CObArray,CMdExpertSet*>		m_cLExpSet;			// List of Experimental Set

			int nExpSet = pLU->m_cExpSet.GetSize();
			for (int j=0;j<nExpSet;j++)
			{
				m_cLExpSet.Add(pLU->m_cExpSet.GetAt(j));
			}
			pLU->m_cExpSet.RemoveAll();
			for (j=0;j<nExpSet;j++)
			{
				pLU->AddDefaultExpSet(nCurrModel);

				CMdExpertSet *pExpSet = m_cLExpSet.GetAt(j);
				if (!pExpSet) continue;
				CMdExpertSet *pExpSet2 = pLU->m_cExpSet.GetAt(j);
				if (!pExpSet2) continue;

				pExpSet2->SetName(pExpSet->GetName());
			}
			for(i = 0; i < m_cLExpSet.GetSize();i++ )
				delete m_cLExpSet.GetAt(i);
			m_cLExpSet.RemoveAll();
		}
	}


	for(i = 0; i < m_cLVarSet.GetSize();i++ )
		delete m_cLVarSet.GetAt(i);
	m_cLVarSet.RemoveAll();
	for (i = 0; i < m_cLParSet.GetSize();i++ )
		delete m_cLParSet.GetAt(i);
	m_cLParSet.RemoveAll();


	GetDocument()->UpdateAllViews(this,DOC_UPDATE_MODEL);
	UpdateData();
}


void CViewSDIbuild::OnUpdateEdit(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(TRUE);
	
}


void CViewSDIbuild::OnEditCopy() 
{
	// TODO: Add your command handler code here
	m_wndEditEqn.Copy();
}

void CViewSDIbuild::OnEditCut() 
{
	// TODO: Add your command handler code here
	m_wndEditEqn.Cut();
}

void CViewSDIbuild::OnEditPaste() 
{
	// TODO: Add your command handler code here
	BOOL bPaste = m_wndEditEqn.CanPaste();
	if (bPaste)
		m_wndEditEqn.Paste();

}

void CViewSDIbuild::OnEditUndo() 
{
	// TODO: Add your command handler code here
	BOOL bPaste = m_wndEditEqn.CanUndo();
	if (bPaste) 
		m_wndEditEqn.Undo();
}

void CViewSDIbuild::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	// TODO: Add your message handler code here
	
}

void CViewSDIbuild::OnEditSelectAll() 
{
	// TODO: Add your command handler code here
	m_wndEditEqn.SetSel(0,-1);
}
