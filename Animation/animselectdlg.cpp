// AnimSelectDlg.cpp : implementation file
//

#include "stdafx.h"
#include "simul.h"
#include "AnimSelectDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "MRTracker.h"

#include <MInteraction\SimulAnimat.h>


/////////////////////////////////////////////////////////////////////////////
// CSelectorDlg dialog


CSelectorDlg::CSelectorDlg(CView* pParent /*=NULL*/)
	: CHelpDlg(CSelectorDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSelectorDlg)
	m_nView = 0;
	//}}AFX_DATA_INIT

	//pView = pParent;
	pmultiTrack = NULL;
	m_ImageList.Create(IDB_ANIM_COMPONENT,16,0,RGB(192,192,192));
	m_BtnImageList.Create(IDB_ANIM_SELECTOR,16,0,RGB(192,192,192));
	m_cObjSet = NULL;
}


void CSelectorDlg::DoDataExchange(CDataExchange* pDX)
{
	CHelpDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSelectorDlg)
	DDX_Control(pDX, IDC_VIEW_SAVE, m_cViewSave);
	DDX_Control(pDX, IDC_VIEW_LOAD, m_cViewLoad);
	DDX_Control(pDX, IDC_OBJ_LIST, m_cObjList);
	DDX_Control(pDX, IDC_VIEW_ICON, m_cViewIcon);
	DDX_Control(pDX, IDC_VIEW_LIST, m_cViewList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSelectorDlg, CHelpDlg)
	//{{AFX_MSG_MAP(CSelectorDlg)
	ON_BN_CLICKED(IDC_VIEW_ICON, OnViewIcon)
	ON_BN_CLICKED(IDC_VIEW_LIST, OnViewList)
	ON_NOTIFY(LVN_ENDLABELEDIT, IDC_OBJ_LIST, OnEndObjLabelEdit)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_OBJ_LIST, OnItemChanged)
	ON_WM_SIZE()
	ON_WM_GETMINMAXINFO()
	ON_NOTIFY(NM_DBLCLK, IDC_OBJ_LIST, OnDblclkObjList)
	ON_NOTIFY(LVN_KEYDOWN, IDC_OBJ_LIST, OnKeydownObjList)
	ON_BN_CLICKED(IDC_VIEW_LOAD, OnLoad)
	ON_BN_CLICKED(IDC_VIEW_SAVE, OnSave)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSelectorDlg message handlers

