// Simul.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Simul.h"

#include "Tools\MVDocTemplate.h"

#include "MainFrm.h"
#include "ChildFrm.h"

#include "SimulDoc.h"

#include <Prefs\Pref.h>
#include "IdentityDlg.h"



#include <MSimulation\SimulView.h>
#include <MInstruction\InstructView.h>

#include <MInteraction\SimulParam.h>
#include <MInteraction\SimulTimer.h>
#include <MInteraction\SimulTable.h>
#include <MInteraction\SimulText.h>
#include <MInteraction\SimulPie.h>
#include <MInteraction\SimulXYGraph.h>
#include <MInteraction\SimulGraph.h>
#include <MInteraction\SimulAnimat.h>
#include <MInteraction\ViewController.h>
#include "ViewWelcome.h"
//#include "ViewHTMLWelcome.h"
//#include "ViewBuildModel.h"

//#include <QHTM.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSimulApp

BEGIN_MESSAGE_MAP(CSimulApp, CWinApp)
	//{{AFX_MSG_MAP(CSimulApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_UPDATE_COMMAND_UI(ID_FILE_NEW, OnUpdateFileNew)
	ON_UPDATE_COMMAND_UI(ID_FILE_OPEN, OnUpdateFileNew)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, OnUpdateFileNew)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_AS, OnUpdateFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
	ON_UPDATE_COMMAND_UI(ID_VIEW_DESIGN_SIMULATION-1, OnUpdateViewDesign)
	ON_UPDATE_COMMAND_UI(ID_VIEW_DESIGN_INSTRUCTION, OnUpdateViewDesign)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSimulApp construction

CSimulApp::CSimulApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	m_pDocTmpl = NULL;
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CSimulApp object

CSimulApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CSimulApp initialization

BOOL CSimulApp::InitInstance()
{

	AfxEnableControlContainer();
	AfxInitRichEdit();
/*	if (!AfxOleInit())
	{
		AfxMessageBox(_T("Unable to initialize OLE"));
		return FALSE;
	}*/

//	VERIFY( QHTM_Initialize( AfxGetInstanceHandle() ) );
//	VERIFY( QHTM_EnableCooltips() );


	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("CREDIT"));

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.
/*	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(
		IDR_SIMULTYPE,
		RUNTIME_CLASS(CSimulDoc),
		RUNTIME_CLASS(CChildFrameSplit), // custom MDI child frame
		RUNTIME_CLASS(CViewSDI));
	AddDocTemplate(pDocTemplate);
	
	pDocTemplate = new CMultiDocTemplate(
			IDR_SIMUL_CONTEXT,
			RUNTIME_CLASS(CSimulDoc),		// document class
			RUNTIME_CLASS(CChildFrame),		// frame class
			RUNTIME_CLASS(CViewCDI));		// view class
	AddDocTemplate(pDocTemplate);

	
	pDocTemplate = new CMultiDocTemplate(
			IDR_SIMULPARAM_TMPL,
			RUNTIME_CLASS(CSimulDoc),		// document class
			RUNTIME_CLASS(CMDIChildWnd),		// frame class
			RUNTIME_CLASS(CViewParam));		// view class
		AddDocTemplate(pDocTemplate);
*/

	///////////////////// HERE

	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	m_pMainWnd = pMainFrame;

	// Enable drag/drop open
	m_pMainWnd->DragAcceptFiles();

	// Enable DDE Execute open
	EnableShellOpen();
