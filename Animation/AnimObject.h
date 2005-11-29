//////////////////////////////////////////////////////////////////////
/// @file AnimObject.h
/// @brief Interface for the CAnimObject and derived classes.
///
/// The CAnimObject class, and its derived classes, implement the various "component" that
/// can be used in an Animation External Representation (see CViewAnimat) in order to build a 
/// more or less complex two-dimensional animation. Such components include:
///		- CAnimObject for a basic shape (rectangle, ellipse, etc.)
///		- CDensityObject for a "population density" diagram
///		- CLinkObject for a connector linking 2 components
///		- CPictDensityObject for a "caterpillar" component
///		- CPictObject for a static bitmap
///		- CPolygonObject for a polygon
///		- CTextObject for a static text
///		- CValueObject for a dynamic value
///		- CSectorObject for a sector.
///
///
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ANIMOBJECT_H__C0064979_F443_11D3_A2F4_00D0B71C8709__INCLUDED_)
#define AFX_ANIMOBJECT_H__C0064979_F443_11D3_A2F4_00D0B71C8709__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Tools\AutoFont.h"

#include "SimulDoc.h"

class CAnimObjPosPage;
class CAnimObjSizePage;
class CAnimPictPage;
class CAnimShapePage;
class CAnimDensityPage;
class CAnimLinkPage;
class CAnimTextPage;
class CAnimSectorPage;
class CAnimValuePage;
class CAnimPolygonPage;
class CAnimPictDensPage;
class CAnimInterPage;

class CSectorObject;
class CAnimObject;

typedef CTypedPtrArray<CObArray,CAnimObject*>	CxAnimObjectList;	///< List of objects (Visual)

void CopyElt(CxAnimObjectList *pList,CxAnimObjectList *pCopyList);

/////////////////////////////////////////////////////////////////////////////
// CEditValueDlg dialog

class CEditValueDlg : public CDialog
{
// Construction
public:
	CEditValueDlg(CWnd* pParent = NULL,CPoint pt=CPoint(0,0));

// Dialog Data
	//{{AFX_DATA(CEditValueDlg)
	enum { IDD = IDD_EDIT_VALUE };
	CString		m_strName;
	CString		m_strOldV;
	CString		m_strValue;
	//}}AFX_DATA
	CPoint m_pt;
	double		m_dMin,m_dMax;
	CWnd*		m_pWndParent;
protected:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditValueDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CEditValueDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////
/// Generic class representing a component used in the Animation ER.
///
/// This class defined the general behavior of all components (i.e. size, position, interaction, etc.).
/// Each inherited class corresponds to one of the various components that can be
/// used in the Animation ER:
///		- CDensityObject
///		- CLinkObject
///		- CPictDensityObject
///		- CPictObject
///		- CPolygonObject
///		- CSectorObject
///		- CTextObject
///		- CValueObject
///
///
//////////////////////////////////////////////////////////////////////
class CAnimObject : public CObject
{
	DECLARE_SERIAL(CAnimObject);
protected:
	//////////////////////////////////////////////////////////////////////
	/// Structure containing the position (default and current) of a dynamic component.
	///
	/// Basically, the current position of the component is defined by:
	///		- #m_ptPos.x = #m_ptDefPos.x + #m_nParamX * #m_nFactX
	///		- #m_ptPos.y = #m_ptDefPos.y + #m_nParamY * #m_nFactY
	//////////////////////////////////////////////////////////////////////
	struct CTObjPos {
		CPoint	m_ptDefPos;		///< Default position (in view's coordinate) of the component
		CPoint	m_ptPos;		///< Current position (in view's coordinate) of the component
		double	m_nFactX;		///< Multiplication factors for the X-axis position of the component
		double	m_nFactY;		///< Multiplication factors for the Y-axis position of the component
		int		m_nParamX;		///< Outcomes' index used to specify the X-axis position of the component
		int		m_nParamY;		///< Outcomes' index used to specify the Y-axis position of the component
	};
	//////////////////////////////////////////////////////////////////////
	/// Structure containing the size (default and current) of a dynamic component.
	///
	/// Basically, the current size of the component is defined by:
	///		- #m_szSize.x = #m_ptDefSize.x + #m_nParamX * #m_nFactX
	///		- #m_szSize.y = #m_ptDefSize.y + #m_nParamY * #m_nFactY
	//////////////////////////////////////////////////////////////////////
	struct CTObjSize {
		CSize	m_ptDefSize;	///< Default size (in view's coordinate) of the component
		CSize	m_szSize;		///< Current size (in view's coordinate) of the component
		double	m_nFactW;		///< Multiplication factors for the width of the component
		double	m_nFactH;		///< Multiplication factors for the height of the component
		int		m_nParamW;		///< Outcomes' index used to specify the width of the component
		int		m_nParamH;		///< Outcomes' index used to specify the height of the component
	};