BOOL CSelectorDlg::OnInitDialog() 
{
	CHelpDlg::OnInitDialog();

	m_cObjList.SetImageList(&m_ImageList,LVSIL_NORMAL);
	m_cObjList.SetImageList(&m_ImageList,LVSIL_SMALL);

	// TODO: Add extra initialization here
//	m_cViewIcon.SetToolTipText("'BS_PUSHLIKE'", "pushed state ('BS_PUSHLIKE')");
//	m_cViewList.SetToolTipText("flat and 'BS_PUSHLIKE'", "flat pushed ('BS_PUSHLIKE')");
	
	//m_cViewIcon.SetIcon(IDI_VIEW_ICON,BS_LEFT);
	//m_cViewList.SetIcon(IDI_VIEW_LIST,BS_LEFT);
	m_cViewIcon.SetIcon(m_BtnImageList.ExtractIcon(0));
	m_cViewList.SetIcon(m_BtnImageList.ExtractIcon(1));
	m_cViewLoad.SetIcon(m_BtnImageList.ExtractIcon(2));
	m_cViewSave.SetIcon(m_BtnImageList.ExtractIcon(3));
	//m_cViewIcon.SetShade(SHS_METAL);
	//m_cViewList.SetShade(SHS_METAL);
	m_cViewIcon.SetShowText(FALSE);
	m_cViewList.SetShowText(FALSE);
	m_cViewLoad.SetShowText(FALSE);
	m_cViewSave.SetShowText(FALSE);

	m_cViewIcon.SetTooltipText(_T("Display Components as Icons"));
	m_cViewList.SetTooltipText(_T("Display Components as a List"));
	m_cViewLoad.SetTooltipText(_T("Load a new set of components"));
	m_cViewSave.SetTooltipText(_T("Save these components in a file"));

	OnViewIcon();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSelectorDlg::OnViewIcon() 
{
	// TODO: Add your control notification handler code here
	m_cViewIcon.SetCheck(TRUE);
	m_cViewList.SetCheck(FALSE);
	if (m_nView==1) return;

	m_cObjList.DeleteAllItems();
	m_nView = 1;

	LONG nlong = ::GetWindowLong(m_cObjList,GWL_STYLE);
	nlong &= ~LVS_REPORT;
	nlong |= LVS_ICON;
	::SetWindowLong(m_cObjList,GWL_STYLE,nlong);
	int nb = m_cObjSet->GetSize();
	for (int i=0;i<nb;i++)
	{
		CAnimObject		*pObj = m_cObjSet->GetAt(i);
		if (!pObj) continue;
		CString mstr = pObj->GetObjectDef();
		int nIcon = pObj->GetObjectIcon();
		LV_ITEM lvi;
		lvi.mask = LVIF_TEXT | LVIF_PARAM | LVIF_IMAGE; //| LVIF_IMAGE | LVIF_STATE;
		lvi.iItem = i;
		lvi.iSubItem = 0;
		lvi.lParam = (LPARAM)pObj;
		lvi.iImage = nIcon;
		lvi.pszText = mstr.GetBuffer(mstr.GetLength());
		m_cObjList.InsertItem(&lvi);
	}
	OnItemChanged(NULL,NULL);
}

void CSelectorDlg::OnViewList() 
{
	// TODO: Add your control notification handler code here
	m_cViewIcon.SetCheck(FALSE);
	m_cViewList.SetCheck(TRUE);
	if (m_nView==2) return;
	m_cObjList.DeleteAllItems();
	m_cObjList.DeleteColumn(0);
	m_cObjList.DeleteColumn(0);
	m_cObjList.DeleteColumn(0);

	m_nView = 2;

	LONG nlong = ::GetWindowLong(m_cObjList,GWL_STYLE);
	nlong &= ~LVS_ICON;
	//nlong &= ~(LVS_SORTASCENDING | LVS_SORTDESCENDING);
	//nlong &= ~LVS_NOSORTHEADER ;
	nlong |= LVS_REPORT ;//| LVS_SORTASCENDING;
	::SetWindowLong(m_cObjList,GWL_STYLE,nlong);

	CRect rect;
	
	m_cObjList.GetWindowRect(&rect);
	m_cObjList.InsertColumn(0, _T("Component"), LVCFMT_LEFT,
		-20+rect.Width() * 1/4, 0);
	m_cObjList.InsertColumn(1, _T("Type"), LVCFMT_LEFT,
		-1+rect.Width() * 1/2, 1);
	m_cObjList.InsertColumn(2, _T("Linked to"), LVCFMT_LEFT,
		rect.Width() * 1/4, 2);
	//ModifyHeaderItems();
	int nb = m_cObjSet->GetSize();
	for (int i=0;i<nb;i++)
	{
		CAnimObject		*pObj = m_cObjSet->GetAt(i);
		if (!pObj) continue;
		CString mstr = pObj->GetObjectDef();
		int nIcon = pObj->GetObjectIcon();
		LV_ITEM lvi;
		lvi.mask = LVIF_TEXT | LVIF_PARAM | LVIF_IMAGE; //| LVIF_IMAGE | LVIF_STATE;
		lvi.iImage = nIcon;
		lvi.iItem = i;
		lvi.iSubItem = 0;
		lvi.lParam = (LPARAM)pObj;
		lvi.pszText = mstr.GetBuffer(mstr.GetLength());
		int nActItem = m_cObjList.InsertItem(&lvi);

		CString tmp;
		pObj->SetObjectDef(_T(""));
		tmp = pObj->GetObjectDef();
		pObj->SetObjectDef(mstr);

		int deb = tmp.Find(_T("n."));
		tmp = tmp.Left(deb-1);
		LV_ITEM lvi2;
		lvi2.mask = LVIF_TEXT ;//| LVIF_IMAGE;// | LVIF_STATE;
		lvi2.iItem = nActItem;
		lvi2.iSubItem = 1;
		lvi2.pszText = tmp.GetBuffer(tmp.GetLength());
		m_cObjList.SetItem(&lvi2);

		if (pObj->m_pRelatedObj)
		{
			CString strLink = pObj->m_pRelatedObj->GetObjectDef();
			LV_ITEM lvi3;
			lvi3.mask = LVIF_TEXT ;//| LVIF_IMAGE;// | LVIF_STATE;
			lvi3.iItem = nActItem;
			lvi3.iSubItem = 2;
			lvi3.pszText = strLink.GetBuffer(strLink.GetLength());
			m_cObjList.SetItem(&lvi3);
		}
		CLinkObject *pLink = DYNAMIC_DOWNCAST(CLinkObject, pObj);
		if (pLink)
		{
			CString strIni,strEnd;
			if (pLink->m_pInit)
				strIni = pLink->m_pInit->GetObjectDef();
			if (pLink->m_pEnd)
				strEnd = pLink->m_pEnd->GetObjectDef();

			CString strLink = strIni + _T(" <-> ") + strEnd;
			LV_ITEM lvi3;
			lvi3.mask = LVIF_TEXT ;//| LVIF_IMAGE;// | LVIF_STATE;
			lvi3.iItem = nActItem;
			lvi3.iSubItem = 2;
			lvi3.pszText = strLink.GetBuffer(strLink.GetLength());
			m_cObjList.SetItem(&lvi3);
		}

	}

}

void CSelectorDlg::UpdateList()
{
	int nb = m_nView;
	m_nView = 0;
	if (nb==1)
		OnViewIcon();
	else if (nb==2)
		OnViewList();
}

void CSelectorDlg::OnEndObjLabelEdit(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	// TODO: Add your control notification handler code here
	if (!pDispInfo) return; 

	int nItem = pDispInfo->item.iItem;
	if (!pDispInfo->item.lParam) return;

	CString mstr = pDispInfo->item.pszText;
	CAnimObject	*pObj = (CAnimObject*)pDispInfo->item.lParam;
	pObj->SetObjectDef(mstr);

	CString mstr2 = pObj->GetObjectDef();

	m_cObjList.SetItemText(pDispInfo->item.iItem,0,mstr2);


	*pResult = 0;
}

void CSelectorDlg::OnItemChanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	UINT nState = 0;

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	if (pNMListView)
		nState = m_cObjList.GetItemState(pNMListView->iItem,LVIS_SELECTED);

//	CWnd *pWnd = GetDlgItem(IDC_OBJ_SELECT);
//	if (pWnd) pWnd->EnableWindow((BOOL)nState);
//	pWnd = GetDlgItem(IDC_OBJ_PROP);
//	if (pWnd) pWnd->EnableWindow((BOOL)nState);

	if (nState && pNMListView->uNewState==3)
	{
		CAnimObject	*pObj = (CAnimObject*)pNMListView->lParam;
		if (pmultiTrack)
		{
			pmultiTrack->RemoveAll();
			pmultiTrack->Add(pObj);
			pView->Invalidate();
			pView->UpdateWindow();
		}
	}
	else if (!nState)
	{
		if (pmultiTrack)
		{
			pmultiTrack->RemoveAll();
			//pmultiTrack->Add(pObj);
			pView->Invalidate();
			pView->UpdateWindow();
		}
	}

	if (pResult) *pResult = 0;
}

