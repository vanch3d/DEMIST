// SimulParam.cpp : implementation file
//

#include "stdafx.h"
#include "Simul.h"

#include "LearnerTrace.h"

#include "simulDoc.h"
#include "SimulParam.h"

#include <MInstruction\LearningUnit.h>
#include <MSimulation\Model.h>

#include "Float.H"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CViewParam

IMPLEMENT_DYNCREATE(CViewParam, CFormView)

CViewParam::CViewParam()
	: CFormView(CViewParam::IDD)
{
	//{{AFX_DATA_INIT(CViewParam)
	//}}AFX_DATA_INIT
	m_ImageList.Create(IDB_VAR_ICONS,14,0,RGB(128,128,128));
}

CViewParam::~CViewParam()
{
	CTrace::T_CLOSE(GetParentFrame());
}

/*BOOL MyAfxSimpleFloatParse(LPCTSTR lpszText, double& d)
{
	ASSERT(lpszText != NULL);
	while (*lpszText == ' ' || *lpszText == '\t')
		lpszText++;

	TCHAR chFirst = lpszText[0];
	d = _tcstod(lpszText, (LPTSTR*)&lpszText);
	if (d == 0.0 && chFirst != '0')
		return FALSE;   // could not convert
	while (*lpszText == ' ' || *lpszText == '\t')
		lpszText++;

	if (*lpszText != '\0')
		return FALSE;   // not terminated properly

	return TRUE;
}*/


void CViewParam::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CViewParam)
	DDX_Control(pDX, IDC_STATIC_PARAM, m_strParam);
	DDX_Control(pDX, IDC_STATIC_INIT, m_strInit);
	//}}AFX_DATA_MAP
//	DDX_Text(pDX, IDC_EDIT1, m_ddsdsdsd);
//	DDV_MinMaxDouble(pDX, m_ddsdsdsd, -10., 40.);
	DDX_GridControl(pDX, IDC_EXPSET_PARAM, m_Parlist);
	DDX_GridControl(pDX, IDC_EXPSET_INIT, m_InitList);