//	RegisterShellFileTypes(TRUE);

	// The main window has been initialized, so show and update it.
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	// Modify Default Windows if Learning Mode
	if (CPref::g_nMode == 1)
	{
		//CString mstr;
		//int nb = CPref::g_nMode;
		//mstr.Format (_T("Mode : %d"),nb);
		//AfxGetMainWnd()->MessageBox(mstr,"sddsds");
		CIdentityDlg dlg(AfxGetMainWnd(),CPref::g_nMode);
		dlg.m_bForceId = FALSE;//TRUE;
		if (dlg.DoModal()==IDOK)
		{
			CPref::g_nMode = dlg.m_nSelMode;
		}
		else
			return FALSE;
/*		POSITION pos = pDocTemplate->m_FrameTemplateList.GetHeadPosition();
		CFrameTemplate *pFrame = pDocTemplate->m_FrameTemplateList.GetAt(pos);
		while (pFrame && pFrame->GetResourceID() != IDR_SIMULTEXT)
		{
			pFrame = pDocTemplate->m_FrameTemplateList.GetNext(pos);
		}
	
		if (pFrame)
		{
			BOOL bframe = pFrame->GetLoadAtStartup();
			pFrame->SetLoadAtStartup(!bframe);
		}

		pos = pDocTemplate->m_FrameTemplateList.GetHeadPosition();
		pFrame = pDocTemplate->m_FrameTemplateList.GetAt(pos);*/
	}

	BOOL bShow = CPref::g_nMode;

	CMvDocTemplate *pDocTemplate = new CMDIMVDocTemplate(
				IDR_SIMULTYPE, 
				RUNTIME_CLASS( CSimulDoc ) );

	//// Simulation Design view
	pDocTemplate->AddFrameTemplate(
		new CFrameTemplate(
			IDR_SIMULTYPE,					// menu & string ID 
			RUNTIME_CLASS( CChildFrameSplit ),	// custom MDI child frame
			RUNTIME_CLASS( CViewSDI ),			// custom MDI view
			ID_VIEW_DESIGN_SIMULATION,			// command ID
			SW_SHOWNORMAL,					// default window statut
			!bShow,							// Load at startup
			FALSE) );						// Allow multiple view

	//// Context Design view
	pDocTemplate->AddFrameTemplate(
		new CFrameTemplate(
			IDR_SIMUL_CONTEXT,					// menu & string ID 
			RUNTIME_CLASS( CChildFrame ),	// custom MDI child frame
			RUNTIME_CLASS( CViewCDI ),	// custom MDI view
			ID_VIEW_DESIGN_INSTRUCTION,					// command ID
			SW_SHOWNORMAL,					// default window statut
			!bShow,							// Load at startup
			FALSE) );						// Allow multiple view

	//// Welcome view
	pDocTemplate->AddFrameTemplate(
		new CFrameTemplate(
			IDR_SIMUL_WELCOME,					// menu & string ID 
			RUNTIME_CLASS( CChildFrame ),	// custom MDI child frame
			RUNTIME_CLASS( CViewWelcome ),	// custom MDI view
			ID_VIEW_WELCOME,					// command ID
			SW_SHOWNORMAL,					// default window statut
			bShow,							// Load at startup
			FALSE) );						// Allow multiple view

	//// Welcome view (QHTML)
/*	pDocTemplate->AddFrameTemplate(
		new CFrameTemplate(
			IDR_SIMUL_WELCOME,					// menu & string ID 
			RUNTIME_CLASS( CChildFrame ),	// custom MDI child frame
			RUNTIME_CLASS( CViewHTMLWelcome ),	// custom MDI view
			ID_VIEW_WELCOME,					// command ID
			SW_SHOWNORMAL,					// default window statut
			bShow,							// Load at startup
			FALSE) );						// Allow multiple view
*/
	//// Controller
    pDocTemplate->AddFrameTemplate(
        new CFrameTemplate( 
            IDR_VIEWCONTROLLER, 
            RUNTIME_CLASS( CChildController ), 
            RUNTIME_CLASS( CViewController), 
            ID_VIEW_CONTROLLER, 
            SW_SHOWNORMAL, 
            FALSE,
			FALSE) );

	//// Timer
    pDocTemplate->AddFrameTemplate(
        new CFrameTemplate( 
            IDR_SIMULTIMER, 
            RUNTIME_CLASS( CChildFrameFix ), 
            RUNTIME_CLASS( CViewTimer ), 
            ID_VIEW_TIMER, 
            SW_SHOWNORMAL, 
            FALSE,
			FALSE) );

