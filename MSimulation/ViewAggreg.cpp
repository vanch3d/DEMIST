// ViewAggreg.cpp : implementation file
//

#include "stdafx.h"
#include "simul.h"

#include "SimulDoc.h"
#include "ViewAggreg.h"

#include <MSimulation\PreyPredModel.h>
#include "GridCtrl\NewCellTypes\GridCellCombo.h"
#include "GridCtrl\NewCellTypes\GridCellColor.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define IDC_GRIDCONTROL	150
/////////////////////////////////////////////////////////////////////////////
// CViewSDIaggreg

IMPLEMENT_DYNCREATE(CViewSDIaggreg, CView)

CViewSDIaggreg::CViewSDIaggreg()
{
	m_pGridCtrl = NULL;
	m_pImageList.Create(IDB_VAR_ICONS,14,0,RGB(128,128,128));
}

CViewSDIaggreg::~CViewSDIaggreg()
{
	if (m_pGridCtrl) delete m_pGridCtrl;
}


BEGIN_MESSAGE_MAP(CViewSDIaggreg, CView)
	//{{AFX_MSG_MAP(CViewSDIaggreg)
	ON_WM_CREATE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
    ON_NOTIFY(GVN_ENDLABELEDIT, IDC_GRIDCONTROL, OnEndGridEdit)
    ON_NOTIFY(NM_DBLCLK , IDC_GRIDCONTROL, OnGridClick)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewSDIaggreg drawing

void CViewSDIaggreg::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CViewSDIaggreg printing

BOOL CViewSDIaggreg::OnPreparePrinting(CPrintInfo* pInfo) 
{
	// TODO: call DoPreparePrinting to invoke the Print dialog box
	
	return DoPreparePrinting(pInfo);
	//CView::OnPreparePrinting(pInfo);
}

void CViewSDIaggreg::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (m_pGridCtrl->GetSafeHwnd())
		m_pGridCtrl->OnEndPrinting(pDC, pInfo);
	//CView::OnEndPrinting(pDC, pInfo);
}

void CViewSDIaggreg::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	if (m_pGridCtrl->GetSafeHwnd())
		m_pGridCtrl->OnBeginPrinting(pDC, pInfo);
	//CView::OnBeginPrinting(pDC, pInfo);
}

void CViewSDIaggreg::OnPrint(CDC* pDC, CPrintInfo* pInfo) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (m_pGridCtrl->GetSafeHwnd())
		m_pGridCtrl->OnPrint(pDC, pInfo);
	//CView::OnPrint(pDC, pInfo);
}

/////////////////////////////////////////////////////////////////////////////
// CViewSDIaggreg diagnostics

#ifdef _DEBUG
void CViewSDIaggreg::AssertValid() const
{
	CView::AssertValid();
}

void CViewSDIaggreg::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CSimulDoc* CViewSDIaggreg::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSimulDoc)));
	return (CSimulDoc*)m_pDocument;
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CViewSDIaggreg message handlers

int CViewSDIaggreg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	
	return 0;
}

void CViewSDIaggreg::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
    if (m_pGridCtrl->GetSafeHwnd())     // Have the grid object and window been created yet?
	{
		CRect rect;
		GetClientRect(rect);        // Get the size of the view's client area
		m_pGridCtrl->MoveWindow(rect);  // Resize the grid to take up that space.
	}	
}

const char *strDefaultTitle[]={
				"Type",
				"Name",
				"Abbrev.",
				"Unit",
				"Def. Color",
				"Initial Value",
				"Minimum Value",
				"Maximum Value",
				"Short Description",
		};

#define NBCOLUMNS	9 
enum {GR_TYPE,GR_NAME,GR_ABBREV,GR_UNIT,GR_COLOR,GR_INIT,GR_MIN,GR_MAX,GR_DESC};

