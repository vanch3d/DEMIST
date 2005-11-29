// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "Simul.h"
#include "ChildFrm.h"

#include "Tools\BCMenu.h"


#include "Tools\MVDocTemplate.h"
#include "SimulDoc.h"

#include "MainFrm.h"

#include <MInstruction\LearningUnit.h>

#include <Prefs\Pref.h>
#include "IdentityDlg.h"
#include "LearnerTrace.h"

#include "Prefs\TraceInfoPage.h"
#include "Prefs\TraceConfigPage.h"
#include "Prefs\PrefSimulPage.h"

#include <afxpriv.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


extern CSimulApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_COMMAND(ID_VIEW_FULLSCREEN, OnViewFullscreen)
	ON_UPDATE_COMMAND_UI(ID_VIEW_FULLSCREEN, OnUpdateViewFullscreen)
	ON_NOTIFY(TBN_DROPDOWN, AFX_IDW_TOOLBAR, OnToolbarDropDown)
	ON_NOTIFY(TBN_DROPDOWN, AFX_IDW_CONTROLBAR_FIRST + 20 + 1, OnToolbarDropDown)
	ON_WM_SHOWWINDOW()
	ON_COMMAND(ID_FILE_PREF, OnFilePref)
	ON_COMMAND(ID_FILE_SWITCHMODE, OnFileSwitchMode)
	ON_WM_ACTIVATE()
	ON_COMMAND(ID_VIEW_TASKBAR, OnViewTaskbar)
	ON_UPDATE_COMMAND_UI(ID_VIEW_TASKBAR, OnUpdateViewTaskbar)
	ON_UPDATE_COMMAND_UI(ID_FILE_PREF, OnUpdateFilePref)
	ON_UPDATE_COMMAND_UI(ID_FILE_SWITCHMODE, OnUpdateSwitchMode)
	ON_WM_TIMER()
	ON_COMMAND(ID_VIEW_ERS, OnViewERs)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ERS, OnUpdateViewERs)
	//}}AFX_MSG_MAP
	// Global help commands
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_MODE, OnUpdateMode)
	ON_COMMAND(ID_CONTEXT_HELP, CMDIFrameWnd::OnContextHelp)
	ON_COMMAND(ID_DEFAULT_HELP, CMDIFrameWnd::OnHelpFinder)
	ON_COMMAND(ID_HELP_FINDER, CMDIFrameWnd::OnHelpFinder)
	ON_COMMAND(ID_HELP_INDEX, CMDIFrameWnd::OnHelpIndex)
	ON_COMMAND(ID_HELP_USING, CMDIFrameWnd::OnHelpUsing)
	ON_COMMAND(ID_HELP, CMDIFrameWnd::OnHelp)
	//ON_NOTIFY_EX( TTN_NEEDTEXT, 0, OnMERIconsToolTips)
	ON_NOTIFY_EX( TTN_NEEDTEXT, 0, OnMERIconsToolTips)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTW, 0, 0xFFFF, OnToolTipText)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTA, 0, 0xFFFF, OnToolTipText)
	ON_MESSAGE(WM_SETMESSAGESTRING, OnSetMessageString)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_MODE,
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
//	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	m_FullScreenMode=false;
	
}

CMainFrame::~CMainFrame()
{
}

BOOL CMainFrame::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	CString sProfile = _T("BarState");
	CSizingControlBar::GlobalSaveState(this, sProfile);
	SaveBarState(sProfile);

	CPref::WritePref();
	
	return CMDIFrameWnd::DestroyWindow();
}