	//////////////////////////////////////////////////////////////////////
	/// Basic shapes for all the animated components
	//////////////////////////////////////////////////////////////////////
	enum TSCptShape {
		CSHAPE_RECT,		///< A rectangle
		CSHAPE_ELLIPSE,		///< An ellipse
		CSHAPE_DIAMOND,		///< A diamond
		CSHAPE_TRIANGLEU,	///< A triangle pointing upward.
		CSHAPE_TRIANGLED	///< A triangle pointing downward.
		};

	//////////////////////////////////////////////////////////////////////
	/// Basic shapes for all the animated components
	//////////////////////////////////////////////////////////////////////
	enum TSCptIcon {
		CICON_TEXT,		///< Icon for the static text (CTextObject)
		CICON_VAL,		///< Icon for the dynamic value (CValueObject)
		CICON_SHAPE,	///< Icon for the basic shape (CAnimObject)
		CICON_PICT,		///< Icon for the static bitmap (CPictObject)
		CICON_POPD,		///< Icon for the population density (CDensityObject)
		CICON_CATER,	///< Icon for the caterpillar (CPictDensityObject)
		CICON_LINK,		///< Icon for the connector (CLinkObject)
		CICON_POLY,		///< Icon for the polygon (CPolygonObject)
		CICON_SECT		///< Icon for the sector (CSectorObject)
		};

	//enum TrackerState { normal, selected, active };

public:
	CAnimObject();
	CAnimObject(CAnimObject* pObj);
	CAnimObject(int ID,const CRect& position);
	virtual ~CAnimObject();
	virtual void Serialize(CArchive& ar);
	virtual CAnimObject* Clone();

	void		 SetObjectDoc(CSimulDoc *pDoc) { m_pDoc = pDoc;};
	CSimulDoc*	 GetObjectDoc() { return m_pDoc;};
	virtual CWnd* GetObjectView() { return NULL;};
	virtual void  SetObjectView(CWnd *pWnd) {};

	virtual CString SetObjectType();
	virtual void SetObjectDef(CString mstr);
	CString		 GetObjectDef();
	virtual	int	 GetObjectIcon();

	virtual int  GetRelatedIdx() { return m_nRelatedID;};
	virtual CAnimObject* GetRelatedObj() { return m_pRelatedObj;};
	virtual void SetRelatedObj(int nIdx=-1,CxAnimObjectList *myList = NULL);
	virtual void UpdateID(int &nID);

	virtual void SetExpSet(int nExpSet);
	virtual void SetCurrTime(int nCurrTime);


    virtual void Draw (CDC* pDC,BOOL bParent=TRUE);
	// Tracker managment
    //virtual void DrawTracker(CDC* pDC, TrackerState state);
	//virtual int	   GetHandleCount();
	//virtual CPoint GetHandle(int nHandle);
	//CRect GetHandleRect(int nHandleID, CView* pView);
	//virtual HCURSOR GetHandleCursor(int nHandle);

	virtual int	 OnEditProperty(CView *pView,int nShowSheet = 0);
	virtual void SetPropSheetPages(CPropertySheet *pSheet,CxAnimObjectList *pList=NULL);
	void		 SetPropSheetShape(CPropertySheet *pSheet,CxAnimObjectList *pList=NULL);
	void		 SetPropSheetPos(CPropertySheet *pSheet,CxAnimObjectList *pList=NULL);
	void		 SetPropSheetSize(CPropertySheet *pSheet,CxAnimObjectList *pList=NULL);
	virtual void ModifyObjectProperty();
	virtual void DisposeProperty();