void CViewSDIaggreg::OnInitialUpdate() 
{
	CView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
    if (m_pGridCtrl == NULL)
	{
		m_pGridCtrl = new CGridCtrl;        // Create the Gridctrl object
		if (!m_pGridCtrl) return;
		
		CRect rect;                         // Create the Gridctrl window
		GetClientRect(rect);
		m_pGridCtrl->Create(rect, this, IDC_GRIDCONTROL);
		//DWORD dwStyle =  WS_CHILD | WS_BORDER | WS_TABSTOP | WS_VISIBLE;
		//m_pGridCtrl->CreateEx(0,GRIDCTRL_CLASSNAME,NULL,dwStyle,rect, this, 100);
	}
	m_pGridCtrl->EnableWysiwygPrinting(TRUE);
	
	m_pGridCtrl->SetRowCount(1);        // fill it up with stuff
	m_pGridCtrl->SetColumnCount(NBCOLUMNS);
	m_pGridCtrl->SetFixedColumnCount(1);
	m_pGridCtrl->SetFixedRowCount(1);

	m_pGridCtrl->SetRowResize(false);
	m_pGridCtrl->SetColumnResize(true);	
	m_pGridCtrl->SetListMode();
	m_pGridCtrl->SetSingleRowSelection();
	m_pGridCtrl->SetHeaderSort(TRUE);
	m_pGridCtrl->SetEditable();
	m_pGridCtrl->EnableSelection(FALSE);

	m_pGridCtrl->SetImageList(&m_pImageList);

	for (int i=0;i<NBCOLUMNS;i++)
	{
		GV_ITEM Item;
		Item.mask = GVIF_TEXT | GVIF_FORMAT;
		
		Item.row = 0;
		Item.col = i;
		Item.nFormat = DT_CENTER |DT_WORDBREAK; //DT_CENTER|DT_VCENTER
			//Item.szText.Format(_T("%s"),CS->m_dataName);
			//Item.strText.Format(_T("%s"),CS->m_dataName);
		Item.strText.Format(_T("%s"),strDefaultTitle[i]);
		m_pGridCtrl->SetItem(&Item);
		//m_pGridCtrl->RedrawCell(0,i);
	}
	int nRowH = m_pGridCtrl->GetRowHeight(0);
	m_pGridCtrl->SetRowHeight(0,2*nRowH);


	m_BtnDataBase.SetGrid(m_pGridCtrl);

	// retain old cell properties
//	CGridBtnCell GridCellCopy;
//	GridCellCopy.SetBtnDataBase( &m_BtnDataBase);
//	CGridCellBase* pCurrCell = m_pGridCtrl->GetCell( 0, GR_COLOR);
//	if (pCurrCell)
//		GridCellCopy = *pCurrCell;  // data will be stored in m_BtnDataBase

	OnUpdate(NULL,DOC_UPDATE_MODEL,0L);

}