LRESULT CMainFrame::OnSetMessageString(WPARAM wParam, LPARAM lParam)
{
	UINT nIDLast = m_nIDLastMessage;
	m_nFlags &= ~WF_NOPOPMSG;

	CWnd* pMessageBar = GetMessageBar();
	if (pMessageBar != NULL)
	{
		LPCTSTR lpsz = NULL;
		CString strMessage;

		// set the message bar text
		if (lParam != 0)
		{
			ASSERT(wParam == 0);    // can't have both an ID and a string
			lpsz = (LPCTSTR)lParam; // set an explicit string
		}
		else if (wParam != 0)
		{
			// map SC_CLOSE to PREVIEW_CLOSE when in print preview mode
			if (wParam == AFX_IDS_SCCLOSE && m_lpfnCloseProc != NULL)
				wParam = AFX_IDS_PREVIEW_CLOSE;

			// get message associated with the ID indicated by wParam
			if (wParam > ID_VIEW_EXTREP1 && wParam <= ID_VIEW_EXTREP20)
			{
				int nSize = m_strToolTip.GetSize();
				int nItem = wParam - ID_VIEW_EXTREP1;
				if (nItem>=nSize)
					GetMessageString(nIDLast, strMessage);
				else
				{
					CString mstr;
					GetMessageString(wParam, mstr);
					mstr += _T(" : ") + m_strToolTip.GetAt(nItem);

					strMessage = mstr;
				}
			}
			else 
				GetMessageString(wParam, strMessage);
			lpsz = strMessage;
		}
		pMessageBar->SetWindowText(lpsz);

		// update owner of the bar in terms of last message selected
		//CFrameWnd* pFrameWnd = pMessageBar->GetParentFrame();
		//if (pFrameWnd != NULL)
		//{
		//	pFrameWnd->m_nIDLastMessage = (UINT)wParam;
		//	pFrameWnd->m_nIDTracking = (UINT)wParam;
		//}
	}

	m_nIDLastMessage = (UINT)wParam;    // new ID (or 0)
	m_nIDTracking = (UINT)wParam;       // so F1 on toolbar buttons work
	return nIDLast;
}


BOOL CMainFrame::OnMERIconsToolTips( UINT id, NMHDR * pTTTStruct, LRESULT * pResult )
{
    TOOLTIPTEXT *pTTT = (TOOLTIPTEXT *)pTTTStruct;
    UINT nID =pTTTStruct->idFrom;

	if (nID >= ID_VIEW_EXTREP1 && nID <= ID_VIEW_EXTREP20)
	{
		int r=0;
	}

	return FALSE;
}

BOOL CMainFrame::OnToolTipText(UINT, NMHDR* pNMHDR, LRESULT* pResult)
{
	ASSERT(pNMHDR->code == TTN_NEEDTEXTA || pNMHDR->code == TTN_NEEDTEXTW);

	// allow top level routing frame to handle the message
//	if (GetRoutingFrame() != NULL)
//		return FALSE;

	// need to handle both ANSI and UNICODE versions of the message
	TOOLTIPTEXTA* pTTTA = (TOOLTIPTEXTA*)pNMHDR;
	TOOLTIPTEXTW* pTTTW = (TOOLTIPTEXTW*)pNMHDR;
	TCHAR szFullText[256];
	CString strTipText;
	UINT nID = pNMHDR->idFrom;
	if (pNMHDR->code == TTN_NEEDTEXTA && (pTTTA->uFlags & TTF_IDISHWND) ||
		pNMHDR->code == TTN_NEEDTEXTW && (pTTTW->uFlags & TTF_IDISHWND))
	{
		// idFrom is actually the HWND of the tool
		nID = ((UINT)(WORD)::GetDlgCtrlID((HWND)nID));
	}

	if (nID != 0) // will be zero on a separator
	{
		if (nID >= ID_VIEW_EXTREP1 && nID <= ID_VIEW_EXTREP20)
		{
			int nSize = m_strToolTip.GetSize();
			int nItem = nID - ID_VIEW_EXTREP1;
			if (nItem>=nSize) return FALSE;
			CString mstr = m_strToolTip.GetAt(nItem);
			strTipText = mstr;
		}
		else
		{
			AfxLoadString(nID, szFullText);
			// this is the command id, not the button index
			AfxExtractSubString(strTipText, szFullText, 1, '\n');
		}
	}
#ifndef _UNICODE
	if (pNMHDR->code == TTN_NEEDTEXTA)
		lstrcpyn(pTTTA->szText, strTipText,
			(sizeof(pTTTA->szText)/sizeof(pTTTA->szText[0])));
	else
		_mbstowcsz(pTTTW->szText, strTipText,
			(sizeof(pTTTW->szText)/sizeof(pTTTW->szText[0])));
#else
	if (pNMHDR->code == TTN_NEEDTEXTA)
		_wcstombsz(pTTTA->szText, strTipText,
			(sizeof(pTTTA->szText)/sizeof(pTTTA->szText[0])));
	else
		lstrcpyn(pTTTW->szText, strTipText,
			(sizeof(pTTTW->szText)/sizeof(pTTTW->szText[0])));
#endif
	*pResult = 0;

	// bring the tooltip window above other popup windows
	::SetWindowPos(pNMHDR->hwndFrom, HWND_TOP, 0, 0, 0, 0,
		SWP_NOACTIVATE|SWP_NOSIZE|SWP_NOMOVE);

	return TRUE;    // message was handled
}


