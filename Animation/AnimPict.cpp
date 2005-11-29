// AnimObject.cpp: implementation of the CAnimObject class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <wingdi.h>
#include <math.h>
#include "Shlwapi.h"

#include "Simul.h"

//#include "MRTracker.h"
#include "SimulDoc.h"
#include "AnimObject.h"
#include <MInstruction\LearningUnit.h>
#include <MSimulation\Model.h>

#include <Prefs\Pref.h>


#include <MInteraction\SimulAnimat.h>
//#include "DlgFormatAnimation.h"


//#include "AnimObjectPage.h"
//#include "AnimObjSizePage.h"
//#include "AnimShapePage.h"
#include "AnimPictPage.h"
//#include "AnimTextPage.h"
//#include "AnimValuePage.h"
//#include "AnimDensityPage.h"
//#include "AnimLinkPage.h"
//#include "AnimSectorPage.h"
#include "AnimPictDensPage.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// CPictObject
//////////////////////////////////////////////////////////////////////

IMPLEMENT_SERIAL(CPictObject, CAnimObject, VERSIONABLE_SCHEMA | 2)

CPictObject::CPictObject (int ID,const CRect &position,UINT nBitmap): 
	CAnimObject (ID,position), m_nBitmap(nBitmap)
{
	m_pPictPage=0;
	m_bResize = false;
//	m_ObjectDef.Format(_T("Picture n.%d"),m_ObjectID);
//	SetObjectDef(_T(""));
	m_scaleXFactor = m_scaleYFactor = 1.0;
//	m_scaleFactor = 1.0;
//	m_ImageList = 0;
	m_clrTransp = RGB(255,255,255);
	m_bTransp = TRUE;
	SetObjectColor();
}

CPictObject::CPictObject (): 
	CAnimObject ()
{
	m_nBitmap = -1;
	m_pPictPage=0;
	m_bResize = false;
//	m_ObjectDef.Format(_T("Picture n.%d"),m_ObjectID);
//	SetObjectDef(_T(""));
	m_scaleXFactor = m_scaleYFactor = 1.0;
//	m_scaleFactor = 1.0;
//	m_ImageList = 0;
	m_clrTransp = RGB(255,255,255);
	m_bTransp = TRUE;
	SetObjectColor();
}

CPictObject::CPictObject(CPictObject *pObj): 
	CAnimObject (pObj)
{
	m_pPictPage=NULL;
	m_nBitmap = pObj->m_nBitmap;
	m_sBMPFile = pObj->m_sBMPFile;
	m_scaleXFactor = pObj->m_scaleXFactor;
	m_scaleYFactor =pObj->m_scaleYFactor;
	m_clrTransp = pObj->m_clrTransp;
	m_bTransp = pObj->m_bTransp;
}

CAnimObject* CPictObject::Clone()
{
	CPictObject *pObj = new CPictObject(this);
	return pObj;
}


CPictObject::~CPictObject()
{
	if (m_pPictPage) delete m_pPictPage;
//	if (m_ImageList)
//		delete 	m_ImageList;
//	m_ImageList = 0;
}

int	 CPictObject::GetObjectIcon()
{
	return CICON_PICT;
}

CString CPictObject::SetObjectType()
{
	CString mstr;
	mstr.Format(_T("Picture n.%d"),m_ObjectID);
	return mstr;
}

/*void CPictObject::SetObjectDef(CString mstr)
{
	if (mstr.GetLength() == 0)
	{
		m_ObjectDef.Format(_T("Picture n.%d"),m_ObjectID);
	}
	else
		m_ObjectDef = mstr;
}*/


void CPictObject::Serialize(CArchive& ar)
{
	CAnimObject::Serialize(ar);
	if( ar.IsStoring( ) )
	{
		ar << m_nBitmap;
		ar << m_sBMPFile;
		ar << m_clrTransp;
	}
	else
	{
		int nb = ar.GetObjectSchema();
		ar >> m_nBitmap;
		ar >> m_sBMPFile;
		ar >> m_clrTransp;
	}
}