/*	if (pDX->m_bSaveAndValidate)
	{
		CLearningUnit*	pLU = GetDocument()->GetCurrentLU();
		if (!pLU) return;
		CMdEquation*	m_pEqu = GetDocument()->GetCurrentModel();
		if (!m_pEqu) return;

		int nbC = m_Parlist.GetColumnCount();
		int nbR = m_Parlist.GetRowCount();
		for (int i=1;i<nbC;i++)
			for (int j=1;j<nbR;j++)
			{
				LPARAM nItem = m_Parlist.GetItemData(j,i);
				UINT   nState = m_Parlist.GetItemState(j,i);
				BOOL bMod = nState & GVIS_MODIFIED;
				//bMod = nState | GVIS_MODIFIED;
				CModelPar *pPar= m_pEqu->m_cParSet.GetAt((int)nItem);
				if (pPar)
				{
					CString mstr = m_Parlist.GetItemText(j,i);
					double val = atol(mstr.GetBuffer(mstr.GetLength()));
					BOOL mb ;//= MyAfxSimpleFloatParse(mstr,val);
					if (!mb)
					{
						//nState |= GVIS_FOCUSED; // | GVIS_SELECTED;
						//m_Parlist.SetItemState(j,i,nState);
						//m_Parlist.SetFocusCell(j,i);
						m_Parlist.SetFocusCell(j,i);
						AfxMessageBox(AFX_IDP_PARSE_REAL);
						pDX->Fail();            // throws exception
					}
					double min = pPar->GetMin();
					double max = pPar->GetMax();
					//DDV_MinMaxDouble(pDX, val, min, max);

					if (val < min || val > max)
					{
						//_AfxFailMinMaxReal(pDX, (double)minVal, (double)maxVal, DBL_DIG,
						//AFX_IDP_PARSE_REAL_RANGE);
						m_Parlist.SetFocusCell(j,i);
						int precision = DBL_DIG;
						UINT nIDPrompt = AFX_IDP_PARSE_REAL_RANGE;
						TCHAR szMin[32], szMax[32];
						CString prompt;

						_stprintf(szMin, _T("%.*g"), precision, min);
						_stprintf(szMax, _T("%.*g"), precision, max);
						AfxFormatString2(prompt, nIDPrompt, szMin, szMax);

						AfxMessageBox(prompt, MB_ICONEXCLAMATION, nIDPrompt);
						prompt.Empty(); // exception prep
						pDX->Fail();	
					}

					CMdExpertSet*	pExpSet = pLU->m_cExpSet.GetAt(i-1);
					double ddd = pExpSet->GetValue(nItem);
					pExpSet->SetValue(nItem,val);

				}

			}
		m_Parlist.SetFocusCell(-1,-1);

		nbC = m_InitList.GetColumnCount();
		nbR = m_InitList.GetRowCount();
		for (i=1;i<nbC;i++)
			for (int j=1;j<nbR;j++)
			{
				LPARAM nItem = m_InitList.GetItemData(j,i);
				UINT   nState = m_InitList.GetItemState(j,i);
				BOOL bMod = nState & GVIS_MODIFIED;
				//bMod = nState | GVIS_MODIFIED;
				CModelPar *pPar= m_pEqu->m_cParSet.GetAt((int)nItem);
				if (pPar)
				{
					CString mstr = m_InitList.GetItemText(j,i);
					double val = atol(mstr.GetBuffer(mstr.GetLength()));
					BOOL mb ;//= MyAfxSimpleFloatParse(mstr,val);
					if (!mb)
					{
						//nState |= GVIS_FOCUSED ; // | GVIS_SELECTED;
						//m_Parlist.SetItemState(j,i,nState);
						//m_Parlist.SetFocusCell(j,i);
						m_InitList.SetFocusCell(j,i);
						AfxMessageBox(AFX_IDP_PARSE_REAL);
						pDX->Fail();            // throws exception
					}
					double min = pPar->GetMin();
					double max = pPar->GetMax();
					if (val < min || val > max)
					{
						//_AfxFailMinMaxReal(pDX, (double)minVal, (double)maxVal, DBL_DIG,
						//AFX_IDP_PARSE_REAL_RANGE);
						m_InitList.SetFocusCell(j,i);
						int precision = DBL_DIG;
						UINT nIDPrompt = AFX_IDP_PARSE_REAL_RANGE;
						TCHAR szMin[32], szMax[32];
						CString prompt;

						_stprintf(szMin, _T("%.*g"), precision, min);
						_stprintf(szMax, _T("%.*g"), precision, max);
						AfxFormatString2(prompt, nIDPrompt, szMin, szMax);

						AfxMessageBox(prompt, MB_ICONEXCLAMATION, nIDPrompt);
						prompt.Empty(); // exception prep
						pDX->Fail();	
					}

					CMdExpertSet*	pExpSet = pLU->m_cExpSet.GetAt(i-1);
					double ddd = pExpSet->GetValue(nItem);
					pExpSet->SetValue(nItem,val);
				}

			}
		m_InitList.SetFocusCell(-1,-1);
	}*/
}


BEGIN_MESSAGE_MAP(CViewParam, CFormView)
	//{{AFX_MSG_MAP(CViewParam)
	ON_WM_GETMINMAXINFO()
	ON_WM_CREATE()
	ON_COMMAND(ID_PARAMETERS_RESETVALUES, OnResetParameters)
	ON_COMMAND(ID_PARAMETERS_UPDATECHANGES, OnApplyParameters)
	ON_COMMAND(ID_PARAMETERS_ADDEXPSET, OnAddExpSet)
	ON_COMMAND(ID_PARAMETERS_REMOVEEXPSET, OnRemoveExpSet)
	ON_UPDATE_COMMAND_UI(ID_PARAMETERS_RESETVALUES, OnUpdateParametersCmd)
	ON_UPDATE_COMMAND_UI(ID_PARAMETERS_UPDATECHANGES, OnUpdateParametersCmd)
	ON_UPDATE_COMMAND_UI(ID_PARAMETERS_ADDEXPSET, OnUpdateParametersCmd)
	ON_UPDATE_COMMAND_UI(ID_PARAMETERS_REMOVEEXPSET, OnUpdateParametersCmd)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
	ON_MESSAGE(TRACE_VIEW_ACTIVATE, OnActivateER)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewParam diagnostics