void CSelectorDlg::OnSize(UINT nType, int cx, int cy) 
{
	CHelpDlg::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if (m_cObjList.GetSafeHwnd())
	{
		CRect mrect,mrect2;
		GetWindowRect(mrect);
		m_cObjList.GetWindowRect(mrect2);
		//m_cObjList.GetClientRect(mrect);
		CSize ms = mrect2.TopLeft()-mrect.TopLeft();
		m_cObjList.SetWindowPos(NULL,0,0,cx-12,cy-40,SWP_NOMOVE|SWP_NOZORDER);
		m_cObjList.Arrange(LVA_DEFAULT);
	}
}

void CSelectorDlg::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	// TODO: Add your message handler code here and/or call default
	//lpMMI->ptMinTrackSize = CPoint(348,266);
	lpMMI->ptMinTrackSize = CPoint(220,180);
	CHelpDlg::OnGetMinMaxInfo(lpMMI);
}

void CSelectorDlg::OnDblclkObjList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	if (!pNMListView) return;
	if (pNMListView->iItem == -1) return;
	//if (!pNMListView->lParam) return;

	//CAnimObject	*pObj = (CAnimObject*)pNMListView->lParam;

	CAnimObject	*pObj = (CAnimObject*)m_cObjList.GetItemData(pNMListView->iItem);
	if (pObj)
	{
		int res = pObj->OnEditProperty(pView);
		if (res==IDOK)
		{
			pView->OnUpdateData();
			pView->Invalidate();
			pView->UpdateWindow();
		}
	}
	*pResult = 0;
}