bool CPictObject::LoadBMPImage(CString sBMPFile)
{
	CFile file;

	if (GetObjectDoc())
	{
		CString strDocPath = GetObjectDoc()->GetPathName();
		LPSTR lpStr1 = strDocPath.GetBuffer(strDocPath.GetLength());
		LPSTR lpStr2 = sBMPFile.GetBuffer(sBMPFile.GetLength());
		if (PathIsRelative(lpStr2))
		{
			char szOut[MAX_PATH] = "";

			PathRemoveFileSpec(lpStr1);
			PathCombine(szOut,lpStr1,lpStr2);
			sBMPFile = CString(szOut);;

		}
	}

	if( !file.Open(sBMPFile, CFile::modeRead) )
		return FALSE;

	BITMAPFILEHEADER bmfHeader;

	// Read file header
	if (file.Read((LPSTR)&bmfHeader, sizeof(bmfHeader)) != sizeof(bmfHeader))
		return FALSE;

	// File type should be 'BM'
	if (bmfHeader.bfType != ((WORD) ('M' << 8) | 'B'))
		return FALSE;

	// Get length of the remainder of the file and allocate memory
	DWORD nPackedDIBLen = file.GetLength() - sizeof(BITMAPFILEHEADER);
	HGLOBAL hDIB = ::GlobalAlloc(GMEM_FIXED, nPackedDIBLen);
	if (hDIB == 0)
		return FALSE;
	
	// Read the remainder of the bitmap file.
	if (file.ReadHuge((LPSTR)hDIB, nPackedDIBLen) != nPackedDIBLen )
	{
		::GlobalFree(hDIB);
		return FALSE;
	}

	BITMAPINFOHEADER &bmiHeader = *(LPBITMAPINFOHEADER)hDIB ;
	BITMAPINFO &bmInfo = *(LPBITMAPINFO)hDIB ;
	
	// If bmiHeader.biClrUsed is zero we have to infer the number
	// of colors from the number of bits used to specify it.
	int nColors = bmiHeader.biClrUsed ? bmiHeader.biClrUsed : 
						1 << bmiHeader.biBitCount;
						
	LPVOID lpDIBBits;
	if( bmInfo.bmiHeader.biBitCount > 8 )
		lpDIBBits = (LPVOID)((LPDWORD)(bmInfo.bmiColors + bmInfo.bmiHeader.biClrUsed) + 
					((bmInfo.bmiHeader.biCompression == BI_BITFIELDS) ? 3 : 0));
	else
		lpDIBBits = (LPVOID)(bmInfo.bmiColors + nColors);

	// Create the logical palette
	if( m_palette.m_hObject != NULL )
	{
		// Create the palette
		if( nColors <= 256 )
		{
			UINT nSize = sizeof(LOGPALETTE) + (sizeof(PALETTEENTRY) * nColors);
			LOGPALETTE *pLP = (LOGPALETTE *) new BYTE[nSize];
			
			pLP->palVersion = 0x300;
			pLP->palNumEntries = nColors;
			
			for( int i=0; i < nColors; i++)
			{
				pLP->palPalEntry[i].peRed = bmInfo.bmiColors[i].rgbRed;
				pLP->palPalEntry[i].peGreen = bmInfo.bmiColors[i].rgbGreen;
				pLP->palPalEntry[i].peBlue = bmInfo.bmiColors[i].rgbBlue;
				pLP->palPalEntry[i].peFlags = 0;
			}
			
			m_palette.CreatePalette( pLP );
			delete[] pLP;
		}
	}
	
	CClientDC dc(NULL);
	CPalette* pOldPalette = NULL;
	if( m_palette.m_hObject )
	{
		pOldPalette = dc.SelectPalette( &m_palette, FALSE );
		dc.RealizePalette();
	}

	HBITMAP hBmp = CreateDIBitmap( dc.m_hDC,                // handle to device context 
		&bmiHeader,     // pointer to bitmap size and format data 
		CBM_INIT,       // initialization flag 
		lpDIBBits,      // pointer to initialization data 
		&bmInfo,        // pointer to bitmap color-format data 
		DIB_RGB_COLORS); 
	
	// color-data usage 
	if (m_bitmap.m_hObject)
	{	
		HGDIOBJ oldobj =m_bitmap.Detach();
		DeleteObject(oldobj);
		m_bitmap.m_hObject = NULL;
	}
	m_bitmap.Attach( hBmp );

	if( pOldPalette )
		dc.SelectPalette( pOldPalette, FALSE );

	::GlobalFree(hDIB);
	return true;
}