	virtual void OnInitialUpdate();
	virtual void OnClose();
	virtual void OnUpdate();
	virtual void OnUpdateHypothesis(CUserData *pData);
	virtual void OnUpdateTimer();
	virtual void OnUpdatePosition();
	virtual void OnUpdateSize();
	virtual void OnUpdateSelection(int nData);

	virtual	BOOL OnDoTranslation(CPoint pt,int& nOutcome);
	virtual	BOOL OnDoEdit(CPoint pt,int& nOutcome,double& nval,double& oval,CWnd* pParent=NULL);

	virtual CAnimObject*	OnHitTest(CPoint pt,CxAnimObjectList *pList=NULL);
	virtual CAnimObject*	OnHitTest(CRect rct,CxAnimObjectList *pList=NULL);

    virtual	CRect GetRect ();
    virtual void SetRect (CRect lpSrcRect);
    virtual void MoveRect (CRect lpSrcRect);
    virtual void ResizeRect (CSize lpSize);

	void SetObjectColor(COLORREF clrLin=CLR_DEFAULT,int nSize=1,
						COLORREF clrBru=CLR_DEFAULT);

public:
	int			m_currExpSet;			///< Current Experimental Set
	int			m_currTime;				///< Current Time

	BOOL		m_bIsSelected;			///< TRUE if this component is selected in the ER (CViewAnimat), FALSE otherwise.
	BOOL		m_bResize ;				///< TRUE if this component can be resized by user, FALSE otherwise.

	CPoint		m_ObjPosition;			///< Position (X,Y) of the center of the component in the View
	CTObjPos	m_nDDPos;				///< Structure holding information about the position of the component
	CTObjSize	m_nDDsize;				///< Structure holding information about the size of the component

	int			m_nShape;				///< Basic shape of the component (see #TSCptShape).
	int			m_ObjectID;				///< ID of the object
	int			m_nDepth;				///< ????
	int			m_nRelatedID;			///< If this component is linked to another one, contains the ID of the target
										///< component, -1 otherwise

	CAnimObject*		m_pRelatedObj;	///< Pointer to the linked component if relevant, NULL otherwise (see #m_nRelatedID)
	CSectorObject*		m_pSector;		///< Pointer to a sector component if this component is built inside, NULL otherwise.
	CxAnimObjectList	m_cConnectors;	///< List of all connectors related to this component (see CLinkObject)

protected:
	CString		m_ObjectDef;			///< Name of this component

	BOOL		m_bShowTrace;			///< When animated, this component will display its previous position (as a trace)
	BOOL		m_bShowAxes;			///< When animated, this component will display the range of its displacement (by a local system of axes)

	CRect		m_ObjRect;				///< Bounding rect of this component
	CSize		m_ObjSize;				///< Dimension of this component
    CPen		m_pPen;					///< Pen used to draw the border of the component (for usage in the DC)
	CBrush		m_pBrush;				///< Brush used to fill the content of the component (for usage in the DC)
	LOGPEN		m_pLogPen;				///< Pen used to draw the border of the component (for storage)
	LOGBRUSH	m_pLogBrush;			///< Pen used to draw the border of the component (for storage)
	CPoint		m_ptXLim,				///< Minimum and maximum position of the component on the X-axis
				m_ptYLim;				///< Minimum and maximum position of the component on the Y-axis

	CArray<CPoint,CPoint>	m_cTracePt;	///< List of previous position of the component (if animated)