void CSelectorDlg::OnKeydownObjList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_KEYDOWN* pLVKeyDow = (LV_KEYDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	//pLVKeyDow->wVKey;
	//pLVKeyDow->flags;
	if (pLVKeyDow && pLVKeyDow->wVKey == VK_DELETE)
	{
		CxAnimObjectList myList;
		int nb = m_cObjList.GetItemCount();
		for (int i=0;i<nb;i++)
		{
			if (!m_cObjList.GetItemState(i,LVIS_SELECTED)) continue;

			CAnimObject	*pObj = (CAnimObject*)m_cObjList.GetItemData(i);
			if (!pObj) continue;
			myList.Add(pObj);
		}
		nb = myList.GetSize();

		pView->SendMessage(WM_COMMAND,ID_EDIT_CLEAR);
//		pView->OnUpdateData();
//		pView->Invalidate();
//		pView->UpdateWindow();
	}

	*pResult = 0;
}

void CSelectorDlg::OnLoad() 
{
	// TODO: Add your control notification handler code here
	CFileDialog dlg( TRUE,_T("cpn"),_T("*.cpn"),
					 OFN_FILEMUSTEXIST | OFN_HIDEREADONLY,
					 _T("DEMIST Component File (*.cpn)|*.cpn|"));
	//dlg.m_ofn.lpstrInitialDir = "C:\\nicolas\\temp\\s3\\";
	if(dlg.DoModal()!=IDOK ) return;

	CString m_sFileName = dlg.GetPathName();
			
	CDocument	mdoc;
	CFileException fe;
	CFile* pFile = NULL;
	pFile = new CFile(m_sFileName, CFile::modeRead|CFile::shareDenyWrite);

	if (pFile == NULL)
	{
		mdoc.ReportSaveLoadException(m_sFileName, &fe,
						TRUE, AFX_IDP_INVALID_FILENAME);
		return /*FALSE*/;
	}

	CTypedPtrArray<CObArray,CAnimObject*>	mlocObjSet;			// List of objects
	CArchive saveArchive(pFile, CArchive::load | CArchive::bNoFlushOnDelete);
	saveArchive.m_pDocument = &mdoc;
	saveArchive.m_bForceFlat = FALSE;
	TRY
	{
		CWaitCursor wait;
		//Serialize(saveArchive);     // save me
		if (pFile->GetLength() != 0)
		{
			mlocObjSet.Serialize(saveArchive);
		}
		saveArchive.Close();
		//ReleaseFile(pFile, FALSE);
		pFile->Close();
		delete pFile;
	}
	CATCH_ALL(e)
	{
		pFile->Abort(); // will not throw an exception
		delete pFile;

		TRY
		{
			mdoc.ReportSaveLoadException(m_sFileName, e,
				TRUE, AFX_IDP_FAILED_TO_SAVE_DOC);
		}
		END_TRY
//		DELETE_EXCEPTION(e);
		for (int jj= 0;jj<mlocObjSet.GetSize();jj++)
		{
			CAnimObject *pObj = mlocObjSet.GetAt(jj);
			if (pObj) delete pObj;
		}
		mlocObjSet.RemoveAll();
		return /*FALSE*/;
	}
	END_CATCH_ALL

	for(int i = 0; i < m_cObjSet->GetSize();i++ )
		delete m_cObjSet->GetAt(i);
	m_cObjSet->RemoveAll();

	CxAnimObjectList pList;
	CopyElt(&mlocObjSet,&pList);
	for (int jj= 0;jj<mlocObjSet.GetSize();jj++)
	{
		CAnimObject *pObj = mlocObjSet.GetAt(jj);
		if (!pObj) continue;
		
		m_cObjSet->Add(pObj);
		pObj->SetObjectDoc(pView->GetDocument());

		if (pObj && (pObj->m_nRelatedID != -1))
		{
			CAnimObject *pObj2 = m_cObjSet->GetAt(pObj->m_nRelatedID);
			pObj->m_pRelatedObj = pObj2;
		}
		CLinkObject *pLO = DYNAMIC_DOWNCAST(CLinkObject,pObj);
		if (pLO)
		{
			CAnimObject *p1=NULL;
			CAnimObject *p2=NULL;
			if (pLO->m_nIDInit != -1)
			{
				//CAnimObject *pObj2 = m_cObjSet->GetAt(pLO->m_nIDInit);
				p1 = pList.GetAt(pLO->m_nIDInit-1);
				//pLO->LinkToObjects(pObj2,NULL);

				//pLO->m_pInit = pObj2;
			}
			if (pLO->m_nIDEnd != -1)
			{
				//CAnimObject *pObj2 = m_cObjSet->GetAt(pLO->m_nIDEnd);
				p2 = pList.GetAt(pLO->m_nIDEnd-1);
				//pLO->m_pEnd = pObj2;
			}
			pLO->LinkToObjects(p1,p2);
		}
	}
	mlocObjSet.RemoveAll();
	pmultiTrack->RemoveAll();

//	UpdateComponentList();
//	m_CBAnimCat.SetCurSel(-1);
	pView->Invalidate();
	pView->UpdateWindow();
	UpdateList();
}