void CPictObject::Draw (CDC* pDC,BOOL bParent)
{
	int res;

	if (bParent) CAnimObject::Draw (pDC,bParent);
	if (m_bitmap.m_hObject== NULL)
	{
		if (m_nBitmap <= 0)
			res = LoadBMPImage(m_sBMPFile);
		else
			res = m_bitmap.LoadBitmap (m_nBitmap);

		if (res)
		{
			BITMAP bm;
			m_bitmap.GetObject (sizeof (BITMAP), &bm);
			m_iBmpWidth = bm.bmWidth;
			m_iBmpHeight = bm.bmHeight;
			m_nDDsize.m_ptDefSize = CSize(m_iBmpWidth,m_iBmpHeight);//m_ObjRect.Size();

			m_ObjRect.bottom = m_ObjRect.top+(long)(m_iBmpHeight);
			m_ObjRect.right = m_ObjRect.left+(long)(m_iBmpWidth);

			if (m_bTransp)
				PrepareMask(&m_bitmap,&m_maskbitmap,m_clrTransp);
		}
		else
		{
			pDC->FillRect(m_ObjRect, &m_pBrush);
			return;
		}
	}


	if (/*m_nBitmap <=0 && */m_bitmap.m_hObject)
	{
		BLENDFUNCTION m_bf;
		m_bf.BlendOp = AC_SRC_OVER;
		m_bf.BlendFlags = 0;
		m_bf.SourceConstantAlpha = 155 ;
		m_bf.AlphaFormat = 0;

		CRect rc = GetRect();
		BITMAP bm;
		m_bitmap.GetBitmap( &bm );
		rc = CRect(rc.left, rc.top, 
				rc.left+(int)(rc.Width()*m_scaleXFactor), 
				rc.top+(int)(rc.Height()*m_scaleYFactor));

		if (m_bTransp)
			DrawTransparent(pDC);
		else
			DrawNormal(pDC);
	}
}

void CPictObject::PrepareMask( CBitmap* pBmpSource,
                          CBitmap* pBmpMask,
                          COLORREF clrpTransColor, // Pass null if unknown
                          int iTransPixelX,      // = 0
                          int iTransPixelY       // = 0
                        )
{
   BITMAP bm;

   // Get the dimensions of the source bitmap
   pBmpSource->GetObject(sizeof(BITMAP), &bm);

   // Create the mask bitmap
   pBmpMask->DeleteObject();
   pBmpMask->CreateBitmap( bm.bmWidth, bm.bmHeight, 1, 1, NULL);

   // We will need two DCs to work with. One to hold the Image
   // (the source), and one to hold the mask (destination).
   // When blitting onto a monochrome bitmap from a color, pixels
   // in the source color bitmap that are equal to the background
   // color are blitted as white. All the remaining pixels are
   // blitted as black.

   CDC hdcSrc, hdcDst;

   hdcSrc.CreateCompatibleDC(NULL);
   hdcDst.CreateCompatibleDC(NULL);

   // Load the bitmaps into memory DC
   CBitmap* hbmSrcT = (CBitmap*) hdcSrc.SelectObject(pBmpSource);
   CBitmap* hbmDstT = (CBitmap*) hdcDst.SelectObject(pBmpMask);

   // Dynamically get the transparent color
   COLORREF clrTrans;
  // if (clrpTransColor == NULL)
   if (iTransPixelX !=-1 && iTransPixelY != -1)
   {
      // User did not specify trans color so get it from bmp
      clrTrans = hdcSrc.GetPixel(iTransPixelX, iTransPixelY);
   }
   else
   {
      clrTrans = clrpTransColor;
   }


   // Change the background to trans color
   COLORREF clrSaveBk  = hdcSrc.SetBkColor(clrTrans);

   // This call sets up the mask bitmap.
   hdcDst.BitBlt(0,0,bm.bmWidth, bm.bmHeight, &hdcSrc,0,0,SRCCOPY);

   // Now, we need to paint onto the original image, making
   // sure that the "transparent" area is set to black. What
   // we do is AND the monochrome image onto the color Image
   // first. When blitting from mono to color, the monochrome
   // pixel is first transformed as follows:
   // if  1 (black) it is mapped to the color set by SetTextColor().
   // if  0 (white) is is mapped to the color set by SetBkColor().
   // Only then is the raster operation performed.

   COLORREF clrSaveDstText = hdcSrc.SetTextColor(RGB(255,255,255));
   hdcSrc.SetBkColor(RGB(0,0,0));

   hdcSrc.BitBlt(0,0,bm.bmWidth, bm.bmHeight, &hdcDst,0,0,SRCAND);

   // Clean up by deselecting any objects, and delete the
   // DC's.
   hdcDst.SetTextColor(clrSaveDstText);

   hdcSrc.SetBkColor(clrSaveBk);
   hdcSrc.SelectObject(hbmSrcT);
   hdcDst.SelectObject(hbmDstT);

   hdcSrc.DeleteDC();
   hdcDst.DeleteDC();
}