void CViewSDIaggreg::OnCellsUpdate(int nModelID)
{
	//int nCurrModel = GetDocument()->m_nModSelItem;
	CMdEquation* pEq = GetDocument()->m_pSimul->GetEquation(nModelID);

	int nbItem = 0;
	int nbPar = 0;
	int nbVar = 0;
	if (pEq)
	{
		nbPar = pEq->m_cParSet.GetSize();
		nbVar = pEq->m_cVarSet.GetSize();
		nbItem = nbVar+nbPar;
	}

	m_pGridCtrl->SetRowCount(nbItem+1);        // fill it up with stuff

	for (int i=0;i<nbItem;i++)
	{
		CModelObj *obj;
		if (i < nbPar)
			obj = pEq->m_cParSet.GetAt(i);
		else
			obj = pEq->m_cVarSet.GetAt(i-nbPar);

		int nbImage = obj->GetIcon();
/*		int nbImage;
		if (i >= nbPar)
		{
			if ( ((CModelVar *)obj)->GetInitValue() == -1)
				nbImage = 3;	// A
			else
				nbImage = 0;	// V
		}
		else 
		{	if ( ((CModelPar *)obj)->m_InitValue == -1)
				nbImage = 1;	// P
			else nbImage = 2;	// I
		}*/

		for (int j=0;j<NBCOLUMNS;j++)
		{
			GV_ITEM Item;

			Item.mask = GVIF_TEXT|GVIF_FORMAT|GVIF_PARAM;
			Item.lParam = (long)obj;
			Item.row = i+1;
			Item.col = j;
			//Item.szText.Format(_T("%s"),CS->m_dataName);
			//Item.strText.Format(_T("%s"),CS->m_dataName);

			switch (j){
			case GR_TYPE:
				{
				CString mchr;
				if (nbImage ==0) mchr = "Var.";
				if (nbImage ==1) mchr = "Par.";
				if (nbImage ==2) mchr = "Ini.";
				if (nbImage ==3) mchr = "Agg.";
				if (nbImage ==4) mchr = "Time";
				Item.mask |= GVIF_IMAGE|GVIF_STATE;
				Item.strText.Format(_T("%s"),mchr);
				Item.iImage = nbImage;
				Item.nState = m_pGridCtrl->GetItemState(i+1,j) | GVIS_READONLY;
				}
				break;
			case GR_NAME:
				Item.nFormat = DT_LEFT |DT_WORDBREAK; //DT_CENTER|DT_VCENTER
				Item.strText = obj->GetName();
				break;
			case GR_ABBREV:
				Item.nFormat = DT_LEFT |DT_WORDBREAK; //DT_CENTER|DT_VCENTER
				Item.strText = obj->GetAbbrev();
				break;
			case GR_UNIT:
				Item.nFormat = DT_LEFT |DT_WORDBREAK; //DT_CENTER|DT_VCENTER
				Item.strText = obj->GetUnit();
				break;
			case GR_INIT:
				{
				Item.mask |= GVIF_STATE|GVIF_BKCLR;
				Item.nFormat = DT_RIGHT |DT_WORDBREAK; //DT_CENTER|DT_VCENTER
				int nformat = m_pGridCtrl->GetDefaultCell(FALSE,FALSE)->GetState();


				if (i<nbPar)
				{
					double nVal = ((CModelPar *)obj)->GetValue();
					Item.nState = nformat & ~GVIS_READONLY;
					Item.strText.Format(_T("%.5g"),nVal);
					Item.crBkClr = ::GetSysColor(COLOR_WINDOW);
				}
				else
				{
					Item.nState = nformat | GVIS_READONLY;
					Item.strText = _T("");
					Item.crBkClr = ::GetSysColor(COLOR_3DSHADOW);
				}}
				break;
			case GR_MIN:
				{
				Item.mask |= GVIF_STATE|GVIF_BKCLR;
				Item.nFormat = DT_RIGHT |DT_WORDBREAK; //DT_CENTER|DT_VCENTER
				int nformat = m_pGridCtrl->GetDefaultCell(FALSE,FALSE)->GetState();


				if (i<nbPar)
				{
					double nVal = ((CModelPar *)obj)->GetMin();
					Item.nState = nformat & ~GVIS_READONLY;
					Item.strText.Format(_T("%.5g"),nVal);
					Item.crBkClr = ::GetSysColor(COLOR_WINDOW);
				}
				else
				{
					Item.nState = nformat | GVIS_READONLY;
					Item.strText = _T("");
					Item.crBkClr = ::GetSysColor(COLOR_3DSHADOW);
				}}
				break;
			case GR_MAX:
				{
				Item.mask |= GVIF_STATE|GVIF_BKCLR;
				Item.nFormat = DT_RIGHT |DT_WORDBREAK; //DT_CENTER|DT_VCENTER
				int nformat = m_pGridCtrl->GetDefaultCell(FALSE,FALSE)->GetState();


				if (i<nbPar)
				{
					double nVal = ((CModelPar *)obj)->GetMax();
					Item.nState = nformat & ~GVIS_READONLY;
					Item.strText.Format(_T("%.5g"),nVal);
					Item.crBkClr = ::GetSysColor(COLOR_WINDOW);
				}
				else
				{
					Item.nState = nformat | GVIS_READONLY;
					Item.strText = _T("");
					Item.crBkClr = ::GetSysColor(COLOR_3DSHADOW);
				}}
				break;
			case GR_COLOR:
				{
				Item.mask = GVIF_BKCLR|GVIF_PARAM;
				Item.mask |= GVIF_STATE;
				int nformat = m_pGridCtrl->GetDefaultCell(FALSE,FALSE)->GetState();
				Item.nState = nformat | GVIS_READONLY;
				Item.crBkClr = obj->GetColor();}
				break;
			case GR_DESC:
				Item.nFormat = DT_LEFT |DT_END_ELLIPSIS; //DT_CENTER|DT_VCENTER
				Item.strText = obj->GetDesc();
				break;
			}
			m_pGridCtrl->SetItem(&Item);
			if (j==GR_UNIT)
			{
				//m_pGridCtrl->SetCellType(i+1,j,NULL);
				if (!m_pGridCtrl->SetCellType(i+1,j, RUNTIME_CLASS(CGridCellCombo)))
					continue;
				//m_Grid.SetItemText(1,1, _T("www.codeproject.com"));
				CStringArray options;
				options.Add(_T("cm"));
				options.Add(_T("nb/ha"));
				options.Add(_T("l"));
				options.Add(_T("m^2"));
				CGridCellCombo *pCell = (CGridCellCombo*) m_pGridCtrl->GetCell(i+1,j);
				pCell->SetOptions(options);
			}
			else if (j==GR_COLOR)
			{
				int iComboCtlWidth = GetSystemMetrics( SM_CXVSCROLL);
				//if (!m_pGridCtrl->SetCellType(i+1,j, RUNTIME_CLASS(CGridCellColor)))
				//	continue;

				// retain old cell properties
				/*CGridBtnCell GridCellCopy;
				GridCellCopy.SetBtnDataBase( &m_BtnDataBase);
				CGridCellBase* pCurrCell = m_pGridCtrl->GetCell( i+1, j);
				if (pCurrCell && !pCurrCell->IsKindOf( RUNTIME_CLASS( CGridBtnCell) ))
				{
					GridCellCopy = *pCurrCell;  // data will be stored in m_BtnDataBase
					m_pGridCtrl->SetCellType( i+1, j, RUNTIME_CLASS(CGridBtnCell) );
				}
				CGridBtnCell* pGridBtnCell = (CGridBtnCell*)m_pGridCtrl->GetCell( i+1, j);
				// tell new cell where to get its property data
				pGridBtnCell->SetBtnDataBase( &m_BtnDataBase);
				pGridBtnCell->SetupBtns(
					0,                  // zero-based index of image to draw
					DFC_BUTTON,         // type of frame control to draw e.g. DFC_BUTTON
					DFCS_BUTTONPUSH,    // like DrawFrameControl()'s nState  e.g. DFCS_BUTTONCHECK
					CGridBtnCellBase::CTL_ALIGN_RIGHT,	// horizontal alignment of control image
					iComboCtlWidth,                  // fixed width of control or 0 for size-to-fit
					FALSE,              // T=btn is member of a radio group
					"Btn Text" );       // Text to insert centered in button; if NULL no text*/

			}
		}
	}
	m_pGridCtrl->AutoSizeColumns();
	m_pGridCtrl->AutoSizeColumn(NBCOLUMNS-1,GVS_HEADER);
	int wdth= m_pGridCtrl->GetColumnWidth(GR_UNIT);
	m_pGridCtrl->SetColumnWidth(GR_UNIT,wdth+20);
	m_pGridCtrl->SortItems(0,FALSE);
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
void CViewSDIaggreg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	BOOL bRedraw = FALSE;
	// TODO: Add your specialized code here and/or call the base class
	if (lHint == DOC_UPDATE_MODEL)
	{
		//bRedraw = TRUE;
		int nCurrModel = GetDocument()->m_nModSelItem;
		OnCellsUpdate(nCurrModel);
		//bRedraw = TRUE;
		if (m_pGridCtrl) m_pGridCtrl->Refresh();
	}
	if (bRedraw)
	{
		Invalidate();
		UpdateWindow();
	}

}