	CAnimObjPosPage*	m_pPosPage;		///< Pointer to the configuration interface (component's position)
	CAnimObjSizePage*	m_pSizePage;	///< Pointer to the configuration interface (component's dimension)
	CAnimShapePage*		m_pShapPage;	///< Pointer to the configuration interface (component's basic shape)
	CAnimInterPage*		m_pIntPage;		///< Pointer to the configuration interface (component's interaction)
	CSimulDoc*			m_pDoc;			///< Pointer to the document associated with this animation component

//	int					m_PosParam;
//	int					m_SizParam;
//	double				m_xFact,m_yFact;
//	CxAnimObjectList	*m_cObjSet;
//	CView*				m_pView;		
};


//////////////////////////////////////////////////////////////////////
/// Class implementing a static picture component.
///
/// 
//////////////////////////////////////////////////////////////////////
class CPictObject : public CAnimObject
{
	DECLARE_SERIAL(CPictObject);
protected:
    CBitmap			m_bitmap;		///< Bitmap used to draw this component
    CBitmap			m_maskbitmap;	///< Bitmap used to draw the mask of this component (for transparency)
    CPalette		m_palette;		///< Color palette used to draw this component
    int				m_iBmpWidth;	///< Width of the bitmap associated with this component
    int				m_iBmpHeight;	///< Height of the bitmap associated with this component
	CAnimPictPage	*m_pPictPage;	///< Pointer to the configuration interface (component's picture)
	CString			m_sBMPFile;		///< Path of the picture associated with this component
	COLORREF		m_clrTransp;	///< Color used as transparent background
	BOOL			m_bTransp;		///< TRUE if the component uses a transparent picture

public:
    UINT			m_nBitmap;		///< NOT USED: Resource ID for the bitmap
	double			m_scaleXFactor;	///< Scale factor used to resize the picture (along the X-axis)
	double			m_scaleYFactor;	///< Scale factor used to resize the picture (along the Y-axis)

public:
	CPictObject ();
	CPictObject (CPictObject *pObj);
    CPictObject (int ID,const CRect& position,UINT nBitmap);
	virtual ~CPictObject();
	virtual CAnimObject* Clone();

	virtual	int	 GetObjectIcon();

	virtual CString SetObjectType();
	//virtual void SetObjectDef(CString mstr);

	virtual void Serialize(CArchive& ar);
	virtual void OnUpdate();
	virtual void OnInitialUpdate();
	virtual void OnClose();

	virtual void OnUpdateSelection(int nData);
	virtual	BOOL OnDoTranslation(CPoint pt,int& nOutcome);

	virtual void SetPropSheetPages(CPropertySheet *pSheet,CxAnimObjectList *pList=NULL);
	virtual void ModifyObjectProperty();
	virtual void DisposeProperty();
	bool LoadBMPImage(CString sBMPFile);
    virtual void Draw (CDC* pDC,BOOL bParent=TRUE);
	void PrepareMask( CBitmap* pBmpSource,CBitmap* pBmpMask,COLORREF clrpTransColor,
					  int iTransPixelX = -1,int iTransPixelY = -1 );	
	virtual void DrawTransparent(CDC *pDC);
	virtual void DrawNormal(CDC *pDC);
};

//////////////////////////////////////////////////////////////////////
/// Contains the information required for each serie in a Population Density component.
///
///
//////////////////////////////////////////////////////////////////////
class CDensityData : public CObject
{
public:
	DECLARE_SERIAL(CDensityData);
	CDensityData();
	virtual ~CDensityData();

	void ClearData();

	virtual void Serialize(CArchive& ar);
public:
	int						m_nOutcome;		///< Index to the ouctomes uz
	double					m_nPrec;		///< Decimal 
	CArray<CPoint, CPoint&> m_ptArray;		///< List of the positions where the bitmap will be displayed to represent a spatial distribution.
	CPictObject*			m_oPict;		///< Pointer to the bitmap used to represent the information.
};


//////////////////////////////////////////////////////////////////////
/// Class implementing a "population density" component
///
///
//////////////////////////////////////////////////////////////////////
class CDensityObject : public CAnimObject
{
	DECLARE_SERIAL(CDensityObject);
public:
    CDensityObject ();
    CDensityObject (CDensityObject *pObj);
    CDensityObject (int ID,const CRect& position);
	virtual ~CDensityObject();
	virtual CAnimObject* Clone();

	virtual	int	 GetObjectIcon();

	virtual void Serialize(CArchive& ar);

	virtual	BOOL OnDoTranslation(CPoint pt,int& nOutcome);
	virtual void OnUpdateHypothesis(CUserData *pData);

	virtual void SetPropSheetPages(CPropertySheet *pSheet,CxAnimObjectList *pList=NULL);
	virtual void ModifyObjectProperty();
	virtual void DisposeProperty();
    virtual void Draw (CDC* pDC,BOOL bParent=TRUE);
	virtual void OnUpdate();
	virtual void OnUpdateSelection(int nData);

	virtual CString SetObjectType();
	//virtual void SetObjectDef(CString mstr);
    virtual void SetRect (CRect lpSrcRect);