void CPictObject::DrawTransparent(CDC *pDC)
{

	// We are going to paint the two DDB's in sequence to the destination.
	// 1st the monochrome bitmap will be blitted using an AND operation to
	// cut a hole in the destination. The color image will then be ORed
	// with the destination, filling it into the hole, but leaving the
	// surrounding area untouched.

	CDC dcMem,hdcMem;                  // memory device contexts
	dcMem.CreateCompatibleDC(pDC);
	hdcMem.CreateCompatibleDC(NULL);

	CRect rc = GetRect();
	BITMAP bm;
	m_bitmap.GetBitmap( &bm );
//	rc = CRect(rc.left, rc.top, 
//				rc.left+(int)(rc.Width()*m_scaleXFactor), 
//				rc.top+(int)(rc.Height()*m_scaleYFactor));
	
	CBitmap* pOldBmp = (CBitmap*) dcMem.SelectObject(&m_bitmap);
	CBitmap* hbmT = hdcMem.SelectObject(&m_maskbitmap);
	
//	pDC->BitBlt( rc.left, rc.top, rc.Width(),rc.Height(), &hdcMem,
//					0, 0, SRCAND);
	pDC->StretchBlt(rc.left, rc.top, rc.Width(),rc.Height(), &hdcMem,
					0, 0, m_iBmpWidth,m_iBmpHeight,SRCAND);
	
	// Also note the use of SRCPAINT rather than SRCCOPY.
	
//	pDC->BitBlt(rc.left, rc.top, rc.Width(),rc.Height(), &dcMem,
//					0, 0,SRCPAINT);
	pDC->StretchBlt(rc.left, rc.top, rc.Width(),rc.Height(), &dcMem,
					0, 0,m_iBmpWidth,m_iBmpHeight,SRCPAINT);

	// Now, clean up.

	dcMem.SelectObject(pOldBmp);
	hdcMem.SelectObject(hbmT);
	hdcMem.DeleteDC();
}

void CPictObject::DrawNormal(CDC *pDC)
{

	// We are going to paint the two DDB's in sequence to the destination.
	// 1st the monochrome bitmap will be blitted using an AND operation to
	// cut a hole in the destination. The color image will then be ORed
	// with the destination, filling it into the hole, but leaving the
	// surrounding area untouched.

	CDC dcMem,hdcMem;                  // memory device contexts
	dcMem.CreateCompatibleDC(pDC);
//	hdcMem.CreateCompatibleDC(NULL);

	CRect rc = GetRect();
	BITMAP bm;
	m_bitmap.GetBitmap( &bm );
//	rc = CRect(rc.left, rc.top, 
//				rc.left+(int)(rc.Width()*m_scaleXFactor), 
//				rc.top+(int)(rc.Height()*m_scaleYFactor));
	
	CBitmap* pOldBmp = (CBitmap*) dcMem.SelectObject(&m_bitmap);
//	CBitmap* hbmT = hdcMem.SelectObject(&m_maskbitmap);
	
//	pDC->BitBlt( rc.left, rc.top, rc.Width(),rc.Height(), &hdcMem,
//					0, 0, SRCAND);
//	pDC->StretchBlt(rc.left, rc.top, rc.Width(),rc.Height(), &hdcMem,
//					0, 0, m_iBmpWidth,m_iBmpHeight,SRCAND);
	
	// Also note the use of SRCPAINT rather than SRCCOPY.
	
//	pDC->BitBlt(rc.left, rc.top, rc.Width(),rc.Height(), &dcMem,
//					0, 0,SRCPAINT);
	pDC->StretchBlt(rc.left, rc.top, rc.Width(),rc.Height(), &dcMem,
					0, 0,m_iBmpWidth,m_iBmpHeight,SRCCOPY);

	// Now, clean up.

	dcMem.SelectObject(pOldBmp);
//	hdcMem.SelectObject(hbmT);
//	hdcMem.DeleteDC();
}

