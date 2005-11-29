/*
Module : ICONCOMBOBOX.H
Purpose: Defines the interface for 2 MFC classes
         to implement an icon selection combo box
Created: PJN / 2-08-1998
History: None

Copyright (c) 1998 by PJ Naughter.  
All rights reserved.

*/

#ifndef __ICONCOMBOBOX_H__
#define __ICONCOMBOBOX_H__

class CSmallIconComboBox : public CComboBox
{
public:
//Methods
  int  AddIcon(UINT nIconID);
	int AddImage(UINT nIconID);
	int  InsertIcon(int nIndex, UINT nIconID);
  int  DeleteIcon(int nIndex);
  UINT GetCurSelIcon() const;
	int  SetCurSelIcon(UINT nIconID);
  int  AddString(LPCTSTR lpszString);
  int  InsertString(int nIndex, LPCTSTR lpszString);
  int  DeleteString(int nIndex);


protected:
	//{{AFX_VIRTUAL(CSmallIconComboBox)
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void PreSubclassWindow();
	virtual void DeleteItem(LPDELETEITEMSTRUCT lpDeleteItemStruct);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CSmallIconComboBox)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
	DECLARE_DYNAMIC(CSmallIconComboBox)
};


class CLargeIconComboBox : public CSmallIconComboBox
{
public:
//Methods
  int  AddIcon(UINT nIconID);
	int  InsertIcon(int nIndex, UINT nIconID);
  //other public methods for the class are derived from CSmallIconComboBox

protected:
	//{{AFX_VIRTUAL(CLargeIconComboBox)
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CLargeIconComboBox)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
	DECLARE_DYNAMIC(CLargeIconComboBox)
};


//DDX Support
void DDX_CBIcon(CDataExchange* pDX, int nIDC, UINT& nIconID);


#endif //__ICONCOMBOBOX_H__