#ifdef _DEBUG
void CViewParam::AssertValid() const
{
	CFormView::AssertValid();
}

void CViewParam::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CSimulDoc* CViewParam::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSimulDoc)));
	return (CSimulDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CViewParam printing

BOOL CViewParam::OnPreparePrinting(CPrintInfo* pInfo) 
{
	// TODO: call DoPreparePrinting to invoke the Print dialog box
	return DoPreparePrinting(pInfo);
	//return CFormView::OnPreparePrinting(pInfo);
}

void CViewParam::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CFormView::OnEndPrinting(pDC, pInfo);
}

void CViewParam::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CFormView::OnBeginPrinting(pDC, pInfo);
}

void CViewParam::OnPrint(CDC* pDC, CPrintInfo* pInfo) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CFormView::OnPrint(pDC, pInfo);
}

/////////////////////////////////////////////////////////////////////////////
// CViewParam message handlers

/*void CViewParam::OnUpdateParams() 
{
	// TODO: Add your control notification handler code here
	BOOL bUpdate = false;
	CSimulDoc* pDoc = GetDocument();

	if (!UpdateData())
		return;

	m_bSingleView = !m_bSingleView;
	OnUpdateForm();
	OnUpdate(0,0,0);
	Invalidate();
	UpdateWindow();

	if (bUpdate)
	{	// if the document stored data then we would call SetModifiedFlag here
		pDoc->UpdateDoc(this);
	}
}

void CViewParam::OnDefaultParams() 
{
	// TODO: Add your control notification handler code here
	int nb = m_Parlist.GetRowHeight(0);
	nb = m_Parlist.GetColumnWidth(0);
	m_pParBtn[m_nbParBtn] = new CEdit();

	DWORD style2 = m_cdfdfd.GetStyle();
//	CRect mRect,mrect2;
//	m_cdfdfd.GetRect(&mRect);
//	m_cdfdfd.GetWindowRect(&mrect2);

	int aa = m_nbParBtn % 5;
	int bb = m_nbParBtn / 5;
	if (m_pParBtn[m_nbParBtn])
	{
		CFont *pFont = GetFont();
		DWORD style = WS_CHILD | WS_VISIBLE | WS_TABSTOP | 128;
		CSize msize(50,20);

		CPoint pt(10+aa*msize.cx,10+bb*msize.cy);
		CPoint pt2 = pt;
		CRect mrect(pt,pt2);
		mrect.InflateRect(0,0,msize.cx,msize.cy);
//		m_pParBtn[m_nbParBtn]->Create(style,CRect(10,10,10+mRect.Width(),
//					10+mRect.Height()),this,100+m_nbParBtn);
		m_pParBtn[m_nbParBtn]->CreateEx(WS_EX_CLIENTEDGE,"EDIT","10.0",style,
					mrect,this,100+m_nbParBtn);
		m_pParBtn[m_nbParBtn]->SetFont(pFont);
		//m_pParBtn[m_nbParBtn]->ModifyStyle(WS_BORDER|WS_CAPTION, style & (WS_BORDER|WS_CAPTION));
		m_pParBtn[m_nbParBtn]->ModifyStyleEx(0, WS_EX_CLIENTEDGE);

		int fgfg = m_pParBtn[m_nbParBtn]->GetStyle();
		m_nbParBtn++;
	}
	Invalidate();
	UpdateWindow();
}*/

