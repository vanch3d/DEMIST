// AnimInterPage.cpp : implementation file
//

#include "stdafx.h"
#include "simul.h"
#include "AnimInterPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAnimInterPage dialog


CAnimInterPage::CAnimInterPage() : CAnimPage(CAnimInterPage::IDD)
{
	//{{AFX_DATA_INIT(CAnimInterPage)
	m_nDragX = 0;
	m_nDragY = 0;
	m_nMapRel = 0;
	m_nResH = 0;
	m_nResW = 0;
	//}}AFX_DATA_INIT

	m_pImageList.Create(IDB_VAR_ICONS,14,0,RGB(128,128,128));

	m_sDragX.Add(1);
	m_sDragX.Add(5);
	m_sDragY.Add(1);
	m_sResW.Add(2);
	m_sResW.Add(3);
	m_sMapRel.Add(1);

}


void CAnimInterPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAnimInterPage)
	DDX_Control(pDX, IDC_DRAG_X, m_wndDragX);
	DDX_Control(pDX, IDC_DRAG_Y, m_wndDragY);
	DDX_Control(pDX, IDC_RESIZE_W, m_wndResizW);
	DDX_Control(pDX, IDC_RESIZE_H, m_wndResizH);
	DDX_Control(pDX, IDC_MAPREL, m_wndMapRel);
	DDX_CBIndex(pDX, IDC_DRAG_X, m_nDragX);
	DDX_CBIndex(pDX, IDC_DRAG_Y, m_nDragY);
	DDX_CBIndex(pDX, IDC_MAPREL, m_nMapRel);
	DDX_CBIndex(pDX, IDC_RESIZE_H, m_nResH);
	DDX_CBIndex(pDX, IDC_RESIZE_W, m_nResW);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAnimInterPage, CPropertyPage)
	//{{AFX_MSG_MAP(CAnimInterPage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAnimInterPage message handlers

BOOL CAnimInterPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_wndDragX.SetImageList(&m_pImageList);
	m_wndDragY.SetImageList(&m_pImageList);
	m_wndResizW.SetImageList(&m_pImageList);
	m_wndResizH.SetImageList(&m_pImageList);
	m_wndMapRel.SetImageList(&m_pImageList);

	int nb = m_sOutcomeIcon.GetSize();

	m_wndDragX.ResetContent();
	m_wndDragY.ResetContent();
	m_wndResizW.ResetContent();
	m_wndResizH.ResetContent();
	m_wndMapRel.ResetContent();

	m_wndDragX.AddBitmap(-1,_T("< none >"));
	m_wndDragY.AddBitmap(-1,_T("< none >"));
	m_wndResizW.AddBitmap(-1,_T("< none >"));
	m_wndResizH.AddBitmap(-1,_T("< none >"));
	m_wndMapRel.AddBitmap(-1,_T("< none >"));

/*	int nbS1 = m_sVarList.GetSize();
	int nbS2 = m_sOutcomeIcon.GetSize();
	for (int i=0;i<nbS1;i++)
	{
		UINT nb1 = m_sOutcomeIcon.GetAt(i);
		CString mstr = m_sVarList.GetAt(i);
		m_wndDragX.AddBitmap(nb1,mstr);
		m_wndDragY.AddBitmap(nb1,mstr);
		m_wndResizW.AddBitmap(nb1,mstr);
		m_wndResizH.AddBitmap(nb1,mstr);
		m_wndMapRel.AddBitmap(nb1,mstr);

	}*/

	for (int i=0;i<m_sDragX.GetSize();i++)
	{
		UINT nb1 = m_sDragX.GetAt(i);
		UINT nb2 = m_sOutcomeIcon.GetAt(nb1);
		CString mstr = m_sVarList.GetAt(nb1);
		m_wndDragX.AddBitmap(nb2,mstr);
	}
	for (i=0;i<m_sDragY.GetSize();i++)
	{
		UINT nb1 = m_sDragY.GetAt(i);
		UINT nb2 = m_sOutcomeIcon.GetAt(nb1);
		CString mstr = m_sVarList.GetAt(nb1);
		m_wndDragY.AddBitmap(nb2,mstr);
	}

	for (i=0;i<m_sResW.GetSize();i++)
	{
		UINT nb1 = m_sResW.GetAt(i);
		UINT nb2 = m_sOutcomeIcon.GetAt(nb1);
		CString mstr = m_sVarList.GetAt(nb1);
		m_wndResizW.AddBitmap(nb2,mstr);
	}
	for (i=0;i<m_sResH.GetSize();i++)
	{
		UINT nb1 = m_sResH.GetAt(i);
		UINT nb2 = m_sOutcomeIcon.GetAt(nb1);
		CString mstr = m_sVarList.GetAt(nb1);
		m_wndResizH.AddBitmap(nb2,mstr);
	}
	for (i=0;i<m_sMapRel.GetSize();i++)
	{
		UINT nb1 = m_sMapRel.GetAt(i);
		UINT nb2 = m_sOutcomeIcon.GetAt(nb1);
		CString mstr = m_sVarList.GetAt(nb1);
		m_wndMapRel.AddBitmap(nb2,mstr);
	}



	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