void CMainFrame::OnSetMERIcons(CLearningUnit *pLU)
{
	CSize	sBtn(39,38),sImg(32,32);
	m_wndTools.SetSizes(sBtn,sImg);
	if (!pLU || (pLU && !pLU->m_bCanStart))
	{
		m_wndTools.SetButtons(NULL, 2);
		m_wndTools.SetButtonInfo(0, ID_VIEW_EXTREP1,TBBS_BUTTON, 2);
		m_wndTools.SetButtonInfo(1, ID_SEPARATOR,TBBS_SEPARATOR, 12);
		m_wndTools.Invalidate();
		AfxGetApp()->OnIdle(-1);
		m_strToolTip.RemoveAll();
		return;
	}

	// Set the toolbar to show only partial commmand list
	int nbER = pLU->m_cMERSet.GetSize();
	m_wndTools.SetButtons(NULL, nbER+1);

	m_wndTools.SetButtonInfo(0, ID_VIEW_EXTREP1,TBBS_BUTTON, 2);
	m_wndTools.SetButtonInfo(1, ID_SEPARATOR,TBBS_SEPARATOR, 12);

	m_strToolTip.RemoveAll();
	CExternRepUnit *pER=pLU->m_cMERSet.GetAt(0);
	if (pER)
	{
		CString mstr = pER->GetName();
		m_strToolTip.SetAtGrow(0,mstr);

	}


	for (int i=1;i<nbER;i++)
	{
		CExternRepUnit *pER=pLU->m_cMERSet.GetAt(i);
		if (!pER) continue;

		//int nnn = pER->GetID();
		int nnn = pER->GetIcon();
		m_wndTools.SetButtonInfo(i+1, ID_VIEW_EXTREP1 + i,TBBS_BUTTON, nnn - 129);

		CString mstr = pER->GetName();
		m_strToolTip.SetAtGrow(i,mstr);
	}

	CToolBarCtrl& pToolBar = m_wndTools.GetToolBarCtrl( );
	CToolTipCtrl* pToolTips = pToolBar.GetToolTips();
	if (pToolTips)
	{
		int nb = pToolTips->GetToolCount();
		for (i=0;i<nb;i++)
		{
			CToolInfo mInfo;
			BOOL bRes = pToolTips->GetToolInfo(mInfo,&m_wndTools);

		}
	}

	// invalidate the call update handlers before painting
	m_wndTools.Invalidate();
	AfxGetApp()->OnIdle(-1);
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	if (!m_wndTaskBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC,
		CRect(0,0,0,0),
		AFX_IDW_CONTROLBAR_FIRST + 20 + 1) ||
		!m_wndTaskBar.LoadToolBar(IDR_TASKBAR))
	{
		TRACE0("Failed to create taskbar\n");
		return -1;      // fail to create
	}

	if (!m_wndTools.Create(this,
			CBRS_LEFT|CBRS_TOOLTIPS|CBRS_FLYBY|WS_VISIBLE) ||
		!m_wndTools.LoadBitmap(IDB_ER_ICONS))
	{
		TRACE0("Failed to create toolbar\n");
		return FALSE;       // fail to create
	}
 
	OnSetMERIcons();
	
	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	m_wndTaskBar.EnableDocking(CBRS_ALIGN_ANY);

	DockControlBar(&m_wndToolBar);
	DockControlBar(&m_wndTaskBar);

	// NVL : Add drop arrow to a toolbar button
	DWORD dwExStyle = TBSTYLE_EX_DRAWDDARROWS;
	m_wndToolBar.GetToolBarCtrl().SetExtendedStyle(dwExStyle);
	DWORD dwStyle = m_wndToolBar.GetButtonStyle(m_wndToolBar.CommandToIndex(ID_FILE_OPEN));
	dwStyle |= TBSTYLE_DROPDOWN;
	m_wndToolBar.SetButtonStyle(m_wndToolBar.CommandToIndex(ID_FILE_OPEN), dwStyle);

	m_wndTaskBar.GetToolBarCtrl().SetExtendedStyle(dwExStyle);
	dwStyle = m_wndTaskBar.GetButtonStyle(m_wndTaskBar.CommandToIndex(ID_VIEW_ERS));
	dwStyle |= TBSTYLE_DROPDOWN;
	m_wndTaskBar.SetButtonStyle(m_wndTaskBar.CommandToIndex(ID_VIEW_ERS), dwStyle);

	
	// Reload Bars State from Registry
	CString sProfile = _T("BarState");
	if (VerifyBarState(sProfile))
	{
		CSizingControlBar::GlobalLoadState(this, sProfile);
		LoadBarState(sProfile);
	}
	CPref::ReadPref();
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers


void CMainFrame::OnViewFullscreen() 
{
	if (m_FullScreenMode)
		FullScreenModeOff();
	else 
		FullScreenModeOn();
}

void CMainFrame::OnUpdateViewFullscreen(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_FullScreenMode);			
}

void CMainFrame::FullScreenModeOn()
{
	// available only if there is an active doc
	CMDIChildWnd* pChild=MDIGetActive();
	//NVL if(!pChild) return;
	
	bool m_bToolBarWasVisible=(m_wndToolBar.IsWindowVisible()!=0);
	bool m_bStatusBarWasVisible=(m_wndStatusBar.IsWindowVisible()!=0);
	bool m_bTaskBarWasVisible=(m_wndTaskBar.IsWindowVisible()!=0);
	//NVL ShowControlBar(&m_wndToolBar, FALSE, FALSE );  
	//NVL ShowControlBar(&m_wndStatusBar, FALSE, FALSE ); 
	
	// now save the old positions of the main and child windows
	GetWindowRect(&m_mainRect);

		// remove the caption of the mainWnd:
	LONG style=::GetWindowLong(m_hWnd,GWL_STYLE);
	style&=~WS_CAPTION;
	::SetWindowLong(m_hWnd,GWL_STYLE,style);
	int screenx=GetSystemMetrics(SM_CXSCREEN); 
	int screeny=GetSystemMetrics(SM_CYSCREEN); 

	// resize:
	SetWindowPos(NULL,-4,-4,screenx+8,screeny+8,SWP_NOZORDER);
	//NVL change style only if child present
//	if (!pChild)
		m_bChildMax=false;
/*	else
	{
		style=::GetWindowLong(pChild->m_hWnd,GWL_STYLE);
		m_bChildMax=(style & WS_MAXIMIZE)?true:false;
		pChild->ShowWindow(SW_SHOWMAXIMIZED);
	}*/

	RecalcLayout();

	// create the new toolbar
	// this will contain the full-screen off button
/*	m_pwndFullScreenBar=new CToolBar;
	m_pwndFullScreenBar->CreateEx(this, TBSTYLE_FLAT, 
			WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_SIZE_DYNAMIC |
			CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY )	;
	m_pwndFullScreenBar->LoadToolBar(IDR_FULLSCREEN);
	m_pwndFullScreenBar->EnableDocking(CBRS_ALIGN_ANY);
	// place the full-screen off button somewhere:
	CPoint pt(300,200);
	FloatControlBar(m_pwndFullScreenBar,pt);*/

	m_FullScreenMode=true;
}

void CMainFrame::FullScreenModeOff()
{
	// You can use SaveBarState() in OnClose(),
	// so remove the newly added toolbar entirely
	// in order SaveBarState() not to save its state. 
	// That is why I used dynamic allocation
	//if (m_pwndFullScreenBar)
	//	delete m_pwndFullScreenBar;
	//m_pwndFullScreenBar = 0;

	LONG style=::GetWindowLong(m_hWnd,GWL_STYLE);
	style|=WS_CAPTION;
	::SetWindowLong(m_hWnd,GWL_STYLE,style);
	/*NVL no toolbar modifications
	if(m_bToolBarWasVisible)
		ShowControlBar(&m_wndToolBar, TRUE, FALSE ); 
	if(m_bStatusBarWasVisible)
		ShowControlBar(&m_wndStatusBar, TRUE, FALSE );
	*/
	MoveWindow(&m_mainRect);
	RecalcLayout();
	CMDIChildWnd* pChild=MDIGetActive();
	// pchild can be NULL if the USER closed all the
	// childs during Full Screen Mode:
	if(pChild){
		if(m_bChildMax)
			MDIMaximize(pChild);
		else MDIRestore(pChild);
	}
	m_FullScreenMode=false;
}