LRESULT CViewParam::OnActivateER(WPARAM wp, LPARAM lp)
{
	static BOOL bFirst = TRUE;

	if (!bFirst) CTrace::T_SWITCH(this, (CWnd*)wp);
	bFirst = FALSE;
	return 0;
}


void CViewParam::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	CTrace::T_OPEN(GetParentFrame());

	// TODO: Add your specialized code here and/or call the base class
	m_Parlist.SetTextBkColor(RGB(0xFF, 0xFF, 0xE0));
	m_InitList.SetTextBkColor(RGB(0xFF, 0xFF, 0xE0));
	m_Parlist.EnableSelection(FALSE);
	m_InitList.EnableSelection(FALSE);
	m_Parlist.SetRowResize(FALSE);
	m_InitList.SetRowResize(FALSE);
	m_Parlist.SetColumnResize(FALSE);
	m_InitList.SetColumnResize(FALSE);
	//m_Parlist.SetEditable(false);
	//m_Parlist.SetListMode(true);
	//m_Parlist.EnableDragAndDrop(FALSE);
	//m_Parlist.SetSingleRowSelection(true);

	m_Parlist.SetImageList(&m_ImageList);
	m_InitList.SetImageList(&m_ImageList);
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
void CViewParam::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (LOWORD(lHint) == DOC_UPDATE_TIMERDATA)
	{
		return;
	}
	if (LOWORD(lHint) == DOC_UPDATE_ER)
	{
		UINT nState = m_Parlist.GetItemState(1,1);
		m_Parlist.SetItemState(1,1,nState | GVIS_FIXED);
		return;
	}
	/*if (LOWORD(lHint) == DOC_UPDATE_TIMERDATA)
	{
		return;
	}*/