	CObList		m_lData;			///< List of individual population (CDensityData) to be displayed in the components
protected:
	CAnimDensityPage	*m_pDensityPage;	///< Pointer to the configuration interface (component's population density)
	BOOL				m_bShowLegend;		///< ?????
	int					m_bShowValues;		///< ?????
//    CObList		m_lPicts;
//	CArray<CPoint, CPoint&> m_myArray1;
//	CArray<CPoint, CPoint&> m_myArray2;
};

//////////////////////////////////////////////////////////////////////
/// Class implementing a static text component
///
/// 
//////////////////////////////////////////////////////////////////////
class CTextObject : public CAnimObject
{
	DECLARE_SERIAL(CTextObject);
protected:
    CString		m_sText;		///< ???????
	LOGFONT		m_logFont;		///< ???????
	COLORREF	m_clrText;		///< ???????
	int			m_nAlign;		///< ??????? (0)Justify (1)Left (2)Center (3)Right

public:
	CTextObject ();
	CTextObject (CTextObject *pObj);
    CTextObject (int ID,const CRect& position,CString mtext);
	virtual ~CTextObject();
	virtual CAnimObject* Clone();

	virtual	int	 GetObjectIcon();

	virtual void Serialize(CArchive& ar);

	virtual int TextLen(CDC *pDC);
	virtual int MaxHeight(CDC *pDC);
    virtual void Draw (CDC* pDC,BOOL bParent=TRUE);

	virtual CString SetObjectType();
	//virtual void SetObjectDef(CString mstr);

	virtual void SetPropSheetPages(CPropertySheet *pSheet,CxAnimObjectList *pList=NULL);
	virtual void ModifyObjectProperty();
	virtual void DisposeProperty();

protected:
	CPropertyPage	*m_pTextPage;
};

//////////////////////////////////////////////////////////////////////
/// Class implementing a dynamic value component
///
/// 
//////////////////////////////////////////////////////////////////////
class CValueObject : public CTextObject
{
	DECLARE_SERIAL(CValueObject);
public:
	int			m_iValueName;		///< ???????
	int			m_iPrecision;		///< ???????
	double		m_dValue;			///< ???????
	BOOL		m_bDisplayName;		///< ???????
	BOOL		m_bDisplayUnit;		///< ???????
	BOOL		m_bPrevTime;		///< ???????
public:
	CValueObject ();
	CValueObject (CValueObject *pObj);
    CValueObject (int ID,const CRect& position,int vName);
	virtual ~CValueObject();
	virtual CAnimObject* Clone();

	virtual	int	 GetObjectIcon();

	virtual void Serialize(CArchive& ar);

	virtual CString SetObjectType();
	//virtual void SetObjectDef(CString mstr);
	//virtual void SetPropSheetPages(CPropertySheet *pSheet,CxAnimObjectList *pList=NULL);
	//virtual void ModifyObjectProperty();
	virtual void OnUpdate();
	virtual void OnUpdateHypothesis(CUserData *pData);
	virtual void SetPropSheetPages(CPropertySheet *pSheet,CxAnimObjectList *pList=NULL);
	virtual void ModifyObjectProperty();

	virtual void OnUpdateSelection(int nData);
	virtual	BOOL OnDoTranslation(CPoint pt,int& nOutcome);
	virtual	BOOL OnDoEdit(CPoint pt,int& nOutcome,double& nval,double& oval,CWnd* pParent=NULL);
};

//////////////////////////////////////////////////////////////////////
/// Class implementing a connector between two components.
///
/// 
//////////////////////////////////////////////////////////////////////
class CLinkObject : public CAnimObject
{
	DECLARE_SERIAL(CLinkObject);
public:
	int				m_nIDInit;			///< ???????
	int				m_nIDEnd;			///< ???????
	BOOL			m_bArrInit;			///< ???????
	BOOL			m_bArrEnd;			///< ???????
	CAnimObject*	m_pInit;			///< ???????
	CAnimObject*	m_pEnd;				///< ???????
	int				m_nArrowStyle;		///< ???????
protected:
	CAnimLinkPage*	m_pLinkPage;		///< ???????
	CPoint			m_ptInit;			///< ???????
	CPoint			m_ptEnd;			///< ???????
	int				m_bAttInit;			///< ???????
	int				m_bAttEnd;			///< ???????
public:
	CLinkObject ();
	CLinkObject (CLinkObject *pObj);
    CLinkObject (int ID);
	virtual ~CLinkObject();
	virtual CAnimObject* Clone();