void CPictObject::OnInitialUpdate()
{
	CAnimObject::OnInitialUpdate();
}

void CPictObject::OnClose()
{
	CAnimObject::OnClose();

	m_maskbitmap.DeleteObject();
	m_bitmap.DeleteObject();
}

void CPictObject::OnUpdate()
{
	CAnimObject::OnUpdate();

	CLearningUnit *pLU= m_pDoc->GetCurrentLU();
	if (!pLU)		return;

	//m_currTime = m_pDoc->m_currTimer;
	int nExpSet = m_currExpSet;

	if (m_nDDsize.m_nParamH >= pLU->GetDataSize())
		m_nDDsize.m_nParamH = -1;
	if (m_nDDsize.m_nParamW >= pLU->GetDataSize())
		m_nDDsize.m_nParamW = -1;
	//if (m_SizParam==-1) return;

	double dXValue = 0.0;
	double dYValue = 0.0;

/*	if (m_nDDsize.m_nParam.W == -1)
	{
		dXValue = m_currTime;
	}
	else*/ if (m_nDDsize.m_nParamW >= 0)
	{
		CMdData *pData = pLU->GetDataAt(m_nDDsize.m_nParamW);
		if (!pData) return;
		double iValue = pData->GetAt(0,nExpSet);
		dXValue = pData->GetAt(m_currTime,nExpSet);
		//m_scaleFactor = 1+ m_xFact * (dValue/*-iValue*/) ;
	}
/*	if (m_nDDsize.m_nParam.H == -1)
	{
		dYValue = m_currTime;
	}
	else */if (m_nDDsize.m_nParamH >= 0)
	{
		CMdData *pData = pLU->GetDataAt(m_nDDsize.m_nParamH);
		if (!pData) return;
		double iValue = pData->GetAt(0,nExpSet);
		dYValue = pData->GetAt(m_currTime,nExpSet);
		//m_scaleFactor = 1+ m_xFact * (dValue/*-iValue*/) ;
	}

	m_scaleXFactor = (m_nDDsize.m_ptDefSize.cx + m_nDDsize.m_nFactW * (dXValue/*-iValue*/)) / m_nDDsize.m_ptDefSize.cx;
	m_scaleYFactor = (m_nDDsize.m_ptDefSize.cy + m_nDDsize.m_nFactH * (dYValue/*-iValue*/)) / m_nDDsize.m_ptDefSize.cy;
}

void CPictObject::SetPropSheetPages(CPropertySheet *pSheet,CxAnimObjectList *pList)
{
	if (!m_pPictPage)
		m_pPictPage = new CAnimPictPage();

	CImageList pImageList;
	pImageList.Create(IDB_ANIM_CPNTPAGE,16,0,RGB(192,192,192));

	m_pPictPage->m_psp.dwFlags |= PSP_USEHICON ;
	m_pPictPage->m_psp.hIcon = pImageList.ExtractIcon(3);


	pSheet->AddPage(m_pPictPage);

	m_pPictPage->m_bTransBk = m_bTransp;
	m_pPictPage->m_sFileName = m_sBMPFile;
	m_pPictPage->m_clrTrans = m_clrTransp ;
	CAnimObject::SetPropSheetPages(pSheet,pList);
}


void CPictObject::ModifyObjectProperty()
{
	CAnimObject::ModifyObjectProperty();
	if (m_pPictPage->m_bIsFileSelected || m_bTransp != m_pPictPage->m_bTransBk ||
		m_clrTransp != m_pPictPage->m_clrTrans)
	{
		m_sBMPFile = m_pPictPage->m_sFileName;
		m_nBitmap = 0;
		m_bitmap.DeleteObject();
		m_maskbitmap.DeleteObject();
		//m_bitmap.m_hObject= NULL;
		//m_maskbitmap.m_hObject= NULL;
/*		if (LoadBMPImage(m_sBMPFile))
		{
			BITMAP bm;
			m_bitmap.GetObject (sizeof (BITMAP), &bm);
			m_iBmpWidth = bm.bmWidth;
			m_iBmpHeight = bm.bmHeight;
			m_ObjRect.bottom = m_ObjRect.top+m_iBmpHeight;
			m_ObjRect.right = m_ObjRect.left+m_iBmpWidth;
			SetRect(m_ObjRect);
		}*/
	}
	m_bTransp = m_pPictPage->m_bTransBk;
	m_clrTransp = m_pPictPage->m_clrTrans;

}