/*	CSimulDoc* pDoc = GetDocument();

	m_preyFert = pDoc->PreyFertility;
	m_CarryCap = pDoc->CarryingCap;
	m_Cover = pDoc->Cover;
	m_PredDeath = pDoc->PredDeathRate;
	m_PreyInit = pDoc->Prey;
	m_PredInit = pDoc->Predators;

	m_SpinPred.SetPos((int)m_PredInit);
	m_SpinPrey.SetPos((int)m_PreyInit);
	m_SpinCCap.SetPos((int)m_CarryCap);
	m_SpinCover.SetPos((int)m_Cover);
	m_SpinCover.SetRange(0,(int)m_PreyInit);*/

		
	CMdEquation*	m_pEqu = GetDocument()->GetCurrentModel();
	if (!m_pEqu) return;
	CLearningUnit*	pLU = GetDocument()->GetCurrentLU();
	if (!pLU) return;


	int nbExpSet =  pLU->m_cExpSet.GetSize();
	int colN,colD,colF;

	colN = nbExpSet;
	colD = 0;
	colF = nbExpSet;

	m_Parlist.SetColumnCount(1+colN);
	m_Parlist.SetFixedColumnCount(1);
	m_InitList.SetColumnCount(1+colN);
	m_InitList.SetFixedColumnCount(1);

	CMdExpertSet*	m_pExpSet = pLU->m_cExpSet.GetAt(0);
	int nbparam = m_pExpSet->GetSize();
	int nbinit = 0;
	int nb2 = m_pEqu->m_cParSet.GetSize();

	for (int row = 0; row < nb2; row++)
	{
		CModelPar *pPar= m_pEqu->m_cParSet.GetAt(row);
		if (pPar->m_InitValue != -1)
		{
			nbparam--;
			nbinit++;
		}
	}

	m_Parlist.SetRowCount(nbparam+1);
	m_InitList.SetRowCount(nbinit+1);
	m_Parlist.SetFixedRowCount(1);
	m_InitList.SetFixedRowCount(1);

	for (int col = colD;col < colF; col++)
	{
		CMdExpertSet*	pExpSet = pLU->m_cExpSet.GetAt(col);
		DWORD dwTextStyle = DT_LEFT|DT_VCENTER|DT_SINGLELINE;
		int row1=1,row2=1;
		for (row = 0; row < nb2; row++)
		{
			CModelPar *pPar= m_pEqu->m_cParSet.GetAt(row);
			CString name = pPar->GetFormatName(pLU->m_bShowAbbrev);
			double	val = pExpSet->GetValue(row);

			GV_ITEM Item,Item2,Item3;
			Item.mask = GVIF_TEXT|GVIF_FORMAT|GVIF_PARAM;
			Item.nFormat = dwTextStyle;
			Item.lParam = row;
			Item2 = Item;
			Item3 = Item;

			Item.col = 0;
			Item.strText = name;
			Item.mask |= GVIF_IMAGE;


			Item2.col = col-colD+1;
			name.Format(_T("%.4g"),val);
			Item2.strText = name;

			Item3.col = col-colD+1;
			Item3.row = 0;
			Item3.strText = pExpSet->GetName();

			if (pPar->m_InitValue != -1)
			{
				Item.row = row2;
				Item.iImage = 2;
				Item2.row = row2;
				m_InitList.SetItem(&Item);
				m_InitList.SetItem(&Item2);
				m_InitList.SetItem(&Item3);
				//m_InitList.SetItemImage(row2,col,0);
				row2++;
			}
			else
			{	
				Item.row = row1;
				Item.iImage = 1;
				Item2.row = row1;
				m_Parlist.SetItem(&Item);
				m_Parlist.SetItem(&Item2);
				m_Parlist.SetItem(&Item3);
				//m_Parlist.SetItemImage(row1,col,0);
				row1++;
			}
		}

	}

	if (LOWORD(lHint) != DOC_UPDATE_ER)
	{
		m_InitList.AutoSizeColumn(0);
		m_Parlist.AutoSizeColumn(0);
		int s1 = m_InitList.GetColumnWidth(0);
		int s2 = m_Parlist.GetColumnWidth(0);
		int s3 = max(s1,s2)+15;
		m_InitList.SetColumnWidth(0,s3);
		m_Parlist.SetColumnWidth(0,s3);

	//	m_Parlist.ExpandColumnsToFit();
	//	m_InitList.ExpandColumnsToFit();
		for (int i=0;i<m_Parlist.GetRowCount();i++)
			m_Parlist.SetRowHeight(i,17);
		for (i=0;i<m_InitList.GetRowCount();i++)
			m_InitList.SetRowHeight(i,17);

		m_InitList.Invalidate();
		m_Parlist.Invalidate();

		m_InitList.AutoSizeColumn(0);
		m_Parlist.AutoSizeColumn(0);
		s1 = m_InitList.GetColumnWidth(0);
		s2 = m_Parlist.GetColumnWidth(0);
		s3 = max(s1,s2)+15;
		m_InitList.SetColumnWidth(0,s3);
		m_Parlist.SetColumnWidth(0,s3);
	
		int ghgh = 4;
		for (int hh=0;hh<m_Parlist.GetColumnCount();hh++)
		{
			if (hh > 6) break;
			ghgh+= m_Parlist.GetColumnWidth(hh);
		}

		CRect mr1;
		m_Parlist.GetWindowRect(mr1);
		if (mr1.Width() != ghgh)
		{
			mr1.right = mr1.left+ ghgh;
			m_Parlist.SetWindowPos(0,0,0,mr1.Width(),mr1.Height(),SWP_NOOWNERZORDER | SWP_NOMOVE );
			m_InitList.SetWindowPos(0,0,0,mr1.Width(),mr1.Height(),SWP_NOOWNERZORDER | SWP_NOMOVE );

			
			CRect rect,rect2;
		CWnd *par = GetParent( );
		if (!par) return;		par->GetWindowRect(&rect);
		par->GetParent()->GetWindowRect(&rect2);
	//	par->MoveWindow(rect.left-rect2.left-2,rect.top-rect2.top-2,mr1.Width(),89);
		GetWindowRect(&rect2);
		SetWindowPos(0,0,0,mr1.Width()+1,rect2.Height()+30,SWP_NOOWNERZORDER | SWP_NOMOVE );
		GetParentFrame()->RecalcLayout();
		GetWindowRect(&rect2);
		par->GetWindowRect(&rect);
		par->GetParent()->GetWindowRect(&rect2);
		par->MoveWindow(rect.left-rect2.left-2,rect.top-rect2.top-2,mr1.Width()+8,390);
		m_strInit.GetWindowRect(&rect);
		rect.right = mr1.right;
		m_strInit.SetWindowPos(0,0,0,mr1.Width(),rect.Height(),SWP_NOOWNERZORDER | SWP_NOMOVE );
		m_strParam.SetWindowPos(0,0,0,mr1.Width(),rect.Height(),SWP_NOOWNERZORDER | SWP_NOMOVE );
		UpdateData(FALSE); 
		m_InitList.GetWindowRect(rect);
		GetWindowRect(rect2);
		par->SetWindowPos(0,0,0,rect.Width()+10,rect.bottom-rect2.top+52,SWP_NOOWNERZORDER | SWP_NOMOVE );
		}

/*		
	

*/
	}

}

