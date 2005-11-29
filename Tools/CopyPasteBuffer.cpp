// CopyPasteBuffer.cpp: implementation of the CCopyPasteBuffer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "simul.h"
#include "CopyPasteBuffer.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCopyPaste::CCopyPaste()
{
	m_nCopyType	= COPY_NONE;
	m_bIsOwner	= FALSE;
}

CCopyPaste::~CCopyPaste()
{
	ClearBuffer();
}


BOOL CCopyPaste::ClearBuffer()
{
	int nb = m_cCopySet.GetSize();
	if (IsOwner())
	{
		for (int i =0;i<nb;i++)	
		{
			CObject *pObj = m_cCopySet.GetAt(i);
			if (pObj) delete pObj;
		}
	}
	m_cCopySet.RemoveAll();

	m_bIsOwner	= FALSE;
	m_nCopyType	= COPY_NONE;
	return TRUE;
}

CObject* CCopyPaste::GetAt(int i)
{
	CObject* pObj = NULL;
	if (i < m_cCopySet.GetSize())
		pObj = m_cCopySet.GetAt(i);
	return pObj;
}

int	CCopyPaste::Add(CObject *pObj)
{
	int nb = m_cCopySet.GetSize();
	if (pObj)
	{
		m_cCopySet.Add(pObj);
		nb++;
	}
	return nb;
}