	virtual	int	 GetObjectIcon();

	virtual void Serialize(CArchive& ar);
    virtual void Draw (CDC* pDC,BOOL bParent=TRUE);

	virtual void OnInitialUpdate();
	virtual void OnClose();

	CSectorObject	*GetTopSector(CAnimObject *pObj);
	BOOL		 LinkToObjects(CAnimObject *pObjSrc,CAnimObject *pObjDest);
	BOOL		 UnlinkScr();
	BOOL		 UnlinkDest();
	BOOL		 UnlinkObject(CAnimObject *pObj);
//	BOOL		 UnlinkFromObjects(CAnimObject *pObjSrc,CAnimObject *pObjDest);

	virtual int	 OnEditProperty(CView *pView,int nShowSheet = 0);
	virtual void DisposeProperty();
	virtual CString SetObjectType();
	//virtual void SetObjectDef(CString mstr);
	virtual void OnUpdate();
	virtual void SetPropSheetPages(CPropertySheet *pSheet,CxAnimObjectList *pList=NULL);
	virtual void ModifyObjectProperty();
};

//////////////////////////////////////////////////////////////////////
/// Class implementing a sector component.
///
/// 
//////////////////////////////////////////////////////////////////////
class CSectorObject : public CAnimObject
{
	DECLARE_SERIAL(CSectorObject);
public:
	BOOL			m_bIsOpen;			///< ???????
	BOOL			m_bIsLocked;		///< ???????
	BOOL			m_bIsValue;			///< ???????
	int				m_nValueName;		///< ???????
	int				m_nPrecision;		///< ???????
	BOOL			m_bDisplayUnit;		///< ???????
	BOOL			m_bPrevTime;		///< ???????
protected:
	CAnimSectorPage	*m_pSectorPage;		///< ???????
	CString			m_strLabel;			///< ???????
	LOGFONT			m_logFont;			///< ???????
	COLORREF		m_clrText;			///< ???????
	CRect			m_rReduced;			///< ???????
	CRect			m_rOBtn,			///< ???????
					m_rCBtn;			///< ???????
	//CAnimLinkPage*	m_pLinkPage;

//	CTypedPtrArray<CObArray,CAnimObject*>	m_cSectorObjSet;			// List of objects in sector
public:
	CxAnimObjectList m_cSectorObjSet;	///< ???????

	CSectorObject ();
	CSectorObject (CSectorObject *pObj);
    CSectorObject (int ID);
	virtual ~CSectorObject();
	virtual CAnimObject* Clone();

	virtual	int	 GetObjectIcon();

	virtual void Serialize(CArchive& ar);
    virtual void Draw (CDC* pDC,BOOL bParent=TRUE);

	virtual void SetExpSet(int nExpSet);
	virtual void SetCurrTime(int nCurrTime);
	virtual void UpdateID(int &nID);

	BOOL	IsCloseBtnHit(CPoint pt);
	BOOL	IsOPenBtnHit(CPoint pt);
	void	ToggleState();

    virtual CRect GetRect ();
    virtual void SetRect (CRect lpSrcRect);
    virtual void MoveRect (CRect lpSrcRect);
    virtual void ResizeRect (CSize lpSize);

	BOOL	AddObjectInSector(CAnimObject *pObj);
	BOOL	RemoveObjectFromSector(CAnimObject *pObj);
	CAnimObject *GetFromSectorAt(int i);
	int		GetSectorSize() { return m_cSectorObjSet.GetSize();};

	virtual CString SetObjectType();
	//virtual void SetObjectDef(CString mstr);
	virtual void OnUpdate();
	virtual void OnUpdateSelection(int nData);

	virtual CAnimObject*	OnHitTest(CPoint pt,CxAnimObjectList *pList=NULL);
	virtual CAnimObject*	OnHitTest(CRect rct,CxAnimObjectList *pList=NULL);