void CViewParam::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	CRect r1,r2;
	GetWindowRect(&r1);
	GetClientRect(&r2);

//	if (IsIconic() || IsZoomed())
//		CFormView::OnGetMinMaxInfo(lpMMI );

	CRect mr1;
	CRect mr2;
	m_Parlist.GetWindowRect(mr1);
	lpMMI->ptMinTrackSize.x=mr1.Width()+24;
	m_InitList.GetWindowRect(mr1);
	GetWindowRect(mr2);
	lpMMI->ptMinTrackSize.y=mr1.bottom-mr2.top+52;
	lpMMI->ptMaxTrackSize.x=lpMMI->ptMinTrackSize.x;//mr1.Width()+12;
	lpMMI->ptMaxTrackSize.y=lpMMI->ptMinTrackSize.y;//mr1.Height();

}

int CViewParam::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CFormView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	if (!m_cToolBar.CreateEx(GetParent(), TBSTYLE_FLAT | TBSTYLE_TOOLTIPS, 
			WS_CHILD | WS_VISIBLE | CBRS_TOP | 
			/*CBRS_GRIPPER | */CBRS_TOOLTIPS | CBRS_FLYBY | CCS_ADJUSTABLE,
			CRect(0, 0, 0, 0), 120) ||
		!m_cToolBar.LoadToolBar(IDR_SIMULPARAM_TMPL))
	{
		TRACE0("Failed to create toolbar1\n");
		return -1;      // fail to create  
	}
	
	return 0;
}

void CViewParam::OnUpdateParametersCmd(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	BOOL bEn = FALSE;
	switch(pCmdUI->m_nID){
	case ID_PARAMETERS_RESETVALUES:
	case ID_PARAMETERS_UPDATECHANGES:
		bEn = TRUE;
		break;
	default:
		break;
	}
	pCmdUI->Enable(bEn);
	
}

void CViewParam::OnResetParameters() 
{
	// TODO: Add your command handler code here
	
}

void CViewParam::OnApplyParameters() 
{
	// TODO: Add your command handler code here
	if (!UpdateData()) return;

	CLearningUnit*	pLU = GetDocument()->GetCurrentLU();
	if (!pLU) return;
	CMdEquation*	pEqu = GetDocument()->GetCurrentModel();
	if (!pEqu) return;
	pEqu->OnUpdate(pLU,TRUE);

	//GetDocument()->UpdateAllViews(NULL,
	GetDocument()->UpdateDoc(this);

}

void CViewParam::OnAddExpSet() 
{
	// TODO: Add your command handler code here
	
}

void CViewParam::OnRemoveExpSet() 
{
	// TODO: Add your command handler code here
	
}

void CViewParam::OnDestroy() 
{
	CTrace::T_CLOSE(GetParentFrame());
	CFormView::OnDestroy();
}
