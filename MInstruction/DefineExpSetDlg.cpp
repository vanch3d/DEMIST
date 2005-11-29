// DefineExpSetDlg.cpp : implementation file
//

#include "stdafx.h"
#include "simul.h"

#include <MInstruction\DefineExpSetDlg.h>
#include <MSimulation\Model.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDefineExpSetDlg dialog


CDefineExpSetDlg::CDefineExpSetDlg(CWnd* pParent /*=NULL*/,
								   CMdEquation *pEqu /*= NULL*/,
								   CMdExpertSet *pExpSet /*= NULL*/)
	: CHelpDlg(CDefineExpSetDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDefineExpSetDlg)
	m_ExpName = pExpSet->GetName();
	//}}AFX_DATA_INIT
	m_pEqu = pEqu;
	m_pExpSet = pExpSet;
	m_pImageList.Create(IDB_VAR_ICONS,14,0,RGB(128,128,128));
}


void CDefineExpSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CHelpDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDefineExpSetDlg)
	DDX_Text(pDX, IDC_EXPSET_NAME, m_ExpName);
	//}}AFX_DATA_MAP

	DDX_GridControl(pDX, IDC_EXPSET_PARAM, m_Parlist);
	//DDX_GridControl(pDX, IDC_EXPSET_INIT, m_InitList);
}


BEGIN_MESSAGE_MAP(CDefineExpSetDlg, CHelpDlg)
	//{{AFX_MSG_MAP(CDefineExpSetDlg)
	ON_BN_CLICKED(IDC_EXPSET_DEFAULT, OnDefaultValue)
	//}}AFX_MSG_MAP
	ON_NOTIFY(GVN_ENDLABELEDIT,IDC_EXPSET_PARAM,OnParamEditChange)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDefineExpSetDlg message handlers

BOOL CDefineExpSetDlg::OnInitDialog() 
{
	CHelpDlg::OnInitDialog();

	m_Parlist.SetTextBkColor(RGB(0xFF, 0xFF, 0xE0));
	//m_InitList.SetTextBkColor(RGB(0xFF, 0xFF, 0xE0));

	m_Parlist.SetColumnCount(2);
	m_Parlist.SetFixedColumnCount(1);
	//m_InitList.SetColumnCount(2);
	//m_InitList.SetFixedColumnCount(1);
	////m_Parlist.SetEditable(true);
	////m_Parlist.SetListMode(true);
	////m_Parlist.EnableDragAndDrop(FALSE);
	////m_Parlist.SetSingleRowSelection(true);
	m_Parlist.SetRowResize(false);
	//m_InitList.SetRowResize(false);
	m_Parlist.SetColumnResize(true);
	//m_InitList.SetColumnResize(true);

	m_Parlist.SetImageList(&m_pImageList);
	//m_InitList.SetImageList(&m_pImageList);

	

	int nbparam = m_pExpSet->GetSize();
	//int nbinit = 0;
	int nb2 = m_pEqu->m_cParSet.GetSize();

	/*for (int row = 0; row < nb2; row++)
	{
		CModelPar *pPar= m_pEqu->m_cParSet.GetAt(row);
		if (pPar->m_InitValue != -1)
		{
			nbparam--;
			nbinit++;
		}
	}*/

	//m_Parlist.SetRowCount(nbparam);
	m_Parlist.SetRowCount(nb2);
	//m_InitList.SetRowCount(nbinit);

	DWORD dwTextStyle = DT_LEFT|DT_VCENTER|DT_SINGLELINE;
	int row1=0,row2=0;
	//for (row = 0; row < nb2; row++)
	for (int row = 0; row < nb2; row++)
	{
		CModelPar *pPar= m_pEqu->m_cParSet.GetAt(row);
		CString name = pPar->GetName();
		double	val = m_pExpSet->GetValue(row);
		int nbImage = pPar->GetIcon();

		GV_ITEM Item,Item2;
		Item.mask = GVIF_TEXT|GVIF_FORMAT|GVIF_PARAM;
		Item.nFormat = dwTextStyle;
		Item.lParam = row;
		Item2 = Item;

		Item.col = 0;
		Item.strText = name;
		Item.mask |=GVIF_IMAGE;
		Item.iImage = nbImage;

		Item2.col = 1;
		name.Format(_T("%.4g"),val);
		Item2.strText = name;

	/*	if (pPar->m_InitValue != -1)
		{
			Item.row = row2;
			Item2.row = row2;
			m_InitList.SetItem(&Item);
			m_InitList.SetItem(&Item2);
			row2++;
		}
		else*/
		{	
			row1 = row;
			Item.row = row1;
			Item2.row = row1;
			m_Parlist.SetItem(&Item);
			m_Parlist.SetItem(&Item2);
		//	row1++;
		}

	}

	//m_InitList.AutoSizeColumn(0);
	m_Parlist.AutoSizeColumn(0);
	//int s1 = m_InitList.GetColumnWidth(0);
	int s2 = m_Parlist.GetColumnWidth(0);
	//int s3 = max(s1,s2);
	//m_InitList.SetColumnWidth(0,s3);
	//m_Parlist.SetColumnWidth(0,s3);
	m_Parlist.SetColumnWidth(0,s2);
	m_Parlist.ExpandColumnsToFit();
	//m_InitList.ExpandColumnsToFit();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CDefineExpSetDlg::OnOK()
{
	//int nb = m_InitList.GetRowCount();
/*	for (int i=0;i<nb;i++)
	{
		int row = (int)m_InitList.GetItemData(i,1);
		CString str = m_InitList.GetItemText(i,1);
		double val = atof(str);
		CModelPar *pPar= m_pEqu->m_cParSet.GetAt(row);
		CString name = pPar->GetName();
		m_pExpSet->SetValue(row,val);
	}*/
	int nb = m_Parlist.GetRowCount();
	for (int i=0;i<nb;i++)
	{
		int row = (int)m_Parlist.GetItemData(i,1);
		CString str = m_Parlist.GetItemText(i,1);
		double val = atof(str);
		CModelPar *pPar= m_pEqu->m_cParSet.GetAt(row);
		CString name = pPar->GetName();
		m_pExpSet->SetValue(row,val);
	}
	CHelpDlg::OnOK();
}

void CDefineExpSetDlg::OnDefaultValue() 
{
	// TODO: Add your control notification handler code here
	
}

void CDefineExpSetDlg::OnParamEditChange(NMHDR* pNMHDR, LRESULT* pResult)
{
    GV_DISPINFO *pgvDispInfo = (GV_DISPINFO *)pNMHDR;
    GV_ITEM     *pgvItem = &pgvDispInfo->item;
	*pResult = 1;

}