void CMainFrame::OnToolbarDropDown(NMTOOLBAR* pnmtb, LRESULT *plr)
{
	CWnd *pWnd;
	UINT nID = -1;

	*plr = 0;

	CMenu menu;
	CMenu* pPopup = NULL;
	
	// Switch on button command id's.
	switch (pnmtb->iItem)
	{
	case ID_FILE_OPEN:
		nID  = IDR_DROP_OPEN;
		menu.LoadMenu(nID);
		pWnd = &m_wndToolBar;
		pPopup = menu.GetSubMenu(0);
		break;
	case ID_VIEW_ERS:
		{
			CMDIChildWnd *pChldWnd = MDIGetActive();
			if (!pChldWnd) return;

			CSimulDoc *pDoc = DYNAMIC_DOWNCAST( CSimulDoc, pChldWnd->GetActiveDocument());
			if (pDoc) 
			{
				pWnd = &m_wndTaskBar;
				CRect rc;
				pWnd->SendMessage(TB_GETRECT, pnmtb->iItem, (LPARAM)&rc);
				pWnd->ClientToScreen(&rc);
				pDoc->UpdateViewMenuDrop(this,rc);
			}
		}
	default:
		return;
	}
	
	//menu.LoadMenu(nID);
	//CMenu* pPopup = menu.GetSubMenu(0);
	ASSERT(pPopup);
	
	CRect rc;
	pWnd->SendMessage(TB_GETRECT, pnmtb->iItem, (LPARAM)&rc);
	pWnd->ClientToScreen(&rc);
	
	pPopup->TrackPopupMenu( TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_VERTICAL,
		rc.left, rc.bottom, this, &rc);
}

// This function is Copyright (c) 2000, Cristi Posea.
// See www.datamekanix.com for more control bars tips&tricks.
BOOL CMainFrame::VerifyBarState(LPCTSTR lpszProfileName)
{
    CDockState state;
    state.LoadState(lpszProfileName);

    for (int i = 0; i < state.m_arrBarInfo.GetSize(); i++)
    {
        CControlBarInfo* pInfo = (CControlBarInfo*)state.m_arrBarInfo[i];
        ASSERT(pInfo != NULL);
        int nDockedCount = pInfo->m_arrBarID.GetSize();
        if (nDockedCount > 0)
        {
            // dockbar
            for (int j = 0; j < nDockedCount; j++)
            {
                UINT nID = (UINT) pInfo->m_arrBarID[j];
                if (nID == 0) continue; // row separator
                if (nID > 0xFFFF)
                    nID &= 0xFFFF; // placeholder - get the ID
                if (GetControlBar(nID) == NULL)
                    return FALSE;
            }
        }
        
        if (!pInfo->m_bFloating) // floating dockbars can be created later
            if (GetControlBar(pInfo->m_nBarID) == NULL)
                return FALSE; // invalid bar ID
    }
    return TRUE;
}

void CMainFrame::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CMDIFrameWnd::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	
}

void CMainFrame::OnUpdateMode(CCmdUI *pCmdUI)
{
    pCmdUI->Enable(); 
    CString strMode;
	strMode.LoadString(pCmdUI->m_nID+CPref::g_nMode);
    //strPage.Format( "Page %d", m_nPage ); 
    pCmdUI->SetText( strMode ); 
}


void CMainFrame::OnFilePref() 
{
	static int nb = 0;
/*	// TODO: Add your command handler code here
	CString mstr;

	mstr.Format(_T("dd\tn. %d\tCeci est un exemple"),nb);
	CTrace::WriteString(mstr);
	nb++;

	CTrace::ReadLog();*/

	CPropertySheet mdlg(_T("Trace"),this);

	CPrefCategoryPage	pInfo;
	CPrefTracePage		pConfig;
	CPrefSimulPage		pSimul;

	mdlg.AddPage(&pInfo);
	mdlg.AddPage(&pConfig);
	mdlg.AddPage(&pSimul);

	int ret = mdlg.DoModal();
	if (ret == IDOK)
	{
		CPref::g_sTraceRep = pInfo.m_strFolder;
		CPref::g_nSpeedAcc = pSimul.m_nSpeedAuth;
		CPref::g_nSpeedNorm= pSimul.m_nSpeedLearn;
		CPref::g_crPast = pSimul.m_clrPast;
		CPref::g_crFuture= pSimul.m_clrFuture;
	}
	CString mstr;

	mstr.Format(_T("dd\tn. %d\tCeci est un exemple"),nb);
	CTrace::WriteString(mstr);
	nb++;

	CTrace::ReadLog();
}