void CSelectorDlg::OnSave() 
{
	// TODO: Add your control notification handler code here
	CFileDialog dlg( FALSE,_T("cpn"),_T("*.cpn"),
					 OFN_FILEMUSTEXIST | OFN_HIDEREADONLY,
					 _T("DEMIST Component File (*.cpn)|*.cpn|"));
	//dlg.m_ofn.lpstrInitialDir = "C:\\nicolas\\temp\\s3\\";
	if(dlg.DoModal()!=IDOK) return;

	CString m_sFileName = dlg.GetPathName();
			
	CDocument	mdoc;
	CFileException fe;
	CFile* pFile = NULL;
	pFile = new CFile(m_sFileName, CFile::modeCreate |
					CFile::modeReadWrite | CFile::shareExclusive);

	if (pFile == NULL)
	{
		mdoc.ReportSaveLoadException(m_sFileName, &fe,
			TRUE, AFX_IDP_INVALID_FILENAME);
		return /*FALSE*/;
	}

	CArchive saveArchive(pFile, CArchive::store | CArchive::bNoFlushOnDelete);
	saveArchive.m_pDocument = &mdoc;
	saveArchive.m_bForceFlat = FALSE;
	TRY
	{
		CWaitCursor wait;
		m_cObjSet->Serialize(saveArchive);
		saveArchive.Close();
		pFile->Close();
		delete pFile;
	}
	CATCH_ALL(e)
	{
		pFile->Abort(); // will not throw an exception
		delete pFile;

		TRY
		{
			mdoc.ReportSaveLoadException(m_sFileName, e,
				TRUE, AFX_IDP_FAILED_TO_SAVE_DOC);
		}
		END_TRY
//		DELETE_EXCEPTION(e);
		return /*FALSE*/;
	}
	END_CATCH_ALL
}