BOOL CViewSDIaggreg::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
    if (m_pGridCtrl && IsWindow(m_pGridCtrl->m_hWnd))
		if (m_pGridCtrl->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
			return TRUE;	
	return CView::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void CViewSDIaggreg::OnEndGridEdit(NMHDR* pNMHDR, LRESULT* pResult)
{
    GV_DISPINFO *pgvDispInfo = (GV_DISPINFO *)pNMHDR;
    GV_ITEM     *pgvItem = &pgvDispInfo->item;

	//In case OnEndInPlaceEdit called as window is being destroyed
	//if (!IsWindow(GetSafeHwnd()))
	//	return;

	UINT itemState = m_pGridCtrl->GetItemState(pgvItem->row,pgvItem->col);
	BOOL test = itemState & GVNI_MODIFIED;
	test = m_pGridCtrl->GetModified(pgvItem->row,pgvItem->col);
	CModelObj *obj = (CModelObj *)m_pGridCtrl->GetItemData(pgvItem->row,pgvItem->col);
	CModelPar *pParObj = DYNAMIC_DOWNCAST( CModelPar, obj);
	CModelVar *pVarObj = DYNAMIC_DOWNCAST( CModelVar, obj);

	CString mstr = m_pGridCtrl->GetItemText(pgvItem->row,pgvItem->col);
	CString newstr(_T(""));
	double val=0;
	BOOL bRedraw = TRUE;
	
	switch (pgvItem->col){
	case GR_NAME:
		obj->SetName(mstr);
		break;
	case GR_ABBREV:
		obj->SetAbbrev(mstr);
		break;
	case GR_UNIT:
		obj->SetUnit(mstr);
		m_pGridCtrl->SetItemText(pgvItem->row,pgvItem->col,mstr);
		break;
	case GR_COLOR:
		break;
	case GR_INIT:
		val = atof(mstr.GetBuffer(mstr.GetLength()));
		if (pParObj) pParObj->SetValue(val);
		newstr.Format(_T("%.5g"),val);
		if (newstr != mstr)
		{
			m_pGridCtrl->SetItemText(pgvItem->row,pgvItem->col,newstr);
			bRedraw = TRUE;

		}
		//if (pVarObj) pVarObj->SetInitValue(val);
		break;
	case GR_MIN:
		val = atof(mstr.GetBuffer(mstr.GetLength()));
		if (pParObj) pParObj->SetMin(val);
		newstr.Format(_T("%.5g"),val);
		if (newstr != mstr)
		{
			m_pGridCtrl->SetItemText(pgvItem->row,pgvItem->col,newstr);
			bRedraw = TRUE;

		}
		//if (pVarObj) pVarObj->SetInitValue(val);
		break;
	case GR_MAX:
		val = atof(mstr.GetBuffer(mstr.GetLength()));
		if (pParObj) pParObj->SetMax(val);
		newstr.Format(_T("%.5g"),val);
		if (newstr != mstr)
		{
			m_pGridCtrl->SetItemText(pgvItem->row,pgvItem->col,newstr);
			bRedraw = TRUE;

		}
		break;
	case GR_DESC:
		obj->SetDesc(mstr);
		break;
	default:
		bRedraw = FALSE;
		break;
	}
	if (bRedraw)
	{
		GetDocument()->SetModifiedFlag();
		m_pGridCtrl->RedrawCell(pgvItem->row,pgvItem->col);
	}
	*pResult = 0;
}

void CViewSDIaggreg::OnGridClick(NMHDR *pNotifyStruct, LRESULT* pResult)
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;

    int nRow = pItem->iRow;
    int nCol = pItem->iColumn;

	if (nCol==GR_COLOR && nRow)
	{
		CModelObj *obj = (CModelObj *)m_pGridCtrl->GetItemData(nRow,nCol);
		COLORREF oldClr = obj->GetColor();

		//MessageBox("fdfdfdfdfd");
		CColorDialog dlg(oldClr,CC_RGBINIT);
		if (dlg.DoModal() == IDOK)
		{
			m_pGridCtrl->SetItemBkColour(nRow,nCol,dlg.m_cc.rgbResult);

			obj->SetColor(dlg.m_cc.rgbResult);
			GetDocument()->SetModifiedFlag();
			m_pGridCtrl->RedrawCell(nRow,nCol);
		}


	}
}