void CMainFrame::OnFileSwitchMode()
{
	CIdentityDlg dlg(AfxGetMainWnd(),CPref::g_nMode);
	if (dlg.DoModal())
	{
		CPref::g_nMode = dlg.m_nSelMode;
		CTrace::CloseLogFile();
		CMvDocTemplate *pMVTmpl = theApp.GetDocManager();
		if (pMVTmpl)
		{
			int nb = pMVTmpl->m_FrameTemplateList.GetCount();
			POSITION pos = pMVTmpl->m_FrameTemplateList.GetHeadPosition();
			while (pos)
			{
				CFrameTemplate* pFTmpl = pMVTmpl->m_FrameTemplateList.GetNext(pos);
				if (pFTmpl)
				{
					if (pFTmpl->GetResourceID() == IDR_SIMUL_CONTEXT ||
						pFTmpl->GetResourceID() == IDR_SIMULTYPE)
					{
						int r=0;
						pFTmpl->SetLoadAtStartup(!CPref::g_nMode);
					}
					else if (pFTmpl->GetResourceID() == IDR_SIMUL_WELCOME)
					{
						int r=0;
						pFTmpl->SetLoadAtStartup(CPref::g_nMode);
					}
					if (pFTmpl->GetResourceID() != IDR_SIMUL_WELCOME &&
						pFTmpl->GetResourceID() != IDR_VIEWCONTROLLER &&
						pFTmpl->GetResourceID() != IDR_SIMUL_CONTEXT &&
						pFTmpl->GetResourceID() != IDR_SIMULTYPE)
					{
						if (CPref::g_nMode)
						{
							pFTmpl->SetFrameRTC(RUNTIME_CLASS( CChildFrameFix ));
						}
						else
						{
							pFTmpl->SetFrameRTC(RUNTIME_CLASS( CChildFrame));
						}
					}
				}
			}
		}
	}
}


void CMainFrame::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
	CMDIFrameWnd::OnActivate(nState, pWndOther, bMinimized);
	// TODO: Add your message handler code here
	//SendMessage(DOC_CHANGE_DOCKABLEVIEW);
}

void CMainFrame::OnViewTaskbar() 
{
	ShowControlBar(&m_wndTaskBar, !m_wndTaskBar.IsVisible(), FALSE);
}

void CMainFrame::OnUpdateViewTaskbar(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
	pCmdUI->SetCheck(m_wndTaskBar.IsVisible());
}

void CMainFrame::OnUpdateFilePref(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(CPref::g_nMode == 0);
}

void CMainFrame::OnUpdateSwitchMode(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CMDIChildWnd *pDoc = MDIGetActive();
	BOOL bRes = (pDoc==NULL);
	pCmdUI->Enable(bRes);
}

void CMainFrame::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	CMDIChildWnd *pView = MDIGetActive();
	if (pView)
	{
		CDocument *pDoc = pView->GetActiveDocument();
		CSimulDoc *pSimulDoc = DYNAMIC_DOWNCAST( CSimulDoc, pDoc);
		if (pSimulDoc)
		{
		}

	}
	
	CMDIFrameWnd::OnTimer(nIDEvent);
}

void CMainFrame::OnViewERs() 
{
	// TODO: Add your command handler code here
	CMDIChildWnd *pChldWnd = MDIGetActive();
	if (!pChldWnd) return;

	CSimulDoc *pDoc = DYNAMIC_DOWNCAST( CSimulDoc, pChldWnd->GetActiveDocument());
	if (pDoc) 
	{
		CRect rc;
		m_wndTaskBar.SendMessage(TB_GETRECT, ID_VIEW_ERS, (LPARAM)&rc);
		m_wndTaskBar.ClientToScreen(&rc);
		pDoc->UpdateViewMenuDrop(this,rc);
	}
	
}

void CMainFrame::OnUpdateViewERs(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	BOOL bEnab = FALSE;

	CMDIChildWnd *pChldWnd = MDIGetActive();
	if (pChldWnd)
	{
		CSimulDoc *pDoc = DYNAMIC_DOWNCAST( CSimulDoc, pChldWnd->GetActiveDocument());
		if (pDoc) 
			bEnab = TRUE;
	}
	pCmdUI->Enable(bEnab);
}