/*    //// Parameters
	pDocTemplate->AddFrameTemplate(
        new CFrameTemplate( 
            IDR_SIMULPARAM_TMPL, 
            RUNTIME_CLASS( CChildFrameFix ), 
            RUNTIME_CLASS( CViewParam ), 
            ID_VIEW_PARAM, 
            SW_SHOWNORMAL, 
            FALSE,
			FALSE) );*/

	//// Charts
	pDocTemplate->AddFrameTemplate(
		new CFrameTemplate(
			IDR_SIMULPIE,
			(CPref::g_nMode) ? RUNTIME_CLASS( CChildFrameFix ) : RUNTIME_CLASS( CChildFrame ),
			RUNTIME_CLASS( CViewPie ),
			ID_VIEW_CHART,
			SW_SHOWNORMAL,
			FALSE,
			TRUE) );

	//// Table
	pDocTemplate->AddFrameTemplate(
		new CFrameTemplate(
			IDR_SIMULTABLE,
			(CPref::g_nMode) ? RUNTIME_CLASS( CChildFrameFix ) : RUNTIME_CLASS( CChildFrame ),
			RUNTIME_CLASS( CViewTable ),
			ID_VIEW_TABLE,
			SW_SHOWNORMAL,
			FALSE,
			TRUE) );

	//// Model
	pDocTemplate->AddFrameTemplate(
		new CFrameTemplate(
			IDR_SIMULTEXT,
			(CPref::g_nMode) ? RUNTIME_CLASS( CChildFrameFix ) : RUNTIME_CLASS( CChildFrame ),
			RUNTIME_CLASS( CViewText ),
			ID_VIEW_MODEL,
			SW_SHOWNORMAL,
			FALSE,
			FALSE) );

	//// XY-Graph (Time Graph)
	pDocTemplate->AddFrameTemplate(
		new CFrameTemplate(
			IDR_SIMULGRAPH,
			(CPref::g_nMode) ? RUNTIME_CLASS( CChildFrameFix ) : RUNTIME_CLASS( CChildFrame ),
			RUNTIME_CLASS( CViewGraph ),
			ID_VIEW_GRAPH,
			SW_SHOWNORMAL,
			FALSE,
			TRUE) );

	//// XY-Graph (Phase Plot)
	pDocTemplate->AddFrameTemplate(
		new CFrameTemplate(
			IDR_SIMULXYGRAPH,
			(CPref::g_nMode) ? RUNTIME_CLASS( CChildFrameFix ) : RUNTIME_CLASS( CChildFrame ),
			RUNTIME_CLASS( CViewPhasePlot ),
			ID_VIEW_XYGRAPH,
			SW_SHOWNORMAL,
			FALSE,
			TRUE) );

	//// Animation
	pDocTemplate->AddFrameTemplate(
		new CFrameTemplate(
			IDR_SIMULANIM,
			(CPref::g_nMode) ? RUNTIME_CLASS( CChildFrameFix ) : RUNTIME_CLASS( CChildFrame ),
			RUNTIME_CLASS( CViewAnimat ),
			ID_VIEW_ANIM,
			SW_SHOWNORMAL,
			FALSE,
			TRUE) );

	pDocTemplate->AddFrameTemplate(
		new CFrameTemplate(
			IDR_VIEW_MULTIPANES,					// menu & string ID 
			RUNTIME_CLASS( CChildFramePanes),	// custom MDI child frame
			RUNTIME_CLASS( CView),				// custom MDI view
			ID_INSTRUCTION_MULTIPANES,					// command ID
			SW_SHOWNORMAL,					// default window statut
			FALSE,							// Load at startup
			FALSE) );						// Allow multiple view

	
	


	AddDocTemplate(pDocTemplate);
	m_pDocTmpl = pDocTemplate;

	RegisterShellFileTypes(TRUE);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Don't open default document
	if(cmdInfo.m_nShellCommand == CCommandLineInfo::FileNew)
		cmdInfo.m_nShellCommand = CCommandLineInfo::FileNothing;
  
	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	if (CPref::g_sTraceRep == _T(""))
	{
		TCHAR szBuff[_MAX_PATH];
		VERIFY(::GetModuleFileName(AfxGetApp()->m_hInstance, szBuff, _MAX_PATH));
		CString strAppPath = szBuff;

		int nb = strAppPath.ReverseFind('\\');
		strAppPath = strAppPath.Left(nb);

		CPref::g_sTraceRep = strAppPath;
	}
	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
		// No message handlers
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CSimulApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

void CSimulApp::OnUpdateFileNew(CCmdUI* pCmdUI)
{
	BOOL bEna = TRUE;
	switch (pCmdUI->m_nID)
	{
	case ID_FILE_NEW:
	case ID_FILE_SAVE:
	case ID_FILE_SAVE_AS:
		bEna = !CPref::g_nMode;
		break;
	case ID_FILE_OPEN:
		{
			CMainFrame *pActFrm = DYNAMIC_DOWNCAST( CMainFrame, AfxGetMainWnd());
			if (pActFrm)
			{
				CMDIChildWnd *pDoc = pActFrm->MDIGetActive();
				bEna = !CPref::g_nMode || (pDoc==NULL);
			}
		}
		break;
	}
	pCmdUI->Enable(bEna);
}

void CSimulApp::OnUpdateViewDesign(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(!CPref::g_nMode);
}

/////////////////////////////////////////////////////////////////////////////
// CSimulApp message handlers