void CPictObject::DisposeProperty()
{
	CAnimObject::DisposeProperty();
	if (m_pPictPage) 
	{
		delete m_pPictPage;
		m_pPictPage=NULL;
	}
}

void CPictObject::OnUpdateSelection(int nData)
{
	CAnimObject::OnUpdateSelection(nData);

//	m_bIsSelected = (nData == m_iValueName);
}

BOOL CPictObject::OnDoTranslation(CPoint pt,int& nOutcome)
{
	return CAnimObject::OnDoTranslation(pt,nOutcome);
	/*	BOOL bRedraw = FALSE;
	CxAnimObjectList pList;
	CAnimObject* pObj = OnHitTest(pt,&pList);
	if (pList.GetSize())
	{
		if (!m_bIsSelected)
		{
			m_bIsSelected = TRUE;
			nOutcome = m_iValueName;
			bRedraw = TRUE;
		}
	}
	else
	{
		if (m_bIsSelected)
		{
			m_bIsSelected = FALSE;
			bRedraw = TRUE;
		}
	}

	return bRedraw;*/
}

//////////////////////////////////////////////////////////////////////
// CPictDensityObject
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CPictDensityObject, CPictObject, VERSIONABLE_SCHEMA | 2)

CPictDensityObject::CPictDensityObject () : CPictObject()
{
	m_dX = m_dY = 0;
	m_nDelta = 1;
	m_iValueName = -1;
	m_nBitmapNb = 0;
	m_bLogScale = FALSE;
	m_pPictDensPage = NULL;
	m_bFirst = TRUE;
}

CPictDensityObject::CPictDensityObject (CPictDensityObject *pObj) : CPictObject(pObj)
{
	m_dX = pObj->m_dX;
	m_dY = pObj->m_dY;
	m_nDelta = pObj->m_nDelta;
	m_iValueName = pObj->m_iValueName;
	m_nBitmapNb = 0;
	m_bLogScale = pObj->m_bLogScale ;
	m_pPictDensPage = NULL;
	m_bFirst = TRUE;
}
 
CPictDensityObject::CPictDensityObject (int ID,const CRect& position,UINT nBitmap) :
	CPictObject (ID,position,nBitmap)
{
	m_dX = m_dY = 0;
	m_nDelta = 1;
	m_iValueName = -1;
	m_nBitmapNb = 0;
	m_bLogScale = FALSE;
	m_pPictDensPage = NULL;
	m_bFirst = TRUE;
}

CPictDensityObject::~CPictDensityObject()
{
}

int	 CPictDensityObject::GetObjectIcon()
{
	return CICON_CATER;
}


CAnimObject* CPictDensityObject::Clone()
{
	CPictDensityObject *pObj = new CPictDensityObject(this);
	return pObj;
}

CString CPictDensityObject::SetObjectType()
{
	CString mstr;
	mstr.Format(_T("Picture Density n.%d"),m_ObjectID);
	return mstr;
}

/*void CPictDensityObject::SetObjectDef(CString mstr)
{
	if (mstr.GetLength() == 0)
	{
		m_ObjectDef.Format(_T("Picture Density n.%d"),m_ObjectID);
	}
	else
		m_ObjectDef = mstr;
}*/

void CPictDensityObject::Serialize(CArchive& ar)
{
	CPictObject::Serialize(ar);
	if( ar.IsStoring( ) )
	{
		ar << m_dX;
		ar << m_dY;
		ar << m_nDelta;
		ar << m_iValueName ;
		ar << m_bLogScale;
	}
	else
	{
		ar >> m_dX;
		ar >> m_dY;
		ar >> m_nDelta;
		ar >> m_iValueName ;
		ar >> m_bLogScale;
		m_bFirst = TRUE;
	}
}