	virtual void SetPropSheetPages(CPropertySheet *pSheet,CxAnimObjectList *pList=NULL);
	virtual int	 OnEditProperty(CView *pView,int nShowSheet = 0);
	virtual void ModifyObjectProperty();
	virtual void DisposeProperty();
};

//////////////////////////////////////////////////////////////////////
/// Class implementing a "Caterpillar" component
///
/// 
//////////////////////////////////////////////////////////////////////
class CPictDensityObject : public CPictObject
{
	DECLARE_SERIAL(CPictDensityObject);
public:
	int				m_dX,				///< ???????
					m_dY;				///< ???????
	int				m_nDelta;			///< ???????
	int				m_iValueName;		///< ???????
	int				m_nBitmapNb;		///< ???????
	BOOL			m_bLogScale;		///< ???????
	BOOL			m_bFirst;			///< ???????
protected:
	CAnimPictDensPage*	m_pPictDensPage;
public:

	CPictDensityObject ();
	CPictDensityObject (CPictDensityObject *pObj);
    CPictDensityObject (int ID,const CRect& position,UINT nBitmap);
	virtual ~CPictDensityObject();

	virtual	int	 GetObjectIcon();
	
	virtual CAnimObject* Clone();

	virtual CString SetObjectType();
	//virtual void SetObjectDef(CString mstr);

	virtual void Serialize(CArchive& ar);
	virtual void OnUpdate();

	virtual void SetPropSheetPages(CPropertySheet *pSheet,CxAnimObjectList *pList=NULL);
	virtual void ModifyObjectProperty();
	virtual void DisposeProperty();
    virtual void Draw (CDC* pDC,BOOL bParent=TRUE);
};

//////////////////////////////////////////////////////////////////////
// NOT YET IMPLEMENTED - Class implementing a vector component.
//////////////////////////////////////////////////////////////////////
class CVectorObject : public CAnimObject
{
	DECLARE_SERIAL(CVectorObject);
public:
	int				m_nIDInit;		///< ???????
	CAnimObject*	m_pInit;		///< ???????
	int				m_nArrowStyle;	///< ???????
protected:
	CAnimLinkPage*	m_pLinkPage;	///< ???????
	CPoint			m_ptInit;		///< ???????
	CPoint			m_ptEnd;		///< ???????
public:
	CVectorObject ();
	CVectorObject (CVectorObject *pObj);
    CVectorObject (int ID);

	virtual	int	 GetObjectIcon();

	virtual ~CVectorObject();
	virtual CAnimObject* Clone();

	virtual void Serialize(CArchive& ar);
    virtual void Draw (CDC* pDC,BOOL bParent=TRUE);

	//virtual int	 OnEditProperty(CView *pView,int nShowSheet = 0);
	virtual void DisposeProperty();
	virtual CString SetObjectType();
	//virtual void SetObjectDef(CString mstr);
	virtual void OnUpdate();
	virtual void SetPropSheetPages(CPropertySheet *pSheet,CxAnimObjectList *pList=NULL);
	virtual void ModifyObjectProperty();
};


//////////////////////////////////////////////////////////////////////
/// Class implementing a polygon component.
///
/// 
//////////////////////////////////////////////////////////////////////
class CPolygonObject : public CAnimObject
{
	DECLARE_SERIAL(CPolygonObject);
public:
	CArray<CPoint,CPoint>	m_cPolyPt;					///< ???????
	CArray<CPoint,CPoint>	m_cPolyLine;				///< ???????
	CArray<CAnimObject*,CAnimObject*>	m_cPolyObj;		///< ???????

protected:
	CAnimPolygonPage*	m_pPolyPage;					///< ???????

public:
	CPolygonObject ();
	CPolygonObject (CPolygonObject *pObj);
    CPolygonObject (int ID);
	virtual ~CPolygonObject();
	virtual CAnimObject* Clone();

	virtual	int	 GetObjectIcon();

	virtual void Serialize(CArchive& ar);
    virtual void Draw (CDC* pDC,BOOL bParent=TRUE);

	virtual void OnInitialUpdate();
	virtual void OnClose();

	BOOL		 LinkToObjects(CxAnimObjectList *pObjList);

	virtual int	 OnEditProperty(CView *pView,int nShowSheet = 0);
	virtual void DisposeProperty();
	virtual CString SetObjectType();
	virtual void OnUpdate();
	virtual void SetPropSheetPages(CPropertySheet *pSheet,CxAnimObjectList *pList=NULL);
	virtual void ModifyObjectProperty();
};

#endif // !defined(AFX_ANIMOBJECT_H__C0064979_F443_11D3_A2F4_00D0B71C8709__INCLUDED_)
