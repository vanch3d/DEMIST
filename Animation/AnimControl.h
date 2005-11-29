// AnimControl.h: interface for the CAnimControl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ANIMCONTROL_H__82779320_A5C2_11D4_A2FA_00D0B71C8709__INCLUDED_)
#define AFX_ANIMCONTROL_H__82779320_A5C2_11D4_A2FA_00D0B71C8709__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AnimObject.h"

#define ANIM_DRAW_COMPONENT		(WM_USER+550)

class CComponentStatic;

//////////////////////////////////////////////////////////////////////
// Animation Component containing an interface control - Not Yet Implemented.
//////////////////////////////////////////////////////////////////////
class CAnimControl : public CAnimObject  
{
public:
	BOOL				m_bEnable;
	CComponentStatic	*m_pControl;
	CWnd				*m_pWnd;
	CAnimControl();
	virtual ~CAnimControl();

	virtual	int	 GetObjectIcon();

	virtual void Draw (CDC* pDC,BOOL bParent=TRUE);
	virtual void OnUpdate();

	virtual CWnd* GetObjectView();
	virtual void SetObjectView(CWnd *pWnd);

	virtual void ModifyObjectProperty();
};

#endif // !defined(AFX_ANIMCONTROL_H__82779320_A5C2_11D4_A2FA_00D0B71C8709__INCLUDED_)