void CPictDensityObject::OnUpdate()
{
	CPictObject::OnUpdate();

	CLearningUnit *pLU= m_pDoc->GetCurrentLU();
	if (!pLU)		return;

	//m_currTime = m_pDoc->m_currTimer;
	int nExpSet = m_currExpSet;

	double dValue = 0;

/*	if (m_iValueName == -1)
	{
		dValue = m_currTime;
	}
	else*/ if (m_iValueName >= 0)
	{
		CMdData *pData = pLU->GetDataAt(m_iValueName);
		if (!pData) return;
		double iValue = pData->GetAt(0,nExpSet);
		dValue = pData->GetAt(m_currTime,nExpSet);
		//m_scaleFactor = 1+ m_xFact * (dValue/*-iValue*/) ;
	}
	//dValue = dValue / m_nDelta;
	if (m_bLogScale)
		m_nBitmapNb = (int)log10(dValue);
	else
	{
		dValue = dValue / m_nDelta;
		m_nBitmapNb = (int)dValue;
	}
	//double bbbb= log(dValue);

}

void CPictDensityObject::SetPropSheetPages(CPropertySheet *pSheet,CxAnimObjectList *pList)
{
	if (!	m_pPictDensPage)
		m_pPictDensPage = new CAnimPictDensPage();

	//m_pPictDensPage->m_bTransBk = m_bTransp;
	//m_pPictDensPage->m_sFileName = m_sBMPFile;
	//m_pPictDensPage->m_clrTrans = m_clrTransp ;
	m_pPictDensPage->m_sVarList.RemoveAll();
	CLearningUnit *pLU= m_pDoc->GetCurrentLU();
	int nb = pLU->GetDataSize();
	for (int i=0;i<nb;i++)
	{
		CMdData *pData = pLU->GetDataAt(i);
		CString mstr = pData->GetDataName(pLU->m_bShowAbbrev);
		m_pPictDensPage->m_sVarList.Add(mstr);
	}
	m_pPictDensPage->m_bLog = m_bLogScale;
	m_pPictDensPage->m_bShow = TRUE;
	m_pPictDensPage->m_nAlign = 0;
	m_pPictDensPage->m_nData = m_iValueName+1;
	m_pPictDensPage->m_nDelta = m_nDelta;
	m_pPictDensPage->m_nDX = m_dX;
	m_pPictDensPage->m_nDY = m_dY;

	CImageList pImageList;
	pImageList.Create(IDB_ANIM_CPNTPAGE,16,0,RGB(192,192,192));

	m_pPictDensPage->m_psp.dwFlags |= PSP_USEHICON ;
	m_pPictDensPage->m_psp.hIcon = pImageList.ExtractIcon(5);

	
	pSheet->AddPage(m_pPictDensPage);
	CPictObject::SetPropSheetPages(pSheet,pList);
}

void CPictDensityObject::ModifyObjectProperty()
{
	CPictObject::ModifyObjectProperty();

	m_bLogScale = m_pPictDensPage->m_bLog;
	//m_pPictDensPage->m_bShow = TRUE;
	//m_pPictDensPage->m_nAlign = 0;
	m_iValueName = m_pPictDensPage->m_nData-1;
	m_nDelta = m_pPictDensPage->m_nDelta;
	m_dX = m_pPictDensPage->m_nDX;
	m_dY = m_pPictDensPage->m_nDY;

	m_bFirst = TRUE;
}

void CPictDensityObject::DisposeProperty()
{
	CAnimObject::DisposeProperty();
	if (m_pPictDensPage) 
	{
		delete m_pPictDensPage;
		m_pPictDensPage=NULL;
	}
}

void CPictDensityObject::Draw (CDC* pDC,BOOL bParent)
{
	if (m_bFirst /*&& m_bitmap.m_hObject*/)
	{
		CPictObject::Draw(pDC,bParent);
		m_bFirst = FALSE;
	}
	CRect oldR = GetRect();
	for (int i=m_nBitmapNb-1;i>=0;i--)	
	{
		CRect mrect = oldR;
		CPoint mpt(i*m_dX,i*m_dY);
		mrect.OffsetRect(mpt);
		SetRect(mrect);
		CPictObject::Draw(pDC,(i==0));
		mrect.DeflateRect(0,mrect.Height()/4,5,0);
		CString mstr;
		mstr.Format(_T("%d"),i+1);
		pDC->DrawText(mstr,mrect,DT_RIGHT);
	}
	SetRect(oldR);
	//CPictObject::Draw(pDC);
}
