// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__40951555_F048_11D3_A2F4_00D0B71C8709__INCLUDED_)
#define AFX_STDAFX_H__40951555_F048_11D3_A2F4_00D0B71C8709__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#define _SCB_REPLACE_MINIFRAME
#define _SCB_MINIFRAME_CAPTION
#include "ControlBar\sizecbar.h"
#include "ControlBar\scbarg.h"
#include "ControlBar\scbarcf.h"
#define baseCMyBar CSizingControlBarCF

/*
//#ifndef _CJX_EXT_CLASS
//#ifdef _AFXDLL
//#define _CJX_EXT_CLASS	AFX_CLASS_IMPORT // Always import CJLibrary files...
//#else
#define _CJX_EXT_CLASS	// not used with static builds.
//#endif
//#endif // _CJX_EXT_CLASS

#ifndef _CJXLIB_INLINE
#define _CJXLIB_INLINE inline
#endif // _CJXLIB_INLINE

#include <..\src\afximpl.h>
#define _destroyIcon(h){if(h){::DestroyIcon(h);h=NULL;}}
#define _delete(p){if(p){delete p;p=NULL;}}
*/

#include <afxtempl.h>    // MFC templates
#include <afxrich.h>

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.


#endif // !defined(AFX_STDAFX_H__40951555_F048_11D3_A2F4_00D0B71C8709__INCLUDED_)
