// CopyPasteBuffer.h: interface for the CCopyPasteBuffer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COPYPASTEBUFFER_H__1AD0D3C0_573A_11D4_A2FA_00D0B71C8709__INCLUDED_)
#define AFX_COPYPASTEBUFFER_H__1AD0D3C0_573A_11D4_A2FA_00D0B71C8709__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CCopyPaste : public CObject  
{
public:
	enum eCopyType {COPY_NONE=0,COPY_LUNIT,COPY_EXTREP,
					COPY_ANIMCPNT=20};
protected:
	BOOL		m_bIsOwner;
	eCopyType	m_nCopyType;
	CTypedPtrArray<CObArray,CObject*>	m_cCopySet;	// List of copied Object 
public:
	CCopyPaste();
	virtual ~CCopyPaste();

	int		GetBufferType() { return m_nCopyType;};
	void	SetBufferType(eCopyType nb) { m_nCopyType = nb;};
	BOOL	IsOwner() { return m_bIsOwner;};
	void	SetOwner(BOOL bOwner=TRUE) { m_bIsOwner = bOwner;};
	BOOL	ClearBuffer();

	int			GetSize() { return m_cCopySet.GetSize();};
	CObject*	GetAt(int i);
	int			Add(CObject *pObj);

};

#endif // !defined(AFX_COPYPASTEBUFFER_H__1AD0D3C0_573A_11D4_A2FA_00D0B71C8709__INCLUDED_)
