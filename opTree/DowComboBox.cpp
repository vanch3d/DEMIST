#include "stdafx.h"
#include "TreeOptionsCtrl.h"
#include "DowComboBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CDowComboBox, CTreeOptionsCombo)

CDowComboBox::CDowComboBox()
{
}

CDowComboBox::~CDowComboBox()
{
}

BEGIN_MESSAGE_MAP(CDowComboBox, CTreeOptionsCombo)
	//{{AFX_MSG_MAP(CDowComboBox)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


int CDowComboBox::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CTreeOptionsCombo::OnCreate(lpCreateStruct) == -1)
		return -1;
	
  //Add some strings to the combo
  AddString(_T("Monday"));
  AddString(_T("Tuesday"));
  AddString(_T("Wednesday"));
  AddString(_T("Thursday"));
  AddString(_T("Friday"));
  AddString(_T("Saturday"));
  AddString(_T("Sunday"));
	
	return 0;
}
